/***************************************************************************
                          tagaction.h  -  description
                             -------------------
    begin                : ?
    copyright            : (C) ? Dmitry Poplavsky
                           (C) 2002-2003 Andras Mantia <amantia@kde.org>
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

#include <kdeversion.h>

#include <kaction.h>
#include <kactioncollection.h>

#include <qstring.h>
#include <qdom.h>

class QuantaView;
class KProcess;
class QDomElement;
class QFile;
class QTimer;
class MessageOutput;
class Node;

/**
 * An action for inserting an XML tag.
 *
 * @author Dmitry Poplavsky, dima@kde.org
 *         Andras Mantia, amantia@kde.org
 */

class TagAction : public KAction
{
    Q_OBJECT

public:
    /**
     * Create an insert from dom element.
     */
    TagAction( QDomElement *element, KActionCollection *parent);
    virtual ~TagAction();

    QDomElement data() const { return tag; }
    void setModified(bool modified) { m_modified = modified;}
    bool isModified() const {return m_modified;}
    void setOutputFile(QFile* file);
    void setInputFileName(const QString& fileName);
    void addArguments(const QStringList& arguments);
    void execute();
    QString type();
    /**
     * Remove accelerator from tag action name *
     */
    QString actionText();
    /**
     * This function take the output of the TagAction, parse it into Nodes and insert it
     * in the Node tree. Then kafka will take care of updating itself from the Node Tree.
     */
    void insertOutputInTheNodeTree(const QString &str1, const QString &str2 = QString::null, Node *node = 0L);

public slots:
    virtual bool insertTag(bool inputFromFile = false, bool outputToFile = false);


protected slots:
    virtual void slotGetScriptOutput( KProcess *, char *buffer, int buflen );
    virtual void slotGetScriptError( KProcess *, char *buffer, int buflen );
    virtual void scriptDone();
    void slotTimeout();
    void slotProcessExited(KProcess *);

private:
    KProcess *proc;
    bool firstError;
    bool firstOutput;
    bool m_modified;
    bool loopStarted;
    QString scriptOutputDest;
    QString scriptErrorDest;
    QTimer* timer;
    QDomElement tag;
    QFile* m_file;
    QString m_inputFileName;
    MessageOutput *m_appMessages;
    QStringList m_argsList;
    uint m_killCount;
};


#endif // TAGACTION_H
