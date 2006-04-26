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

//app includes
#include "usertoolbar.h"
#include "toolbartabwidget.h"

//qt includes
#include <QMouseEvent>
#include <qtoolbutton.h>

//kde includes
#include <kglobalsettings.h>
#include <kiconloader.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kmenu.h>


UserToolBar::UserToolBar(QWidget *parent, const char *name, bool honor_style, bool readConfig)
:KToolBar (parent, honor_style, readConfig)
{
//the context menu for the toolbar
  m_popupMenu = new KMenu(this);
  currentActionName = "";

//create our own menu for icon-text style
  m_iconTextMenu = new KMenu(this);
  m_iconTextMenu->setTitle(i18n("Text Position"));
  m_iconTextMenu->setCheckable(true);
  m_iconTextMenu->insertItem(i18n("Icons Only"), 0);
  m_iconTextMenu->insertItem(i18n("Text Only"), 1);
  m_iconTextMenu->insertItem(i18n("Text Alongside Icons"), 2);
  m_iconTextMenu->insertItem(i18n("Text Under Icons"), 3);

  connect(m_iconTextMenu, SIGNAL(activated(int)), SLOT(slotIconTextChanged(int)));
  connect(m_iconTextMenu, SIGNAL(aboutToShow()), SLOT(slotIconTextMenuAboutToShow()));
  connect(this, SIGNAL(removeAction(const QString&, const QString&)), ToolbarTabWidget::ref(), SIGNAL(removeAction(const QString&, const QString&)));
  connect(this, SIGNAL(editAction(const QString&)), ToolbarTabWidget::ref(), SIGNAL(editAction(const QString&)));
  setToolButtonStyle(ToolbarTabWidget::ref()->iconText());
}

void UserToolBar::slotIconTextMenuAboutToShow()
{
  m_iconTextMenu->setItemChecked(0, false);
  m_iconTextMenu->setItemChecked(1, false);
  m_iconTextMenu->setItemChecked(2, false);
  m_iconTextMenu->setItemChecked(3, false);
  switch (ToolbarTabWidget::ref()->iconText())
  {
    case Qt::ToolButtonIconOnly:
        m_iconTextMenu->setItemChecked(0, true);
        break;
    case Qt::ToolButtonTextOnly:
        m_iconTextMenu->setItemChecked(1, true);
        break;
    case Qt::ToolButtonTextBesideIcon:
        m_iconTextMenu->setItemChecked(2, true);
        break;
    case Qt::ToolButtonTextUnderIcon:
        m_iconTextMenu->setItemChecked(3, true);
        break;
  }
}

void UserToolBar::slotIconTextChanged(int id)
{
  ToolbarTabWidget *toolbarTab = ToolbarTabWidget::ref();
  int width = toolbarTab->width();
  int bigHeight = iconSize().height() + QFontMetrics(KGlobalSettings::toolBarFont()).height() + 10;
  int normalHeight = iconSize().height() + 10;
  for (int i = 0; i < toolbarTab->count(); i++)
  {
    UserToolBar *tb = static_cast<UserToolBar*>(toolbarTab->page(i));
    switch (id)
    {
      case 0: tb->setToolButtonStyle(Qt::ToolButtonIconOnly);
              tb->setGeometry(0,0, width, normalHeight);
              break;
      case 1: tb->setToolButtonStyle(Qt::ToolButtonTextOnly);
              tb->setGeometry(0,0, width, normalHeight);
              break;
      case 2: tb->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
              tb->setGeometry(0,0, width, normalHeight);
              break;
      case 3: tb->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
              tb->setGeometry(0,0, width, bigHeight);
              break;
    }
  }
//FIXME: is it needed?  toolbarTab->setToolButtonStyle(iconText());
  if (id == 3)
  {
    toolbarTab->setFixedHeight(toolbarTab->tabHeight() + height() + 3);
  } else
  {
    toolbarTab->setFixedHeight(toolbarTab->tabHeight() + height() + 3);
  }
}

void UserToolBar::mousePressEvent(QMouseEvent *e)
{
  if (e->button() == Qt::RightButton)
  {
    //build the context menu, based on the mouse position
    m_popupMenu->clear();
    QPoint p = e->globalPos();
    ToolbarTabWidget *toolbarTab = ToolbarTabWidget::ref();
    if (toolbarTab)
    {
      //common entries
      toolbarTab->tabUnderMouse = toolbarTab->id(toolbarTab->currentPageIndex());
      toolbarTab->tabUnderMouseLabel = toolbarTab->label(toolbarTab->currentPageIndex());
//FIXME:      m_popupMenu->insertTitle(i18n("Toolbar Menu") + " - "
//                               + i18n(toolbarTab->tabUnderMouseLabel.toUtf8()));
      m_popupMenu->addAction(i18n("New Action..."), toolbarTab, SIGNAL(newAction()));
      //find if the user clicked on an action button or not
      QList<QObject*> childrenList = children();
      for (int i = 0; i < childrenList.count(); i++)
      {
        QToolButton *w = dynamic_cast<QToolButton*>(childrenList.at(i));
        if (w)
        {
          QPoint p1 = w->parentWidget()->mapToGlobal(w->pos());
          QPoint p2 = QPoint(p1.x() + w->width(), p1.y()+w->height());
          if (QRect(p1, p2).contains(p))
          {
            currentActionName = w->textLabel();
            QString actionName = currentActionName;
            m_popupMenu->addAction(i18n("Remove Action - %1").arg(actionName.replace('&',"&&")), this, SLOT(slotRemoveAction()));
            m_popupMenu->addAction(i18n("Edit Action - %1").arg(actionName), this, SLOT(slotEditAction()));
            break;
          }
        }
      }
      //more common entries
      m_popupMenu->addSeparator();
      m_popupMenu->addAction(i18n("New Toolbar..."), toolbarTab, SIGNAL(addToolbar()));
      m_popupMenu->addAction(i18n("Remove Toolbar"), toolbarTab, SLOT(slotRemoveToolbar()));
      m_popupMenu->addAction(i18n("Rename Toolbar..."), toolbarTab, SLOT(slotRenameToolbar()));
      m_popupMenu->addSeparator();
      m_popupMenu->addMenu(m_iconTextMenu);
      m_popupMenu->addAction(SmallIconSet("configure_toolbars"), i18n("Configure Toolbars..."), toolbarTab, SLOT(slotEditToolbar()));
    }
    m_popupMenu->popup(p);
  }
}

void UserToolBar::slotEditAction()
{
  emit editAction(currentActionName);
}

void UserToolBar::slotRemoveAction()
{
  if ( KMessageBox::warningContinueCancel(this, i18n("<qt>Are you sure you want to remove the <b>%1</b> action?</qt>").arg(currentActionName),QString::null,KStdGuiItem::del()) == KMessageBox::Continue )
  {
    emit removeAction(ToolbarTabWidget::ref()->tabUnderMouse, currentActionName);
  }
}

#include "usertoolbar.moc"
