/***************************************************************************
                             toolbartabwidget.cpp
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

//qt includes
#include <qevent.h>
#include <qlayout.h>
#include <qobjectlist.h>
#include <qpoint.h>
#include <qtabbar.h>
#include <qwidgetstack.h>
#include <qtabwidget.h>
#include <qfontmetrics.h>

//kde includes
#include <kaction.h>
#include <kaccelmanager.h>
#include <kdeversion.h>
#include <kiconloader.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kpopupmenu.h>
#include <ktoolbar.h>
#include <ktoolbarbutton.h>
#include <kdebug.h>
#include <kglobalsettings.h>

//app includes
#include "toolbartabwidget.h"

ToolbarTabWidget::ToolbarTabWidget(QWidget * parent, const char * name, WFlags f)
:QTabWidget(parent, name, f)
{
  m_popupMenu = new KPopupMenu(this);
  m_popupMenu->insertTitle(i18n("Toolbar Menu"), 1);
  m_popupMenu->insertItem(i18n("New Action..."), parent, SLOT(slotNewAction()));
  m_popupMenu->insertSeparator();
  m_popupMenu->insertItem(i18n("New Toolbar..."), parent, SLOT(slotAddToolbar()));
  m_popupMenu->insertItem(i18n("Remove Toolbar"), this, SLOT(slotRemoveToolbar()));
  m_popupMenu->insertItem(i18n("Rename Toolbar..."), this, SLOT(slotRenameToolbar()));
  m_popupMenu->insertItem(SmallIconSet("configure_toolbars"), i18n("Configure Toolbars..."), this, SLOT(slotEditToolbar()));

  connect(this, SIGNAL(removeToolbar(const QString&)),
          parent, SLOT(slotRemoveToolbar(const QString&)));
  connect(this, SIGNAL(renameToolbar(const QString&)),
          parent, SLOT(slotRenameToolbar(const QString&)));
  connect(this, SIGNAL(editToolbar(const QString&)),
          parent, SLOT(slotConfigureToolbars(const QString&)));
  connect(this, SIGNAL(newAction()),
          parent, SLOT(slotNewAction()));
  connect(this, SIGNAL(addToolbar()),
          parent, SLOT(slotAddToolbar()));
#if KDE_VERSION > KDE_MAKE_VERSION(3,3,90)
  KAcceleratorManager::setNoAccel(this);
#endif
}

void ToolbarTabWidget::insertTab(QWidget * child, const QString & label )
{
  if (child->inherits("KToolBar") && child->parentWidget())
  {
    QTabWidget::insertTab(child->parentWidget(), label);
    toolbarList.append(child);
  }
}

QWidget* ToolbarTabWidget::page(int index)
{
  QWidget *w = QTabWidget::page(index);

  for (QWidget *tb = toolbarList.first(); tb; tb = toolbarList.next())
  {
    if (tb->parentWidget() == w)
    {
      w = tb;
      break;
    }
  }
  return w;
}

QWidget* ToolbarTabWidget::page(const QString& label)
{
  QWidget *w = 0L;
  for (int i = 0; i < count(); i++)
  {
    if (this->label(i) == label)
    {
      w = page(i);
      break;
    }
  }
  return w;
}

void ToolbarTabWidget::removePage(QWidget * w )
{
  QWidget *parent = w->parentWidget();
  if (w->inherits("KToolBar") && parent)
  {
    QTabWidget::removePage(parent);
    toolbarList.remove(w);
    delete parent;
  }
}

void ToolbarTabWidget::slotRemoveToolbar()
{
  emit removeToolbar(tabUnderMouse.lower());
}

void ToolbarTabWidget::slotRenameToolbar()
{
  emit renameToolbar(tabUnderMouse.lower());
}

void ToolbarTabWidget::slotEditToolbar()
{
  emit editToolbar(tabUnderMouse + " <quanta>");
}

void ToolbarTabWidget::mousePressEvent ( QMouseEvent * e )
{
  if (e->button() == Qt::RightButton)
  {
    QPoint p = e->globalPos();
    QTab *tab = 0L;
    for (int i =0; i < tabBar()->count(); i++)
    {
      tab = tabBar()->tabAt(i);
      QRect r = tab->rect();
      QPoint p1 = mapToGlobal(r.topLeft());
      QPoint p2 = mapToGlobal(r.bottomRight());
      if (QRect(p1, p2).contains(p))
          break;
      else
        tab = 0L;
    }
    tabUnderMouse = tab ? tab->text() : label(currentPageIndex());
    m_popupMenu->changeTitle(1, i18n("Toolbar Menu") + " - " + i18n(tabUnderMouse.utf8()));
    m_popupMenu->popup(p);
  }
}


void ToolbarTabWidget::resizeEvent(QResizeEvent *e)
{
  QWidget::resizeEvent(e);
  QWidget *tb;
  for (uint i = 0; i < toolbarList.count(); i++)
  {
    tb = toolbarList.at(i);
    tb->resize(QSize(width(), tb->height()));
  }
  int i = currentPageIndex();
  if (i > 0)
  {
    setCurrentPage(i -1);
  } else
  if (i+1 < count())
  {
    setCurrentPage(i + 1);
  }
  setCurrentPage(i);
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


QuantaToolBar::QuantaToolBar(QWidget *parent, const char *name, bool honor_style, bool readConfig)
:KToolBar (parent, name=0, honor_style, readConfig)
{
  m_popupMenu = new KPopupMenu(this);
  m_toolbarTab = dynamic_cast<ToolbarTabWidget*>(parent->parentWidget());
  currentActionName = "";
}

void QuantaToolBar::mousePressEvent(QMouseEvent *e)
{
  if (e->button() == Qt::RightButton)
  {
    m_popupMenu->clear();
    QPoint p = e->globalPos();
    if (m_toolbarTab)
    {
      m_toolbarTab->tabUnderMouse = m_toolbarTab->label(m_toolbarTab->currentPageIndex());
      m_popupMenu->insertTitle(i18n("Toolbar Menu") + " - "
                               + i18n(m_toolbarTab->tabUnderMouse.utf8()));
      m_popupMenu->insertItem(i18n("New Action..."), m_toolbarTab, SIGNAL(newAction()));
      QObjectList* childrenList = queryList("KToolBarButton");
      for (uint i = 0; i < childrenList->count(); i++)
      {
        KToolBarButton *w = static_cast<KToolBarButton*>(childrenList->at(i));
        QPoint p1 = w->parentWidget()->mapToGlobal(w->pos());
        QPoint p2 = QPoint(p1.x() + w->width(), p1.y()+w->height());
        if (QRect(p1, p2).contains(p))
        {
          currentActionName = w->textLabel();
          QString actionName = currentActionName;
          m_popupMenu->insertItem(i18n("Remove Action - %1").arg(actionName.replace('&',"&&")), this, SLOT(slotRemoveAction()));
          m_popupMenu->insertItem(i18n("Edit Action - %1").arg(actionName), this, SLOT(slotEditAction()));
          break;
        }
      }
      m_popupMenu->insertSeparator();
      m_popupMenu->insertItem(i18n("New Toolbar..."), m_toolbarTab, SIGNAL(addToolbar()));
      m_popupMenu->insertItem(i18n("Remove Toolbar"), m_toolbarTab, SLOT(slotRemoveToolbar()));
      m_popupMenu->insertItem(i18n("Rename Toolbar..."), m_toolbarTab, SLOT(slotRenameToolbar()));
      m_popupMenu->insertItem(SmallIconSet("configure_toolbars"), i18n("Configure Toolbars..."), m_toolbarTab, SLOT(slotEditToolbar()));
    }
    m_popupMenu->popup(p);
  }
}

void QuantaToolBar::slotEditAction()
{
  emit editAction(currentActionName);
}

void QuantaToolBar::slotRemoveAction()
{
  if ( KMessageBox::warningYesNo(this, i18n("<qt>Are you sure you want to remove the <b>%1</b> action?</qt>").arg(currentActionName)) == KMessageBox::Yes )
  {
    emit removeAction(m_toolbarTab->tabUnderMouse, currentActionName);
  }
}


#include "toolbartabwidget.moc"
