/***************************************************************************
                              kafkahtmlpart.cpp
                             -------------------

    copyright            : (C) 2001 - The Kafka Team
                           (C) 2003 - Nicolas Deschildre
    email                : kde-kafka@master.kde.org && nicolasdchd@ifrance.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <kapplication.h>
#include <kdebug.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <ktrader.h>
#include <klibloader.h>
#include <kparts/factory.h>
#include <qlayout.h>
#include <qmainwindow.h>

#include <dom/dom_text.h>
#include <dom/dom_exception.h>
#include <dom/dom_string.h>
#include <khtml_events.h>

#include <qfile.h>
#include <qpainter.h>
#include <qtextstream.h>
#include <qstringlist.h>
#ifdef KAFKA_DEBUG_UTILITIES
#include <qdialog.h>
#endif

#include "domtreeview.h"
#include "wkafkapart.h"
#include "nodeproperties.h"
#include "kafkahtmlview.h"
#include "kafkahtmlpart.moc"

class KafkaHTMLPartPrivate
{
public:
	KafkaHTMLPartPrivate()
	{
		cursorTimer = 0;
		drawCursor = true;
	}
	~KafkaHTMLPartPrivate()
	{
	}
	int cursorTimer;
	bool cursorOn;
	bool drawCursor;
	bool m_readOnly;
	int m_cursorOffset;
	int m_pressOffset;
	int m_releaseOffset;
	/** when going up and down, trying to be as close as possible from the
		original node X pos like a good text editor :=) */
	bool stuckCursorHorizontalPos;
	int stuckedCursorPosX;
};

KafkaHTMLPart::KafkaHTMLPart(QWidget *parent, QWidget *widgetParent, WKafkaPart *part,
	const char *name)
	: KHTMLPart(new KafkaHTMLView(this, widgetParent, name), parent, name),
	w(part)
{
	d = new KafkaHTMLPartPrivate();

	d->m_cursorOffset = 0;
	d->m_pressOffset = 0;
	d->m_releaseOffset = 0;
	d->m_readOnly = false;
	d->cursorOn = false;
	d->stuckCursorHorizontalPos = false;

	view()->setMouseTracking(true);
	view()->installEventFilter(this);

//for debug purposes, we add a DOM tree view
#ifdef KAFKA_DEBUG_UTILITIES
	domdialog = new KafkaDOMTreeDialog(view(), this);
#endif
	//IMPORTANT:without him, no document() is created in khtmlPart
	begin();
	write("<html></html>");
	end();
}

KafkaHTMLPart::~KafkaHTMLPart()
{
}

void KafkaHTMLPart::newDocument()
{
 //FIXME: Somehow we should get it from Quanta settings: qConfig.attrValueQuotation
 //-->No need for that: Quotations aren't stored in the DOM::Nodes
	QString newPageHTMLCode = "<html>\n"
		"<head>\n"
		"<title>" + i18n("New Homepage") + "</title>\n"
  		"<meta name=\"GENERATOR\" content=\"Quanta Plus\">\n"
		"</head>\n"
		"<body bgColor=\"#FFFFFF\" text=\"#000000\">\n"
		"</body>\n"
		"</html>\n";

	begin();
	write(newPageHTMLCode);
	end();
	finishedLoading();

}

bool KafkaHTMLPart::openDocument(const KURL &url)
{
	return openURL(url);
}

bool KafkaHTMLPart::saveDocument(const KURL &url)
{
	if(url.isLocalFile())
	{
		QFile file(url.path());
		if(file.open(IO_WriteOnly))
		{
			QTextStream stream(&file);

			DOM::Node node = static_cast<DOM::Node>(document());
			if(!node.isNull())
				stream << node.toHTML() << endl;
			else
			{
				KMessageBox::error(0L, i18n("Couldn't save file %1!").arg(url.path()));
				file.close();
				return false;
			}

			file.close();
			return true;
		}
	}
	else
		KMessageBox::information(0L, i18n("Remote saving is not done yet! Sorry!"));

	return false;
}

bool KafkaHTMLPart::readOnly()
{
	return d->m_readOnly;
}

void KafkaHTMLPart::setReadOnly(bool value)
{
	d->m_readOnly = value;
}

bool KafkaHTMLPart::insertNode(DOM::Node node, DOM::Node panode, bool insertBefore)
{
	DOM::Node parentNode = panode.parentNode();
	if(parentNode.isNull())
		return false;

	kdDebug(25001) << "KafkaHTMLPart::insertNode() - Node (" <<
		parentNode.nodeName().string() << ").insertNode(" <<
		node.nodeName().string() << ")" << endl;

	DOM::Node nextSibling;
	if(insertBefore)
		nextSibling = panode;
	else
		nextSibling = panode.nextSibling();

	if(nextSibling.isNull())
		panode.appendChild(node);
	else
		parentNode.insertBefore(node, nextSibling);

	panode.applyChanges();

	emit domNodeInserted(node, false);
	emit domChanged();
	return true;
}

bool KafkaHTMLPart::insertNode(DOM::Node node, bool insertBefore)
{
	return insertNode(node, m_currentNode, insertBefore);
}

bool KafkaHTMLPart::removeNode(DOM::Node node)
{
	DOM::Node temp = node.parentNode();
	emit domNodeIsAboutToBeRemoved(node, true);
	temp.removeChild(node);
	temp.applyChanges();

	emit domChanged();
	return true;
}

void KafkaHTMLPart::insertText(DOM::Node node, const QString &text, int position)
{
	int focus = w->getAttrs(node)->chCurFoc();
	int cbModified = w->getAttrs(node)->cbMod();
	if(position < 0) return;//nothing to do if something is selected
	if(focus == kNodeAttrs::no || !cbModified) return;//can't add text in this Node.
	if(focus == kNodeAttrs::textNode && node.nodeType() == DOM::Node::TEXT_NODE)
	{
		try
		{
			DOM::DOMString textNode = node.nodeValue();
			DOM::DOMString textSplitted = textNode.split(position);
			node.setNodeValue(textNode + text + textSplitted);
			d->m_cursorOffset += text.length();
			emit domNodeModified(node);
			kdDebug(25001) << "KafkaHTMLPart::insertText() - added text" << endl;
		} catch(DOM::DOMException e){kdDebug(25001)<< "KafkaHTMLPart::insertText()" <<
			"ERROR - code : " << e.code << endl;}
	}
	else if(position == 0)
	{
		try
		{
			DOM::Text textNode = document().createTextNode(text);
			DOM::Node parent = node.parentNode();
			parent.insertBefore(textNode, node);
			m_currentNode = textNode;
			d->m_cursorOffset = text.length();
			emit domNodeInserted(textNode, false);
			kdDebug(25001) << "KafkaHTMLPart::insertText() - added text - 2" << endl;
		} catch(DOM::DOMException e){kdDebug(25001)<< "KafkaHTMLPart::insertText()" <<
			"ERROR - code : " << e.code << endl;}
	}
	else if(position == 3 || (position == 1 && (focus == kNodeAttrs::singleNode ||
		focus == kNodeAttrs::singleNodeAndItself)))
	{
		try
		{
			DOM::Text textNode = document().createTextNode(text);
			DOM::Node parent = node.parentNode();
			parent.insertBefore(textNode, node.nextSibling());
			m_currentNode = textNode;
			d->m_cursorOffset = text.length();
			emit domNodeInserted(textNode, false);
			kdDebug(25001) << "KafkaHTMLPart::insertText() - added text - 3" << endl;
		} catch(DOM::DOMException e){kdDebug(25001)<< "KafkaHTMLPart::insertText()" <<
			"ERROR - code : " << e.code << endl;}
	}
	else if(position == 1)
	{
		try
		{
			DOM::Text textNode = document().createTextNode(text);
			if(!node.firstChild().isNull())
				node.insertBefore(textNode, node.firstChild());
			else
				node.appendChild(textNode);
			m_currentNode = textNode;
			d->m_cursorOffset = text.length();
			emit domNodeInserted(textNode, false);
			kdDebug(25001) << "KafkaHTMLPart::insertText() - added text - 4" << endl;
		} catch(DOM::DOMException e){kdDebug(25001)<< "KafkaHTMLPart::insertText()" <<
			"ERROR - code : " << e.code << endl;}
	}
	else if(position == 2)
	{
		try
		{
			DOM::Text textNode = document().createTextNode(text);
			node.appendChild(textNode);
			m_currentNode = textNode;
			d->m_cursorOffset = text.length();
			emit domNodeInserted(textNode, false);
			kdDebug(25001) << "KafkaHTMLPart::insertText() - added text - 5" << endl;
		} catch(DOM::DOMException e){kdDebug(25001)<< "KafkaHTMLPart::insertText()" <<
			"ERROR - code : " << e.code << endl;}
	}
	emit domNodeNewCursorPos(m_currentNode, d->m_cursorOffset);
}

