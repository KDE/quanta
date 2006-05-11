/***************************************************************************
 *   Copyright (C) 2005 by Jens Herden                                     *
 *   jens@kdewebdev.org                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "projecttreepart.h"
#include "projecttreewidget.h"
#include "projecttreeglobalconfig.h"
#include "projecttreeprojectconfig.h"
#include "projecttreeview.h"

#include <QTimer>
#include <QMenu>
#include <QWhatsThis>

#include <klocale.h>
#include <kaction.h>
#include <kdialogbase.h>
#include <kiconloader.h>
#include <kmessagebox.h>

//kdevelop includes
#include <interfaces/kdevplugininfo.h>
#include <interfaces/kdevgenericfactory.h>
#include <interfaces/kdevcore.h>
#include <interfaces/kdevmainwindow.h>
#include <util/configwidgetproxy.h>


typedef KDevGenericFactory<ProjectTreePart> ProjectTreeFactory;
KDevPluginInfo data("kdevprojecttree");
K_EXPORT_COMPONENT_FACTORY( libkdevprojecttree, ProjectTreeFactory( data ) );

#define GLOBALDOC_OPTIONS 1
#define PROJECTDOC_OPTIONS 2

ProjectTreePart::ProjectTreePart(QObject *parent, const QStringList &/*args*/)
    : KDevPlugin(&data, parent)
{
  setInstance(ProjectTreeFactory::instance());
  setXMLFile("kdevprojecttree.rc");

  m_widget = new ProjectTreeWidget(this);
  m_widget->setWindowTitle(i18n("Project Tree"));
  m_widget->setWindowIcon(SmallIcon(info()->icon()));

  m_widget->setWhatsThis(i18n("This treeview manages the files and folers of your project."));

  // now you decide what should happen to the widget. Take a look at kdevcore.h
  // or at other plugins how to embed it.

  // if you want to embed your widget as an outputview, simply uncomment
  // the following line.
//   mainWindow()->embedOutputView( m_widget, "name that should appear", "enter a tooltip" );

  // if you want to embed your widget as a selectview (at the left), simply uncomment
  // the following line.
  mainWindow()->embedSelectView( m_widget, i18n("Project Tree"), "Project Managment" );

  // if you want to embed your widget as a selectview (at the right), simply uncomment
  // the following line.
  // mainWindow()->embedSelectViewRight( m_widget, "name that should appear", "enter a tooltip" );

  setupActions();

  m_configProxy = new ConfigWidgetProxy(core());
  m_configProxy->createGlobalConfigPage(i18n("Project Tree"), GLOBALDOC_OPTIONS, info()->icon());
  m_configProxy->createProjectConfigPage(i18n("Project Tree"), PROJECTDOC_OPTIONS, info()->icon());
  connect(m_configProxy, SIGNAL(insertConfigWidget(const KDialogBase*, QWidget*, unsigned int )),
          this, SLOT(insertConfigWidget(const KDialogBase*, QWidget*, unsigned int)));

  connect(core(), SIGNAL(contextMenu(QMenu *, const Context *)),
          this, SLOT(contextMenu(QMenu *, const Context *)));
  connect(core(), SIGNAL(projectOpened()), this, SLOT(projectOpened()));
  connect(core(), SIGNAL(projectClosed()), this, SLOT(projectClosed()));


  QTimer::singleShot(0, this, SLOT(init()));
}

ProjectTreePart::~ProjectTreePart()
{
  // if you embed a widget, you need to tell the mainwindow when you remove it
  if ( m_widget )
  {
    mainWindow()->removeView( m_widget );
  }
  delete m_widget;
  delete m_configProxy;
}

void ProjectTreePart::init()
{
  // delayed initialization stuff goes here
}

void ProjectTreePart::setupActions()
{
  // create XMLGUI actions here
}

void ProjectTreePart::insertConfigWidget(const KDialogBase *dlg, QWidget *page, unsigned int pageNo)
{
  // create configuraton dialogs here
  switch (pageNo)
  {
    case GLOBALDOC_OPTIONS:
      {
        ProjectTreeGlobalConfig *w = new ProjectTreeGlobalConfig(this, page);
        connect(dlg, SIGNAL(okClicked()), w, SLOT(accept()));
        connect(w, SIGNAL(accepted()), m_widget->tree(), SLOT(slotSettingsChanged()));
        break;
      }
    case PROJECTDOC_OPTIONS:
    {
      ProjectTreeProjectConfig *w = new ProjectTreeProjectConfig(this, page);
      connect(dlg, SIGNAL(okClicked()), w, SLOT(accept()));
      break;
    }
  }
}

void ProjectTreePart::contextMenu(QMenu */*popup*/, const Context *context)
{
  // put actions into the context menu here
  if (context->hasType(Context::EditorContext))
  {
    // editor context menu
//     const EditorContext *econtext = static_cast<const EditorContext*>(context);

    // use context and plug actions here
//     action->plug(popup);

    // or create menu items on the fly
    // int id = -1;
    // id = popup->insertItem(i18n("Do Something Here"),
    //     this, SLOT(doSomething()) );
    // popup->setWhatsThis(id, i18n("<b>Do something here</b><p>Describe here what does this action do."
  }
  else if (context->hasType(Context::FileContext))
  {
    // file context menu
//     const FileContext *fcontext = static_cast<const FileContext*>(context);

    //use context and plug actions here
  }
  else if (context->hasType(Context::ProjectItemContext))
  {
    // project tree context menu
//     const ProjectItemContext *pcontext = static_cast<const ProjectItemContext*>(context);

    // use context and plug actions here
  }
  else if (context->hasType(Context::CodeItemContext))
  {
    // class tree context menu
//     const CodeItemContext *mcontext = static_cast<const CodeItemContext*>(context);

    // use context and plug actions here
  }
  else if (context->hasType(Context::DocumentationContext))
  {
    // documentation viewer context menu
//     const DocumentationContext *dcontext = static_cast<const DocumentationContext*>(context);

    // use context and plug actions here
  }
}

void ProjectTreePart::projectOpened()
{
  // do something when the project is opened
}

void ProjectTreePart::projectClosed()
{
  // do something when the project is closed
}


void ProjectTreePart::savePartialProjectSession(QDomElement *el)
{
  Q_UNUSED(el)
  emit saveProject();
}

#include "projecttreepart.moc"

// kate: space-indent on; indent-width 2; mixedindent off; indent-mode cstyle;
