/**************************************************************************
*   Copyright (C) 2005 by Andras Mantia                                   *
*   amantia@kde.org                                                       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/

#include "quantaprojectpart.h"
#include "browserpopupmenu.h"
#include "extfileinfo.h"
#include "quantanetaccess.h"
#include "quantaprojectmanager.h"
#include "quantaprojectprojectconfig.h"
#include "quantafilemanager.h"

#include <QTimer>
#include <QMenu>
#include <QStringList>
#include <QWhatsThis>

#include <klocale.h>
#include <kaction.h>
#include <kdebug.h>
#include <kdialog.h>
#include <kfiledialog.h>
#include <kgenericfactory.h>
#include <kmainwindow.h>
#include <kmessagebox.h>
#include <kurlrequester.h>
#include <kurlrequesterdlg.h>

//kdevelop includes
#include <domutil.h>
#include <kdevcore.h>
#include <kdevcontext.h>
#include <kdevmainwindow.h>
#include <kdevprojectmodel.h>

typedef KGenericFactory<QuantaProjectPart> QuantaProjectFactory;
K_EXPORT_COMPONENT_FACTORY( libkdevquantaproject, QuantaProjectFactory("kdevquantaproject") );

#define GLOBALDOC_OPTIONS 1
#define PROJECTDOC_OPTIONS 2

QuantaProjectPart::QuantaProjectPart( QObject *parent, const QStringList & /*args*/ )
  : KDevProject(QuantaProjectFactory::instance(), parent)
{
  kDebug( 24000 ) << "QuantaProjectPart loaded" << endl;
  setXMLFile( "kdevquantaproject.rc" );

  m_widget = new QuantaProjectManager( this, 0 );
  m_widget->setWindowTitle( "Project Manager" );
//FIXME  m_widget->setWindowIcon( KIcon( info()->icon() ) );
  
//FIXME  m_widget->setWhatsThis(i18n( "WHAT DOES THIS PART DO?" ) );
  
  KDevCore::mainWindow()->embedSelectViewRight(m_widget, i18n("Project Manager"), i18n("Project Manager"));


  m_projectModel = new KDevProjectModel(this);

  m_browserMenu = 0L;

  setupActions();

//FIXME: New KCM modules need to be created for each config page  m_configProxy = new ConfigWidgetProxy( core() );
  /*
  m_configProxy->createProjectConfigPage( i18n( "Quanta" ), PROJECTDOC_OPTIONS, info() ->icon() );
  connect( m_configProxy, SIGNAL( insertConfigWidget( const KDialog*, QWidget*, unsigned int ) ),
           this, SLOT( insertConfigWidget( const KDialog*, QWidget*, unsigned int ) ) );
  */
  connect( KDevCore::mainWindow(), SIGNAL( contextMenu( QMenu *, const Context * ) ),
           this, SLOT( contextMenu( QMenu *, const Context * ) ) );

  QuantaFileManager *qFileManager = new QuantaFileManager(this);
  setFileManager(qFileManager);

  QTimer::singleShot( 0, this, SLOT( init() ) );
}

QuantaProjectPart::~QuantaProjectPart()
{
  kDebug( 24000 ) << "QuantaProjectPart unloaded" << endl;
  // if you embed a widget, you need to tell the mainwindow when you remove it
  //     if ( m_widget )
  //     {
  //         mainWindow()->removeView( m_widget );
  //     }
  delete m_widget;
//  delete m_configProxy;
}

void QuantaProjectPart::init()
{
  // delayed initialization stuff goes here
  
}

void QuantaProjectPart::setupActions()
{
  // create XMLGUI actions here
  KAction *action;
  action = new KAction(i18n("&Insert Files..."), actionCollection(), "insert_files");
  connect(action, SIGNAL(triggered(bool)), SLOT(slotInsertFiles()));
  action->setToolTip( i18n( "Insert Files" ) );
  action->setWhatsThis( i18n( "Insert Files - Insert new files into the project." ) );

  action = new KAction(i18n("&Insert Folder..."), actionCollection(), "insert_folder");
  connect(action, SIGNAL(triggered(bool)), SLOT(slotInsertFolder()));
  action->setToolTip( i18n( "Insert Folder" ) );
  action->setWhatsThis( i18n( "Insert Folder - Insert a new folder into the project." ) );
}

