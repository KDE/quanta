#include "quanta_actions.h"

QuantaTagAction::QuantaTagAction( const QString& text, int accel,
                 const QObject* receiver, const char* slot,
                 QObject* parent, const char* name )
    : KAction( text, accel, receiver, slot, parent, name )
{

}

QuantaTagAction::QuantaTagAction( const QString& text, const QString& pix, int accel,
                 const QObject* receiver, const char* slot,
                 QObject* parent = 0, const char* name = 0 )
    : KAction( text, pix, accel, receiver, slot, parent, name )
{

}

QuantaTagAction::~QuantaTagAction()
{
}

void QuantaTagAction::setTag( const QString &openTag, const QString &closeTag )
{
    this->openTag_ = openTag;
    this->closeTag_ = closeTag;
}

#include "quanta_actions.moc"

