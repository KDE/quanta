/***************************************************************************
                          tagaction.cpp  -  description
                             -------------------
    begin                : ?
    copyright            : (C) ? Dmitry Poplavsky, (C) 2002 by Andras Mantia
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

//kde includes
#include <kprocess.h>
#include <klocale.h>
#include <kdeversion.h>
#include <ktexteditor/cursorinterface.h>
#include <ktexteditor/viewcursorinterface.h>
#include <ktexteditor/editinterface.h>
#include <ktexteditor/selectioninterface.h>

#if (KDE_VERSION >= 309)
#include <ktexteditor/selectioninterfaceext.h>
#endif

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

TagAction::TagAction( QDomElement *element, KActionCollection *parent)
  : KAction( element->attribute("text"), 0, 0, 0, parent, element->attribute("name") ),
    tag(*element)
{
   m_view = quantaApp->getView();
   setIcon( tag.attribute("icon","") );

   if ( m_view )
        connect( this, SIGNAL(activated()), SLOT(insertTag()) );
}

TagAction::TagAction( QDomElement *element, QuantaView *a_view, KActionCollection *parent)
  : KAction( element->attribute("text"), 0, 0, 0, parent, element->attribute("name") ),
    tag(*element)
{
   m_view = a_view;
   setIcon( tag.attribute("icon","") );

   if ( m_view )
        connect( this, SIGNAL(activated()), SLOT(insertTag()) );
}


TagAction::~TagAction()
{
}



void TagAction::insertTag()
{
  if ( !m_view )
     return;

  QString space="";
  unsigned int line, col;

  Document *w = m_view->write();
  dynamic_cast<KTextEditor::ViewCursorInterface *> (w->view())->cursorPosition(&line, &col);
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
     while ( !attr[i].isSpace() && !attr[i].isNull() )	i++;
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

  if ( type == "script" ) {

   	KProcess *proc = new KProcess();
    proc ->clearArguments();

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

    if (scriptOutputDest == "message" || scriptErrorDest == "message")
    {
      quantaApp->slotShowBottDock(true);
    }

    proc->start(KProcess::NotifyOnExit, KProcess::All);

    QString buffer;

    QString inputType = script.attribute("input","none");
    scriptOutputDest = script.attribute("output","none");
    scriptErrorDest  = script.attribute("error","none");

    if ( inputType == "current" ) {
    	buffer = dynamic_cast<KTextEditor::EditInterface*> (w->doc())->text();
      proc->writeStdin( buffer.local8Bit(), buffer.length() );
    }

    if ( inputType == "selected" ) {
    	buffer = dynamic_cast<KTextEditor::SelectionInterface*>(w->doc())->selection();
      proc->writeStdin( buffer.local8Bit(), buffer.length() );
    }
    proc->closeStdin();
  }

}

void TagAction::slotGetScriptOutput( KProcess *, char *buffer, int buflen )
{
  QCString tmp( buffer, buflen + 1 );
  QString text( QString::fromLocal8Bit(tmp) );
  Document *w = m_view->write();

  if ( scriptOutputDest == "cursor" )
  {
     if ( firstOutput )
     {
#if (KDE_VERSION >= 309)
       int line = dynamic_cast<KTextEditor::SelectionInterfaceExt*>(w->doc())->selEndLine();
       int col = dynamic_cast<KTextEditor::SelectionInterfaceExt*>(w->doc())->selEndCol();
       w->viewCursorIf->setCursorPositionReal(line, col);     
#endif
       dynamic_cast<KTextEditor::SelectionInterface*>(w->doc())->removeSelectedText();
     }
     w->insertTag( text );
  }
  if ( scriptOutputDest == "replace" )
  {
    if ( firstOutput )
       dynamic_cast<KTextEditor::EditInterface*>(w->doc())->clear();
    w->insertTag( text );
  }

  if ( scriptOutputDest == "new" )
  {
    if ( firstOutput )
    {
        quantaApp->getDoc()->openDocument( KURL() );
        m_view = quantaApp->getView();
        w = m_view->write();
    }
    w->insertTag( text );
  }

  if ( scriptOutputDest == "message" )
  {
    MessageOutput *appMessages = quantaApp->getMessages();
    if ( firstOutput )
    {
      appMessages->clear();
      appMessages->insertItem( i18n( "Script output:\n" ) );
    }
    appMessages->showMessage( text );
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
  {
     w->insertTag( text );
  }
  if ( scriptErrorDest == "replace" )
  {
    if ( firstOutput )
       dynamic_cast<KTextEditor::EditInterface*>(w->doc())->clear();
    w->insertTag( text );
  }

  if ( scriptErrorDest == "new" )
  {
    if ( firstOutput )
    {
        quantaApp->getDoc()->openDocument( KURL() );
        m_view = quantaApp->getView();
        w = m_view->write();
    }
    w->insertTag( text );
  }

  if ( scriptErrorDest == "message" )
  {
    MessageOutput *appMessages = quantaApp->getMessages();
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





#include "tagaction.moc"
