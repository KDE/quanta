/***************************************************************************
 *   Copyright (C) 2005-2006 by Jens Herden                                *
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

#include "quantafilestreepart.h"

#include <QTimer>
#include <QMenu>
#include <QWhatsThis>
#include <QLayout>
#include <qtoolbox.h>

#include <klocale.h>
#include <kaction.h>
#include <kdialogbase.h>
#include <kiconloader.h>
#include <kmessagebox.h>
#include <k3listviewsearchline.h>

//kdevelop includes
#include <interfaces/kdevplugininfo.h>
#include <interfaces/kdevgenericfactory.h>
#include <interfaces/kdevdocumentcontroller.h>
#include <interfaces/kdevcore.h>
#include <interfaces/kdevmainwindow.h>
#include <util/configwidgetproxy.h>

#include "filestreeview.h"
#include "quantafilestreeglobalconfig.h"
#include "quantafilestreeprojectconfig.h"

typedef KDevGenericFactory<QuantaFilesTreePart> QuantaFilesTreeFactory;
KDevPluginInfo data("kdevquantafilestree");
K_EXPORT_COMPONENT_FACTORY( libkdevquantafilestree, QuantaFilesTreeFactory( data ) );

#define GLOBALDOC_OPTIONS 1
#define PROJECTDOC_OPTIONS 2

QuantaFilesTreePart::QuantaFilesTreePart(QObject *parent, const QStringList &/*args*/)
    : KDevPlugin(&data, parent)
{
    setInstance(QuantaFilesTreeFactory::instance());
    setXMLFile("kdevquantafilestree.rc");

    m_widget = new QWidget();
    m_widget->setObjectName("FilesTreeWidget");
    m_widget->setWindowTitle("Files Tree");
    m_widget->setWindowIcon(SmallIcon(info()->icon()));

    m_widget->setWhatsThis(i18n("Here you can manage your filesystem, either local or remote."));

    // now you decide what should happen to the widget. Take a look at kdevcore.h
    // or at other plugins how to embed it.

    // if you want to embed your widget as an outputview, simply uncomment
    // the following line.
    // mainWindow()->embedOutputView( m_widget, "name that should appear", "enter a tooltip" );

    // if you want to embed your widget as a selectview (at the left), simply uncomment
    // the following line.
    mainWindow()->embedSelectView( m_widget, "Files Tree", "File management" );

    // if you want to embed your widget as a selectview (at the right), simply uncomment
    // the following line.
    // mainWindow()->embedSelectViewRight( m_widget, "name that should appear", "enter a tooltip" );

    setupActions();

    m_configProxy = new ConfigWidgetProxy(core());
    m_configProxy->createGlobalConfigPage(i18n("Files Tree"), GLOBALDOC_OPTIONS, info()->icon());
//     m_configProxy->createProjectConfigPage(i18n("Files Tree"), PROJECTDOC_OPTIONS, info()->icon());
    connect(m_configProxy, SIGNAL(insertConfigWidget(const KDialogBase*, QWidget*, unsigned int )),
        this, SLOT(insertConfigWidget(const KDialogBase*, QWidget*, unsigned int)));

    connect(core(), SIGNAL(contextMenu(QMenu *, const Context *)),
        this, SLOT(contextMenu(QMenu *, const Context *)));
    connect(core(), SIGNAL(projectOpened()), this, SLOT(projectOpened()));
    connect(core(), SIGNAL(projectClosed()), this, SLOT(projectClosed()));

    QTimer::singleShot(0, this, SLOT(init()));
}

QuantaFilesTreePart::~QuantaFilesTreePart()
{
// if you embed a widget, you need to tell the mainwindow when you remove it
    if ( m_widget )
    {
        mainWindow()->removeView( m_widget );
    }
    delete m_widget;
    delete m_configProxy;
}

void QuantaFilesTreePart::init()
{
// delayed initialization stuff goes here
  m_tree = new FilesTreeView(this, m_widget);
  connect(documentController(), SIGNAL(documentClosed(KDevDocument*)), m_tree, SLOT(slotDocumentClosed(KDevDocument*)));

  K3ListViewSearchLineWidget * sl = new K3ListViewSearchLineWidget(m_tree, m_widget);

  QVBoxLayout *l = new QVBoxLayout(m_widget);
  l->addWidget(sl);
  l->addWidget(m_tree);
}

void QuantaFilesTreePart::setupActions()
{
// create XMLGUI actions here
/*    action = new KAction(i18n("&Do Something..."), 0,
        this, SLOT(doSomething()), actionCollection(), "plugin_action" );
    action->setToolTip(i18n("Do something"));
    action->setWhatsThis(i18n("<b>Do something</b><p>Describe here what does this action do."));*/
}

void QuantaFilesTreePart::insertConfigWidget(const KDialogBase *dlg, QWidget *page, unsigned int pageNo)
{
// create configuraton dialogs here
    switch (pageNo)
    {
        case GLOBALDOC_OPTIONS:
        {
            QuantaFilesTreeGlobalConfig *w = new QuantaFilesTreeGlobalConfig(this, page);
            connect(dlg, SIGNAL(okClicked()), w, SLOT(accept()));
            connect(w, SIGNAL(accepted()), m_tree, SLOT(slotSettingsChanged()));
            break;
        }
/*        case PROJECTDOC_OPTIONS:
        {
            QuantaFilesTreeProjectConfig *w = new QuantaFilesTreeProjectConfig(this, page);
            connect(dlg, SIGNAL(okClicked()), w, SLOT(accept()));
            break;
        }*/
    }
}

void QuantaFilesTreePart::contextMenu(QMenu */*popup*/, const Context */*context*/)
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