void KafkaHTMLPart::insertText(const QString &text, int position)
{
	insertText(m_currentNode, text, (position == -1 ? d->m_cursorOffset : position));
}

void KafkaHTMLPart::nextOffset(int offset)
{
	int i;
	for(i = 0;i < offset;i++)
		keyRight();
}

void KafkaHTMLPart::keyRight()
{
	int focus;
	DOM::Node _node = m_currentNode;
	DOM::Node _nodeCursor;
	bool b;

	if(m_currentNode.isNull())
	{
		kdDebug(25001)<< "KafkaHTMLPart::keyRight() - Bad Node" << endl;
		return;
	}
	focus = w->getAttrs(m_currentNode)->chCurFoc();
	if(focus != kNodeAttrs::textNode && focus != kNodeAttrs::no && ((d->m_cursorOffset > 3
		&& (focus == kNodeAttrs::right || focus == kNodeAttrs::leftAndRight)) ||
		 (d->m_cursorOffset > 1 && (focus == kNodeAttrs::left ||
		 focus == kNodeAttrs::singleNode || focus == kNodeAttrs::singleNodeAndItself))))
	{
		kdError(25001)<< "KafkaHTMLPart::keyLeft() - Invalid offset" << endl;
	}

	if(d->m_cursorOffset < 0)//A tag is selected, nothing to do.
		return;

	if((focus == kNodeAttrs::textNode && ((unsigned)d->m_cursorOffset <
		(static_cast<DOM::CharacterData>(m_currentNode)).length())) ||
		(focus != kNodeAttrs::textNode && focus != kNodeAttrs::no &&
		d->m_cursorOffset == 0) || ((focus == kNodeAttrs::right ||
		focus == kNodeAttrs::leftAndRight) && d->m_cursorOffset == 2))
	{
		kdDebug(25001)<< "KafkaHTMLPart::keyRight() - Moving one pos right " << endl;
		d->m_cursorOffset++;
		postprocessCursorPosition();
		emit domNodeNewCursorPos(m_currentNode, d->m_cursorOffset);
		return;
	}

	//looking for the next node
	while(1)
	{
		_node = getNextNode(_node, b);
		if(_node.isNull()) break;
		focus = w->getAttrs(_node)->chCurFoc();
		if((focus == kNodeAttrs::textNode
			&& (static_cast<DOM::CharacterData>(_node)).length() != 0)
			|| (focus != kNodeAttrs::no && focus != kNodeAttrs::textNode))
		{
			_nodeCursor = _node;
			break;
		}
	}

	if(!_nodeCursor.isNull())
	{
		kdDebug(25001)<< "KafkaHTMLPart::keyRight() - _nodeCursor = "
			<< _nodeCursor.nodeName().string() <<  endl;
		d->m_cursorOffset = 1;
		m_currentNode = _nodeCursor;
		postprocessCursorPosition();
		emit domNodeNewCursorPos(m_currentNode, d->m_cursorOffset);
	}
	kdDebug(25001)<< "KafkaHTMLPart::keyRight() - new cursorNode = "
		<< m_currentNode.nodeName().string() << endl;
}



void KafkaHTMLPart::previousOffset(int offset)
{
	int i;
	for(i = 0;i < offset;i++)
		keyLeft();
}

void KafkaHTMLPart::keyLeft()
{
	int focus;
	DOM::Node _startNode = m_currentNode;
	DOM::Node _node = m_currentNode;
	DOM::Node _nodeCursor;
	bool b;

	if(m_currentNode.isNull())
	{
		kdDebug(25001)<< "KafkaHTMLPart::keyLeft() - Bad Node" << endl;
		return;
	}
	focus = w->getAttrs(m_currentNode)->chCurFoc();
	if(focus != kNodeAttrs::textNode && focus != kNodeAttrs::no && ((d->m_cursorOffset > 3
		&& (focus == kNodeAttrs::right || focus == kNodeAttrs::leftAndRight)) ||
		 (d->m_cursorOffset > 1 && (focus == kNodeAttrs::left ||
		 focus == kNodeAttrs::singleNode || focus == kNodeAttrs::singleNodeAndItself))))
	{
		kdError(25001)<< "KafkaHTMLPart::keyLeft() - Invalid offset" << endl;
	}

	if(d->m_cursorOffset < 0)//A tag is selected, nothing to do.
		return;

	if(((focus == kNodeAttrs::right || focus == kNodeAttrs::leftAndRight) &&
		d->m_cursorOffset == 3) || (focus == kNodeAttrs::textNode &&
		d->m_cursorOffset > 0) || ((focus == kNodeAttrs::singleNode ||
		focus == kNodeAttrs::singleNodeAndItself || focus == kNodeAttrs::left ||
		focus == kNodeAttrs::leftAndRight) && d->m_cursorOffset == 1))
	{
		kdDebug(25001)<< "KafkaHTMLPart::keyLeft() - Moving one pos left1 " << endl;
		d->m_cursorOffset--;
		postprocessCursorPosition();
		emit domNodeNewCursorPos(m_currentNode, d->m_cursorOffset);
		return;
	}

	//looking for the next node
	while(1)
		{
		_node = getPrevNode(_node, b);
		if(_node.isNull()) break;
		focus = w->getAttrs(_node)->chCurFoc();
		if((focus == kNodeAttrs::textNode
			&& (static_cast<DOM::CharacterData>(_node)).length() != 0)
			|| (focus != kNodeAttrs::no && focus != kNodeAttrs::textNode))
		{
			_nodeCursor = _node;
			break;
		}
	}

	if(!_nodeCursor.isNull())
	{
		kdDebug(25001)<< "KafkaHTMLPart::keyLeft() - _nodeCursor = "
			<< _nodeCursor.nodeName().string() << endl;
		if(focus == kNodeAttrs::textNode)
			d->m_cursorOffset =
				(static_cast<DOM::CharacterData>(_nodeCursor)).length() - 1;
		else if((focus == kNodeAttrs::leftAndRight && !b) ||
			(focus == kNodeAttrs::right && !b))
			d->m_cursorOffset = 2;
		else
			d->m_cursorOffset = 0;
		m_currentNode = _nodeCursor;
		postprocessCursorPosition();
		emit domNodeNewCursorPos(m_currentNode, d->m_cursorOffset);
	}
	kdDebug(25001)<< "KafkaHTMLPart::keyLeft() - new cursorNode = "
		<< m_currentNode.nodeName().string() << endl;

}

void KafkaHTMLPart::normalize(DOM::Node _node)
{
	kdDebug(25001)<< "KafkaHTMLPart::normalize()" << endl;
	DOM::Node childNode = _node.firstChild();
	while(childNode != 0 && w->getAttrs(childNode)->chCurFoc() == kNodeAttrs::textNode)
	{
		while(childNode.nextSibling() != 0 &&
			w->getAttrs(childNode.nextSibling())->chCurFoc() ==
			kNodeAttrs::textNode )
		{
			childNode.setNodeValue(childNode.nodeValue() +
				childNode.nextSibling().nodeValue());
			emit domNodeModified(childNode);
			emit domNodeIsAboutToBeRemoved(childNode.nextSibling(), true);
			_node.removeChild(childNode.nextSibling());
		}
		if(childNode.nextSibling() != 0)
			childNode = childNode.nextSibling();
		else
			return;
	}
}

