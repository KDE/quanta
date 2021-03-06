/***************************************************************************
    begin                : Tue May 17 2005
    copyright            : (C) 2005 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/


#ifndef USERTOOLBARSPART_H
#define USERTOOLBARSPART_H

#include <kurl.h>

#include <QHash>
#include <QList>

//kdevelop includes
#include <interfaces/iplugin.h>

class KMenu;
class KAction;
class KRecentFilesAction;
class KDialog;
class KTempDir;
class KTemporaryFile;
namespace KDevelop
{
   class Context; 
   class IProject;
}
class ConfigWidgetProxy;

class UserAction;
class NewToolbarStuff;

struct ToolbarEntry;

/**
Please read the README.dox file for more info about this part
*/
class UserToolbarsPart: public KDevelop::IPlugin
{
    Q_OBJECT
public:
    UserToolbarsPart(QObject *parent, const QVariantList &args);
    ~UserToolbarsPart();
    /**
     * Enables/disables the creation of the Actions menu
     * @param create if true, menu creation is enabled
     */
    void setCreateActionsMenu(bool create);
    bool createActionsMenu() {return m_createActionsMenu;}
    /**
     * Enables/disables the creation of an independent toolbar for each loaded toolbar
     * @param separate if true the above is enabled, otherwise the toolbars are put on a tabwidget
     */
    void setSeparateToolbars(bool separate);
    bool separateToolbars() {return m_separateToolbars;}

public slots:
    /**
     * Delete an action
     * @param action pointer to the actions
     */
    void slotDeleteAction(QAction *action);


private slots:
    void init();

    void contextMenu(KMenu *popup, const KDevelop::Context *context);
    void projectOpened(KDevelop::IProject *project);
    void projectClosed(KDevelop::IProject *project);
    /**
     * Loads a toolbar file.
     * @param url The url pointing to the toolbar file.
     */
    void slotLoadToolbarFile(const KUrl& url);
    /**
     * Loads a toolbar from the disk. It asks for the toolbar file name.
     */
    void slotLoadToolbar();
    /**
     * Load a global toolbar from the disk.
     */
    void slotLoadGlobalToolbar();
    /**
     * Save a local toolbar.
     */
    void slotSaveLocalToolbar() {saveToolbar(true);}
    /**
     * Saves a project toolbar.
     */
    void slotSaveProjectToolbar() {saveToolbar(false);}

    /** Remove a user toolbar.
     *
     * @param id the unique id of the toolbar to be removed
     * @return true if the toolbar was successfully removed, false otherwise
     */
    bool slotRemoveToolbar(const QString& name);
    /** Asks for selecting a user toolbar and removes it.
     *
     * @return true if the toolbar was successfully removed, false otherwise
     */
    bool slotRemoveToolbar();
    /**
     * Adds a new, empty toolbar.
     */
    void slotAddToolbar();
    /**
     * Rename the toolbar.
     * @param id the unique id of the toolbar
     */
    void slotRenameToolbar(const QString& ud);
    /**
     * Rename a toolbar.
     */
    void slotRenameToolbar();
    /**
     * Send a toolbar in mail.
     */
    void slotSendToolbar();
    /**
     * Downloads a toolbar from the main server
     */
    void slotDownloadToolbar();
    /**
     * Uploads a toolbar to the main server
     */
    void slotUploadToolbar();
    /**
     * Enables or disables actions that cannot be used at the moment.
     */
    void slotAdjustActions();
    /**
     * Configure toolbars, show defaultToolbar by default
     * @param defaultToolbar toolbar to configure
     */
    void slotConfigureToolbars(const QString& defaultToolbar);
    /**
     * Called when a new toolbar configuration comes to effect while configuring
     * the toolbars (shortly: Apply was pressed)
     */
    void slotNewToolbarConfig();
    /**
     * Called when a toolbar was inserted to the tabwidget
     * @param id the id of the toolbar
     */
    void slotToolbarLoaded(const QString &id);
    /**
     * Called when a toolbar was removed from the tabwidget
     * @param id the id of the toolbar
     */
    void slotToolbarRemoved(const QString &id);
    /**
     * Edit a user action.
     * @param actionName the action to edit
     */
    void slotEditAction(const QString& actionName);
    /**
     * Creates a new, empty action.
     */
    void slotNewAction();
    /**
     * Remove an action from a toolbar
     * @param id the id of the toolbar holding the action
     * @param actionName the name of the action
     */
    void slotRemoveAction(const QString& id, const QString& actionName);
    /**
     * Brings up the action configuration dialog.
     */
    void slotConfigureActions();
    /**
     * Show the outputview plugin.
     */
    void slotShowOutputView();
    /**
     * Show a message in the outputview plugin.
     * @param message the message string
     * @param append if true, the message is appened to the last one, if false, the message is put in a new line
     */
    void slotShowMessage(const QString &message, bool append);

signals:
    /**
     * Emmitted when a toolbar is removed by the user.
     * @param  the unique id of the toolbar
     */
    void toolbarRemoved(const QString&);
private:
    void setupActions();
    /**
     * Initiates a save of a toolbar. The destination is checked, so it is not
     * possible to save a project toolbar outside of the project directory.
     * @param localToolbar true if the toolbar is a local toolbar, false if it is a project toolbar
     * @param toolbarToSave the name of the toolbar. If empty, a dialog is shown where the user can select the toolbar to save.
     * @param destURL where to save the toolbar. If empty, a file dialog is shown to select the destination URL.
     * @return true if successful, false if saving failed or was cancelled
     */
    bool saveToolbar(bool localToolbar = true, const QString& toolbarToSave = QString(), const KUrl& destURL = KUrl());
    /**
     * Saves the toolbar and the actions.
     *
     * @param toolbarName the unique id of the toolbar
     * @param destFile the destination URL
     * @return  the URL of the actions file belonging to this toolbar
     */
    KUrl saveToolbarToFile(const QString& toolbarName, const KUrl& destFile);
    /**
     * Creates a toolbar tarball which can be uploaded or sent in email.
     * @return the name of the created file or QString() if creation has failed.
     */
    QString createToolbarTarball();
    /**
     * Removes all user toolbars and ask for save the modified ones.
     * @return true if removing was successful, false if it was cancelled
     */
    bool removeToolbars();
    /**
     * Opens a dialog for the user to choose a toolbar
     * @param caption the caption of the dialog
     * @return the id of the toolbar or QString() if canceled
     */
    QString selectToolbarDialog(const QString &caption);

    KAction *action;
    KRecentFilesAction *m_projectToolbarFiles;

    ConfigWidgetProxy *m_configProxy;
    uint m_userToolbarsCount; ///< the number of loaded user toolbars
    QString m_tmpDir; ///< the directory where the temporary files are created
    QHash<QString, ToolbarEntry*> m_toolbarList; ///< a list of the loaded toolbars
    QList<KTemporaryFile*> m_tempFileList; ///< a list of temporary files created during the plugins lifetime
    QList<KTempDir*> m_tempDirList; ///< a list of temporary directories created during the plugins lifetime
    QList<UserAction*> m_userActions;    ///< list of action loaded from toolbar files
   //FIXME: knewstuff2 NewToolbarStuff *m_newToolbarStuff;
    int m_currentTabPage; ///< the index of the currently visible toolbar tab
    bool m_separateToolbars; ///< if true, the toolbars are created as standalone toolbars, not as part of a tabwidget
    bool m_createActionsMenu; ///< if true a menu is created with the user actions, grouped by toolbars in submenus
    KDevelop::IPlugin *m_outputPlugin;
};

#endif
