/***************************************************************************
                          project.cpp  -  description
                             -------------------
    begin                : Thu Mar 16 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavsky
    							         (C) 2001-2002 by Andras Mantia
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
#include <qdir.h>
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
#include <kwizard.h>
#include <klocale.h>
#include <kaction.h>
#include <ktempfile.h>
#include <kstdaction.h>
#include <kstddirs.h>
#include <kfiledialog.h>
#include <klineeditdlg.h>
#include <kmessagebox.h>
#include <kio/job.h>
#include <kio/netaccess.h>

// application headers
#include "copyto.h"
#include "../qextfileinfo.h"
#include "projectnewgeneral.h"
#include "projectnewlocal.h"
#include "projectnewweb.h"
#include "projectnewfinal.h"
#include "projectupload.h"
#include "rescanprj.h"
#include "../resource.h"


extern QString globalDataDir;

Project::Project( QWidget *, const char *name )
        : QWidget(0L,name)
{
  name=QString::null;
  config = 0L;
  modified=false;
  olfwprj=false;
  usePreviewPrefix=false;
  m_defaultDTD = qConfig.defaultDocType;
}

Project::~Project()
{
  if ( hasProject() ) saveProject();
}

bool Project::hasProject(){
  return ( !name.isNull() );
}

QStringList Project::fileNameList(bool check)
{
	QStringList list;

	QDomNodeList nl = dom.firstChild().firstChild().childNodes();

	for ( unsigned int i=0; i<nl.count(); i++ )
  {
		QDomElement el = nl.item(i).toElement();
		if ( el.nodeName() == "item" )	{
		  if ( !check )
		    list.append( el.attribute("url") );
		  else {
		    QFileInfo fi( basePath + el.attribute("url") );
		    if ( fi.exists() || !url.isLocalFile() )
		      list.append( el.attribute("url") );
		  }
		}
  }
	return list;
}

void Project::insertFile( QString rname, bool repaint )
{
  if ( rname.left(5) == "file:" ) rname.remove(0,5);

  QString fname = QExtFileInfo::toRelative( rname, basePath);

  if ( fname.left(2) == ".." )
  {
    CopyTo *dlg = new CopyTo( basePath, this, i18n("%1: copy to project...").arg(rname) );

    if ( dlg->exec() )
    {
      fname = dlg->copy( rname );
      fname = QExtFileInfo::toRelative( fname, basePath);
    }
    else {
    	delete dlg;
      return;
    }
    delete dlg;
  }

	QDomElement  el;
	QDomNodeList nl = dom.firstChild().firstChild().childNodes();

	for ( unsigned int i=0; i<nl.count(); i++ )
  {
		el = nl.item(i).toElement();
		if ( el.nodeName() == "item" )
			if ( el.attribute("url") == fname )
				return;
  }

  el = dom.createElement("item");
  el.setAttribute("url", fname );

  dom.firstChild().firstChild().appendChild( el );
  modified = true;

  if ( repaint )
  {
    emit reloadTree( fileNameList(), false, false );
    emit newStatus();
  }
}


/** insert files from dir recursive */
void Project::insertFiles( QString path, QString mask )
{
    QStringList list;

    list.append(path);
    list += QExtFileInfo::allFiles( path, mask );
	insertFiles(list);
}

/** insert files */
void Project::insertFiles( QStringList files )
{
  QStringList::Iterator it;
  for ( it = files.begin(); it != files.end(); ++it )
	{
		if ( (*it).left(5)=="file:" ) (*it).remove(0,5);
		*it = QExtFileInfo::toRelative( *it, basePath);
	}

  QDomElement  el;
  QDomNodeList nl = dom.firstChild().firstChild().childNodes();

  for ( unsigned int i=0; i<nl.count(); i++ )
  {
		el = nl.item(i).toElement();
		if ( el.nodeName() == "item" )
			if ( files.contains( el.attribute("url")))
				files.remove( el.attribute("url"));
  }

  for ( it = files.begin(); it != files.end(); ++it )
  {
	if (! (*it).isEmpty())
	{
  		el = dom.createElement("item");
  		el.setAttribute("url", *it );
  		dom.firstChild().firstChild().appendChild( el );
  		modified = true;
  	}
  }
  emit newStatus();
}

