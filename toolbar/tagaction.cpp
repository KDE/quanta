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

TagAction::TagAction( QDomElement *element, QuantaView *view,KActionCollection *collection )
  : KAction( element->attribute("text"), 0, collection, element->attribute("name") ),
    tag(*element),
    view_(view)
{
   setIcon( tag.attribute("icon","") );

   if ( view_ )
        connect( this, SIGNAL(activated()), SLOT(insertTag()) );
}


TagAction::~TagAction()
{
}



void TagAction::insertTag()
{
  if ( !view_ )
     return;

  QString space="";
  unsigned int line, col;

  dynamic_cast<KTextEditor::ViewCursorInterface *> (view_->write()->view())->cursorPosition(&line, &col);
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

     if ( otag.attribute("useDialog","false") == "true" )
    {

         view_->insertNewTag(name, attr, xtag.attribute("inLine","true") == "true");
//         TagDialog *dlg = new TagDialog( view_->write(), name, s, xtag.attribute("inLine","true") == "true" );
//         dlg->show();

     }
     else
     {
       QString s1 = QuantaCommon::tagCase(name);
       if (otag.text().left(1) == "<") s1 = "<"+s1;
       if (!attr.isEmpty())
          s1 += " "+QuantaCommon::attrCase(attr);
       if (otag.text().right(1) == ">") s1 += ">";

       QString s2;
       if (useCloseTag)
          s2 = QuantaCommon::tagCase(xtag.text());
       if ( xtag.attribute("use","false") == "true" )
       {
         if ( xtag.attribute("inLine","true") == "true" )
         {
           view_->write()->insertTag( s1, s2 );
         }
         else
         {
           view_->write()->insertTag( s1+"\n"+space+"  ", "\n"+space+s2 );
         }
       }
       else
         view_->write()->insertTag( s1 );
     }
  }

  if ( type == "text" )
    view_->write()->insertTag( tag.namedItem("text").toElement().text() );

  if ( type == "script" ) {

   	KProcess *proc = new KProcess();
    proc ->clearArguments();

    QDomElement script = tag.namedItem("script").toElement();
    QString command = script.text();


    if ( !view_->write()->isUntitled() ) {
      QString fname = view_->write()->url().url();
      if ( fname.left(5) == "file:")
        fname.remove(0,5);
      command.replace( QRegExp("%f"), fname );
    }

    *proc << "sh";
    *proc << "-c" << command;

    firstOutput = true;
    firstError  = true;

    connect( proc, SIGNAL(receivedStdout(   KProcess*,char*,int)), this,
                 SLOT(  slotGetScriptOutput(KProcess*,char*,int)));

		connect( proc, SIGNAL(receivedStderr(   KProcess*,char*,int)), this,
                 SLOT(  slotGetScriptError(KProcess*,char*,int)));

    proc->start(KProcess::NotifyOnExit, KProcess::All);

    QString buffer;

    QString inputType = script.attribute("input","none");
    scriptOutputDest = script.attribute("output","none");
    scriptErrorDest  = script.attribute("error","none");

    if ( inputType == "current" ) {
    	buffer = dynamic_cast<KTextEditor::EditInterface*> (view_->write()->doc())->text();
      proc->writeStdin( buffer.local8Bit(), buffer.length() );
    }

    if ( inputType == "selected" ) {
    	buffer = dynamic_cast<KTextEditor::SelectionInterface*>(view_->write()->doc())->selection();
      proc->writeStdin( buffer.local8Bit(), buffer.length() );
    }
    proc->closeStdin();
  }

}

void TagAction::slotGetScriptOutput( KProcess *, char *buffer, int buflen )
{
    QCString tmp( buffer, buflen + 1 );
    QString text( tmp );


    if ( scriptOutputDest == "cursor" )
        view_->write()->insertTag( text );

    if ( scriptOutputDest == "replace" ) {
        if ( firstOutput )
            dynamic_cast<KTextEditor::EditInterface*>(view_->write()->doc())->clear();
        view_->write()->insertTag( text );
    }

    if ( scriptOutputDest == "new" ) {
        if ( firstOutput )
            view_->getDoc()->openDocument( KURL() );
        view_->write()->insertTag( text );
    }

    if ( scriptOutputDest == "message" ) {
        if ( firstOutput ) {
            view_->getApp()->getMessages()->clear();
            view_->getApp()->getMessages()->insertItem( i18n( "Script output:\n" ) );
        }

        view_->getApp()->getMessages()->showMessage( text );
    }

    firstOutput = false;
}

void TagAction::slotGetScriptError( KProcess *, char *buffer, int buflen )
{
    QCString tmp( buffer, buflen + 1 );
    QString text( tmp );

    if ( scriptErrorDest == "merge" ) {
       	scriptErrorDest = scriptOutputDest;
  	      firstError = firstOutput;
    }

    if ( scriptErrorDest == "cursor" )
        view_->write()->insertTag( text );

    if ( scriptErrorDest == "replace" ) {
        if ( firstOutput )
            dynamic_cast<KTextEditor::EditInterface*>(view_->write()->doc())->clear();
        view_->write()->insertTag( text );
    }

    if ( scriptErrorDest == "new" ) {
        if ( firstOutput )
            view_->getDoc()->openDocument( KURL() );
        view_->write()->insertTag( text );
    }

    if ( scriptErrorDest == "message" ) {
        if ( firstError ) {
            view_->getApp()->getMessages()->clear();
            view_->getApp()->getMessages()->insertItem( i18n( "Script output:\n" ) );
        }

        view_->getApp()->getMessages()->showMessage( text );
    }



    firstError = false;
}

void TagAction::scriptDone()
{
    delete proc;
    proc = 0;
}





#include "tagaction.moc"
