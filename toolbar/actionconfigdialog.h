//
//
// C++ Interface: h
//
// Description: 
//
//
// Author: Andras Mantia <amantia@freemail.hu>, (C) 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef ACTIONCONFIGDIALOG_H
#define ACTIONCONFIGDIALOG_H

#include "actionconfigdialogs.h"

class KPopupMenu;
class KShortcut;

class ActionConfigDialog: public ActionConfigDialogS
{
  Q_OBJECT

public:

  ActionConfigDialog( QWidget* parent = 0, const char* name = 0, bool modal = true, WFlags fl = 0 );
  ~ActionConfigDialog();

public slots:
  virtual void accept();
  void slotToggled(bool);
  void slotTextChanged();
  void slotTextChanged(const QString&);

private slots:
  void slotAddToolbar();
  void slotRemoveToolbar();
  void slotEditToolbar();
  void slotContextMenu(KListView *,QListViewItem *,const QPoint &);
  void slotSelectionChanged(QListViewItem *);
  void slotShortcutCaptured(const KShortcut&);
  void slotNewAction();
  void slotDeleteAction();
  void slotApply();
  void slotAddContainerToolbar();
  void slotRemoveContainerToolbar();

private:
  void saveCurrentAction();

  KPopupMenu *treeMenu;
  KAction *currentAction;
  QListViewItem *allActionsItem;
};

#endif
