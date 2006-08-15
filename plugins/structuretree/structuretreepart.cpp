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

#include "structuretreepart.h"
#include "structuretreewidget.h"
#include "groupswidget.h"
#include "structuretreeglobalconfig.h"
#include "structuretreeprojectconfig.h"
#include "quantacoreif.h"
#include "searchlinewidget.h"

#include <QTimer>
#include <QMenu>
#include <QWhatsThis>
#include <qtoolbox.h>
#include <QLayout>

#include <klocale.h>
#include <kaction.h>
#include <kdialog.h>
#include <kgenericfactory.h>
#include <kiconloader.h>
#include <kmessagebox.h>
#include <k3listviewsearchline.h>

//kdevelop includes
#include <kdevcore.h>
#include <kdevmainwindow.h>
#include <kdevprojectcontroller.h>

typedef KGenericFactory<StructureTreePart> StructureTreeFactory;
K_EXPORT_COMPONENT_FACTORY( libkdevstructuretree, StructureTreeFactory("kdevstructuretree") );

#define GLOBALDOC_OPTIONS 1
#define PROJECTDOC_OPTIONS 2

StructureTreePart::StructureTreePart(QObject *parent, const QStringList &/*args*/)
  : KDevPlugin(StructureTreeFactory::instance(), parent)
{
    setXMLFile("kdevstructuretree.rc");

    m_widget = new QToolBox();
    m_widget->setObjectName("StructureTreeWidget");
    m_widget->setWindowTitle(i18n("Structure Tree"));
//    m_widget->setWindowIcon(SmallIcon(info()->icon()));

    m_widget->setWhatsThis(i18n("Displays the structure of current document. Manipulation of the document is also possible."));

    // now you decide what should happen to the widget. Take a look at kdevcore.h
    // or at other plugins how to embed it.

    // if you want to embed your widget as an outputview, simply uncomment
    // the following line.
    // mainWindow()->embedOutputView( m_widget, "name that should appear", "enter a tooltip" );

    // if you want to embed your widget as a selectview (at the left), simply uncomment
    // the following line.
    KDevCore::mainWindow()->embedSelectView( m_widget, i18n("Document Structure"), i18n("Structure of the current document") );

    // if you want to embed your widget as a selectview (at the right), simply uncomment
    // the following line.
    // mainWindow()->embedSelectViewRight( m_widget, "name that should appear", "enter a tooltip" );

    setupActions();
//FIXME: New KCM modules need to be created for each config page
    /*
    m_configProxy = new ConfigWidgetProxy(core());
    m_configProxy->createGlobalConfigPage(i18n("Document Structure"), GLOBALDOC_OPTIONS, info()->icon());
    m_configProxy->createProjectConfigPage(i18n("Document Structure"), PROJECTDOC_OPTIONS, info()->icon());
    connect(m_configProxy, SIGNAL(insertConfigWidget(const KDialog*, QWidget*, unsigned int )),
        this, SLOT(insertConfigWidget(const KDialog*, QWidget*, unsigned int)));
    */
    connect(KDevCore::mainWindow(), SIGNAL(contextMenu(QMenu *, const Context *)),
        this, SLOT(contextMenu(QMenu *, const Context *)));
    connect(KDevCore::projectController(), SIGNAL(projectOpened()), this, SLOT(projectOpened()));
    connect(KDevCore::projectController(), SIGNAL(projectClosed()), this, SLOT(projectClosed()));


    QTimer::singleShot(0, this, SLOT(init()));
}

StructureTreePart::~StructureTreePart()
{
// if you embed a widget, you need to tell the mainwindow when you remove it
  if ( m_widget )
  {
    KDevCore::mainWindow()->removeView( m_widget );
  }
  delete m_widget;
//  delete m_configProxy;
}

