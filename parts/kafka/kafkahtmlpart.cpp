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
#include "kafkahtmlview.h"
#include "kafkahtmlpart.moc"

class KafkaHTMLPartPrivate
{
public:
	KafkaHTMLPartPrivate()
	{
		cursorTimer = 0;
		drawCursor = true;
		/** TagsNonDeletable are blocking tags : they can't be moved or deleted : the cursor must stop */
		BlockingTags << "AREA" << "BASE" << "BODY" << "CAPTION" << "COL" << "COLGROUP" << "FRAME"
			<< "FRAMESET" << "HEAD" << "HTML" << "IFRAME" << "LINK" << "META" << "NOFRAMES" << "NOSCRIPT"
			<< "PARAM" << "STYLE" << "TBODY" << "TD" << "TFOOT" << "TH" << "TITLE" << "TR";
		/** TagsDeletable are tags in which the cursor can't enter but can delete */
		TagsDeletable << "APPLET" << "BR" << "BUTTON" << "HR" << "IMG" << "INPUT" << "ISINDEX" << "MAP"
			<< "OBJECT" << "OPTION" << "SELECT" << "TABLE" << "TEXTAREA";
		/** TagsTextDeletable are tags in which the cursor can enter and can delete when leaving empty */
		TagsTextDeletable << "A" << "ABBR" << "ACRONYM" << "ADDRESS" << "B" << "BDO" << "BIG" << "BLOCKQUOTES"
			<< "CENTER" << "CITE" << "CODE" << "DD" << "DEL" << "DFN" << "DIR" << "DIV" << "DL" << "DH"
			<< "EM" << "FIELDSET" << "FONT" << "FORM" << "H1" << "H2" << "H3" << "H4" << "H5" << "H6" << "I"
			<< "INS" << "KDB" << "LABEL" << "LEGEND" << "LI" << "MENU" << "OL" << "OPTGROUP" << "P" << "PRE"
			<< "Q" << "S" << "SAMP" << "SMALL" << "SPAN" << "STRIKE" << "STRONG" << "SUB" << "SUP" << "TT"
			<< "U" << "UL" << "VAR";
		/** TagsMoveable --TEMPORARY-- are tags that are to be moved if there are about to be deleted
		  * TODO : remplace the script and comments tags with icons
		  */
		TagsMoveable << "SCRIPT";
		/** TagsTransparentelyDeletable are tags which are deleted whithout moving the cursor */
		TagsTransparentelyDeletable << "BASEFONT";
	}
	~KafkaHTMLPartPrivate()
	{
	}
	int cursorTimer;
	bool cursorOn;
	bool drawCursor;
	/** lists of all the HTML 4.01 tags which determine backspace and delete behavior */
	QStringList BlockingTags;
	QStringList TagsDeletable;
	QStringList TagsTextDeletable;
	QStringList TagsMoveable;
	QStringList TagsTransparentelyDeletable;
	bool m_readOnly;
	int m_cursorOffset;
	int m_pressOffset;
	int m_releaseOffset;
	/** when going up and down, trying to be as close as possible from the original node X pos
	* like a good text editor :=) */
	bool stuckCursorHorizontalPos;
	int stuckedCursorPosX;
};

KafkaHTMLPart::KafkaHTMLPart(QWidget *parent, QWidget *widgetParent, const char *name)
	: KHTMLPart(new KafkaHTMLView(this, widgetParent, name), parent, name)
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
	//connect(this, SIGNAL(domChanged()), DOMTreeWidget, SLOT(showTree(const DOM::Node &pNode)));
#endif
//end

	newDocument();
}

KafkaHTMLPart::~KafkaHTMLPart()
{
}

