/***************************************************************************
                             toolbartabwidget.h
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

#ifndef TOOLBARTABWIDGET_H
#define TOOLBARTABWIDGET_H

#include <qwidget.h>
#include <qptrlist.h>

class QTabWidget;
class KPopupMenu;
class QWidgetStack;
class QTabBar;
class KToolBar;

class ToolbarTabWidget: public QTabWidget
{
  Q_OBJECT
public:
  QWidgetStack *m_widgetStack;

  ToolbarTabWidget(QWidget * parent = 0, const char * name = 0, WFlags f = 0);
  virtual ~ToolbarTabWidget(){};

  virtual void insertTab(QWidget * child, const QString & label);
  virtual QWidget* page(int index);
  virtual QWidget* page(const QString& label);
  int tabHeight() const;

public slots:
  virtual void removePage(QWidget * w );

  virtual void resizeEvent(QResizeEvent *);
private slots:
  void slotRemoveToolbar();
  void slotRenameToolbar();
  void slotEditToolbar();

signals:
  void removeToolbar(const QString&);
  void renameToolbar(const QString&);
  void editToolbar(const QString&);

public:
  QString tabUnderMouse;

protected:
  virtual void mousePressEvent ( QMouseEvent * e );

  KPopupMenu *m_popupMenu;
  QTabBar *m_tabBar;
  QPtrList<QWidget> toolbarList;
};

class QuantaToolBar: public KToolBar
{
  Q_OBJECT

public:
  QuantaToolBar (QWidget *parent, const char *name=0, bool honor_style=FALSE, bool readConfig=TRUE);
  virtual ~QuantaToolBar() {};

private slots:
  void slotEditAction();
  void slotRemoveAction();

signals:
  void removeAction(const QString&, const QString&);
  void editAction(const QString&);

protected:
  virtual void mousePressEvent ( QMouseEvent * e );

  KPopupMenu *m_popupMenu;
  ToolbarTabWidget *m_toolbarTab;
  QString currentActionName;
};

#endif
