#include "quantaview.h"
#include "document.h"
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
                                        const QObject* receiver, const char* slot,
                                        QObject* parent, const char* name )
    : KAction( text, accel, receiver, slot, parent, name )
{
}

QuantaScriptAction::QuantaScriptAction( const QString& text, const QString& pix, int accel,
                                        const QObject* receiver, const char* slot,
                                        QObject* parent, const char* name )
   : KAction( text, pix, accel, receiver, slot, parent, name )
{
}

QuantaScriptAction::~QuantaScriptAction()
{
}

void QuantaScriptAction::runScript()
{

}


#include "quanta_actions.moc"