void Project::createEmptyDom()
{
  QString sf = url.prettyURL();
  if (sf.left(5) == "file:" ) sf.remove(0,5);

  QFile f(sf);
  if ( !f.open( IO_WriteOnly ) )
  {
    KMessageBox::sorry(this, i18n("Can't open file %1 for writing only.").arg(sf));
    return;
  }

  QTextStream qts( &f );

  qts << "<!DOCTYPE webproject ><webproject>" << endl;
  qts << "\t<project name=\"" << name << "\">" << endl;
  qts << "\t\t<upload />" << endl;
  qts << "\t</project>" << endl;
  qts << "</webproject>" << endl;

  f.close();

  loadProject( url );
}

void Project::readConfig (KConfig *config)
{
  this->config = config;

  config->setGroup  ("Projects");
  projectRecent->loadEntries(config, "RecentProjects");

  /*
  QString url = config->readEntry("Last Project")
  KURL u(url);
  if ( url.isEmpty())   return;
  if ( u.isMalformed()) return;

  closeProject();
  loadProject ( url );
  */
}

void Project::readLastConfig(KConfig *c)
{
  KConfig *config;
  if(c == 0)
    config = this->config;
  else
    config = c;

  config->setGroup("Projects");
  QString url = config->readEntry("Last Project");

  KURL u(url);
  if ( url.isEmpty())   return;
  if ( u.isMalformed()) return;

   closeProject();
   loadProject ( url );
}

