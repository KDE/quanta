/***************************************************************************
                              kafkahtmlpart.cpp
                             -------------------
 
    copyright            : (C) 2001 - The Kafka Team
                           (C) 2003, 2004 - Nicolas Deschildre
    email                : kde-kafka@master.kde.org && ndeschildre@kdewebdev.org
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
#include <qtooltip.h>
#include <qpopupmenu.h> 

#include <kdebug.h>
#include <klocale.h>
#include <khtmlview.h>
#include <kmessagebox.h>
#include <ktrader.h>
#include <klibloader.h>
#include <kparts/factory.h>
#include <dom/dom_node.h>
#include <dom/dom_text.h>
#include <dom/dom_exception.h>
#include <dom/dom_string.h>
#include <dom/dom2_range.h>
#include <khtml_events.h>
//#include <khtml_part.h>

#include "kafkacommon.h"
#ifdef HEAVY_DEBUG
#include "domtreeview.h"
#include <qdialog.h>
#endif
#include "wkafkapart.h"
#include "undoredo.h"
#include "nodeproperties.h"
#include "tagaction.h"
#include "tagactionset.h"
#include "document.h"
#include "resource.h"
#include "quantacommon.h"
#include "quanta.h"
#include "quantaview.h"
#include "tagattributetree.h"
#include "tagactionmanager.h"
#include "tagactionset.h"
#include "cursors.h"

#include "viewmanager.h"

class KafkaWidgetPrivate
{
public:
    KafkaWidgetPrivate()
    {}
    ~KafkaWidgetPrivate()
    {}
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
        : KHTMLPart(widgetParent, name, parent, name),
        w(part)
{
    m_contextPopupMenu = new QPopupMenu();
    
    d = new KafkaWidgetPrivate();

    d->m_cursorOffset = 0;
    d->m_pressOffset = 0;
    d->m_releaseOffset = 0;
    d->stuckCursorHorizontalPos = false;

    m_modifs = 0L;

    // With the mix of Leo Savernik's caret Mode and the current editing
    // functions, it will be kind of VERY messy
    setCaretMode(true);
    connect(this, SIGNAL(caretPositionChanged(const DOM::Node &, long)),
            this, SLOT(slotNewCursorPos(const DOM::Node &, long)));
    setCaretDisplayPolicyNonFocused(KHTMLPart::CaretVisible);
    
    connect(this, SIGNAL(popupMenu(const QString&, const QPoint&)),
            this, SLOT(slotContextMenuRequested(const QString&, const QPoint&)));

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
{}

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

}

void KafkaWidget::insertText(DOM::Node node, const QString &text, int position)
{
#ifdef LIGHT_DEBUG
    kdDebug(25001)<< "KafkaWidget::insertText text " << text << " pos " << position << endl;
#endif

    int focus;
    kNodeAttrs *attrs = w->getAttrs(node);

    if(!attrs)
        return;

    focus = attrs->chCurFoc();

    if(position < 0)
        return;//nothing to do if something is selected
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
            emit domNodeInserted(textNode, false, m_modifs);
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
        emit domNodeModified(node, m_modifs);
#ifdef LIGHT_DEBUG

        kdDebug(25001) << "KafkaWidget::insertText() - added text" << endl;
#endif

    }
    else if(position == 0)
    {
        DOM::Text textNode = document().createTextNode(text);
        DOM::Node parent = node.parentNode();
//FIXME: Andras: safety checks, as parent  can be null. Maybe it just hides the error...
        if (!parent.isNull())
          parent.insertBefore(textNode, node);
        else
          node.appendChild(textNode);
        m_currentNode = textNode;
        d->m_cursorOffset = text.length();
        emit domNodeInserted(textNode, false, m_modifs);
#ifdef LIGHT_DEBUG

        kdDebug(25001) << "KafkaWidget::insertText() - added text - 2" << endl;
#endif

    }
    else if(position == 3 || (position == 1 && (focus == kNodeAttrs::singleNodeAndItself)))
    {
        DOM::Text textNode = document().createTextNode(text);
        DOM::Node parent = node.parentNode();
//FIXME: Andras: safety checks, as parent and node.nextSibling can be null. Maybe it just hides the error...
//Also it seems that position can be 3 and node is "body". See bug 112733.
        if (node.nodeName().string().lower() != "body" && !parent.isNull())
        {
          if (!node.nextSibling().isNull())
            parent.insertBefore(textNode, node.nextSibling());
          else
            parent.insertBefore(textNode, node);
        }
        else
          node.appendChild(textNode);
        m_currentNode = textNode;
        d->m_cursorOffset = text.length();
        emit domNodeInserted(textNode, false, m_modifs);
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
        emit domNodeInserted(textNode, false, m_modifs);
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

//FIXME: Andras: getAttrs() can sometimes return NULL and causes a crash. No idea why and what it means though.
    DOM::Node childNode = _node.firstChild();
    while(!childNode.isNull())
    {
        if(w->getAttrs(childNode) && w->getAttrs(childNode)->chCurFoc() == kNodeAttrs::textNode)
        {
            while(!childNode.nextSibling().isNull() &&
                    w->getAttrs(childNode.nextSibling()) && w->getAttrs(childNode.nextSibling())->chCurFoc() ==
                    kNodeAttrs::textNode )
            {
                childNode.setNodeValue(childNode.nodeValue() +
                                       childNode.nextSibling().nodeValue());
                emit domNodeModified(childNode, m_modifs);
                emit domNodeIsAboutToBeRemoved(childNode.nextSibling(), true, m_modifs);
                _node.removeChild(childNode.nextSibling());
            }
        }
        childNode = childNode.nextSibling();
    }
}

void KafkaWidget::keyReturn(bool specialPressed)
{
    //WARNING : HTML-specific function
    DOM::Node text, text2, pDomNode, pDomNode2, brDomNode, brDomNode2, tmp, PNode, emptyText;
    int focus;
    //	QTag *qTag;
    bool childOfP;
    //kNodeAttrs *props;

    if(m_currentNode.isNull())
        return;
    if(!w->getAttrs(m_currentNode))
        return;

    focus = w->getAttrs(m_currentNode)->chCurFoc();
    if (focus == kNodeAttrs::textNode)
    {
        //First determine if the current Node is a child of a P
        childOfP = false;
        tmp = m_currentNode;
        while(!tmp.isNull())
        {
            if(tmp.nodeName().string().lower() == "p")
            {
                PNode = tmp;
                childOfP = true;
                break;
            }
            tmp = tmp.parentNode();
        }

        //Then split if necessary the text
        if((static_cast<DOM::CharacterData>(m_currentNode)).length() == 0)
            text = m_currentNode;
        else if(d->m_cursorOffset <= 0)
            text2 = m_currentNode;
        else if((unsigned)d->m_cursorOffset >=
                (static_cast<DOM::CharacterData>(m_currentNode)).length())
            text = m_currentNode;
        else
        {
            text = m_currentNode;
            text2 = (static_cast<DOM::Text>(m_currentNode)).splitText(d->m_cursorOffset);
            emit domNodeModified(m_currentNode, m_modifs);
            emit domNodeInserted(text2, false, m_modifs);
        }

        if(!specialPressed)
        {
            if(childOfP)
            {}
            else
            {}
        }
        else
        {
            if(childOfP)
            {}
            else
            {}
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
        emit domNodeInserted(brDomNode, false, m_modifs);
        if(!text2.isNull())
            m_currentNode = text2;
        else
        {
            if(!brDomNode.nextSibling().isNull())
                m_currentNode = brDomNode.nextSibling();
            if(!brDomNode.nextSibling().isNull() && brDomNode.nextSibling().nextSibling().isNull())
            {
                //TEMP before the webcore caret.
                brDomNode2 = kafkaCommon::createDomNode("br", w->getCurrentDoc()->defaultDTD(),
                                                        document());
                if(!brDomNode.nextSibling().isNull())
                    w->insertDomNode(brDomNode2, m_currentNode.parentNode(),
                                     DOM::Node());

                emit domNodeInserted(brDomNode2, false, m_modifs);
                m_currentNode = brDomNode;
            }

        }
        d->m_cursorOffset = 0;

    }
    else if( m_currentNode.nodeName().string().lower() == "br")
    {
        brDomNode = kafkaCommon::createDomNode("br", w->getCurrentDoc()->defaultDTD(),
                                               document());
        w->insertDomNode(brDomNode, m_currentNode.parentNode(),
                         brDomNode.nextSibling());
        emit domNodeInserted(brDomNode, false, m_modifs);
        m_currentNode = brDomNode;
        d->m_cursorOffset = 0;
    }

#ifdef HEAVY_DEBUG

    kdDebug(25001)<< "CURNODE : " << m_currentNode.nodeName().string() << ":"
    << m_currentNode.nodeValue().string() << " : " << d->m_cursorOffset << endl;
    QTimer::singleShot(0, this, SLOT(slotDelayedSetCaretPosition()));
    kdDebug(25001)<< "CURNODE : " << m_currentNode.nodeName().string() << ":"
    << m_currentNode.nodeValue().string() << " : " << d->m_cursorOffset << endl;
    //emit domNodeNewCursorPos(m_currentNode, d->m_cursorOffset);
    //		kdDebug(25001)<< "CURNODE : " << m_currentNode.nodeName().string() << ":"
    //	<< m_currentNode.nodeValue().string() << " : " << d->m_cursorOffset << endl;
    //postprocessCursorPosition();
    kdDebug(25001)<< "CURNODE : " << m_currentNode.nodeName().string() << ":"
    << m_currentNode.nodeValue().string() << " : " << d->m_cursorOffset << endl;
#endif

    QTimer::singleShot(0, this, SLOT(slotDelayedSetCaretPosition()));
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

        //Create a new NodeModifsSet where the changes will be logged.
        m_modifs = new NodeModifsSet();

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
            if(!m_currentNode.isNull() && w->getAttrs(m_currentNode) &&
                    w->getAttrs(m_currentNode)->chCurFoc() != kNodeAttrs::no)
            {
                // @todo check tab settings in Quanta
                if(hasSelection())
                    removeSelection();
                insertText("    ", -1);
                makeCursorVisible();
            }
            forgetEvent = true;
            d->stuckCursorHorizontalPos = false;
            break;
        case Key_BackTab:
            break;
        case Key_Return:
        case Key_Enter:
#ifdef LIGHT_DEBUG

            kdDebug(25001)<< "KafkaWidget::eventFilter() Return" << endl;
#endif
            if(hasSelection())
                removeSelection();
                
            applyQueuedToggableTagActions();                
            
            keyReturn(keyevent->state() & ControlButton);
            d->stuckCursorHorizontalPos = false;
            break;
        case Key_Insert:
            break;
        case Key_Pause:
#ifdef HEAVY_DEBUG

            kafkaCommon::coutTree(baseNode, 2);
            kafkaCommon::coutDomTree(document(), 2);
            w->coutLinkTree(baseNode, 2);
#endif

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
            else if(w->getAttrs(m_currentNode) &&
                    w->getAttrs(m_currentNode)->chCurFoc() != kNodeAttrs::no ||
                    m_currentNode.nodeName().string().lower() == "body")
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
                {
                    if(hasSelection())
                        removeSelection();
                
                    applyQueuedToggableTagActions();                

                    insertText(keyevent->text(), -1);
                }
                makeCursorVisible();
#ifdef HEAVY_DEBUG
                //w->coutLinkTree(baseNode, 2);
#endif

            }
            forgetEvent = true;
            d->stuckCursorHorizontalPos = false;
            break;
        }

        //Submit the modifs to the undoRedo system.
        ViewManager::ref()->activeDocument()->docUndoRedo->addNewModifsSet(m_modifs, undoRedo::KafkaModif, 0, qConfig.replaceAccented);
        m_modifs = 0L;
    }

#ifdef LIGHT_DEBUG
    kdDebug(25001)<< "Current Offset : " << m_currentNode.nodeName().string()  << ":" <<
    d->m_cursorOffset << " (" << event->type() << ")" << endl;
#endif

    return forgetEvent;
}

void KafkaWidget::slotContextMenuRequested(const QString& /*url*/, const QPoint& point)
{
    TagActionManager::self()->fillWithTagActions(m_contextPopupMenu, nodeUnderMouse());

    if(m_contextPopupMenu->count() != 0)
        m_contextPopupMenu->popup(point);
}


