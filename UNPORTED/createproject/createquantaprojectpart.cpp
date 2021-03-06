/***************************************************************************
 *   Copyright (C) 2005 by Andras Mantia   *
 *   amantia@kde.org   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

//app includes
#include "createquantaprojectpart.h"
#include "extfileinfo.h"
#include "firstwizardpage.h"
#include "secondwizardpage.h"
#include "localimportwizardpage.h"
#include "webimportwizardpage.h"
#include "quantacoreif.h"
#include "quantanetaccess.h"

//qt includes
#include <qdom.h>
#include <QFile>
#include <QLineEdit>
#include <QMenu>
#include <qstackedwidget.h>
#include <QTextStream>
#include <QTimer>
#include <QWhatsThis>
#include <q3wizard.h>

//kde includes
#include <kio/netaccess.h>
#include <klocale.h>
#include <kaction.h>
#include <kdebug.h>
#include <kdialog.h>
#include <kicon.h>
#include <kiconloader.h>
#include <kglobal.h>
#include <kxmlguiwindow.h>
#include <kmessagebox.h>
#include <kstandarddirs.h>
#include <ktemporaryfile.h>
#include <kactioncollection.h>
#include <kpluginfactory.h>
#include <kpluginloader.h>


#include <kparts/mainwindow.h>

//kdevelop includes

#include <shell/core.h>
#include <interfaces/iplugincontroller.h>
#include <interfaces/iprojectcontroller.h>
#include <interfaces/iuicontroller.h>

K_PLUGIN_FACTORY(CreateQuantaProjectFactory, registerPlugin<CreateQuantaProjectPart>();)
K_EXPORT_PLUGIN(CreateQuantaProjectFactory("kdevcreatequantaproject"))


#define GLOBALDOC_OPTIONS 1
#define PROJECTDOC_OPTIONS 2

CreateQuantaProjectPart::CreateQuantaProjectPart(QObject *parent, const QVariantList &/*args*/)
  : KDevelop::IPlugin(CreateQuantaProjectFactory::componentData(), parent)
{
  kDebug(24000) << "Quanta create project plugin loaded";
  setXMLFile("kdevcreatequantaproject.rc");

  setupActions();

  QTimer::singleShot(0, this, SLOT(init()));
}

CreateQuantaProjectPart::~CreateQuantaProjectPart()
{
}

void CreateQuantaProjectPart::init()
{
// delayed initialization stuff goes here
}

void CreateQuantaProjectPart::setupActions()
{
// create XMLGUI actions here
  action =  actionCollection()->addAction("new_project");
  action->setText(i18n("&New Project..."));
  action->setIcon(KIcon("document-new"));
  connect(action, SIGNAL(triggered(bool)), SLOT(slotCreateNewProject()));
  action->setToolTip(i18n("Create new project"));
}