void QuantaProjectPart::insertConfigWidget( const KDialog *dlg, QWidget *page, unsigned int pageNo )
{
  // create configuraton dialogs here
  switch ( pageNo )
  {
    case PROJECTDOC_OPTIONS:
      {
        QuantaProjectProjectConfig * w = new QuantaProjectProjectConfig( this, page);
        connect( dlg, SIGNAL( okClicked() ), w, SLOT( accept() ) );
        break;
      }
  }
}

void QuantaProjectPart::contextMenu( QMenu *popup, const Context *context )
{
  // put actions into the context menu here
  if ( context->hasType( Context::EditorContext ) )
  {
    // editor context menu
//     const EditorContext * econtext = static_cast<const EditorContext*>( context );

    // use context and plug actions here
//     action->plug( popup );

    // or create menu items on the fly
    // int id = -1;
    // id = popup->insertItem(i18n("Do Something Here"),
    //     this, SLOT(doSomething()) );
    // popup->setWhatsThis(id, i18n("<b>Do something here</b><p>Describe here what does this action do."
  }
  else if ( context->hasType( Context::FileContext ) )
  {
    // file context menu
    const FileContext * fcontext = static_cast<const FileContext*>( context );
    m_fileContextURLs = fcontext->urls();
    bool isInProject = true;
    KUrl::List::ConstIterator end = m_fileContextURLs.constEnd();
    for (KUrl::List::ConstIterator it = m_fileContextURLs.constBegin(); it != end; ++it)
    {
      if (!inProject(*it))
      {
        isInProject = false; //at least one of the files is outside of the project
        break;
      }
    }
    if (!isInProject)
    {
      QStringList menus;
      menus << i18n("&Copy Here");
      //menus << i18n("&Move Here");
      m_browserMenu = new BrowserPopupMenu(m_projectBase, true, menus, 0L);
      m_browserMenu->setTitle(i18n("Insert in Project"));
      popup->addMenu(m_browserMenu);
      connect(m_browserMenu, SIGNAL(triggered(QAction*, const KUrl&)), SLOT(slotTargetFolderSelected(QAction*, const KUrl&)));
    }

    //use context and plug actions here
  }
  else if ( context->hasType( Context::ProjectItemContext ) )
  {
    // project tree context menu
//     const ProjectItemContext * pcontext = static_cast<const ProjectItemContext*>( context );

    // use context and plug actions here
  }
  else if ( context->hasType( Context::CodeItemContext ) )
  {
    // class tree context menu
//     const CodeItemContext * mcontext = static_cast<const CodeItemContext*>( context );

    // use context and plug actions here
  }
/*  else if ( context->hasType( Context::DocumentationContext ) )
  {
    // documentation viewer context menu
//     const DocumentationContext * dcontext = static_cast<const DocumentationContext*>( context );

    // use context and plug actions here
}*/
}

void QuantaProjectPart::closeProject()
{
  // do something when the project is closed
}

void QuantaProjectPart::openProject( const KUrl &dirName, const QString &projectName )
{ 
  m_projectBase = dirName;
  m_projectBase.adjustPath(KUrl::AddTrailingSlash);
  m_projectName = projectName;

  kDebug(24000) << "dirName: " << dirName << " projectName: " << projectName << " baseUrl:" << m_projectBase << endl;
  
  KDevFileManager *manager = fileManager();
  KDevProjectFolderItem *baseItem = static_cast<KDevProjectFolderItem *>(manager->import(m_projectModel, m_projectBase));
  manager->parse(baseItem);
  
  //FIXME: there is no projectDom anymore!!
#warning There is no projectDom!! Change it!  
/*  m_projectDom = KDevApi::self()->projectDom();
  m_projectDomElement = DomUtil::elementByPath(*m_projectDom, "/project");
  if (m_projectDomElement.isNull())
    m_projectDomElement = DomUtil::createElementByPath(*m_projectDom, "/project");
  QDomElement el = DomUtil::elementByPath(*m_projectDom, "/project/items");
  if (el.isNull())
    el = DomUtil::createElementByPath(*m_projectDom, "/project/items");
  QDomNodeList items = el.childNodes();
  for (int i = 0; i < items.count(); i++)
  {
    el = items.item(i).toElement();
    m_files.insert(el.attribute("url"), el);
}*/
  QList<KDevProjectFileItem*> fileList = recurseFiles(baseItem);
  QListIterator<KDevProjectFileItem*> it(fileList);
  while (it.hasNext())
  {
    KUrl url = it.next()->url();
    m_files.insert(url, QDomElement());
/*    if (inProject(url))
      kDebug(24000) << url << " is in project." << endl;*/
  }
//   kDebug(24000) << "Files in the project: " << m_files.keys() << endl;
  kDebug(24000) << "Project base: " << m_projectBase << " name: " << projectName << " baseItem: "<< baseItem->url() << endl;
}