#if 0
void KafkaWidget::keyDeleteNodes(DOM::Node &startNode, long &offset, bool backspace)
{
    DOM::Node domNode = startNode, nextNode;
    kNodeAttrs *attrs;
    long nextOffset;
    DOM::DOMString nodeText, textSplitted;

    while(!domNode.isNull())
    {
        //Start by getting the attributes of the Node
        attrs = w->getAttrs(domNode);
        if(!attrs)
        {
#ifdef LIGHT_DEBUG
            kdDebug(25001)<<"KafkaWidget::keyDeleteNodes() - ERROR KNodeAttrs not found!"
            << endl;
#endif

            break;
        }

        //Get the next Location
        nextNode = domNode;
        nextOffset = offset;
        if(backspace)
            getPrevNodeRangeSpecs(nextNode, offset, blok! );
        else
            getNextNodeRangeSpecs(nextNode, offset);

        //Nodes that can't be deleted stop the cursor (e.g. TBODY)
        if(!attrs->cbDel())
            break;

        //If we are in some text, and a letter can be deleted, delete it.
        if(domNode.nodeType() == DOM::Node::TEXT_NODE && ((backspace && offset != 0)) ||
                (!backspace && offset != (static_cast<DOM::CharacterData>(domNode)).length()))
            )
        {
            nodeText = domNode.nodeValue();
            textSplitted = nodeText.split(backspace?offset:offset + 1);
            nodeText.split(backspace?offset - 1:offset);
            domNode.setNodeValue(nodeText + textSplitted);
            //m_currentNode.parentNode().applyChanges();
            emit domNodeModified(domNode);
            //postprocessCursorPosition();
            break;
        }

        //If we are in an empty text (shoudn't occur), delete it
        if(domNode.nodeType()
                    == DOM::Node::TEXT_NODE &&
                    (static_cast<DOM::CharacterData>(domNode)).length() == 0)
        {
            emit domNodeIsAboutToBeRemoved(domNode, true);
            domNode.parentNode().removeChild(domNode);
            domNode = nextNode;
            continue;
        }

        //If we are in an empty Node (Inline), delete it
        if(domNode.nodeType()
                    == DOM::Node::ELEMENT_NODE && offset == 0 && !domNode.hasChildNodes())
        {

            continue;
        }

        //If the current Node is an empty Text, delete it
        if()
    {
        continue;
    }

    //If the current Node is an empty Node (kNodeAttrs::singleNodeAndItself)), delete it
    if()
    {
        return;
    }

    //If the current Node is an empty Node (Inline) delete it
    if()
    {
        continue;
    }
}


}
#endif

