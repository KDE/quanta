/***************************************************************************
                          tagaction.cpp  -  description
                             -------------------
    begin                : ?
    copyright            : (C) ? Dmitry Poplavsky
                           (C) 2002-2003 Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

//other includes
#include <sys/types.h>
#include <unistd.h>


//qt includes
#include <qdir.h>
#include <qdom.h>
#include <qfile.h>
#include <qtimer.h>

//kde includes
#include <kapplication.h>
#include <kdebug.h>
#include <kprocess.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kshortcut.h>
#include <kstandarddirs.h>
#include <ktempfile.h>
#include <ktexteditor/document.h>
#include <ktexteditor/viewcursorinterface.h>
#include <ktexteditor/editinterface.h>
#include <ktexteditor/selectioninterface.h>
#include <ktexteditor/selectioninterfaceext.h>

//app includes
#include "tagaction.h"
#include "myprocess.h"
#include "document.h"
#include "quantaview.h"
#include "quanta.h"
// #include "quantadoc.h"
#include "tagdialog.h"
#include "messageoutput.h"
#include "quantacommon.h"
#include "resource.h"
#include "qextfileinfo.h"
#ifdef BUILD_KAFKAPART
#include "kafkacommon.h"
#include "kafkahtmlpart.h"
#include "wkafkapart.h"
#include "undoredo.h"
#include "tag.h"
#endif
#include "project.h"

#include "viewmanager.h"

MyProcess::MyProcess():KProcess()
{
}

int MyProcess::commSetupDoneC()
{
  ::setpgid(pid_, 0);
  return KProcess::commSetupDoneC();
}

TagAction::TagAction( QDomElement *element, KActionCollection *parent)
  : KAction( element->attribute("text").isEmpty() ? QString("") : i18n(element->attribute("text").utf8()), KShortcut(element->attribute("shortcut")), 0, 0, parent, element->attribute("name") )
{
  m_modified = false;
  tag = element->cloneNode().toElement();
  QString s = tag.attribute("icon");
  if (!QFileInfo(s).exists())
  {
    s = QFileInfo(s).fileName();
  }
  setIcon( s );
  m_file = 0L;
  loopStarted = false;
  m_appMessages = quantaApp->messageOutput();
  connect( this, SIGNAL(activated()), SLOT(insertTag()) );
}

TagAction::~TagAction()
{
}

QString TagAction::type()
{
   return tag.attribute("type","");
}

bool TagAction::insertTag(bool inputFromFile, bool outputToFile)
{
  QuantaView *view = ViewManager::ref()->activeView();
  if ( !view || !view->document())
     return false;

  QString space="";
#ifdef BUILD_KAFKAPART
  QString output;
#endif
  unsigned int line, col;

  Document *w = view->document();
  w->viewCursorIf->cursorPositionReal(&line, &col);
  space.fill( ' ', col);

  QString type = tag.attribute("type","");

  if ( type == "tag" ) {
     QDomElement otag = (tag.namedItem("tag")).toElement();
     QDomElement xtag = (tag.namedItem("xtag")).toElement();

     QString attr = otag.text();
     if ( attr[0] == '<' )
         attr.remove(0,1);
     if ( attr.right(1) == ">" )
         attr.remove( attr.length()-1, 1 );
     attr = attr.stripWhiteSpace();
     int i = 0;
     while ( !attr[i].isSpace() && !attr[i].isNull() )  i++;
     QString name = attr.left(i);
     attr = attr.remove(0,i).stripWhiteSpace();

     if (otag.attribute("useDialog","false") == "true" && QuantaCommon::isKnownTag(w->defaultDTD()->name, name))
     {
#ifdef BUILD_KAFKAPART
       if (view->hadLastFocus() == QuantaView::VPLFocus || (view->hadLastFocus() == QuantaView::SourceFocus &&
         qConfig.smartTagInsertion))
         insertOutputInTheNodeTree("", "", quantaApp->showTagDialogAndReturnNode(name, attr));
       else
         view->insertNewTag(name, attr, xtag.attribute("inLine","true") == "true");
#else
       view->insertNewTag(name, attr, xtag.attribute("inLine","true") == "true");
       //FIXME: get the output
#endif
     }
     else
     {
       QString s1 = QuantaCommon::tagCase(name);
       if (otag.text().left(1) == "<") s1 = "<"+s1;
       if (!attr.isEmpty())
          s1 += " "+QuantaCommon::attrCase(attr);
       if (otag.text().right(1) == ">")
       {
         QTag *dtdTag = QuantaCommon::tagFromDTD(w->defaultDTD(), name);
         if ( w->defaultDTD()->singleTagStyle == "xml" && dtdTag &&
              (dtdTag->isSingle() || (!qConfig.closeOptionalTags && dtdTag->isOptional()))
            )
         {
           s1.append(" /");
         }

         s1.append(">");
       }

       QString s2;
       if ( xtag.attribute("use","false") == "true" )
       {
         if (qConfig.closeTags)
            s2 = QuantaCommon::tagCase(xtag.text());
         if ( xtag.attribute("inLine","true") == "true" )
         {
#ifdef BUILD_KAFKAPART
           /** FIXME this is quick and temporary */
           insertOutputInTheNodeTree(s1, s2);
