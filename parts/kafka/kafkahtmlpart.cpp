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

#include <qfile.h>
#include <qpainter.h>
#include <qtextstream.h>
#include <qstringlist.h>
#include <qlayout.h>
#include <qmainwindow.h>
#include <qtimer.h>

#include <kapplication.h>
#include <kdebug.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <ktrader.h>
#include <klibloader.h>
#include <kparts/factory.h>
#include <dom/dom_text.h>
#include <dom/dom_exception.h>
#include <dom/dom_string.h>
#include <dom/dom2_range.h>
#include <khtml_events.h>

#include "kafkacommon.h"
#ifdef HEAVY_DEBUG
#include "domtreeview.h"
#include <qdialog.h>
#endif
#include "wkafkapart.h"
#include "nodeproperties.h"
#include "../../resource.h"
#include "../../quanta.h"
#include "../../treeviews/tagattributetree.h"
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
#ifdef HEAVY_DEBUG
	KafkaDOMTreeDialog *domdialog;
#endif
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

	// With the mix of Leo Savernik's caret Mode and the current editing
	// functions, it will be kind of VERY messy
	setCaretMode(true);
	connect(this, SIGNAL(caretPositionChanged(const DOM::Node &, long)),
		this, SLOT(slotNewCursorPos(const DOM::Node &, long)));
	setCaretDisplayPolicyNonFocused(KHTMLPart::CaretVisible);

	view()->setMouseTracking(true);
	view()->installEventFilter(this);

//for debug purposes, we add a DOM tree view
#ifdef HEAVY_DEBUG
	d->domdialog = new KafkaDOMTreeDialog(view(), this);
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
		"</head>\n"
		"<body>\n"
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
	kdDebug(25001)<< "KafkaHTMLPart::insertText text " << text << " pos " << position << endl;
	int focus = w->getAttrs(node)->chCurFoc();

	if(position < 0) return;//nothing to do if something is selected
	//if(focus == kNodeAttrs::no || !cbModified) return;//can't add text in this Node.
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
		} catch(DOM::DOMException e)
		{
			kdDebug(25001)<< "KafkaHTMLPart::insertText()" <<
				"ERROR - code : " << e.code << endl;
		}
	}
	else if(position == 0 && node.nodeName().string().lower() == "body")
	{
		//SPECIFIC HTML code!!!
		try
		{
			DOM::Text textNode = document().createTextNode(text);
			node.appendChild(textNode);
			m_currentNode = textNode;
			d->m_cursorOffset = text.length();
			emit domNodeInserted(textNode, false);
			kdDebug(25001) << "KafkaHTMLPart::insertText() - added text - 1" << endl;
		} catch(DOM::DOMException e)
		{
			kdDebug(25001)<< "KafkaHTMLPart::insertText()" <<
				"ERROR - code : " << e.code << endl;
		}
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
		} catch(DOM::DOMException e)
		{
			kdDebug(25001)<< "KafkaHTMLPart::insertText()" <<
				"ERROR - code : " << e.code << endl;
		}
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
		} catch(DOM::DOMException e)
		{
			kdDebug(25001)<< "KafkaHTMLPart::insertText()" <<
				"ERROR - code : " << e.code << endl;
		}
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
		} catch(DOM::DOMException e)
		{
			kdDebug(25001)<< "KafkaHTMLPart::insertText()" <<
				"ERROR - code : " << e.code << endl;
		}
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
		} catch(DOM::DOMException e)
		{
			kdDebug(25001)<< "KafkaHTMLPart::insertText()" <<
				"ERROR - code : " << e.code << endl;
		}
	}
			document().updateRendering();
	QTimer::singleShot(0, this, SLOT(slotDelayedSetCaretPosition()));
	//setCaretPosition(m_currentNode, (long)d->m_cursorOffset);
}

void KafkaHTMLPart::slotDelayedSetCaretPosition()
{
	setCaretPosition(m_currentNode, (long)d->m_cursorOffset);
	emit domNodeNewCursorPos(m_currentNode, d->m_cursorOffset);
}

void KafkaHTMLPart::insertText(const QString &text, int position)
{
	insertText(m_currentNode, text, (position == -1 ? d->m_cursorOffset : position));
}


