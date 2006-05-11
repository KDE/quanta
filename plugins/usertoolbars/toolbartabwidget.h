/***************************************************************************
                             toolbartabwidget.h
                             ---------------------
    copyright            : (C) 2003-2005 by Andras Mantia <amantia@kde.org>
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

#include <QTabWidget>
#include <qmap.h>

#include <ktoolbar.h>

class QTabWidget;
class KMenu;
class QTabBar;

/**
 The tabwidget which holds the user toolbars. It is a singleton as there is only one
 such toolbar holder during the application's lifetime. 
 
 @author Andras Mantia <amantia@kde.org>
*/
class ToolbarTabWidget: public QTabWidget
{
  Q_OBJECT
public:

  
  /**
   * Get a reference to the object.
   * @param parent passed to the QTabWidget constructor
   * @param name passed to the QTabWidget constructor
   * @return the reference to the singleton object
   */
  static ToolbarTabWidget* const ref(QWidget *parent = 0L, const char *name = 0L)
  {
    static ToolbarTabWidget *m_ref;
    if (!m_ref) m_ref = new ToolbarTabWidget (parent, name);
    return m_ref;
  }
  virtual ~ToolbarTabWidget() {};

  /**
   * Inserts a toolbar as a new tab.
   * @param child the widget to be inserted. Only KToolBar derived widgets are inserted.
   * @param label the user visible name of the toolbar. Will be used as the tab label.
   * @param id an unique id of the inserted toolbar
   */
  virtual void insertTab(QWidget *child, const QString &label, const QString &id);
  /**
   * Returns the toolbar widget from a position.
   * @param index the index of the queried widget.
   * @return the KToolBar object from the queried position
   */
  virtual QWidget* page(int index);
  /**
   * Returns the toolbar with the specified id.
   * @param id the unique id of the toolbar
   * @return the toolbar with the id
   */
  virtual QWidget* page(const QString& id);
  /**
   * Get the id for a toolbar widget.
   * @param widget the widget for which the id is requested
   * @return the id of the widget
   */
  QString id(QWidget *widget) const;
  /**
   * Get the id for a toolbar widget.
   * @param index the position of the widget in the tabwidget
   * @return the id of the widget
   */
  QString id(int index) const;

  /**
   * @return The selected icon-text mode
   */
  Qt::ToolButtonStyle iconText() const {return m_iconText;}
  /**
   * Set the icon-text mode for user toolbars.
   * @param mode the desired mode, one of the following: 
   *             KToolBar::IconsOnly, KToolBar::TextOnly,
   *             KToolBar::IconTextRight, KToolBar::IconTextBottom
   */
  void setIconText(Qt::ToolButtonStyle mode) {m_iconText = mode; emit iconTextModeChanged();}

  /**
   * @return the height of the tab in pixels
   */
  int tabHeight() const;

public slots:
  /**
   * Remove a toolbar from the tabwidget
   * @param widget the widget to be removed
   */
  virtual void removePage(QWidget *widget);
  virtual void resizeEvent(QResizeEvent *e);

private slots:
  void slotRemoveToolbar();
  void slotRenameToolbar();
  void slotEditToolbar();

signals:
  void removeToolbar(const QString&);
  void renameToolbar(const QString&);
  void editToolbar(const QString&);
  void newAction();
  void removeAction(const QString&, const QString&);
  void editAction(const QString&);
  void addToolbar();
  void iconTextModeChanged();
  void toolbarInserted(const QString&);
  void toolbarRemoved(const QString&);

public:
  QString tabUnderMouse;
  QString tabUnderMouseLabel;

protected:
  virtual void mousePressEvent(QMouseEvent * e);

  KMenu *m_popupMenu;
  QTabBar *m_tabBar;
  QMap<QString, QWidget*> m_toolbarList; ///< a list with the added toolbars indexed according to the unique id
  Qt::ToolButtonStyle  m_iconText;

private:
  /**
   * Private constructor
   */
  ToolbarTabWidget(QWidget * parent = 0, const char * name = 0, Qt::WFlags f = 0);
};


#endif
