
#include "tagaction.h"
#include "../quantaview.h"  
#include "../quanta.h"
#include "../quantadoc.h"
#include "../document.h"
#include "../tagdialogs/tagdialog.h"
#include "../messages/messageoutput.h"

#include <kprocess.h>
#include <qdom.h>
#include <klocale.h>

TagAction::TagAction( QDomElement *element, QObject* parent, const char* , QuantaView *view )
  : KAction( element->attribute("text",""), 0, parent, element->attribute("name","") ),
    tag(element),
    view_(view)
    
  
{
   setIcon( tag->attribute("icon","") );
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
	space.fill( ' ',view_->write()->currentColumn() );

  QString type = tag->attribute("type","");

  if ( type == "tag" ) {
     QDomElement otag = (tag->namedItem("tag")).toElement();
     QDomElement xtag = (tag->namedItem("xtag")).toElement();

     if ( otag.attribute("useDialog","false") == "true" ) {
         QString s = otag.text();

         if ( s[0] == '<' )
            s.remove(0,1);

         if ( s.right(1) == ">" )
            s.remove( s.length()-1, 1 );

         s = s.stripWhiteSpace();

         int i = 0;
         while ( !s[i].isSpace() && !s[i].isNull() )	i++;

         QString name = s.left(i);

         s = s.remove(0,i).stripWhiteSpace();

         TagDialog *dlg = new TagDialog( view_->write(), name, s, xtag.attribute("inLine","true") == "true" );
         dlg->show();

     }
     else {
       if ( xtag.attribute("use","false") == "true" ) {
         if ( xtag.attribute("inLine","true") == "true" )
           view_->write()->insertTag( otag.text(), xtag.text() );
         else
           view_->write()->insertTag( otag.text()+"\n"+space+"  ", "\n"+space+xtag.text() );
       }
       else
         view_->write()->insertTag( otag.text() );
     }
  }

  if ( type == "text" )
    view_->write()->insertTag( tag->namedItem("text").toElement().text() );

  if ( type == "script" ) {

   	KProcess *proc = new KProcess();
    proc ->clearArguments();

    QDomElement script = tag->namedItem("script").toElement();
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
    	buffer = view_->write()->text();
      proc->writeStdin( buffer.local8Bit(), buffer.length() );
    }

    if ( inputType == "selected" ) {
    	buffer = view_->write()->markedText();
      proc->writeStdin( buffer.local8Bit(), buffer.length() );
    }
    proc->closeStdin();
  }
    
}

void TagAction::slotGetScriptOutput( KProcess *, char *buffer, int buflen )
{
    QCString tmp( buffer, buflen );
    QString text( tmp );

    
    if ( scriptOutputDest == "cursor" )
        view_->write()->insertTag( text );
    
    if ( scriptOutputDest == "replace" ) {
        if ( firstOutput )
            view_->write()->setText("");
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
    QCString tmp( buffer, buflen );
    QString text( tmp );
    
    if ( scriptErrorDest == "merge" ) {
       	scriptErrorDest = scriptOutputDest;
  	      firstError = firstOutput;
    }
    
    if ( scriptErrorDest == "cursor" )
        view_->write()->insertTag( text );
    
    if ( scriptErrorDest == "replace" ) {
        if ( firstOutput )
            view_->write()->setText("");
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





