#include <kprocess.h>
#include <klocale.h>
#include <qcstring.h>

#include "quantaview.h"
#include "quantadoc.h"
#include "quanta.h"
#include "document.h"

#include "messages/messageoutput.h"

#include "quanta_actions.h"

QuantaTagAction::QuantaTagAction( const QString& text, int accel,
                                  QObject* parent, const char* name,
                                  QuantaView *view )
    : KAction( text, accel, 0, 0, parent, name ),
      openTag_(),
      closeTag_(),
      allOnOneLine_(false),
      showTagDialog_(false),
      view_(view)
{
    if ( view_ )
        connect( this, SIGNAL(activated()), SLOT(insertTag()) );
}

QuantaTagAction::QuantaTagAction( const QString& text, const QString& pix, int accel,
                 QObject* parent, const char* name,
                 QuantaView *view )
    : KAction( text, pix, accel, 0, 0, parent, name ),
      openTag_(),
      closeTag_(),
      allOnOneLine_(false),
      showTagDialog_(false),
      view_(view)
{
    if ( view_ )
        connect( this, SIGNAL(activated()), SLOT(insertTag()) );
}

QuantaTagAction::~QuantaTagAction()
{
}

void QuantaTagAction::setTag( const QString &openTag, const QString &closeTag )
{
    this->openTag_ = openTag;
    this->closeTag_ = closeTag;
}

void QuantaTagAction::insertTag()
{
    if ( view_ ) {
        if ( closeTag_.isNull() )
            view_->insertTag( openTag_ );
        else
            view_->write()->insertTag( openTag_, closeTag_ );
    }
}

QuantaScriptAction::QuantaScriptAction( const QString& text, int accel,
                                        QObject* parent, const char* name,
                                        QuantaView *view )
    : KAction( text, accel, 0, 0, parent, name ),
      proc(0),
      view_(view)
{
    connect( this, SIGNAL(activated()), SLOT(runScript()) );
}

QuantaScriptAction::QuantaScriptAction( const QString& text, const QString& pix, int accel,
                                        QObject* parent, const char* name,
                                        QuantaView *view )
    : KAction( text, pix, accel, 0, 0, parent, name ),
      proc(0),
      view_(view)
{
    connect( this, SIGNAL(activated()), SLOT(runScript()) );
}

QuantaScriptAction::~QuantaScriptAction()
{
    if ( proc )
        delete proc;
}

void QuantaScriptAction::runScript()
{
    //
    // Build the command string
    //
    QString command = filename_;

    if ( view_->write()->hasFileName() ) {
      QString docname = view_->write()->fileName();
      if ( docname.left(5) == "file:" )
        docname.remove(0,5);
      command.replace( QRegExp("%f"), docname );
    }

    //
    // Fire off the process
    //
  	proc = new KProcess();
    *proc << "sh" << "-c" << command;

    if ( output_ != NoOutput ) {
        connect( proc, SIGNAL( receivedStdout( KProcess *,char*,int) ), 
                 this, SLOT( slotGetScriptOutput( KProcess *,char*,int ) ) );
    }

    if ( error_ != NoOutput ) {
        connect( proc, SIGNAL( receivedStderr(   KProcess *,char*,int) ),
                 this, SLOT( slotGetScriptError( KProcess *,char*,int ) ) );
    }
    
    connect( proc, SIGNAL( processExited( KProcess * ) ),
             this, SLOT( scriptDone() ) );

    proc->start( KProcess::NotifyOnExit, KProcess::All );

    firstError = firstOutput = true;

    //
    // Send the specified input
    //
    QCString buffer;

    switch ( input_ ) {
    case NoInput:
        break;
    case Selection:
    	buffer = view_->write()->markedText().local8Bit();
        break;
    case Document:
    	buffer = view_->write()->text().local8Bit();
        break;
    default:
        qWarning( "Invalid input specification %d", input_ );
        break;
    }

    if ( !buffer.isNull() ) {
        proc->writeStdin( buffer, buffer.length() );
    }

    proc->closeStdin();
}

void QuantaScriptAction::slotGetScriptOutput( KProcess *, char *buffer, int buflen )
{
    QCString tmp( buffer, buflen );
    QString text( tmp );

    switch ( output_ ) {
    case NoOutput:
        break;
    case InsertAtCursor:
        view_->write()->insertTag( text );
        break;
    case ReplaceDocument:
        if ( firstOutput )
            view_->write()->setText("");
        view_->write()->insertTag( text );
        break;
    case NewDocument:
        if ( firstOutput )
            view_->getDocument()->newDocument();
        view_->write()->insertTag( text );
        break;
    case MessageWindow:
        if ( firstOutput ) {
            view_->getApp()->getMessages()->clear();
            view_->getApp()->getMessages()->insertItem( i18n( "Script output:\n" ) );
        }

        view_->getApp()->getMessages()->showMessage( text );
        break;
    case MergeWithOutput:
        // Only makes sense for stderr
        break;
    default:
        break;
    };

    firstOutput = false;
}

void QuantaScriptAction::slotGetScriptError( KProcess *, char *buffer, int buflen )
{
    QCString tmp( buffer, buflen );
    QString text( tmp );

    switch ( error_ ) {
    case NoOutput:
        break;
    case InsertAtCursor:
        view_->write()->insertTag( text );
        break;
    case ReplaceDocument:
        if ( firstError )
            view_->write()->setText("");
        view_->write()->insertTag( text );
        break;
    case NewDocument:
        if ( firstError )
            view_->getDocument()->newDocument();
        view_->write()->insertTag( text );
        break;
    case MessageWindow:
        if ( firstError ) {
            view_->getApp()->getMessages()->clear();
            view_->getApp()->getMessages()->insertItem( i18n( "Script output:\n" ) );
        }

        view_->getApp()->getMessages()->showMessage( text );
        break;
    case MergeWithOutput:
        slotGetScriptOutput( proc, buffer, buflen );
        break;
    default:
        break;
    };

    firstError = false;
}

void QuantaScriptAction::scriptDone()
{
    delete proc;
    proc = 0;
}

#include "quanta_actions.moc"