void KafkaHTMLPart::keyUp()
{
	int tmpX, tmpY, tmpH, currentX, focus;
	int oldX, newX, oldY, newY, finalX = 10000;
	int i, tmpOffset = d->m_cursorOffset, finalOffset;
	bool b;
	DOM::Node _node = m_currentNode, _finalNode = 0;

	if(m_currentNode.isNull()) return;
	focus = w->getAttrs(m_currentNode)->chCurFoc();
	if(focus != kNodeAttrs::no && focus != kNodeAttrs::textNode &&
		d->m_cursorOffset < 0)//A DOM::Node is selected, nothing to do.
		return;

	getCursor(_node, d->m_cursorOffset, currentX, oldY, tmpH);
	oldX = currentX;
	if(d->stuckCursorHorizontalPos)
		currentX = d->stuckedCursorPosX;
	newY = oldY;
	newX = oldX - 1;
	for(i = 0; i < 2; i++)
	{
		while(newX <= oldX && newY <= (oldY + 6) && newY >= (oldY - 6))
		{
			if(tmpOffset > 0)
				tmpOffset--;
			else
			{
				_node = getPrevNode(_node, b);
				if(_node == 0)
					break;
				focus = w->getAttrs(_node)->chCurFoc();
				if(focus == kNodeAttrs::textNode)
					tmpOffset =
						(static_cast<DOM::CharacterData>(_node)).length();
				else if(focus != kNodeAttrs::no && focus !=
					kNodeAttrs::textNode)
					tmpOffset = 1;
			}
			if(getCursor(_node, tmpOffset, tmpX, tmpY, tmpH))
			{
				oldX = newX;
				newX = tmpX;
				oldY = newY;
				newY = tmpY;
				if((i == 1 && newX <= oldX && newY <= (oldY + 6) &&
					newY >= (oldY - 6)) || (i == 0 && (newX > oldX ||
					 newY > (oldY + 6) || newY < (oldY - 6))))
				{//try to find the nearest node
					if((currentX - newX)*(currentX -newX) <
						 (currentX - finalX)*(currentX - finalX))
					{//another method to get the absolute value :)
						finalX = newX;
						finalOffset = tmpOffset;
						_finalNode = _node;
					}
				}
			}
			kdDebug(25001)<< "KafkaHTMLPart::keyUp() - going through node :" <<
				_node.nodeName().string() << ":" << tmpOffset <<
				"; x:" << newX << ":y:" << newY << endl;
		}
		kdDebug(25001)<< "KafkaHTMLPart::keyUp() - i = " << i << endl;
		oldX = newX + 1;
		oldY = newY;
	}
	if(_finalNode != 0)
	{
		kdDebug(25001)<< "KafkaHTMLPart::keyUp() - final node changed" << endl;
		m_currentNode = _finalNode;
		d->m_cursorOffset = finalOffset;
		postprocessCursorPosition();
		emit domNodeNewCursorPos(m_currentNode, d->m_cursorOffset);
		if(!d->stuckCursorHorizontalPos)
		{
			d->stuckCursorHorizontalPos = true;
			d->stuckedCursorPosX = currentX;
		}
	}

	kdDebug(25001)<< "KafkaHTMLPart::keyUp() - Final Node : " <<
		m_currentNode.nodeName().string() <<
		":" << d->m_cursorOffset << endl;
}

void KafkaHTMLPart::keyDown()
{
	int tmpX, tmpY, tmpH, focus, currentX;
	int oldX, newX, oldY, newY, finalX = 10000;
	int i, tmpOffset = d->m_cursorOffset, finalOffset;
	bool b;
	DOM::Node _node = m_currentNode, _finalNode = 0;

	if(m_currentNode.isNull()) return;
	focus = w->getAttrs(m_currentNode)->chCurFoc();
	if(focus != kNodeAttrs::no && focus != kNodeAttrs::textNode &&
		d->m_cursorOffset < 0)//A DOM::Node is selected, nothing to do.
		return;

	getCursor(_node, d->m_cursorOffset, currentX, oldY, tmpH);
	oldX = currentX;
	if(d->stuckCursorHorizontalPos)
		currentX = d->stuckedCursorPosX;
	newY = oldY;
	newX = oldX + 1;
	for(i = 0; i < 2; i++)
	{
		while(newX >= oldX && newY <= (oldY + 6) && newY >= (oldY - 6))
		{
			if(_node == 0)
				break;
			focus = w->getAttrs(_node)->chCurFoc();
			if((focus == kNodeAttrs::textNode &&
				(unsigned)tmpOffset <
				(static_cast<DOM::CharacterData>(_node)).length()) ||
				(focus != kNodeAttrs::no && focus != kNodeAttrs::textNode &&
				tmpOffset == 0))
				tmpOffset++;
			else
			{
				_node = getNextNode(_node, b);
				if(_node == 0)
					break;
				focus = w->getAttrs(_node)->chCurFoc();
				if(focus != kNodeAttrs::no)
					tmpOffset = 0;
			}
			if(getCursor(_node, tmpOffset, tmpX, tmpY, tmpH))
			{
				oldX = newX;
				newX = tmpX;
				oldY = newY;
				newY = tmpY;
				if((i == 1 && newX >= oldX && newY <= (oldY + 6) &&
					newY >= (oldY - 6)) || (i == 0 && (newX < oldX ||
					 newY > (oldY + 6) || newY < (oldY - 6))))
				{//try to find the nearest node
					if((currentX - newX)*(currentX -newX) <
						 (currentX - finalX)*(currentX - finalX))
					{//another method to get the absolute value :)
						finalX = newX;
						finalOffset = tmpOffset;
						_finalNode = _node;
					}
				}
			}
			kdDebug(25001)<< "KafkaHTMLPart::keyDown() - going through node :" <<
				_node.nodeName().string() << ":" << tmpOffset <<
				"; x:" << newX << ":y:" << newY << endl;
		}
		kdDebug(25001)<< "KafkaHTMLPart::keyDown() - i = " << i << endl;
		oldX = newX - 1;
		oldY = newY;
	}
	if(_finalNode != 0)
	{
		kdDebug(25001)<< "KafkaHTMLPart::keyDown() - final node changed" << endl;
		m_currentNode = _finalNode;
		d->m_cursorOffset = finalOffset;
		postprocessCursorPosition();
		emit domNodeNewCursorPos(m_currentNode, d->m_cursorOffset);
		if(!d->stuckCursorHorizontalPos)
		{
			d->stuckCursorHorizontalPos = true;
			d->stuckedCursorPosX = currentX;
		}
	}
	kdDebug(25001)<< "KafkaHTMLPart::keyDown() - Final Node : " << m_currentNode.nodeName().string() <<
		":" << d->m_cursorOffset << endl;
}

void KafkaHTMLPart::keyReturn()
{
	//Temporary HTML-specific function
	DOM::Node brNode, secondPartOfText;
	int focus;

	if(m_currentNode.isNull()) return;
	focus = w->getAttrs(m_currentNode)->chCurFoc();
	if (focus == kNodeAttrs::textNode)
	{
		brNode = document().createElement("BR");
		if(d->m_cursorOffset == 0)
		{
			m_currentNode.parentNode().insertBefore(brNode, m_currentNode);
			emit domNodeInserted(brNode, false);
		}
		else if((unsigned)d->m_cursorOffset ==
			(static_cast<DOM::CharacterData>(m_currentNode)).length())
		{
			if(m_currentNode.parentNode().lastChild() == m_currentNode)
				m_currentNode.parentNode().appendChild(brNode);
			else
				m_currentNode.parentNode().insertBefore(brNode,
					m_currentNode.nextSibling());
			emit domNodeInserted(brNode, false);
			m_currentNode = brNode;
			d->m_cursorOffset = 1;
			postprocessCursorPosition();
			emit domNodeNewCursorPos(m_currentNode, d->m_cursorOffset);
		}
		else
		{
			secondPartOfText = (static_cast<DOM::Text>(m_currentNode)).splitText(d->m_cursorOffset);
			emit domNodeModified(m_currentNode);
			emit domNodeInserted(secondPartOfText, false);
			m_currentNode.parentNode().insertBefore(brNode, secondPartOfText);
			emit domNodeInserted(brNode, false);
			m_currentNode = secondPartOfText;
			d->m_cursorOffset = 0;
			emit domNodeNewCursorPos(m_currentNode, d->m_cursorOffset);
		}
	}
}

