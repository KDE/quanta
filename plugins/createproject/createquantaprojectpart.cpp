/***************************************************************************
 *   Copyright (C) 2005 by Andras Mantia   *
 *   amantia@kde.org   *
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
 *   51 Franklin Steet, Fifth Floor, Boston, MA 02110-1301, USA.           *
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
#include "quantaprojectif.h"

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
#include <kiconloader.h>
#include <kgenericfactory.h>
#include <kglobal.h>
#include <kmainwindow.h>
#include <kmessagebox.h>
#include <kstandarddirs.h>
#include <ktempfile.h>

//kdevelop includes
#include <interfaces/kdevcore.h>
#include <interfaces/kdevmainwindow.h>
#include <interfaces/kdevplugincontroller.h>
#include <interfaces/kdevproject.h>
#include <interfaces/domutil.h>


typedef KGenericFactory<CreateQuantaProjectPart> CreateQuantaProjectFactory;
K_EXPORT_COMPONENT_FACTORY( libkdevcreatequantaproject, CreateQuantaProjectFactory("kdevcreatequantaproject") );

#define GLOBALDOC_OPTIONS 1
#define PROJECTDOC_OPTIONS 2

CreateQuantaProjectPart::CreateQuantaProjectPart(QObject *parent, const QStringList &/*args*/)
  : KDevPlugin(CreateQuantaProjectFactory::instance(), parent)
{
  kDebug(24000) << "Quanta create project plugin loaded" << endl;
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
  action = new KAction(KIcon("filenew"), i18n("&New Project..."), actionCollection(), "new_project");
  connect(action, SIGNAL(triggered(bool)), SLOT(slotCreateNewProject()));
  action->setToolTip(i18n("Create new project"));
}


void CreateQuantaProjectPart::slotCreateNewProject()
{
  QuantaCoreIf *qCore = extension<QuantaCoreIf>("KDevelop/Quanta");
  if (!qCore)
  {
    KMessageBox::error(KDevApi::self()->mainWindow()->main(), i18n("<qt>The <b>create new Quanta project</b> plugin requires the <b>Quanta core</b> plugin to be loaded."), i18n("Quanta core not loaded"));
    return;
  }
  Q3Wizard *wizard = new Q3Wizard(KDevApi::self()->mainWindow()->main(), "new", true);
  wizard->setWindowTitle(i18n("New Project Wizard"));
  wizard->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  QStackedWidget *stack = new QStackedWidget(wizard);
  FirstWizardPage *firstPage = new FirstWizardPage(qCore, instance(), wizard);
  SecondWizardPage *secondPage = new SecondWizardPage(stack, instance(), wizard);

  LocalImportWizardPage *localImportPage = new LocalImportWizardPage(instance(), stack);
  WebImportWizardPage *webImportPage = new WebImportWizardPage(instance(), stack);
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
    QDomElement el = DomUtil::createElementByPath(projectDom, "/project/templates");
    el.setAttribute("url", secondPage->templates());
    el = DomUtil::createElementByPath(projectDom, "/project/toolbars");
    el.setAttribute("url", secondPage->toolbars());
    el = DomUtil::elementByPath(projectDom, "/project");
    el.setAttribute("name", firstPage->name());
    el.setAttribute("encoding", firstPage->encoding());
    el.setAttribute("defaultDTEP", firstPage->dtep());
    el = DomUtil::elementByPath(projectDom, "/preview");
    el.setAttribute("previewPrefixURL", secondPage->previewPrefix());
    el.setAttribute("usePreviewPrefix", secondPage->usePreviewPrefix());

    f.close();
    KTempFile tempFile(KGlobal::dirs()->resourceDirs("tmp")[0] + "quanta", ".template");
    tempFile.setAutoDelete(false);
    (*tempFile.textStream()) << projectDom.toString(2);
    tempFile.close();
    KUrl dest = baseURL;
    dest.addPath("/" + firstPage->fileName());
    KIO::NetAccess::upload(tempFile.name(), dest, KDevApi::self()->mainWindow()->main());
    KDevApi::self()->core()->openProject(dest.path()); //FIXME: use the URL when it is supported by the framework
    QFile::remove(tempFile.name());
    if (secondPage->insertGlobalTemplates())
    {
      //FIXME do not hardcode the quanta name
      QString globalDataDir = KGlobal::dirs()->findResourceDir("data", "quanta/global");
      KUrl templateSource = KUrl::fromPathOrUrl(globalDataDir + "quanta/templates/");
      KUrl::List templates = ExtFileInfo::listDir(templateSource, "*");
      KUrl templateDest = baseURL;
      templateDest.addPath(secondPage->templates());
      templateDest.adjustPath(KUrl::AddTrailingSlash);
      QuantaNetAccess::dircopy(templates, templateDest, this, false);
    }
    if (secondPage->insertLocalTemplates())
    {
      //FIXME do not hardcode the quanta name
      KUrl templateSource = KUrl::fromPathOrUrl(KStandardDirs::locateLocal("data", "quanta/templates/"));
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
    if (source.path().endsWith("/"))
    {
      kDebug(24000) << "create: " << dest << endl;
      QuantaNetAccess::mkdir(dest, this, -1, false);
    } else
    {
      kDebug(24000) << "copy: " << source << " to: "<< dest << endl;
      QuantaNetAccess::file_copy(source, dest, -1, true, false, this, false);
    }
  }
}

#include "createquantaprojectpart.moc"