void KafkaWidget::keyDelete()
{
    kNodeAttrs *attrs, *attrsTmp;
    int focus, childPosition;
    DOM::Node _nodeParent, _node, _nodeNext, temp, tempParent, nextSibling, nodeNextNext;
    DOM::Node toplevelBlock, toplevelBlock2, startNode, endNode, startNode2, endNode2;
    DOM::Node childOfCommonParent, childOfCommonParent2, commonParent;
    bool _goingTowardsRootNode, isParent, singleNodeDeleted, nextIsBlock, startNode2IsNotInline;

    if(hasSelection())
    {
        removeSelection();
        return;
    }
    
    if(m_currentNode.isNull())
        return;
    attrs = w->getAttrs(m_currentNode);
    if(!attrs)
        return;

    //OLD PART, TO BE REMOVED or #ifdef'ed
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
        emit domNodeModified(m_currentNode, m_modifs);
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
            if(_node == 0)
                break;
            attrs = w->getAttrs(_node);
            if(attrs && attrs->chCurFoc() == kNodeAttrs::textNode)
            {
                m_currentNode = _node;
                d->m_cursorOffset =
                    (static_cast<DOM::CharacterData>(_node)).length();
                keyDelete();
                setCaretPosition(m_currentNode, (long)d->m_cursorOffset);
                emit domNodeNewCursorPos(m_currentNode, d->m_cursorOffset);
                return;
            }
            if(attrs && attrs->chCurFoc() != kNodeAttrs::no &&
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
            if(_node == 0)
                break;
            attrs = w->getAttrs(_node);
            if(attrs && attrs->chCurFoc() != kNodeAttrs::no)
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
        emit domNodeIsAboutToBeRemoved(m_currentNode, true, m_modifs);
        _node.removeChild(m_currentNode);
        m_currentNode = document().createTextNode("");
        _node.appendChild(m_currentNode);
        emit domNodeInserted(m_currentNode, false, m_modifs);
        setCaretPosition(m_currentNode, (long)d->m_cursorOffset);
        emit domNodeNewCursorPos(m_currentNode, d->m_cursorOffset);
    }

    //Beginning of the actual keyDelete
    _node = m_currentNode;
    _goingTowardsRootNode = false;
    singleNodeDeleted = false;
    _nodeNext = getNextNode(_node, _goingTowardsRootNode);
    while(!_nodeNext.isNull())
    {
#ifdef LIGHT_DEBUG
        kdDebug(25001)<< "KafkaWidget::keyDelete() - currentNode: " <<
        _nodeNext.nodeName().string() << endl;
#endif

        attrs = w->getAttrs(_nodeNext);

        //If this Node can't be deleted, we stop here.
        if(!attrs || !attrs->cbDel())
            return;

        //If we are in a TEXT node, we remove a letter
        if(attrs->chCurFoc() == kNodeAttrs::textNode)
        {
            if((static_cast<DOM::CharacterData>(_nodeNext)).length() != 0)
            {
#ifdef LIGHT_DEBUG
                kdDebug(25001)<< "KafkaWidget::keyDelete() - one letter" <<
                " removed - 2" << endl;
#endif

                DOM::DOMString nodeText = _nodeNext.nodeValue();
                DOM::DOMString textSplitted = nodeText.split(1);
                _nodeNext.setNodeValue(textSplitted);
                emit domNodeModified(_nodeNext, m_modifs);
                postprocessCursorPosition();
                normalize(_nodeNext.parentNode());
                break;
            }
            else
            {//if we are in an empty text, delete it
#ifdef LIGHT_DEBUG
                kdDebug(25001)<< "KafkaWidget::keyDelete() - deleting" <<
                "empty #text" << endl;
#endif

                _nodeParent = _nodeNext.parentNode();
                //If this empty text contains the cursor, change node to its parent.
                if(_nodeNext == _node && _nodeParent.firstChild() == _nodeNext &&
                        _nodeNext.nextSibling().isNull())
                {
                    _node = _nodeParent;
                    //d->m_cursorOffset = -2;
                }
                emit domNodeIsAboutToBeRemoved(_nodeNext, true, m_modifs);
                _nodeParent.removeChild(_nodeNext);
                singleNodeDeleted = true;
                _nodeParent.applyChanges();
                _nodeNext = _node;
            }
        }
        //Else if the current Node is a BLOCK which can be entered/leaved e.g. H1
        else if(attrs->chCurFoc() == kNodeAttrs::blockNode)
        {
            //First look if it is one of _node's parent
            isParent = false;
            temp = _node;
            while(!temp.isNull())
            {
                if(_nodeNext == temp)
                    isParent = true;
                temp = temp.parentNode();
            }

            //1 - Locate the toplevel blocks
            temp = _nodeNext;
            if(isParent)
            {
                toplevelBlock = temp;
                while(temp.parentNode().lastChild() == temp && w->getAttrs(temp.parentNode()) &&
                        w->getAttrs(temp.parentNode())->chCurFoc() == kNodeAttrs::blockNode)
                    temp = temp.parentNode();
                childOfCommonParent = temp;
                temp = temp.nextSibling();
            }
            if(temp.isNull())
                break;
            childOfCommonParent2 = temp;
            commonParent = temp.parentNode();
            attrsTmp = w->getAttrs(temp);
            nextIsBlock = (attrsTmp && attrsTmp->chCurFoc() == kNodeAttrs::blockNode);
            while(!temp.isNull() && temp.hasChildNodes() && w->getAttrs(temp.firstChild()) &&
                    w->getAttrs(temp.firstChild())->chCurFoc() == kNodeAttrs::blockNode)
                temp = temp.firstChild();
            toplevelBlock2 = temp;

            //2 - Determine the Nodes which could be moved
            if(!toplevelBlock.isNull() && toplevelBlock.hasChildNodes())
                endNode = toplevelBlock.lastChild();
            else if(!childOfCommonParent2.isNull() && !childOfCommonParent2.previousSibling().isNull())
                endNode = childOfCommonParent2.previousSibling();
            temp = endNode;
            while(!temp.isNull() && !temp.previousSibling().isNull() &&
                    ((kafkaCommon::isInline(temp) && (temp.previousSibling().isNull() ||
                                                      kafkaCommon::isInline(temp.previousSibling()))) /**||
                                                               				(!isInline(temp) && temp.previousSibling().isNull())*/))
                temp = temp.previousSibling();
            startNode = temp;

            if(!toplevelBlock2.isNull() && toplevelBlock2.hasChildNodes())
                startNode2 = toplevelBlock2.firstChild();
            else if(!childOfCommonParent.isNull() && !childOfCommonParent.nextSibling().isNull())
                startNode2 = childOfCommonParent.nextSibling();
            startNode2IsNotInline = false;
            temp = startNode2;
            attrsTmp = w->getAttrs(temp);
            if(attrsTmp && (attrsTmp->chCurFoc() == kNodeAttrs::singleNodeAndItself ||
                            attrsTmp->chCurFoc() == kNodeAttrs::no))
                startNode2IsNotInline = true;
            while(!temp.isNull() && !temp.nextSibling().isNull() &&
                    ((kafkaCommon::isInline(temp) && (temp.nextSibling().isNull() ||
                                                      kafkaCommon::isInline(temp.nextSibling())))/** ||
                                                               				(!isInline(temp) && temp.nextSibling().isNull())*/))
                temp = temp.nextSibling();
            endNode2 = temp;

            //3 - Move Nodes.
            if(!startNode2.isNull() && startNode2IsNotInline)
            {
                emit domNodeIsAboutToBeRemoved(startNode2, true, m_modifs);
                startNode2.parentNode().removeChild(startNode2);
            }
            else if(isParent && !nextIsBlock)
            {
                if(kafkaCommon::parentSupports(toplevelBlock, startNode2, endNode2,
                                               w->getCurrentDoc()->defaultDTD()))
                    moveDomNodes(toplevelBlock, startNode2, endNode2, DOM::Node(), false);
                else
                {
                    if(kafkaCommon::parentSupports(commonParent, startNode, endNode,
                                                   w->getCurrentDoc()->defaultDTD()))
                        moveDomNodes(commonParent, startNode, endNode, childOfCommonParent2,
                                     true);
                    else
                    {
                        //Damn it! What to do??
                    }
                }
            }
            else if(isParent && nextIsBlock)
            {
                if(kafkaCommon::parentSupports(toplevelBlock, startNode2, endNode2,
                                               w->getCurrentDoc()->defaultDTD()))
                    moveDomNodes(toplevelBlock, startNode2, endNode2, DOM::Node(), false);
                else
                {
                    if(kafkaCommon::parentSupports(commonParent, startNode, endNode,
                                                   w->getCurrentDoc()->defaultDTD()) && kafkaCommon::parentSupports(
                                commonParent, startNode2, endNode2, w->getCurrentDoc()->defaultDTD()))
                    {
                        moveDomNodes(commonParent, startNode, endNode, childOfCommonParent,
                                     false);
                        moveDomNodes(commonParent, startNode2, endNode2, childOfCommonParent2,
                                     true);
                    }
                    else
                    {
                        //Damn it! What to do??
                    }
                }
            }
            else if(!isParent && nextIsBlock)
            {
                if(kafkaCommon::parentSupports(commonParent, startNode2, endNode2,
                                               w->getCurrentDoc()->defaultDTD()))
                    moveDomNodes(commonParent, startNode2, endNode2, childOfCommonParent2, true);
                else
                {
                    //Damn it! What to do??
                }
            }
            if(!endNode.isNull())
                normalize(endNode.parentNode());

            //4 - Delete empty Block Nodes.
            if(!toplevelBlock.isNull())
            {
                temp = toplevelBlock;
                attrsTmp = w->getAttrs(temp);
                while(attrsTmp && attrsTmp->chCurFoc() == kNodeAttrs::blockNode &&
                        !temp.hasChildNodes())
                {
                    tempParent = temp.parentNode();
                    emit domNodeIsAboutToBeRemoved(temp, true, m_modifs);
                    tempParent.removeChild(temp);
                    temp = tempParent;
                    attrsTmp = w->getAttrs(temp);
                }
            }
            if(!toplevelBlock2.isNull())
            {
                temp = toplevelBlock2;
                attrsTmp = w->getAttrs(temp);
                while(attrsTmp && attrsTmp->chCurFoc() == kNodeAttrs::blockNode &&
                        !temp.hasChildNodes())
                {
                    tempParent = temp.parentNode();
                    emit domNodeIsAboutToBeRemoved(temp, true, m_modifs);
                    tempParent.removeChild(temp);
                    temp = tempParent;
                    attrsTmp = w->getAttrs(temp);
                }
            }
            break;
        }
        //Else if the nextNode is a BLOCK, or an invisible Node, Inline Node
        //which can be deleted, delete it!
        else if(attrs->chCurFoc() == kNodeAttrs::singleNodeAndItself || ((attrs->chCurFoc() ==
                kNodeAttrs::no || attrs->chCurFoc() == kNodeAttrs::inlineNode) &&
                !_nodeNext.hasChildNodes()))
        {
#ifdef LIGHT_DEBUG
            kdDebug(25001)<< "KafkaWidget::keyDelete() - deleting" <<
            " a Node" << endl;
#endif

            _nodeParent = _nodeNext.parentNode();
            //If this block is used to define the cursor pos, change node to its parent.
            if(_nodeNext == _node && _nodeParent.firstChild() == _nodeNext &&
                    _nodeNext.nextSibling().isNull())
            {
                _node = _nodeParent;
                //d->m_cursorOffset = -2;
            }
            focus = w->getAttrs(_nodeNext)->chCurFoc();
            emit domNodeIsAboutToBeRemoved(_nodeNext, true, m_modifs);
            _nodeParent.removeChild(_nodeNext);
            singleNodeDeleted = true;
            _nodeNext = _node;
            if(focus == kNodeAttrs::singleNodeAndItself)
            {
                normalize(_nodeParent);
                break;
            }
        }
        _nodeNext = getNextNode(_nodeNext, _goingTowardsRootNode);
    }

    //If the node which is defining the cursor position has been deleted (thus changed)
    if(false && singleNodeDeleted)
    {
        //Now that we have deleted something, the cursor may end up in something weird, e.g.
        //in an empty text or empty Inline. So delete them.
        _nodeNext = _nodeParent;//<== !!!!
        _nodeParent = _node.parentNode();
        childPosition = -1;
        while(!_nodeNext.isNull())
        {
            attrs = w->getAttrs(_nodeNext);

            //If this Node can't be deleted, we stop here.
            if(!attrs || !attrs->cbDel())
                break;

            //Let's delete useless Nodes
            if((_nodeNext.nodeType() == DOM::Node::TEXT_NODE &&
                    (static_cast<DOM::CharacterData>(_nodeNext)).length() == 0) ||
                    (attrs->chCurFoc() == kNodeAttrs::inlineNode && _nodeNext.hasChildNodes())
              )
            {
                childPosition = kafkaCommon::childPosition(_node);
                _node = _nodeParent;
                emit domNodeIsAboutToBeRemoved(_nodeNext, true, m_modifs);
                _nodeParent.removeChild(_nodeNext);
                normalize(_nodeParent);
            }
            else
                break;

            _nodeNext = _nodeParent;
        }

        //And finally, if the cursor is at a bad place (e.g. inside a Inline with childs), move it
        attrs = w->getAttrs(_node);
        while(attrs && attrs->chCurFoc() == kNodeAttrs::inlineNode && _node.hasChildNodes())
        {
            _node = kafkaCommon::getChildNode(_node, childPosition, true);
            childPosition = 1;
        }
    }

    /**m_currentNode = _node;
    setCaretPosition(m_currentNode, (long)d->m_cursorOffset);
    emit domNodeNewCursorPos(m_currentNode, d->m_cursorOffset, m_modifs);*/
}