QStringList QuantaProjectPart::allFiles() const
{
  return m_files.keys();
}

QList<KDevProjectFileItem*> QuantaProjectPart::allFiles()
{
  return recurseFiles(fileManager()->top());
}

void QuantaProjectPart::addFiles( const QStringList &fileList )
{
  kDebug(24000) << "Files added to project: " << fileList << endl;
  QDomElement itemsElement = DomUtil::elementByPath(*m_projectDom, "/project/items");
  QDomElement el;
  QStringList::ConstIterator end = fileList.constEnd();
  for (QStringList::ConstIterator it = fileList.constBegin(); it != end; ++it)
  {
    if (! m_files.contains(*it))
    {
      el = m_projectDom->createElement("item");
      el.setAttribute("url", *it);
      itemsElement.appendChild(el);
      m_files.insert(*it, el);
    }
  }
//FIXME  emit addedFilesToProject(fileList);
}


void QuantaProjectPart::removeFiles( const QStringList &fileList )
{
//FIXME  emit removedFilesFromProject(removeItems(fileList));
}

void QuantaProjectPart::removeFile( const QString &fileName )
{
//FIXME  emit removedFilesFromProject(removeItems(QStringList(fileName)));
}


QStringList QuantaProjectPart::removeItems(const QStringList &items)
{
  QStringList removedItems;
  QString fileName;
  QStringList::ConstIterator itemsEnd = items.constEnd();
  for (QStringList::ConstIterator itemIt = items.constBegin(); itemIt != itemsEnd; ++itemIt)
  {
    QMap<QString, QDomElement>::Iterator end = m_files.end();
    QMap<QString, QDomElement>::Iterator deleteIt = end;
    QMap<QString, QDomElement>::Iterator it = m_files.begin();
    while (it != end)
    {
      fileName = it.key();
      if (fileName == *itemIt || fileName.startsWith(*itemIt + "/"))
      {
        kDebug(24000) << "File removed from project: " << fileName << endl;
        QDomElement el = it.value();
        el.parentNode().removeChild(el);
        deleteIt = it;
        removedItems += fileName;
      }
      ++it;
      if (deleteIt != end)
      {
        m_files.erase(deleteIt);
        deleteIt = end;
      }
    }
  }
  return removedItems;
}


void QuantaProjectPart::slotInsertFiles()
{
  KUrl::List urls = KFileDialog::getOpenUrls(m_projectBase, i18n("*"), KDevCore::mainWindow(), i18n("Insert Files in Project"));

  if (!urls.isEmpty())
  {
    if (!m_projectBase.isParentOf(urls.first()))
    {
      KUrlRequesterDlg urlRequesterDlg(m_projectBase.pathOrUrl(), KDevCore::mainWindow());
      urlRequesterDlg.setWindowTitle(i18n("Copy Files to Project"));
      urlRequesterDlg.urlRequester()->setMode(KFile::Directory | KFile::ExistingOnly);
      urlRequesterDlg.exec();
      KUrl destination = urlRequesterDlg.selectedURL();
      if (!destination.isEmpty())
      {
        destination.adjustPath(KUrl::AddTrailingSlash);
        QuantaNetAccess::dircopy(urls, destination, this, false);
      }
    } else
    {
      QStringList filenames;
      KUrl url = KUrl::relativeUrl(m_projectBase, urls.first());
      QStringList sections = url.path(KUrl::AddTrailingSlash).split('/', QString::SkipEmptyParts);
      QString section;
      for (int i = 0 ; i < sections.count() - 1; i++)
      {
        section += sections[i] + "/";
        filenames += section;
      }
      KUrl::List::ConstIterator end = urls.constEnd();
      for (KUrl::List::ConstIterator it = urls.begin(); it != end; ++it)
      {
        url = KUrl::relativeUrl(m_projectBase, *it);
        filenames += url.path();
      }
      addFiles(filenames);
    }
  }
}