void KafkaHTMLPart::newDocument()
{
 QChar attrValueQuotation = '"'; //FIXME: Somehow we should get it from Quanta settings: qConfig.attrValueQuotation
	QString newPageHTMLCode = "<html>\n"
					 "<head>\n"
					"<title>" + i18n("New Homepage") + "</title>\n"
					"</head>\n"
					"<body bgColor="+attrValueQuotation+"#FFFFFF"+attrValueQuotation+" text="+attrValueQuotation+"#000000"+attrValueQuotation+">\n"
					"&nbsp;\n"
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

			DOM::Node node = static_cast<DOM::Node>(htmlDocument());
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

	kdDebug(25001) << "KafkaHTMLPart::insertNode() - Node (" << parentNode.nodeName().string() << ").insertNode(" << node.nodeName().string() << ")" << endl;

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

	emit domNodeInserted(node);
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
//	node.nodeValue().insert(text, position);
//	node.parentNode().applyChanges(); //buggy, perhaps because of khtml
	if(node.nodeType() == DOM::Node::TEXT_NODE)
	{
		try
		{
			DOM::DOMString textNode = node.nodeValue();
			DOM::DOMString textSplitted = textNode.split(position);
			node.setNodeValue(textNode + text + textSplitted);
			//node.parentNode().applyChanges();
			d->m_cursorOffset++;
			kdDebug(25001) << "KafkaHTMLPart::insertText() - added text" << endl;
		} catch(DOM::DOMException e){kdDebug(25001)<< "KafkaHTMLPart::insertText() ERROR - code : " << e.code << endl;}
		emit domNodeModified(node);
	}
	else if(d->TagsDeletable.contains(node.nodeName().string().upper()))
	{
		try
		{
			DOM::Text textNode = htmlDocument().createTextNode(text);
			DOM::Node parent = node.parentNode();
			parent.insertBefore(textNode, node.nextSibling());
			parent.applyChanges();
			m_currentNode = textNode;
			d->m_cursorOffset = 1;
			emit domNodeInserted(textNode);
			kdDebug(25001) << "KafkaHTMLPart::insertText() - added text - 2" << endl;
		} catch(DOM::DOMException e){kdDebug(25001)<< "KafkaHTMLPart::insertText() ERROR - code : " << e.code << endl;}
		emit domChanged();
	}
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
	/** Nicolas D.
	  * we search through the DOM tree for a TextNode or a TagsDeletable Node
	  * and considering the Nodes around where was the cursor, we set it.
	  * It seems to be working fine now.
	*/

//some checks
	/**	if(m_currentNode.isNull() || !(m_currentNode.nodeType() == DOM::Node::TEXT_NODE
		|| d->TagsDeletable.contains(m_currentNode.nodeName().string().upper())))
	{
		kdDebug(25001)<< "KafkaHTMLPart::keyRight() - Bad Node" << endl;
		return;
	}*/
	if(m_currentNode.isNull())
	{
		kdDebug(25001)<< "KafkaHTMLPart::keyRight() - Bad Node" << endl;
		return;
	}
	if(d->TagsDeletable.contains(m_currentNode.nodeName().string().upper()) &&
		d->m_cursorOffset > 1)
	{
		kdError(25001)<< "KafkaHTMLPart::keyLeft() - Invalid offset" << endl;
	}

	if(m_currentNode.nodeType() == DOM::Node::TEXT_NODE &&
		((unsigned)d->m_cursorOffset < (static_cast<DOM::CharacterData>(m_currentNode)).length()))
	{
		kdDebug(25001)<< "KafkaHTMLPart::keyRight() - Moving one pos right " << endl;
		d->m_cursorOffset++;
		postprocessCursorPosition();
		return;
	}
	else if(d->TagsDeletable.contains(m_currentNode.nodeName().string().upper()) &&
		d->m_cursorOffset == 0)
	{
		kdDebug(25001)<< "KafkaHTMLPart::keyRight() - Moving one pos right " << endl;
		d->m_cursorOffset++;
		postprocessCursorPosition();
		return;
	}

	DOM::Node _node = m_currentNode;
	DOM::Node _nodeCursor;
	bool b;
	//looking for the next node
	while(1)
	{
		_node = getNextNode(_node, b);
		if(_node == 0 || (_node.nodeType() == DOM::Node::TEXT_NODE
			&& (static_cast<DOM::CharacterData>(_node)).length() != 0)
			|| d->TagsDeletable.contains(_node.nodeName().string().upper()))
		{
			_nodeCursor = _node;
			break;
		}
	}
	kdDebug(25001)<< "KafkaHTMLPart::keyRight() - _nodeCursor = "
		<< _nodeCursor.nodeName().string() <<  endl;

	if(!_nodeCursor.isNull())
	{
		d->m_cursorOffset = 1;
		m_currentNode = _nodeCursor;
		emit domNodeGetFocus(m_currentNode);
		postprocessCursorPosition();
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
	/** Nicolas D.
	  * we search through the DOM tree for a TextNode or a TagsDeletable Node
	  * and considering the last Node where was the cursor, we set it.
	  * It seems to be working fine now.
	*/

	//some checks
/**	if(m_currentNode.isNull() || !(m_currentNode.nodeType() == DOM::Node::TEXT_NODE
		|| d->TagsDeletable.contains(m_currentNode.nodeName().string().upper())))
	{
		kdDebug(25001)<< "KafkaHTMLPart::keyLeft() - Bad Node" << endl;
		return;
	}*/
	if(m_currentNode.isNull())
	{
		kdDebug(25001)<< "KafkaHTMLPart::keyLeft() - Bad Node" << endl;
		return;
	}
	if(d->TagsDeletable.contains(m_currentNode.nodeName().string().upper()) &&
		d->m_cursorOffset > 1)
	{
		kdError(25001)<< "KafkaHTMLPart::keyLeft() - Invalid offset" << endl;
	}

	if((m_currentNode.nodeType() == DOM::Node::TEXT_NODE ||
		d->TagsDeletable.contains(m_currentNode.nodeName().string().upper())) &&
		d->m_cursorOffset > 0)
	{
		kdDebug(25001)<< "KafkaHTMLPart::keyLeft() - Moving one pos left1 " << endl;
		d->m_cursorOffset--;
		postprocessCursorPosition();
		return;
	}

	DOM::Node _startNode = m_currentNode;
	DOM::Node _node = m_currentNode;
	DOM::Node _nodeCursor;
	bool b;
	//looking for the next node
	while(1)
		{
		_node = getPrevNode(_node, b);
		if(_node == 0 || (_node.nodeType() == DOM::Node::TEXT_NODE
			&& (static_cast<DOM::CharacterData>(_node)).length() != 0)
			|| d->TagsDeletable.contains(_node.nodeName().string().upper()))
		{
			_nodeCursor = _node;
			break;
		}
	}


	kdDebug(25001)<< "KafkaHTMLPart::keyLeft() - _nodeCursor = "
		<< _nodeCursor.nodeName().string() << endl;

	if(!_nodeCursor.isNull())
	{
		//here d->m_cursorOffset == 0
		if(_nodeCursor.nodeType() == DOM::Node::TEXT_NODE)
			d->m_cursorOffset = (static_cast<DOM::CharacterData>(_nodeCursor)).length() - 1;
		//if _nodeCursor is a TagDeletable, it keeps the same cursorOffset
		m_currentNode = _nodeCursor;
		emit domNodeGetFocus(m_currentNode);
		postprocessCursorPosition();
	}
	kdDebug(25001)<< "KafkaHTMLPart::keyLeft() - new cursorNode = "
		<< m_currentNode.nodeName().string() << endl;

}

void KafkaHTMLPart::normalize(DOM::Node _node)
{
	kdDebug(25001)<< "KafkaHTMLPart::normalize()" << endl;
	DOM::Node childNode = _node.firstChild();
	while(childNode != 0 && childNode.nodeType() == DOM::Node::TEXT_NODE)
	{
		while(childNode.nextSibling() != 0 &&
			childNode.nextSibling().nodeType() == DOM::Node::TEXT_NODE )
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
	int tmpX, tmpY, tmpH;
	int currentX;
	int oldX, newX, oldY, newY, finalX = 10000;
	int i, tmpOffset = d->m_cursorOffset, finalOffset;
	bool b;
	DOM::Node _node = m_currentNode, _finalNode = 0;
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
				else if(_node.nodeType() == DOM::Node::TEXT_NODE)
					tmpOffset = (static_cast<DOM::CharacterData>(_node)).length();
				else if(d->TagsDeletable.contains(_node.nodeName().string().upper()))
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
		emit domNodeGetFocus(m_currentNode);
		d->m_cursorOffset = finalOffset;
		postprocessCursorPosition();
		if(!d->stuckCursorHorizontalPos)
		{
			d->stuckCursorHorizontalPos = true;
			d->stuckedCursorPosX = currentX;
		}
	}
	kdDebug(25001)<< "KafkaHTMLPart::keyUp() - Final Node : " << m_currentNode.nodeName().string() <<
		":" << d->m_cursorOffset << endl;
}

void KafkaHTMLPart::keyDown()
{
	int tmpX, tmpY, tmpH;
	int currentX;
	int oldX, newX, oldY, newY, finalX = 10000;
	int i, tmpOffset = d->m_cursorOffset, finalOffset;
	bool b;
	DOM::Node _node = m_currentNode, _finalNode = 0;
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
			if((_node.nodeType() == DOM::Node::TEXT_NODE &&
				(unsigned)tmpOffset < (static_cast<DOM::CharacterData>(_node)).length()) ||
				(d->TagsDeletable.contains(_node.nodeName().string().upper()) && tmpOffset == 0))
				tmpOffset++;
			else
			{
				_node = getNextNode(_node, b);
				if(_node == 0)
					break;
				else if(_node.nodeType() == DOM::Node::TEXT_NODE ||
					d->TagsDeletable.contains(_node.nodeName().string().upper()))
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
		emit domNodeGetFocus(m_currentNode);
		d->m_cursorOffset = finalOffset;
		postprocessCursorPosition();
		if(!d->stuckCursorHorizontalPos)
		{
			d->stuckCursorHorizontalPos = true;
			d->stuckedCursorPosX = currentX;
		}
	}
	kdDebug(25001)<< "KafkaHTMLPart::keyDown() - Final Node : " << m_currentNode.nodeName().string() <<
		":" << d->m_cursorOffset << endl;
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
				d->stuckCursorHorizontalPos = false;
				break;
			case Key_BackTab:
				break;
			case Key_Return:
				d->stuckCursorHorizontalPos = false;
				break;
			case Key_Enter:/**
				tmpNode = createNode("IMG");
				attr = htmlDocument().createAttribute("src");
				attr.setNodeValue("/home/guest/wallpapers/matrix08-1024.jpg");
				tmpNode.attributes().setNamedItem(attr);
				m_currentNode.parentNode().appendChild(tmpNode);
				d->stuckCursorHorizontalPos = false;
				emit domNodeInserted(tmpNode);*/
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
				else if(m_currentNode.nodeType() == DOM::Node::TEXT_NODE ||
					d->TagsDeletable.contains(m_currentNode.nodeName().string().upper()))
				{
					kdDebug(25001) << "KafkaHTMLPart::eventFilter() Text - " <<
						keyevent->text() << endl;
					if(keyevent->key() >= Key_Space && keyevent->key() != Key_unknown &&
						 !readOnly())
						insertText(keyevent->text(), -1);
					makeCursorVisible();
				}
				if(keyevent->key() == Qt::Key_Space)
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
	if(m_currentNode.isNull())
		return;
	if(m_currentNode.nodeType() == DOM::Node::TEXT_NODE &&
		(unsigned)d->m_cursorOffset != (static_cast<DOM::CharacterData>(m_currentNode)).length())
	{//if we are in the middle of some text, we remove one letter
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
	if(d->TagsDeletable.contains(m_currentNode.nodeName().string().upper()) &&
		d->m_cursorOffset == 0)
	{//if we delete ourselves, which node will be m_currentNode??
		kdDebug(25001)<< "KafkaHTMLPart::keyDelete() - deleting a TagDeletable - 2" << endl;
		DOM::Node _node = m_currentNode;
		bool b = false;
		//WARNING : we assume that there is no cursor node left because of
		//posprocessCursorPosition() which move the cursor to another one if one
		while(1)
		{//try to find a next node from which we can delete the node
			_node = getNextNode(_node, b);
			if(_node == 0)
				break;
			if(_node.nodeType() == DOM::Node::TEXT_NODE ||
				d->TagsDeletable.contains(_node.nodeName().string().upper()))
			{
				m_currentNode = _node;
				emit domNodeGetFocus(m_currentNode);
				d->m_cursorOffset = 0;
				keyBackspace();
				return;
			}
		}
		//here, there is no node right and left that can have the cursor focus
		_node = m_currentNode.parentNode();
		emit domNodeIsAboutToBeRemoved(m_currentNode, true);
		_node.removeChild(m_currentNode);
		m_currentNode = htmlDocument().createTextNode("");
		_node.appendChild(m_currentNode);
		emit domNodeInserted(m_currentNode);
		emit domNodeGetFocus(m_currentNode);
	}

	DOM::Node _nodeParent;
	DOM::Node _node = m_currentNode;
	DOM::Node _nodeNext = m_currentNode;
	bool _goingTowardsRootNode;
	while(1)
	{
		_nodeNext = getNextNode(_nodeNext, _goingTowardsRootNode);

		//second part : depending of the node, we choose what to do
		if(_nodeNext == 0)
			return;
		if(_nodeNext.nodeType() == DOM::Node::TEXT_NODE)
		{
			if((static_cast<DOM::CharacterData>(_nodeNext)).length() != 0)
			{//if we are in text, remove a letter
				kdDebug(25001)<< "KafkaHTMLPart::keyDelete() - one letter removed - 2" << endl;
				/**(static_cast<DOM::Text>(_nodeNext)).splitText(1);
				_nodeParent = _nodeNext.parentNode();
				_nodeParent.removeChild(_nodeNext);
				_nodeParent.applyChanges();*/
				DOM::DOMString nodeText = _nodeNext.nodeValue();
				DOM::DOMString textSplitted = nodeText.split(1);
				_nodeNext.setNodeValue(textSplitted);
				emit domNodeModified(_nodeNext);
				postprocessCursorPosition();
				return;
			}
			else
			{//if we are in an empty text
				kdDebug(25001)<< "KafkaHTMLPart::keyDelete() - deleting empty #text" << endl;
				_nodeParent = _nodeNext.parentNode();
				emit domNodeIsAboutToBeRemoved(_nodeNext, true);
				_nodeParent.removeChild(_nodeNext);
				_nodeParent.applyChanges();
				_nodeNext = _node;
				continue;
			}
		}
		else if(d->TagsDeletable.contains(_nodeNext.nodeName().string().upper()))
		{//if it is a deletable tag, remove it
			kdDebug(25001)<< "KafkaHTMLPart::keyDelete() - deleting a TagDeletable" << endl;
			_nodeParent = _nodeNext.parentNode();
			emit domNodeIsAboutToBeRemoved(_nodeNext, true);
			_nodeParent.removeChild(_nodeNext);
			//_nodeParent.normalize();
			normalize(_nodeParent);
			_nodeParent.applyChanges();
			postprocessCursorPosition();
			return;
		}
		else if(d->TagsTextDeletable.contains(_nodeNext.nodeName().string().upper()))
		{//if it is a Text deletable tag
			if(!_nodeNext.hasChildNodes())
			{//if we go up and we can delete the tag, delete it :)
				kdDebug(25001)<< "KafkaHTMLPart::keyDelete() - deleting a TagTextDeletable" << endl;
				emit domNodeIsAboutToBeRemoved(_nodeNext, true);
				_nodeNext.parentNode().removeChild(_nodeNext);
				_nodeNext = _node;
				continue;
			}
			else if(_goingTowardsRootNode && _nodeNext.childNodes().length() >= 1)
			{
				continue;
			}
		}
		else if(d->TagsTransparentelyDeletable.contains(_nodeNext.nodeName().string().upper()))
		{
			kdDebug(25001)<< "KafkaHTMLPart::keyDelete() - deleting a TagTransparentlyDeletable" << endl;
			_nodeParent = _nodeNext.parentNode();
			emit domNodeIsAboutToBeRemoved(_nodeNext, true);
			_nodeParent.removeChild(_nodeNext);
			//_nodeParent.normalize();
			normalize(_nodeParent);
			_nodeParent.applyChanges();
			_nodeNext = _node;
			continue;
		}
		else if(d->TagsMoveable.contains(_nodeNext.nodeName().string().upper()))
		{
			kdDebug(25001)<< "KafkaHTMLPart::keyDelete() - moving a MoveableTag" << endl;
			DOM::Node tmp;
			emit domNodeIsAboutToBeRemoved(_nodeNext, true);
			tmp = _nodeNext.parentNode().removeChild(_nodeNext);
			_node.parentNode().insertBefore(tmp,_node);
			_nodeNext.parentNode().applyChanges();
			emit domNodeInserted(tmp);
			continue;
		}
	}
}

void KafkaHTMLPart::keyBackspace()
{
	if(m_currentNode.isNull())
		return;
	if(m_currentNode.nodeType() == DOM::Node::TEXT_NODE && d->m_cursorOffset != 0)
	{//if we are in the middle of some text, we remove one letter
		kdDebug(25001)<< "KafkaHTMLPart::keyBackspace() - one letter removed - 1" << endl;
		DOM::DOMString nodeText = m_currentNode.nodeValue();
		DOM::DOMString textSplitted = nodeText.split(d->m_cursorOffset);
		nodeText.split(d->m_cursorOffset - 1);
		m_currentNode.setNodeValue(nodeText + textSplitted);
		m_currentNode.parentNode().applyChanges();
		d->m_cursorOffset--;
		emit domNodeModified(m_currentNode);
		postprocessCursorPosition();
		return;
	}
	if(d->TagsDeletable.contains(m_currentNode.nodeName().string().upper()) &&
		d->m_cursorOffset == 1)
	{//if we delete ourselves, which node will be m_currentNode??
		kdDebug(25001)<< "KafkaHTMLPart::keyBackspace() - deleting a TagDeletable - 2" << endl;
		DOM::Node _node = m_currentNode;
		bool b = false;
		while(1)
		{//try to find a previous node from which we can delete the node
			_node = getPrevNode(_node, b);
			if(_node == 0)
				break;
			if(_node.nodeType() == DOM::Node::TEXT_NODE)
			{
				m_currentNode = _node;
				emit domNodeGetFocus(m_currentNode);
				d->m_cursorOffset = (static_cast<DOM::CharacterData>(_node)).length();
				keyDelete();
				return;
			}
			if(d->TagsDeletable.contains(_node.nodeName().string().upper()))
			{
				m_currentNode = _node;
				emit domNodeGetFocus(m_currentNode);
				d->m_cursorOffset = 1;
				keyDelete();
				return;
			}
		}
		_node = m_currentNode;
		b = false;
		while(1)
		{//try to find a next node from which we can delete the node
			_node = getNextNode(_node, b);
			if(_node == 0)
				break;
			if(_node.nodeType() == DOM::Node::TEXT_NODE ||
				d->TagsDeletable.contains(_node.nodeName().string().upper()))
			{
				m_currentNode = _node;
				emit domNodeGetFocus(m_currentNode);
				d->m_cursorOffset = 0;
				keyBackspace();
				return;
			}
		}
		//here, there is no node right and left that can have the cursor focus
		_node = m_currentNode.parentNode();
		emit domNodeIsAboutToBeRemoved(m_currentNode, true);
		_node.removeChild(m_currentNode);
		m_currentNode = htmlDocument().createTextNode("");
		_node.appendChild(m_currentNode);
		emit domNodeInserted(m_currentNode);
		emit domNodeGetFocus(m_currentNode);

	}

	DOM::Node _nodeParent;
	DOM::Node _node = m_currentNode;
	DOM::Node _nodePrev = m_currentNode;
	bool _goingTowardsRootNode;
	bool deleteOldCurrentNode = (m_currentNode.nodeType() == DOM::Node::TEXT_NODE &&
		(static_cast<DOM::CharacterData>(m_currentNode)).length() == 0)?true:false;
	DOM::Node oldCurrentNode = m_currentNode;

	while(1)
	{
		_nodePrev = getPrevNode(_nodePrev, _goingTowardsRootNode);

		//second part : depending of the node, we choose what to do
		if(_nodePrev == 0)
			return;
		if(_nodePrev.nodeType() == DOM::Node::TEXT_NODE)
		{
			if((static_cast<DOM::CharacterData>(_nodePrev)).length() != 0)
			{//if we are in text, remove a letter
				kdDebug(25001)<< "KafkaHTMLPart::keyBackspace() - one letter removed - 2" << endl;
				/**DOM::Node letterToRemove;
				letterToRemove = (static_cast<DOM::Text>(_nodePrev)).splitText((static_cast<DOM::CharacterData>(_nodePrev)).length() - 1);
				letterToRemove = _nodePrev.parentNode().removeChild(letterToRemove);*/
				DOM::DOMString nodeText = _nodePrev.nodeValue();
				nodeText.split((static_cast<DOM::CharacterData>(_nodePrev)).length() - 1);
				_nodePrev.setNodeValue(nodeText);
				_nodePrev.parentNode().applyChanges();
				/**if(deleteOldCurrentNode) -> TODO:make it work in more general cases
				{//deleting the old empty txt where we were
					m_currentNode = _nodePrev;
					d->m_cursorOffset = (static_cast<DOM::CharacterData>(_nodePrev)).length();
					_nodeParent = _nodePrev.parentNode();
					_nodeParent.removeChild(_nodePrev);
				}*/
				postprocessCursorPosition();
				emit domNodeModified(_nodePrev);
				return;
			}
			else
			{//if we are in an empty text
				kdDebug(25001)<< "KafkaHTMLPart::keyBackspace() - deleting empty #text" << endl;
				_nodeParent = _nodePrev.parentNode();
				emit domNodeIsAboutToBeRemoved(_nodePrev, true);
				_nodeParent.removeChild(_nodePrev);
				_nodeParent.applyChanges();
				_nodePrev = _node;
				continue;
			}
		}
		else if(d->TagsDeletable.contains(_nodePrev.nodeName().string().upper()))
		{//if it is a deletable tag, remove it
			kdDebug(25001)<< "KafkaHTMLPart::keyBackspace() - deleting a TagDeletable" << endl;
			if((_nodePrev.previousSibling() != 0) && _nodePrev.previousSibling().nodeType() == DOM::Node::TEXT_NODE &&
				_node.nodeType() == DOM::Node::TEXT_NODE && _nodePrev.parentNode() == _nodePrev.previousSibling().parentNode())
			{//to avoid a bug due to normalize()
				m_currentNode = _nodePrev.previousSibling();
				emit domNodeGetFocus(m_currentNode);
				d->m_cursorOffset = (static_cast<DOM::CharacterData>(m_currentNode)).length();
			}
			_nodeParent = _nodePrev.parentNode();
			emit domNodeIsAboutToBeRemoved(_nodePrev, true);
			_nodeParent.removeChild(_nodePrev);
			//_nodeParent.normalize();
			normalize(_nodeParent);
			_nodeParent.applyChanges();
			postprocessCursorPosition();
			return;
		}
		else if(d->TagsTextDeletable.contains(_nodePrev.nodeName().string().upper()))
		{//if it is a Text deletable tag
			if(!_nodePrev.hasChildNodes())
			{//if we go up and we can delete the tag, delete it :)
				kdDebug(25001)<< "KafkaHTMLPart::keyBackspace() - deleting a TagTextDeletable" << endl;
				emit domNodeIsAboutToBeRemoved(_nodePrev, true);
				_nodePrev.parentNode().removeChild(_nodePrev);
				_nodePrev = _node;
				continue;
			}
			else if(_goingTowardsRootNode && _nodePrev.childNodes().length() >= 1)
			{
				continue;
			}
		}
		else if(d->TagsTransparentelyDeletable.contains(_nodePrev.nodeName().string().upper()))
		{
			kdDebug(25001)<< "KafkaHTMLPart::keyBackspace() - deleting a TagTransparentlyDeletable" << endl;
			if((_nodePrev.previousSibling() != 0) && _nodePrev.previousSibling().nodeType() == DOM::Node::TEXT_NODE &&
				_node.nodeType() == DOM::Node::TEXT_NODE && _nodePrev.parentNode() == _nodePrev.previousSibling().parentNode())
			{//to avoid a bug due to normalize()
				_node = _nodePrev.previousSibling();
				m_currentNode = _nodePrev.previousSibling();
				d->m_cursorOffset = (static_cast<DOM::CharacterData>(m_currentNode)).length();
				if(d->m_cursorOffset > 0)
				{
					d->m_cursorOffset--;
					postprocessCursorPosition();
					return;
				}
			}
			_nodeParent = _nodePrev.parentNode();
			emit domNodeIsAboutToBeRemoved(_nodePrev, true);
			_nodeParent.removeChild(_nodePrev);
			//_nodeParent.normalize();
			normalize(_nodeParent);
			_nodeParent.applyChanges();
			continue;
		}
		else if(d->TagsMoveable.contains(_nodePrev.nodeName().string().upper()))
		{
			kdDebug(25001)<< "KafkaHTMLPart::keyBackspace() - moving a MoveableTag" << endl;
			DOM::Node tmp;
			emit domNodeIsAboutToBeRemoved(_nodePrev, true);
			tmp = _nodePrev.parentNode().removeChild(_nodePrev);
			_nodePrev.parentNode().appendChild(tmp);
			_nodePrev.parentNode().applyChanges();
			emit domNodeInserted(tmp);
			continue;
		}
	}
}

DOM::Node KafkaHTMLPart::getNextNode(DOM::Node _node, bool &goingTowardsRootNode)
{
	if(_node == 0)
		return 0;
	if(_node.hasChildNodes() && goingTowardsRootNode == false &&
		!d->BlockingTags.contains(_node.nodeName().string().upper()) &&
		!d->TagsDeletable.contains(_node.nodeName().string().upper()))
	{//if we can descend to a child node, we do it
		kdDebug(25001)<< "KafkaHTMLPart::getNextNode() - descending from node : " <<
			_node.nodeName().string() << " to " <<
			_node.firstChild().nodeName().string() << endl;
		return _node.firstChild();
	}
	if(_node.nextSibling() != 0)
	{//else if there is a sibling, we move to it
		goingTowardsRootNode = false;
		kdDebug(25001)<< "KafkaHTMLPart::getNextNode() - going from node : " <<
			_node.nodeName().string() <<
		 	" to " << _node.nextSibling().nodeName().string() << endl;
		return _node.nextSibling();
	}
	if(_node.nextSibling() == 0)
	{//else if there is no sibling, we go up if we can
		goingTowardsRootNode = true;
		if(!d->BlockingTags.contains(_node.parentNode().nodeName().string().upper()))
		{
			kdDebug(25001)<< "KafkaHTMLPart::getNextNode() - going up from node : " <<
				_node.nodeName().string() << " to " <<
				_node.parentNode().nodeName().string() << endl;
			return _node.parentNode();
		}
		else
			return 0;
	}
	kdError()<< "KafkaHTMLPart::getNextNode() ERROR" << endl;
	return 0;
}

DOM::Node KafkaHTMLPart::getPrevNode(DOM::Node _node, bool &goingTowardsRootNode)
{
	if(_node == 0)
		return 0;
	if(_node.hasChildNodes() && goingTowardsRootNode == false &&
		!d->BlockingTags.contains(_node.nodeName().string().upper()) &&
		!d->TagsDeletable.contains(_node.nodeName().string().upper()))
	{//if we can descend to a child node, we do it
		kdDebug(25001)<< "KafkaHTMLPart::getPrevNode() - descending from node : " <<
			_node.nodeName().string() << " to " <<
			_node.lastChild().nodeName().string() << endl;
		return _node.lastChild();
	}
	if(_node.previousSibling() != 0)
	{//else if there is a sibling, we move to it
		goingTowardsRootNode = false;
		kdDebug(25001)<< "KafkaHTMLPart::getPrevNode() - going from node : " << _node.nodeName().string() <<
		 	" to " << _node.previousSibling().nodeName().string() << endl;
		return _node.previousSibling();
	}
	if(_node.previousSibling() == 0)
	{//else if there is no sibling, we go up if we can
		goingTowardsRootNode = true;
		if(!d->BlockingTags.contains(_node.parentNode().nodeName().string().upper()))
		{
			kdDebug(25001)<< "KafkaHTMLPart::getPrevNode() - going up from node : " <<
				_node.nodeName().string() << " to " <<
				_node.parentNode().nodeName().string() << endl;
			return _node.parentNode();
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
	if(m_currentNode == 0)
		return;
	DOM::Node _prevNextNode;
	DOM::Node _nextNode = m_currentNode;
	bool b = false;
	if(m_currentNode.nodeType() == DOM::Node::TEXT_NODE &&
		d->m_cursorOffset == 0)
	{
		while(1)
		{
			_prevNextNode = _nextNode;
			_nextNode = getPrevNode(_nextNode, b);
			if(_nextNode == 0)
				break;
			else if(_nextNode.nodeType() == DOM::Node::TEXT_NODE &&
				(static_cast<DOM::CharacterData>(_nextNode)).length() != 0)
			{
				m_currentNode = _nextNode;
				emit domNodeGetFocus(m_currentNode);
				d->m_cursorOffset = (static_cast<DOM::CharacterData>(_nextNode)).length();
				kdDebug(25001)<< "kafkaHTMLPart::postprocessCursorPosition() - new currentNode :"
					<< m_currentNode.nodeName().string() << endl;
				break;
			}
			else if(d->TagsDeletable.contains(_nextNode.nodeName().string().upper()))
				break;
			else
				continue;
		}
	}
	else if(d->TagsDeletable.contains(m_currentNode.nodeName().string().upper()))
	{
		if(d->m_cursorOffset == 0)
		{
			while(1)
			{
				_prevNextNode = _nextNode;
				_nextNode = getPrevNode(_nextNode, b);
				if(_nextNode == 0)
					break;
				else if(_nextNode.nodeType() == DOM::Node::TEXT_NODE &&
					(static_cast<DOM::CharacterData>(_nextNode)).length() != 0)
				{
					m_currentNode = _nextNode;
					emit domNodeGetFocus(m_currentNode);
					d->m_cursorOffset = (static_cast<DOM::CharacterData>(_nextNode)).length();
					kdDebug(25001)<< "kafkaHTMLPart::postprocessCursorPosition() - new currentNode :"
						<< m_currentNode.nodeName().string() << endl;
					break;
				}
				else if(d->TagsDeletable.contains(_nextNode.nodeName().string().upper()))
				{
					m_currentNode = _nextNode;
					emit domNodeGetFocus(m_currentNode);
					d->m_cursorOffset = 1;
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
				if(_nextNode == 0)
					break;
				else if(d->TagsDeletable.contains(_nextNode.nodeName().string().upper()))
					break;
				else if(_nextNode.nodeType() == DOM::Node::TEXT_NODE &&
					(static_cast<DOM::CharacterData>(_nextNode)).length() != 0)
				{
					m_currentNode = _nextNode;
					emit domNodeGetFocus(m_currentNode);
					d->m_cursorOffset = 0;
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

	DOM::Range tempRange(htmlDocument());

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
	if(event->innerNode() == 0)
	{
		kdDebug(25001)<< "KafkaHTMLPart::khtmlMousePressEvent() - event->innerNode().isNull() = " <<
		event->innerNode().isNull() << endl;
	}
	m_currentNode = event->innerNode();
	d->stuckCursorHorizontalPos = false;
	/** TEMPORARY before - i hope - khtml changes*/
	//look for the child node which has really the cursor focus
	if((d->TagsTextDeletable.contains(m_currentNode.nodeName().string().upper()) ||
		d->TagsDeletable.contains(m_currentNode.nodeName().string().upper()) ||
		d->BlockingTags.contains(m_currentNode.nodeName().string().upper())) &&
		m_currentNode.hasChildNodes())
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
	//theses nodes can't have the cursor focus : using Pythagore to get the
	//nearest node that can have the cursor focus
	if(d->TagsTextDeletable.contains(m_currentNode.nodeName().string().upper()) ||
		d->BlockingTags.contains(m_currentNode.nodeName().string().upper()))
	{
		if(!m_currentNode.hasChildNodes())
		{
			m_currentNode.appendChild(htmlDocument().createTextNode(""));
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
					tmpX = (_node.getRect().left() + _node.getRect().right())/2;
					tmpY = (_node.getRect().top() + _node.getRect().bottom())/2;
					if(((tmpX - event->x())*(tmpX - event->x()) +
						(tmpY - event->y())*(tmpY - event->y())) <
						((finalX - event->x())*(finalX - event->x()) +
						(finalY - event->y())*(finalY - event->y())))
					{//half pythagore too :p
						finalX = tmpX;
						finalY = tmpY;
						_tmp = _node;
					}
				}while(_node.nextSibling() != 0);
				_node = _tmp;
			}
			m_currentNode = _node;
			kdDebug(25001)<< "KafkaHTMLPart::khtmlMousePressEvent() - new CursorNode : " <<
			m_currentNode.nodeName().string() << endl;
		}
	}

	if(d->TagsDeletable.contains(m_currentNode.nodeName().string().upper()) ||
		d->BlockingTags.contains(m_currentNode.nodeName().string().upper()))
	{//TODO:allow the resizing of theses nodes
		kdDebug(25001)<< "KafkaHTMLPart::khtmlMousePressEvent() - upcoming selection"
			<< endl;
		d->m_cursorOffset = -1;
	}
	//if our node is a text, find out which offset is the better one
	if(m_currentNode.nodeType() == DOM::Node::TEXT_NODE)
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
	emit domNodeGetFocus(m_currentNode);
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
//	kdDebug(25001) << "KafkaHTMLPart::khtmlDrawContentsEvent(khtml::DrawContentsEvent *event)" << endl;
	try
	{
		if(m_currentNode.nodeType() == DOM::Node::TEXT_NODE ||
		d->TagsDeletable.contains(m_currentNode.nodeName().string().upper()))
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
	if(_node.nodeType() == DOM::Node::TEXT_NODE)
	{
		_node.getCursor(offset, _x, _y, height);
		return true;
	}
	else if(d->TagsDeletable.contains(_node.nodeName().string().upper()))
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
			else//if m_cursorOffset == 0
			{
				_x = _node.getRect().left() - 1;
				_y = _node.getRect().bottom() - 17;
				height = 16;
			}
		}
		return true;
	}
	return false;
}

void KafkaHTMLPart::paintCursor(QPainter *p)
{
	if(m_currentNode == 0)
		return;
	if(m_currentNode.nodeType() == DOM::Node::TEXT_NODE ||
		d->TagsDeletable.contains(m_currentNode.nodeName().string().upper()))
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
		return htmlDocument().createTextNode("");
	return htmlDocument().createElement(_nodename);
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
	DOM::Node _node = htmlDocument();
        bool b = false;
	if(_node == 0)
		kdDebug(25001)<< "KafkaHTMLPart::finishedLoading() ERROR : no htmlDocument()!" << endl;
	while(_node != 0)
	{
		_node = getNextNode(_node, b);
		if(_node == 0)
		{
			_node = htmlDocument();
			break;
		}
		if(_node.nodeType() == DOM::Node::TEXT_NODE || d->TagsDeletable.contains(_node.nodeName().string().upper()))
			break;
	}
	m_currentNode = _node;
	emit domNodeGetFocus(m_currentNode);
        d->m_cursorOffset = 0;
}