void KafkaWidget::keyBackspace()
{
    kNodeAttrs *attrs, *attrsTmp;
    int focus, m_currentNodeType;
    DOM::Node _nodeParent, _node, _nodePrev, oldCurrentNode, temp, tempParent, prevSibling, nodePrevPrev;
    DOM::Node toplevelBlock, toplevelBlock2, startNode, endNode, startNode2, endNode2;
    DOM::Node childOfCommonParent, childOfCommonParent2, commonParent;
    bool _goingTowardsRootNode, singleNodeDeleted, isParent, prevIsBlock, endNodeIsNotInline, boolTmp;
    QString text;

    if(hasSelection())
    {
        removeSelection();
        return;
    }
    
    if(m_currentNode.isNull())
        return;

    attrs = w->getAttrs(m_currentNode);
    if(!attrs)
        return;
    m_currentNodeType = m_currentNode.nodeType();

#ifdef HEAVY_DEBUG

    kdDebug(25001)<< "m_currentNode(" << m_currentNode.handle() << ") : " << m_currentNode.nodeName() <<
    endl;
#endif

    //OLD PART, to be removed or #ifdef'ed
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
        
#ifdef LIGHT_DEBUG
        kdDebug(25001) << nodeText.string() << textSplitted.string() << endl;
#endif
        
        m_currentNode.setNodeValue(nodeText + textSplitted);
        m_currentNode.parentNode().applyChanges();
        --(d->m_cursorOffset);
        emit domNodeModified(m_currentNode, m_modifs);
        postprocessCursorPosition();
        setCaretPosition(m_currentNode, (long)d->m_cursorOffset);
        emit domNodeNewCursorPos(m_currentNode, d->m_cursorOffset);
        return;
    }
    if(attrs->chCurFoc() == kNodeAttrs::singleNodeAndItself &&
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
            if(_node == 0)
                break;
            attrs = w->getAttrs(_node);
            if(attrs && attrs->chCurFoc() == kNodeAttrs::textNode)
            {
                m_currentNode = _node;
                d->m_cursorOffset = (static_cast<DOM::CharacterData>(_node)).length();
                keyDelete();
                setCaretPosition(m_currentNode, (long)d->m_cursorOffset);
                emit domNodeNewCursorPos(m_currentNode, d->m_cursorOffset);
                return;
            }
            if(attrs && attrs->chCurFoc() != kNodeAttrs::no && attrs->chCurFoc() !=
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
            if(_node == 0)
                break;
            attrs = w->getAttrs(_node);
            if(attrs && attrs->chCurFoc() != kNodeAttrs::no)
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
        emit domNodeIsAboutToBeRemoved(m_currentNode, true, m_modifs);
        _node.removeChild(m_currentNode);
        m_currentNode = document().createTextNode("");
        _node.appendChild(m_currentNode);
        emit domNodeInserted(m_currentNode, false, m_modifs);
        setCaretPosition(m_currentNode, (long)d->m_cursorOffset);
        emit domNodeNewCursorPos(m_currentNode, d->m_cursorOffset);

    }

    //Beginning of the actual keyBackspace
    _node = m_currentNode;
    _goingTowardsRootNode = false;
    singleNodeDeleted = false;
    _nodePrev = getPrevNode(_node, _goingTowardsRootNode);
    oldCurrentNode = m_currentNode;

    while(!_nodePrev.isNull())
    {
#ifdef LIGHT_DEBUG
        kdDebug(25001)<< "KafkaWidget::keyBackspace() - currentNode: " <<
        _nodePrev.nodeName().string() << endl;
#endif

        attrs = w->getAttrs(_nodePrev);
        if(!attrs)
        {
            kdError(25001) << "NULL kNodeAttrs instance: attrs = w->getAttrs(_nodePrev);" << endl;
            kafkaCommon::coutDomTree(_nodePrev, 3);
            return; 
            // FIXME Understand why this happen.
//             Test case:
//             1. Write two words in a new VPL document and make the first one a link;
//             2. Put the cursor at most right and then press backspace until it crashes
//             When you get to the link the cursor stays in the same plave and you have press it several times until it crashes.
        }

        //If this Node can't be deleted, we stop here.
        if(!attrs->cbDel())
            return;

        //If we are in a TEXT node, we remove a letter
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
                
                m_currentNode = _nodePrev;
                d->m_cursorOffset = (static_cast<DOM::CharacterData>(_nodePrev)).length();
                                
                postprocessCursorPosition();
                emit domNodeModified(_nodePrev, m_modifs);
                return;
            }
            else
            {//if we are in an empty text
#ifdef LIGHT_DEBUG
                kdDebug(25001)<< "KafkaWidget::keyBackspace() - deleting" <<
                " empty #text" << endl;
#endif

                _nodeParent = _nodePrev.parentNode();
                emit domNodeIsAboutToBeRemoved(_nodePrev, true, m_modifs);
                _nodeParent.removeChild(_nodePrev);
                _nodeParent.applyChanges();
                _nodePrev = _node;
                continue;
            }
        }
        //Else if the current Node if a BLOCK which can be entered/leaved e.g. H1, P
        else if(attrs->chCurFoc() == kNodeAttrs::blockNode)
        {
            //First look if it is one of _node's parent
            isParent = false;
            temp = _node;
            while(!temp.isNull())
            {
                if(_nodePrev == temp)
                    isParent = true;
                temp = temp.parentNode();
            }

            //1 - Locate the toplevel blocks
            temp = _nodePrev;
            if(isParent)
            {
                toplevelBlock2 = temp;
                while(temp.parentNode().firstChild() == temp && w->getAttrs(temp.parentNode()) &&
                        w->getAttrs(temp.parentNode())->chCurFoc() == kNodeAttrs::blockNode)
                    temp = temp.parentNode();
                childOfCommonParent2 = temp;
                temp = temp.previousSibling();
            }
            if(temp.isNull())
                break;
            childOfCommonParent = temp;
            commonParent = temp.parentNode();
            attrsTmp = w->getAttrs(temp);
            prevIsBlock = (attrsTmp && attrsTmp->chCurFoc() == kNodeAttrs::blockNode);
            while(!temp.isNull() && temp.hasChildNodes() && w->getAttrs(temp.lastChild()) &&
                    w->getAttrs(temp.lastChild())->chCurFoc() == kNodeAttrs::blockNode)
                temp = temp.lastChild();
            toplevelBlock = temp;

            //2 - Determine the Nodes which could be moved
            if(!toplevelBlock.isNull() && toplevelBlock.hasChildNodes())
                endNode = toplevelBlock.lastChild();
            else if(!childOfCommonParent2.isNull() && !childOfCommonParent2.previousSibling().isNull())
                endNode = childOfCommonParent2.previousSibling();
            endNodeIsNotInline = false;
            temp = endNode;
            attrsTmp = w->getAttrs(temp);
            if(attrsTmp && (attrsTmp->chCurFoc() == kNodeAttrs::singleNodeAndItself ||
                            attrs->chCurFoc() == kNodeAttrs::no))
                endNodeIsNotInline = true;
            while(!temp.isNull() && !temp.previousSibling().isNull() &&
                    ((kafkaCommon::isInline(temp) && (temp.previousSibling().isNull() ||
                                                      kafkaCommon::isInline(temp.previousSibling())))))
                temp = temp.previousSibling();
            startNode = temp;

            if(!toplevelBlock2.isNull() && toplevelBlock2.hasChildNodes())
                startNode2 = toplevelBlock2.firstChild();
            else if(!childOfCommonParent.isNull() && !childOfCommonParent.nextSibling().isNull())
                startNode2 = childOfCommonParent.nextSibling();
            temp = startNode2;
            while(!temp.isNull() && !temp.nextSibling().isNull() &&
                    ((kafkaCommon::isInline(temp) && (temp.nextSibling().isNull() ||
                                                      kafkaCommon::isInline(temp.nextSibling())))))
                temp = temp.nextSibling();
            endNode2 = temp;

            //3 - Move Nodes.
            if(!endNode.isNull() && endNodeIsNotInline)
            {
                emit domNodeIsAboutToBeRemoved(endNode, true, m_modifs);
                endNode.parentNode().removeChild(endNode);
            }
            else if(isParent && !prevIsBlock)
            {
                if(kafkaCommon::parentSupports(toplevelBlock2, startNode, endNode,
                                               w->getCurrentDoc()->defaultDTD()))
                    moveDomNodes(toplevelBlock2, startNode, endNode, toplevelBlock2.firstChild(),
                                 true);
                else
                {
                    if(kafkaCommon::parentSupports(commonParent, startNode2, endNode2,
                                                   w->getCurrentDoc()->defaultDTD()))
                        moveDomNodes(commonParent, startNode2, endNode2,
                                     childOfCommonParent2, true);
                    else
                    {
                        //Damn it! What to do??
                    }
                }
            }
            else if(isParent && prevIsBlock)
            {
                if(kafkaCommon::parentSupports(toplevelBlock2, startNode, endNode,
                                               w->getCurrentDoc()->defaultDTD()))
                    moveDomNodes(toplevelBlock2, startNode, endNode, toplevelBlock2.firstChild(),
                                 true);
                else
                {
                    if(kafkaCommon::parentSupports(commonParent, startNode, endNode,
                                                   w->getCurrentDoc()->defaultDTD()) && kafkaCommon::parentSupports(
                                commonParent, startNode2, endNode2, w->getCurrentDoc()->defaultDTD()))
                    {
                        moveDomNodes(commonParent, startNode, endNode, childOfCommonParent,
                                     false);
                        moveDomNodes(commonParent, startNode2, endNode2, childOfCommonParent2,
                                     true);
                    }
                    else
                    {
                        //Damn it! What to do??
                    }
                }
            }
            else if(!isParent && prevIsBlock)
            {
                if(kafkaCommon::parentSupports(commonParent, startNode, endNode,
                                               w->getCurrentDoc()->defaultDTD()))
                    moveDomNodes(commonParent, startNode, endNode, childOfCommonParent, false);
                else
                {
                    //Damn it! What to do??
                }
            }
            if(!startNode2.isNull() && startNode2.nodeType() == DOM::Node::TEXT_NODE)
            {
                //normalize(startNode2.parentNode());
                temp = startNode2.previousSibling();
                if(!temp.isNull() && temp.nodeType() == DOM::Node::TEXT_NODE)
                {
                    boolTmp = false;
                    if(m_currentNode == startNode2)
                    {
                        m_currentNode = temp;
                        d->m_cursorOffset += temp.nodeValue().length();
                        boolTmp = true;
                    }
                    text = temp.nodeValue().string() + startNode2.nodeValue().string();
                    tempParent = temp.parentNode();
                    emit domNodeIsAboutToBeRemoved(startNode2, true, m_modifs);
                    tempParent.removeChild(startNode2);

                    temp.setNodeValue(text);
                    emit domNodeModified(temp, m_modifs);

                    if(boolTmp)
                        QTimer::singleShot(0, this, SLOT(slotDelayedSetCaretPosition()));
                }

            }

            //4 - Delete empty Block Nodes.
            if(!toplevelBlock.isNull())
            {
                temp = toplevelBlock;
                attrsTmp = w->getAttrs(temp);
                while(attrsTmp && attrsTmp->chCurFoc() == kNodeAttrs::blockNode &&
                        !temp.hasChildNodes())
                {
                    tempParent = temp.parentNode();
                    emit domNodeIsAboutToBeRemoved(temp, true, m_modifs);
                    tempParent.removeChild(temp);
                    temp = tempParent;
                    attrsTmp = w->getAttrs(temp);
                }
            }
            if(!toplevelBlock2.isNull())
            {
                temp = toplevelBlock2;
                attrsTmp = w->getAttrs(temp);
                while(attrsTmp && attrsTmp->chCurFoc() == kNodeAttrs::blockNode &&
                        !temp.hasChildNodes())
                {
                    tempParent = temp.parentNode();
                    emit domNodeIsAboutToBeRemoved(temp, true, m_modifs);
                    tempParent.removeChild(temp);
                    temp = tempParent;
                    attrsTmp = w->getAttrs(temp);
                }
            }
            break;
        }
        //Else if the prevNode is a BLOCK or an invisible Node, Inline Node
        //which can be deleted, delete it!
        else if(attrs->chCurFoc() ==	kNodeAttrs::singleNodeAndItself || ((attrs->chCurFoc() ==
                kNodeAttrs::no || attrs->chCurFoc() == kNodeAttrs::inlineNode) &&
                !_nodePrev.hasChildNodes()))
        {
#ifdef LIGHT_DEBUG
            kdDebug(25001)<< "KafkaWidget::keyBackspace() - deleting" <<
            " a Node" << endl;
#endif

            _nodeParent = _nodePrev.parentNode();
            focus = w->getAttrs(_nodePrev)->chCurFoc();
            emit domNodeIsAboutToBeRemoved(_nodePrev, true, m_modifs);
            _nodeParent.removeChild(_nodePrev);
            //normalize(_nodeParent);
            if(focus == kNodeAttrs::singleNodeAndItself)
            {
                postprocessCursorPosition();
                //merge the previous DOM::Node if it is a text.
                //domNodeIsAboutToBeRemoved() already do it in the Node tree.
                //=> It seems it was removed from it.
                _nodePrev = _node.previousSibling();
                if(!_nodePrev.isNull() && _nodePrev.nodeType() == DOM::Node::TEXT_NODE &&
                        m_currentNodeType == DOM::Node::TEXT_NODE)
                {
                    if(_node == m_currentNode)
                    {
                        m_currentNode = _nodePrev;
                        d->m_cursorOffset += (static_cast<DOM::CharacterData>(_nodePrev)).length();
                        QTimer::singleShot(0, this, SLOT(slotDelayedSetCaretPosition()));
                    }
                    _nodePrev.setNodeValue(_nodePrev.nodeValue() + _node.nodeValue());
                    emit domNodeModified(_nodePrev, m_modifs);
                    //_nodeParent = _nodePrev.parentNode();
                    emit domNodeIsAboutToBeRemoved(_node, true, m_modifs);
                    _nodeParent.removeChild(_node);
                }
                //dirty workaround when after having deleted a br, there is only one br left
                //Anyway webcore will override this
                if(m_currentNode.nodeName().string().lower() == "br" &&
                        (m_currentNode.previousSibling().isNull() || (m_currentNode.previousSibling().nodeType() ==
                                DOM::Node::TEXT_NODE && m_currentNode.previousSibling().previousSibling().isNull())) &&
                        (m_currentNode.nextSibling().isNull() || (m_currentNode.nextSibling().nodeType() ==
                                DOM::Node::TEXT_NODE && m_currentNode.nextSibling().nextSibling().isNull())))
                {
                    if(!m_currentNode.previousSibling().isNull())
                    {
                        m_currentNode = m_currentNode.previousSibling();
                        d->m_cursorOffset = 0;
                        
                        QTimer::singleShot(0, this, SLOT(slotDelayedSetCaretPosition()));
                    }
                    else if(!m_currentNode.nextSibling().isNull())
                    {
                        m_currentNode = m_currentNode.nextSibling();
                        d->m_cursorOffset = 0;
                    }
                }
                break;
            }
            _nodePrev = _node;
        }
        _nodePrev = getPrevNode(_nodePrev, _goingTowardsRootNode);
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