void Project::writeConfig(KConfig *config)
{
  config->setGroup  ("Projects");
  config->writeEntry("Last Project", url.url());

  projectRecent->saveEntries(config, "RecentProjects");

  saveProject();
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
// SLOTS implementation
//

/** open project file */
void Project::openProject()
{
  KURL url = KFileDialog::getOpenURL( QString::null,
                       i18n("*.wpj *.webprj|Project files\n*|All files"), this);

  if( !url.url().isEmpty() )
  {
    closeProject();
    loadProject ( url );
    emit closeFiles();

    projectRecent->addURL( url );
  }

  emit newStatus();
}

void Project::openProject(const KURL &url)
{
  if ( url.url().isEmpty() ) return;

  QString fn = url.prettyURL();
  if ( fn.left(5) == "file:" ) fn.remove(0,5);

  if ( (url.isLocalFile()) && (! QFileInfo(fn).exists()) )
 {
  	if (KMessageBox::questionYesNo( this,
  	     i18n("The file %1 does not exist.\n Do you want to remove from the list?").arg(fn) )
  	     == KMessageBox::Yes)
  	{
    	projectRecent->removeURL(url);
  	}
  } else
  {
  	closeProject();
  	loadProject ( url );
  	emit closeFiles();
  	projectRecent->addURL( url );
  }
}

/** save project file */
bool Project::saveProject()
{
  if ( !hasProject()  ) return false;

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

	QString fn;
	KTempFile *tmp;

  if ( !url.isLocalFile() )
  {
    tmp = new KTempFile;
    fn   = tmp->name();
  }
  else
  {
    fn = url.prettyURL();
    if ( fn.left(5) == "file:" ) fn.remove(0,5);
  }

  QFile f( fn );
  if ( !f.open( IO_ReadWrite | IO_Truncate ) )
  {
    return false;
  }
  QTextStream qts( &f );
  dom.save( qts, 0);
  f.close();

  if ( !url.isLocalFile() )
  {
    if ( KIO::NetAccess::upload( tmp->name(), url ) )
    {
      emit statusMsg( i18n( "Wrote project %1..." ).arg( url.url() ) );
    }
    else
    {
      emit statusMsg( QString::null );
      KMessageBox::error( this, KIO::NetAccess::lastErrorString() );
    }

    delete tmp;
  }

  modified = false;
  emit newStatus();

  return true;
}

/** close project and edited files */
void Project::closeProject()
{
  //fix: add save/no for remote
  if (modified) saveProject();

  dom.clear();

  name = QString::null;
  m_defaultDTD = qConfig.defaultDocType;

  emit closeFiles();

  emit setBasePath		( basePath );
  emit setProjectName	( i18n( "No Project" ) );
  emit templateDirChanged(QString::null);
  emit reloadTree 		( fileNameList(), true, false );

  url  = KURL();
  modified = false;
  passwd = "";

  emit newStatus();
}

/** load project from file: name */
void Project::loadProject(const KURL &url)
{
  KURL u(url);

  u = url.prettyURL();
  this->url =KURL();
  this->name=QString::null;

  if (u.isMalformed())
  {
      QString s = i18n("Malformed URL\n%1").arg(u.prettyURL());
      KMessageBox::sorry(this, s);
      return;
  }

  if ( !u.isLocalFile() )
  {
    emit statusMsg(i18n("Loading project %1").arg(u.url()));
    emit checkOpenAction(false);
    // clear
    QByteArray b;
    buff.setBuffer(b);

    KIO::Job *job = KIO::get( u );

    connect( job, SIGNAL( result( KIO::Job * ) ), this, SLOT( slotProjectReadFinish(KIO::Job *)));
    connect( job, SIGNAL( data(KIO::Job *,const QByteArray &)), this, SLOT( slotProjectReadData(KIO::Job *,const QByteArray &)));

    this->url = u;

    QString s = u.prettyURL();
    s.remove(s.findRev('/')+1,s.length());

    basePath = s;
  }
  else
  {
    QString fn = u.prettyURL();

    if ( fn.left(5) == "file:" ) fn.remove(0,5);

    QFile       f( fn );
    QFileInfo   fi( fn );

    basePath = fi.dirPath();
    if ( basePath.right(1) != "/" )	basePath += "/";

    if ( !f.exists() ) { KMessageBox::sorry( this, i18n("The file %1 does not exist.").arg(fn) );return;}
    if ( !f.open(IO_ReadOnly))  { KMessageBox::sorry( this, i18n("Can't open %1 for reading only.").arg(fn) );return;}
    if ( !dom.setContent( &f )) { KMessageBox::sorry( this, i18n("Didn't find XML info in file %1.").arg(fn) );return;}

    this->url = u;

    loadProjectXML();
  }

  emit newProjectLoaded();
}

void Project::slotProjectReadFinish(KIO::Job *job)
{
  emit checkOpenAction(true);
  if ( job->error() ) job->showErrorDialog();

  QString s(buff.buffer());

  if ( !dom.setContent( s )) { KMessageBox::sorry( this, i18n("Didn't find XML info in file.") );return;}

  loadProjectXML();

  // we finish load project
  // and now very cool if we load
  // remote "last opened" files
  // for this first ftp session
  if (olfwprj) return;
  config->setGroup("General Options");

  QStrList urls;
    config->readListEntry("List of opened files", urls);

  for ( urls.last();urls.current();urls.prev() )
  {
    KURL fu(urls.current());

    if ( !fu.isLocalFile() )
      emit openFile( fu, qConfig.defaultEncoding );
  }
  olfwprj=true;
}

void Project::slotProjectReadData(KIO::Job *,const QByteArray &data)
{
  buff.open(IO_WriteOnly | IO_Append );
  buff.writeBlock( data.data(), data.size() );
  buff.close();
}

void Project::loadProjectXML()
{
  QDomNode    no;
  QDomElement el;

  if ((no=dom.firstChild().firstChild()).isNull())      { KMessageBox::sorry( this, i18n("Wrong project's file.") );return;}
  if ((name=no.toElement().attribute("name")).isNull()) { KMessageBox::sorry( this, i18n("Wrong project's file.") );return;}

  QString s = no.toElement().attribute("previewPrefix");
  if ( s.right(1) != "/" && !s.isEmpty() ) s+="/";
  previewPrefix = s;

  usePreviewPrefix = ( no.toElement().attribute("usePreviewPrefix") == "1");

  no = dom.firstChild().firstChild().namedItem("author");
  author = no.firstChild().nodeValue();
  no = dom.firstChild().firstChild().namedItem("email");
  email = no.firstChild().nodeValue();
  no = dom.firstChild().firstChild().namedItem("defaultDTD");
  if(no.isNull()) // compatability
    m_defaultDTD = qConfig.defaultDocType;
  else
    m_defaultDTD = no.firstChild().nodeValue();

  no = dom.firstChild().firstChild().namedItem("templates");
  if(no.isNull()) // compatability
    templateDir = basePath + "templates";
  else
    templateDir = no.firstChild().nodeValue();
  no = dom.firstChild().firstChild().namedItem("toolbars");
  if(no.isNull()) // compatability
    toolbarDir = basePath + "toolbars";
  else
    toolbarDir = no.firstChild().nodeValue();

  QDomNodeList nl = dom.firstChild().firstChild().childNodes();

  for ( unsigned int i=0; i<nl.count(); i++ )
  {
    el = nl.item(i).toElement();

  	if ( el.nodeName() == "openfile" )
  	{
  		KURL u(el.attribute("url"));
  		emit openFile( u, qConfig.defaultEncoding );
  	}

  	if ( el.nodeName() == "item" )
  	{
     	if ( url.isLocalFile() )
    	{
    	  QFileInfo fi( basePath+el.attribute("url") );
    	  if ( !fi.exists() )
    	  {
    		  el.parentNode().removeChild( el );
    		  i--;
    	  }
    	}
  	}
  }

  emit setBasePath( basePath );
  emit setProjectName( name );
  emit reloadTree( fileNameList(true), true, false );

  emit showTree();
  emit newStatus();

  modified = false;
  passwd = "";
}

// slot for insert file
void Project::insertFile( QString name )
{
  insertFile( name, true );
}

/** dialog for add files */
void Project::addFiles()
{
	KURL::List list = KFileDialog::getOpenURLs(
		basePath,	i18n("*"), this, i18n("Insert Files in Project"));

	QStringList files = list.toStringList();

	if ( files.isEmpty() ) return;

	QString t = files.first();
	if ( t.left(5)=="file:" ) t.remove(0,5);
	t = QExtFileInfo::toRelative( t, basePath );

	if ( t.left(2)=="..")
	{
		CopyTo *dlg = new CopyTo( basePath, this, i18n("Files: copy to project...") );

    if ( dlg->exec() ) files = dlg->copy( files );
    else {
    	delete dlg;
      return;
    }
    delete dlg;
	}

	insertFiles( files );

	emit reloadTree( fileNameList(), true, false );
}

void Project::addDirectory()
{
	QString dir = KFileDialog::getExistingDirectory(
		basePath, this, i18n("Insert Directory in Project"));

	addDirectory(dir);
}

void Project::addDirectory(QString rdir)
{
	addDirectory(rdir, true);
}

void Project::addDirectory(QString rdir, bool showDlg)
{
	if ( !hasProject() ) return;
	if ( rdir.isEmpty() ) return;

	if ( rdir.left(5) == "file:" ) rdir.remove(0,5);
	if ( rdir.right(1) != "/" ) rdir += "/";

	QString sdir = rdir;
  sdir = QExtFileInfo::toRelative( sdir, basePath);

  if ( sdir.left(2) == ".." )
  {
  	CopyTo *dlg = new CopyTo( basePath, this, i18n("%1: copy to project...").arg(rdir) );

    if ( (showDlg == false) ||
          (dlg->exec() ) )
    {
      //if ( rdir.right(1) == "/" ) rdir.remove( rdir.length()-1,1);
      rdir = dlg->copy( rdir );
      connect(dlg, SIGNAL(addFilesToProject(QString,CopyTo*)),
                   			  SLOT  (insertFilesAfterCopying(QString,CopyTo*)));
      return;
    }
    else {
    	delete dlg;
      return;
    }
    delete dlg;
  }

  if ( rdir.right(1) != "/" ) rdir += "/";

  insertFiles( rdir, "*" );
//And again, insert now directly the directory name into the project.
//It's important if rdir doesn't contain any files or subdirectories.
  insertFiles(rdir);
  emit reloadTree( fileNameList(), true, false );
}

void Project::insertFilesAfterCopying(QString rdir,CopyTo* dlg)
{
//The CopyTo dlg is deleted only here!!
  delete dlg;
  if ( rdir.right(1) != "/" ) rdir += "/";
  insertFiles( rdir, "*" );
  emit reloadTree( fileNameList(), true, false );
}

void Project::renameFinished()
{
  emit removeFromProject(0); //remove without confirm
  emit reloadTree( fileNameList(), false, false );
  emit newStatus();
}

void Project::slotRenameFile (QString oldName)
{
   QString newName;
   KLineEditDlg dlg(i18n("Enter new name:"), QFileInfo(oldName).fileName(), this);
   dlg.setCaption(i18n("Rename File"));

    if ( dlg.exec() )
    {
      newName = dlg.text();

      QString newFullName = QFileInfo(oldName).dirPath() + '/'+newName;
      if ( oldName != newFullName )
      {
         KURL oldUrl, newUrl;
         QuantaCommon::setUrl(oldUrl, oldName);
         QuantaCommon::setUrl(newUrl, newFullName);
         KIO::SimpleJob *job = KIO::rename( oldUrl, newUrl, true );
         connect( job, SIGNAL( result( KIO::Job *) ), SLOT( renameFinished() ));
      }

	  QDomElement el;
  	  QDomNodeList nl = dom.firstChild().firstChild().childNodes();

  	  for ( unsigned int i=0; i<nl.count(); i++ )
  	  {
  		el = nl.item(i).toElement();
  		if ( el.nodeName() == "item" )
      	if ( oldName == (basePath+el.attribute("url")))
      	{
     	   oldName =  QFileInfo(el.attribute("url")).dirPath();
      	    if (oldName != ".")
      	    {
      	    	newName = oldName+'/'+newName;
      	    }
  			el.setAttribute("url", newName );
      		modified = true;
			break;
      	 }
       }
    }
}

void Project::slotRenameFolder (QString oldName)
{
   QString newName;

   //do this trick to get the last directory name from oldName
   QString dirName = oldName;
   dirName.truncate(dirName.length()-1);

   KLineEditDlg dlg(i18n("Enter new name:"), QFileInfo(dirName).fileName(), this);
   dlg.setCaption(i18n("Rename Directory"));

   if ( dlg.exec() )
    {
      QString newName = QFileInfo(dirName).dirPath() + '/'+dlg.text()+'/';
      dirName += '/';

      if ( dirName != newName )
      {
         KURL oldUrl, newUrl;
         QuantaCommon::setUrl(oldUrl, dirName);
         QuantaCommon::setUrl(newUrl, newName);
         KIO::SimpleJob *job = KIO::rename( oldUrl, newUrl, true );
         connect( job, SIGNAL(result( KIO::Job *)), SLOT( renameFinished() ));
      }

	  QDomElement el;
  	  QDomNodeList nl = dom.firstChild().firstChild().childNodes();
  	  QString tmpString;

      newName = QExtFileInfo::toRelative( newName, basePath);
      oldName = QExtFileInfo::toRelative( oldName, basePath);
  	  for ( unsigned int i=0; i<nl.count(); i++ )
  	  {
  		el = nl.item(i).toElement();
  		if ( el.nodeName() == "item" )
  		{
  			tmpString = el.attribute("url");
  			tmpString.replace(QRegExp(oldName),newName);
  			el.setAttribute("url",tmpString);
  		}
       }
    	modified = true;
    }
}

void Project::slotRemoveFile(QString fname)
{
	QDomElement el;

  	QDomNodeList nl = dom.firstChild().firstChild().childNodes();

  	for ( unsigned int i=0; i<nl.count(); i++ )
  	{
  		el = nl.item(i).toElement();
  		if ( el.nodeName() == "item" )
      	if ( fname == (basePath+el.attribute("url")))
      	{
      		el.parentNode().removeChild( el );
      		modified = true;
      		emit newStatus();
      		return;
      	}
   }
}

void Project::slotRemoveFolder(QString fname)
{
	QDomElement  el;
	QDomNodeList nl = dom.firstChild().firstChild().childNodes();


	for ( unsigned int i=0; i<nl.count(); i++ )
  {
		el = nl.item(i).toElement();
		if ( el.nodeName() == "item" )
    	if ( !qstrncmp( fname, basePath+el.attribute("url"), fname.length() ) )
    	{
    		el.parentNode().removeChild( el );
    		modified = true;
    		i--;
    	}
  }
  emit newStatus();
}

/** create new project */
void Project::newProject()
{
	wiz = new QWizard( 0, "new", true);
	wiz->setMinimumSize(500,500);
	wiz->setMaximumSize(500,500);
	wiz->setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));

	png = new ProjectNewGeneral( wiz );

	stack = new QWidgetStack( wiz );

	pnl = new ProjectNewLocal( stack );
	pnw = new ProjectNewWeb  ( stack );
	pnf = new ProjectNewFinal( wiz );

	stack->addWidget( pnl, 0);
	stack->addWidget( pnw, 1 );

	wiz->addPage( png,   i18n("<b>General setting of project</b>"));
	wiz->addPage( stack, i18n("<b>Insert files in project</b>"));
	wiz->addPage( pnf,   i18n("<b>Some settings of project</b>"));

	wiz->setNextEnabled  ( png,   false );
	wiz->setBackEnabled  ( stack, true  );
	wiz->setNextEnabled  ( stack, true  );
	wiz->setNextEnabled  ( pnf, 	false );
	wiz->setFinishEnabled( pnf, 	true  );

	connect( png, SIGNAL(enableNextButton(QWidget *,bool)),
					 wiz, SLOT(setNextEnabled(QWidget*,bool)));
	connect( png, SIGNAL(enableNextButton(QWidget *,bool)),
					 pnl, SLOT(setDestDir(QWidget*,bool)));
	connect( png, SIGNAL(setBasePath(QString)),
					 pnw, SLOT(  setBasePath(QString)));
	connect( this,SIGNAL(setLocalFiles(bool)),
					 pnl, SLOT(setFiles(bool)));

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
  closeProject();

  name     = png->linePrjName->text();
  basePath = png->linePrjDir ->text();

  if ( basePath.right(1) != "/" )	basePath += "/";

  url  = KURL( basePath+png->linePrjFile->text());

  QExtFileInfo::createDir( basePath );

  createEmptyDom();

  name     = png->linePrjName->text();
  basePath = png->linePrjDir ->text();
  templateDir = png->linePrjTmpl->text();
  toolbarDir = png->linePrjToolbar->text();
  email    = png->lineEmail  ->text();
  author   = png->lineAuthor ->text();
  if ( basePath.right(1) != "/" )	basePath += "/";

  url  = KURL( basePath+png->linePrjFile->text());

  previewPrefix = pnf->linePrefix->text();
  if ( (previewPrefix.right(1) != "/") && (! previewPrefix.isEmpty()) ) previewPrefix += "/";
  usePreviewPrefix = pnf->checkPrefix->isChecked();

  QDomElement el;

  el = dom.firstChild().firstChild().toElement();
  el.setAttribute("type", png->type());
  el.setAttribute("name", name );
  el.setAttribute("previewPrefix", previewPrefix );
  el.setAttribute("usePreviewPrefix",usePreviewPrefix);

  el = dom.createElement("author");
  dom.firstChild().firstChild().appendChild( el );
  el.appendChild( dom.createTextNode( author ) );

  el = dom.createElement("email");
  dom.firstChild().firstChild().appendChild( el );
  el.appendChild( dom.createTextNode( email ) );

  el = dom.createElement("templates");
  dom.firstChild().firstChild().appendChild( el );
  el.appendChild( dom.createTextNode( templateDir ) );

  el = dom.createElement("toolbars");
  dom.firstChild().firstChild().appendChild( el );
  el.appendChild( dom.createTextNode( toolbarDir ) );

  QStringList list;
  if ( png->type() == "Local" ) list = pnl->files();
  if ( png->type() == "Web"   ) list = pnw->files();

  for ( QStringList::Iterator it = list.begin(); it != list.end(); ++it )
 {
	QString fname = QExtFileInfo::toRelative( *it, basePath );

	el = dom.createElement("item");
	el.setAttribute("url", fname );
    dom.firstChild().firstChild().appendChild( el );
 }

