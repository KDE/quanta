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

#include <kdeversion.h>

#include <kaction.h>
#if KDE_IS_VERSION(3,1,90)
#include <kactioncollection.h>
#endif

#include <qstring.h>
#include <qdom.h>

class QuantaView;
class KProcess;
class QDomElement;
class QFile;
class QTimer;
class MessageOutput;

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
    virtual ~TagAction();

    QDomElement data() const { return tag; }
    void setModified(bool modified) { m_modified = modified;}
    bool isModified() const {return m_modified;}
    void setOutputFile(QFile* file);
    void setInputFileName(const QString& fileName);
    void execute();
#ifdef BUILD_KAFKAPART
    /**
     * This function take the output of the TagAction, parse it into Nodes and insert it
     * in the Node tree. Then kafka will take care of updating itself from the Node Tree.
     */
    void insertOutputInTheNodeTree(QString str1, QString str2 = "");

    /**
     * This function is TEMPORARY and try to insert Tag only when possible/necessary
     */
    void insertTag(QString str1, QString str2 = "");
#endif

public slots:
    virtual void insertTag(bool inputFromFile = false, bool outputToFile = false);


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
    QuantaView *m_view;
    QFile* m_file;
    QString m_inputFileName;
    MessageOutput *m_appMessages;
};


#endif // TAGACTION_H
