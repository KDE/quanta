#ifndef TAGACTION_H
#define TAGACTION_H

#include <kaction.h>
#include <qstring.h>
#include <qdom.h>

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
    TagAction( QDomElement *element, QuantaView *view, KActionCollection* );

    virtual ~TagAction();
    
    QDomElement data() { return tag; }


protected slots:
    virtual void insertTag();
    
    virtual void slotGetScriptOutput( KProcess *, char *buffer, int buflen );
    virtual void slotGetScriptError( KProcess *, char *buffer, int buflen );
    virtual void scriptDone();

private:
    KProcess *proc;
    bool firstError;
    bool firstOutput;
    QString scriptOutputDest;
    QString scriptErrorDest;
  
    QDomElement tag;
    QuantaView *view_;
};


#endif // TAGACTION_H