#else
           w->insertTag( s1, s2 );
#endif
         }
         else
         {
#ifdef BUILD_KAFKAPART
           insertOutputInTheNodeTree(s1, s2);
#else
           w->insertTag( s1+"\n"+space+"  ", "\n"+space+s2 );
#endif
         }
       }
       else
#ifdef BUILD_KAFKAPART
         insertOutputInTheNodeTree(s1, s2);
#else
         w->insertTag( s1 );
#endif
     }
  }

#ifdef BUILD_KAFKAPART
  if (view->hadLastFocus() != QuantaView::VPLFocus)
  {
#endif

  if ( type == "text" )
    w->insertTag( tag.namedItem("text").toElement().text() );

  if ( type == "script" )
  {
    proc = new MyProcess();
    proc->setWorkingDirectory(quantaApp->projectBaseURL().path());

    QDomElement script = tag.namedItem("script").toElement();
    QString command = script.text();


    if ( !w->isUntitled() ) {
      QString fname = w->url().url();
      if ( fname.left(5) == "file:")
        fname.remove(0,5);
      command.replace("%f", fname );
    }

    pid_t pid = ::getpid();
    if (kapp->inherits("KUniqueApplication"))
    {
      command.replace("%pid", QString("unique %1").arg(pid));
    } else
    {
      command.replace("%pid", QString("%1").arg(pid));
    }
    QString buffer;
    QString inputType = script.attribute("input","none");

    if ( inputType == "current" ) {
      buffer = w->editIf->text();
    } else
    if ( inputType == "selected" ) {
      buffer = w->selectionIf->selection();
    }
    command.replace("%input", buffer);
    command = command.stripWhiteSpace();
    int pos = command.find(' ');
    QString args;
    if (pos != -1)
    {
      args = command.mid(pos+1);
      command = command.left(pos);
    }
    if (command.startsWith("~"))
    {
      command = command.mid(1);
      command.prepend(QDir::homeDirPath());
    }

    *proc << command.stripWhiteSpace();
    args = args.stripWhiteSpace();
    if (!args.isEmpty())
    {
      pos = 0;
      while (pos != -1 )
      {
        pos = args.find("%scriptdir");
        QString scriptname;
        if (pos != -1)
        {
          int begin = args.findRev('"', pos);
          int end = -1;
          if (begin == -1)
          {
            begin = args.findRev('\'', pos);
            if (begin != -1)
                end = args.find('\'', pos);
          }  else
          {
            end = args.find('"', pos);
          }
          if (begin == -1 || end != -1)
          {
            begin = args.findRev(' ', pos);
            if (begin == -1)
                begin = 0;
            end = args.find(' ', pos);
            if (end == -1)
                end = args.length();
          }
          scriptname = args.mid(begin, end - begin).stripWhiteSpace();
          scriptname.replace("%scriptdir","scripts");
   //       kdDebug(24000) << "Script name is: |" << scriptname << "|" << endl;
          scriptname = " " + locate("appdata", scriptname);
   //       kdDebug(24000) << "Script found at: " << scriptname << endl;
          args.replace(begin, end - begin, scriptname);
   //       kdDebug(24000) << "Modified argument list: " << args << endl;
        }
      }
      int pos = args.find("%projectbase");
      if (pos != -1)
      {
          QString s;
          if (Project::ref()->hasProject())
            s = Project::ref()->projectBaseURL().url();
          args.replace("%projectbase", s);
      }
      pos = args.find("%userarguments");
      if (pos != -1)
      {
         QString s = m_argsList.join(" ");
         args.replace("%userarguments", s);
      }
      QStringList argsList = QStringList::split(' ', args);
      m_argsList.clear();
      *proc << argsList;
    }
    firstOutput = true;
    firstError  = true;

    connect( proc, SIGNAL(receivedStdout(   KProcess*,char*,int)), this,
                 SLOT(  slotGetScriptOutput(KProcess*,char*,int)));
    connect( proc, SIGNAL(receivedStderr(   KProcess*,char*,int)), this,
                 SLOT(  slotGetScriptError(KProcess*,char*,int)));
    connect( proc, SIGNAL(processExited(   KProcess*)), this,
                 SLOT(  slotProcessExited(KProcess*)));



    if (!outputToFile)
        scriptOutputDest = script.attribute("output","none");
    else
        scriptOutputDest = "file";
    scriptErrorDest  = script.attribute("error","none");
    if (scriptOutputDest == "message")
    {
      quantaApp->slotShowMessagesView();
    }

    if (inputFromFile)
    {
      *proc << m_inputFileName;
    }

    if (proc->start(KProcess::NotifyOnExit, KProcess::All))
    {
      m_appMessages->clear();
      m_appMessages->showMessage( i18n("The \"%1\" script started.\n").arg(actionText()) );
      if (!inputFromFile)
      {
        if ( inputType == "current" || inputType == "selected" )
        {
          proc->writeStdin( buffer.local8Bit(), buffer.length() );
        }
      }
      proc->closeStdin();
    } else
    {
      KMessageBox::error(quantaApp, i18n("<qt>There was an error running <b>%1</b>.<br>Check that you have the <i>%2</i> executable installed and it is accessible.</qt>").arg(command + " " + args).arg(command), i18n("Script Not Found"));
      ViewManager::ref()->activeView()->setFocus();
      if (loopStarted)
      {
          qApp->exit_loop();
          loopStarted = false;
      }
      return false;
    }
  }
#ifdef BUILD_KAFKAPART
 }
