/***************************************************************************
                          tagaction.cpp  -  description
                             -------------------
    begin                : ?
    copyright            : (C) ? Dmitry Poplavsky
                           (C) 2002-2003 Andras Mantia <amantia@freemail.hu>
    email                : amantia@freemail.hu
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
#include <kdebug.h>
#include <kprocess.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kshortcut.h>
#include <kstandarddirs.h>
#include <ktempfile.h>
#include <ktexteditor/cursorinterface.h>
#include <ktexteditor/viewcursorinterface.h>
#include <ktexteditor/editinterface.h>
#include <ktexteditor/selectioninterface.h>

//app includes
#include "tagaction.h"
#include "myprocess.h"
#include "../document.h"
#include "../quantaview.h"
#include "../quanta.h"
#include "../quantadoc.h"
#include "../tagdialogs/tagdialog.h"
#include "../messages/messageoutput.h"
#include "../quantacommon.h"
#include "../resource.h"
#include "../qextfileinfo.h"
#ifdef BUILD_KAFKAPART
#include "../parts/kafka/kafkacommon.h"
#include "../parts/kafka/kafkahtmlpart.h"
#include "../parts/kafka/wkafkapart.h"
#include "../parts/kafka/undoredo.h"
#include "../parser/tag.h"
#endif

MyProcess::MyProcess():KProcess()
{
}

int MyProcess::commSetupDoneC()
{
  ::setpgid(pid_, 0);
  return KProcess::commSetupDoneC();
}

TagAction::TagAction( QDomElement *element, KActionCollection *parent)
  : KAction( element->attribute("text"), KShortcut(element->attribute("shortcut")), 0, 0, parent, element->attribute("name") )
{
  m_modified = false;
  tag = element->cloneNode().toElement();
  m_view = quantaApp->view();
  QString s = tag.attribute("icon");
  if (!QFileInfo(s).exists())
  {
    s = QFileInfo(s).fileName();
  }
  setIcon( s );
  loopStarted = false;
  m_appMessages = quantaApp->getMessageOutput();
  if ( m_view )
      connect( this, SIGNAL(activated()), SLOT(insertTag()) );
}

TagAction::~TagAction()
{
}

void TagAction::insertTag(bool inputFromFile, bool outputToFile)
{
  if ( !m_view || !m_view->writeExists())
     return;

  QString space="";
#ifdef BUILD_KAFKAPART
  QString output;
#endif
  unsigned int line, col;

  Document *w = m_view->write();
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

     if ( otag.attribute("useDialog","false") == "true" && QuantaCommon::isKnownTag(w->defaultDTD()->name, name))
     {
#ifdef BUILD_KAFKAPART

#else
       m_view->insertNewTag(name, attr, xtag.attribute("inLine","true") == "true");
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
       if (qConfig.closeTags)
          s2 = QuantaCommon::tagCase(xtag.text());
       if ( xtag.attribute("use","false") == "true" )
       {
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
    command.replace("%pid", QString("%1").arg(pid));
    QString buffer;
    QString inputType = script.attribute("input","none");

    if ( inputType == "current" ) {
      buffer = w->editIf->text();
    } else
    if ( inputType == "selected" ) {
      buffer = w->selectionIf->selection();
    }
    command.replace("%input", buffer);

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
        scriptname = args.mid(begin, end - begin);
        scriptname.replace("%scriptdir","scripts");
//        kdDebug(24000) << "Script name is: " << scriptname << endl;
        scriptname = locate("appdata", scriptname);
//        kdDebug(24000) << "Script found at: " << scriptname << endl;
        args.replace(begin, end - begin, scriptname);
//        kdDebug(24000) << "Modified argument list: " << args << endl;
      }
      *proc << args;
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
      quantaApp->slotShowBottDock(true);
    }

    if (inputFromFile)
    {
      *proc << m_inputFileName;
    }

    if (proc->start(KProcess::NotifyOnExit, KProcess::All))
    {
      m_appMessages->clear();
      m_appMessages->showMessage( i18n("\"%1\" script started.\n").arg(tag.attribute("text")) );
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
      KMessageBox::error(quantaApp, i18n("<qt>There was an error running <b>%1</b>.<br>Check that you have the <i>%2</i> executable installed and it is accessible!</qt>").arg(command + " " + args).arg(command), i18n("Script not found"));
    }
  }

}

void TagAction::slotGetScriptOutput( KProcess *, char *buffer, int buflen )
{
  kdDebug(24000) << "Script output received." << endl;
  QCString tmp( buffer, buflen + 1 );
  QString text( QString::fromLocal8Bit(tmp) );
  Document *w = m_view->write();
  if (!w)
  {
    kdDebug(24000) << "Document not found!" << endl;
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
        quantaApp->doc()->openDocument( KURL() );
        m_view = quantaApp->view();
        w = m_view->write();
    }
    w->insertTag( text );
  } else
  if ( scriptOutputDest == "message" )
  {
    if ( firstOutput )
    {
      m_appMessages->showMessage( i18n( "\"%1\" script output:\n" ).arg(tag.attribute("text")) );
    }
    m_appMessages->showMessage( text );
  } else
  if ( scriptOutputDest == "file" )
  {
    if (!m_file->isOpen())
       m_file->open(IO_ReadWrite);
    m_file->writeBlock(buffer, buflen);
  }

  firstOutput = false;
}

void TagAction::slotGetScriptError( KProcess *, char *buffer, int buflen )
{
  Document *w = m_view->write();
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
        quantaApp->doc()->openDocument( KURL() );
        m_view = quantaApp->view();
        w = m_view->write();
    }
    w->insertTag( text );
  } else
  if ( scriptErrorDest == "message" )
  {
    if ( firstError )
    {
      quantaApp->slotShowBottDock(true);
      m_appMessages->showMessage( i18n( "\"%1\" script output:\n" ).arg(tag.attribute("text")) );
    }
    m_appMessages->showMessage( text );
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

void TagAction::slotProcessExited(KProcess *)
{
  if (loopStarted)
  {
    qApp->exit_loop();
    loopStarted = false;
  }
  m_appMessages->showMessage( i18n("\nThe \"%1\" script has exited.\n").arg(tag.attribute("text")) );
}

void TagAction::execute()
{
  insertTag(true, true);
 //To avoid lock-ups, start a timer.
  timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), SLOT(slotTimeout()));
  timer->start(180*1000, true);
  QExtFileInfo internalFileInfo;
  loopStarted = true;
  internalFileInfo.enter_loop();
  delete timer;
}

/** Timeout occurred while waiting for some network function to return. */
void TagAction::slotTimeout()
{
  if (KMessageBox::warningYesNo(quantaApp, i18n("<qt>The filtering action <b>%1</b> seems to be locked.<br>Do you want to terminate it?</qt>").arg(tag.attribute("text")), i18n("Action not responding")) == KMessageBox::Yes)
  {
    if (::kill(-proc->pid(), SIGTERM))
    {
      return;
    }
  }
  timer->start(180*1000, true);
}