// el = dom.createElement("item");
// el.setAttribute("url","templates/");
// dom.firstChild().firstChild().appendChild(el);
//setup the templates directory
 bool  createTemplateDir = true;
 if (pnf->insertGlobalTemplates->isChecked())
 {
   addDirectory(qConfig.globalDataDir+"quanta/templates/", false);
   createTemplateDir = false;
 }
 if (pnf->insertLocalTemplates->isChecked())
 {
   addDirectory(locateLocal("data","quanta/templates/"), false);
   createTemplateDir = false;
 }

 if (createTemplateDir)
 {
// 	QDir dir(basePath);
// 	dir.mkdir("templates");
    QDir dir;
    dir.mkdir(templateDir, true);
 }


 QDir tDir;
 tDir.mkdir(toolbarDir, true);

 emit closeFiles();
 emit setBasePath( basePath );
 emit setProjectName( name );
 emit reloadTree( fileNameList(), true, false );
 emit   showTree();
 emit newProjectLoaded();

 modified = true;

 saveProject();
}

void Project::options()
{
	QTabDialog *dlg = new QTabDialog(0L, i18n("Project Options"), true);

	png = new ProjectNewGeneral( dlg );
	pnf = new ProjectNewFinal  ( dlg );

//	png ->setMargin(10);
//	pnf ->setMargin(10);

	png ->imagelabel->hide();
	pnf ->imagelabel->hide();

	png ->bGroupSources->hide();

	dlg->addTab( png, i18n("General") );
	dlg->addTab( pnf, i18n("Network") );

	dlg->setOkButton();
	dlg->setCancelButton();

	png->linePrjDir ->setEnabled( false );
	png->linePrjFile->setEnabled( false );
	png->buttonDir  ->setEnabled( false );

	png->linePrjDir ->setText( basePath );
	png->linePrjName->setText( name );
	png->linePrjFile->setText( url.url() );
  png->linePrjTmpl->setText( templateDir );
  png->linePrjToolbar->setText( toolbarDir );
	png->lineAuthor ->setText( author );
	png->lineEmail  ->setText( email );

  QDictIterator<DTDStruct> it(*dtds);
  for( ; it.current(); ++it )
  {
    if (it.current()->family == Xml)
    {
      int index = -1;
      if (it.current()->name == m_defaultDTD) index = 0;
      png->dtdCombo->insertItem(QuantaCommon::getDTDNickNameFromName(it.current()->name), index);
    }
  }

	pnf->linePrefix->setText(previewPrefix);
	pnf->checkPrefix->setChecked(usePreviewPrefix);

	if ( dlg->exec() )
	{
		name        	= png->linePrjName->text();
    templateDir = png->linePrjTmpl->text();
    toolbarDir = png->linePrjToolbar->text();
		author		= png->lineAuthor ->text();
		email			= png->lineEmail	->text();
    m_defaultDTD = QuantaCommon::getDTDNameFromNickName(png->dtdCombo->currentText());

    QDir dir;
    dir.mkdir(templateDir,true);
    dir.mkdir(toolbarDir,true);

		previewPrefix = pnf->linePrefix->text();
		if ( (previewPrefix.right(1) !="/") && (! previewPrefix.isEmpty()) )
		{
		   previewPrefix += "/";
		 }
		usePreviewPrefix = pnf->checkPrefix->isChecked();

		QDomElement el;

  		el = dom.firstChild().firstChild().toElement();
 		el.setAttribute("name",    name );
 		el.setAttribute("previewPrefix", previewPrefix );
 		el.setAttribute("usePreviewPrefix", usePreviewPrefix );

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
    if(el.isNull())
    {
      el = dom.createElement("templates");
      dom.firstChild().firstChild().appendChild(el);
      el.appendChild(dom.createTextNode(templateDir));
    }
    else
    {
      el.firstChild().setNodeValue(templateDir);
    }

    el = dom.firstChild().firstChild().namedItem("toolbars").toElement();
    if(el.isNull())
    {
      el = dom.createElement("toolbars");
      dom.firstChild().firstChild().appendChild(el);
      el.appendChild(dom.createTextNode(toolbarDir));
    }
    else
    {
      el.firstChild().setNodeValue(toolbarDir);
    }

 		modified = true;

		emit setProjectName( name );
    emit templateDirChanged( templateDir );
		emit newStatus();
	}

	delete dlg;
}

