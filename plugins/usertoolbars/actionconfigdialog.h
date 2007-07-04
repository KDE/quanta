/***************************************************************************
                             actionconfigdialog.cpp
                             ---------------------
    copyright            : (C) 2003 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/

#ifndef ACTIONCONFIGDIALOG_H
#define ACTIONCONFIGDIALOG_H

#include <qmap.h>

#include <kshortcut.h>
#include <kurl.h>


#include "ui_actionconfigdialogbase.h"

class QDomDocument;
class QMenu;

class KAction;
namespace KDevelop { 
  class IPlugin; 
}
class KMenu;
class KShortcut;
class KXMLGUIClient;

/**
  The ToolbarEntry describes a user toolbar.
*/
struct ToolbarEntry{
  KXMLGUIClient *guiClient; ///< the GUI client of the toolbar, loaded from the XML file
  QDomDocument *dom; ///< the XML dom tree which describes the toolbar
  QMenu *menu; ///< a menu with the actions that are on the toolbar
  KUrl url; ///< the url pointing to the toolbar file
  QString name; ///< the user visible name of the toolbar
  QString id; ///< an unique id of the toolbar
  bool user;
  bool visible;
  bool nameModified; ///< True if the name of the toolbar was modified by Quanta on loading:" (n)" was added
};

/**
 * An dialog to configure user actions and user toolbars.
 *
 * @author Andras Mantia <amantia@kde.org>
 */
class ActionConfigDialog: public QDialog, public Ui_ActionConfigDialogBase
{
  Q_OBJECT

public:

  /**
   * Constructs the dialog.
   * @param toolbarList a list with the currently loaded user toolbars
   * @param parent the parent of the dialog (a Koncrete::Plugin)
   * @param name name of the dialog object
   * @param modal true if the dialog should be modal
   * @param fl window flags
   * @param defaultAction the name of the action which should be selected by default
   * @return 
   */
  ActionConfigDialog(const QHash<QString, ToolbarEntry*> &toolbarList, KDevelop::IPlugin* parent, bool modal = true, Qt::WFlags fl = 0 , const QString& defaultAction = QString());
  ~ActionConfigDialog();
  /**
   * Autmatically creates a script action
   * @param name the desired name of the action
   * @param script the script execution line (path, arguments)
   */
  void createScriptAction(const QString& name, const QString& script);

public slots:
  /**
   * Called when the dialog is closed with OK.
   */
  virtual void accept();
  /**
   * Called when the dialog is closed with CANCEL.
   */
  virtual void reject();
  /**
   * Called when something has been modified, so the Apply button is enabled.
   */
  void slotToggled(bool);
  /**
   * Called when something has been modified, so the Apply button is enabled.
   */
  void slotTextChanged();
  /**
   * Called when something has been modified, so the Apply button is enabled.
   */
  void slotTextChanged(const QString&);
  /**
   * Create a new, empty action.
   */
  void slotNewAction();

private slots:
  /**
   * Add a new user toolbar.
   */
  void slotAddToolbar();
  /**
   * Remove the current user toolbar.
   */
  void slotRemoveToolbar();
  /**
   * Edit the current user toolbar.
   */
  void slotEditToolbar();
  /**
   * Brings up the treeview context menu.
   */
  void slotContextMenu(K3ListView *,Q3ListViewItem *,const QPoint &);
  /**
   * Called when a new item is selected in the treeview.
   * @param item the currently selected item
   */
  void slotSelectionChanged(Q3ListViewItem *item);
  /**
   * Called when a new shortcut is set.
   * @param shortcut the new shortcut
   */
  void slotShortcutCaptured(const KShortcut &shortcut);
  /**
   * Delete an action.
   */
  void slotDeleteAction();
  /**
   * Apply changes to the current action.
   */
  void slotApply();
  /**
   * Add the current action to a toolbar.
   */
  void slotAddContainerToolbar();
  /**
   * Remove the current action from a toolbar.
   */
  void slotRemoveContainerToolbar();
  /**
   * Called when a toolbar was completely removed.
   * @param toolbarName the name of the toolbar (it is ignored)
   */
  void slotToolbarRemoved(const QString& toolbarName);
  
signals:
  /**
   * A new toolbar was added in the dialog.
   */
  void addToolbar();
  /**
   * A toolbar was removed from the dialog.
   */
  void removeToolbar(const QString&);      
  /**
   * An action was deleted in the dialog.
   */
  void deleteUserAction(KAction *);
  /**
   * Initiates configuring of a toolbar.
   */
  void configureToolbars(const QString&);

private:
  /**
   * Save the changes in the current action.
   */
  void saveCurrentAction();

  KMenu *treeMenu; ///< the treeview holding the toolbars and actions
  KAction *currentAction; ///< the current action loaded in the editor
  Q3ListViewItem *allActionsItem; ///< the ALL item in the treeview
  QMap<QString, QString> globalShortcuts; ///< list with the global KDE shortcuts
  KShortcut selectedShortcut; 
  KDevelop::IPlugin *m_plugin; ///< the parent plugin object
  QHash<QString, ToolbarEntry*> m_toolbarList; ///< list of currently loaded user toolbars
  Q3ListViewItem *m_toolbarItem; ///< the currently selected user toolbar item
};

#endif
