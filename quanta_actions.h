// -*- c++ -*-

#ifndef QUANTA_ACTIONS
#define QUANTA_ACTIONS

#include <kaction.h>
#include <qstring.h>

class QuantaTagAction : public KAction
{
    Q_OBJECT

public:
    QuantaTagAction( const QString& text, int accel,
                     const QObject* receiver, const char* slot,
                     QObject* parent, const char* name = 0 );

    QuantaTagAction( const QString& text, const QString& pix, int accel,
                     const QObject* receiver, const char* slot,
                     QObject* parent, const char* name );

    virtual ~QuantaTagAction();

    QString openTag() const { return openTag_; }
    QString closeTag() const { return closeTag_; }
    void setTag( const QString &openTag, const QString &closeTag = QString::null );

private:
    QString openTag_;
    QString closeTag_;
};

#endif // QUANTA_ACTIONS
