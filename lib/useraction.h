/***************************************************************************
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

#ifndef USERACTION_H
#define USERACTION_H

#include "extfileinfo.h"
#include "quantaexport.h"

#include <qdom.h>
#include <kaction.h>
#include <ktoggleaction.h>


namespace KDevelop 
{ 
  class IPlugin; 
}
class K3Process;
class QFile;
class QTimer;


/**
 * An action for inserting an XML tag, a text or start an external script.
 *
 * @author Dmitry Poplavsky, dima@kde.org
 *         Andras Mantia, amantia@kde.org
 *         Paulo Moura Guedes, moura@kdewebdev.org
 */

class KDEVQUANTA_EXPORT UserAction : public KToggleAction
{
    Q_OBJECT

public:
    /**
     * Creates the action from a DOM element.
     * @param element the DOM element describing the action
     * @param plugin the plugin to where this action belongs
     * @param toggle If set to true, the class behaves like a KToggleAction;
     *               Else it behaves like a KAction. This avoids the multi-inheritance problem.
     */
    UserAction(QDomElement *element, KDevelop::IPlugin *plugin, bool toggle = false);
    virtual ~UserAction();

    QDomElement data() const { return tag; }
    void setModified(bool modified) { m_modified = modified;}
    bool isModified() const {return m_modified;}
    void setOutputFile(QFile* file) {m_file = file;}
    void setInputFileName(const QString& fileName) {m_inputFileName = fileName;}
    void addArguments(const QStringList& arguments);
    bool toggable() const {return tag.attribute("toggable") == "true";}
    
    /** Activates the action.
      @param blocking in case of script actions, the script is run in blocking mode, if this argument is true. Default is non-blocking mode.
    */
    void execute(bool blocking = false);
    /**
     * @return the type of the action (tag, text or script)
     */
    QString type() const {return tag.attribute("type","");}
    /**
     * @return the user visible name of the action, without accelerators
     */
    QString actionText();

    
protected slots:
    virtual void slotActivated();    
    /**
     * Called when the action is activated
     * @return true if the activation was successfuly completed
     */
    virtual bool slotActionActivated();
    virtual void slotGetScriptOutput( K3Process *, char *buffer, int buflen );
    virtual void slotGetScriptError( K3Process *, char *buffer, int buflen );
    virtual void scriptDone();
    void slotTimeout();
    void slotProcessExited(K3Process *);

signals:
   /**
    * Request showing of the output view.
    */
   void showOutputView();
    /**
     * Request showing a message in the output view.
     * @param message the message string
     * @param append if true, the message is appened to the last one, if false, the message is put in a new line 
     */
    void showMessage(const QString& message, bool append);
private:
    K3Process *proc;
    bool m_firstError;
    bool m_firstOutput;
    bool m_modified;
    bool m_loopStarted; ///< true if an internal even loop was started for synchronous execution
    bool m_useInputFile;
    bool m_useOutputFile;
    QString scriptOutputDest;
    QString scriptErrorDest;
    QTimer* timer;
    QDomElement tag;
    QFile* m_file;
    KDevelop::IPlugin *m_plugin;
    QString m_inputFileName;
    QStringList m_argsList;
    uint m_killCount;
    bool const m_toggle;
    ExtFileInfo internalFileInfo;
};


#endif // USERACTION_H
