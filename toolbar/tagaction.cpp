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

//qt includes
#include <qdom.h>
#include <qfile.h>
#include <qtimer.h>

//kde includes
#include <kdebug.h>
#include <kprocess.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kshortcut.h>
#include <ktempfile.h>
#include <ktexteditor/cursorinterface.h>
#include <ktexteditor/viewcursorinterface.h>
#include <ktexteditor/editinterface.h>
#include <ktexteditor/selectioninterface.h>

//app includes
#include "tagaction.h"
#include "../document.h"
#include "../quantaview.h"
#include "../quanta.h"
#include "../quantadoc.h"
#include "../tagdialogs/tagdialog.h"
#include "../messages/messageoutput.h"
#include "../quantacommon.h"
#include "../resource.h"
#include "../qextfileinfo.h"

TagAction::TagAction( QDomElement *element, KActionCollection *parent)
  : KAction( element->attribute("text"), KShortcut(element->attribute("shortcut")), 0, 0, parent, element->attribute("name") )
{
  m_modified = false;
  tag = element->cloneNode().toElement();
  m_view = quantaApp->view();
  setIcon( tag.attribute("icon","") );
  loopStarted = false;
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
       m_view->insertNewTag(name, attr, xtag.attribute("inLine","true") == "true");
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
           w->insertTag( s1, s2 );
         }
         else
         {
           w->insertTag( s1+"\n"+space+"  ", "\n"+space+s2 );
         }
       }
       else
         w->insertTag( s1 );
     }
  }

  if ( type == "text" )
    w->insertTag( tag.namedItem("text").toElement().text() );

  if ( type == "script" )
  {
    KProcess *proc = new KProcess();
    proc->clearArguments();
    proc->setWorkingDirectory(quantaApp->projectBaseURL().path());

    QDomElement script = tag.namedItem("script").toElement();
    QString command = script.text();


    if ( !w->isUntitled() ) {
      QString fname = w->url().url();
      if ( fname.left(5) == "file:")
        fname.remove(0,5);
      command.replace( QRegExp("%f"), fname );
    }

    int pos = command.find(' ');
    QString args;
    if (pos != -1)
    {
      args = command.mid(pos+1);
      command = command.left(pos);
    }
    *proc << command.stripWhiteSpace();
    args = args.stripWhiteSpace();
    if (!args.isEmpty())
      *proc << args;

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
    if (scriptOutputDest == "message" || scriptErrorDest == "message")
    {
      quantaApp->slotShowBottDock(true);
    }

    if (inputFromFile)
    {
      *proc << m_inputFileName;
    }

    if (proc->start(KProcess::NotifyOnExit, KProcess::All))
    {
      if (!inputFromFile)
      {
        QString buffer;
        QString inputType = script.attribute("input","none");

        if ( inputType == "current" ) {
          buffer = w->editIf->text();
          proc->writeStdin( buffer.local8Bit(), buffer.length() );
        } else
        if ( inputType == "selected" ) {
          buffer = w->selectionIf->selection();
          proc->writeStdin( buffer.local8Bit(), buffer.length() );
        }
      }
      proc->closeStdin();
    } else
    {
      KMessageBox::error(quantaApp, i18n("There was an error running \"%1\".\nCheck that you have the executable installed and in the PATH!").arg(command + " " + args), i18n("Script not found"));
    }
    kdDebug(24000) << "Script started\n";
  }

}

void TagAction::slotGetScriptOutput( KProcess *, char *buffer, int buflen )
{
  QCString tmp( buffer, buflen + 1 );
  QString text( QString::fromLocal8Bit(tmp) );
  Document *w = m_view->write();
  kdDebug(24000) << "Script output" << endl;
  if ( scriptOutputDest == "cursor" )
  {
     if ( firstOutput )
     {
#if (KDE_VERSION >= 309)
       int line = dynamic_cast<KTextEditor::SelectionInterfaceExt*>(w->doc())->selEndLine();
       int col = dynamic_cast<KTextEditor::SelectionInterfaceExt*>(w->doc())->selEndCol();
       w->viewCursorIf->setCursorPositionReal(line, col);
#endif
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
    MessageOutput *appMessages = quantaApp->getMessageOutput();
    if ( firstOutput )
    {
      appMessages->clear();
      appMessages->insertItem( i18n( "Script output:\n" ) );
    }
    appMessages->showMessage( text );
  } else
  if ( scriptOutputDest == "file" )
  {
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

  if ( scriptErrorDest == "replace" )
  {
    if ( firstOutput )
       w->editIf->clear();
    w->insertTag( text );
  }

  if ( scriptErrorDest == "new" )
  {
    if ( firstOutput )
    {
        quantaApp->doc()->openDocument( KURL() );
        m_view = quantaApp->view();
        w = m_view->write();
    }
    w->insertTag( text );
  }

  if ( scriptErrorDest == "message" )
  {
    MessageOutput *appMessages = quantaApp->getMessageOutput();
    if ( firstError )
    {
      appMessages->clear();
      appMessages->insertItem( i18n( "Script output:\n" ) );
    }
    appMessages->showMessage( text );
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
}

void TagAction::execute()
{
  insertTag(true, true);
 //To avoid lock-ups, start a timer.
  QTimer* timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), SLOT(slotTimeout()));
  timer->start(10*1000, true);
  QExtFileInfo internalFileInfo;
  loopStarted = true;
  internalFileInfo.enter_loop();
  delete timer;
}

/** Timeout occured while waiting for some network function to return. */
void TagAction::slotTimeout()
{
  qApp->exit_loop();
}


#include "tagaction.moc"
