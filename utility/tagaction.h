/***************************************************************************
                          tagaction.h  -  description
                             -------------------
    begin                : ?
    copyright            : (C) ? Dmitry Poplavsky
                           (C) 2002-2005 Andras Mantia <amantia@kde.org>
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

class KMainWindow;
class QuantaView;
class KProcess;
class QDomElement;
class QFile;
class QTimer;
class MessageOutput;
class Node;
class NodeModifsSet;
class NodeSelection;

/**
 * An action for inserting an XML tag.
 *
 * @author Dmitry Poplavsky, dima@kde.org
 *         Andras Mantia, amantia@kde.org
 *         Paulo Moura Guedes, moura@kdewebdev.org
 */

class TagAction : public KToggleAction
{
    Q_OBJECT

public:
    /**
     * Create an insert from dom element.
     * @param toggle If set to true, the class behaves like a KToggleAction;
     *               Else it behaves like a KAction. This avoids the multi-inheritance problem.
     */
    TagAction(QDomElement *element, KMainWindow *parentMainWindow, bool toggle = false);
    virtual ~TagAction();

    QDomElement data() const { return tag; }
    void setModified(bool modified) { m_modified = modified;}
    bool isModified() const {return m_modified;}
    void setOutputFile(QFile* file);
    void setInputFileName(const QString& fileName);
    void addArguments(const QStringList& arguments);
    bool toggable() const {return tag.attribute("toggable") == "true";}
    bool toggled() const {return isChecked();}
    
    /** Activates the action.
      @param blocking in case of script actions, the script is run in blocking mode, if this argument is true
    */
    void execute(bool blocking);
    QString type();
    /**
     * Remove accelerator from tag action name *
     */
    QString actionText();

    QString XMLTagName() const;
    QString openXMLTagString() const;
    QString closeXMLTagString() const;

signals:
    void showMessage(const QString& msg, bool append);
    void clearMessages();
    void showMessagesView();
    void createNewFile();

public slots:
    virtual void slotActivated();
    
    /**
     * We need this information in order to know if queued actions should be applied.
     * It's public because it can be activated by other classes if there are any queued actions.
     */
    virtual bool slotActionActivated(KAction::ActivationReason reason, Qt::ButtonState state);

protected slots:
    virtual void slotGetScriptOutput( KProcess *, char *buffer, int buflen );
    virtual void slotGetScriptError( KProcess *, char *buffer, int buflen );
    virtual void scriptDone();
    void slotTimeout();
    void slotProcessExited(KProcess *);
    virtual bool slotActionActivated();

private:
    void applyTagInSelection(Node* start_node, int start_offset, Node* end_node, int end_offset, 
                             NodeSelection& selection, NodeModifsSet* modifs) const;
    void applyTagInMixedSelection(Node* start_node, int start_offset, Node* end_node, int end_offset, 
                                  NodeSelection& selection, NodeModifsSet* modifs) const;
    void deapplyTagInSelection(Node* start_node, int start_offset, Node* end_node, int end_offset, 
                               NodeSelection& selection, NodeModifsSet* modifs) const;
    void deapplyTagInMixedSelection(Node* start_node, int start_offset, Node* end_node, int end_offset, 
                                    NodeSelection& selection, NodeModifsSet* modifs) const;
    
private:
    KProcess *proc;
    bool firstError;
    bool firstOutput;
    bool m_modified;
    bool loopStarted;
    bool m_useInputFile;
    bool m_useOutputFile;
    QString scriptOutputDest;
    QString scriptErrorDest;
    QTimer* timer;
    QDomElement tag;
    QFile* m_file;
    KMainWindow *m_parentMainWindow;
    QString m_inputFileName;
    QStringList m_argsList;
    uint m_killCount;
    bool const m_toggle;
};


#endif // TAGACTION_H
