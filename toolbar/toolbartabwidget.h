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

#ifndef TOOLBARTABWIDGET_H
#define TOOLBARTABWIDGET_H

#include <qwidget.h>
#include <qptrlist.h>

class QTabWidget;
class KPopupMenu;
class QWidgetStack;
class QTabBar;

#ifdef private
#undef private
#endif
#define private protected
#include <ktoolbar.h>
#undef private
#define private private

class ToolbarTabWidget: public QTabWidget
{
  Q_OBJECT
public:
  QWidgetStack *m_widgetStack;

  ToolbarTabWidget(QWidget * parent = 0, const char * name = 0, WFlags f = 0);
  virtual ~ToolbarTabWidget(){};

  virtual void insertTab(QWidget * child, const QString & label);
  int tabHeight() const;

public slots:
  virtual void removePage(QWidget * w );

  virtual void resizeEvent(QResizeEvent *);
private slots:
  void slotRemoveToolbar();
  void slotRenameToolbar();

signals:
  void removeToolbar(const QString&);
  void renameToolbar(const QString&);

protected:
  virtual void mousePressEvent ( QMouseEvent * e );

  KPopupMenu *m_popupMenu;
  QTabBar *m_tabBar;
  QPtrList<QWidget> toolbarList;
  QString tabUnderMouse;
};

class QuantaToolBar: public KToolBar
{
  Q_OBJECT

public:
  QuantaToolBar (QWidget *parent, const char *name=0, bool honor_style=FALSE, bool readConfig=TRUE);
  virtual ~QuantaToolBar() {};

protected:
  virtual void mousePressEvent ( QMouseEvent * e );

  KPopupMenu *m_popupMenu;
  ToolbarTabWidget *m_toolbarTab;
};

#endif