bool KafkaHTMLPart::eventFilter(QObject *object, QEvent *event)
{
	bool forgetEvent = false;
	//tmp
	//DOM::Node attr, tmpNode;
	//end tmp

	if(event->type() == QEvent::FocusIn)
	{
		kdDebug(25001) << "KafkaHTMLPart::eventFilter() FocusIn" << endl;
		if(!d->cursorTimer)
			{
				d->cursorTimer = startTimer(500);
				d->drawCursor = true;
				d->cursorOn = true;
			}
	}

	if(event->type() == QEvent::FocusOut)
	{
		kdDebug(25001) << "KafkaHTMLPart::eventFilter() FocusOut" << endl;
		if(d->cursorTimer)
		{
			killTimer(d->cursorTimer);
			d->cursorTimer = 0;
		}
			d->cursorOn = false;
	}

	if(event->type() == QEvent::KeyPress)
	{
		QKeyEvent *keyevent = static_cast<QKeyEvent *>(event);

		switch(keyevent->key())
		{
			case Key_Left:
				kdDebug(25001) << "KafkaHTMLPart::eventFilter() Left" << endl;
				previousOffset(1);
				d->stuckCursorHorizontalPos = false;
				forgetEvent = true;//to avoid the scrolling of the page
				break;

			case Key_Right:
				kdDebug(25001) << "KafkaHTMLPart::eventFilter() Right" << endl;
				nextOffset(1);
				d->stuckCursorHorizontalPos = false;
				forgetEvent = true;
				break;

			case Key_Backspace:
				kdDebug(25001)<< "KafkaHTMLPart::eventFilter() Backspace" << endl;
				if(!readOnly())
					keyBackspace();
				d->stuckCursorHorizontalPos = false;
				break;

			case Key_Delete:
				kdDebug(25001)<< "KafkaHTMLPart::eventFilter() Delete" << endl;
				if(!readOnly())
					keyDelete();
				d->stuckCursorHorizontalPos = false;
				break;

			case Key_Up:
				kdDebug(25001)<< "KafkaHTMLPart::eventFilter() Up" << endl;
				keyUp();
				forgetEvent = true;
				break;
			case Key_Down:
				kdDebug(25001)<< "KafkaHTMLPart::eventFilter() Down" << endl;
				keyDown();
				forgetEvent = true;
				break;
			case Key_Escape:
				break;
			case Key_Tab:
				if(!m_currentNode.isNull() &&
					w->getAttrs(m_currentNode)->chCurFoc() != kNodeAttrs::no)
				{
					insertText("    ", -1);
					makeCursorVisible();
				}
				forgetEvent = true;
				d->stuckCursorHorizontalPos = false;
				break;
			case Key_BackTab:
				break;
			case Key_Return:
				kdDebug(25001)<< "KafkaHTMLPart::eventFilter() Return" << endl;
				keyReturn();
				d->stuckCursorHorizontalPos = false;
				break;
			case Key_Enter:/**
				tmpNode = createNode("IMG");
				attr = document().createAttribute("src");
				attr.setNodeValue("/home/guest/wallpapers/matrix08-1024.jpg");
				tmpNode.attributes().setNamedItem(attr);
				m_currentNode.parentNode().appendChild(tmpNode);
				d->stuckCursorHorizontalPos = false;
				emit domNodeInserted(tmpNode, false);*/
				break;
			case Key_Insert:
				break;
			case Key_Pause:
				break;
			case Key_Print:
				break;
			case Key_SysReq:
				break;
			case Key_Home:
				d->stuckCursorHorizontalPos = false;
				break;
			case Key_End:
				d->stuckCursorHorizontalPos = false;
				break;
			case Key_Next:
				break;
			case Key_Shift:
				break;
			case Key_Control:
				break;
			case Key_Meta:
				break;
			case Key_Alt:
				break;
			case Key_CapsLock:
				break;
			case Key_NumLock:
				break;
			case Key_ScrollLock:
				break;

			default:
				kdDebug(25001) << "KafkaHTMLPart::eventFilter() Text" << endl;
				if(m_currentNode.isNull())
					break;
				else if(w->getAttrs(m_currentNode)->chCurFoc() !=
					kNodeAttrs::no)
				{
					kdDebug(25001) << "KafkaHTMLPart::eventFilter() Text - " <<
						keyevent->text() << endl;
					if(/**keyevent->key() >= Key_Space &&*/ keyevent->key() !=
						Key_unknown && !readOnly())
						insertText(keyevent->text(), -1);
					makeCursorVisible();
				}
				//if(keyevent->key() == Qt::Key_Space)
				forgetEvent = true;
				d->stuckCursorHorizontalPos = false;
				break;
		}
		if(d->cursorTimer)
		{
			killTimer(d->cursorTimer);
			d->cursorTimer = startTimer(500);
			d->drawCursor = true;
			view()->updateContents();
		}
	}

	if(!forgetEvent)
		return KHTMLPart::eventFilter(object, event);
	else
		return true;
}

void KafkaHTMLPart::keyDelete()
{
	kNodeAttrs *attrs;
	int focus;
	DOM::Node _nodeParent, _node, _nodeNext;
	bool _goingTowardsRootNode;

	if(m_currentNode.isNull()) return;
	attrs = w->getAttrs(m_currentNode);
	if(attrs->chCurFoc() == kNodeAttrs::textNode && (unsigned)d->m_cursorOffset !=
		(static_cast<DOM::CharacterData>(m_currentNode)).length())
	{//if we are in the middle of some text, we remove one letter
		if(!attrs->cbMod())
			return;
		kdDebug(25001)<< "KafkaHTMLPart::keyDelete() - one letter removed - 1" << endl;
		DOM::DOMString nodeText = m_currentNode.nodeValue();
		DOM::DOMString textSplitted = nodeText.split(d->m_cursorOffset + 1);
		nodeText.split(d->m_cursorOffset);
		m_currentNode.setNodeValue(nodeText + textSplitted);
		m_currentNode.parentNode().applyChanges();
		emit domNodeModified(m_currentNode);
		postprocessCursorPosition();
		return;
	}

	if(attrs->chCurFoc() != kNodeAttrs::no && attrs->chCurFoc() != kNodeAttrs::textNode &&
		d->m_cursorOffset < 0)
	{//if we delete ourselves, which node will be m_currentNode??
		if(!attrs->cbDel())
			return;
		kdDebug(25001)<< "KafkaHTMLPart::keyDelete() - deleting a Node - 2" <<
			endl;
		DOM::Node _node = m_currentNode;
		bool b = false;
		while(1)
		{//try to find a prev node from which we can delete the node
			_node = getPrevNode(_node, b);
			if(_node == 0) break;
			attrs = w->getAttrs(_node);
			if(attrs->chCurFoc() == kNodeAttrs::textNode)
			{
				m_currentNode = _node;
				d->m_cursorOffset =
					(static_cast<DOM::CharacterData>(_node)).length();
				keyDelete();
				emit domNodeNewCursorPos(m_currentNode, d->m_cursorOffset);
				return;
			}
			if(attrs->chCurFoc() != kNodeAttrs::no &&
				attrs->chCurFoc() != kNodeAttrs::textNode)
			{
				m_currentNode = _node;
				d->m_cursorOffset = 1;
				keyDelete();
				emit domNodeNewCursorPos(m_currentNode, d->m_cursorOffset);
				return;
			}
		}
		b = false;
		while(1)
		{//try to find a next node from which we can delete the node
			_node = getNextNode(_node, b);
			if(_node == 0) break;
			attrs = w->getAttrs(_node);
			if(attrs->chCurFoc() != kNodeAttrs::no)
			{
				m_currentNode = _node;
				d->m_cursorOffset = 0;
				keyBackspace();
				emit domNodeNewCursorPos(m_currentNode, d->m_cursorOffset);
				return;
			}
		}
		//here, there is no node right and left that can have the cursor focus
		_node = m_currentNode.parentNode();
		emit domNodeIsAboutToBeRemoved(m_currentNode, true);
		_node.removeChild(m_currentNode);
		m_currentNode = document().createTextNode("");
		_node.appendChild(m_currentNode);
		emit domNodeInserted(m_currentNode, false);
		emit domNodeNewCursorPos(m_currentNode, d->m_cursorOffset);
	}

	_node = m_currentNode;
	_nodeNext = m_currentNode;
	_goingTowardsRootNode = false;
	while(1)
	{
		_nodeNext = getNextNode(_nodeNext, _goingTowardsRootNode);

		//second part : depending of the node, we choose what to do
		if(_nodeNext == 0)
			return;
		attrs = w->getAttrs(_nodeNext);
		if(!attrs->cbDel())
			return;
		if(attrs->chCurFoc() == kNodeAttrs::textNode)
		{
			if((static_cast<DOM::CharacterData>(_nodeNext)).length() != 0)
			{//if we are in text, remove a letter
				kdDebug(25001)<< "KafkaHTMLPart::keyDelete() - one letter" <<
					" removed - 2" << endl;
				DOM::DOMString nodeText = _nodeNext.nodeValue();
				DOM::DOMString textSplitted = nodeText.split(1);
				_nodeNext.setNodeValue(textSplitted);
				emit domNodeModified(_nodeNext);
				postprocessCursorPosition();
				return;
			}
			else
			{//if we are in an empty text
				kdDebug(25001)<< "KafkaHTMLPart::keyDelete() - deleting" <<
					"empty #text" << endl;
				_nodeParent = _nodeNext.parentNode();
				emit domNodeIsAboutToBeRemoved(_nodeNext, true);
				_nodeParent.removeChild(_nodeNext);
				_nodeParent.applyChanges();
				_nodeNext = _node;
				continue;
			}
		}
		//else if(!_nodeNext.hasChildNodes())
		else if(attrs->chCurFoc() == kNodeAttrs::singleNode || attrs->chCurFoc() ==
			kNodeAttrs::singleNodeAndItself || ((attrs->chCurFoc() ==
			kNodeAttrs::no || attrs->chCurFoc() == kNodeAttrs::left) &&
			!_nodeNext.hasChildNodes()))
		{
			kdDebug(25001)<< "KafkaHTMLPart::keyDelete() - deleting" <<
				" a Node" << endl;
			_nodeParent = _nodeNext.parentNode();
			focus = w->getAttrs(_nodeNext)->chCurFoc();
			emit domNodeIsAboutToBeRemoved(_nodeNext, true);
			_nodeParent.removeChild(_nodeNext);
			//normalize(_nodeParent);
			if(focus != kNodeAttrs::no)
			{
				postprocessCursorPosition();
				return;
			}
			_nodeNext = _node;
			continue;
		}
		/**else if(d->TagsDeletable.contains(_nodeNext.nodeName().string().upper()))
		{//if it is a deletable tag, remove it
			kdDebug(25001)<< "KafkaHTMLPart::keyDelete() - deleting a" <<
				" TagDeletable" << endl;
			_nodeParent = _nodeNext.parentNode();
			emit domNodeIsAboutToBeRemoved(_nodeNext, true);
			_nodeParent.removeChild(_nodeNext);
			//normalize(_nodeParent);
			_nodeParent.applyChanges();
			postprocessCursorPosition();
			return;
		}
		else if(d->TagsTextDeletable.contains(_nodeNext.nodeName().string().upper()))
		{//if it is a Text deletable tag
			if(!_nodeNext.hasChildNodes())
			{//if the tagTextDeletable has no childs, delete it
				kdDebug(25001)<< "KafkaHTMLPart::keyDelete() - deleting" <<
					" a TagTextDeletable" << endl;
				emit domNodeIsAboutToBeRemoved(_nodeNext, true);
				_nodeNext.parentNode().removeChild(_nodeNext);
				_nodeNext = _node;
				continue;
			}
			else if(_goingTowardsRootNode && _nodeNext.childNodes().length() >= 1)
			{
				continue;
			}
		}*/
	}
}

