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

class KPopupMenu;
class KShortcut;

class ActionConfigDialog: public ActionConfigDialogS
{
  Q_OBJECT

public:

  ActionConfigDialog( QWidget* parent = 0, const char* name = 0, bool modal = true, WFlags fl = 0 , const QString& defaultAction = QString::null);
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

private:
  void saveCurrentAction();

  KPopupMenu *treeMenu;
  KAction *currentAction;
  QListViewItem *allActionsItem;
  QMap<QString, QString> globalShortcuts;
  KShortcut selectedShortcut;
};

#endif
