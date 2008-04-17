/***************************************************************************
 *   Copyright (C) 2005 by Jens Herden                                     *
 *   jens@kdewebdev.org                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "structuretreepart.h"
#include "structuretreeglobalconfig.h"
#include "structuretreeprojectconfig.h"
#include "quantacoreif.h"
#include "searchlinewidget.h"
#include "structuretreeview.h"

#include <QTimer>
#include <QMenu>
#include <QWhatsThis>
#include <QToolBox>
#include <QLayout>
#include <QTreeView>

#include <klocale.h>
#include <kaction.h>
#include <kdialog.h>
#include <kiconloader.h>
#include <kmessagebox.h>
#include <kpluginfactory.h>
#include <kpluginloader.h>

#include <kparts/mainwindow.h>

//kdevelop includes
#include <core.h>
#include <iprojectcontroller.h>
#include <iplugincontroller.h>
#include <iuicontroller.h>
#include <iproject.h>

#define GLOBALDOC_OPTIONS 1
#define PROJECTDOC_OPTIONS 2

K_PLUGIN_FACTORY(StructureTreeFactory, registerPlugin<StructureTreePart>();)
K_EXPORT_PLUGIN(StructureTreeFactory("kdevstructuretree"))


class StructureTreeWidgetFactory: public KDevelop::IToolViewFactory
{
  public:
    StructureTreeWidgetFactory( StructureTreePart *part ): m_part( part )
    {}
    virtual QWidget* create( QWidget *parent = 0 )
    {
      
      QToolBox *widget = new QToolBox(parent);
      widget->setObjectName("StructureTreeWidget");
      widget->setWindowTitle(i18n("Structure Tree"));
//    m_widget->setWindowIcon(SmallIcon(info()->icon()));

      widget->setWhatsThis(i18n("Displays the structure of the current document. Manipulation of the document is also possible."));
 
      QWidget *w = new QWidget(widget);
      w->setObjectName("structuretreecontainer");

      //FIXME: here comes
      StructureTreeView *documentTree = new StructureTreeView(w);

      QVBoxLayout *l = new QVBoxLayout(w);
      w->setLayout(l);
      l->setContentsMargins(0, 0, 0, 0);
          //       l->addWidget(sl);
      l->addWidget(documentTree);
     
      KDevelop::IPlugin *corePlugin = KDevelop::Core::self()->pluginController()->pluginForExtension("org.kdevelop.QuantaCoreIf");
      QuantaCoreIf *qcore = corePlugin->extension<QuantaCoreIf>();
  
//       QObject::connect(corePlugin, SIGNAL(startParsing()), documentTree, SLOT(slotBlockGUI()));

      QObject::connect(corePlugin, SIGNAL(finishedParsing(const ParseResult *)), documentTree, SLOT(newDataArrived(const ParseResult *)));

      QObject::connect(corePlugin, SIGNAL(newCursorPosition(const QPoint &)), documentTree, SLOT(slotNewCursorPosition(const QPoint &)));

      QTreeView *groupsTree = new QTreeView(widget);
#if 0
      QObject::connect(corePlugin, SIGNAL(startParsing()), groupsTree, SLOT(slotBlockGUI()));

      QObject::connect(corePlugin, SIGNAL(finishedParsing(const ParseResult *)), groupsTree, SLOT(slotBuild(const ParseResult *)));

      QObject::connect(corePlugin, SIGNAL(groupsParsed(const ParseResult *)), groupsTree, SLOT(slotGroupsParsed(const ParseResult *)));
#endif

  // add the widgets to the qtoolbox
      widget->addItem(groupsTree, i18n("Groups"));
      widget->addItem(w, i18n("Document"));

      widget->setCurrentIndex(widget->indexOf(w));
      
      return widget;
    }
    virtual Qt::DockWidgetArea defaultPosition(const QString &/*areaName*/)
    {
      return Qt::LeftDockWidgetArea;
    }
  private:
    StructureTreePart *m_part;
};
    
    
StructureTreePart::StructureTreePart(QObject *parent, const QVariantList &/*args*/)
  : KDevelop::IPlugin(StructureTreeFactory::componentData(), parent)
{
    setXMLFile("kdevstructuretree.rc");

    StructureTreeWidgetFactory *factory = new StructureTreeWidgetFactory(this);
    core()->uiController()->addToolView("Structure Tree", factory);

    setupActions();
//FIXME: New KCM modules need to be created for each config page
    /*
    m_configProxy = new ConfigWidgetProxy(core());
    m_configProxy->createGlobalConfigPage(i18n("Document Structure"), GLOBALDOC_OPTIONS, info()->icon());
    m_configProxy->createProjectConfigPage(i18n("Document Structure"), PROJECTDOC_OPTIONS, info()->icon());
    QObject::connect(m_configProxy, SIGNAL(insertConfigWidget(const KDialog*, QWidget*, unsigned int )),
        this, SLOT(insertConfigWidget(const KDialog*, QWidget*, unsigned int)));
    */
    connect(KDevelop::Core::self()->uiController()->activeMainWindow(), SIGNAL(contextMenu(QMenu *, const KDevelop::Context *)),
        this, SLOT(contextMenu(QMenu *, const Context *)));
    connect(KDevelop::Core::self()->projectController(), SIGNAL(projectOpened( KDevelop::IProject*)), this, SLOT(projectOpened( KDevelop::IProject*)));
    connect(KDevelop::Core::self()->projectController(), SIGNAL(projectClosed( KDevelop::IProject*)), this, SLOT(projectClosed( KDevelop::IProject*)));


    QTimer::singleShot(0, this, SLOT(init()));
}

StructureTreePart::~StructureTreePart()
{
}
  
void StructureTreePart::init()
{
  KDevelop::IPlugin *corePlugin = KDevelop::Core::self()->pluginController()->pluginForExtension("org.kdevelop.QuantaCoreIf");
  m_qcore = corePlugin->extension<QuantaCoreIf>();
  
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
/*            StructureTreeGlobalConfig *w = new StructureTreeGlobalConfig(this, page);
            QObject::connect(dlg, SIGNAL(okClicked()), w, SLOT(accept()));
            QObject::connect(w, SIGNAL(accepted()), m_documentTree, SLOT(slotNewSettings()));
            QObject::connect(w, SIGNAL(accepted()), m_groupsTree, SLOT(slotNewSettings()));*/
            break;
        }
        case PROJECTDOC_OPTIONS:
        {
/*            StructureTreeProjectConfig *w = new StructureTreeProjectConfig(this, page);
            QObject::connect(dlg, SIGNAL(okClicked()), w, SLOT(accept()));*/
            break;
        }
    }
}

void StructureTreePart::contextMenu(QMenu */*popup*/, const KDevelop::Context */*context*/)
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

void StructureTreePart::projectOpened(KDevelop::IProject *project)
{
// do something when the project is opened
}

void StructureTreePart::projectClosed(KDevelop::IProject *project)
{
// do something when the project is closed
}


#include "structuretreepart.moc"

// kate: space-indent on; indent-width 2; mixedindent off; indent-mode cstyle;
