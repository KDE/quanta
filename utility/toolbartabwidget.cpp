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
  KAcceleratorManager::setNoAccel(this);
}

void ToolbarTabWidget::insertTab(QWidget *child, const QString &label, const QString &id)
{
  if (child->inherits("KToolBar") && child->parentWidget())
  {
    QTabWidget::insertTab(child->parentWidget(), label);
    toolbarList.insert(id, child);
  }
}

QWidget* ToolbarTabWidget::page(int index)
{
  QWidget *w = QTabWidget::page(index);

  for (QMap<QString, QWidget*>::Iterator it = toolbarList.begin(); it != toolbarList.end(); ++it)
  {
    if (it.data()->parentWidget() == w)
    {
      w = *it;
      break;
    }
  }
  return w;
}

QString ToolbarTabWidget::id(QWidget *w) const
{
  QString idStr;
  for (QMap<QString, QWidget*>::ConstIterator it = toolbarList.constBegin(); it != toolbarList.constEnd(); ++it)
  {
    if (it.data()->parentWidget() == w)
    {
      idStr = it.key();
      break;
    }
  }
  return idStr;
}

QString ToolbarTabWidget::id(int index) const
{
  QWidget *w = QTabWidget::page(index);
  QString idStr;
  for (QMap<QString, QWidget*>::ConstIterator it = toolbarList.constBegin(); it != toolbarList.constEnd(); ++it)
  {
    if (it.data()->parentWidget() == w)
    {
      idStr = it.key();
      break;
    }
  }
  return idStr;
}

QWidget* ToolbarTabWidget::page(const QString& id)
{
  QWidget *w = toolbarList.find(id).data();
  return w;
}