void KafkaHTMLPart::keyBackspace()
{
	kNodeAttrs *attrs;
	int focus;
	DOM::Node _nodeParent, _node, _nodePrev, oldCurrentNode;
	bool _goingTowardsRootNode;

	if(m_currentNode.isNull())
		return;
	attrs = w->getAttrs(m_currentNode);
	if(attrs->chCurFoc() == kNodeAttrs::textNode && d->m_cursorOffset != 0)
	{//if we are in the middle of some text, we remove one letter
		if(!attrs->cbMod())
			return;
		kdDebug(25001)<< "KafkaHTMLPart::keyBackspace() - one letter removed - 1" << endl;
		DOM::DOMString nodeText = m_currentNode.nodeValue();
		DOM::DOMString textSplitted = nodeText.split(d->m_cursorOffset);
		nodeText.split(d->m_cursorOffset - 1);
		m_currentNode.setNodeValue(nodeText + textSplitted);
		m_currentNode.parentNode().applyChanges();
		d->m_cursorOffset--;
		emit domNodeModified(m_currentNode);
		postprocessCursorPosition();
		emit domNodeNewCursorPos(m_currentNode, d->m_cursorOffset);
		return;
	}
	if(attrs->chCurFoc() != kNodeAttrs::no && attrs->chCurFoc() != kNodeAttrs::textNode &&
		d->m_cursorOffset != 0)
	{//if we delete ourselves, which node will be m_currentNode??
		if(!attrs->cbDel())
			return;
		kdDebug(25001)<< "KafkaHTMLPart::keyBackspace() - deleting a TagDeletable - 2" <<
			endl;
		DOM::Node _node = m_currentNode;
		bool b = false;
		while(1)
		{//try to find a previous node from which we can delete the node
			_node = getPrevNode(_node, b);
			if(_node == 0) break;
			attrs = w->getAttrs(_node);
			if(attrs->chCurFoc() == kNodeAttrs::textNode)
			{
				m_currentNode = _node;
				d->m_cursorOffset = (static_cast<DOM::CharacterData>(_node)).length();
				keyDelete();
				emit domNodeNewCursorPos(m_currentNode, d->m_cursorOffset);
				return;
			}
			if(attrs->chCurFoc() != kNodeAttrs::no && attrs->chCurFoc() !=
				kNodeAttrs::textNode)
			{
				m_currentNode = _node;
				d->m_cursorOffset = 1;
				keyDelete();
				emit domNodeNewCursorPos(m_currentNode, d->m_cursorOffset);
				return;
			}
		}
		_node = m_currentNode;
		b = false;
		while(1)
		{//try to find a next node from which we can delete the node
			_node = getNextNode(_node, b);
			if(_node == 0) break;
			attrs = w->getAttrs(_node);
			if(attrs->chCurFoc() != kNodeAttrs::no)
			{
				m_currentNode = _node;
				d->m_cursorOffset = 0;
				keyBackspace();
				emit domNodeNewCursorPos(m_currentNode, d->m_cursorOffset);
				return;
			}
		}
		//here, there is no node right and left that can have the cursor focus
		_node = m_currentNode.parentNode();
		emit domNodeIsAboutToBeRemoved(m_currentNode, true);
		_node.removeChild(m_currentNode);
		m_currentNode = document().createTextNode("");
		_node.appendChild(m_currentNode);
		emit domNodeInserted(m_currentNode, false);
		emit domNodeNewCursorPos(m_currentNode, d->m_cursorOffset);

	}

	_node = m_currentNode;
	_nodePrev = m_currentNode;
	oldCurrentNode = m_currentNode;
	_goingTowardsRootNode = false;

	while(1)
	{
		_nodePrev = getPrevNode(_nodePrev, _goingTowardsRootNode);

		//second part : depending of the node, we choose what to do
		if(_nodePrev == 0)
			return;
		attrs = w->getAttrs(_nodePrev);
		if(!attrs->cbDel())
			return;
		if(attrs->chCurFoc() == kNodeAttrs::textNode)
		{
			if((static_cast<DOM::CharacterData>(_nodePrev)).length() != 0)
			{//if we are in text, remove a letter
				kdDebug(25001)<< "KafkaHTMLPart::keyBackspace() - one" <<
					" letter removed - 2" << endl;
				DOM::DOMString nodeText = _nodePrev.nodeValue();
				nodeText.split((static_cast<DOM::CharacterData>(_nodePrev)).length() - 1);
				_nodePrev.setNodeValue(nodeText);
				_nodePrev.parentNode().applyChanges();
				postprocessCursorPosition();
				emit domNodeModified(_nodePrev);
				return;
			}
			else
			{//if we are in an empty text
				kdDebug(25001)<< "KafkaHTMLPart::keyBackspace() - deleting" <<
					" empty #text" << endl;
				_nodeParent = _nodePrev.parentNode();
				emit domNodeIsAboutToBeRemoved(_nodePrev, true);
				_nodeParent.removeChild(_nodePrev);
				_nodeParent.applyChanges();
				_nodePrev = _node;
				continue;
			}
		}
		else if(attrs->chCurFoc() == kNodeAttrs::singleNode || attrs->chCurFoc() ==
			kNodeAttrs::singleNodeAndItself || ((attrs->chCurFoc() ==
			kNodeAttrs::no || attrs->chCurFoc() == kNodeAttrs::left) &&
			!_nodePrev.hasChildNodes()))
		{
			kdDebug(25001)<< "KafkaHTMLPart::keyBackspace() - deleting" <<
				" a Node" << endl;
			_nodeParent = _nodePrev.parentNode();
			focus = w->getAttrs(_nodePrev)->chCurFoc();
			emit domNodeIsAboutToBeRemoved(_nodePrev, true);
			_nodeParent.removeChild(_nodePrev);
			//normalize(_nodeParent);
			if(focus != kNodeAttrs::no)
			{
				postprocessCursorPosition();
				return;
			}
			_nodePrev = _node;
			continue;
		}
		/**else if(d->TagsDeletable.contains(_nodePrev.nodeName().string().upper()))
		{//if it is a deletable tag, remove it
			kdDebug(25001)<< "KafkaHTMLPart::keyBackspace() - deleting a TagDeletable"
				<< endl;
			if((_nodePrev.previousSibling() != 0) &&
				_nodePrev.previousSibling().nodeType() == DOM::Node::TEXT_NODE &&
				_node.nodeType() == DOM::Node::TEXT_NODE &&
				_nodePrev.parentNode() == _nodePrev.previousSibling().parentNode())
			{//to avoid a bug due to normalize()
				m_currentNode = _nodePrev.previousSibling();
				d->m_cursorOffset = (static_cast<DOM::CharacterData>(m_currentNode)).length();
				emit domNodeNewCursorPos(m_currentNode, d->m_cursorOffset);
			}
			_nodeParent = _nodePrev.parentNode();
			emit domNodeIsAboutToBeRemoved(_nodePrev, true);
			_nodeParent.removeChild(_nodePrev);
			//_nodeParent.normalize();
			//normalize(_nodeParent);
			_nodeParent.applyChanges();
			postprocessCursorPosition();
			return;
		}
		else if(d->TagsTextDeletable.contains(_nodePrev.nodeName().string().upper()))
		{//if it is a Text deletable tag
			if(!_nodePrev.hasChildNodes())
			{//if we go up and we can delete the tag, delete it :)
				kdDebug(25001)<< "KafkaHTMLPart::keyBackspace() - deleting" <<
					" a TagTextDeletable" << endl;
				emit domNodeIsAboutToBeRemoved(_nodePrev, true);
				_nodePrev.parentNode().removeChild(_nodePrev);
				_nodePrev = _node;
				continue;
			}
			else if(_goingTowardsRootNode && _nodePrev.childNodes().length() >= 1)
			{
				continue;
			}
		}*/
	}
}

