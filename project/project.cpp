/***************************************************************************
                          project.cpp  -  description
                             -------------------
    begin                : Thu Mar 16 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavsky <pdima@mail.univ.kiev.ua>
                           (C) 2001-2003 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "project.h"

// unix includes
#include <time.h>
#include <unistd.h>

// include QT files
#include <qdict.h>
#include <qdom.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qfileinfo.h>
#include <qtabdialog.h>
#include <qfiledialog.h>
#include <qtextstream.h>
#include <qstringlist.h>
#include <qradiobutton.h>
#include <qwidgetstack.h>
#include <qbuttongroup.h>
#include <qpushbutton.h>
#include <qurloperator.h>
#include <qlineedit.h>
#include <qregexp.h>
#include <qeventloop.h>

// include files for KDE
#include <kdebug.h>
#include <kurl.h>
#include <kfile.h>
#include <kcharsets.h>
#include <kwizard.h>
#include <klocale.h>
#include <kaction.h>
#include <kapplication.h>
#include <ktempfile.h>
#include <kstdaction.h>
#include <kstandarddirs.h>
#include <kfiledialog.h>
#include <kinputdialog.h>
#include <kmessagebox.h>
#include <kprogress.h>
#include <kio/job.h>
#include <kio/netaccess.h>
#include <kurlrequesterdlg.h>
#include <kurlrequester.h>
#include <kprotocolinfo.h>
#include <kpassdlg.h>
#include <kstringhandler.h>
#include <kdeversion.h>
#include <kmainwindow.h>
#include <kparts/componentfactory.h>
#include <kinputdialog.h>
#include <kactioncollection.h>
#include <kiconloader.h>

// application headers
#include "copyto.h"
#include "dtdselectdialog.h"
#include "qextfileinfo.h"
#include "projectnewgeneral.h"
#include "projectnewlocal.h"
#include "projectnewweb.h"
#include "projectnewfinal.h"
#include "projectupload.h"
#include "projectoptions.h"
#include "rescanprj.h"
#include "resource.h"
#include "document.h"
#include "quantadoc.h"
#include "quantaview.h"
#include "doctreeview.h"
#include "quantacommon.h"
#include "quanta.h" //TODO: should get rid of it
#include "dtds.h"
#include "debuggerclient.h"

#include "viewmanager.h"

Project::Project(KMainWindow *parent)
        : QObject()
{
  m_parent = parent;
  config = 0L;
  keepPasswd = true;
  init();
  initActions(parent->actionCollection());
}

Project::~Project()
{
  if (hasProject())
    slotSaveProject();
}

void Project::init()
{
  projectURL  = KURL();
  templateURL = KURL();
  projectName = QString::null;
  m_modified=false;
  m_defaultDTD = qConfig.defaultDocType;
  excludeRx.setPattern(".*~$");
  excludeList.clear();
  excludeList.append("*~");
  usePreviewPrefix=false;
  previewPrefix = KURL();
  m_excludeCvsignore = false;
}


void Project::initActions(KActionCollection *ac)
{
  (void) new KAction( i18n( "&New Project..." ), 0,
                        this, SLOT( slotNewProject() ),
                        ac, "project_new" );

  (void) new KAction( i18n( "&Open Project..." ), BarIcon("folder_new"), 0,
                        this, SLOT( slotOpenProject() ),
                        ac, "project_open" );
  projectRecent =
      KStdAction::openRecent(this, SLOT(slotOpenProject(const KURL&)),
                            ac, "project_open_recent");
  projectRecent->setText(i18n("Open Recent Project"));
  projectRecent->setIcon("folder_new");
  projectRecent->setMaxItems(32);
  projectRecent->setToolTip(i18n("Open / Open Recent Project"));
  connect(projectRecent, SIGNAL(activated()),
            this, SLOT(slotOpenProject()));


  closeprjAction =  new KAction( i18n( "&Close Project" ), SmallIcon("fileclose"), 0,
                        this, SLOT( slotCloseProject() ),
                        ac, "project_close" );


  openPrjViewAction = new KAction( i18n( "Open Project &View..." ), 0,
                        this, SLOT( slotOpenProjectView() ),
                        ac, "project_view_open" );

  savePrjViewAction = new KAction( i18n( "&Save Project View" ), 0,
                        this, SLOT( slotSaveProjectView() ),
                        ac, "project_view_save" );
  saveAsPrjViewAction = new KAction( i18n( "Save Project View &As..." ), 0,
                        this, SLOT( slotSaveAsProjectView() ),
                        ac, "project_view_save_as" );
  deletePrjViewAction = new KAction( i18n( "&Delete Project View..." ), 0,
                        this, SLOT( slotDeleteProjectView() ),
                        ac, "project_view_delete" );



  insertFileAction = new KAction( i18n( "&Insert Files..." ), 0,
                        this, SLOT( slotAddFiles() ),
                        ac, "project_insert_file" );

  insertDirAction = new KAction( i18n( "Inser&t Directory..." ), 0,
                        this, SLOT( slotAddDirectory() ),
                        ac, "project_insert_directory" );

  rescanPrjDirAction = new KAction( i18n( "&Rescan Project Folder..." ), SmallIcon("reload"), 0,
                        this, SLOT( slotRescanPrjDir() ),
                        ac, "project_rescan" );

  uploadProjectAction = new KAction( i18n( "&Upload Project..." ), Key_F8,
                        this, SLOT( slotUpload() ),
                        ac, "project_upload" );

  projectOptionAction = new KAction( i18n( "&Project Properties..." ), SHIFT + Key_F7,
                        this, SLOT( slotOptions() ),
                        ac, "project_options" );

  saveAsProjectTemplateAction =
            new KAction( i18n( "Save as Project Template..." ), 0,
                        m_parent, SLOT( slotFileSaveAsProjectTemplate() ),
                        ac, "save_project_template" );

  saveSelectionAsProjectTemplateAction =
                        new KAction( i18n( "Save Selection to Project Template File..." ), 0,
                                     m_parent, SLOT( slotFileSaveSelectionAsProjectTemplate() ),
                                     ac, "save_selection_project_template" );
  adjustActions();
}

void Project::adjustActions()
{
  bool projectExists = hasProject();
  closeprjAction->setEnabled(projectExists);
  openPrjViewAction->setEnabled(projectExists);
  savePrjViewAction->setEnabled(projectExists);
  saveAsPrjViewAction->setEnabled(projectExists);
  deletePrjViewAction->setEnabled(projectExists);

  insertFileAction->setEnabled(projectExists);
  insertDirAction->setEnabled(projectExists);
  rescanPrjDirAction->setEnabled(projectExists);
  uploadProjectAction->setEnabled(projectExists);
  projectOptionAction->setEnabled(projectExists);
  saveAsProjectTemplateAction->setEnabled(projectExists);
  saveSelectionAsProjectTemplateAction->setEnabled(projectExists);
}

/** Retrieve the list of urls which are in the project. If check is true,
for local files it verifies if it exists or not, and adds only the exisiting
files.*/
KURL::List Project::fileNameList(bool check)
{
  KURL::List result;
  ProjectUrlList list;
  //cout << dom.toString() << "\n";
  QDomNodeList nl = dom.elementsByTagName("item");
  uint nlCount = nl.count();
  for ( uint i=0; i < nlCount; i++ )
  {
    QDomElement el = nl.item(i).cloneNode().toElement();
    ProjectURL url = baseURL;
    QuantaCommon::setUrl(url, el.attribute("url"));
    if (!excludeRx.exactMatch(url.path()))
    {
      url.fileDesc = el.attribute("desc");
      if (url.isLocalFile() && check)
      {
        QFileInfo fi(baseURL.path(1)+url.path());
        if (fi.exists()) {
          list.append(url);
          result.append(url);
        }
      } else
      {
  //      entry.toolTip = el.attribute("tooltip", url.prettyURL());
        list.append(url);
        result.append(url);
      }
    }
  }
  m_projectFiles = list;
  return result;
}

