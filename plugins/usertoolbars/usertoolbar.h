/***************************************************************************
    begin                : Thu May 19 2005
    copyright            : (C) 2003-2005 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/

#ifndef USERTOOLBAR_H
#define USERTOOLBAR_H

#include <ktoolbar.h>

class KMenu;
class QAction;

/**
 The class that holds an user toolbar.

@author Andras Mantia <amantia@kde.org>
*/
class UserToolBar: public KToolBar
{
  Q_OBJECT

public:
  explicit UserToolBar (QWidget *parent, const char *name=0, bool honor_style=false, bool readConfig=true);
  virtual ~UserToolBar() {};
  /**
   * Set the status of the toolbar.
   * @param separate true, if is separate, false if it is on a tabwidget
   */
  void setSeparate(bool separate) {m_separateToolbar = separate;}
  /**
   * Returns the status of the toolbar.
   * @return true, if is separate, false if it is on a tabwidget
   */
  bool separate() { return m_separateToolbar;}

private slots:
  void slotEditAction();
  void slotRemoveAction();
  void slotIconTextChanged(QAction *action);
  void slotIconTextMenuAboutToShow();

signals:
  void removeAction(const QString& toolbarName, const QString& actionName);
  void editAction(const QString& actionName);

protected:
  virtual void mousePressEvent (QMouseEvent *e);

  KMenu *m_popupMenu;
  KMenu *m_iconTextMenu;
  QString currentActionName;
  bool m_separateToolbar;

private:
  QAction *m_iconOnlyAction;
  QAction *m_textOnlyAction;
  QAction *m_textBesideAction;
  QAction *m_textUnderAction;
};

#endif //USERTOOLBAR_H
