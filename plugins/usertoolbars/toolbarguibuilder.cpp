/***************************************************************************
    begin                : Wed May 18 2005
    copyright            : (C) 2005 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/

//own includes
#include "toolbarguibuilder.h"
#include "toolbartabwidget.h"
#include "usertoolbar.h"

//qt includes
#include <qtextstream.h>

//kde includes
#include <kdebug.h>
#include <klocale.h>
#include <kglobalsettings.h>
#include <ktoolbar.h>

// from kfiledialog.cpp - avoid qt warning in STDERR (~/.xsessionerrors)
static void silenceQToolBar(QtMsgType, const char *){}

QWidget *ToolbarGUIBuilder::createContainer(QWidget *parent, int index, const QDomElement &element, int &id)
{
//  kDebug(24000) << "createContainer: this=" << this << " parent=" << parent << endl;
  QWidget *container = 0L;
  QString tabname = element.attribute( "i18ntabname", "" );
  QString idStr = element.attribute( "id", "" );

  ToolbarTabWidget *toolbarTab = ToolbarTabWidget::ref();
  if ( element.tagName().toLower() == "toolbar" && !tabname.isEmpty())
  {
    //avoid QToolBar warning in the log
    QtMsgHandler oldHandler = qInstallMsgHandler(silenceQToolBar);

    //create the toolbar on the tabwidget
    QWidget *w = new QWidget(toolbarTab, QString("ToolbarHoldingWidget" + element.attribute("name")).toUtf8());
    UserToolBar *tb = new UserToolBar(w, element.attribute("name").toUtf8(), true, true);
    tb->loadState(element);
    tb->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

    //set the correct fixed height of the toolbar
  //kDebug(24000) << "tb->iconSize() " << tb->iconSize() << endl;
    if (toolbarTab->iconText() == Qt::ToolButtonTextUnderIcon)
    {
      tb->setGeometry(0,0, toolbarTab->width(), tb->iconSize().height() + QFontMetrics(KGlobalSettings::toolBarFont()).height() + 10);
      toolbarTab->setFixedHeight(toolbarTab->tabHeight() + tb->height() + 3);
    } else
    {
      tb->setGeometry(0,0, toolbarTab->width(), tb->iconSize().height() + 10);
      toolbarTab->setFixedHeight(toolbarTab->tabHeight() + tb->height() + 3);
    }
/*
   kDebug(24000) << "tb->height() " << tb->height() << endl;
   kDebug(24000) << "toolbarTab->height() " << toolbarTab->height() << endl;
   kDebug(24000) << "toolbarTab->tabHeight() " << toolbarTab->tabHeight() << endl;
*/
    toolbarTab->insertTab(tb, i18n(tabname.toUtf8()), idStr);
    toolbarTab->showPage(w);
    if (toolbarTab->parentWidget()->parentWidget())
      toolbarTab->parentWidget()->parentWidget()->show();
    qInstallMsgHandler(oldHandler);
    tb->setSeparate(m_separateToolbars);

    if (m_separateToolbars)
    {
      container = KXMLGUIBuilder::createContainer(parent, index, element, id);
      static_cast<KToolBar*>(container)->setWindowTitle(i18n(tabname.toUtf8()));
      toolbarTab->parentWidget()->parentWidget()->hide();
    } else
    {
      container = tb;
    }
  } else
  {
    container = KXMLGUIBuilder::createContainer(parent, index, element, id);
  }

//  kDebug(24000) << "container " << element.attribute("name") << " created: " << container << endl;
  //The tabwidget needs to be the child of the user toolbar, so detect when it is created
  //and make the child of it.
  if (element.attribute("name") == "userToolbar")
  {
    toolbarTab->parentWidget()->reparent(container, QPoint(0,0));
    if (toolbarTab->count() == 0 || m_separateToolbars)
      container->hide();
  }
  return container;
}

void ToolbarGUIBuilder::removeContainer(QWidget *container, QWidget *parent, QDomElement &element, int id)
{
  QString s;
  QTextStream str(&s, IO_ReadWrite);
  element.save(str, 2);
  kDebug(24000) << "Remove element:" << s << endl;
//  kDebug(24000) << "removeContainer: this=" << this << " parent=" << parent << "container = " << container << endl;
  //We need to reparent the tabwidget, otherwise it gets deleted when for example the
  //toolbars are configured and the GUI is rebuilt.
  ToolbarTabWidget *toolbarTab = ToolbarTabWidget::ref();
  if (container == toolbarTab->parentWidget()->parent())
    toolbarTab->parentWidget()->reparent(0, QPoint(0,0));

  QString tabname = element.attribute( "i18ntabname", "" );
  QString idStr = element.attribute( "id", "" );

  if ( element.tagName().toLower() == "toolbar" && !tabname.isEmpty())
  {
    UserToolBar *tb = dynamic_cast<UserToolBar*>(toolbarTab->page(idStr));
    if (tb)
    {
      if (tb->separate())
        KXMLGUIBuilder::removeContainer(container, parent, element, id);
      toolbarTab->removePage(tb);
      if (toolbarTab->count() == 0 && toolbarTab->parentWidget()->parentWidget())
        toolbarTab->parentWidget()->parentWidget()->hide();
      return;
    }
  }

  KXMLGUIBuilder::removeContainer(container, parent, element, id);
}