#endif
  return true;
}

void TagAction::slotGetScriptOutput( KProcess *, char *buffer, int buflen )
{
  QCString tmp( buffer, buflen + 1 );
  QString text( QString::fromLocal8Bit(tmp) );
//  kdDebug(24000) << "Script output received: |" << text << "|" << endl;
  Document *w = ViewManager::ref()->activeDocument();
  if (!w)
  {
    kdDebug(24000) << "Document not found." << endl;
    return;
  }
  if ( scriptOutputDest == "cursor" )
  {
     w->insertTag( text );
  } else
  if ( scriptOutputDest == "selection" )
  {
     if ( firstOutput )
     {
       int line = dynamic_cast<KTextEditor::SelectionInterfaceExt*>(w->doc())->selEndLine();
       int col = dynamic_cast<KTextEditor::SelectionInterfaceExt*>(w->doc())->selEndCol();
       w->viewCursorIf->setCursorPositionReal(line, col);
       w->selectionIf->removeSelectedText();
     }
     w->insertTag( text );
  } else
  if ( scriptOutputDest == "replace" )
  {
    if ( firstOutput )
       w->editIf->clear();
    w->insertTag( text );
  } else
  if ( scriptOutputDest == "new" )
  {
    if ( firstOutput )
    {
        quantaApp->slotFileNew();
        w = ViewManager::ref()->activeDocument();
    }
    w->insertTag( text );
  } else
  if ( scriptOutputDest == "message" )
  {
    if ( firstOutput )
    {
      quantaApp->slotShowMessagesView();
      m_appMessages->showMessage( i18n( "The \"%1\" script output:\n" ).arg( actionText() ) );
    }
    m_appMessages->showMessage( text, true );
  } else
  if ( scriptOutputDest == "file" && m_file)
  {
    if (!m_file->isOpen())
       m_file->open(IO_ReadWrite);
    m_file->writeBlock(buffer, buflen);
  }

  firstOutput = false;
}

