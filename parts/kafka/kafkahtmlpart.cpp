/***************************************************************************
                              kafkahtmlpart.cpp
                             -------------------

    copyright            : (C) 2001 - The Kafka Team
                                   (C) 2003, 2004 - Nicolas Deschildre
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
#include "document.h"
#include "resource.h"
#include "quanta.h"
#include "quantaview.h"
#include "tagattributetree.h"
#include "kafkahtmlpart.moc"

class KafkaWidgetPrivate
{
public:
	KafkaWidgetPrivate()
	{
	}
	~KafkaWidgetPrivate()
	{
	}
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

KafkaWidget::KafkaWidget(QWidget *parent, QWidget *widgetParent, KafkaDocument *part,
	const char *name)
	: KHTMLPart(new KafkaHTMLView(this, widgetParent, name), parent, name),
	w(part)
{
	d = new KafkaWidgetPrivate();

	d->m_cursorOffset = 0;
	d->m_pressOffset = 0;
	d->m_releaseOffset = 0;
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
	//d->domdialog = new KafkaDOMTreeDialog(view(), this);
	//d->domdialog->show();
#endif
	//IMPORTANT:without him, no document() is created in khtmlPart
	begin();
	write("<html></html>");
	end();
}

KafkaWidget::~KafkaWidget()
{
}

void KafkaWidget::newDocument()
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
	putCursorAtFirstAvailableLocation();

}

void KafkaWidget::insertText(DOM::Node node, const QString &text, int position)
{
#ifdef LIGHT_DEBUG
	kdDebug(25001)<< "KafkaWidget::insertText text " << text << " pos " << position << endl;
#endif
	int focus = w->getAttrs(node)->chCurFoc();

	if(position < 0) return;//nothing to do if something is selected
	//if(focus == kNodeAttrs::no || !cbModified) return;//can't add text in this Node.
	if(position == 0 && node.nodeName().string().lower() == "body")
	{
		//SPECIFIC HTML code!!!
		//doesn't work!
		//putCursorAtFirstAvailableLocation();
		if(!node.firstChild().isNull() && node.firstChild().nodeType() == DOM::Node::TEXT_NODE)
		{
			node = m_currentNode = node.firstChild();
			position = 0;
		}

		if(position == 0 && node.nodeName().string().lower() == "body")
		{
			//We shouldn't go here...
			DOM::Text textNode = document().createTextNode(text);
			node.appendChild(textNode);
			m_currentNode = textNode;
			d->m_cursorOffset = text.length();
			emit domNodeInserted(textNode, false);
#ifdef LIGHT_DEBUG
			kdDebug(25001) << "KafkaWidget::insertText() - added text - 1" << endl;
#endif
			QTimer::singleShot(0, this, SLOT(slotDelayedSetCaretPosition()));
			return;
		}
	}

	if(focus == kNodeAttrs::textNode && node.nodeType() == DOM::Node::TEXT_NODE)
	{
		DOM::DOMString textNode = node.nodeValue();
		DOM::DOMString textSplitted = textNode.split(position);
		node.setNodeValue(textNode + text + textSplitted);
		d->m_cursorOffset += text.length();
		emit domNodeModified(node);
#ifdef LIGHT_DEBUG
		kdDebug(25001) << "KafkaWidget::insertText() - added text" << endl;
#endif
	}
	else if(position == 0)
	{
		DOM::Text textNode = document().createTextNode(text);
		DOM::Node parent = node.parentNode();
		parent.insertBefore(textNode, node);
		m_currentNode = textNode;
		d->m_cursorOffset = text.length();
		emit domNodeInserted(textNode, false);
#ifdef LIGHT_DEBUG
		kdDebug(25001) << "KafkaWidget::insertText() - added text - 2" << endl;
#endif
	}
	else if(position == 3 || (position == 1 && (focus == kNodeAttrs::singleNode ||
		focus == kNodeAttrs::singleNodeAndItself)))
	{
		DOM::Text textNode = document().createTextNode(text);
		DOM::Node parent = node.parentNode();
		parent.insertBefore(textNode, node.nextSibling());
		m_currentNode = textNode;
		d->m_cursorOffset = text.length();
		emit domNodeInserted(textNode, false);
#ifdef LIGHT_DEBUG
		kdDebug(25001) << "KafkaWidget::insertText() - added text - 3" << endl;
#endif
	}
	else if(position == 1)
	{
		DOM::Text textNode = document().createTextNode(text);
		if(!node.firstChild().isNull())
			node.insertBefore(textNode, node.firstChild());
		else
			node.appendChild(textNode);
		m_currentNode = textNode;
		d->m_cursorOffset = text.length();
		emit domNodeInserted(textNode, false);
#ifdef LIGHT_DEBUG
		kdDebug(25001) << "KafkaWidget::insertText() - added text - 4" << endl;
#endif
	}
	//document().updateRendering();
	QTimer::singleShot(0, this, SLOT(slotDelayedSetCaretPosition()));
}

void KafkaWidget::slotDelayedSetCaretPosition()
{
	setCaretPosition(m_currentNode, (long)d->m_cursorOffset);
	emit domNodeNewCursorPos(m_currentNode, d->m_cursorOffset);
}

void KafkaWidget::insertText(const QString &text, int position)
{
	insertText(m_currentNode, text, (position == -1 ? d->m_cursorOffset : position));
}


void KafkaWidget::normalize(DOM::Node _node)
{
#ifdef LIGHT_DEBUG
	kdDebug(25001)<< "KafkaWidget::normalize()" << endl;
#endif
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

void KafkaWidget::keyReturn()
{
	//WARNING : HTML-specific function
	DOM::Node text, text2, pDomNode, pDomNode2, brDomNode;
	int focus;
//	QTag *qTag;

	if(m_currentNode.isNull())
		return;
	if(!w->getAttrs(m_currentNode))
		return;

	focus = w->getAttrs(m_currentNode)->chCurFoc();
	if (focus == kNodeAttrs::textNode)
	{
		//First split if necessary the text
		if(d->m_cursorOffset == 0)
			text2 = m_currentNode;
		else if((unsigned)d->m_cursorOffset ==
			(static_cast<DOM::CharacterData>(m_currentNode)).length())
			text = m_currentNode;
		else
		{
			text = m_currentNode;
			text2 = (static_cast<DOM::Text>(m_currentNode)).splitText(d->m_cursorOffset);
			emit domNodeModified(m_currentNode);
			emit domNodeInserted(text2, false);
		}

		//Then look if we are in a area which can handle a P
		//and if it is ok and necessary, insert the current text in a P
		//TODO: Change a bit for the p so that it handle every case
		/**qTag = QuantaCommon::tagFromDTD(w->getCurrentDoc()->defaultDTD(),
			m_currentNode.parentNode().nodeName().string());
		pDomNode = kafkaCommon::hasParent(m_currentNode, "p");
		if(pDomNode.isNull() && qTag && qTag->isChild("p"))
		{
			if(!text.isNull())
			{
				emit domNodeIsAboutToBeRemoved(text, false);
				w->removeDomNode(text);
			}

			pDomNode = kafkaCommon::createDomNode("p", w->getCurrentDoc()->defaultDTD(),
				document());
			w->insertDomNode(pDomNode, m_currentNode.parentNode(),
				m_currentNode.nextSibling());
			emit domNodeInserted(pDomNode, false);

			if(!text.isNull())
			{
				w->::insertDomNode(text, pDomNode);
				emit domNodeInserted(text, false);
			}
		}

		//Then we insert either a P or a BR tag.
		if(qTag && qTag->isChild("p") && !pDomNode.isNull())
		{
			if(!text2.isNull())
			{
				emit domNodeIsAboutToBeRemoved(text2, false);
				w->::removeDomNode(text2);
			}

			pDomNode2 = kafkaCommon::createDomNode("p", w->getCurrentDoc()->defaultDTD(),
				document());
			w->insertDomNode(pDomNode2, pDomNode.parentNode(),
				pDomNode.nextSibling());
			emit domNodeInserted(pDomNode2, false);

			if(!text2.isNull())
			{
				w->insertDomNode(text2, pDomNode2);
				emit domNodeInserted(text2, false);
			}
			m_currentNode = pDomNode2.firstChild();
			d->m_cursorOffset = 0;
		}
		else
		{*/
			brDomNode = kafkaCommon::createDomNode("br", w->getCurrentDoc()->defaultDTD(),
				document());
			if(!text.isNull())
				w->insertDomNode(brDomNode, m_currentNode.parentNode(),
					text.nextSibling());
			else
				w->insertDomNode(brDomNode, m_currentNode.parentNode(),
					text2);
			emit domNodeInserted(brDomNode, false);
			if(!text2.isNull())
				m_currentNode = text2;
			else
				m_currentNode = brDomNode.nextSibling();
			d->m_cursorOffset = 0;
		/**}*/

		kdDebug(25001)<< "CURNODE : " << m_currentNode.nodeName().string() << ":"
			<< m_currentNode.nodeValue().string() << " : " << d->m_cursorOffset << endl;
		QTimer::singleShot(0, this, SLOT(slotDelayedSetCaretPosition()));
		//setCaretPosition(m_currentNode, (long)d->m_cursorOffset);
				kdDebug(25001)<< "CURNODE : " << m_currentNode.nodeName().string() << ":"
			<< m_currentNode.nodeValue().string() << " : " << d->m_cursorOffset << endl;
		//emit domNodeNewCursorPos(m_currentNode, d->m_cursorOffset);
		//		kdDebug(25001)<< "CURNODE : " << m_currentNode.nodeName().string() << ":"
		//	<< m_currentNode.nodeValue().string() << " : " << d->m_cursorOffset << endl;
		postprocessCursorPosition();
				kdDebug(25001)<< "CURNODE : " << m_currentNode.nodeName().string() << ":"
			<< m_currentNode.nodeValue().string() << " : " << d->m_cursorOffset << endl;
	}
}

