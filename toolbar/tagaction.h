#ifndef TAGACTION_H
#define TAGACTION_H

#include <kaction.h>
#include <qstring.h>

class QuantaView;
class KProcess;
class QDomElement;

/**
 * An action for inserting an XML tag.
 *
 * @author Dmitry Poplavsky, dima@kde.org
 */

class TagAction : public KAction
{
    Q_OBJECT

public:
    /**
     * Create an insert from dom element.
     */
    TagAction( QDomElement *element, QObject* parent, const char* name, QuantaView *view );

    virtual ~TagAction();
    
    QDomElement *data();


protected slots:
    virtual void insertTag();
    virtual void runScript();
    virtual void slotGetScriptOutput( KProcess *, char *buffer, int buflen );
    virtual void slotGetScriptError( KProcess *, char *buffer, int buflen );
    virtual void scriptDone();

private:
    KProcess *proc;
    bool firstError;
    bool firstOutput;

    QDomElement *tag;
    QuantaView *view_;
};


#endif // TAGACTION_H