void Project::insertFile(const KURL& nameURL, bool repaint )
{
  if (excludeRx.exactMatch(nameURL.path()))
      return;
  KURL relNameURL = QExtFileInfo::toRelative( nameURL, baseURL);

  if ( relNameURL.path().startsWith("/") || relNameURL.path().startsWith(".")  )
  {
    KURLRequesterDlg *urlRequesterDlg = new KURLRequesterDlg( baseURL.prettyURL(), m_parent, "");
    urlRequesterDlg->setCaption(i18n("%1: Copy to Project").arg(nameURL.prettyURL(0, KURL::StripFileProtocol)));
    urlRequesterDlg->urlRequester()->setMode( KFile::Directory | KFile::ExistingOnly);
    urlRequesterDlg->exec();
    KURL destination = urlRequesterDlg->selectedURL();
    delete urlRequesterDlg;
    if ( !destination.isEmpty() )
    {
      CopyTo *dlg = new CopyTo(baseURL);
      connect(dlg, SIGNAL(deleteDialog(CopyTo*)),
                   SLOT(slotDeleteCopytoDlg(CopyTo*)));
      KURL tmpURL = dlg->copy( nameURL, destination );
      relNameURL = QExtFileInfo::toRelative( tmpURL, baseURL);
    }
    else  // Copy canceled, addition aborted
    {
      return;
    }
  }

  QDomElement  el;
  QDomNodeList nl = dom.elementsByTagName("item");

  if (m_projectFiles.contains(relNameURL))
    return;
  KURL url = relNameURL;
  url.setPath(relNameURL.directory(false));
  while (!url.path().isEmpty())
  {
    if (!m_projectFiles.contains(url))
    {
      el = dom.createElement("item");
      el.setAttribute("url", QuantaCommon::qUrl(url));
      dom.firstChild().firstChild().appendChild( el );
      m_projectFiles.append(url);
    }
    url.setPath(url.directory(false));
  }

  el = dom.createElement("item");
  el.setAttribute("url", QuantaCommon::qUrl(relNameURL) );

  dom.firstChild().firstChild().appendChild( el );
  m_projectFiles.append(relNameURL);
  m_modified = true;

  if ( repaint )
  {
    emit reloadTree( m_projectFiles, false);
    emit newStatus();
  }
}


/** insert files from dir recursive */
void Project::insertFiles(const KURL& pathURL, const QString& mask )
{
  KURL::List list;

  list.append(pathURL);
  list += QExtFileInfo::allFiles( pathURL, mask );
  insertFiles(list);
}

/** insert files */
void Project::insertFiles( KURL::List files )
{
  QDomElement  el;
  QDomNodeList nl = dom.elementsByTagName("item");
  emit statusMsg( i18n("Adding files to the project...") );
  progressBar->setTotalSteps(2 * files.count() - 2);
  progressBar->setValue(0);
  progressBar->setTextEnabled(true);

  KURL::List::Iterator it;
  for ( it = files.begin(); it != files.end(); ++it )
  {
    *it = QExtFileInfo::toRelative( *it, baseURL);
    if (m_projectFiles.contains(*it))
    {
        it = files.erase(it);
        --it;
    }
    progressBar->advance(1);
  }
  for ( it = files.begin(); it != files.end(); ++it )
  {
    if (! (*it).isEmpty())
    {
      KURL url = *it;
      url.setPath(url.directory(false));
      while (!url.path().isEmpty())
      {
        if (!m_projectFiles.contains(url))
        {
          el = dom.createElement("item");
          el.setAttribute("url", QuantaCommon::qUrl(url));
          dom.firstChild().firstChild().appendChild(el);
          m_projectFiles.append(url);
          m_modified = true;
        }
        url.setPath(url.directory(false));
      }
      el = dom.createElement("item");
      url = *it;
      if (!excludeRx.exactMatch(url.path()))
      {
        el.setAttribute("url", QuantaCommon::qUrl(url));
        dom.firstChild().firstChild().appendChild(el);
        m_projectFiles.append(url);
        m_modified = true;
      }
     }
     progressBar->advance(1);
  }
  progressBar->setTotalSteps(1);
  progressBar->setValue(0);
  progressBar->setTextEnabled(false);

  emit statusMsg(QString::null);
  emit newStatus();
}

bool Project::createEmptyDom()
{
  QString str;
  QTextStream stream( &str, IO_WriteOnly );
  stream.setEncoding(QTextStream::UnicodeUTF8);

  stream << "<!DOCTYPE webproject ><webproject>" << endl;
  stream << "\t<project name=\"" << projectName << "\">" << endl;
  stream << "\t\t<upload />" << endl;
  stream << "\t</project>" << endl;
  stream << "</webproject>" << endl;

  bool result = true;

  if (! projectURL.isLocalFile())
  {
    KTempFile *tempFile = new KTempFile(tmpDir);
    tempFile->setAutoDelete(true);
    tempFile->textStream()->setEncoding(QTextStream::UnicodeUTF8);
    *(tempFile->textStream()) << str;
    tempFile->close();
    result = QExtFileInfo::createDir(baseURL);
    if (result) result = KIO::NetAccess::upload(tempFile->name(), projectURL, m_parent);
    delete tempFile;
  } else
  {
    QFile f(projectURL.path());
    if (f.open( IO_WriteOnly ))
    {
      QTextStream fstream(&f);
      fstream.setEncoding(QTextStream::UnicodeUTF8);
      fstream << str;
    } else
    {
      result = false;
    }
    f.close();
  }

  if (!result)
  {
    emit hideSplash();
    KMessageBox::sorry(m_parent, i18n("<qt>Cannot open file <b>%1</b> for writing.</qt>").arg(projectURL.prettyURL(0, KURL::StripFileProtocol)));
    return false;
  }

  //slotLoadProject( projectURL );
  dom.setContent( str);
  m_projectFiles.clear();
  return true;
}

void Project::readConfig (KConfig *config)
{
  this->config = config;

  config->setGroup  ("Projects");
  projectRecent->loadEntries(config, "RecentProjects");
}

void Project::readLastConfig(KConfig *c)
{
  KConfig *config;
  if (c == 0)
    config = this->config;
  else
    config = c;

  config->setGroup("Projects");
  QString urlPath = config->readPathEntry("Last Project");

  KURL url;
  QuantaCommon::setUrl(url, urlPath);

  slotCloseProject();
  if ( !urlPath.isEmpty() && url.isValid())
  {
   slotLoadProject ( url );
  }
}