void TagAction::slotGetScriptError( KProcess *, char *buffer, int buflen )
{
  Document *w = ViewManager::ref()->activeDocument();
  QCString tmp( buffer, buflen + 1 );
  QString text( QString::fromLocal8Bit(tmp) );

  if ( scriptErrorDest == "merge" )
  {
    scriptErrorDest = scriptOutputDest;
    firstError = firstOutput;
  }
  if ( scriptErrorDest == "cursor" )
     w->insertTag( text );
  else
  if ( scriptErrorDest == "selection" )
  {
     if ( firstError )
     {
       int line = dynamic_cast<KTextEditor::SelectionInterfaceExt*>(w->doc())->selEndLine();
       int col = dynamic_cast<KTextEditor::SelectionInterfaceExt*>(w->doc())->selEndCol();
       w->viewCursorIf->setCursorPositionReal(line, col);
       w->selectionIf->removeSelectedText();
     }
     w->insertTag( text );
  } else
  if ( scriptErrorDest == "replace" )
  {
    if ( firstError )
       w->editIf->clear();
    w->insertTag( text );
  } else
  if ( scriptErrorDest == "new" )
  {
    if ( firstError )
    {
        quantaApp->slotFileNew();
        w = ViewManager::ref()->activeDocument();
    }
    w->insertTag( text );
  } else
  if ( scriptErrorDest == "message" )
  {
    if ( firstError )
    {
      quantaApp->slotShowMessagesView();
      m_appMessages->showMessage( i18n( "The \"%1\" script output:\n" ).arg(actionText()) );
    }
    m_appMessages->showMessage( text, true );
  }

  firstError = false;
}

void TagAction::scriptDone()
{
    delete proc;
    proc = 0;
}

void TagAction::setOutputFile(QFile* file)
{
  m_file = file;
}

void TagAction::setInputFileName(const QString& fileName)
{
  m_inputFileName = fileName;
}

QString TagAction::actionText()
{
   QString t = tag.attribute("text");
   int pos = t.find('&');
   if (pos < (int)t.length()-1 && t[pos+1] != '&')
     return t.remove(pos, 1);
   else
     return t;
}


void TagAction::slotProcessExited(KProcess *)
{
  if (loopStarted)
  {
    qApp->exit_loop();
    loopStarted = false;
  }
  m_appMessages->showMessage( i18n("The \"%1\" script has exited.").arg(actionText()) );
}

void TagAction::addArguments(const QStringList &arguments)
{
  m_argsList = arguments;
}

void TagAction::execute()
{
  if (insertTag(!m_inputFileName.isEmpty(), m_file))
  {
    //To avoid lock-ups, start a timer.
      timer = new QTimer(this);
      connect(timer, SIGNAL(timeout()), SLOT(slotTimeout()));
      timer->start(180*1000, true);
      QExtFileInfo internalFileInfo;
      loopStarted = true;
      m_killCount = 0;
      internalFileInfo.enter_loop();
      delete timer;
  }
}

/** Timeout occurred while waiting for some network function to return. */
void TagAction::slotTimeout()
{
  if ((m_killCount == 0) && (KMessageBox::questionYesNo(quantaApp, i18n("<qt>The filtering action <b>%1</b> seems to be locked.<br>Do you want to terminate it?</qt>").arg(actionText()), i18n("Action Not Responding")) == KMessageBox::Yes))
  {
    if (::kill(-proc->pid(), SIGTERM))
    {
      m_killCount++;
      return;
    }
  }
  if (m_killCount > 0)
  {
    ::kill(-proc->pid(), SIGKILL);
    if (loopStarted)
    {
        qApp->exit_loop();
        loopStarted = false;
    }
    return;
  }
  timer->start(180*1000, true);
}

