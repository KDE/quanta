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


#include "toolbartabwidget.moc"