bool KafkaWidget::eventFilter(QObject *, QEvent *event)
{
	bool forgetEvent = false;
	//tmp
	//DOM::Node attr, tmpNode;
	//end tmp

	if(event->type() == QEvent::FocusIn)
	{
#ifdef LIGHT_DEBUG
		kdDebug(25001) << "KafkaWidget::eventFilter() FocusIn" << endl;
#endif
		emit hasFocus(true);
	}

	if(event->type() == QEvent::FocusOut)
	{
#ifdef LIGHT_DEBUG
		kdDebug(25001) << "KafkaWidget::eventFilter() FocusOut" << endl;
#endif
		emit hasFocus(false);
	}

	if(event->type() == QEvent::KeyPress)
	{
		QKeyEvent *keyevent = static_cast<QKeyEvent *>(event);

		switch(keyevent->key())
		{
			case Key_Left:
#ifdef LIGHT_DEBUG
				kdDebug(25001) << "KafkaWidget::eventFilter() Left" << endl;
#endif
				//previousOffset(1);
				d->stuckCursorHorizontalPos = false;
				//forgetEvent = true;//to avoid the scrolling of the page
				break;

			case Key_Right:
#ifdef LIGHT_DEBUG
				kdDebug(25001) << "KafkaWidget::eventFilter() Right" << endl;
#endif
				//nextOffset(1);
				d->stuckCursorHorizontalPos = false;
				//forgetEvent = true;
				break;

			case Key_Backspace:
#ifdef LIGHT_DEBUG
				kdDebug(25001)<< "KafkaWidget::eventFilter() Backspace" << endl;
#endif
				keyBackspace();
				d->stuckCursorHorizontalPos = false;
				break;

			case Key_Delete:
#ifdef LIGHT_DEBUG
				kdDebug(25001)<< "KafkaWidget::eventFilter() Delete" << endl;
#endif
				keyDelete();
				d->stuckCursorHorizontalPos = false;
				break;

			case Key_Up:
#ifdef LIGHT_DEBUG
				kdDebug(25001)<< "KafkaWidget::eventFilter() Up" << endl;
#endif
				//keyUp();
				//forgetEvent = true;
				break;
			case Key_Down:
#ifdef LIGHT_DEBUG
				kdDebug(25001)<< "KafkaWidget::eventFilter() Down" << endl;
#endif
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
#ifdef LIGHT_DEBUG
				kdDebug(25001)<< "KafkaWidget::eventFilter() Return" << endl;
#endif
				keyReturn();
				d->stuckCursorHorizontalPos = false;
				break;
			case Key_Enter:
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
				if(m_currentNode.isNull())
				{
#ifdef LIGHT_DEBUG
					kdDebug(25001)<< "KafkaWidget::eventFilter() - DOM::Node NULL" << endl;
#endif
					break;
				}
				else if(w->getAttrs(m_currentNode)->chCurFoc() !=
					kNodeAttrs::no || m_currentNode.nodeName().string().lower() == "body")
				{
#ifdef LIGHT_DEBUG
					kdDebug(25001) << "KafkaWidget::eventFilter() Text - " <<
						keyevent->text() << endl;
#endif
					//if(( keyevent->state() & Qt::ShiftButton) || ( keyevent->state() == Qt::NoButton))
					if( keyevent->text().length() &&
						( !( keyevent->state() & ControlButton ) &&
						!( keyevent->state() & AltButton ) &&
						!( keyevent->state() & MetaButton ) ||
						( ( (keyevent->state()&ControlButton) | AltButton ) == (ControlButton|AltButton) ) ) &&
						( !keyevent->ascii() || keyevent->ascii() >= 32 || keyevent->text() == "\t" ) )
						insertText(keyevent->text(), -1);
					makeCursorVisible();
#ifdef HEAVY_DEBUG
				//w->coutLinkTree(baseNode, 2);
#endif
				}
				forgetEvent = true;
				d->stuckCursorHorizontalPos = false;
				break;
		}
	}

#ifdef LIGHT_DEBUG
	kdDebug(25001)<< "Current Offset : " << m_currentNode.nodeName().string()  << ":" <<
		d->m_cursorOffset << " (" << event->type() << ")" << endl;
#endif

	if(!forgetEvent)
		return false;
	else
		return true;
}