DOM::Node KafkaHTMLPart::getNextNode(DOM::Node _node, bool &goingTowardsRootNode, bool skipParentNodes, bool dontBlock, DOM::Node _endNode)
{
	kNodeAttrs *attrs = 0L;

	if(_node == 0)
		return 0;
	attrs = w->getAttrs(_node);
	if(!attrs)
	{
		kdDebug(25001)<< "KafkaHTMLPart::getNextNode() Attrs not found!"<< endl;
		return 0;
	}
	if(_node.hasChildNodes() && goingTowardsRootNode == false &&
		(attrs->ccanEnter() || dontBlock))
	{//if we can descend to a child node, we do it
		kdDebug(25001)<< "KafkaHTMLPart::getNextNode() - descending from node : " <<
			_node.nodeName().string() << " to " <<
			_node.firstChild().nodeName().string() << endl;
		if(_endNode == _node.firstChild())
			return 0;
		return _node.firstChild();
	}
	if(_node.nextSibling() != 0)
	{//else if there is a sibling, we move to it
		goingTowardsRootNode = false;
		kdDebug(25001)<< "KafkaHTMLPart::getNextNode() - going from node : " <<
			_node.nodeName().string() <<
		 	" to " << _node.nextSibling().nodeName().string() << endl;
		if(_endNode == _node.nextSibling())
			return 0;
		return _node.nextSibling();
	}
	if(_node.nextSibling() == 0)
	{//else if there is no sibling, we go up if we can
		goingTowardsRootNode = true;
		if(_node.parentNode().isNull())
			return 0;
		if(w->getAttrs(_node.parentNode()) &&
			w->getAttrs(_node.parentNode())->ccanEnter() || dontBlock)
		{
			if(!_node.parentNode().isNull())
				kdDebug(25001)<< "KafkaHTMLPart::getNextNode() - going" <<
					" up from node : " << _node.nodeName().string() <<
				 	" to " << _node.parentNode().nodeName().string() << endl;
			else
				kdDebug(25001)<< "KafkaHTMLPart::getNextNode() - going" <<
					" up from node : " << _node.nodeName().string() <<
					" to an empty Node" << endl;
			if(skipParentNodes)
			{
				if(_endNode == _node.parentNode())
					return 0;
				return getNextNode(_node.parentNode(), goingTowardsRootNode,
					skipParentNodes, dontBlock);
			}
			else
			{
				if(_endNode == _node.parentNode())
					return 0;
				return _node.parentNode();
			}
		}
		else
			return 0;
	}
	kdError()<< "KafkaHTMLPart::getNextNode() ERROR" << endl;
	return 0;
}

DOM::Node KafkaHTMLPart::getPrevNode(DOM::Node _node, bool &goingTowardsRootNode, bool skipParentNodes, bool dontBlock, DOM::Node _endNode)
{
	kNodeAttrs *attrs = 0L;

	if(_node == 0)
		return 0;
	attrs = w->getAttrs(_node);
	if(!attrs)
	{
		kdDebug(25001)<< "KafkaHTMLPart::getPrevNode() Attrs not found!"<< endl;
		return 0;
	}
	if(_node.hasChildNodes() && goingTowardsRootNode == false &&
		(attrs->ccanEnter() || dontBlock))
	{//if we can descend to a child node, we do it
		kdDebug(25001)<< "KafkaHTMLPart::getPrevNode() - descending from node : " <<
			_node.nodeName().string() << " to " <<
			_node.lastChild().nodeName().string() << endl;
		if(_endNode == _node.lastChild())
			return 0L;
		return _node.lastChild();
	}
	if(_node.previousSibling() != 0)
	{//else if there is a sibling, we move to it
		goingTowardsRootNode = false;
		kdDebug(25001)<< "KafkaHTMLPart::getPrevNode() - going from node : " <<
			_node.nodeName().string() <<
		 	" to " << _node.previousSibling().nodeName().string() << endl;
		if(_endNode == _node.previousSibling())
			return 0L;
		return _node.previousSibling();
	}
	if(_node.previousSibling() == 0)
	{//else if there is no sibling, we go up if we can
		goingTowardsRootNode = true;
		if(_node.parentNode().isNull())
			return 0L;
		if(w->getAttrs(_node.parentNode()) &&
			w->getAttrs(_node.parentNode())->ccanEnter() || dontBlock)
		{
			if(!_node.parentNode().isNull())
				kdDebug(25001)<< "KafkaHTMLPart::getPrevNode() - going up from" <<
					" node : " << _node.nodeName().string() << " to " <<
					_node.parentNode().nodeName().string() << endl;
			else
				kdDebug(25001)<< "KafkaHTMLPart::getPrevNode() - going up from" <<
					" node : " << _node.nodeName().string() << " to an " <<
					"empty Node" << endl;
			if (skipParentNodes)
			{
				if(_endNode == _node.parentNode())
					return 0L;
				return getPrevNode(_node.parentNode(), goingTowardsRootNode,
					skipParentNodes, dontBlock);
			}
			else
			{
				if(_endNode == _node.parentNode())
					return 0L;
				return _node.parentNode();
			}
		}
		else
			return 0;
	}
	kdError()<< "KafkaHTMLPart::getPrevNode() ERROR" << endl;
	return 0;
}

void KafkaHTMLPart::makeCursorVisible(int xMargin, int yMargin)
{
	if(m_currentNode == 0)
		return;
	kdDebug(25001)<< "KafkaHTMLPart::makeCursorVisible" << endl;
	int X, Y, dummy;
	getCursor(m_currentNode, d->m_cursorOffset, X, Y, dummy);
	view()->ensureVisible (X, Y, xMargin, yMargin);
}

