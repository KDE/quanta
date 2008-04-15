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
#include <QTextStream>
#include <QDomElement>

//kde includes
#include <kdebug.h>
#include <klocale.h>
#include <kglobalsettings.h>
#include <ktoolbar.h>

#include <core.h>
#include <iuicontroller.h>

#if 0
//code to create a toolview for each toolbar
class UTViewFactory: public KDevelop::IToolViewFactory
{
  public:
    UTViewFactory(const QDomElement &element) : m_domElement(element)
    {}
    
    virtual QWidget* create( QWidget *parent = 0 )
    {
      UserToolBar *tb = new UserToolBar(parent, m_domElement.attribute("name").toUtf8(), true, true);
      tb->loadState(m_domElement);
      tb->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
      tb->setWindowTitle(i18n(m_domElement.attribute( "i18ntabname", "" ).toUtf8()));
      
      return tb;
    }
    virtual Qt::DockWidgetArea defaultPosition(const QString &/*areaName*/)
    {
      return Qt::TopDockWidgetArea;
    }
    
  private:
    QDomElement m_domElement;
};
#endif

QWidget *ToolbarGUIBuilder::createContainer(QWidget *parent, int index, const QDomElement &element, QAction* &action)
{
//  kDebug(24000) << "createContainer: this=" << this << " parent=" << parent;
  QWidget *container = 0L;
  QString tabname = element.attribute( "i18ntabname", "" );
  QString idStr = element.attribute( "id", "" );

  ToolbarTabWidget *toolbarTab = ToolbarTabWidget::ref();
  if ( element.tagName().toLower() == "toolbar" && !tabname.isEmpty())
  {
#if 0
    //code to create a toolview for each toolbar
    UTViewFactory *factory = new UTViewFactory(element);
    KDevelop::Core::self()->uiController()->addToolView(i18n(tabname.toUtf8()), factory);        
    return container;
#endif
        
    //create the toolbar on the tabwidget
    QWidget *w = new QWidget(toolbarTab);
    w->setObjectName(QString("ToolbarHoldingWidget" + element.attribute("name")).toUtf8());
    UserToolBar *tb = new UserToolBar(w, element.attribute("name").toUtf8(), true, true);
    tb->loadState(element);
    tb->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

    //set the correct fixed height of the toolbar
  //kDebug(24000) << "tb->iconSize() " << tb->iconSize();
    
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
   kDebug(24000) << "tb->height() " << tb->height();
   kDebug(24000) << "toolbarTab->height() " << toolbarTab->height();
   kDebug(24000) << "toolbarTab->tabHeight() " << toolbarTab->tabHeight();
   */
    
    toolbarTab->insertTab(tb, i18n(tabname.toUtf8()), idStr);
    toolbarTab->setCurrentWidget(w);
    toolbarTab->show();
    if (m_userToolbar)
      m_userToolbar->show();
     container = tb;
  } else
  {
    container = KXMLGUIBuilder::createContainer(parent, index, element, action);
  }

  //The tabwidget needs to be the inside the user toolbar, so detect when it is created
  //and add to it.
  if (element.attribute("name") == "userToolbar" && dynamic_cast<QToolBar*>(container))
  {
    m_userToolbar = static_cast<QToolBar*>(container);
    m_userToolbar->addWidget(toolbarTab);
    if (toolbarTab->count() == 0 || m_separateToolbars)
      container->hide();
  }
  return container;
}

void ToolbarGUIBuilder::removeContainer(QWidget *container, QWidget *parent, QDomElement &element, QAction* action)
{
  /*
  QString s;
  QTextStream str(&s, IO_ReadWrite);
  element.save(str, 2);
  kDebug(24000) << "Remove element:" << s;
  kDebug(24000) << "removeContainer: this=" << this << " parent=" << parent << "container = " << container;
  */
  
  //We need to reparent the tabwidget, otherwise it gets deleted when for example the
  //toolbars are configured and the GUI is rebuilt.
  if (!m_separateToolbars)
  {
    ToolbarTabWidget *toolbarTab = ToolbarTabWidget::ref();
    if (container == m_userToolbar)
      toolbarTab->setParent(0);
  
    QString tabname = element.attribute( "i18ntabname", "" );
    QString idStr = element.attribute( "id", "" );
  
    if ( element.tagName().toLower() == "toolbar" && !tabname.isEmpty())
    {
      UserToolBar *tb = dynamic_cast<UserToolBar*>(toolbarTab->page(idStr));
      if (tb)
      {
        toolbarTab->removePage(tb);
        if (toolbarTab->count() == 0 && m_userToolbar)
          m_userToolbar->hide();
        return;
      }
    }
  }
  KXMLGUIBuilder::removeContainer(container, parent, element, action);
}

