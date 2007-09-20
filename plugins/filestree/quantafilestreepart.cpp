/***************************************************************************
 *   Copyright (C) 2005-2006 by Jens Herden                                *
 *   jens@kdewebdev.org                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "quantafilestreepart.h"
#include "filestreeview.h"
#include "quantafilestreeglobalconfig.h"
#include "quantafilestreeprojectconfig.h"

#include <QTimer>
#include <QMenu>
#include <QWhatsThis>
#include <QLayout>
#include <qtoolbox.h>

#include <klocale.h>
#include <kaction.h>
#include <kdialog.h>
#include <kiconloader.h>
#include <kmessagebox.h>
#include <k3listviewsearchline.h>
#include <kpluginfactory.h>
#include <kpluginloader.h>


#include <kparts/mainwindow.h>

//kdevelop includes
#include <core.h>
#include <idocumentcontroller.h>
#include <iprojectcontroller.h>
#include <iuicontroller.h>

#define GLOBALDOC_OPTIONS 1
#define PROJECTDOC_OPTIONS 2
    
K_PLUGIN_FACTORY(QuantaFilesTreeFactory, registerPlugin<QuantaFilesTreePart>();)
K_EXPORT_PLUGIN(QuantaFilesTreeFactory("kdevquantafilestree"))


    
class FilesTreeViewFactory: public KDevelop::IToolViewFactory{
  public:
    FilesTreeViewFactory(QuantaFilesTreePart *part): m_part(part) {}

    virtual QWidget* create(QWidget *parent = 0)
    {
      FilesTreeView *tree = new FilesTreeView(m_part, parent);
      QObject::connect(KDevelop::Core::self()->documentController(), SIGNAL(documentClosed(KDevelop::IDocument*)), tree, SLOT(slotDocumentClosed(KDevelop::IDocument*)));

      K3ListViewSearchLineWidget * sl = new K3ListViewSearchLineWidget(tree, parent);

      QVBoxLayout *l = new QVBoxLayout(parent);
      l->addWidget(sl);
      l->addWidget(tree);
      return tree;
    }

    virtual Qt::DockWidgetArea defaultPosition(const QString &/*areaName*/)
    {
      return Qt::LeftDockWidgetArea;
    }

  private:
    QuantaFilesTreePart *m_part;
};

QuantaFilesTreePart::QuantaFilesTreePart(QObject *parent, const QVariantList &/*args*/)
  : KDevelop::IPlugin(QuantaFilesTreeFactory::componentData(), parent)
{
    setXMLFile("kdevquantafilestree.rc");
    FilesTreeViewFactory *factory = new FilesTreeViewFactory(this);
    core()->uiController()->addToolView( "File Manager", factory );
    
    setupActions();
//FIXME: New KCM modules need to be created for each config page
    /*
    m_configProxy = new ConfigWidgetProxy(core());
    m_configProxy->createGlobalConfigPage(i18n("Files Tree"), GLOBALDOC_OPTIONS, info()->icon());
//     m_configProxy->createProjectConfigPage(i18n("Files Tree"), PROJECTDOC_OPTIONS, info()->icon());
    connect(m_configProxy, SIGNAL(insertConfigWidget(const KDialog*, QWidget*, unsigned int )),
        this, SLOT(insertConfigWidget(const KDialog*, QWidget*, unsigned int)));
    */
    connect(KDevelop::Core::self()->uiController()->activeMainWindow(), SIGNAL(contextMenu(QMenu *, const KDevelop::Context *)),
        this, SLOT(contextMenu(QMenu *, const Context *)));
    connect(KDevelop::Core::self()->projectController(), SIGNAL(projectOpened()), this, SLOT(projectOpened()));
    connect(KDevelop::Core::self()->projectController(), SIGNAL(projectClosed()), this, SLOT(projectClosed()));

    QTimer::singleShot(0, this, SLOT(init()));
}

QuantaFilesTreePart::~QuantaFilesTreePart()
{
}
  
void QuantaFilesTreePart::init()
{
// delayed initialization stuff goes here
}

void QuantaFilesTreePart::setupActions()
{
// create XMLGUI actions here
/*    action = new KAction(i18n("&Do Something..."), 0,
        this, SLOT(doSomething()), actionCollection(), "plugin_action" );
    action->setToolTip(i18n("Do something"));
    action->setWhatsThis(i18n("<b>Do something</b><p>Describe here what does this action do."));*/
}

void QuantaFilesTreePart::insertConfigWidget(const KDialog *dlg, QWidget *page, unsigned int pageNo)
{
// create configuraton dialogs here
  /*
    switch (pageNo)
    {
        case GLOBALDOC_OPTIONS:
        {
            QuantaFilesTreeGlobalConfig *w = new QuantaFilesTreeGlobalConfig(this, page);
            connect(dlg, SIGNAL(okClicked()), w, SLOT(accept()));
            connect(w, SIGNAL(accepted()), m_tree, SLOT(slotSettingsChanged()));
            break;
        }
        case PROJECTDOC_OPTIONS:
        {
            QuantaFilesTreeProjectConfig *w = new QuantaFilesTreeProjectConfig(this, page);
            connect(dlg, SIGNAL(okClicked()), w, SLOT(accept()));
            break;
        }
    }*/
}

//FIXME: context menu handling was changed!
void QuantaFilesTreePart::contextMenu(QMenu */*popup*/, const KDevelop::Context */*context*/)
{
// put actions into the context menu here
/*    if (context->hasType(Context::EditorContext))
    {
        // editor context menu
        const EditorContext *econtext = static_cast<const EditorContext*>(context);

        // use context and plug actions here
        action->plug(popup);

        // or create menu items on the fly
        // int id = -1;
        // id = popup->insertItem(i18n("Do Something Here"),
        //     this, SLOT(doSomething()) );
        // popup->setWhatsThis(id, i18n("<b>Do something here</b><p>Describe here what does this action do."
    }
    else if (context->hasType(Context::FileContext))
    {
        // file context menu
        const FileContext *fcontext = static_cast<const FileContext*>(context);

        //use context and plug actions here
    }
    else if (context->hasType(Context::ProjectModelItemContext))
    {
        // project tree context menu
        const ProjectModelItemContext *pcontext = static_cast<const ProjectModelItemContext*>(context);

        // use context and plug actions here
    }
    else if (context->hasType(Context::CodeModelItemContext))
    {
        // class tree context menu
        const CodeModelItemContext *mcontext = static_cast<const CodeModelItemContext*>(context);

        // use context and plug actions here
    }
    else if (context->hasType(Context::DocumentationContext))
    {
        // documentation viewer context menu
        const DocumentationContext *dcontext = static_cast<const DocumentationContext*>(context);

        // use context and plug actions here
    }*/
}

void QuantaFilesTreePart::projectOpened()
{
// do something when the project is opened
}

void QuantaFilesTreePart::projectClosed()
{
// do something when the project is closed
}

// void QuantaFilesTreePart::doSomething()
// {
// // do something useful here instead of showing the message box
//     KMessageBox::information(m_widget, i18n("This action does nothing."), i18n("QuantaFilesTree Plugin"));
// }

#include "quantafilestreepart.moc"

// kate: space-indent on; indent-width 2; mixedindent off; indent-mode cstyle;