void KafkaHTMLPart::postprocessCursorPosition()
{
	kNodeAttrs *attrs, *attrs2;
	if(m_currentNode == 0)
		return;
	attrs = w->getAttrs(m_currentNode);
	DOM::Node _prevNextNode;
	DOM::Node _nextNode = m_currentNode;
	bool b = false;
	if(attrs->chCurFoc() == kNodeAttrs::textNode &&
		d->m_cursorOffset == 0)
	{
		while(1)
		{
			_prevNextNode = _nextNode;
			_nextNode = getPrevNode(_nextNode, b);
			if(_nextNode == 0)
				break;
			attrs2 = w->getAttrs(_nextNode);
			if(attrs2->chCurFoc() == kNodeAttrs::textNode &&
				(static_cast<DOM::CharacterData>(_nextNode)).length() != 0)
			{
				m_currentNode = _nextNode;
				d->m_cursorOffset = (static_cast<DOM::CharacterData>(_nextNode)).length();
				emit domNodeNewCursorPos(m_currentNode, d->m_cursorOffset);
				kdDebug(25001)<< "kafkaHTMLPart::postprocessCursorPosition()" <<
					" - new currentNode :" <<
					m_currentNode.nodeName().string() << endl;
				break;
			}
			else if(attrs2->chCurFoc() == kNodeAttrs::singleNode ||
				attrs2->chCurFoc() == kNodeAttrs::singleNodeAndItself ||
				(attrs2->chCurFoc() == kNodeAttrs::left && b))
				break;
			else
				continue;
		}
	}
	else if(attrs->chCurFoc() == kNodeAttrs::singleNode || attrs->chCurFoc() ==
		kNodeAttrs::singleNodeAndItself || attrs->chCurFoc() == kNodeAttrs::left)
	{
		if(d->m_cursorOffset == 0)
		{
			while(1)
			{
				_prevNextNode = _nextNode;
				_nextNode = getPrevNode(_nextNode, b);
				if(_nextNode == 0)
					break;
				attrs2 = w->getAttrs(_nextNode);
				if(attrs2->chCurFoc() == kNodeAttrs::textNode &&
					(static_cast<DOM::CharacterData>(_nextNode)).length()
					!= 0)
				{
					m_currentNode = _nextNode;
					d->m_cursorOffset = (static_cast<DOM::CharacterData>(_nextNode)).length();
					emit domNodeNewCursorPos(m_currentNode,
						d->m_cursorOffset);
					kdDebug(25001)<< "kafkaHTMLPart::postprocessCursorPosition()" " - new currentNode :"
						<< m_currentNode.nodeName().string() << endl;
					break;
				}
				else if(attrs2->chCurFoc() == kNodeAttrs::singleNode ||
					attrs2->chCurFoc() == kNodeAttrs::singleNodeAndItself ||
					(attrs2->chCurFoc() == kNodeAttrs::left && b))
				{
					m_currentNode = _nextNode;
					d->m_cursorOffset = 1;
					emit domNodeNewCursorPos(m_currentNode, d->m_cursorOffset);
					kdDebug(25001)<< "kafkaHTMLPart::postprocessCursorPosition() - new currentNode :"
						<< m_currentNode.nodeName().string() << endl;
					break;
				}
				else
					continue;
			}
		}
		else if(d->m_cursorOffset == 1)
		{
			while(1)
			{
				_prevNextNode = _nextNode;
				_nextNode = getNextNode(_nextNode, b);
				if(_nextNode == 0) break;
				attrs2 = w->getAttrs(_nextNode);
				if(attrs2->chCurFoc() == kNodeAttrs::singleNode ||
					attrs2->chCurFoc() == kNodeAttrs::singleNodeAndItself ||
					(attrs2->chCurFoc() == kNodeAttrs::left && b))
					break;
				else if(attrs2->chCurFoc() == kNodeAttrs::textNode &&
					(static_cast<DOM::CharacterData>(_nextNode)).length() != 0)
				{
					m_currentNode = _nextNode;
					d->m_cursorOffset = 0;
					emit domNodeNewCursorPos(m_currentNode, d->m_cursorOffset);
					kdDebug(25001)<< "kafkaHTMLPart::postprocessCursorPosition() - new currentNode :"
						<< m_currentNode.nodeName().string() << endl;
					break;
				}
				else
					continue;
			}
		}
	}
	makeCursorVisible();
}

void KafkaHTMLPart::khtmlMouseMoveEvent(khtml::MouseMoveEvent *event)
{
	DOM::Node mouseNode = event->innerNode();
	if(mouseNode == 0)
		return;
	if(mouseNode.nodeType() == DOM::Node::TEXT_NODE)
		KApplication::setOverrideCursor(Qt::ibeamCursor);
	else
		KApplication::setOverrideCursor(Qt::arrowCursor);
	//temporaly disable that, give some weird result on khtml CVS20030303
	/**
	DOM::Node mouseNode = event->innerNode();
	int mouseOffset;


	if(mouseNode.nodeType() == DOM::Node::TEXT_NODE)
		{}//mouseOffset = event->offset(); temp to make it compile on KDE3.1
	else
		mouseOffset = 0;

	if(mouseNode.nodeType() == DOM::Node::TEXT_NODE)
		KApplication::setOverrideCursor(Qt::ibeamCursor);
	else
		KApplication::setOverrideCursor(Qt::arrowCursor);

	// Niko Z:
	// Selection handling
	//kdDebug(25001) << "KafkaHTMLPart::khtmlMouseMoveEvent() Press Offset: " << d->m_pressOffset << " Current Offset: " << mouseOffset << endl;

	DOM::Range tempRange(document());

	if(mouseOffset > d->m_pressOffset)
	{
		if(!mouseNode.isNull() || !m_currentNode.isNull())
		{
			if(!mouseNode.handle() || !m_currentNode.handle())
				return;

			kdDebug(25001) << "KafkaHTMLPart::khtmlMouseMoveEvent() Selection: Left to Right" << endl;
//			tempRange.setStart(mouseNode, mouseOffset);
//			tempRange.setEnd(m_currentNode, d->m_cursorOffset);

			setSelection(tempRange);
			m_currentSelection = tempRange;
		}
	}
	else if(mouseOffset < d->m_pressOffset)
	{
		if(!mouseNode.isNull() || !m_currentNode.isNull())
		{
			if(!mouseNode.handle() || !m_currentNode.handle())
				return;

			kdDebug(25001) << "KafkaHTMLPart::khtmlMouseMoveEvent() Selection: Right to Left" << endl;
			tempRange.setStart(m_currentNode, d->m_cursorOffset);
			tempRange.setEnd(mouseNode, mouseOffset);

			setSelection(tempRange);
			m_currentSelection = tempRange;
		}
	}*/
}

