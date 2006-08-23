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

#include "templatestreeview.h"
#include "templatestreeglobalconfig.h"
#include "templatestreeprojectconfig.h"
#include "templatestreepart.h"
#include "quantacoreif.h"
#include "quantanetaccess.h"
// #include "extfileinfo.h"
#include "helper.h"

#include <QTimer>
#include <QMenu>
#include <QWhatsThis>

#include <klocale.h>
#include <kaction.h>
#include <kdialog.h>
#include <kgenericfactory.h>
#include <kiconloader.h>
#include <kmessagebox.h>
#include <kfiledialog.h>
#include <ktar.h>
#include <kmainwindow.h>
#include <kstandarddirs.h>

//kdevelop includes
#include <kdevdocumentcontroller.h>
#include <kdevcore.h>
#include <kdevcontext.h>
#include <kdevmainwindow.h>
#include <kdevprojectcontroller.h>


typedef KGenericFactory<TemplatesTreePart> TemplatesTreeFactory;
K_EXPORT_COMPONENT_FACTORY( libkdevtemplatestree, TemplatesTreeFactory("kdevtemplatestree") )

#define GLOBALDOC_OPTIONS 1
#define PROJECTDOC_OPTIONS 2

TemplatesTreePart::TemplatesTreePart(QObject *parent, const QStringList &/*args*/)
  : KDevPlugin(TemplatesTreeFactory::instance(), parent)
{
    setXMLFile("kdevtemplatestree.rc");

    m_widget = new TemplatesTreeView(this);
    m_widget->setWindowTitle(i18n("Templates Tree"));
    m_widget->setObjectName("TemplatesTreeWidget");
//    m_widget->setWindowIcon(SmallIcon(info()->icon()));

    m_widget->setWhatsThis(i18n("Working with templates"));

    connect(KDevCore::mainWindow(), SIGNAL(projectOpened()), m_widget, SLOT(slotProjectOpened()));

    setupActions();
//FIXME: New KCM modules need to be created for each config page
    /*
    m_configProxy = new ConfigWidgetProxy(core());
    m_configProxy->createGlobalConfigPage(i18n("TemplatesTree"), GLOBALDOC_OPTIONS, info()->icon());
    m_configProxy->createProjectConfigPage(i18n("TemplatesTree"), PROJECTDOC_OPTIONS, info()->icon());
    connect(m_configProxy, SIGNAL(insertConfigWidget(const KDialog*, QWidget*, unsigned int )),
        this, SLOT(insertConfigWidget(const KDialog*, QWidget*, unsigned int)));
    */
    connect(KDevCore::mainWindow(), SIGNAL(contextMenu(QMenu *, const Context *)),
        this, SLOT(contextMenu(QMenu *, const Context *)));
    connect(KDevCore::projectController(), SIGNAL(projectOpened()), this, SLOT(projectOpened()));
    connect(KDevCore::projectController(), SIGNAL(projectClosed()), this, SLOT(projectClosed()));

    connect(KDevCore::documentController(), SIGNAL(documentClosed(KDevDocument*)), m_widget, SLOT(slotDocumentClosed(KDevDocument*)));

    QTimer::singleShot(0, this, SLOT(init()));
}

TemplatesTreePart::~TemplatesTreePart()
{
}
  

QWidget *TemplatesTreePart::pluginView() const
{
  return m_widget;
}


void TemplatesTreePart::init()
{
// delayed initialization stuff goes here
  m_qcore = extension<QuantaCoreIf>("KDevelop/Quanta");
  connect(m_widget, SIGNAL(insertTag(const KUrl &, Helper::DirInfo *)), m_qcore, SLOT(slotInsertTag( const KUrl&, Helper::DirInfo* )));
}

void TemplatesTreePart::setupActions()
{
// create XMLGUI actions here
  m_createTemplateAction = new KAction(i18n("Create Site &Template..."), actionCollection(), "site_template_action" );
  connect(m_createTemplateAction, SIGNAL(triggered(bool)), SLOT(slotCreateSiteTemplate()));
}