void KafkaWidget::updateToggableTagActions(/*const DOM::Node &domNode, long offset*/) const
{
//Andras: Disable toggle behavior. It is just too broken.
return;

    quantaApp->removeAllTagActionPoolItems();
    
    NodeSelectionInd selection;
    selection.fillWithVPLCursorSelection();
    
    Node* start_node = 0, *end_node = 0;
//     int start_offset = 0, end_offset = 0;
        
    start_node = kafkaCommon::getNodeFromLocation(selection.cursorNode());
//     start_offset = selection.cursorOffset();
    
    if(!start_node)
        return;
    
    if(selection.hasSelection())
    {
        end_node = kafkaCommon::getNodeFromLocation(selection.cursorNodeEndSel());
//         end_offset = selection.cursorOffsetEndSel();
    }
    else
    {
        end_node = start_node;
//         end_offset = start_offset;
    }
    
    // Iterate all toggable toolbar actions and toggle them on or off
    // Look if there is a selection
    TagAction* tag_action = 0;    
    QPtrList<TagAction> tag_actions = quantaApp->tagActions();        
    for (tag_action = tag_actions.first(); tag_action; tag_action = tag_actions.next())
    {
        if(tag_action->toggable())
        {
            QString tag_name = tag_action->XMLTagName();
            if(tag_name.isEmpty())
                break;
            
            QDomElement data(tag_action->data());
            QString attribute_name(data.attribute("attribute_name", QString()));
            QString attribute_value(data.attribute("attribute_value", QString()));
            
            int inside_tag;
            if(!attribute_name.isEmpty() && !attribute_value.isEmpty())
                inside_tag = kafkaCommon::isInsideTag(start_node, end_node, tag_name, attribute_name, attribute_value);
            else
                inside_tag = kafkaCommon::isInsideTag(start_node, end_node, tag_name);
            
            tag_action->setChecked(inside_tag == 1);
        }
    }
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
        /** while(1)
         {
             _prevNextNode = _nextNode;
             _nextNode = kafkaCommon::getPrevDomNode(_nextNode);
             if(_nextNode.isNull())
                 break;
             attrs2 = w->getAttrs(_nextNode);
             if(attrs2 && attrs2->chCurFoc() == kNodeAttrs::textNode &&
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
             else if(attrs2->chCurFoc() == kNodeAttrs::singleNodeAndItself ||
                     attrs2->chCurFoc() == kNodeAttrs::inlineNode ||
                     attrs2->chCurFoc() == kNodeAttrs::blockNode)
                 break;
             else
                 continue;
         }*/
    }
    else if(attrs->chCurFoc() == kNodeAttrs::singleNodeAndItself)
    {
        if(d->m_cursorOffset == 0  && !m_currentNode.isNull() &&
                (m_currentNode.nodeName().string().lower() != "br" ||
                 (m_currentNode.nodeName().string().lower() == "br" && /**!m_currentNode.nextSibling().isNull() &&
                            m_currentNode.nextSibling().nodeType() == DOM::Node::TEXT_NODE &&
                            m_currentNode.nextSibling().nodeValue().string().isEmpty() &&
                            m_currentNode.nextSibling().nextSibling().isNull() &&*/
                  !m_currentNode.previousSibling().isNull() &&
                  m_currentNode.previousSibling().nodeType() == DOM::Node::TEXT_NODE &&
                  !m_currentNode.previousSibling().nodeValue().string().isEmpty())))
        {
            while(1)
            {
                _prevNextNode = _nextNode;
                _nextNode = getPrevNode(_nextNode, b);
                if(_nextNode == 0)
                    break;
                attrs2 = w->getAttrs(_nextNode);
                if(attrs2 && attrs2->chCurFoc() == kNodeAttrs::textNode &&
                        (static_cast<DOM::CharacterData>(_nextNode)).length()
                        != 0)
                {
                    m_currentNode = _nextNode;
                    d->m_cursorOffset = (static_cast<DOM::CharacterData>(_nextNode)).length();
                    setCaretPosition(m_currentNode, (long)d->m_cursorOffset);
                    emit domNodeNewCursorPos(m_currentNode, d->m_cursorOffset);
#ifdef LIGHT_DEBUG

                    kdDebug(25001)<< "KafkaWidget::postprocessCursorPosition()" <<
                    " - new currentNode :" << m_currentNode.nodeName().string() << endl;
#endif

                    break;
                }
                else if(attrs2 && attrs2->chCurFoc() == kNodeAttrs::singleNodeAndItself)
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
                if(_nextNode == 0)
                    break;
                attrs2 = w->getAttrs(_nextNode);
                if(attrs2 && attrs2->chCurFoc() == kNodeAttrs::singleNodeAndItself)
                    break;
                else if(attrs2 && attrs2->chCurFoc() == kNodeAttrs::textNode &&
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
    {
      return;
    }
    if(mouseNode.nodeType() == DOM::Node::TEXT_NODE)
      view()->setCursor(Qt::ibeamCursor);
    else
      view()->setCursor(Qt::arrowCursor);

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

void KafkaWidget::getCurrentNode(DOM::Node &_currentNode, long &offset)
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

void KafkaWidget::setCurrentNode(Node* cursorNode, int cursorOffset)
{
    DOM::Node domNode;
    long longDomNodeOffset;
    KafkaDocument::ref()->translateNodeIntoKafkaCursorPosition(cursorNode, cursorOffset, domNode, longDomNodeOffset);
    if (!domNode.isNull() && domNode.nodeType() != DOM::Node::TEXT_NODE &&
         !domNode.firstChild().isNull() && domNode.firstChild().nodeType() == DOM::Node::TEXT_NODE)
        domNode = domNode.firstChild();
    if (!domNode.isNull())
        setCurrentNode(domNode, (int)longDomNodeOffset);       
}

void KafkaWidget::putCursorAtFirstAvailableLocation()
{
    kNodeAttrs *attrs = 0L;
    DOM::Node node = w->body;
    bool b = false;

#ifdef HEAVY_DEBUG

    w->coutLinkTree(baseNode, 2);
    kafkaCommon::coutTree(baseNode, 2);
    kafkaCommon::coutDomTree(document(), 2);
#endif

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
        if(!attrs)
        {
            node = w->body;
            break;
        }
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

    if(quantaApp->aTab && ViewManager::ref()->activeView()->hadLastFocus() == QuantaView::VPLFocus)
        quantaApp->aTab->setCurrentNode(w->getNode(domNode));

    updateToggableTagActions(/*domNode, offset*/);            
}

void KafkaWidget::moveDomNodes(DOM::Node newParent, DOM::Node startNode, DOM::Node endNode,
                               DOM::Node refNode, bool before)
{
    DOM::Node domNode, domNodeNext;

    if(newParent.isNull())
        return;

    if(before)
    {
        domNode = endNode;
        while(!domNode.isNull())
        {
            domNodeNext = domNode.previousSibling();
            emit domNodeIsAboutToBeMoved(domNode, newParent, refNode, m_modifs);
            //emit domNodeIsAboutToBeRemoved(domNode, true);
            domNode = domNode.parentNode().removeChild(domNode);
            if(!refNode.isNull())
                newParent.insertBefore(domNode, refNode);
            else
                newParent.insertBefore(domNode, DOM::Node());
            //emit domNodeInserted(domNode, true);
            if(domNode == startNode)
                break;
            domNode = domNodeNext;
        }
    }
    else
    {
        domNode = startNode;
        while(!domNode.isNull())
        {
            domNodeNext = domNode.nextSibling();
            //emit domNodeIsAboutToBeRemoved(domNode, true);
            if(!refNode.isNull())
                emit domNodeIsAboutToBeMoved(domNode, newParent, refNode.nextSibling(), m_modifs);
            else
                emit domNodeIsAboutToBeMoved(domNode, newParent, DOM::Node(), m_modifs);
            domNode = domNode.parentNode().removeChild(domNode);
            if(!refNode.isNull())
                newParent.insertBefore(domNode, refNode.nextSibling());
            else
                newParent.insertBefore(domNode, DOM::Node());
            //emit domNodeInserted(domNode, true);
            if(domNode == endNode)
                break;
            domNode = domNodeNext;
        }
    }
}

void KafkaWidget::removeSelection()
{
    Q_ASSERT(hasSelection());
    
    NodeSelectionInd selection;
    selection.fillWithVPLCursorSelection();
    Node* cursorNode = kafkaCommon::getNodeFromLocation(selection.cursorNode());
    long cursorOffset = 0;
    long domNodeCursorOffset = 0;
        
    kafkaCommon::DTDRemoveSelection(selection, &cursorNode, cursorOffset, m_modifs);
    
    KafkaDocument::ref()->translateNodeIntoKafkaCursorPosition(cursorNode, cursorOffset, m_currentNode, domNodeCursorOffset);
    d->m_cursorOffset = domNodeCursorOffset;
    
    setCurrentNode(m_currentNode, domNodeCursorOffset);
    
    QTimer::singleShot(0, this, SLOT(slotDelayedSetCaretPosition()));
    
    NodeSelection* cursorPos = new NodeSelection();
    cursorPos->setCursorNode(cursorNode);
    cursorPos->setCursorOffset(cursorOffset);
    
    ViewManager::ref()->activeDocument()->docUndoRedo->addNewModifsSet(m_modifs, undoRedo::NodeTreeModif, cursorPos);
    m_modifs = 0;        

    delete cursorPos;

    makeCursorVisible();
}

void KafkaWidget::applyQueuedToggableTagActions()
{
    QStringList queued_actions = quantaApp->tagActionPool();
    QPtrList<TagAction> action_list = quantaApp->tagActions();
    for(QStringList::Iterator it = queued_actions.begin(); it != queued_actions.end(); ++it) 
    {
        TagAction* tag_action = 0;    
        for (tag_action = action_list.first(); tag_action; tag_action = action_list.next())
        {
            if(tag_action->name() == *it)
            {
                tag_action->slotActionActivated(KAction::EmulatedActivation, Qt::NoButton);
                break;
            }
        }
    }
    quantaApp->removeAllTagActionPoolItems();
}

#include "kafkahtmlpart.moc"
