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
#include "actionconfigdialogs.h"

class QDomDocument;
class QPopupMenu;

class KAction;
class KMainWindow;
class KPopupMenu;
class KShortcut;
class KXMLGUIClient;

typedef struct ToolbarEntry{
  KXMLGUIClient *guiClient;
  QDomDocument *dom;
  QPopupMenu *menu;
  KURL url;
  QString name;
  QString id;
  bool user;
  bool visible;
  bool nameModified; ///< True if the name of the toolbar was modified by Quanta on loading:" (n)" was added
};

class ActionConfigDialog: public ActionConfigDialogS
{
  Q_OBJECT

public:

  ActionConfigDialog(const QDict<ToolbarEntry> &toolbarList, KMainWindow* parent = 0, const char* name = 0, bool modal = true, WFlags fl = 0 , const QString& defaultAction = QString::null);
  ~ActionConfigDialog();
  void createScriptAction(const QString& a_name, const QString& a_script);

public slots:
  virtual void accept();
  virtual void reject();
  void slotToggled(bool);
  void slotTextChanged();
  void slotTextChanged(const QString&);
  void slotNewAction();

private slots:
  void slotAddToolbar();
  void slotRemoveToolbar();
  void slotEditToolbar();
  void slotContextMenu(KListView *,QListViewItem *,const QPoint &);
  void slotSelectionChanged(QListViewItem *);
  void slotShortcutCaptured(const KShortcut&);
  void slotDeleteAction();
  void slotApply();
  void slotAddContainerToolbar();
  void slotRemoveContainerToolbar();
  void slotToolbarRemoved(const QString& toolbarName);
  
signals:
  void addToolbar();
  void removeToolbar(const QString&);      
  void deleteUserAction(KAction *);
  void configureToolbars(const QString&);

private:
  void saveCurrentAction();

  KPopupMenu *treeMenu;
  KAction *currentAction;
  QListViewItem *allActionsItem;
  QMap<QString, QString> globalShortcuts;
  KShortcut selectedShortcut;
  KMainWindow *m_mainWindow;
  QDict<ToolbarEntry> m_toolbarList;
  QListViewItem *m_toolbarItem;
};

#endif