#ifdef BUILD_KAFKAPART
void TagAction::insertOutputInTheNodeTree(QString str1, QString str2, Node *node)
{
        QuantaView *view = ViewManager::ref()->activeView();
#ifdef LIGHT_DEBUG
	if(node)
		kdDebug(25001)<< "TagAction::insertOutputInTheNodeTree() - node : " << node->tag->name <<
			" - type : " << node->tag->type << endl;
	else
		kdDebug(25001)<< "TagAction::insertOutputInTheNodeTree() - str1 : " << str1 <<
			" - str2 : " << str2 << endl;
#endif
	KafkaDocument *wkafka = KafkaDocument::ref();
	KafkaWidget *kafkaPart = wkafka->getKafkaWidget();
	NodeModifsSet *modifs;
	DOM::Node domNode, domStartContainer, domEndContainer;
	QString tagName;
	QTag *nodeQTag, *qTag, *nodeParentQTag;
	Node *nodeCursor, *startContainer, *endContainer, *nodeParent, *dummy;
	QPtrList<QTag> qTagList;
	int nodeOffset, domNodeOffset, startCol, startLine, endCol, endLine;
	bool specialTagInsertion = false;
	long startOffset, endOffset, longDomNodeOffset;
	QValueList<int> loc;
        uint line, col;
        bool smartTagInsertion, hasSelection, nodeTreeModified;

	if(!node && str1 == "" || node && str1 != "")
		return;
                
        //Three cases :
        //- Tag insertion in VPL
        //- Normal tag insertion in kate
        //- Smart tag insertion in kate
        smartTagInsertion = (view->hadLastFocus() == QuantaView::SourceFocus && qConfig.smartTagInsertion);
        
        if(view->hadLastFocus() == QuantaView::VPLFocus || smartTagInsertion)
	{
		modifs = new NodeModifsSet();
		if(!node && str1 != "")
		{
			//We build the node from the str1
			node = kafkaCommon::createNode("", "", Tag::XmlTag, view->document());
			node->tag->parse(str1, view->document());
			node->tag->name = QuantaCommon::tagCase(node->tag->name);
			node->tag->single = QuantaCommon::isSingleTag(view->document()->defaultDTD()->name,
				node->tag->name);
		}
                if(view->hadLastFocus() == QuantaView::VPLFocus)
                {
                  kafkaPart->getCurrentNode(domNode, domNodeOffset);
                  nodeCursor = wkafka->getNode(domNode);
                }
                else
                {
                  view->document()->viewCursorIf->cursorPositionReal(&line, &col);
                  nodeCursor = parser->nodeAt(line, col, false);
                }

		if (!nodeCursor || !view->document())
			return;

		nodeParent = nodeCursor;
		if(nodeParent->tag->type == Tag::Text)
			nodeParent = nodeParent->parent;

		//Checking if at least one parent of node can have a Text Node as child, otherwise
		//it is impossible for the
		//user to add this node. In that case, try to insert the Node in the closest parent accepting it.
		//e.g. TR : a normal insertion would require to have the caret in the TABLE Node, but it is 
                //impossible
		nodeQTag = QuantaCommon::tagFromDTD(view->document()->defaultDTD(),
			node->tag->name);

		if(!nodeQTag)
			return;

		qTagList = nodeQTag->parents();
#ifdef HEAVY_DEBUG
		kdDebug(25001)<< "nodeQTag name : " << nodeQTag->name() << endl;
		/**kdDebug(25001)<< nodeQTag->isChild("#text", false) << endl;
		kdDebug(25001)<< nodeQTag->isChild("#text", true) << endl;*/
#endif
		for(qTag = qTagList.first(); qTag; qTag = qTagList.next())
		{
			if(qTag->isChild("#text", false))
				break;
			if(qTag == qTagList.getLast())
				specialTagInsertion = true;
		}
                
                if(view->hadLastFocus() == QuantaView::VPLFocus)
                {
                  nodeOffset = wkafka->translateKafkaIntoNodeCursorPosition(domNode, domNodeOffset);
                  kafkaPart->selection(domStartContainer, startOffset, domEndContainer, endOffset);
                  startContainer = wkafka->getNode(domStartContainer);
                  endContainer = wkafka->getNode(domEndContainer);
                  startOffset = wkafka->translateKafkaIntoNodeCursorPosition(domStartContainer, startOffset);
                  endOffset = wkafka->translateKafkaIntoNodeCursorPosition(domEndContainer, endOffset);
                  hasSelection = kafkaPart->hasSelection();
                }
                else
                {
                  wkafka->translateQuantaIntoNodeCursorPosition(line, col, &dummy, nodeOffset);
                  startCol = view->document()->selectionIfExt->selStartCol();
                  startLine = view->document()->selectionIfExt->selStartLine();
                  endCol = view->document()->selectionIfExt->selEndCol();
                  endLine = view->document()->selectionIfExt->selEndLine();
                  wkafka->translateQuantaIntoNodeCursorPosition((unsigned)startLine, (unsigned)startCol,
                   &startContainer, (int&)startOffset);
                  wkafka->translateQuantaIntoNodeCursorPosition((unsigned)endLine, (unsigned)endCol,
                   &endContainer, (int&)endOffset);
                   hasSelection = view->document()->selectionIf->hasSelection();
                   if(startContainer == endContainer && startContainer->tag->type == Tag::Empty)
                   {
                     hasSelection = false;
                   }
                   if(endContainer && endContainer->tag->type == Tag::XmlTag && endOffset < (signed)endContainer->tag->tagStr().length())
                   {
                     endContainer = endContainer->previousSibling();
                     endOffset = (endContainer)?endContainer->tag->tagStr().length():0;
                   }
                   /**else
                   {
                     if(startContainer && startContainer->tag->type == Tag::Empty)
                       startContainer = startContainer->nextNE();
                     if(endContainer && endContainer->tag->type == Tag::Empty)
                       endContainer = endContainer->prevNE();
                   }*/
                }

                nodeTreeModified = false;
		if(specialTagInsertion)
		{
			//let's try to insert this node in the closest parent accepting it.
			while(nodeParent)
			{
				nodeParentQTag =
					QuantaCommon::tagFromDTD(view->document()->defaultDTD(),
					nodeParent->tag->name);
				if(nodeParentQTag && nodeParentQTag->isChild(node))
				{
					nodeCursor = kafkaCommon::createMandatoryNodeSubtree(node,
						view->document());
					nodeOffset = 0;
					kafkaCommon::insertNodeSubtree(node, nodeParent, 0L, 0L, modifs);
                                        nodeTreeModified = true;
					break;
				}
				nodeParent = nodeParent->parent;
			}
		}
		else if(hasSelection && !nodeQTag->isSingle())
		{
			//If some text is selected in kafka, surround the selection with the new Node.
			if(!startContainer || !endContainer)
				return;
			nodeTreeModified = kafkaCommon::DTDinsertRemoveNode(node, startContainer, (int)startOffset,
				endContainer, (int)endOffset, view->document(), &nodeCursor,
					nodeOffset, modifs);
		}
		else
		{
			//Nothing is selected, simply inserting the Node if it is not an inline.
			if(!kafkaCommon::isInline(node->tag->name) || nodeQTag->isSingle())
			{
				nodeTreeModified = kafkaCommon::DTDinsertRemoveNode(node, nodeCursor, (int)nodeOffset, nodeCursor,
					(int)nodeOffset, view->document(), &nodeCursor, nodeOffset, modifs);
			}
		}

		view->document()->docUndoRedo->addNewModifsSet(modifs, undoRedo::NodeTreeModif);
                if(view->hadLastFocus() == QuantaView::VPLFocus)
                {
                  //view->reloadVPLView();
                  //Now update the VPL cursor position
                  wkafka->translateNodeIntoKafkaCursorPosition(nodeCursor, nodeOffset, domNode,
                    longDomNodeOffset);
                  if(!domNode.isNull() && domNode.nodeType() != DOM::Node::TEXT_NODE &&
                    !domNode.firstChild().isNull() && domNode.firstChild().nodeType() ==
                    DOM::Node::TEXT_NODE)
                    domNode = domNode.firstChild();
                  if(!domNode.isNull())
                    wkafka->getKafkaWidget()->setCurrentNode(domNode, (int)longDomNodeOffset);
                }
                else
                {
                  //view->reloadSourceView();
                  //Now update the source cursor position
                  wkafka->translateNodeIntoQuantaCursorPosition(nodeCursor, nodeOffset, line, col);
                  view->document()->viewCursorIf->setCursorPositionReal(line, col);
                }
                if(!nodeTreeModified)
                  quantaApp->slotStatusMsg(i18n("Can't insert the tag : Invalid location."));

        }
        else
        {
          view->document()->insertTag(str1, str2);
        }
}
#endif

#include "tagaction.moc"
#include "myprocess.moc"