void CreateQuantaProjectPart::slotCreateNewProject()
{
  KDevelop::IPlugin *corePlugin =KDevelop::Core::self()->pluginController()->pluginForExtension("org.kdevelop.QuantaCoreIf");
  if (!corePlugin)
  {
    KMessageBox::error(KDevelop::Core::self()->uiController()->activeMainWindow(), i18n("<qt>The <b>create new Quanta project</b> plugin requires the <b>Quanta core</b> plugin to be loaded.</qt>"), i18n("Quanta core not loaded"));
    return;
  }
  QuantaCoreIf *qCore = corePlugin->extension<QuantaCoreIf>();
  
  Q3Wizard *wizard = new Q3Wizard(KDevelop::Core::self()->uiController()->activeMainWindow(), "new", true);
  wizard->setWindowTitle(i18n("New Project Wizard"));
  wizard->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  QStackedWidget *stack = new QStackedWidget(wizard);
  FirstWizardPage *firstPage = new FirstWizardPage(qCore, iconLoader(), wizard);
  SecondWizardPage *secondPage = new SecondWizardPage(stack, iconLoader(), wizard);

  LocalImportWizardPage *localImportPage = new LocalImportWizardPage(iconLoader(), stack);
  WebImportWizardPage *webImportPage = new WebImportWizardPage(iconLoader(), stack);
  stack->addWidget(localImportPage);
  stack->addWidget(webImportPage);

  wizard->addPage(firstPage, i18n("<b>Main Project Settings</b>"));
  wizard->addPage(secondPage, i18n("<b>Detailed Project Settings</b>"));
  wizard->addPage(stack, i18n("<b>Insert Files in Project</b>"));

  wizard->setNextEnabled(firstPage, false);
  wizard->setFinishEnabled(firstPage, false);
  wizard->setBackEnabled(secondPage, true);
  wizard->setNextEnabled(secondPage, true);
  wizard->setFinishEnabled(secondPage, true);
  wizard->setBackEnabled(stack, true);
  wizard->setFinishEnabled(stack, true);
  wizard->setNextEnabled(stack, true);

  connect(firstPage, SIGNAL(enableNextButton(QWidget *,bool)), wizard, SLOT(setNextEnabled(QWidget*,bool)));
  connect(firstPage, SIGNAL(enableFinishButton(QWidget *,bool)), wizard, SLOT(setFinishEnabled(QWidget*,bool)));
  connect(firstPage, SIGNAL(newBaseURL(const KUrl&)), localImportPage, SLOT(setBaseURL(const KUrl&)));
  connect(firstPage, SIGNAL(newBaseURL(const KUrl&)), webImportPage, SLOT(setBaseURL(const KUrl&)));
  connect(firstPage, SIGNAL(enableWebDownload(bool)), secondPage, SLOT(slotEnableWebDownload(bool)));

/*FIXME What output should we use?
  connect(webImportPage, SIGNAL(enableMessagesWidget()), this, SIGNAL(enableMessageWidget()));
  connect(webImportPage, SIGNAL(messages(const QString&)), this, SLOT  (slotGetMessages(const QString&)));
*/

  connect(webImportPage, SIGNAL(enableNextButton(QWidget *,bool)), wizard, SLOT(setNextEnabled(QWidget*,bool)));
  connect(webImportPage, SIGNAL(enableNextButton(QWidget *,bool)), wizard, SLOT(setBackEnabled(QWidget*,bool)));


  firstPage->linePrjName->setFocus();
  if (wizard->exec())
  {
    KUrl baseURL = firstPage->baseURL();
    if (!ExtFileInfo::exists(baseURL) && !QuantaNetAccess::mkdir(baseURL, 0L /*don't try to add to the project*/, -1, false))
      return;//TODO Error message
    QString templateFile = KGlobal::dirs()->findResource("data", "kdevcreatequantaproject/quanta-project.template");
    if (templateFile.isNull())
      return; //TODO Error message
    QDomDocument projectDom;
    QFile f(templateFile);
    if (!f.open(IO_ReadOnly))
      return;
    QString domContent = f.readAll();
    domContent.replace("%{AUTHOR}", firstPage->author());
    domContent.replace("%{EMAIL}", firstPage->email());
    domContent.replace("%{VERSION}", "0.1");
    projectDom.setContent(domContent);
    //FIXME: The new format is not XML!!!
//     QDomElement el = Koncrete::DomUtil::createElementByPath(projectDom, "/project/templates");
//     el.setAttribute("url", secondPage->templates());
//     el = Koncrete::DomUtil::createElementByPath(projectDom, "/project/toolbars");
//     el.setAttribute("url", secondPage->toolbars());
//     el = Koncrete::DomUtil::elementByPath(projectDom, "/project");
//     el.setAttribute("name", firstPage->name());
//     el.setAttribute("encoding", firstPage->encoding());
//     el.setAttribute("defaultDTEP", firstPage->dtep());
//     el = Koncrete::DomUtil::elementByPath(projectDom, "/preview");
//     el.setAttribute("previewPrefixURL", secondPage->previewPrefix());
//     el.setAttribute("usePreviewPrefix", secondPage->usePreviewPrefix());

    f.close();
    KTemporaryFile tempFile;
    tempFile.setPrefix(KGlobal::dirs()->resourceDirs("tmp")[0] + "quanta");
    tempFile.setSuffix(".template");
    tempFile.setAutoRemove(false);
    tempFile.open();
    QTextStream str ( &tempFile );
    str << projectDom.toString(2);
    str.flush();
    KUrl dest = baseURL;
    dest.addPath('/' + firstPage->fileName());
    KIO::NetAccess::upload(tempFile.fileName(), dest, KDevelop::Core::self()->uiController()->activeMainWindow());
    KDevelop::Core::self()->projectController()->openProject(dest); 
    QFile::remove(tempFile.fileName());
    if (secondPage->insertGlobalTemplates())
    {
      //FIXME do not hardcode the quanta name
      QString globalDataDir = KGlobal::dirs()->findResourceDir("data", "quanta/global");
      KUrl templateSource = KUrl(globalDataDir + "quanta/templates/");
      KUrl::List templates = ExtFileInfo::listDir(templateSource, "*");
      KUrl templateDest = baseURL;
      templateDest.addPath(secondPage->templates());
      templateDest.adjustPath(KUrl::AddTrailingSlash);
      QuantaNetAccess::dircopy(templates, templateDest, this, false);
    }
    if (secondPage->insertLocalTemplates())
    {
      //FIXME do not hardcode the quanta name
      KUrl templateSource = KUrl(KStandardDirs::locateLocal("data", "quanta/templates/"));
      KUrl::List templates = ExtFileInfo::listDir(templateSource, "*");
      KUrl templateDest = baseURL;
      templateDest.addPath(secondPage->templates());
      templateDest.adjustPath(KUrl::AddTrailingSlash);
      QuantaNetAccess::dircopy(templates, templateDest, this, false);
    }
    if (secondPage->insertMode() == SecondWizardPage::Local)
      m_files = localImportPage->files();
    else
    {
      KUrl::List list = webImportPage->files();
      KUrl::List::ConstIterator end = list.constEnd();
      for (KUrl::List::ConstIterator it = list.constBegin(); it != end; ++it)
      {
        m_files.append(qMakePair(*it, *it));
      }
    }
    QTimer::singleShot(0, this, SLOT(slotDelayedAddFiles()));
  }
  delete wizard;
}

void CreateQuantaProjectPart::slotDelayedAddFiles()
{
  KUrl dest;
  KUrl source;
  QList<QPair<KUrl, KUrl> >::ConstIterator end = m_files.constEnd();
  for (QList<QPair<KUrl, KUrl> >::ConstIterator it = m_files.constBegin(); it != end; ++it)
  {
    source = (*it).first;
    dest = (*it).second;
    if (source.path().endsWith('/'))
    {
      kDebug(24000) << "create: " << dest;
      QuantaNetAccess::mkdir(dest, this, -1, false);
    } else
    {
      kDebug(24000) << "copy: " << source << " to: "<< dest;
      QuantaNetAccess::file_copy(source, dest, -1, true, false, this, false);
    }
  }
}

#include "createquantaprojectpart.moc"