void TemplatesTreePart::insertConfigWidget(const KDialog *dlg, QWidget *page, unsigned int pageNo)
{
// create configuraton dialogs here
    switch (pageNo)
    {
        case GLOBALDOC_OPTIONS:
        {
            TemplatesTreeGlobalConfig *w = new TemplatesTreeGlobalConfig(this, page);
            connect(dlg, SIGNAL(okClicked()), w, SLOT(accept()));
            connect(w, SIGNAL(accepted()), m_widget, SLOT(slotSettingsChanged()));
            break;
        }
        case PROJECTDOC_OPTIONS:
        {
            TemplatesTreeProjectConfig *w = new TemplatesTreeProjectConfig(this, page);
            connect(dlg, SIGNAL(okClicked()), w, SLOT(accept()));
            break;
        }
    }
}

void TemplatesTreePart::contextMenu(QMenu *popup, const Context *context)
{
  if (context->hasType(Context::FileContext))
  {
    m_fileContextList = static_cast<const FileContext*>(context)->urls();
    kDebug(24000) << m_fileContextList.front().fileName(false) << m_fileContextList.front().fileName(false).isEmpty() << endl;
    if ((m_fileContextList.count() == 1) && m_fileContextList.front().fileName(false).isEmpty())
    {
      popup->addSeparator();
      popup->addAction(m_createTemplateAction);
    }
  }
/*
// put actions into the context menu here
    if (context->hasType(Context::EditorContext))
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

void TemplatesTreePart::projectOpened()
{
// do something when the project is opened
}

void TemplatesTreePart::projectClosed()
{
// do something when the project is closed
}


void TemplatesTreePart::slotCreateSiteTemplate()
{
  KUrl url = m_fileContextList.front(); // FIXME we only use the first
   //TODO: Implement creation from remote folders as well. Requires downloading of the files to a
   //temporary directory
  if (url.protocol() != "file")
  {
    KMessageBox::sorry(KDevCore::mainWindow(), i18n("Currently you can create site templates only from local folders."), i18n("Unsupported Feature"));
    return;
  }


/*  FIXME  if (Project::ref()->hasProject())
   {
     startDir = Project::ref()->templateURL().url();
   } else*/

  QString startDir = KStandardDirs::locateLocal("data", Helper::resourceDir() + "templates/");
  KUrl targetURL = KFileDialog::getSaveUrl(startDir, "*.tgz", KDevCore::mainWindow(), i18n("Create Site Template File"));
  if (targetURL.isEmpty())
    return;

/*    if (Project::ref()->hasProject() && targetURL.url().startsWith(Project::ref()->templateURL().url()))
      valid = true;*/
  if (!KUrl(startDir).isParentOf(targetURL))
    KMessageBox::information(KDevCore::mainWindow(), i18n("This Template will not be visible in your Templates Tree, because you do not save it to the local or project template folder."));


  KTempFile *tempFile = new KTempFile(Helper::tmpFilePrefix());
  tempFile->setAutoDelete(true);
  tempFile->close();
  KTar tar(tempFile->name(), "application/x-gzip");
  bool error = false;
  if (tar.open(IO_WriteOnly))
  {
    tar.addLocalDirectory(url.path(), "");
    tar.close();
  } else
    error = true;
  if (!QuantaNetAccess::copy(KUrl(tempFile->name()), targetURL, this, false))
    error = true;

  if (error)
    KMessageBox::error(KDevCore::mainWindow(), i18n("<qt>There was an error while creating the site template tarball.<br>Check that you can read the files from <i>%1</i>, you have write access to <i>%2</i> and that you have enough free space in your temporary folder.</qt>", url.pathOrUrl(), targetURL.pathOrUrl()), i18n("Template Creation Error"));
  delete tempFile;
}

#include "templatestreepart.moc"

// kate: space-indent on; indent-width 2; mixedindent off;