/***************************************************************************
                          project.cpp  -  description
                             -------------------
    begin                : Thu Mar 16 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavsky
                           (C) 2001-2003 by Andras Mantia
    email                : pdima@mail.univ.kiev.ua, amantia@freemail.hu
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
#include <qinputdialog.h>
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
#include <klineeditdlg.h>
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

// application headers
#include "../dialogs/copyto.h"
#include "../dialogs/dtdselectdialog.h"
#include "../qextfileinfo.h"
#include "projectnewgeneral.h"
#include "projectnewlocal.h"
#include "projectnewweb.h"
#include "projectnewfinal.h"
#include "projectupload.h"
#include "projectoptions.h"
#include "rescanprj.h"
#include "../resource.h"
#include "../document.h"
#include "../quanta.h"
#include "../quantadoc.h"
#include "../quantaview.h"
#include "../treeviews/doctreeview.h"

Project::Project( QWidget *, const char *name )
        : QWidget(0L,name)
{
  projectName = QString::null;
  config = 0L;
  m_modified=false;
  usePreviewPrefix=false;
  m_defaultDTD = qConfig.defaultDocType;
  excludeRx.setPattern(".*~$");
  excludeList.append("*~");
}

Project::~Project()
{
  if (hasProject()) slotSaveProject();
}

bool Project::hasProject()
{
  return !projectName.isNull();
}

/** Retrieve the list of urls which are in the project. If check is true,
for local files it verifies if it exists or not, and adds only the exisiting
files.*/
KURL::List Project::fileNameList(bool check)
{
  KURL::List list;

  //cout << dom.toString() << "\n";
  QDomNodeList nl = dom.elementsByTagName("item");

  for ( unsigned int i=0; i < nl.count(); i++ )
  {
    QDomElement el = nl.item(i).cloneNode().toElement();
    KURL url = baseURL;
    QuantaCommon::setUrl(url,el.attribute("url"));
    if (!excludeRx.exactMatch(url.path()))
    {
      if (url.isLocalFile() && check)
      {
        QFileInfo fi(baseURL.path(1)+url.path());
        if (fi.exists()) list.append(url);
      } else
      {
  //      entry.toolTip = el.attribute("tooltip", url.prettyURL());
        list.append(url);
      }
    }
  }
  m_projectFiles = list;
  return list;
}