void KafkaHTMLPart::khtmlMousePressEvent(khtml::MousePressEvent *event)
{
	kNodeAttrs *attrs;
	if(event->innerNode() == 0)
	{
		kdDebug(25001)<< "KafkaHTMLPart::khtmlMousePressEvent() -" <<
			" event->innerNode().isNull() = " <<
		event->innerNode().isNull() << endl;
	}
	m_currentNode = event->innerNode();
	d->stuckCursorHorizontalPos = false;
	/** TEMPORARY before - i hope - khtml changes*/
	//look for the child node which has really the cursor focus
	if(m_currentNode.hasChildNodes())
	{
		DOM::Node _node = m_currentNode.firstChild();
		//DOM::Node _nodeParent = m_currentNode;
		bool firstChild = true, foundMatchingNode = false;
		do
		{
			if(!firstChild)
				_node = _node.nextSibling();
			else
				firstChild = false;
			if(_node.getRect().contains(event->x(), event->y()))
			{
				if(_node.hasChildNodes())
				{
					//_nodeParent = _node;
					 foundMatchingNode = true;
					_node = _node.firstChild();
					firstChild = true;
					continue;
				}
				else
					break;
			}
		}while(_node.nextSibling() != 0);
		if(foundMatchingNode)
		{
			m_currentNode = _node;
			d->m_cursorOffset = 1;
		}
		kdDebug(25001)<< "KafkaHTMLPart::khtmlMousePressEvent() - CursorNode : "
			<< m_currentNode.nodeName().string() << endl;
	}
	//these nodes can't have the cursor focus : using Pythagore to get the
	//nearest node that can have the cursor focus
	attrs = w->getAttrs(m_currentNode);
	if(attrs->chCurFoc() == kNodeAttrs::no)
	{
		if(!m_currentNode.hasChildNodes())
		{
			m_currentNode.appendChild(document().createTextNode(""));
			m_currentNode = m_currentNode.firstChild();
		}
		else
		{
			int tmpX = 0, tmpY = 0, finalX = 0, finalY = 0;
			DOM::Node _tmp;
			DOM::Node _node = m_currentNode;
			bool _firstNode;
			while(_node.hasChildNodes())
			{
				_firstNode = true;
				_node = _node.firstChild();
				tmpX = 0; tmpY = 0; finalX = 0; finalY = 0;
				do
				{
					if(!_firstNode)
						_node = _node.nextSibling();
					else
						_firstNode = false;
					tmpX = (_node.getRect().left() +
						_node.getRect().right())/2;
					tmpY = (_node.getRect().top() +
						_node.getRect().bottom())/2;
					if(((tmpX - event->x())*(tmpX - event->x()) +
						(tmpY - event->y())*(tmpY - event->y())) <
						((finalX - event->x())*(finalX - event->x()) +
						(finalY - event->y())*(finalY - event->y())))
					{//half pythagore too :p
						finalX = tmpX;
						finalY = tmpY;
						_tmp = _node;
					}
				}while(!_node.nextSibling().isNull());
				_node = _tmp;
			}
			m_currentNode = _node;
			kdDebug(25001)<< "KafkaHTMLPart::khtmlMousePressEvent() - new" <<
				" CursorNode : " <<
			m_currentNode.nodeName().string() << endl;
		}
	}
	if(m_currentNode.isNull())
	{
		kdDebug(25001)<< "KafkaHTMLPart::khtmlMousePressEvent() - NULL DOM::NODE!" <<
			endl;
		return;
	}

	attrs = w->getAttrs(m_currentNode);
	if(attrs->chCurFoc() == kNodeAttrs::singleNodeAndItself)
	{//TODO:allow the resizing of theses nodes
		kdDebug(25001)<< "KafkaHTMLPart::khtmlMousePressEvent() - upcoming selection"
			<< endl;
		d->m_cursorOffset = -1;
	}
	//if our node is a text, find out which offset is the better one
	if(attrs->chCurFoc() == kNodeAttrs::textNode)
	{
		int tmpOffset = 0, finalOffset = 0;
		int tmpOffsetX = 0, finalOffsetX = 0;
		int tmpOffsetY = 0, finalOffsetY = 0;
		int tmpOffsetHeight = 0;
		while((unsigned)tmpOffset <= (static_cast<DOM::CharacterData>(m_currentNode)).length())
		{
			m_currentNode.getCursor(tmpOffset, tmpOffsetX, tmpOffsetY, tmpOffsetHeight);
			if(((event->x() - tmpOffsetX)*(event->x() - tmpOffsetX) +
				(event->y() - (tmpOffsetY + tmpOffsetHeight/2))*(event->y() - (tmpOffsetY + tmpOffsetHeight/2))) <
				(((event->x() - finalOffsetX)*(event->x() - finalOffsetX)) +
				(event->y() - (finalOffsetY + tmpOffsetHeight/2))*(event->y() - (finalOffsetY + tmpOffsetHeight/2))))
			{//half-Pythagore :)
				finalOffset = tmpOffset;
				finalOffsetX = tmpOffsetX;
				finalOffsetY = tmpOffsetY;
			}
			tmpOffset++;
		}
		d->m_cursorOffset = finalOffset;
	}
	kdDebug(25001)<< "KafkaHTMLPart::khtmlMousePressEvent() - offset : " <<
		d->m_cursorOffset <<  endl;

	makeCursorVisible();
	if(d->cursorTimer)
		killTimer(d->cursorTimer);
	d->cursorTimer = startTimer(500);
	d->drawCursor = true;
	d->cursorOn = true;
	view()->updateContents();
	emit domNodeNewCursorPos(m_currentNode, d->m_cursorOffset);
}

void KafkaHTMLPart::khtmlMouseReleaseEvent(khtml::MouseReleaseEvent *event)
{
	/**m_currentNode = event->innerNode();

	if(m_currentNode.nodeType() == DOM::Node::TEXT_NODE)
		{}//d->m_releaseOffset = event->offset(); temp to make it compile on KDE3.1
	else
		d->m_releaseOffset = 0;

	d->m_cursorOffset = d->m_releaseOffset;

	m_currentNode.parentNode().applyChanges();*/
}

void KafkaHTMLPart::khtmlDrawContentsEvent(khtml::DrawContentsEvent *event)
{
	kNodeAttrs *attrs = 0L;
	if(m_currentNode.isNull()) return;
	attrs = w->getAttrs(m_currentNode);
	if(!attrs) return;
	try
	{
		if(attrs->chCurFoc() != kNodeAttrs::no)
		{
			QPainter *p = event->painter();
			if(d->cursorOn)
				paintCursor(p);
		}
	} catch(DOM::DOMException e){;}
	KHTMLPart::khtmlDrawContentsEvent(event);
}

bool KafkaHTMLPart::getCursor(DOM::Node _node, int offset, int &_x, int &_y, int &height)
{//temp method until - i hope :) - khtml will provide this
	kNodeAttrs *attrs = w->getAttrs(_node);
	if(attrs->chCurFoc() == kNodeAttrs::textNode)
	{
		_node.getCursor(offset, _x, _y, height);
		return true;
	}
	else if(attrs->chCurFoc() == kNodeAttrs::singleNode ||
		attrs->chCurFoc() == kNodeAttrs::singleNodeAndItself ||
		attrs->chCurFoc() == kNodeAttrs::left)
	{
		if(_node.nodeName().string() == "BR")
		{//TODO:fix the BR case
			_x = _node.getRect().right();
			_y = _node.getRect().bottom() - 17;
			height = 17;
		}
		else
		{
			if(offset == 1)
			{
				_x = _node.getRect().right() + 1;
				_y = _node.getRect().bottom() - 17;
				height = 16;
			}
			else if(offset == 0)
			{
				_x = _node.getRect().left() - 1;
				_y = _node.getRect().bottom() - 17;
				height = 16;
			}
			else if(offset == -1)
				return false;
		}
		return true;
	}
	return false;
}

void KafkaHTMLPart::paintCursor(QPainter *p)
{
	kNodeAttrs *attrs;

	if(m_currentNode == 0)
		return;
	attrs = w->getAttrs(m_currentNode);
	if(attrs->chCurFoc() != kNodeAttrs::no)
	kdDebug(25001) << "KafkaHTMLPart::paintCursor() - Node:" <<
		m_currentNode.nodeName().string() << ":" << (static_cast<DOM::CharacterData>(m_currentNode)).length() <<
		" ; offset :" << d->m_cursorOffset << endl;
	if(d->drawCursor)
	{
		int x = 0, y = 0, cursorHeight = 0;
		if(getCursor(m_currentNode, d->m_cursorOffset, x, y, cursorHeight))
			p->drawLine(x, y, x, y + cursorHeight);
	}
	d->drawCursor = !d->drawCursor;
}

void KafkaHTMLPart::timerEvent( QTimerEvent *e )
{
	if (e->timerId() == d->cursorTimer)
	{
		view()->updateContents();
	}
	KHTMLPart::timerEvent(e);
}

DOM::Node KafkaHTMLPart::createNode(QString NodeName)
{
	//this will change with the futur DTDs support
        DOM::DOMString _nodename(NodeName);
	if(NodeName == "TEXT")
		return document().createTextNode("");
	return document().createElement(_nodename);
}

void KafkaHTMLPart::showDomTree()
{
		domdialog->show();
}

void KafkaHTMLPart::getCurrentNode(DOM::Node &_currentNode, int &offset)
{
	_currentNode = m_currentNode;
	offset = d->m_cursorOffset;
}

void KafkaHTMLPart::finishedLoading()
{
	kNodeAttrs *attrs = 0L;
	DOM::Node _node = document();
        bool b = false;

	if(_node == 0)
		kdDebug(25001)<< "KafkaHTMLPart::finishedLoading() ERROR : no document()!" << endl;
	while(_node != 0)
	{
		_node = getNextNode(_node, b);
		if(_node == 0)
		{
			_node = document();
			break;
		}
		attrs = w->getAttrs(_node);
		if(!attrs) return;
		if(attrs->chCurFoc() != kNodeAttrs::no)
			break;
	}
	m_currentNode = _node;
        d->m_cursorOffset = 0;
	emit domNodeNewCursorPos(m_currentNode, d->m_cursorOffset);
}

