//
//
// C++ Implementation: cpp
//
// Description:
//
//
// Author: Andras Mantia <amantia@freemail.hu>, (C) 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//

//qt includes
#include <qevent.h>
#include <qlayout.h>
#include <qpoint.h>
#include <qtabbar.h>
#include <qwidgetstack.h>
#include <qtabwidget.h>

//kde includes
#include <kaction.h>
#include <klocale.h>
#include <kpopupmenu.h>
#include <ktoolbarbutton.h>

//app includes
#include "toolbartabwidget.h"
#include "../resource.h"
#include "../quanta.h"

ToolbarTabWidget::ToolbarTabWidget(QWidget * parent, const char * name, WFlags f)
:QTabWidget(parent, name, f)
{
  m_popupMenu = new KPopupMenu(this);
  m_popupMenu->insertTitle(i18n("Toolbar Menu"), 1);
  m_popupMenu->insertItem(i18n("Remove"), this, SLOT(slotRemoveToolbar()));
  m_popupMenu->insertItem(i18n("Rename"), this, SLOT(slotRenameToolbar()));
}

void ToolbarTabWidget::insertTab(QWidget * child, const QString & label )
{
  if (child->inherits("KToolBar") && child->parentWidget())
  {
    QTabWidget::insertTab(child->parentWidget(), label);
    toolbarList.append(child);
  }
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

void ToolbarTabWidget::mousePressEvent ( QMouseEvent * e )
{
  if (e->button() == Qt::RightButton)
  {
    QPoint p = e->globalPos();
    QTab *tab;
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
    m_popupMenu->changeTitle(1, i18n("Toolbar Menu") + " - " + i18n(tabUnderMouse));
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
  return tabBar()->height();
}


QuantaToolBar::QuantaToolBar(QWidget *parent, const char *name, bool honor_style, bool readConfig)
:KToolBar (parent, name=0, honor_style, readConfig)
{
  m_popupMenu = new KPopupMenu(this);
  m_popupMenu->insertTitle(i18n("Toolbar Menu"), 1);
  m_toolbarTab = dynamic_cast<ToolbarTabWidget*>(parent->parentWidget());
  if (m_toolbarTab)
  {
    m_popupMenu->insertItem(i18n("Remove"), parent, SLOT(slotRemoveToolbar()));
    m_popupMenu->insertItem(i18n("Rename"), parent, SLOT(slotRenameToolbar()));
  }
}

void QuantaToolBar::mousePressEvent(QMouseEvent *e)
{
  if (e->button() == Qt::RightButton)
  {
    if (m_toolbarTab)
    {
      m_popupMenu->changeTitle(1, i18n("Toolbar Menu") + " - "
                                  + i18n(m_toolbarTab->label(m_toolbarTab->currentPageIndex())));
/*
      QPtrList<KAction> actionList;
      for (uint i = 0; i < quantaApp->actionCollection()->count(); i++)
      {
        KAction *action = quantaApp->actionCollection()->action(i);
        if (action->isPlugged(this))
            actionList.append(action);
      }
      Id2WidgetMap::Iterator it = id2widget.begin();
      while (it != id2widget.end())
      {
        if (dynamic_cast<KToolBarButton *>(*it))
        {
          m_popupMenu->insertItem(dynamic_cast<KToolBarButton *>(*it)->text());
        }
        ++it;
      } */
    }
    QPoint p = e->globalPos();
    m_popupMenu->popup(p);
  }
}

#include "toolbartabwidget.moc"