void StructureTreePart::init()
{
// delayed initialization stuff goes here
  QWidget *w = new QWidget(m_widget);
  w->setObjectName("structuretreecontainer");

  m_documentTree = new StructureTreeWidget(this, w);

  K3ListViewSearchLineWidget * sl = new K3ListViewSearchLineWidget(m_documentTree, w);

  QVBoxLayout *l = new QVBoxLayout(w);
  l->addWidget(sl);

  l->addWidget(m_documentTree);

  m_qcore = extension<QuantaCoreIf>("KDevelop/Quanta");
  connect(m_qcore, SIGNAL(startParsing()), m_documentTree, SLOT(slotBlockGUI()));

  connect(m_qcore, SIGNAL(finishedParsing(const ParseResult *)), m_documentTree, SLOT(slotBuild(const ParseResult *)));

  connect(m_qcore, SIGNAL(newCursorPosition(const QPoint &)), m_documentTree, SLOT(slotNewCursorPosition(const QPoint &)));


  m_groupsTree = new GroupsWidget(this, m_widget);

  connect(m_qcore, SIGNAL(startParsing()), m_groupsTree, SLOT(slotBlockGUI()));

  connect(m_qcore, SIGNAL(finishedParsing(const ParseResult *)), m_groupsTree, SLOT(slotBuild(const ParseResult *)));

  connect(m_qcore, SIGNAL(groupsParsed(const ParseResult *)), m_groupsTree, SLOT(slotGroupsParsed(const ParseResult *)));


  // add the widgets to the qtoolbox
  m_widget->addItem(m_groupsTree, i18n("Groups"));
  m_widget->addItem(w, i18n("Document"));

  m_widget->setCurrentIndex(m_widget->indexOf(w));
}

void StructureTreePart::setupActions()
{
// create XMLGUI actions here
}

void StructureTreePart::insertConfigWidget(const KDialog *dlg, QWidget *page, unsigned int pageNo)
{
// create configuraton dialogs here
    switch (pageNo)
    {
        case GLOBALDOC_OPTIONS:
        {
            StructureTreeGlobalConfig *w = new StructureTreeGlobalConfig(this, page);
            connect(dlg, SIGNAL(okClicked()), w, SLOT(accept()));
            connect(w, SIGNAL(accepted()), m_documentTree, SLOT(slotNewSettings()));
            connect(w, SIGNAL(accepted()), m_groupsTree, SLOT(slotNewSettings()));
            break;
        }
        case PROJECTDOC_OPTIONS:
        {
/*            StructureTreeProjectConfig *w = new StructureTreeProjectConfig(this, page);
            connect(dlg, SIGNAL(okClicked()), w, SLOT(accept()));*/
            break;
        }
    }
}

void StructureTreePart::contextMenu(QMenu */*popup*/, const Context */*context*/)
{
// put actions into the context menu here
//     if (context->hasType(Context::EditorContext))
//     {
//         // editor context menu
//         const EditorContext *econtext = static_cast<const EditorContext*>(context);
//
//         // use context and plug actions here
// //         action->plug(popup);
//
//         // or create menu items on the fly
//         // int id = -1;
//         // id = popup->insertItem(i18n("Do Something Here"),
//         //     this, SLOT(doSomething()) );
//         // popup->setWhatsThis(id, i18n("<b>Do something here</b><p>Describe here what does this action do."
//     }
//     else if (context->hasType(Context::FileContext))
//     {
//         // file context menu
//         const FileContext *fcontext = static_cast<const FileContext*>(context);
//
//         //use context and plug actions here
//     }
//     else if (context->hasType(Context::ProjectModelItemContext))
//     {
//         // project tree context menu
//         const ProjectModelItemContext *pcontext = static_cast<const ProjectModelItemContext*>(context);
//
//         // use context and plug actions here
//     }
//     else if (context->hasType(Context::CodeModelItemContext))
//     {
//         // class tree context menu
//         const CodeModelItemContext *mcontext = static_cast<const CodeModelItemContext*>(context);
//
//         // use context and plug actions here
//     }
//     else if (context->hasType(Context::DocumentationContext))
//     {
//         // documentation viewer context menu
//         const DocumentationContext *dcontext = static_cast<const DocumentationContext*>(context);
//
//         // use context and plug actions here
//     }
}

void StructureTreePart::projectOpened()
{
// do something when the project is opened
}

void StructureTreePart::projectClosed()
{
// do something when the project is closed
}


#include "structuretreepart.moc"

// kate: space-indent on; indent-width 2; mixedindent off; indent-mode cstyle;
