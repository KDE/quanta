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
#include <qmap.h>

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

  static ToolbarTabWidget* const ref(QWidget *parent = 0L, const char *name = 0L)
  {
    static ToolbarTabWidget *m_ref;
    if (!m_ref) m_ref = new ToolbarTabWidget (parent, name);
    return m_ref;
  }
  virtual ~ToolbarTabWidget(){};

  virtual void insertTab(QWidget *child, const QString &label, const QString &id);
  virtual QWidget* page(int index);
  virtual QWidget* page(const QString& id);
  int tabHeight() const;
  QString id(QWidget *w) const;
  QString id(int index) const;
  KToolBar::IconText iconText() const {return m_iconText;}
  void setIconText(KToolBar::IconText mode) {m_iconText = mode; emit iconTextModeChanged();}

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
  void newAction();
  void addToolbar();
  void iconTextModeChanged();

public:
  QString tabUnderMouse;
  QString tabUnderMouseLabel;

protected:
  virtual void mousePressEvent ( QMouseEvent * e );

  KPopupMenu *m_popupMenu;
  QTabBar *m_tabBar;
  QMap<QString, QWidget*> toolbarList;

private:
  ToolbarTabWidget(QWidget * parent = 0, const char * name = 0, WFlags f = 0);
  KToolBar::IconText m_iconText;

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
  void slotIconTextChanged(int id);
  void slotIconTextMenuAboutToShow();

signals:
  void removeAction(const QString&, const QString&);
  void editAction(const QString&);

protected:
  virtual void mousePressEvent ( QMouseEvent * e );

  KPopupMenu *m_popupMenu;
  KPopupMenu *m_iconTextMenu;
  ToolbarTabWidget *m_toolbarTab;
  QString currentActionName;
};

#endif