void Project::insertFile(const KURL& nameURL, bool repaint )
{
  if (excludeRx.exactMatch(nameURL.path()))
      return;
  KURL relNameURL = QExtFileInfo::toRelative( nameURL, baseURL);

  if ( relNameURL.path().startsWith("/") || relNameURL.path().startsWith(".")  )
  {
    KURLRequesterDlg *urlRequesterDlg = new KURLRequesterDlg( baseURL.prettyURL(), this, "");
    urlRequesterDlg->setCaption(i18n("%1: Copy to Project").arg(nameURL.prettyURL()));
    urlRequesterDlg->urlRequester()->setMode( KFile::Directory | KFile::ExistingOnly);
    urlRequesterDlg->exec();
    KURL destination = urlRequesterDlg->selectedURL();
    delete urlRequesterDlg;
    if ( !destination.isEmpty() )
    {
      CopyTo *dlg = new CopyTo(baseURL);
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

  for ( uint i = 0; i < nl.count(); i++ )
  {
    el = nl.item(i).toElement();
    if ( el.attribute("url") == relNameURL.url() )
        return;                                          //already present
  }

  el = dom.createElement("item");
  el.setAttribute("url", QuantaCommon::qUrl(relNameURL) );

  dom.firstChild().firstChild().appendChild( el );
  m_modified = true;

  if ( repaint )
  {
    emit reloadTree( fileNameList(), false);
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
  KURL::List::Iterator it;
  for ( it = files.begin(); it != files.end(); ++it )
  {
    *it = QExtFileInfo::toRelative( *it, baseURL);
  }

  QDomElement  el;
  QDomNodeList nl = dom.elementsByTagName("item");

  for ( uint i = 0; i < nl.count(); i++ )
  {
    el = nl.item(i).toElement();
    KURL url = baseURL;
    QuantaCommon::setUrl(url,el.attribute("url"));
    if ( files.contains(url))
        files.remove(url.url());
  }

  for ( it = files.begin(); it != files.end(); ++it )
  {
    if (! (*it).isEmpty())
    {
        el = dom.createElement("item");
        KURL url = *it;
        if (!excludeRx.exactMatch(url.path()))
        {
          el.setAttribute("url", QuantaCommon::qUrl(url));
          dom.firstChild().firstChild().appendChild( el );
          m_modified = true;
        }
      }
  }
  emit newStatus();
}

bool Project::createEmptyDom()
{
  QString str;
  QTextStream stream( &str, IO_WriteOnly );

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
    *(tempFile->textStream()) << str;
    tempFile->close();
    result = QExtFileInfo::createDir(baseURL);
    if (result) result = KIO::NetAccess::upload(tempFile->name(), projectURL);
    delete tempFile;
  } else
  {
    QFile f(projectURL.path());
    if (f.open( IO_WriteOnly ))
    {
      QTextStream fstream(&f);
      fstream << str;
    } else
    {
      result = false;
    }
    f.close();
  }

  if (!result)
  {
    KMessageBox::sorry(this, i18n("Can't open file %1 for writing.").arg(projectURL.prettyURL()));
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
  QString urlPath = config->readEntry("Last Project");

  KURL url;
  QuantaCommon::setUrl(url, urlPath);

  slotCloseProject();
  if ( !urlPath.isEmpty() && !url.isMalformed())
  {
   slotLoadProject ( url );
  }
}

void Project::writeConfig(KConfig *config)
{
  config->setGroup  ("Projects");
  config->writeEntry("Last Project", projectURL.url());
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
                       "*.wpj *.webprj"+i18n("|Project Files\n*|All Files"), this);

  if( !url.isEmpty() )
  {
    slotCloseProject();
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
      if (KMessageBox::questionYesNo( this,
           i18n("The file %1 does not exist.\n Do you want to remove it from the list?").arg(url.prettyURL()) )
           == KMessageBox::Yes)
      {
        projectRecent->removeURL(url);
      }
    } else
    {
      slotCloseProject();
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
    tmpFile->setAutoDelete(true);
    dom.save(*(tmpFile->textStream()), 0);
    tmpFile->close();
    if ( KIO::NetAccess::upload( tmpFile->name(), projectURL ) )
    {
        emit statusMsg( i18n( "Wrote project %1..." ).arg( projectURL.prettyURL() ) );
    }
    else
    {
      emit statusMsg( QString::null );
      KMessageBox::error( this, KIO::NetAccess::lastErrorString() );
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
  //fix: add save/no for remote
  if (m_modified)
      slotSaveProject();

  dom.clear();

  projectName = QString::null;
  m_defaultDTD = qConfig.defaultDocType;
  currentProjectView = QString::null;

  emit closeFiles();

  emit setBaseURL( baseURL );
  emit setProjectName( i18n( "No Project" ) );
  emit templateURLChanged(KURL());
  emit reloadTree( fileNameList(), true);

  projectURL  = KURL();
  m_modified = false;
  passwd = "";
  usePreviewPrefix = false;
  previewPrefix = KURL();

  emit newStatus();
}

/** load project from file: name */
void Project::slotLoadProject(const KURL &a_url)
{
  KURL url = a_url;
  projectURL = KURL();
  projectName = QString::null;
  currentProjectView = QString::null;

  if (url.isMalformed())
  {
      KMessageBox::sorry(this, i18n("Malformed URL\n%1").arg(url.prettyURL()));
  } else
  {
    QString tmpName = QString::null;
    if (KIO::NetAccess::download(url, tmpName))
    {
      projectURL = url;
      QFile f(tmpName);
      if (f.open(IO_ReadOnly))
      {
        baseURL = url;
        baseURL.setPath(url.directory(true, true));
        dom.setContent( &f );
        f.close();
        loadProjectXML();
        //load the password for this project
        KConfig *config = quantaApp->config();
        config->setGroup("Projects");
#if KDE_VERSION < KDE_MAKE_VERSION(3,1,90)
        passwd = QuantaCommon::obscure(config->readEntry(projectName, ""));
#else
        passwd = KStringHandler::obscure(config->readEntry(projectName, ""));
#endif
        keepPasswd = !passwd.isEmpty();
        storePasswdInFile = keepPasswd;
        openCurrentView();
        emit newProjectLoaded();
      } else
      {
        KMessageBox::error(this, i18n("Cannot open the file %1 for reading").arg(tmpName));
      }
      KIO::NetAccess::removeTempFile( tmpName);
    } else
    {
      KMessageBox::error(this,i18n("Cannot access the project file %1").arg(url.prettyURL()));
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
    KMessageBox::sorry( this, i18n("Invalid project file.") );
    return;
  }

  m_modified = false;
  QString tmpString = projectNode.toElement().attribute("previewPrefix");
  if ( !tmpString.isEmpty())
  {
    QuantaCommon::setUrl(previewPrefix, tmpString);
    previewPrefix.adjustPath(1);
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

  no = projectNode.namedItem("author");
  author = no.firstChild().nodeValue();
  no = projectNode.namedItem("email");
  email = no.firstChild().nodeValue();
  no = projectNode.namedItem("defaultDTD");
  m_defaultDTD = no.firstChild().nodeValue();
  if (m_defaultDTD.isEmpty()) m_defaultDTD = qConfig.defaultDocType;

  no = projectNode.namedItem("autoload");
  currentProjectView = no.toElement().attribute("projectview");

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
  QString excludeStr = no.firstChild().nodeValue();
  QString regExpStr = "";
  excludeList = QStringList::split(';', excludeStr);
  for (uint i = 0; i < excludeList.count(); i++)
  {
    excludeStr = excludeList[i].stripWhiteSpace();
    if (!excludeStr.endsWith("*"))
      excludeStr.append("$");
    if (!excludeStr.startsWith("*"))
      excludeStr.prepend("^");
    excludeStr.replace(".","\\.");
    excludeStr.replace("*",".*");
    excludeStr.replace("?",".");
    regExpStr.append(excludeStr);
    if (i+1 < excludeList.count())
      regExpStr.append("|");
  }
  excludeRx.setPattern(regExpStr);

  KURL::List fileList;
  QDomNodeList nl = dom.firstChild().firstChild().childNodes();
  quantaApp->slotStatusMsg( i18n("Reading the project file...") );
  progressBar->setTotalSteps(nl.count()-1);
  progressBar->setValue(0);
  progressBar->setTextEnabled(true);
  QString path;
  for ( uint i = 0; i < nl.count(); i++ )
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
            fileList.append(url);
          }
        } else
        {
          if (path.startsWith("doc/") && path.endsWith("/index.html"))
               quantaApp->dTab->addProjectDoc(
               QExtFileInfo::toAbsolute(url, baseURL));
          fileList.append(url);
        }
      } else
      {
        el.parentNode().removeChild( el );
        i--;
      }
    }
    progressBar->advance(1);
  }
  progressBar->setValue(0);
  progressBar->setTextEnabled(false);

  quantaApp->slotStatusMsg( i18n("Done."));

  emit setBaseURL(baseURL);
  emit setProjectName( projectName );
  emit reloadTree( fileList, true);

  emit showTree();
  emit newStatus();

  passwd = "";
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
    baseURL.url(),  i18n("*"), this, i18n("Insert Files in Project"));

  if ( !list.isEmpty() )
  {
    KURL firstURL = list.first();
    firstURL = QExtFileInfo::toRelative( firstURL, baseURL );

    if ( firstURL.path().startsWith("/") || firstURL.path().startsWith("."))
    {
      KURLRequesterDlg *urlRequesterDlg = new KURLRequesterDlg( baseURL.prettyURL(), this, "");
      urlRequesterDlg->setCaption(i18n("Files: Copy to Project"));
      urlRequesterDlg->urlRequester()->setMode( KFile::Directory | KFile::ExistingOnly);
      urlRequesterDlg->exec();
      KURL destination = urlRequesterDlg->selectedURL();
      delete urlRequesterDlg;

      if ( !destination.isEmpty())
      {
        CopyTo *dlg = new CopyTo( baseURL);
        list = dlg->copy( list, destination );
        connect(dlg, SIGNAL(addFilesToProject(const KURL&, CopyTo*)),
                     SLOT  (slotInsertFilesAfterCopying(const KURL&, CopyTo*)));
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

    emit reloadTree( fileNameList(), false);
  }
}

void Project::slotAddDirectory()
{
 KURL url = KURL();
 url = KFileDialog::getExistingURL(baseURL.prettyURL(), this,
                i18n("Insert Directory in Project"));
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
      KURLRequesterDlg *urlRequesterDlg = new KURLRequesterDlg( baseURL.prettyURL(), this, "");
      urlRequesterDlg->setCaption(i18n("%1: Copy to Project").arg(dirURL.prettyURL()));
      urlRequesterDlg->urlRequester()->setMode( KFile::Directory | KFile::ExistingOnly);
      urlRequesterDlg->exec();
      KURL destination = urlRequesterDlg->selectedURL();
      delete urlRequesterDlg;

      if ( (showDlg == false) ||
            (!destination.isEmpty()) )
      {
        CopyTo *dlg = new CopyTo( baseURL);
        //if ( rdir.right(1) == "/" ) rdir.remove( rdir.length()-1,1);
        dirURL = dlg->copy(dirURL, destination);
        connect(dlg, SIGNAL(addFilesToProject(const KURL&, CopyTo*)),
                     SLOT  (slotInsertFilesAfterCopying(const KURL&, CopyTo*)));
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
    emit reloadTree( fileNameList(), false );
  }
}

void Project::slotInsertFilesAfterCopying(const KURL& p_url,CopyTo* dlg)
{
  KURL url = p_url;
//The CopyTo dlg is deleted only here!!
  delete dlg;
  url.adjustPath(1);
  insertFiles( url, "*" );
  emit reloadTree( fileNameList(), false );
}

void Project::slotRenameFinished( KIO::Job * job)
{
  if ( job->error() )
  {
    KMessageBox::error(this,i18n("An error occurred while renaming \"%1\".\nThe error message was:\n\n%2")
                            .arg(oldURL.prettyURL()).arg(job->errorString()));
  } else
  {
    //rename the elements in the project dom tree
    QDomElement el;
    QDomNodeList nl = dom.elementsByTagName("item");
    QString tmpString;
    QString oldString;
    QString oldStr = QuantaCommon::qUrl(oldURL);
    QString newStr = QuantaCommon::qUrl(newURL);
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
    for (uint i = 0; i < nl.count(); i++ )
    {
      el = nl.item(i).toElement();
      tmpString = el.attribute("url");
      if (tmpString.startsWith(oldStr))
      {
        oldString = tmpString;
        tmpString = tmpString.replace(oldStr,newStr);
        if (oldString != tmpString )
        {
          el.setAttribute("url",tmpString);
          el.setAttribute("upload_time","");
        }
      }
    }
    oldURL = KURL();
    newURL = KURL();
    m_modified = true;

    emit reloadTree( fileNameList(), false );
    emit newStatus();
  }
}

/** Rename a file or folder in the project*/
void Project::slotRename(const KURL& url)
{
  bool folderRename = url.fileName(false).isEmpty();

  QString oldName;
  QString caption;
  if (folderRename)
  {
    oldName = url.fileName(true);
    caption = i18n("Rename Directory");
  } else
  {
    oldName = url.fileName();
    caption = i18n("Rename File");
  }

  KLineEditDlg dlg(i18n("Enter the new name:"), oldName, this);
  dlg.setCaption(caption);

  if ( dlg.exec() )
  {
    KURL newUrl = url;
    if (folderRename)
    {
      newUrl.setPath(QFileInfo(url.path(-1)).dirPath() + '/'+dlg.text()+'/');
    } else
    {
      newUrl.setFileName(dlg.text());
    }

    if ( url != newUrl )
    {

      //set the class global attributes
      oldURL = QExtFileInfo::toRelative(url, baseURL);
      newURL = QExtFileInfo::toRelative(newUrl, baseURL);
      bool proceed = true;
      if (QExtFileInfo::exists(newUrl))
      {
        QString s = (newUrl.isLocalFile()) ? newUrl.path() : newUrl.prettyURL();
        proceed = KMessageBox::warningYesNo(this, i18n("<qt>The file <b>%1</b> already exists.<br>Do you want to overwrite it?</qt>").arg(s),i18n("Overwrite")) == KMessageBox::Yes;
      }
      if (proceed)
      {
      //start the rename job
        KIO::SimpleJob *job = KIO::rename( url, newUrl, true );
        connect( job, SIGNAL( result( KIO::Job *) ), SLOT( slotRenameFinished( KIO::Job *) ));
      }
    }
  }
}

void Project::slotRemove(const KURL& urlToRemove)
{
  bool isFolder = urlToRemove.fileName(false).isEmpty();

  QDomElement el;
  QDomNodeList nl = dom.elementsByTagName("item");
  KURL url = QExtFileInfo::toRelative(urlToRemove, baseURL);
  QString tmpString;
  QString urlStr = QuantaCommon::qUrl(url);
  for ( uint i = 0; i < nl.count(); i++ )
  {
    el = nl.item(i).toElement();

    tmpString = el.attribute("url");
    if ( ( !isFolder && tmpString == urlStr) ||
         ( isFolder && tmpString.startsWith(urlStr))
       )
    {
      el.parentNode().removeChild( el );
      m_modified = true;
      if (isFolder)
      {
       i--;
      }
      else break;
    }
  }
  emit reloadTree( fileNameList(), false );
  emit newStatus();
}


/** create new project */
void Project::slotNewProject()
{
  wiz = new QWizard( this, "new", true);
  wiz->setCaption(i18n("New Project Wizard"));
  //wiz->setMinimumSize(620,440);

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

  connect( pnw, SIGNAL(enableMessages()),
          this, SLOT  (slotEnableMessages()));
  connect( pnw, SIGNAL(disableMessages()),
          this, SLOT  (slotDisableMessages()));
  connect( pnw, SIGNAL(messages(QString)),
          this, SLOT  (slotGetMessages(QString)));
  connect( pnw, SIGNAL(enableNextButton(QWidget *,bool)),
           wiz, SLOT(setNextEnabled(QWidget*,bool)));
  connect( pnw, SIGNAL(enableNextButton(QWidget *,bool)),
           wiz, SLOT(setBackEnabled(QWidget*,bool)));

  QString name;
  int index;
  QDictIterator<DTDStruct> it(*dtds);
  for( ; it.current(); ++it )
  {
    index = -1;
    name = it.current()->name;
    if (it.current()->toplevel)
    {
      if (name.lower() == qConfig.defaultDocType) index = 0;
      pnf->dtdCombo->insertItem(QuantaCommon::getDTDNickNameFromName(name), index);
    }
  }

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
  if ( wiz->exec() ) slotAcceptCreateProject();

  delete wiz;

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
  slotCloseProject();
  bool errorOccured = false;

  projectName = png->linePrjName->text();
  QString basePath = png->linePrjDir ->text();

  KURL oldBaseURL = baseURL;
  baseURL = KURL();
  baseURL.setHost(png->lineHost->text());
  baseURL.setUser(png->lineUser->text());
  baseURL.setPass(png->linePasswd->text());
  baseURL.setPort(png->linePort->text().toInt());
  baseURL.setProtocol(png->comboProtocol->currentText());
  if (baseURL.protocol() == i18n("Local")) baseURL.setProtocol("file");
  QuantaCommon::setUrl(baseURL, basePath);
  baseURL.adjustPath(1);
  if (!baseURL.path().startsWith("/")) baseURL.setPath("/"+baseURL.path());
  if (!QExtFileInfo::createDir( baseURL ))
  {
    QuantaCommon::dirCreationError(this, baseURL);
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
      m_defaultDTD = QuantaCommon::getDTDNameFromNickName(pnf->dtdCombo->currentText());
      m_defaultEncoding  = pnf->encodingCombo->currentText();

      QuantaCommon::setUrl(previewPrefix, pnf->linePrefix->text());
      previewPrefix.adjustPath(1);

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
      bool  createTemplateDir = true;
      if (pnf->insertGlobalTemplates->isChecked())
      {
        KURL url;
        QuantaCommon::setUrl(url,qConfig.globalDataDir+"quanta/templates/");
        slotAddDirectory(url, false);
        QuantaCommon::setUrl(templateURL,"templates/");
        createTemplateDir = false;
      }
      if (pnf->insertLocalTemplates->isChecked())
      {
        KURL url;
        QuantaCommon::setUrl(url,locateLocal("data","quanta/templates/"));
        slotAddDirectory(url, false);
        QuantaCommon::setUrl(templateURL,"templates/");
        createTemplateDir = false;
      }

     if (createTemplateDir)
     {
       QuantaCommon::setUrl(templateURL, png->linePrjTmpl->text());
       templateURL.adjustPath(1);
       templateURL = QExtFileInfo::toAbsolute(templateURL, baseURL);
       if (!QExtFileInfo::createDir(templateURL))
       {
         QuantaCommon::dirCreationError(this, templateURL);
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
       QuantaCommon::dirCreationError(this, toolbarURL);
     }
     el = dom.firstChild().firstChild().namedItem("toolbars").toElement();
     url = QExtFileInfo::toRelative(toolbarURL, baseURL);
     el.firstChild().setNodeValue(QuantaCommon::qUrl(url));

     emit closeFiles();
     emit setBaseURL( baseURL );
     emit setProjectName( projectName );
     emit reloadTree( fileNameList(), true );
     emit showTree();
     emit newProjectLoaded();

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
  ProjectOptions optionsPage(quantaApp, "project_options", true );

  optionsPage.linePrjName->setText( projectName );
  url = QExtFileInfo::toRelative(templateURL, baseURL);
  optionsPage.linePrjTmpl->setText(QuantaCommon::qUrl(url));
  url = QExtFileInfo::toRelative(toolbarURL, baseURL);
  optionsPage.linePrjToolbar->setText( QuantaCommon::qUrl(url) );

  optionsPage.lineAuthor->setText( author );
  optionsPage.lineEmail->setText( email );

  QDomElement uploadEl = dom.firstChild().firstChild().namedItem("upload").toElement();

  optionsPage.lineHost->setText(uploadEl.attribute("remote_host",""));
  optionsPage.lineUser->setText(uploadEl.attribute("user",""));
  optionsPage.linePath->setText(uploadEl.attribute("remote_path",""));
  optionsPage.port->setText( uploadEl.attribute("remote_port","") );

  /*
   * keepPasswd->setChecked() would popup the confirm dialog before the
   * project options dialog shows if the user has setted storePasswdInFile
   * and this is ugly. The hack for solving this is ugly too.
   */
  optionsPage.keepPasswd->blockSignals(true);
  optionsPage.keepPasswd->setChecked(storePasswdInFile);
  optionsPage.keepPasswd->blockSignals(false);
  if (storePasswdInFile)
  {
    optionsPage.linePasswd->insert(passwd);
  } else
  {
    optionsPage.linePasswd->clear();
  }

  QString def_p = uploadEl.attribute("remote_protocol","ftp");
  QStringList protocols = KProtocolInfo::protocols();
  protocols.sort();
  for ( uint i = 0; i < protocols.count(); i++ )
  {
    QString p = protocols[i];
    if ( KProtocolInfo::supportsWriting(p) &&
         KProtocolInfo::supportsMakeDir(p) &&
         KProtocolInfo::supportsDeleting(p) )
    {
      optionsPage.comboProtocol->insertItem(p);
      if ( p == def_p )
        optionsPage.comboProtocol->setCurrentItem( optionsPage.comboProtocol->count()-1 );
    }
  }


  QString excludeStr;
  for (uint i = 0; i < excludeList.count(); i++)
  {
    excludeStr.append(excludeList[i]);
    excludeStr.append(";");
  }
  optionsPage.lineExclude->setText(excludeStr);

  optionsPage.linePrefix->setText(previewPrefix.url());
  QStringList lst;
  QDictIterator<DTDStruct> it(*dtds);
  for( ; it.current(); ++it )
  {
    if (it.current()->toplevel)
    {
      lst << it.current()->nickName;
    }
  }
  lst.sort();
  uint pos = 0;
  for (uint i = 0; i < lst.count(); i++)
  {
    optionsPage.dtdCombo->insertItem(lst[i]);
    if (lst[i] == QuantaCommon::getDTDNickNameFromName(m_defaultDTD))
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
  if ( optionsPage.exec() )
  {
    projectName = optionsPage.linePrjName->text();
    author    = optionsPage.lineAuthor ->text();
    email      = optionsPage.lineEmail  ->text();
    m_defaultDTD = QuantaCommon::getDTDNameFromNickName(optionsPage.dtdCombo->currentText()).lower();
    m_defaultEncoding  = optionsPage.encodingCombo->currentText();

    QuantaCommon::setUrl(templateURL, optionsPage.linePrjTmpl->text());
    templateURL.adjustPath(1);
    templateURL = QExtFileInfo::toAbsolute(templateURL, baseURL);
    if (!QExtFileInfo::createDir(templateURL))
    {
      QuantaCommon::dirCreationError(this, templateURL);
    }

    QuantaCommon::setUrl(toolbarURL, optionsPage.linePrjToolbar->text());
    toolbarURL.adjustPath(1);
    toolbarURL = QExtFileInfo::toAbsolute(toolbarURL, baseURL);
    if (!QExtFileInfo::createDir(toolbarURL))
    {
      QuantaCommon::dirCreationError(this, toolbarURL);
    }

    QuantaCommon::setUrl(previewPrefix,optionsPage.linePrefix->text()+"/");
    previewPrefix.adjustPath(1);
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

    excludeStr = optionsPage.lineExclude->text();
    el = dom.firstChild().firstChild().namedItem("exclude").toElement();
    if (!el.isNull())
       el.parentNode().removeChild(el);
    el = dom.createElement("exclude");
    dom.firstChild().firstChild().appendChild( el );
    el.appendChild( dom.createTextNode( excludeStr ) );

    QString regExpStr = "";
    excludeList = QStringList::split(';', excludeStr);
    for (uint i = 0; i < excludeList.count(); i++)
    {
      excludeStr = excludeList[i].stripWhiteSpace();
      if (!excludeStr.endsWith("*"))
        excludeStr.append("$");
      if (!excludeStr.startsWith("*"))
        excludeStr.prepend("^");
      excludeStr.replace(".","\\.");
      excludeStr.replace("*",".*");
      excludeStr.replace("?",".");
      regExpStr.append(excludeStr);
      if (i+1 < excludeList.count())
        regExpStr.append("|");
    }
    excludeRx.setPattern(regExpStr);


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

    QString path = optionsPage.linePath->text();
    if (!path.startsWith("/"))
      path.prepend("/");
    uploadEl.setAttribute("remote_host", optionsPage.lineHost->text() );
    uploadEl.setAttribute("remote_path", path);
    uploadEl.setAttribute("remote_port", optionsPage.port->text() );
    uploadEl.setAttribute("user", optionsPage.lineUser->text() );
    uploadEl.setAttribute("remote_protocol", optionsPage.comboProtocol->currentText() );

    loadProjectXML();

    KConfig *config = quantaApp->config();
    config->setGroup("Projects");
    if (optionsPage.keepPasswd->isChecked())
    {
      passwd = optionsPage.linePasswd->password();
#if KDE_VERSION < KDE_MAKE_VERSION(3,1,90)
      config->writeEntry(projectName, QuantaCommon::obscure(passwd));
#else
      config->writeEntry(projectName, KStringHandler::obscure(passwd));
#endif
      keepPasswd = true;
    }
    else
    {
      config->deleteEntry(projectName);
      keepPasswd = false;
      passwd = "";
    }
    storePasswdInFile = keepPasswd;
    config->sync();
    m_modified = true;

    emit setProjectName( projectName );
    emit templateURLChanged( templateURL );
    emit newStatus();
  }
}

void Project::slotUpload()
{
  emit saveAllFiles();

  ProjectUpload *dlg = new ProjectUpload(this, KURL(), 0, i18n("Upload project items..."), false, Qt::WDestructiveClose);
  dlg->show();
}

void Project::slotUploadURL(const KURL& urlToUpload)
{
  emit saveAllFiles();
  KURL url = QExtFileInfo::toRelative( urlToUpload, baseURL);

  ProjectUpload *dlg = new ProjectUpload(this, url, 0, i18n("Upload project items..."), false, Qt::WDestructiveClose);
  dlg->show();
}

void Project::slotEnableMessages()
{
  emit selectMessageWidget();
}

void Project::slotDisableMessages()
{
  emit disableMessageWidget();
}

void Project::slotGetMessages(QString data)
{
  emit messages(data);
}

void Project::slotRescanPrjDir()
{
  RescanPrj *dlg = new RescanPrj( fileNameList(), baseURL, excludeRx, this, i18n("New Files in Project's Directory"));
  if ( dlg->exec() )
  {
    insertFiles( dlg->files() );
    emit reloadTree( fileNameList(true), false );
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

/** Read property of QString defaultDTD. */
const QString& Project::defaultDTD()
{
  return m_defaultDTD;
}
/** Write property of QString defaultDTD. */
void Project::setDefaultDTD( const QString& p_defaultDTD)
{
  m_defaultDTD = p_defaultDTD;
}

void Project::openCurrentView()
{
 if (!currentProjectView.isEmpty())
 {
   QDomNodeList nl = dom.elementsByTagName("projectview");
   QDomElement el;
   QuantaDoc* doc = quantaApp->doc();
   QuantaView* view = quantaApp->view();
   doc->blockSignals(true);
   view->writeTab()->blockSignals(true);
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
   view->writeTab()->blockSignals(false);
   Document *w = view->write();
   quantaApp->setCaption(w->url().prettyURL() );
   quantaApp->slotUpdateStatus(w);
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
  QString res = QInputDialog::getItem(
                  i18n("Open Project View"),
                  i18n("Select a project view to open."), list, 0, FALSE, &ok, this );

  if ( ok)
  {
    currentProjectView = res;
    openCurrentView();
  }
}

/** Saves a project view (group of files & toolbars) asking for a name. */
void Project::slotSaveAsProjectView(bool askForName)
{
  KLineEditDlg dlg(i18n("Enter the name of the view:"), "", this);
  dlg.setCaption(i18n("Save Project View As"));

  if ( !askForName || dlg.exec() )
  {
    if (askForName) currentProjectView = dlg.text().lower();
    else
    {
      if (KMessageBox::questionYesNo(this, i18n("Do you want to overwrite the project view named \"%1\"?").arg(currentProjectView))
          == KMessageBox::No) return;
    }
    QDomNodeList nl = dom.elementsByTagName("projectview");
    for (uint i =0 ;i < nl.count(); i++)
    {
      QDomNode node = nl.item(i);
      if (node.toElement().attribute("name") == currentProjectView)
      {
        if (!askForName ||
            KMessageBox::questionYesNo(this, i18n("A project view named \"%1\" already exists.\nDo you want to overwrite it?")
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

    KURL::List fileList = fileNameList();
    QDomElement el = dom.createElement("projectview");
    el.setAttribute("name", currentProjectView);
    QDomElement item;
    QTabWidget *tab = quantaApp->view()->writeTab();
    for (int i = 0; i < tab->count(); i++)
    {
      Document *w = dynamic_cast<Document *>(tab->page(i));
      if (!w) {
        continue;
      }
      KURL url = w->url();
      url = QExtFileInfo::toRelative(url, baseURL);
      if (!w->isUntitled() && fileList.contains(url))
      {
       item = dom.createElement("viewitem");
       item.setAttribute("url", QuantaCommon::qUrl(url) );
       el.appendChild(item);
      }
    }

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

}

/** Saves a project view (group of files & toolbars) without asking for a name. */
void Project::slotSaveProjectView()
{
  slotSaveAsProjectView(currentProjectView.isEmpty());
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

#include "project.moc"