void KafkaHTMLPart::normalize(DOM::Node _node)
{
	kdDebug(25001)<< "KafkaHTMLPart::normalize()" << endl;
	DOM::Node childNode = _node.firstChild();
	while(!childNode.isNull() && w->getAttrs(childNode)->chCurFoc() == kNodeAttrs::textNode)
	{
		while(!childNode.nextSibling().isNull() &&
			w->getAttrs(childNode.nextSibling())->chCurFoc() ==
			kNodeAttrs::textNode )
		{
			childNode.setNodeValue(childNode.nodeValue() +
				childNode.nextSibling().nodeValue());
			emit domNodeModified(childNode);
			emit domNodeIsAboutToBeRemoved(childNode.nextSibling(), true);
			_node.removeChild(childNode.nextSibling());
		}
		if(!childNode.nextSibling().isNull())
			childNode = childNode.nextSibling();
		else
			return;
	}
}

void KafkaHTMLPart::keyReturn()
{
	//Temporary HTML-specific function
	//TODO:CHANGE AND USE <P> instead of <BR>
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
			setCaretPosition(m_currentNode, (long)d->m_cursorOffset);
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
			setCaretPosition(m_currentNode, (long)d->m_cursorOffset);
			emit domNodeNewCursorPos(m_currentNode, d->m_cursorOffset);
		}
	}
}