void QuantaProjectPart::slotInsertFolder()
{
  KUrl url = KUrl();
  url = KFileDialog::getExistingUrl(m_projectBase, KDevCore::mainWindow(), i18n("Insert Folder in Project"));

  if (!url.isEmpty())
  {
    if (!m_projectBase.isParentOf(url))
    {
      KUrlRequesterDlg urlRequesterDlg(m_projectBase.pathOrUrl(), KDevCore::mainWindow());
      urlRequesterDlg.setWindowTitle(i18n("%1: Copy to Project", url.pathOrUrl()));
      urlRequesterDlg.urlRequester()->setMode(KFile::Directory | KFile::ExistingOnly);
      urlRequesterDlg.exec();
      KUrl destination = urlRequesterDlg.selectedURL();
      if (!destination.isEmpty())
      {
        destination.adjustPath(KUrl::AddTrailingSlash);
        QuantaNetAccess::dircopy(url, destination, this, false);
      }
    } else
    {
      KUrl::List urls = ExtFileInfo::allFilesRelative(url, "*");
      QStringList filenames;
      KUrl u = KUrl::relativeUrl(m_projectBase, url);
      QStringList sections = u.path(KUrl::AddTrailingSlash).split('/', QString::SkipEmptyParts);
      QString section;
      for (int i = 0 ; i < sections.count(); i++)
      {
        section += sections[i] + "/";
        filenames += section;
      }
      KUrl::List::ConstIterator end = urls.constEnd();
      for (KUrl::List::ConstIterator it = urls.begin(); it != end; ++it)
      {
        filenames += url.fileName() + "/" + (*it).path();
      }
      addFiles(filenames);
    }
  }
}

/*Remove if inProject works!!!
bool QuantaProjectPart::isProjectFile(const KUrl &url)
{
  if (! m_projectBase.isParentOf(url))
    return false;
  if (url.path(KUrl::AddTrailingSlash) == m_projectBase.path())
    return true;

  QString u = url.path().remove(0, m_projectBase.path().length());
  kDebug(24000) << "Is part of the project: url=" <<  url << " relativepath= " << u << " result=" << (m_files.contains(u) || m_files.contains(u.append('/'))) << endl;
  return (m_files.contains(u) || (u.right(1) != "/" && m_files.contains(u.append('/'))));
}
*/

void QuantaProjectPart::slotTargetFolderSelected(QAction *action, const KUrl& url)
{
  Q_UNUSED(action);
  KUrl u = url;
  u.adjustPath(KUrl::AddTrailingSlash);
  QuantaNetAccess::dircopy(m_fileContextURLs, u, this, false);
  m_browserMenu->deleteLater();
  m_browserMenu = 0L;
}


QString QuantaProjectPart::relativeProjectFile(const QString &absPath)
{
  QString result;
  if (absPath.startsWith(m_projectBase.path()))
    result = absPath.mid(m_projectBase.path().length());

  return result;
}

QList<KDevProjectFileItem*> QuantaProjectPart::recurseFiles(KDevProjectItem *item)
{
  QList<KDevProjectFileItem*> files;

  if (KDevProjectFolderItem *folder = item->folder())
  {
    QList<KDevProjectFolderItem*> folder_list = folder->folderList();
    for (QList<KDevProjectFolderItem*>::Iterator it = folder_list.begin(); it != folder_list.end(); ++it)
      files += recurseFiles((*it));

    QList<KDevProjectFileItem*> file_list = folder->fileList();
    for (QList<KDevProjectFileItem*>::Iterator it = file_list.begin(); it != file_list.end(); ++it)
      files += recurseFiles((*it));
  }
  else if (KDevProjectTargetItem *target = item->target())
  {
    QList<KDevProjectFileItem*> file_list = target->fileList();
    for (QList<KDevProjectFileItem*>::Iterator it = file_list.begin(); it != file_list.end(); ++it)
      files += recurseFiles((*it));
  }
  else if (KDevProjectFileItem *file = item->file())
  {
    files.append(file);
  }
  return files;
}



#include "quantaprojectpart.moc"

// kate: space-indent on; indent-width 2; mixedindent off;
