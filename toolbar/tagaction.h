/***************************************************************************
                          tagaction.h  -  description
                             -------------------
    begin                : ?
    copyright            : (C) ? Dmitry Poplavsky
                           (C) 2002-2003 Andras Mantia <amantia@freemail.hu>
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

#ifndef TAGACTION_H
#define TAGACTION_H

#include <kaction.h>
#include <kactioncollection.h>
#include <qstring.h>
#include <qdom.h>

class QuantaView;
class KProcess;
class QDomElement;

/**
 * An action for inserting an XML tag.
 *
 * @author Dmitry Poplavsky, dima@kde.org
 *         Andras Mantia, amantia@freemail.hu
 */

class TagAction : public KAction
{
    Q_OBJECT

public:
    /**
     * Create an insert from dom element.
     */
    TagAction( QDomElement *element, KActionCollection *parent);
    TagAction( QDomElement *element, QuantaView *a_view, KActionCollection *parent);

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
    QuantaView *m_view;
};


#endif // TAGACTION_H