bool KafkaHTMLPart::eventFilter(QObject *, QEvent *event)
{
	bool forgetEvent = false;
	//tmp
	//DOM::Node attr, tmpNode;
	//end tmp

	if(event->type() == QEvent::FocusIn)
	{
		kdDebug(25001) << "KafkaHTMLPart::eventFilter() FocusIn" << endl;
		emit hasFocus(true);
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
		emit hasFocus(false);
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
				//previousOffset(1);
				d->stuckCursorHorizontalPos = false;
				//forgetEvent = true;//to avoid the scrolling of the page
				break;

			case Key_Right:
				kdDebug(25001) << "KafkaHTMLPart::eventFilter() Right" << endl;
				//nextOffset(1);
				d->stuckCursorHorizontalPos = false;
				//forgetEvent = true;
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
				//keyUp();
				//forgetEvent = true;
				break;
			case Key_Down:
				kdDebug(25001)<< "KafkaHTMLPart::eventFilter() Down" << endl;
				//keyDown();
				//forgetEvent = true;
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
				{
					kdDebug(25001)<< "DOM::Node NULLLLLLLLLLLLLLLLLL" << endl;
					break;
				}
				else if(w->getAttrs(m_currentNode)->chCurFoc() !=
					kNodeAttrs::no || m_currentNode.nodeName().string().lower() == "body")
				{
					kdDebug(25001) << "KafkaHTMLPart::eventFilter() Text - " <<
						keyevent->text() << endl;
					if(/**keyevent->key() >= Key_Space &&*/ /**keyevent->key() !=
						Key_unknown && */!readOnly())
						insertText(keyevent->text(), -1);
					makeCursorVisible();
				}
				//kdDebug(25001)<< "NAME : " << m_currentNode.nodeName().string().lower() << endl;
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
		return false;
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
				setCaretPosition(m_currentNode, (long)d->m_cursorOffset);
				emit domNodeNewCursorPos(m_currentNode, d->m_cursorOffset);
				return;
			}
			if(attrs->chCurFoc() != kNodeAttrs::no &&
				attrs->chCurFoc() != kNodeAttrs::textNode)
			{
				m_currentNode = _node;
				d->m_cursorOffset = 1;
				keyDelete();
				setCaretPosition(m_currentNode, (long)d->m_cursorOffset);
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
				setCaretPosition(m_currentNode, (long)d->m_cursorOffset);
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
		setCaretPosition(m_currentNode, (long)d->m_cursorOffset);
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
				//merge the next DOM::Node if it is a text.
				//domNodeIsAboutToBeRemoved() had already done it in the Node tree.
				_nodeNext = _node.nextSibling();
				if(!_nodeNext.isNull() && _nodeNext.nodeType() == DOM::Node::TEXT_NODE)
				{
					_node.setNodeValue(_node.nodeValue() + _nodeNext.nodeValue());
					_nodeParent = _nodeNext.parentNode();
					_nodeParent.removeChild(_nodeNext);
				}
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
		setCaretPosition(m_currentNode, (long)d->m_cursorOffset);
		emit domNodeNewCursorPos(m_currentNode, d->m_cursorOffset);
		return;
	}
	if(attrs->chCurFoc() != kNodeAttrs::no && attrs->chCurFoc() != kNodeAttrs::textNode &&
		d->m_cursorOffset != 0)
	{//if we delete ourselves, which node will be m_currentNode??
		if(!attrs->cbDel())
			return;
		kdDebug(25001)<< "KafkaHTMLPart::keyBackspace() - deleting a TagDeletable - 2" << endl;
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
				setCaretPosition(m_currentNode, (long)d->m_cursorOffset);
				emit domNodeNewCursorPos(m_currentNode, d->m_cursorOffset);
				return;
			}
			if(attrs->chCurFoc() != kNodeAttrs::no && attrs->chCurFoc() !=
				kNodeAttrs::textNode)
			{
				m_currentNode = _node;
				d->m_cursorOffset = 1;
				keyDelete();
				setCaretPosition(m_currentNode, (long)d->m_cursorOffset);
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
				setCaretPosition(m_currentNode, (long)d->m_cursorOffset);
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
		setCaretPosition(m_currentNode, (long)d->m_cursorOffset);
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
				//merge the previous DOM::Node if it is a text.
				//domNodeIsAboutToBeRemoved() had already done it in the Node tree.
				_nodePrev = _node.previousSibling();
				if(!_nodePrev.isNull() && _nodePrev.nodeType() == DOM::Node::TEXT_NODE)
				{
					_nodePrev.setNodeValue(_nodePrev.nodeValue() + _node.nodeValue());
					_nodeParent = _nodePrev.parentNode();
					_nodeParent.removeChild(_node);
				}
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
			return DOM::Node();
		return _node.lastChild();
	}
	if(_node.previousSibling() != 0)
	{//else if there is a sibling, we move to it
		goingTowardsRootNode = false;
		kdDebug(25001)<< "KafkaHTMLPart::getPrevNode() - going from node : " <<
			_node.nodeName().string() <<
		 	" to " << _node.previousSibling().nodeName().string() << endl;
		if(_endNode == _node.previousSibling())
			return DOM::Node();
		return _node.previousSibling();
	}
	if(_node.previousSibling() == 0)
	{//else if there is no sibling, we go up if we can
		goingTowardsRootNode = true;
		if(_node.parentNode().isNull())
			return DOM::Node();
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
					return DOM::Node();
				return getPrevNode(_node.parentNode(), goingTowardsRootNode,
					skipParentNodes, dontBlock);
			}
			else
			{
				if(_endNode == _node.parentNode())
					return DOM::Node();
				return _node.parentNode();
			}
		}
		else
			return 0;
	}
	kdError()<< "KafkaHTMLPart::getPrevNode() ERROR" << endl;
	return 0;
}

void KafkaHTMLPart::makeCursorVisible(int , int )
{
	/**DOM::Range range;
	if(m_currentNode == 0)
		return;
	kdDebug(25001)<< "KafkaHTMLPart::makeCursorVisible()" << endl;
	int X, Y, dummy;
	getCursor(m_currentNode, d->m_cursorOffset, X, Y, dummy);
	view()->ensureVisible (X, Y, xMargin, yMargin);*/
	//does not work... ???
	/**range = selection();
	try{
		range.setStart(m_currentNode, d->m_cursorOffset);
	}
	catch(DOM::RangeException e)
	{
		//ignore
		kdDebug(25001)<< "KafkaHTMLPart::makeCursorVisible() - ERROR " << e.code << endl;
		return;
	}
	catch(DOM::DOMException e)
	{
		kdDebug(25001)<< "KafkaHTMLPart::makeCursorVisible() - ERROR " << e.code << endl;
	}*/
	//range.setEnd(m_currentNode, d->m_cursorOffset);
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
				setCaretPosition(m_currentNode, (long)d->m_cursorOffset);
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
					setCaretPosition(m_currentNode, (long)d->m_cursorOffset);
					emit domNodeNewCursorPos(m_currentNode,
						d->m_cursorOffset);
					kdDebug(25001)<< "kafkaHTMLPart::postprocessCursorPosition()" <<
						" - new currentNode :" << m_currentNode.nodeName().string() << endl;
					break;
				}
				else if(attrs2->chCurFoc() == kNodeAttrs::singleNode ||
					attrs2->chCurFoc() == kNodeAttrs::singleNodeAndItself ||
					(attrs2->chCurFoc() == kNodeAttrs::left && b))
				{
					m_currentNode = _nextNode;
					d->m_cursorOffset = 1;
					setCaretPosition(m_currentNode, (long)d->m_cursorOffset);
					emit domNodeNewCursorPos(m_currentNode, d->m_cursorOffset);
					kdDebug(25001)<< "kafkaHTMLPart::postprocessCursorPosition()" <<
						" - new currentNode :" << m_currentNode.nodeName().string() << endl;
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
					setCaretPosition(m_currentNode, (long)d->m_cursorOffset);
					emit domNodeNewCursorPos(m_currentNode, d->m_cursorOffset);
					kdDebug(25001)<< "kafkaHTMLPart::postprocessCursorPosition() " <<
						"- new currentNode :" << m_currentNode.nodeName().string() << endl;
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
	KHTMLPart::khtmlMouseMoveEvent(event);
}