void Project::writeConfig(KConfig *config)
{
  config->setGroup  ("Projects");
  config->writePathEntry("Last Project", projectURL.url());
  config->deleteGroup("RecentProjects");
  projectRecent->saveEntries(config, "RecentProjects");

//  slotSaveProject();
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
// SLOTS implementation
//

/** open project file */
void Project::slotOpenProject()
{
  KURL url = KFileDialog::getOpenURL( QString::null,
                       "*.wpj *.webprj"+i18n("|Project Files\n*|All Files"), m_parent);

  if( !url.isEmpty() )
  {
    slotLoadProject ( url );

    projectRecent->addURL( url );
  }
  emit newStatus();
}

void Project::slotOpenProject(const KURL &url)
{
  if ( !url.isEmpty() )
  {
    if ( !QExtFileInfo::exists(url) )
    {
      emit hideSplash();
      if (KMessageBox::questionYesNo(m_parent,
           i18n("<qt>The file <b>%1</b> does not exist.<br> Do you want to remove it from the list?</qt>").arg(url.prettyURL(0, KURL::StripFileProtocol)) )
           == KMessageBox::Yes)
      {
        projectRecent->removeURL(url);
      }
    } else
    {
      slotLoadProject ( url );
      projectRecent->addURL( url );
    }
  }
}

/** save project file */
bool Project::slotSaveProject()
{
  int result = true;
  if (hasProject())
  {
    // remove old opened files
    QDomElement  el;
    QDomNodeList nl = dom.firstChild().firstChild().childNodes();

    for ( unsigned int i=0; i<nl.count(); i++ )
    {
      el = nl.item(i).toElement();
      if ( el.nodeName() == "openfile" )
      {
        el.parentNode().removeChild( el );
        i--;
      }
    }

    KTempFile *tmpFile = new KTempFile(tmpDir);
    tmpFile->textStream()->setEncoding(QTextStream::UnicodeUTF8);
    tmpFile->setAutoDelete(true);
    dom.save(*(tmpFile->textStream()), 2);
    tmpFile->close();
    if (KIO::NetAccess::upload( tmpFile->name(), projectURL, m_parent))
    {
        emit statusMsg(i18n( "Wrote project %1..." ).arg( projectURL.prettyURL()));
    }
    else
    {
      emit statusMsg(QString::null );
      KMessageBox::error(m_parent, KIO::NetAccess::lastErrorString() );
      result = false;
    }

    delete tmpFile;
    m_modified = false;
  } else
  {
    result = false;
  }
 if (result)
      projectRecent->addURL( projectURL );
 return result;
}

/** close project and edited files */
void Project::slotCloseProject()
{
  if (!hasProject()) return;
  //fix: add save/no for remote
  if (m_modified)
      slotSaveProject();

  dom.clear();

  init();
  currentProjectView = QString::null;
  m_projectFiles.clear();
  emit closeFiles();

  emit newProjectLoaded(projectName, baseURL, templateURL, m_documentRootURL);
  emit reloadTree( m_projectFiles, true);
  adjustActions();
  emit newStatus();
}

/** load project from file: name */
void Project::slotLoadProject(const KURL &a_url)
{
  if (hasProject()) slotCloseProject();

  KURL url = a_url;
  projectURL = KURL();
  projectName = QString::null;
  currentProjectView = QString::null;

  if (!url.isValid())
  {
      emit hideSplash();
      KMessageBox::sorry(m_parent, i18n("<qt>Malformed URL: <b>%1</b></qt>").arg(url.prettyURL()));
  } else
  {
    QString tmpName = QString::null;
    if (KIO::NetAccess::download(url, tmpName, m_parent))
    {
      projectURL = url;
      QFile f(tmpName);
      if (f.open(IO_ReadOnly))
      {
        baseURL = url;
        baseURL.setPath(url.directory(true, true));
        if (baseURL.isLocalFile())
        {
          QDir dir(baseURL.path());
          baseURL.setPath(dir.canonicalPath());
          baseURL.adjustPath(-1);
        }
        dom.setContent( &f );
        f.close();
        loadProjectXML();
        openCurrentView();
        adjustActions();
      } else
      {
        emit hideSplash();
        KMessageBox::error(m_parent, i18n("<qt>Cannot open the file <b>%1</b> for reading.</qt>").arg(tmpName));
      }
      KIO::NetAccess::removeTempFile( tmpName);
    } else
    {
      emit hideSplash();
      KMessageBox::error(m_parent, i18n("<qt>Cannot access the project file <b>%1</b>.</qt>").arg(url.prettyURL(0, KURL::StripFileProtocol)));
    }
  }
}

void Project::loadProjectXML()
{
  QDomNode    no;
  QDomElement el;
  KURL url;
  QDomNode projectNode = dom.firstChild().firstChild();
  projectName = projectNode.toElement().attribute("name");

  if ( projectNode.isNull() || projectName.isEmpty() )
  {
    emit hideSplash();
    KMessageBox::sorry(m_parent, i18n("Invalid project file.") );
    return;
  }

  m_modified = false;
  QString tmpString = projectNode.toElement().attribute("previewPrefix");
  if ( !tmpString.isEmpty())
  {
    QuantaCommon::setUrl(previewPrefix, tmpString);
    previewPrefix.adjustPath(-1);
    if (tmpString != previewPrefix.url()) //compatibility
    {
      projectNode.toElement().setAttribute("previewPrefix",previewPrefix.url());
      m_modified = true;
    }
  }

  usePreviewPrefix = ( projectNode.toElement().attribute("usePreviewPrefix") == "1");
  m_defaultEncoding = projectNode.toElement().attribute("encoding");
  if (m_defaultEncoding.isEmpty())
  {
    m_defaultEncoding = qConfig.defaultEncoding;
  }
  m_documentRootURL = baseURL;
  m_documentRootURL.setPath("");
  tmpString = projectNode.toElement().attribute("documentRoot");
  if (!tmpString.isEmpty())
  {
    QuantaCommon::setUrl(m_documentRootURL, tmpString);
  }
  no = projectNode.namedItem("author");
  author = no.firstChild().nodeValue();
  no = projectNode.namedItem("email");
  email = no.firstChild().nodeValue();
  no = projectNode.namedItem("defaultDTD");
  m_defaultDTD = no.firstChild().nodeValue();
  if (m_defaultDTD.isEmpty()) m_defaultDTD = qConfig.defaultDocType;

  no = projectNode.namedItem("autoload");
  currentProjectView = no.toElement().attribute("projectview");

  // Debugger
  no = projectNode.namedItem("debuggerclient");
  debuggerClient = no.firstChild().nodeValue();

  no = projectNode.namedItem("templates");
  tmpString = no.firstChild().nodeValue();
  templateURL = baseURL;
  if(no.isNull()) // compatability
  {
    templateURL.setPath("templates/");
    m_modified = true;
  }
  else
  {
    QuantaCommon::setUrl(templateURL, tmpString);
  }
  if (tmpString != QuantaCommon::qUrl(templateURL) )
  {
    el = no.toElement();
    url = QExtFileInfo::toRelative(templateURL, baseURL);
    if(el.isNull())
    {
      el = dom.createElement("templates");
      dom.firstChild().firstChild().appendChild(el);
      el.appendChild(dom.createTextNode(QuantaCommon::qUrl(url)));
    }
    else
    {
      el.firstChild().setNodeValue(QuantaCommon::qUrl(url));
    }
  }
  templateURL = QExtFileInfo::toAbsolute(templateURL, baseURL);

  no = projectNode.namedItem("toolbars");
  toolbarURL = baseURL;
  if(no.isNull()) // compatability
  {
    toolbarURL.setPath(baseURL.path(1) + "toolbars/");
    m_modified = true;
  }
  else
  {
    QuantaCommon::setUrl(toolbarURL,no.firstChild().nodeValue());
  }
  if (tmpString != QuantaCommon::qUrl(toolbarURL))
  {
    el = no.toElement();
    url = QExtFileInfo::toRelative(toolbarURL, baseURL);
    if(el.isNull())
    {
      el = dom.createElement("toolbars");
      dom.firstChild().firstChild().appendChild(el);
      el.appendChild(dom.createTextNode(QuantaCommon::qUrl(url)));
    }
    else
    {
      el.firstChild().setNodeValue(QuantaCommon::qUrl(url));
    }
  }
  toolbarURL = QExtFileInfo::toAbsolute(toolbarURL, baseURL);

  no = projectNode.namedItem("exclude");
  m_excludeCvsignore = (no.toElement().attribute("cvsignore", "false") == "true");
  QString excludeStr = no.firstChild().nodeValue();
  QString regExpStr = "";
  excludeList = QStringList::split(';', excludeStr);
  for (uint i = 0; i < excludeList.count(); i++)
  {
    excludeStr = excludeList[i].stripWhiteSpace();
    if (!excludeStr.endsWith("*"))
      excludeStr = excludeStr + "/*|"+ excludeStr + "$";
    if (!excludeStr.startsWith("*"))
      excludeStr.prepend("^");
    excludeStr.replace(".","\\.");
    excludeStr.replace("*",".*");
    excludeStr.replace("?",".");
    regExpStr.append(excludeStr);
    if (i+1 < excludeList.count())
      regExpStr.append("|");
  }
  QDomNodeList nl = dom.firstChild().firstChild().childNodes();
  if (m_excludeCvsignore && projectURL.isLocalFile())
  {
    QStringList cvsIgnoreList;
    uint nlCount = nl.count();
    for ( uint i = 0; i < nlCount; i++ )
    {
      el = nl.item(i).toElement();
      tmpString = el.attribute("url");
      if (!tmpString.endsWith("/")) continue;
      cvsIgnoreList.append(tmpString);
    }
    cvsIgnoreList.append("");
    for (QStringList::ConstIterator it = cvsIgnoreList.constBegin(); it != cvsIgnoreList.constEnd(); ++it)
    {
      tmpString = *it;
      QString rxStr;
      KURL cvsIgnoreURL;
      cvsIgnoreURL.setPath(baseURL.path(1) + tmpString + ".cvsignore");
      QFile f(cvsIgnoreURL.path());
      if (f.open(IO_ReadOnly))
      {
        QTextStream stream(&f);
        QString line;
        while (!stream.atEnd())
        {
            line = stream.readLine().stripWhiteSpace();
            line.prepend(tmpString);
            if (!line.endsWith("*"))
              line = line + "/*|"+ line + "$";
            if (!line.startsWith("*"))
              line.prepend("^");
            line.replace(".","\\.");
            line.replace("*",".*");
            line.replace("?",".");
            rxStr += line + "|";
        }
        regExpStr.prepend(rxStr);
        f.close();
      }
    }
  }
  excludeRx.setPattern(regExpStr);

  emit statusMsg( i18n("Reading the project file...") );
  progressBar->setTotalSteps(nl.count() - 1);
  progressBar->setValue(0);
  progressBar->setTextEnabled(true);
  QString relTemplateUrlStr = QExtFileInfo::toRelative(templateURL, baseURL).url();
  QString docRootUrlStr = m_documentRootURL.url();
  QString path;
  uint nlCount = nl.count();
  for ( uint i = 0; i < nlCount; i++ )
  {
    el = nl.item(i).toElement();
    tmpString = el.attribute("url");
    if (!tmpString.isEmpty())
    {
      QuantaCommon::setUrl(url,tmpString);
      //Compatibility conversion
      if (tmpString != QuantaCommon::qUrl(url))
      {
        el.setAttribute("url", QuantaCommon::qUrl(url));
        m_modified = true;
      }
    }
    path = url.path();
    if ( el.nodeName() == "item" )
    {
      //remove non-existent local files
      if (!excludeRx.exactMatch(path))
      {
        int defaultUploadStatus = 0;
        if (url.url().startsWith(docRootUrlStr) || url.url().startsWith(relTemplateUrlStr) )
          defaultUploadStatus = 1;
        int uploadStatus = el.attribute("uploadstatus", "-1").toInt();
        if (uploadStatus == -1)
          el.setAttribute("uploadstatus", defaultUploadStatus);
        ProjectURL file(url, el.attribute("desc"), el.attribute("uploadstatus").toInt());
        if ( url.isLocalFile() )
        {
          QFileInfo fi( baseURL.path(1)+path);
          if ( !fi.exists() )
          {
            el.parentNode().removeChild( el );
            i--;
          } else
          {
            if (path.startsWith("doc/") && path.endsWith("/index.html"))
            {
               quantaApp->dTab->addProjectDoc(
               QExtFileInfo::toAbsolute(url, baseURL));
             }
            m_projectFiles.append(file);
          }
        } else
        {
          if (path.startsWith("doc/") && path.endsWith("/index.html"))
               quantaApp->dTab->addProjectDoc(
               QExtFileInfo::toAbsolute(url, baseURL));
          m_projectFiles.append(file);
        }
      } else
      {
        el.parentNode().removeChild(el);
        i--;
      }
    }
    progressBar->advance(1);
  }
  progressBar->setTotalSteps(1);
  progressBar->setValue(0);
  progressBar->setTextEnabled(false);

  emit statusMsg(QString::null);

  emit newProjectLoaded(projectName, baseURL, templateURL, m_documentRootURL);
  emit reloadTree(m_projectFiles, true);

  emit showTree();
  emit newStatus();
}

// slot for insert file
void Project::slotInsertFile( const KURL& url )
{
  insertFile( url, true );
}

/** dialog for add files */
void Project::slotAddFiles()
{
  KURL::List list = KFileDialog::getOpenURLs(
    baseURL.url(),  i18n("*"), m_parent, i18n("Insert Files in Project"));

  if ( !list.isEmpty() )
  {
    KURL firstURL = list.first();
    firstURL = QExtFileInfo::toRelative( firstURL, baseURL );

    if ( firstURL.path().startsWith("/") || firstURL.path().startsWith("."))
    {
      KURLRequesterDlg *urlRequesterDlg = new KURLRequesterDlg( baseURL.prettyURL(), m_parent, "");
      urlRequesterDlg->setCaption(i18n("Files: Copy to Project"));
      urlRequesterDlg->urlRequester()->setMode( KFile::Directory | KFile::ExistingOnly);
      urlRequesterDlg->exec();
      KURL destination = urlRequesterDlg->selectedURL();
      delete urlRequesterDlg;

      if ( !destination.isEmpty())
      {
        CopyTo *dlg = new CopyTo( baseURL);
        connect(dlg, SIGNAL(deleteDialog(CopyTo*)),
                     SLOT  (slotDeleteCopytoDlg(CopyTo*)));
        connect(dlg, SIGNAL(addFilesToProject(const KURL::List&)),
                     SLOT  (slotInsertFilesAfterCopying(const KURL::List&)));
        list = dlg->copy( list, destination );
        return;
      }
      else {
        return;
      }
    }

   insertFiles( list );
   //Take care also of the selected dirs
   KURL dirURL;
   for (uint i = 0; i < list.count(); i++)
   {
     dirURL = list[i];
     if (dirURL.path().endsWith("/"))
     {
       insertFiles( dirURL, "*" );
     }
   }

    emit reloadTree( m_projectFiles, false);
  }
}

void Project::slotAddDirectory()
{
 KURL url = KURL();
 url = KFileDialog::getExistingURL(baseURL.prettyURL(), m_parent,
                i18n("Insert Folder in Project"));
 slotAddDirectory(url);
}

void Project::slotAddDirectory(const KURL& p_dirURL, bool showDlg)
{
  KURL dirURL = p_dirURL;
  if ( hasProject() && !dirURL.isEmpty() )
  {

    dirURL.adjustPath(1);
    KURL relURL = QExtFileInfo::toRelative(dirURL, baseURL);

    if ( relURL.path().startsWith("/") || relURL.path().startsWith("."))
    {
      KURL destination = baseURL;
      if (showDlg)
      {
        KURLRequesterDlg *urlRequesterDlg = new KURLRequesterDlg( baseURL.prettyURL(), m_parent, "");
        urlRequesterDlg->setCaption(i18n("%1: Copy to Project").arg(dirURL.prettyURL(0, KURL::StripFileProtocol)));
        urlRequesterDlg->urlRequester()->setMode( KFile::Directory | KFile::ExistingOnly);
        urlRequesterDlg->exec();
        destination = urlRequesterDlg->selectedURL();
        delete urlRequesterDlg;
      }
      if ( (showDlg == false) ||
            (!destination.isEmpty()) )
      {
        CopyTo *dlg = new CopyTo(baseURL);
        connect(dlg, SIGNAL(addFilesToProject(const KURL::List&)),
                     SLOT  (slotInsertFilesAfterCopying(const KURL::List&)));
        connect(dlg, SIGNAL(deleteDialog(CopyTo *)),
                     SLOT  (slotDeleteCopytoDlg(CopyTo *)));
        //if ( rdir.right(1) == "/" ) rdir.remove( rdir.length()-1,1);
        dirURL = dlg->copy(dirURL, destination);
        return;
      }
      else
      {
        return;
      }
    }
    insertFiles( dirURL, "*" );
  //And again, insert now directly the directory name into the project.
  //It's important if rdir doesn't contain any files or subdirectories.
    insertFiles(dirURL);
    emit reloadTree( m_projectFiles, false );
  }
}

void Project::slotInsertFilesAfterCopying(const KURL::List& a_urlList)
{
  KURL::List::ConstIterator it;
  KURL url;
  for (it = a_urlList.begin(); it != a_urlList.end(); ++it)
  {
    url = *it;
    //url.adjustPath(1);
    insertFiles( url, "*" );
  }
  emit reloadTree( m_projectFiles, false );
}

void Project::slotDeleteCopytoDlg(CopyTo *dlg)
{
//The CopyTo dlg is deleted only here!!
  delete dlg;
}

//rename the elements in the project dom tree
void Project::slotRenamed(const KURL& oldURL, const KURL& newURL)
{
  //rename the elements in the project dom tree
  QDomElement el;
  QDomNodeList nl = dom.elementsByTagName("item");
  QString tmpString;
  QString oldString;
  QString oldStr = QuantaCommon::qUrl(QExtFileInfo::toRelative(oldURL, baseURL));
  QString newStr = QuantaCommon::qUrl(QExtFileInfo::toRelative(newURL, baseURL));
  for (uint i = 0; i < nl.count(); i++ )
  {
    el = nl.item(i).toElement();
    tmpString = el.attribute("url");
    if (tmpString == newStr)
    {
      el.parentNode().removeChild( el );
      break;
    }
  }
  ProjectUrlList::Iterator it;
  it = m_projectFiles.find(newURL);
  if (it != m_projectFiles.end())
      m_projectFiles.erase(it);

  uint nlCount = nl.count();
  emit statusMsg(i18n("Renaming files..."));
  progressBar->setTotalSteps(nlCount - m_projectFiles.count() - 2);
  progressBar->setValue(0);
  progressBar->setTextEnabled(true);
  bool checkStartsWith = oldStr.endsWith("/");
  for (uint i = 0; i < nlCount; i++ )
  {
    el = nl.item(i).toElement();
    tmpString = el.attribute("url");
    if (tmpString == oldStr ||
        (checkStartsWith && tmpString.startsWith(oldStr)))
    {
      oldString = tmpString;
      tmpString = tmpString.replace(oldStr, newStr);
      if (oldString != tmpString )
      {
        el.setAttribute("url",tmpString);
        el.setAttribute("upload_time", "");
      }
    }
    progressBar->advance(1);
  }
  for (ProjectUrlList::Iterator it = m_projectFiles.begin(); it != m_projectFiles.end(); ++it)
  {
    tmpString = (*it).path();
    if (tmpString == oldStr ||
        (checkStartsWith && tmpString.startsWith(oldStr)))
    {
      tmpString = tmpString.replace(oldStr, newStr);
      (*it).setPath(tmpString);
    }
    progressBar->advance(1);
  }
  progressBar->setTotalSteps(1);
  progressBar->setValue(0);
  progressBar->setTextEnabled(false);

  emit statusMsg(QString::null);
  m_modified = true;

  emit reloadTree(m_projectFiles, false);
  emit newStatus();
}


void Project::slotRemove(const KURL& urlToRemove)
{
  bool isFolder = urlToRemove.fileName(false).isEmpty();

  QDomElement el;
  QDomNodeList nl = dom.elementsByTagName("item");
  KURL url = QExtFileInfo::toRelative(urlToRemove, baseURL);
  QString tmpString;
  QString urlStr = QuantaCommon::qUrl(url);
  ProjectUrlList::Iterator it;
  for ( uint i = 0; i < nl.count(); i++ )
  {
    el = nl.item(i).toElement();

    tmpString = el.attribute("url");
    if ( ( !isFolder && tmpString == urlStr) ||
         ( isFolder && tmpString.startsWith(urlStr))
       )
    {
      el.parentNode().removeChild( el );
      url = baseURL;
      QuantaCommon::setUrl(url, tmpString);
      it = m_projectFiles.find(url);
      m_projectFiles.erase(it);
      m_modified = true;
      if (isFolder)
      {
       i--;
      }
      else break;
    }
  }
  emit closeFile(urlToRemove);
  emit reloadTree( m_projectFiles, false );
  emit newStatus();
}


/** create new project */
void Project::slotNewProject()
{
  slotCloseProject();
  wiz = new QWizard(m_parent, "new", true);
  wiz->setCaption(i18n("New Project Wizard"));
  wiz->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

  png = new ProjectNewGeneral( wiz );

  stack = new QWidgetStack( wiz );

  pnl = new ProjectNewLocal( stack );
  pnw = new ProjectNewWeb  ( stack );
  pnf = new ProjectNewFinal( wiz );

  stack->addWidget( pnl, 0);
  stack->addWidget( pnw, 1 );

  wiz->addPage( png,   i18n("<b>General Project Settings</b>"));
  wiz->addPage( stack, i18n("<b>Insert Files in Project</b>"));
  wiz->addPage( pnf,   i18n("<b>More Project Settings</b>"));

  wiz->setNextEnabled  ( png,   false );
  wiz->setBackEnabled  ( stack, true  );
  wiz->setNextEnabled  ( stack, true  );
  wiz->setNextEnabled  ( pnf,   false );
  wiz->setFinishEnabled( pnf,   true  );

  connect( png, SIGNAL(enableNextButton(QWidget *,bool)),
           wiz, SLOT(setNextEnabled(QWidget*,bool)));
  connect( png, SIGNAL(setBaseURL(const KURL&)),
           pnl, SLOT(  setBaseURL(const KURL&)));
  connect( png, SIGNAL(setBaseURL(const KURL&)),
           pnw, SLOT(  setBaseURL(const KURL&)));
  connect( this,SIGNAL(setLocalFiles(bool)),
           pnl, SLOT(slotSetFiles(bool)));

  connect( wiz, SIGNAL(selected(const QString &)),
          this, SLOT  (slotSelectProjectType(const QString &)));

  connect( pnw, SIGNAL(enableMessagesWidget()),
          this, SIGNAL  (enableMessageWidget()));
  connect( pnw, SIGNAL(messages(const QString&)),
          this, SLOT  (slotGetMessages(const QString&)));
  connect( pnw, SIGNAL(enableNextButton(QWidget *,bool)),
           wiz, SLOT(setNextEnabled(QWidget*,bool)));
  connect( pnw, SIGNAL(enableNextButton(QWidget *,bool)),
           wiz, SLOT(setBackEnabled(QWidget*,bool)));

  QStringList lst = DTDs::ref()->nickNameList(true);
  pnf->dtdCombo->insertStringList(lst);
  QString defaultDTDName = DTDs::ref()->getDTDNickNameFromName(qConfig.defaultDocType.lower());
  int pos = lst.findIndex(defaultDTDName);
  if (pos >= 0)
    pnf->dtdCombo->setCurrentItem(pos);

  QStringList availableEncodingNames(KGlobal::charsets()->availableEncodingNames());
  pnf->encodingCombo->insertStringList( availableEncodingNames );
  QStringList::ConstIterator iter;
  int iIndex = -1;
  for (iter = availableEncodingNames.begin(); iter != availableEncodingNames.end(); ++iter)
  {
     ++iIndex;
     if ((*iter).lower() == qConfig.defaultEncoding.lower())
     {
       pnf->encodingCombo->setCurrentItem(iIndex);
       break;
     }
  }

  png->linePrjName->setFocus();
  if ( wiz->exec() )
    slotAcceptCreateProject();

  delete wiz;

  adjustActions();
  emit newStatus();
}

void Project::slotSelectProjectType(const QString &title)
{
  if ( png->radioLocal->isChecked() ) stack->raiseWidget( 0 );
  if ( png->radioWeb  ->isChecked() ) stack->raiseWidget( 1 );
  if ( title == i18n("Insert files in project...") )
    emit setLocalFiles( pnl->checkInsert->isChecked() );
}

void Project::slotAcceptCreateProject()
{
  bool errorOccured = false;

  projectName = png->linePrjName->text();
  QString basePath = png->linePrjDir ->text();

  KURL oldBaseURL = baseURL;
  baseURL = KURL::fromPathOrURL(basePath);
  /*
     it is important to set the fields only if there is some input
     otherwise you set them to an empty string and the treeview will
     not recognize it as parent url because:
                 QString::Null != ""
  */
  if (png->lineHost->text() != "")
    baseURL.setHost(png->lineHost->text());
  if (png->lineUser->text() != "")
    baseURL.setUser(png->lineUser->text());
  if (png->linePasswd->text() != "")
    baseURL.setPass(png->linePasswd->text());
  if (png->linePort->text() != "")
    baseURL.setPort(png->linePort->text().toInt());
  if (png->comboProtocol->currentText() != "")
    baseURL.setProtocol(png->comboProtocol->currentText());
  if (baseURL.protocol() == i18n("Local")) baseURL.setProtocol("file");
  baseURL.adjustPath(1);
  if (!baseURL.path().startsWith("/")) baseURL.setPath("/"+baseURL.path());
  if (!QExtFileInfo::createDir( baseURL ))
  {
    QuantaCommon::dirCreationError(m_parent, baseURL);
    baseURL = oldBaseURL;
  } else
  {
    projectURL = baseURL;
    projectURL.setPath(baseURL.path(1)+png->linePrjFile->text());

    errorOccured = !createEmptyDom();
    if (!errorOccured)
    {
      email = pnf->lineEmail  ->text();
      author = pnf->lineAuthor ->text();
      m_defaultDTD = DTDs::ref()->getDTDNameFromNickName(pnf->dtdCombo->currentText());
      m_defaultEncoding  = pnf->encodingCombo->currentText();

      QuantaCommon::setUrl(previewPrefix, pnf->linePrefix->text());
      previewPrefix.adjustPath(-1);

      usePreviewPrefix = pnf->checkPrefix->isChecked();

      QDomElement el;
      KURL url;

      el = dom.firstChild().firstChild().toElement();
      el.setAttribute("type", png->type());
      el.setAttribute("name", projectName );
      el.setAttribute("previewPrefix", previewPrefix.url() );
      el.setAttribute("usePreviewPrefix",usePreviewPrefix);
      el.setAttribute("encoding", m_defaultEncoding);

      el = dom.createElement("author");
      dom.firstChild().firstChild().appendChild( el );
      el.appendChild( dom.createTextNode( author ) );

      el = dom.createElement("email");
      dom.firstChild().firstChild().appendChild( el );
      el.appendChild( dom.createTextNode( email ) );

      el = dom.createElement("defaultDTD");
      dom.firstChild().firstChild().appendChild(el);
      el.appendChild(dom.createTextNode(m_defaultDTD));

      KURL::List list;
      if ( png->type() == "Local" ) list = pnl->files();
      if ( png->type() == "Web"   ) list = pnw->files();

      for ( KURL::List::Iterator it = list.begin(); it != list.end(); ++it )
      {
        url = QExtFileInfo::toRelative(*it, baseURL );
        el = dom.createElement("item");
        el.setAttribute("url",QuantaCommon::qUrl(url));
        dom.firstChild().firstChild().appendChild( el );
      }

     // el = dom.createElement("item");
     // el.setAttribute("url","templates/");
     // dom.firstChild().firstChild().appendChild(el);

     //setup the templates directory
      templateURL = baseURL;
      bool createTemplateDir = true;
      if (pnf->insertGlobalTemplates->isChecked())
      {
        KURL url;
        QuantaCommon::setUrl(url, qConfig.globalDataDir + resourceDir + "templates/");
        slotAddDirectory(url, false);
        QuantaCommon::setUrl(templateURL, "templates/");
        createTemplateDir = false;
      }
      if (pnf->insertLocalTemplates->isChecked())
      {
        KURL url;
        QuantaCommon::setUrl(url, locateLocal("data", resourceDir + "templates/"));
        slotAddDirectory(url, false);
        QuantaCommon::setUrl(templateURL, "templates/");
        createTemplateDir = false;
      }

     if (createTemplateDir)
     {
       QuantaCommon::setUrl(templateURL, png->linePrjTmpl->text());
       templateURL.adjustPath(1);
       templateURL = QExtFileInfo::toAbsolute(templateURL, baseURL);
       if (!QExtFileInfo::createDir(templateURL))
       {
         QuantaCommon::dirCreationError(m_parent, templateURL);
       }
     }
     //the nodes are already created in loadProjectXML() called from createEmptyDom()
     el = dom.firstChild().firstChild().namedItem("templates").toElement();
     url = QExtFileInfo::toRelative(templateURL, baseURL);
     el.firstChild().setNodeValue(QuantaCommon::qUrl(url));

    //setup the toolbars directory
     toolbarURL = baseURL;
     QuantaCommon::setUrl(toolbarURL,png->linePrjToolbar->text());
     toolbarURL.adjustPath(1);
     toolbarURL = QExtFileInfo::toAbsolute(toolbarURL, baseURL);
     if (!QExtFileInfo::createDir(toolbarURL))
     {
       QuantaCommon::dirCreationError(m_parent, toolbarURL);
     }
     el = dom.firstChild().firstChild().namedItem("toolbars").toElement();
     url = QExtFileInfo::toRelative(toolbarURL, baseURL);
     el.firstChild().setNodeValue(QuantaCommon::qUrl(url));

     emit newProjectLoaded(projectName, baseURL, templateURL, m_documentRootURL);
     fileNameList();
     emit reloadTree( m_projectFiles, true );
     emit showTree();

     m_modified = true;

     slotSaveProject();
   }
 }
 if (errorOccured)
 {
   slotCloseProject();
 }
}

void Project::slotOptions()
{
  KURL url;
  KDialogBase optionsDlg(m_parent, "project_options", true, i18n("Project Options"), KDialogBase::Ok | KDialogBase::Cancel);
  ProjectOptions optionsPage(&optionsDlg);
  optionsDlg.setMainWidget(&optionsPage);

  optionsPage.linePrjName->setText( projectName );
  url = QExtFileInfo::toRelative(templateURL, baseURL);
  optionsPage.linePrjTmpl->setText(QuantaCommon::qUrl(url));
  url = QExtFileInfo::toRelative(toolbarURL, baseURL);
  optionsPage.linePrjToolbar->setText( QuantaCommon::qUrl(url) );

  optionsPage.lineAuthor->setText( author );
  optionsPage.lineEmail->setText( email );

  // Signals to handle debugger settings
  connect(optionsPage.buttonDebuggerOptions, SIGNAL(clicked()),
          this, SLOT(slotDebuggerOptions()));
  connect(optionsPage.comboDebuggerClient, SIGNAL(activated(const QString &)),
          this, SLOT(slotDebuggerChanged(const QString &)));


  // Debuggers Combo
  KTrader::OfferList offers = KTrader::self()->query("Quanta/Debugger");
  KTrader::OfferList::ConstIterator iterDbg;
  optionsPage.comboDebuggerClient->clear();
  optionsPage.comboDebuggerClient->insertItem(i18n("No debugger"));
  int idxDbg = 0;
  m_debuggerClientEdit = debuggerClient;
  optionsPage.buttonDebuggerOptions->setEnabled(false);
  for(iterDbg = offers.begin(); iterDbg != offers.end(); ++iterDbg)
  {
    KService::Ptr service = *iterDbg;
    optionsPage.comboDebuggerClient->insertItem(service->name());
    idxDbg++;
    if(debuggerClient == service->name())
    {
      optionsPage.comboDebuggerClient->setCurrentItem(idxDbg);
      optionsPage.buttonDebuggerOptions->setEnabled(true);
    }
  }
  QDomElement uploadEl = dom.firstChild().firstChild().namedItem("uploadprofiles").toElement();
  optionsPage.profileLabel->setText(uploadEl.attribute("defaultProfile"));

  QString excludeStr;
  for (uint i = 0; i < excludeList.count(); i++)
  {
    excludeStr.append(excludeList[i]);
    excludeStr.append(";");
  }
  optionsPage.lineExclude->setText(excludeStr);
  optionsPage.checkCvsignore->setChecked(m_excludeCvsignore);

  optionsPage.linePrefix->setText(previewPrefix.url());
  QStringList lst = DTDs::ref()->nickNameList(true);
  uint pos = 0;
  for (uint i = 0; i < lst.count(); i++)
  {
    optionsPage.dtdCombo->insertItem(lst[i]);
    if (lst[i] == DTDs::ref()->getDTDNickNameFromName(m_defaultDTD))
       pos = i;
  }
  optionsPage.dtdCombo->setCurrentItem(pos);


  QStringList availableEncodingNames(KGlobal::charsets()->availableEncodingNames());
  optionsPage.encodingCombo->insertStringList( availableEncodingNames );
  QStringList::ConstIterator iter;
  int iIndex = -1;
  for (iter = availableEncodingNames.begin(); iter != availableEncodingNames.end(); ++iter)
  {
     ++iIndex;
     if ((*iter).lower() == m_defaultEncoding.lower())
     {
       optionsPage.encodingCombo->setCurrentItem(iIndex);
       break;
     }
  }


  QStringList list;
  QDomNodeList nl = dom.elementsByTagName("projectview");
  QDomElement el;
  for (uint i = 0; i < nl.count(); i++)
  {
    el = nl.item(i).cloneNode().toElement();
    list += el.attribute("name");
  }
  list.sort();

  QString defaultView = dom.firstChild().firstChild().namedItem("autoload").toElement().attribute("projectview");
  if (list.count() > 0)
  {
    optionsPage.viewCombo->insertStringList(list);
    for (uint i = 0; i < list.count(); i++)
    {
      if (list[i] == defaultView)
      {
        optionsPage.viewCombo->setCurrentItem(i);
        break;
      }
    }
  } else
  {
    optionsPage.viewCombo->insertItem(i18n("No view was saved yet."));
    optionsPage.viewCombo->setEnabled(false);
  }

  optionsPage.checkPrefix->setChecked(usePreviewPrefix);
  if ( optionsDlg.exec() )
  {
    projectName = optionsPage.linePrjName->text();
    author    = optionsPage.lineAuthor ->text();
    email      = optionsPage.lineEmail  ->text();

    // Debugger
    debuggerClient = optionsPage.comboDebuggerClient->currentText();

    m_defaultDTD = DTDs::ref()->getDTDNameFromNickName(optionsPage.dtdCombo->currentText()).lower();
    m_defaultEncoding  = optionsPage.encodingCombo->currentText();

    QuantaCommon::setUrl(templateURL, optionsPage.linePrjTmpl->text());
    templateURL.adjustPath(1);
    templateURL = QExtFileInfo::toAbsolute(templateURL, baseURL);
    if (!QExtFileInfo::createDir(templateURL))
    {
      QuantaCommon::dirCreationError(m_parent, templateURL);
    }

    QuantaCommon::setUrl(toolbarURL, optionsPage.linePrjToolbar->text());
    toolbarURL.adjustPath(1);
    toolbarURL = QExtFileInfo::toAbsolute(toolbarURL, baseURL);
    if (!QExtFileInfo::createDir(toolbarURL))
    {
      QuantaCommon::dirCreationError(m_parent, toolbarURL);
    }

    QuantaCommon::setUrl(previewPrefix,optionsPage.linePrefix->text()+"/");
    previewPrefix.adjustPath(-1);
    usePreviewPrefix = optionsPage.checkPrefix->isChecked();

    QDomElement el;

    el = dom.firstChild().firstChild().toElement();
    el.setAttribute("name",projectName);
    el.setAttribute("previewPrefix", previewPrefix.url() );
    el.setAttribute("usePreviewPrefix", usePreviewPrefix );
    el.setAttribute("encoding", m_defaultEncoding);

    el = dom.firstChild().firstChild().namedItem("author").toElement();
    if (!el.isNull())
       el.parentNode().removeChild(el);
    el = dom.createElement("author");
    dom.firstChild().firstChild().appendChild( el );
    el.appendChild( dom.createTextNode( author ) );

    el = dom.firstChild().firstChild().namedItem("email").toElement();
    if (!el.isNull())
       el.parentNode().removeChild(el);
    el = dom.createElement("email");
    dom.firstChild().firstChild().appendChild( el );
    el.appendChild( dom.createTextNode( email ) );

    // Debugger
    el = dom.firstChild().firstChild().namedItem("debuggerclient").toElement();
    if (!el.isNull())
       el.parentNode().removeChild(el);
    el = dom.createElement("debuggerclient");
    dom.firstChild().firstChild().appendChild( el );
    el.appendChild( dom.createTextNode( debuggerClient ) );

    m_excludeCvsignore = optionsPage.checkCvsignore->isChecked();
    excludeStr = optionsPage.lineExclude->text();
    el = dom.firstChild().firstChild().namedItem("exclude").toElement();
    if (!el.isNull())
       el.parentNode().removeChild(el);
    el = dom.createElement("exclude");
    if (m_excludeCvsignore)
      el.setAttribute("cvsignore", "true");
    else
      el.setAttribute("cvsignore", "false");
    dom.firstChild().firstChild().appendChild( el );
    el.appendChild( dom.createTextNode( excludeStr ) );

    el = dom.firstChild().firstChild().namedItem("defaultDTD").toElement();
    if(el.isNull())
    {
      el = dom.createElement("defaultDTD");
      dom.firstChild().firstChild().appendChild(el);
      el.appendChild(dom.createTextNode(m_defaultDTD));
    }
    else
    {
      el.firstChild().setNodeValue(m_defaultDTD);
    }

    el = dom.firstChild().firstChild().namedItem("templates").toElement();
    url = QExtFileInfo::toRelative(templateURL, baseURL);
    if(el.isNull())
    {
      el = dom.createElement("templates");
      dom.firstChild().firstChild().appendChild(el);
      el.appendChild(dom.createTextNode(QuantaCommon::qUrl(url)));
    }
    else
    {
      el.firstChild().setNodeValue(QuantaCommon::qUrl(url));
    }

    url = QExtFileInfo::toRelative(toolbarURL, baseURL);
    el = dom.firstChild().firstChild().namedItem("toolbars").toElement();
    if(el.isNull())
    {
      el = dom.createElement("toolbars");
      dom.firstChild().firstChild().appendChild(el);
      el.appendChild(dom.createTextNode(QuantaCommon::qUrl(url)));
    }
    else
    {
      el.firstChild().setNodeValue(QuantaCommon::qUrl(url));
    }

    if (optionsPage.viewCombo->isEnabled())
    {
      defaultView = optionsPage.viewCombo->currentText();
       el = dom.firstChild().firstChild().namedItem("autoload").toElement();
       if (el.isNull())
       {
         el = dom.createElement("autoload");
        el.setAttribute("projectview", defaultView);
        dom.firstChild().firstChild().appendChild( el );
       } else
       {
        el.setAttribute("projectview", defaultView);
       }
    }

    m_modified = true;
    slotReloadProject();
  }
}

void Project::slotDebuggerOptions()
{
  // Debuggers Combo
  KTrader::OfferList offers = KTrader::self()->query("Quanta/Debugger");
  KTrader::OfferList::ConstIterator iterDbg;
  for(iterDbg = offers.begin(); iterDbg != offers.end(); ++iterDbg)
  {
    KService::Ptr service = *iterDbg;
    if(m_debuggerClientEdit == service->name())
    {
      int errCode = 0;
      DebuggerClient::DebuggerClient* dbg = KParts::ComponentFactory::createInstanceFromService<DebuggerClient::DebuggerClient>(service, this, 0, QStringList(), &errCode);
      if(dbg)
      {
        QDomNode projectNode = dom.firstChild().firstChild();
        QDomNode nodeThisDbg;
        QDomNode nodeDbg  = projectNode.namedItem("debuggers");
        if(nodeDbg.isNull())
        {
          nodeDbg = dom.createElement("debuggers");
          projectNode.appendChild(nodeDbg);
        }

        nodeThisDbg = nodeDbg.namedItem(service->name());
        if(nodeThisDbg.isNull())
        {
          nodeThisDbg = dom.createElement(service->name());
          nodeDbg.appendChild(nodeThisDbg);
        }
        dbg->showConfig(nodeThisDbg);
        delete dbg;
      }
      else
      {
        emit hideSplash();
        KMessageBox::error(NULL, i18n("<qt>Unable to load the debugger plugin, error code %1 was returned: <b>%2</b>.</qt>").arg(errCode).arg(KLibLoader::self()->lastErrorMessage()), i18n("Debugger Error"));
      }
    }
  }
}

void Project::slotDebuggerChanged(const QString &debugger)
{
  m_debuggerClientEdit = debugger;
}

void Project::slotUpload()
{
  emit saveAllFiles();

  ProjectUpload *dlg = new ProjectUpload(KURL(), false, i18n("Upload project items..."));
  dlg->show();
}

void Project::slotUploadURL(const KURL& urlToUpload)
{
  emit saveAllFiles();
  KURL url = QExtFileInfo::toRelative( urlToUpload, baseURL);

  ProjectUpload *dlg = new ProjectUpload(url, false, i18n("Upload project items..."));
  dlg->show();
}

void Project::slotReloadProject()
{
    loadProjectXML();
    emit newProjectLoaded(projectName, baseURL, templateURL, m_documentRootURL);
    // exclude filter might have changed
    fileNameList(false);
    emit reloadTree( m_projectFiles, false );
    emit newStatus();
}

void Project::slotGetMessages(const QString& data)
{
  emit messages(data);
}

void Project::slotRescanPrjDir()
{
  RescanPrj *dlg = new RescanPrj( m_projectFiles, baseURL, excludeRx, m_parent, i18n("New Files in Project's Folder"));
  if ( dlg->exec() )
  {
    insertFiles( dlg->files() );
    fileNameList(true);
    emit reloadTree(m_projectFiles, false);
  }
  delete dlg;
}
/** Returns the relative url with the prefix inserted. */
KURL Project::urlWithPrefix(const KURL& url)
{
  KURL returnUrl;

  if (usePreviewPrefix)
  {
    KURL tempUrl = QExtFileInfo::toRelative(url, baseURL );
    if ( !tempUrl.path().startsWith("/") )
    {
      returnUrl = previewPrefix;
      returnUrl.setPath(previewPrefix.path(1) + tempUrl.path());
    }
  } else
  {
     returnUrl = url;
  }
  return returnUrl;
}

void Project::openCurrentView()
{
 if (!currentProjectView.isEmpty())
 {
   QDomNodeList nl = dom.elementsByTagName("projectview");
   QDomElement el;
   QuantaDoc* doc = quantaApp->doc();
   doc->blockSignals(true);
   for (uint i = 0; i < nl.count(); i++)
   {
      el = nl.item(i).cloneNode().toElement();
      if (el.attribute("name") == currentProjectView)
      {
        quantaApp->slotFileCloseAll();
        QDomNodeList itemNodes = el.childNodes();
        for (int j = itemNodes.count()-1; j >= 0 ; j--)
        {
          QDomElement el2 = itemNodes.item(j).cloneNode().toElement();
          KURL url = baseURL;
          QuantaCommon::setUrl(url,el2.attribute("url"));
          url = QExtFileInfo::toAbsolute(url, baseURL);
          if (el2.nodeName() == "viewitem")
          {
            doc->openDocument(url, m_defaultEncoding, false);
          }
          if (el2.nodeName() == "viewtoolbar")
          {
            quantaApp->slotLoadToolbarFile(url);
          }
          //kapp->eventLoop()->processEvents( QEventLoop::ExcludeUserInput | QEventLoop::ExcludeSocketNotifiers);
        }
        break;
      }
   }
   doc->blockSignals(false);
   Document *w = ViewManager::ref()->activeDocument();
   quantaApp->setCaption(w->url().prettyURL() );
   //FIXME: quantaApp->slotUpdateStatus(w);
 }
}

/** Opens a project view (toolbars & files). */
void Project::slotOpenProjectView()
{
  QStringList list;
  QDomNodeList nl = dom.elementsByTagName("projectview");
  QDomElement el;
  for (uint i = 0; i < nl.count(); i++)
  {
    el = nl.item(i).cloneNode().toElement();
    list += el.attribute("name");
  }
  list.sort();

  bool ok = FALSE;
  QString res = KInputDialog::getItem(
                  i18n("Open Project View"),
                  i18n("Select a project view to open:"), list, 0, FALSE, &ok, m_parent );
  if ( ok)
  {
    currentProjectView = res;
    openCurrentView();
  }
}

/** Saves a project view (group of files & toolbars) asking for a name. */
void Project::slotSaveAsProjectView(bool askForName)
{
  if (askForName)
  {
    bool ok;
    currentProjectView = KInputDialog::getText(i18n("Save Project View As"), i18n("Enter the name of the view:"), "", &ok, m_parent).lower();
    if (!ok)
      return;
  } else
  {
    if (KMessageBox::questionYesNo(m_parent, i18n("<qt>Do you want to overwrite the <b>%1</b> project view?</qt>").arg(currentProjectView))
        == KMessageBox::No) return;
  }
  QDomNodeList nl = dom.elementsByTagName("projectview");
  for (uint i = 0 ;i < nl.count(); i++)
  {
    QDomNode node = nl.item(i);
    if (node.toElement().attribute("name") == currentProjectView)
    {
      if (!askForName ||
          KMessageBox::questionYesNo(m_parent, i18n("<qt>A project view named <b>%1</b> already exists.<br>Do you want to overwrite it?</qt>")
                                            .arg(currentProjectView)) == KMessageBox::Yes)
      {
        node.parentNode().removeChild(node);
        break;
      } else
      {
        return;
      }
    }
  }

  QDomElement el = dom.createElement("projectview");
  el.setAttribute("name", currentProjectView);
  QDomElement item;
  KMdiIterator<KMdiChildView*> *it = quantaApp->createIterator();
  QuantaView *view;
  for (it->first(); !it->isDone(); it->next()) {
      view = dynamic_cast<QuantaView*>(it->currentItem());
      if (view && view->document())
      {
          Document *w = view->document();
          KURL url = w->url();
          url = QExtFileInfo::toRelative(url, baseURL);
          if (!w->isUntitled() && m_projectFiles.contains(url))
          {
              item = dom.createElement("viewitem");
              item.setAttribute("url", QuantaCommon::qUrl(url) );
              el.appendChild(item);
          }
      }
  }
  delete it;

  KURL::List toolbarList = quantaApp->userToolbarFiles();
  for (uint i =0 ; i < toolbarList.count(); i++)
  {
    item = dom.createElement("viewtoolbar");
    KURL url = toolbarList[i];
    url = QExtFileInfo::toRelative(url, baseURL);
    item.setAttribute("url", QuantaCommon::qUrl(url) );
    el.appendChild(item);
  }

  dom.firstChild().firstChild().appendChild( el );
  slotSaveProject();

}

/** Saves a project view (group of files & toolbars) without asking for a name. */
void Project::slotSaveProjectView()
{
  slotSaveAsProjectView(currentProjectView.isEmpty());
}

void Project::slotDeleteProjectView()
{
  QStringList list;
  QDomNodeList nl = dom.elementsByTagName("projectview");
  QDomElement el;
  for (uint i = 0; i < nl.count(); i++)
  {
    el = nl.item(i).cloneNode().toElement();
    list += el.attribute("name");
  }
  list.sort();

  bool ok = FALSE;
  QString res = KInputDialog::getItem(
                  i18n("Delete Project View"),
                  i18n("Select a project view to delete:"), list, 0, FALSE, &ok, m_parent );
  if ( ok)
  {
    for (uint i = 0; i < nl.count(); i++)
    {
      QDomNode node = nl.item(i);
      el = node.cloneNode().toElement();
      if (el.attribute("name") == res)
      {
        node.parentNode().removeChild(node);
        slotSaveProject();
        break;
      }
    }
  }
}

void Project::setModified(bool modified)
{
  m_modified = modified;
  emit newStatus();
}

/*Returns true if url is already in the project.*/
bool Project::contains(const KURL& url)
{
  if (m_projectFiles.isEmpty())
     fileNameList(false);
  return (m_projectFiles.contains(QExtFileInfo::toRelative(url, baseURL)) > 0);
}

void Project::slotFileDescChanged(const KURL& url, const QString& desc)
{
  ProjectUrlList::Iterator it = m_projectFiles.find(QExtFileInfo::toRelative(url, baseURL));
  if (it != m_projectFiles.end()) {
    QDomNodeList nl = dom.elementsByTagName("item");
    QString qurl = QuantaCommon::qUrl(*it);
    const uint nlCount = nl.count();
    for (uint i = 0; i < nlCount; ++i)
    {
      QDomElement el = nl.item(i).toElement();
      if (el.attribute("url") == qurl) {
        el.setAttribute("desc", desc);
        (*it).fileDesc = desc;
        setModified(true);
      }
    }
  }
}

void Project::slotUploadStatusChanged(const KURL& url, int status)
{
   QString urlStr = QExtFileInfo::toRelative(url, baseURL).url();
   if (url == baseURL)
     urlStr = "";
   QString statusStr = QString("%1").arg(status);
   ProjectUrlList::Iterator it;
   for (it = m_projectFiles.begin(); it != m_projectFiles.end(); ++it)
   {
       if ((*it).url().startsWith(urlStr))
       {
         (*it).uploadStatus = status;
          QDomNodeList nl = dom.elementsByTagName("item");
          QString qurl = QuantaCommon::qUrl(*it);
          const uint nlCount = nl.count();
          for (uint i = 0; i < nlCount; ++i)
          {
            QDomElement el = nl.item(i).toElement();
            if (el.attribute("url") == qurl)
            {
              el.setAttribute("uploadstatus", statusStr);
              setModified(true);
              break;
            }
          }
       }
   }
}

void Project::slotDocumentRootChanged(const KURL &url)
{
   KURL relUrl = QExtFileInfo::toRelative(url, baseURL);
   dom.firstChild().firstChild().toElement().setAttribute("documentRoot", QuantaCommon::qUrl(relUrl));
   m_documentRootURL = relUrl;
   setModified(true);
}


/** Returns the project's base URL if it exists, the HOME dir if there is no project and no opened document (or the current opened document was not saved yet), and the base URL of the opened document, if it is saved somewhere. */
KURL Project::projectBaseURL()
{
  KURL result;
  if  (hasProject())
  {
     result = baseURL;
  } else
  {
    Document *w = ViewManager::ref()->activeDocument();
    if  ( !w || w->isUntitled() )
    {
      result = QExtFileInfo::home();
    } else
    {
       result = QExtFileInfo::path(w->url());
    }
  }
  return result;
}

void Project::savePassword(const QString &entry, const QString &passwd, bool store)
{
  m_passwdList[entry] = passwd;
  KConfig *config = kapp->config();
  config->setGroup("Projects");
  if (store)
  {
    //config->writeEntry(projectName + " | " + entry, KStringHandler::obscure(passwd));
    config->writeEntry(projectName + " | " + entry, passwd);
  }
  else
  {
    config->deleteEntry(projectName + " | " + entry);
  }
  config->sync();
}

QString Project::password(const QString &entry)
{
   if (m_passwdList.contains(entry))
     return m_passwdList[entry];
   else
   {
      KConfig *config = kapp->config();
      config->setGroup("Projects");
      QString passwd =  KStringHandler::obscure(config->readEntry(projectName + " | " + entry,""));
//      QString passwd =  config->readEntry(projectName + " | " + entry,"");
      return passwd;
   }
}

bool Project::passwordSaved(const QString &entry)
{
      KConfig *config = kapp->config();
      config->setGroup("Projects");
      QString passwd =  KStringHandler::obscure(config->readEntry(projectName + " | " + entry,""));
//      QString passwd =  config->readEntry(projectName + " | " + entry,"");
      return !passwd.isEmpty();
}

#include "project.moc"