void ToolbarTabWidget::removePage(QWidget * w)
{
  QWidget *parent = w->parentWidget();
  if (w->inherits("KToolBar") && parent)
  {
    QTabWidget::removePage(parent);
    for (QMap<QString, QWidget*>::ConstIterator it = toolbarList.constBegin(); it != toolbarList.constEnd(); ++it)
    {
      if (it.data() == w)
      {
        toolbarList.remove(it.key());
        break;
      }
    }
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
  emit editToolbar(tabUnderMouseLabel + " <quanta>");
}

void ToolbarTabWidget::mousePressEvent ( QMouseEvent * e )
{
  if (e->button() == Qt::RightButton)
  {
    QPoint p = e->globalPos();
    QTab *tab = 0L;
    QWidget *pageW = 0L;
    for (int i =0; i < tabBar()->count(); i++)
    {
      tab = tabBar()->tabAt(i);
      pageW = page(i);
      QRect r = tab->rect();
      QPoint p1 = mapToGlobal(r.topLeft());
      QPoint p2 = mapToGlobal(r.bottomRight());
      if (QRect(p1, p2).contains(p))
          break;
      else
        tab = 0L;
    }
    tabUnderMouseLabel = tab ? tab->text() : label(currentPageIndex());
    if (!pageW)
      pageW = currentPage();
    for (QMap<QString, QWidget*>::Iterator it = toolbarList.begin(); it != toolbarList.end(); ++it)
    {
      if (it.data()->parentWidget() == pageW)
      {
        tabUnderMouse = it.key();
        break;
      }
    }
    m_popupMenu->changeTitle(1, i18n("Toolbar Menu") + " - " + i18n(tabUnderMouseLabel.utf8()));
    m_popupMenu->popup(p);
  }
}


void ToolbarTabWidget::resizeEvent(QResizeEvent *e)
{
  QWidget::resizeEvent(e);
  QWidget *tb;
  for (QMap<QString, QWidget*>::Iterator it = toolbarList.begin(); it != toolbarList.end(); ++it)
  {
    tb = it.data();
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
  m_iconTextMenu = new KPopupMenu(this);
  m_iconTextMenu->setCheckable(true);
  m_iconTextMenu->insertItem(i18n("Icons Only"), 0);
  m_iconTextMenu->insertItem(i18n("Text Only"), 1);
  m_iconTextMenu->insertItem(i18n("Text Alongside Icons"), 2);
  m_iconTextMenu->insertItem(i18n("Text Under Icons"), 3);
  connect(m_iconTextMenu, SIGNAL(activated(int)), SLOT(slotIconTextChanged(int)));
  connect(m_iconTextMenu, SIGNAL(aboutToShow()), SLOT(slotIconTextMenuAboutToShow()));
  setIconText(ToolbarTabWidget::ref()->iconText(), false);
}

void QuantaToolBar::slotIconTextMenuAboutToShow()
{
  m_iconTextMenu->setItemChecked(0, false);
  m_iconTextMenu->setItemChecked(1, false);
  m_iconTextMenu->setItemChecked(2, false);
  m_iconTextMenu->setItemChecked(3, false);
  switch (ToolbarTabWidget::ref()->iconText()) 
  {
    case IconOnly: m_iconTextMenu->setItemChecked(0, true);
                   break;
    case TextOnly: m_iconTextMenu->setItemChecked(1, true);
                   break;
    case IconTextRight: m_iconTextMenu->setItemChecked(2, true);
                   break;
    case IconTextBottom: m_iconTextMenu->setItemChecked(3, true);
                   break;
  }  
}

void QuantaToolBar::slotIconTextChanged(int id)
{
  ToolbarTabWidget *toolbarTab = ToolbarTabWidget::ref();
  int width = toolbarTab->width();
  int bigHeight = iconSize() + QFontMetrics(KGlobalSettings::toolBarFont()).height() + 10;
  int normalHeight = iconSize() + 10;
  for (int i = 0; i < toolbarTab->count(); i++)
  {
    QuantaToolBar *tb = static_cast<QuantaToolBar*>(toolbarTab->page(i));
    switch (id)
    {
      case 0: tb->setIconText(IconOnly); 
              tb->setGeometry(0,0, width, normalHeight);
              break;
      case 1: tb->setIconText(TextOnly); 
              tb->setGeometry(0,0, width, normalHeight);
              break;
      case 2: tb->setIconText(IconTextRight); 
              tb->setGeometry(0,0, width, normalHeight);
              break;
      case 3: tb->setIconText(IconTextBottom); 
              tb->setGeometry(0,0, width, bigHeight);
              break;
    }    
  }
  toolbarTab->setIconText(iconText()); 
  if (id == 3)
  {
    toolbarTab->setFixedHeight(toolbarTab->tabHeight() + height() + 3);
  } else
  {
    toolbarTab->setFixedHeight(toolbarTab->tabHeight() + height() + 3);
  }
}

void QuantaToolBar::mousePressEvent(QMouseEvent *e)
{
  if (e->button() == Qt::RightButton)
  {
    m_popupMenu->clear();
    QPoint p = e->globalPos();
    if (m_toolbarTab)
    {
      m_toolbarTab->tabUnderMouse = m_toolbarTab->id(m_toolbarTab->currentPageIndex());
      m_toolbarTab->tabUnderMouseLabel = m_toolbarTab->label(m_toolbarTab->currentPageIndex());
      m_popupMenu->insertTitle(i18n("Toolbar Menu") + " - "
                               + i18n(m_toolbarTab->tabUnderMouseLabel.utf8()));
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
      m_popupMenu->insertSeparator();
      m_popupMenu->insertItem( i18n("Text Position"), m_iconTextMenu);
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
    if ( KMessageBox::warningContinueCancel(this, i18n("<qt>Are you sure you want to remove the <b>%1</b> action?</qt>").arg(currentActionName),QString::null,KStdGuiItem::del()) == KMessageBox::Continue )
  {
    emit removeAction(m_toolbarTab->tabUnderMouse, currentActionName);
  }
}


#include "toolbartabwidget.moc"
