/***************************************************************************
                          tagaction.cpp  -  description
                             -------------------
    begin                : ?
    copyright            : (C) ? Dmitry Poplavsky
                           (C) 2002-2005 Andras Mantia <amantia@kde.org>
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
#include "undoredo.h"
#include "tag.h"
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

TagAction::TagAction( QDomElement *element, KMainWindow *parentMainWindow)
  : KAction(element->attribute("text").isEmpty() ? QString("") : i18n(element->attribute("text").utf8()), KShortcut(element->attribute("shortcut")), 0, 0, parentMainWindow->actionCollection(), element->attribute("name"))
{
  m_parentMainWindow = parentMainWindow;
  m_modified = false;
  m_useInputFile = false;
  m_useOutputFile = false;
  tag = element->cloneNode().toElement();
  QString s = tag.attribute("icon");
  if (!QFileInfo(s).exists())
  {
    s = QFileInfo(s).fileName();
  }
  setIcon(s);
  m_file = 0L;
  loopStarted = false;
  connect(this, SIGNAL(activated()), SLOT(slotActionActivated()));
  connect(this, SIGNAL(showMessage(const QString&, bool)), m_parentMainWindow, SIGNAL(showMessage(const QString&, bool)));
  connect(this, SIGNAL(clearMessages()), m_parentMainWindow, SIGNAL(clearMessages()));
  connect(this, SIGNAL(showMessagesView()), m_parentMainWindow, SLOT(slotShowMessagesView()));
  connect(this, SIGNAL(createNewFile()), m_parentMainWindow, SLOT(slotFileNew()));
}

TagAction::~TagAction()
{
}

QString TagAction::type()
{
   return tag.attribute("type","");
}

bool TagAction::slotActionActivated()
{
  QuantaView *view = ViewManager::ref()->activeView();
  if ( !view || !view->document())
     return false;

  QString space="";
  QString output;
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
       view->insertNewTag(name, attr, xtag.attribute("inLine","true") == "true");
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
           /** FIXME this is quick and temporary */
           view->insertOutputInTheNodeTree(s1, s2);
         }
         else
         {
           view->insertOutputInTheNodeTree(s1, s2);
         }
       }
       else
         view->insertOutputInTheNodeTree(s1, s2);
     }
  }

  if (view->hadLastFocus() != QuantaView::VPLFocus)
  {


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
      if ( w->url().protocol() == "file")
        fname = w->url().path();
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
    if ( inputType == "selected" && w->selectionIf) {
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
      QStringList argsList1 = QStringList::split(' ', args);
      QStringList argsList;
      for (uint i = 0; i < argsList1.count(); i++)
      {
        if (argsList1[i] == "%userarguments")
        {
          for (uint j = 0; j < m_argsList.count(); j++)
          {
            argsList.append(m_argsList[j]);
          }      
        } else
          argsList.append(argsList1[i]);
      }
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



    if (!m_useOutputFile)
        scriptOutputDest = script.attribute("output","none");
    else
        scriptOutputDest = "file";
    scriptErrorDest  = script.attribute("error","none");
    if (scriptOutputDest == "message")
    {
      emit showMessagesView();
    }

    if (m_useInputFile)
    {
      *proc << m_inputFileName;
    }

    if (proc->start(KProcess::NotifyOnExit, KProcess::All))
    {
      emit clearMessages();
      emit showMessage(i18n("The \"%1\" script started.\n").arg(actionText()), false);
      if (!m_useInputFile)
      {
        if ( inputType == "current" || inputType == "selected" )
        {
          proc->writeStdin( buffer.local8Bit(), buffer.length() );
        }
      }
      proc->closeStdin();
    } else
    {
      KMessageBox::error(m_parentMainWindow, i18n("<qt>There was an error running <b>%1</b>.<br>Check that you have the <i>%2</i> executable installed and it is accessible.</qt>").arg(command + " " + args).arg(command), i18n("Script Not Found"));
      ViewManager::ref()->activeView()->setFocus();
      if (loopStarted)
      {
          qApp->exit_loop();
          loopStarted = false;
      }
      return false;
    }
  }
 }
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
       if (w->selectionIf) 
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
    if (firstOutput)
    {
        emit createNewFile();
        w = ViewManager::ref()->activeDocument();
    }
    w->insertTag( text );
  } else
  if ( scriptOutputDest == "message" )
  {
    if ( firstOutput )
    {
      emit showMessagesView();
      emit showMessage(i18n("The \"%1\" script output:\n").arg(actionText()), false);
    }
    emit showMessage(text, true);
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
       if (w->selectionIf)
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
    if (firstError)
    {
      emit createNewFile();
      w = ViewManager::ref()->activeDocument();
    }
    w->insertTag( text );
  } else
  if ( scriptErrorDest == "message" )
  {
    if ( firstError )
    {
      emit showMessagesView();
      emit showMessage(i18n("The \"%1\" script output:\n").arg(actionText()), false);
    }
    emit showMessage(text, true);
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


void TagAction::slotProcessExited(KProcess *process)
{
  if (loopStarted)
  {
    qApp->exit_loop();
    loopStarted = false;
  }
  emit showMessage(i18n("The \"%1\" script has exited.").arg(actionText()), false);
  delete process;
}

void TagAction::addArguments(const QStringList &arguments)
{
  m_argsList = arguments;
}

void TagAction::execute(bool blocking)
{
  m_useInputFile = false;
  m_useOutputFile = false;
  if (blocking)
  {
    m_useInputFile = !m_inputFileName.isEmpty();
    m_useOutputFile = (m_file);
    if (slotActionActivated())
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
        m_useInputFile = false;
        m_useOutputFile = false;
    }
  } else
    slotActionActivated();
}

/** Timeout occurred while waiting for some network function to return. */
void TagAction::slotTimeout()
{
  if ((m_killCount == 0) && (KMessageBox::questionYesNo(m_parentMainWindow, i18n("<qt>The filtering action <b>%1</b> seems to be locked.<br>Do you want to terminate it?</qt>").arg(actionText()), i18n("Action Not Responding")) == KMessageBox::Yes))
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

#include "tagaction.moc"
#include "myprocess.moc"