void KafkaHTMLPart::khtmlMouseReleaseEvent(khtml::MouseReleaseEvent *event)
{
	KHTMLPart::khtmlMouseReleaseEvent(event);
	if(m_currentNode.isNull() || m_currentNode.nodeName().string().lower() == "#document")
	{
		m_currentNode = w->body;
		d->m_cursorOffset = 0;
		setCaretPosition(m_currentNode, (long)d->m_cursorOffset);
	}
	if(quantaApp->aTab)
		quantaApp->aTab->setCurrentNode(w->searchCorrespondingNode(event->innerNode()));
}

void KafkaHTMLPart::khtmlMousePressEvent(khtml::MousePressEvent *event)
{
	KHTMLPart::khtmlMousePressEvent(event);
}

void KafkaHTMLPart::khtmlDrawContentsEvent(khtml::DrawContentsEvent *event)
{
	KHTMLPart::khtmlDrawContentsEvent(event);
}

DOM::Node KafkaHTMLPart::createNode(const QString &NodeName)
{
	//this will change with the futur DTDs support
	DOM::Node dn;
        DOM::DOMString _nodename(NodeName);
	if(NodeName == "TEXT")
		return document().createTextNode("");
	try
	{
		dn =  document().createElement(_nodename);
	} catch(DOM::DOMException e)
	{
		return DOM::Node();
	}
	return dn;
}

void KafkaHTMLPart::showDomTree()
{
#ifdef HEAVY_DEBUG
	d->domdialog->show();
#endif
}

void KafkaHTMLPart::getCurrentNode(DOM::Node &_currentNode, int &offset)
{
	_currentNode = m_currentNode;
	offset = d->m_cursorOffset;
}

void KafkaHTMLPart::setCurrentNode(DOM::Node node, int offset)
{
	m_currentNode = node;
	d->m_cursorOffset = offset;
	makeCursorVisible();
	if(!m_currentNode.isNull() && m_currentNode.nodeName().string() != "#document")
		setCaretPosition(m_currentNode, (long)d->m_cursorOffset);
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
		if(attrs->chCurFoc() != kNodeAttrs::no || _node.nodeName().string() == "#text")
			break;
	}
	m_currentNode = _node;
        d->m_cursorOffset = 0;
	//if(!m_currentNode.isNull() && m_currentNode.nodeName().string() != "#document")
		setCaretPosition(m_currentNode, (long)d->m_cursorOffset);
	emit domNodeNewCursorPos(m_currentNode, d->m_cursorOffset);
}

void KafkaHTMLPart::slotNewCursorPos(const DOM::Node &domNode, long offset)
{
	m_currentNode = domNode;
	d->m_cursorOffset = (int)offset;
	kdDebug(25001)<<"kafkaHTMLPart::slotNewCursorPos() offset : " << d->m_cursorOffset << endl;
	if(quantaApp->aTab)
		quantaApp->aTab->setCurrentNode(w->searchCorrespondingNode(domNode));
}