void Project::upload()
{
	emit saveAllFiles();

  ProjectUpload *dlg = new ProjectUpload(this, 0,i18n("Upload project's files..."), false, Qt::WDestructiveClose);
  dlg->show();
  //ProjectUpload *dlg = new ProjectUpload(this, 0,i18n("Upload project's files..."), true);
  //dlg->exec();
  //delete dlg;
 }

void Project::uploadFile(QString file)
{
	emit saveAllFiles();
	QString fname = QExtFileInfo::toRelative( file, basePath);

	ProjectUpload *dlg = new ProjectUpload(fname, this, 0, i18n("Upload project's files..."), false, Qt::WDestructiveClose);
  dlg->show();
  //ProjectUpload *dlg = new ProjectUpload(fname, this, 0, i18n("Upload project's files..."));
  //dlg->exec();
  //delete dlg;
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
	RescanPrj *dlg = new RescanPrj( fileNameList(), basePath, this, i18n("New files in project's dir..."));
	if ( dlg->exec() )
	{
	  insertFiles( dlg->files() );
      emit reloadTree( fileNameList(true), true, false );
	}
	delete dlg;
}
/** Returns the relative url with the prefix inserted. */
QString Project::urlWithPrefix(const KURL& url)
{
    QString returnUrl;

    if (usePreviewPrefix)
    {
    	QString tempUrl = QExtFileInfo::toRelative(url.prettyURL(), basePath );
    	if (tempUrl.left(2) != ".." ) returnUrl = previewPrefix + tempUrl;
    } else
    {
       returnUrl = url.url();
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

#include "project.moc"