void KafkaWidget::keyDelete()
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
#ifdef LIGHT_DEBUG
		kdDebug(25001)<< "KafkaWidget::keyDelete() - one letter removed - 1" << endl;
#endif
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
#ifdef LIGHT_DEBUG
		kdDebug(25001)<< "KafkaWidget::keyDelete() - deleting a Node - 2" << endl;
#endif
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
#ifdef LIGHT_DEBUG
				kdDebug(25001)<< "KafkaWidget::keyDelete() - one letter" <<
					" removed - 2" << endl;
#endif
				DOM::DOMString nodeText = _nodeNext.nodeValue();
				DOM::DOMString textSplitted = nodeText.split(1);
				_nodeNext.setNodeValue(textSplitted);
				emit domNodeModified(_nodeNext);
				postprocessCursorPosition();
				return;
			}
			else
			{//if we are in an empty text
#ifdef LIGHT_DEBUG
				kdDebug(25001)<< "KafkaWidget::keyDelete() - deleting" <<
					"empty #text" << endl;
#endif
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
#ifdef LIGHT_DEBUG
			kdDebug(25001)<< "KafkaWidget::keyDelete() - deleting" <<
				" a Node" << endl;
#endif
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
			kdDebug(25001)<< "KafkaWidget::keyDelete() - deleting a" <<
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
				kdDebug(25001)<< "KafkaWidget::keyDelete() - deleting" <<
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

void KafkaWidget::keyBackspace()
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
#ifdef LIGHT_DEBUG
		kdDebug(25001)<< "KafkaWidget::keyBackspace() - one letter removed - 1" << endl;
#endif
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
#ifdef LIGHT_DEBUG
		kdDebug(25001)<< "KafkaWidget::keyBackspace() - deleting a TagDeletable - 2" << endl;
#endif
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
#ifdef LIGHT_DEBUG
				kdDebug(25001)<< "KafkaWidget::keyBackspace() - one" <<
					" letter removed - 2" << endl;
#endif
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
#ifdef LIGHT_DEBUG
				kdDebug(25001)<< "KafkaWidget::keyBackspace() - deleting" <<
					" empty #text" << endl;
#endif
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
#ifdef LIGHT_DEBUG
			kdDebug(25001)<< "KafkaWidget::keyBackspace() - deleting" <<
				" a Node" << endl;
#endif
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
			kdDebug(25001)<< "KafkaWidget::keyBackspace() - deleting a TagDeletable"
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
				kdDebug(25001)<< "KafkaWidget::keyBackspace() - deleting" <<
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

DOM::Node KafkaWidget::getNextNode(DOM::Node _node, bool &goingTowardsRootNode, bool skipParentNodes, bool dontBlock, DOM::Node _endNode)
{
	kNodeAttrs *attrs = 0L;

	if(_node == 0)
		return 0;
	attrs = w->getAttrs(_node);
	if(!attrs)
	{
		kdDebug(25001)<< "KafkaWidget::getNextNode() Attrs not found!"<< endl;
		return 0;
	}
	if(_node.hasChildNodes() && goingTowardsRootNode == false &&
		(attrs->ccanEnter() || dontBlock))
	{//if we can descend to a child node, we do it
#ifdef LIGHT_DEBUG
		kdDebug(25001)<< "KafkaWidget::getNextNode() - descending from node : " <<
			_node.nodeName().string() << " to " <<
			_node.firstChild().nodeName().string() << endl;
#endif
		if(_endNode == _node.firstChild())
			return 0;
		return _node.firstChild();
	}
	if(_node.nextSibling() != 0)
	{//else if there is a sibling, we move to it
		goingTowardsRootNode = false;
#ifdef LIGHT_DEBUG
		kdDebug(25001)<< "KafkaWidget::getNextNode() - going from node : " <<
			_node.nodeName().string() <<
		 	" to " << _node.nextSibling().nodeName().string() << endl;
#endif
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
			{
#ifdef LIGHT_DEBUG
				kdDebug(25001)<< "KafkaWidget::getNextNode() - going" <<
					" up from node : " << _node.nodeName().string() <<
				 	" to " << _node.parentNode().nodeName().string() << endl;
#endif
			}
			else
			{
#ifdef LIGHT_DEBUG
				kdDebug(25001)<< "KafkaWidget::getNextNode() - going" <<
					" up from node : " << _node.nodeName().string() <<
					" to an empty Node" << endl;
#endif
			}
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
	kdError()<< "KafkaWidget::getNextNode() ERROR" << endl;
	return 0;
}

DOM::Node KafkaWidget::getPrevNode(DOM::Node _node, bool &goingTowardsRootNode, bool skipParentNodes, bool dontBlock, DOM::Node _endNode)
{
	kNodeAttrs *attrs = 0L;

	if(_node == 0)
		return 0;
	attrs = w->getAttrs(_node);
	if(!attrs)
	{
		kdDebug(25001)<< "KafkaWidget::getPrevNode() Attrs not found!"<< endl;
		return 0;
	}
	if(_node.hasChildNodes() && goingTowardsRootNode == false &&
		(attrs->ccanEnter() || dontBlock))
	{//if we can descend to a child node, we do it
#ifdef LIGHT_DEBUG
		kdDebug(25001)<< "KafkaWidget::getPrevNode() - descending from node : " <<
			_node.nodeName().string() << " to " <<
			_node.lastChild().nodeName().string() << endl;
#endif
		if(_endNode == _node.lastChild())
			return DOM::Node();
		return _node.lastChild();
	}
	if(_node.previousSibling() != 0)
	{//else if there is a sibling, we move to it
		goingTowardsRootNode = false;
#ifdef LIGHT_DEBUG
		kdDebug(25001)<< "KafkaWidget::getPrevNode() - going from node : " <<
			_node.nodeName().string() <<
		 	" to " << _node.previousSibling().nodeName().string() << endl;
#endif
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
			{
#ifdef LIGHT_DEBUG
				kdDebug(25001)<< "KafkaWidget::getPrevNode() - going up from" <<
					" node : " << _node.nodeName().string() << " to " <<
					_node.parentNode().nodeName().string() << endl;
#endif
			}
			else
			{
#ifdef LIGHT_DEBUG
				kdDebug(25001)<< "KafkaWidget::getPrevNode() - going up from" <<
					" node : " << _node.nodeName().string() << " to an " <<
					"empty Node" << endl;
#endif
			}
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
	kdError()<< "KafkaWidget::getPrevNode() ERROR" << endl;
	return 0;
}

void KafkaWidget::makeCursorVisible(int , int )
{
	/**DOM::Range range;
	if(m_currentNode == 0)
		return;
	kdDebug(25001)<< "KafkaWidget::makeCursorVisible()" << endl;
	int X, Y, dummy;
	getCursor(m_currentNode, d->m_cursorOffset, X, Y, dummy);
	view()->ensureVisible (X, Y, xMargin, yMargin);*/
	//does not work... ???
	/**range = selection();
	//try{
		range.setStart(m_currentNode, d->m_cursorOffset);
	}
	catch(DOM::RangeException e)
	{
		//ignore
		kdDebug(25001)<< "KafkaWidget::makeCursorVisible() - ERROR " << e.code << endl;
		return;
	}
	catch(DOM::DOMException e)
	{
		kdDebug(25001)<< "KafkaWidget::makeCursorVisible() - ERROR " << e.code << endl;
	}*/
	//range.setEnd(m_currentNode, d->m_cursorOffset);
}

void KafkaWidget::postprocessCursorPosition()
{
	kNodeAttrs *attrs, *attrs2;
	if(m_currentNode == 0)
		return;
	attrs = w->getAttrs(m_currentNode);
	DOM::Node _prevNextNode;
	DOM::Node _nextNode = m_currentNode;
	bool b = false;

	if(!attrs)
	{
#ifdef LIGHT_DEBUG
		kdDebug(25001)<< "KafkaWidget::postprocessCursorPosition() - WARNING no Attrs!! " << endl;
#endif
		return;
	}

	if(attrs->chCurFoc() == kNodeAttrs::textNode &&
		d->m_cursorOffset == 0)
	{
		while(1)
		{
			_prevNextNode = _nextNode;
			_nextNode = kafkaCommon::getPrevDomNode(_nextNode);
			if(_nextNode.isNull())
				break;
			attrs2 = w->getAttrs(_nextNode);
			if(attrs2->chCurFoc() == kNodeAttrs::textNode &&
				(static_cast<DOM::CharacterData>(_nextNode)).length() != 0)
			{
				m_currentNode = _nextNode;
				d->m_cursorOffset = (static_cast<DOM::CharacterData>(_nextNode)).length();
				setCaretPosition(m_currentNode, (long)d->m_cursorOffset);
				emit domNodeNewCursorPos(m_currentNode, d->m_cursorOffset);
#ifdef LIGHT_DEBUG
				kdDebug(25001)<< "KafkaWidget::postprocessCursorPosition()" <<
					" - new currentNode :" <<
					m_currentNode.nodeName().string() << endl;
#endif
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
#ifdef LIGHT_DEBUG
					kdDebug(25001)<< "KafkaWidget::postprocessCursorPosition()" <<
						" - new currentNode :" << m_currentNode.nodeName().string() << endl;
#endif
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
#ifdef LIGHT_DEBUG
					kdDebug(25001)<< "KafkaWidget::postprocessCursorPosition()" <<
						" - new currentNode :" << m_currentNode.nodeName().string() << endl;
#endif
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
#ifdef LIGHT_DEBUG
					kdDebug(25001)<< "KafkaWidget::postprocessCursorPosition() " <<
						"- new currentNode :" << m_currentNode.nodeName().string() << endl;
#endif
					break;
				}
				else
					continue;
			}
		}
	}
	makeCursorVisible();
}

void KafkaWidget::khtmlMouseMoveEvent(khtml::MouseMoveEvent *event)
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

void KafkaWidget::khtmlMouseReleaseEvent(khtml::MouseReleaseEvent *event)
{
	KHTMLPart::khtmlMouseReleaseEvent(event);
	if(m_currentNode.isNull() || m_currentNode.nodeName().string().lower() == "#document")
	{
		m_currentNode = w->body;
		d->m_cursorOffset = 0;
		setCaretPosition(m_currentNode, (long)d->m_cursorOffset);
	}
	if(quantaApp->aTab)
		quantaApp->aTab->setCurrentNode(w->getNode(event->innerNode()));
}

void KafkaWidget::khtmlMousePressEvent(khtml::MousePressEvent *event)
{
	KHTMLPart::khtmlMousePressEvent(event);
	if(d->m_cursorOffset == 0 && !m_currentNode.isNull() &&
		m_currentNode.nodeName().string().lower() == "body")
		putCursorAtFirstAvailableLocation();
#ifdef HEAVY_DEBUG
	//d->domdialog->domview->showTree(document());
#endif
}

void KafkaWidget::khtmlDrawContentsEvent(khtml::DrawContentsEvent *event)
{
	KHTMLPart::khtmlDrawContentsEvent(event);
}

void KafkaWidget::getCurrentNode(DOM::Node &_currentNode, int &offset)
{
	_currentNode = m_currentNode;
	offset = d->m_cursorOffset;
}

void KafkaWidget::setCurrentNode(DOM::Node node, int offset)
{
	m_currentNode = node;
	d->m_cursorOffset = offset;
	makeCursorVisible();
	if(!m_currentNode.isNull() && m_currentNode.nodeName().string() != "#document")
		QTimer::singleShot(0, this, SLOT(slotDelayedSetCaretPosition()));
		//setCaretPosition(m_currentNode, (long)d->m_cursorOffset);
}

void KafkaWidget::putCursorAtFirstAvailableLocation()
{
	kNodeAttrs *attrs = 0L;
	DOM::Node node = document();
        bool b = false;

	while(!node.isNull())
	{
		node = kafkaCommon::getNextDomNode(node, b);
		if(node.isNull())
		{
			if(!w->body.isNull())
				node = w->body;
			else
				node = DOM::Node();
			break;
		}
		attrs = w->getAttrs(node);
		if(!attrs) return;
		if(node.nodeType() == DOM::Node::TEXT_NODE)
			break;
	}
	m_currentNode = node;
        d->m_cursorOffset = 0;
	QTimer::singleShot(0, this, SLOT(slotDelayedSetCaretPosition()));

#ifdef LIGHT_DEBUG
	if(!m_currentNode.isNull())
		kdDebug(25001)<< "KafkaWidget::putCursorAtFirstAvailableLocation() - " <<
			m_currentNode.nodeName().string() << endl;
#endif
}

void KafkaWidget::slotNewCursorPos(const DOM::Node &domNode, long offset)
{
	if(!w->isLoaded())
		return;

	m_currentNode = domNode;
	d->m_cursorOffset = (int)offset;
#ifdef LIGHT_DEBUG
	kdDebug(25001)<<"KafkaWidget::slotNewCursorPos() offset : " << d->m_cursorOffset << endl;
#endif
	if(quantaApp->aTab && quantaApp->view() &&
		quantaApp->view()->hadLastFocus() == QuantaView::kafkaFocus)
		quantaApp->aTab->setCurrentNode(w->getNode(domNode));
}
