// -*- c++ -*-

#ifndef QUANTA_ACTIONS_H
#define QUANTA_ACTIONS_H

#include <kaction.h>
#include <qstring.h>

/**
 * An action for inserting an XML tag.
 *
 * @author Richard Moore, rich@kde.org
 * @version $Id$
 */
class QuantaTagAction : public KAction
{
    Q_OBJECT

    Q_PROPERTY( QString openTag READ openTag WRITE setOpenTag DESIGNABLE true )
    Q_PROPERTY( QString closeTag READ closeTag WRITE setCloseTag DESIGNABLE true )

public:
    /**
     * Create an insert without an icon.
     */
    QuantaTagAction( const QString& text, int accel,
                     const QObject* receiver, const char* slot,
                     QObject* parent, const char* name = 0 );

    /**
     * Create an insert with an icon.
     */
    QuantaTagAction( const QString& text, const QString& pix, int accel,
                     const QObject* receiver, const char* slot,
                     QObject* parent, const char* name );

    virtual ~QuantaTagAction();

    QString openTag() const { return openTag_; }
    void setOpenTag( const QString &open ) { openTag_ = open; }

    QString closeTag() const { return closeTag_; }
    void setCloseTag( const QString &close ) { closeTag_ = close; }

    void setTag( const QString &openTag, const QString &closeTag = QString::null );

private:
    QString openTag_;
    QString closeTag_;
};

#endif // QUANTA_ACTIONS_H
