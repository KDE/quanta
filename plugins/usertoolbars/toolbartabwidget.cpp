/***************************************************************************
                             toolbartabwidget.cpp
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

//app includes
#include "toolbartabwidget.h"
#include "usertoolbar.h"

//qt includes
#include <QMouseEvent>
#include <qtabbar.h>

//kde includes
#include <kacceleratormanager.h>
#include <kdebug.h>
#include <kglobalsettings.h>
#include <kiconloader.h>
#include <klocale.h>
#include <kmenu.h>


ToolbarTabWidget::ToolbarTabWidget(QWidget * parent, const char * name, Qt::WFlags f)
:QTabWidget(parent)
{
  setObjectName(name);
  setWindowFlags(f);
  //the context menu
  m_popupMenu = new KMenu(this);
//FIXME:  m_popupMenu->insertTitle(i18n("Toolbar Menu"), 1);
  m_popupMenu->addAction(i18n("New Action..."), this, SIGNAL(newAction()));
  m_popupMenu->addSeparator();
  m_popupMenu->addAction(i18n("New Toolbar..."), this, SIGNAL(addToolbar()));
  m_popupMenu->addAction(i18n("Remove Toolbar"), this, SLOT(slotRemoveToolbar()));
  m_popupMenu->addAction(i18n("Rename Toolbar..."), this, SLOT(slotRenameToolbar()));
  m_popupMenu->addAction(SmallIconSet("configure_toolbars"), i18n("Configure Toolbars..."), this, SLOT(slotEditToolbar()));

  KAcceleratorManager::setNoAccel(this);
  m_iconText = Qt::ToolButtonIconOnly;

  //FIXME: this ensures that the tabwidget doesn't become too small. Would be
  //better if the toolbar holding the tabwidget could be resized and when
  //that toolbar changes the size, the tabwidget also changes its size
  setMinimumWidth(500);
}

void ToolbarTabWidget::insertTab(QWidget *child, const QString &label, const QString &id)
{
  if (child->inherits("KToolBar") && child->parentWidget())
  {
    QTabWidget::addTab(child->parentWidget(), label);
    m_toolbarList.insert(id, child);
    emit toolbarInserted(id);
  }
}

QWidget* ToolbarTabWidget::page(int index)
{
  QWidget *w = QTabWidget::widget(index);

  for (QMap<QString, QWidget*>::Iterator it = m_toolbarList.begin(); it != m_toolbarList.end(); ++it)
  {
    if (it.value()->parentWidget() == w)
    {
      w = *it;
      break;
    }
  }
  return w;
}

QString ToolbarTabWidget::id(QWidget *widget) const
{
  QString idStr;
  for (QMap<QString, QWidget*>::ConstIterator it = m_toolbarList.constBegin(); it != m_toolbarList.constEnd(); ++it)
  {
    if (it.value()->parentWidget() == widget)
    {
      idStr = it.key();
      break;
    }
  }
  return idStr;
}

QString ToolbarTabWidget::id(int index) const
{
  QWidget *w = QTabWidget::widget(index);
  QString idStr;
  for (QMap<QString, QWidget*>::ConstIterator it = m_toolbarList.constBegin(); it != m_toolbarList.constEnd(); ++it)
  {
    //the toolbar widget itself is included in a QWidget and only that one is
    //added to the tabwidget
    if (it.value()->parentWidget() == w)
    {
      idStr = it.key();
      break;
    }
  }
  return idStr;
}

QWidget* ToolbarTabWidget::page(const QString& id)
{
  QWidget *w = m_toolbarList.find(id).value();
  return w;
}

void ToolbarTabWidget::removePage(QWidget *widget)
{
  //the toolbar widget itself is included in a QWidget and only that one is
  //added to the tabwidget
  QWidget *parent = widget->parentWidget();
  if (widget->inherits("KToolBar") && parent)
  {
    removeTab(indexOf(parent));
    for (QMap<QString, QWidget*>::ConstIterator it = m_toolbarList.constBegin(); it != m_toolbarList.constEnd(); ++it)
    {
      if (it.value() == widget)
      {
        QString id = it.key();
        m_toolbarList.remove(id);
        emit toolbarRemoved(id);
        break;
      }
    }
    delete parent;
  }
}

void ToolbarTabWidget::slotRemoveToolbar()
{
  emit removeToolbar(tabUnderMouse.toLower());
}

void ToolbarTabWidget::slotRenameToolbar()
{
  emit renameToolbar(tabUnderMouse.toLower());
}

void ToolbarTabWidget::slotEditToolbar()
{
//TODO: Support toolbar files where their name do not contain <quanta>, but
// <usertoolbar> or something like that
  emit editToolbar(tabUnderMouseLabel + " <quanta>");
}

void ToolbarTabWidget::mousePressEvent (QMouseEvent *e)
{
  if (e->button() == Qt::RightButton)
  {
    QPoint p = e->globalPos();
    QWidget *pageW = 0L;
    int id = -1;
    //Find the tab where the user clicked.
    //FIXME: Seems to not work since some time...
    for (int i =0; i < tabBar()->count(); i++)
    {
      pageW = page(i);
      QRect r = tabBar()->tabRect(i);
      QPoint p1 = mapToGlobal(r.topLeft());
      QPoint p2 = mapToGlobal(r.bottomRight());
      if (QRect(p1, p2).contains(p))
      {
        id = i;
        break;
      }
    }
    tabUnderMouseLabel = (id != -1) ? tabBar()->tabText(id) : tabText(currentIndex());
    if (!pageW)
      pageW = currentWidget();
    for (QMap<QString, QWidget*>::Iterator it = m_toolbarList.begin(); it != m_toolbarList.end(); ++it)
    {
      if (it.value()->parentWidget() == pageW)
      {
        tabUnderMouse = it.key();
        break;
      }
    }
//FIXME:    m_popupMenu->changeTitle(1, i18n("Toolbar Menu") + " - " + i18n(tabUnderMouseLabel.toUtf8()));
    m_popupMenu->popup(p);
  }
}


void ToolbarTabWidget::resizeEvent(QResizeEvent *e)
{
//  kDebug(24000) << "ToolbarTabWidget::resizeEvent. width = " << width() << "height = " << height() << endl;
  QWidget::resizeEvent(e);
  //Resize every included toolbar if the tabwidget is resized
  QWidget *tb;
  for (QMap<QString, QWidget*>::Iterator it = m_toolbarList.begin(); it != m_toolbarList.end(); ++it)
  {
    tb = it.value();
    tb->setFixedSize(QSize(width(), tb->height()));
//    kDebug(24000) << it.data() << " resize" << endl;
  }
  //force update of the widget and the toolbars contained inside
  int i = currentIndex();
  if (i > 0)
  {
    setCurrentIndex(i - 1);
  } else
  if (i+1 < count())
  {
    setCurrentIndex(i + 1);
  }
  setCurrentIndex(i);
}

int ToolbarTabWidget::tabHeight() const
{
  int height = tabBar()->height();
  if (height < 2)
  {
      height = QFontMetrics(KGlobalSettings::generalFont()).height() + 12;
  }
  return height;
}


#include "toolbartabwidget.moc"
