/***************************************************************************
                          project.cpp  -  description
                             -------------------
    begin                : Thu Mar 16 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavsky
    							         (C) 2001, 2002 by Andras Mantia
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

// include files for KDE
#include <kurl.h>
#include <kfile.h>
#include <kcharsets.h>
#include <kwizard.h>
#include <klocale.h>
#include <kaction.h>
#include <ktempfile.h>
#include <kstdaction.h>
#include <kstddirs.h>
#include <kfiledialog.h>
#include <klineeditdlg.h>
#include <kmessagebox.h>
#include <kprogress.h>
#include <kio/job.h>
#include <kio/netaccess.h>

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

Project::Project( QWidget *, const char *name )
        : QWidget(0L,name)
{
  projectName = QString::null;
  config = 0L;
  modified=false;
  olfwprj=false;
  usePreviewPrefix=false;
  m_defaultDTD = qConfig.defaultDocType;
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
	return list;
}

void Project::insertFile(const KURL& nameURL, bool repaint )
{
  KURL relNameURL = QExtFileInfo::toRelative( nameURL, baseURL);

  if ( relNameURL.path().startsWith("/") || relNameURL.path().startsWith(".")  )
  {
    CopyTo *dlg = new CopyTo( baseURL, this, i18n("%1: copy to project...").arg(nameURL.prettyURL()) );

    if ( dlg->exec() )
    {
      KURL tmpURL = dlg->copy( nameURL );
      relNameURL = QExtFileInfo::toRelative( tmpURL, baseURL);
    }
    else  // Copy canceled, addition aborted
    {
    	delete dlg;
      return;
    }
    delete dlg;
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
  modified = true;

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
    		el.setAttribute("url", QuantaCommon::qUrl(url));
    		dom.firstChild().firstChild().appendChild( el );
    		modified = true;
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
    KTempFile *tempFile = new KTempFile();
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
                       "*.wpj *.webprj"+i18n("|Project files\n*|All files"), this);

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
    	     i18n("The file %1 does not exist.\n Do you want to remove from the list?").arg(url.prettyURL()) )
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
  
  	KTempFile *tmpFile;
    tmpFile = new KTempFile;
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
    modified = false;
    emit newStatus();
  } else
  {
    result = false;
  }
 if (result) projectRecent->addURL( projectURL );
 return result;
}

/** close project and edited files */
void Project::slotCloseProject()
{
  //fix: add save/no for remote
  if (modified) slotSaveProject();

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
  modified = false;
  passwd = "";  

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
        openCurrentView();
        emit newProjectLoaded();
      } else
      {
        KMessageBox::error(this, i18n("Cannot open the file %1 for reading").arg(tmpName));
      }
      KIO::NetAccess::removeTempFile( tmpName);
    } else
    {
      KMessageBox::error(this,i18n("Cannot acces the project file %1").arg(url.prettyURL()));
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
	
  modified = false;
  QString tmpString = projectNode.toElement().attribute("previewPrefix");
  if ( !tmpString.isEmpty())
  {
    QuantaCommon::setUrl(previewPrefix, tmpString);
    previewPrefix.adjustPath(1);
    if (tmpString != previewPrefix.url()) //compatibility
    {
      projectNode.toElement().setAttribute("previewPrefix",previewPrefix.url());
      modified = true;
    }
  }

  usePreviewPrefix = ( projectNode.toElement().attribute("usePreviewPrefix") == "1");
  m_defaultEncoding = projectNode.toElement().attribute("encoding");
  if (m_defaultEncoding.isEmpty())
  {
    m_defaultEncoding = qConfig.defaultEncoding;
  }

  m_newFileType = projectNode.toElement().attribute("newfiles");
  if (m_newFileType.isEmpty())
  {
    m_newFileType = qConfig.newFileType;
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
    modified = true;
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
    modified = true;
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

  KURL::List fileList;
  QDomNodeList nl = dom.firstChild().firstChild().childNodes();
  quantaApp->slotStatusMsg( i18n("Reading the project file...") );
  progressBar->setTotalSteps(nl.count()-1);
  progressBar->setValue(0);
  progressBar->setTextEnabled(true);
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
        modified = true;
      }
    }
  	if ( el.nodeName() == "item" )
  	{
      //remove non-existent local files
     	if ( url.isLocalFile() )
    	{
    	  QFileInfo fi( baseURL.path(1)+url.path());
    	  if ( !fi.exists() )
    	  {
    		  el.parentNode().removeChild( el );
    		  i--;
    	  } else
        {
          fileList.append(url);
        }
    	} else
      {
        fileList.append(url);
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
		baseURL.url(),	i18n("*"), this, i18n("Insert Files in Project"));
		
	if ( !list.isEmpty() )
  {
  	KURL firstURL = list.first();
  	firstURL = QExtFileInfo::toRelative( firstURL, baseURL );
  	
  	if ( firstURL.path().startsWith("/") || firstURL.path().startsWith("."))
  	{
  		CopyTo *dlg = new CopyTo( baseURL, this, i18n("Files: copy to project...") );

      if ( dlg->exec() )
      {
        list = dlg->copy( list );
        connect(dlg, SIGNAL(addFilesToProject(const KURL&, CopyTo*)),
                     SLOT  (slotInsertFilesAfterCopying(const KURL&, CopyTo*)));
        return;
      }
      else {
      	delete dlg;
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
 if ( KDE_VERSION < 308)
 {
	 QString dir = KFileDialog::getExistingDirectory(baseURL.prettyURL(), this,
                i18n("Insert Directory in Project"));
   url = baseURL;             
   QuantaCommon::setUrl(url,dir);
 } else
 {
   url = KFileDialog::getExistingURL(baseURL.prettyURL(), this,
                i18n("Insert Directory in Project"));
 }
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
    	CopyTo *dlg = new CopyTo( baseURL, this, i18n("%1: copy to project...").arg(dirURL.prettyURL()) );

      if ( (showDlg == false) ||
            (dlg->exec() ) )
      {
        //if ( rdir.right(1) == "/" ) rdir.remove( rdir.length()-1,1);
        dirURL = dlg->copy(dirURL);
        connect(dlg, SIGNAL(addFilesToProject(const KURL&, CopyTo*)),
                     SLOT  (slotInsertFilesAfterCopying(const KURL&, CopyTo*)));
        return;
      }
      else
      {
      	delete dlg;
        return;
      }
      delete dlg;
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
    KMessageBox::error(this,i18n("An error occured while renaming \"%1\".\nThe error message was:\n\n%2")
                            .arg(oldURL.prettyURL()).arg(job->errorString()));
  } else
  {
    //rename the elements in the project dom tree
    QDomElement el;
    QDomNodeList nl = dom.elementsByTagName("item");
    QString tmpString;
    QString oldStr = QuantaCommon::qUrl(oldURL);
    QString newStr = QuantaCommon::qUrl(newURL);
    for (uint i = 0; i < nl.count(); i++ )
    {
      el = nl.item(i).toElement();
      tmpString = el.attribute("url");    
      tmpString = tmpString.replace(QRegExp(oldStr),newStr);
      el.setAttribute("url",tmpString);
    }
    oldURL = KURL();
    newURL = KURL();
    modified = true;

    emit reloadTree( fileNameList(), false );
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
      //start the rename job

      KIO::SimpleJob *job = KIO::rename( url, newUrl, true );
      connect( job, SIGNAL( result( KIO::Job *) ), SLOT( slotRenameFinished( KIO::Job *) ));
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
      modified = true;
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
	wiz = new QWizard( 0, "new", true);
	wiz->setMinimumSize(620,440);
	wiz->setMaximumSize(620,440);
	wiz->setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));

	png = new ProjectNewGeneral( wiz );

	stack = new QWidgetStack( wiz );

	pnl = new ProjectNewLocal( stack );
	pnw = new ProjectNewWeb  ( stack );
	pnf = new ProjectNewFinal( wiz );

	stack->addWidget( pnl, 0);
	stack->addWidget( pnw, 1 );

	wiz->addPage( png,   i18n("<b>General project settings</b>"));
	wiz->addPage( stack, i18n("<b>Insert files in project</b>"));
	wiz->addPage( pnf,   i18n("<b>More project settings</b>"));

	wiz->setNextEnabled  ( png,   false );
	wiz->setBackEnabled  ( stack, true  );
	wiz->setNextEnabled  ( stack, true  );
	wiz->setNextEnabled  ( pnf, 	false );
	wiz->setFinishEnabled( pnf, 	true  );

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
    if (it.current()->family == Xml)
    {
      if (name.lower() == qConfig.defaultDocType) index = 0;
      pnf->dtdCombo->insertItem(QuantaCommon::getDTDNickNameFromName(name), index);
      index = -1;
    }
    if (name.lower() == qConfig.newFileType) index = 0;
    pnf->newfileCombo->insertItem(QuantaCommon::getDTDNickNameFromName(name), index);
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

      el = dom.createElement("author");
      dom.firstChild().firstChild().appendChild( el );
      el.appendChild( dom.createTextNode( author ) );

      el = dom.createElement("email");
      dom.firstChild().firstChild().appendChild( el );
      el.appendChild( dom.createTextNode( email ) );

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

     modified = true;

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
	QTabDialog *dlg = new QTabDialog(quantaApp, i18n("Project Options"), true);
  KURL url;

	ProjectOptions* optionsPage = new ProjectOptions(dlg );

	dlg->addTab( optionsPage, i18n("General") );

	dlg->setOkButton();
	dlg->setCancelButton();

  optionsPage->linePrjName->setText( projectName );
  url = QExtFileInfo::toRelative(templateURL, baseURL);
  optionsPage->linePrjTmpl->setText(QuantaCommon::qUrl(url));
  url = QExtFileInfo::toRelative(toolbarURL, baseURL);
  optionsPage->linePrjToolbar->setText( QuantaCommon::qUrl(url) );

  optionsPage->lineAuthor->setText( author );
	optionsPage->lineEmail->setText( email );
	
	optionsPage->linePrefix->setText(previewPrefix.url());
  QString name;
  int index;
  QDictIterator<DTDStruct> it(*dtds);
  for( ; it.current(); ++it )
  {
    index = -1;
    name = it.current()->name;
    if (it.current()->family == Xml)
    {
      if (name.lower() == m_defaultDTD) index = 0;
      optionsPage->dtdCombo->insertItem(QuantaCommon::getDTDNickNameFromName(name), index);
      index = -1;
    }
    if (name.lower() == m_newFileType) index = 0;
    optionsPage->newfileCombo->insertItem(QuantaCommon::getDTDNickNameFromName(name), index);
  }

  QStringList availableEncodingNames(KGlobal::charsets()->availableEncodingNames());
  optionsPage->encodingCombo->insertStringList( availableEncodingNames );
  QStringList::ConstIterator iter;
  int iIndex = -1;
  for (iter = availableEncodingNames.begin(); iter != availableEncodingNames.end(); ++iter)
  {
     ++iIndex;
     if ((*iter).lower() == m_defaultEncoding.lower())
     {
       optionsPage->encodingCombo->setCurrentItem(iIndex);
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
    optionsPage->viewCombo->insertStringList(list);
    for (uint i = 0; i < list.count(); i++)
    {
      if (list[i] == defaultView)
      {
        optionsPage->viewCombo->setCurrentItem(i);
        break;
      }
    }
  } else
  {
    optionsPage->viewCombo->insertItem(i18n("No view was saved yet."));
    optionsPage->viewCombo->setEnabled(false);
  }

	optionsPage->checkPrefix->setChecked(usePreviewPrefix);
	if ( dlg->exec() )
	{
		projectName = optionsPage->linePrjName->text();
		author		= optionsPage->lineAuthor ->text();
		email			= optionsPage->lineEmail	->text();
    m_defaultDTD = QuantaCommon::getDTDNameFromNickName(optionsPage->dtdCombo->currentText()).lower();
    m_defaultEncoding  = optionsPage->encodingCombo->currentText();
    m_newFileType  = QuantaCommon::getDTDNameFromNickName(optionsPage->newfileCombo->currentText());

    QuantaCommon::setUrl(templateURL, optionsPage->linePrjTmpl->text());
    templateURL.adjustPath(1);
    templateURL = QExtFileInfo::toAbsolute(templateURL, baseURL);
    if (!QExtFileInfo::createDir(templateURL))
    {
      QuantaCommon::dirCreationError(this, templateURL);
    }

    QuantaCommon::setUrl(toolbarURL, optionsPage->linePrjToolbar->text());
    toolbarURL.adjustPath(1);
    toolbarURL = QExtFileInfo::toAbsolute(toolbarURL, baseURL);
    if (!QExtFileInfo::createDir(toolbarURL))
    {
      QuantaCommon::dirCreationError(this, toolbarURL);
    }
		
		QuantaCommon::setUrl(previewPrefix,optionsPage->linePrefix->text()+"/");
    previewPrefix.adjustPath(1);
		usePreviewPrefix = optionsPage->checkPrefix->isChecked();

		QDomElement el;

 		el = dom.firstChild().firstChild().toElement();
 		el.setAttribute("name",projectName);
 		el.setAttribute("previewPrefix", previewPrefix.url() );
 		el.setAttribute("usePreviewPrefix", usePreviewPrefix );
    el.setAttribute("encoding", m_defaultEncoding);
    el.setAttribute("newfiles", m_newFileType);

 		el = dom.firstChild().firstChild().namedItem("author").toElement();
 		if (el.isNull())
 		{
 		  el = dom.createElement("author");
		  dom.firstChild().firstChild().appendChild( el );
          el.appendChild( dom.createTextNode( author ) );
 		} else
 		{
 		  el.firstChild().setNodeValue(author);
 		}

 		el = dom.firstChild().firstChild().namedItem("email").toElement();
 		if (el.isNull())
 		{
 		  el = dom.createElement("email");
		  dom.firstChild().firstChild().appendChild( el );
          el.appendChild( dom.createTextNode( email ) );
 		} else
 		{
 		  el.firstChild().setNodeValue(email);
 		}

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

    if (optionsPage->viewCombo->isEnabled())
    {
      defaultView = optionsPage->viewCombo->currentText();
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
    modified = true;
		
		emit setProjectName( projectName );
    emit templateURLChanged( templateURL );
		emit newStatus();
	}

	delete dlg;
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
  RescanPrj *dlg = new RescanPrj( fileNameList(), baseURL, this, i18n("New files in project's dir..."));
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
   for (uint i = 0; i < nl.count(); i++)
   {
      el = nl.item(i).cloneNode().toElement();
      if (el.attribute("name") == currentProjectView)
      {
        quantaApp->slotFileCloseAll(); //TODO: make a signal for closeAll
        QDomNodeList itemNodes = el.childNodes();
        for (int j = itemNodes.count()-1; j >= 0 ; j--)
        {
          QDomElement el2 = itemNodes.item(j).cloneNode().toElement();
          KURL url = baseURL;
          QuantaCommon::setUrl(url,el2.attribute("url"));
          url = QExtFileInfo::toAbsolute(url, baseURL);
          if (el2.nodeName() == "viewitem")
          {
            emit openFile(url, m_defaultEncoding);
          }
          if (el2.nodeName() == "viewtoolbar")
          {
            quantaApp->slotLoadToolbarFile(url);
          }
        }
        break;
      }
   }
 }  
}

/** Opens a project view (toolbars & files). */
void Project::slotOpenProjectView()
{
  DTDSelectDialog dlg(this);
  dlg.setCaption(i18n("Open project view"));
  dlg.messageLabel->setText(i18n("Select a project view to open."));
  dlg.comboLabel->setText(i18n("Available views"));
  dlg.textLabel->hide();
  dlg.currentDTD->hide();
  dlg.convertDTD->hide();
  QStringList list;
  QDomNodeList nl = dom.elementsByTagName("projectview");
  QDomElement el;
  for (uint i = 0; i < nl.count(); i++)
  {
    el = nl.item(i).cloneNode().toElement();
    list += el.attribute("name");
  }
  list.sort();
  dlg.dtdCombo->insertStringList(list);
  
  if ( dlg.exec() )
  {
    currentProjectView = dlg.dtdCombo->currentText();
    openCurrentView();
  }
}

/** Saves a project view (group of files & toolbars) asking for a name. */
void Project::slotSaveAsProjectView(bool askForName)
{
  KLineEditDlg dlg(i18n("Enter the name of the view:"), "", this);
  dlg.setCaption(i18n("Save project view as"));

  if ( !askForName || dlg.exec() )
  {
    if (askForName) currentProjectView = dlg.text().lower();
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
    QDictIterator<Document> it( * quantaApp->getDoc()->docList());
    QDomElement item;
    while ( Document *w = it.current() )
    {
      KURL url = w->url();
      url = QExtFileInfo::toRelative(url, baseURL);
      if (!w->isUntitled() && fileList.contains(url))
      {
       item = dom.createElement("viewitem");
       item.setAttribute("url", QuantaCommon::qUrl(url) );
       el.appendChild(item);
      }
      ++it;
    }

    KURL::List toolbarList = quantaApp->userToolbarFiles();
    for (uint i =0 ; i < toolbarList.count(); i++)
    {
      item = dom.createElement("viewtoolbar");
      item.setAttribute("url", QuantaCommon::qUrl(toolbarList[i]) );
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


#include "project.moc"