#ifdef BUILD_KAFKAPART
void TagAction::insertOutputInTheNodeTree(QString str1, QString str2)
{
#ifdef LIGHT_DEBUG
	kdDebug(25001)<< "TagAction::insertOutputInTheNodeTree() - str1 : " << str1 <<
		" - str2 : " << str2 << endl;
#endif
	KafkaHTMLPart *kafkaPart = m_view->getKafkaInterface()->getKafkaPart();
	WKafkaPart *wkafka = m_view->getKafkaInterface();
	NodeModifsSet modifs;
	DOM::Node domNode;
	DOM::Range range;
	QString tagName;
	QTag *qtag;
	Node *node, *startContainer, *endContainer;
	int offset;

	modifs.cursorX = 0;
	modifs.cursorY = 0;
	modifs.cursorX2 = 0;
	modifs.cursorY2 = 0;
	modifs.isModified = true;//TODO:determine this

	if(m_view->hadLastFocus() == QuantaView::kafkaFocus)
	{
		//m_view->reloadQuantaView();

		tagName = str1.mid(1, str1.length() - 2);
		kafkaPart->getCurrentNode(domNode, offset);
		node = wkafka->searchCorrespondingNode(domNode);
		kdDebug(25001)<< "BOO"<< endl;
		if(!node)
			return;
			kdDebug(25001)<< "BOO2"<< endl;
		qtag = QuantaCommon::tagFromDTD(quantaApp->view()->write()->defaultDTD(),
			domNode.nodeName().string());
		range = kafkaPart->selection();
		if(kafkaPart->hasSelection())
		{
		kdDebug(25001)<< "BOO3"<< endl;
			startContainer = wkafka->searchCorrespondingNode(range.startContainer());
			endContainer = wkafka->searchCorrespondingNode(range.endContainer());
			if(!startContainer || !endContainer)
				return;
				kdDebug(25001)<< "BOO4-2"<< endl;
			kafkaCommon::DTDcreateAndInsertNode(tagName, "", Tag::XmlTag, m_view->write(),
				startContainer, range.startOffset(), endContainer, range.endOffset(), modifs);
		}
		else
		{
		kdDebug(25001)<< "BOO4"<< endl;
			if(kafkaCommon::getNodeType(tagName) != kafkaCommon::inlineDisplay)
			{
				if((qtag && qtag->isChild(tagName)) || domNode.nodeType() == DOM::Node::TEXT_NODE)
				{
					kafkaCommon::splitNode(node, offset, modifs);
					kafkaCommon::createAndInsertNode(tagName, "", Tag::XmlTag, m_view->write(),
						node->parent, node->next, node->next, modifs);
				}
			}
		}

		m_view->write()->docUndoRedo->addNewModifsSet(modifs, undoRedo::NodeTreeModif);
		m_view->reloadKafkaView();
	}
	else
		m_view->write()->insertTag(str1, str2);
}

void TagAction::insertTag(QString , QString )
{
	//TEMPORARY and dirty Function to insert Tag.
	//It reload quanta, insert the tag if necessary/possible and refresh kafka

}
#endif

#include "tagaction.moc"
#include "myprocess.moc"
