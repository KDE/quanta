/***************************************************************************
                          project.cpp  -  description
                             -------------------
    begin                : Thu Mar 16 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavsky
    email                : pdima@mail.univ.kiev.ua
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
#include <qfileinfo.h>
#include <qtabdialog.h>
#include <qfiledialog.h>
#include <qtextstream.h>
#include <qstringlist.h>
#include <qradiobutton.h>
#include <qwidgetstack.h>
#include <qbuttongroup.h>
#include <qurloperator.h>

// include files for KDE
#include <kurl.h>
#include <kfile.h>
#include <kwizard.h>
#include <klocale.h>
#include <kaction.h>
#include <ktempfile.h>
#include <kstdaction.h>
#include <kfiledialog.h>
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

extern QString fileMaskHtml;
extern QString fileMaskJava;
extern QString fileMaskText;
extern QString fileMaskImage;

Project::Project( QWidget *, const char *name )
        : QWidget(0L,name)
{
  this->name=QString::null;
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
    CopyTo *dlg = new CopyTo( basePath, this, rname+": copy to project..." );

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

  if ( repaint ) emit reloadTree( fileNameList(), false, false );
}


/** insert files from dir recursive */
void Project::insertFiles( QString path, QString mask )
{
	insertFiles( QExtFileInfo::allFiles( path, mask ));
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
  	el = dom.createElement("item");
  	el.setAttribute("url", *it );
  	dom.firstChild().firstChild().appendChild( el );
  }

  emit reloadTree( fileNameList(), true, false );
}

void Project::createEmptyDom()
{
#warning fix  
  QFile f( url.url() );
  if ( !f.open( IO_WriteOnly ) )
  {
    KMessageBox::sorry(this, i18n("Can't open file %s for IO_WriteOnly").arg(url.url()));
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
  config->setGroup  ("Projects");
  QString url = config->readEntry("Last Project");
  projectRecent->loadEntries(config, "RecentProjects");
  
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
    
    projectRecent->addURL( url );
  }
  
  emit newStatus();
}

void Project::openProject(const KURL &url)
{
  if ( url.url().isEmpty() ) return;
  
  closeProject();
  loadProject ( url );
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
    fn = url.url();
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
  
  emit newStatus();
  
  return true;
}

/** close project and edited files */
void Project::closeProject()
{
  //fix: add save/no for remote
  saveProject();

  dom.clear();

  emit closeFiles();

  emit setBasePath		( basePath );
  emit setProjectName	( i18n( "No project" ) );
  emit reloadTree 		( fileNameList(), true, false );

  url  = KURL();
  name = QString::null;
  
  emit newStatus();
}

/** load project from file: name */
void Project::loadProject(const KURL &url)
{
  KURL u(url);

  this->url =KURL();
  this->name=QString::null;
  
  if (u.isMalformed()) 
  {
      QString s = i18n("Malformed URL\n%1").arg(url.prettyURL());
      KMessageBox::sorry(this, s);
      return;
  }  

  if ( !url.isLocalFile() )
  {
    emit statusMsg(i18n("Loading project %s").arg(url.url()));

    // clear
    QByteArray b;
    buff.setBuffer(b);
    
    KIO::Job *job = KIO::get( url );

    connect( job, SIGNAL( result( KIO::Job * ) ), this, SLOT( slotProjectReadFinish(KIO::Job *)));
    connect( job, SIGNAL( data(KIO::Job *,const QByteArray &)), this, SLOT( slotProjectReadData(KIO::Job *,const QByteArray &)));
  
    this->url = url;
    
    QString s = url.url();
    s.remove(s.findRev('/')+1,s.length());
    
    basePath = s;
  }
  else 
  {
    QString fn = url.url();
    
    if ( fn.left(5) == "file:" ) fn.remove(0,5);
    
    QFile       f( fn );
    QFileInfo   fi( fn );
    
    basePath = fi.dirPath();
    if ( basePath.right(1) != "/" )	basePath += "/";
    
    if ( !f.exists() )          { KMessageBox::sorry( this, i18n("File don't exists") );return;}
    if ( !f.open(IO_ReadOnly))  { KMessageBox::sorry( this, i18n("Can't open for IO_ReadOnly") );return;}
    if ( !dom.setContent( &f )) { KMessageBox::sorry( this, i18n("Not found XML info in file") );return;}
  
    this->url = url;
    
    loadProjectXML();
  }
}

void Project::slotProjectReadFinish(KIO::Job *job)
{
  if ( job->error() ) job->showErrorDialog();
  
  QString s(buff.buffer());
  
  if ( !dom.setContent( s )) { KMessageBox::sorry( this, i18n("Not found XML info in file") );return;}
  
  loadProjectXML();
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
	
  if ((no=dom.firstChild().firstChild()).isNull())      { KMessageBox::sorry( this, i18n("Wrong project's file") );return;}
  if ((name=no.toElement().attribute("name")).isNull()) { KMessageBox::sorry( this, i18n("Wrong project's file") );return;}
	
  QString s = no.toElement().attribute("preview");
  if ( s.right(1) != "/" && !s.isEmpty() ) s+="/";
  previewPrefix = s;
		
  QDomNodeList nl = dom.firstChild().firstChild().childNodes();
	
  for ( unsigned int i=0; i<nl.count(); i++ )
  {
    el = nl.item(i).toElement();
		
  	if ( el.nodeName() == "openfile" ) 
  	{
  		KURL u(el.attribute("url"));
  		emit openFile( u );
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

  emit setBasePath		( basePath );
  emit setProjectName	( name );
  emit reloadTree 		( fileNameList(true), true, false );
  
  emit showTree();
  emit newStatus();
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
		basePath,	i18n("*"), this, i18n("Insert files in project..."));
		
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
	
	emit reloadTree 		( fileNameList(), true, false );
}

void Project::addDirectory()
{
	QString dir = KFileDialog::getExistingDirectory(
		basePath, this, i18n("Insert directory in project..."));
	
	addDirectory(dir);
}

void Project::addDirectory(QString rdir)
{
	if ( !hasProject() ) return;
	if ( rdir.isEmpty() ) return;
	
	if ( rdir.left(5) == "file:" ) rdir.remove(0,5);
	if ( rdir.right(1) != "/" ) rdir += "/";
	
	QString sdir = rdir;
  sdir = QExtFileInfo::toRelative( sdir, basePath);
	
  if ( sdir.left(2) == ".." )
  {
  	CopyTo *dlg = new CopyTo( basePath, this, rdir+i18n(": copy to project...") );

    if ( dlg->exec() )
    { 
      if ( rdir.right(1) == "/" ) rdir.remove( rdir.length()-1,1);
      rdir = dlg->copy( rdir );
      connect(dlg, SIGNAL(addFilesToProject(QString)),
                   SLOT  (insertFilesAfterCopying(QString)));
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
}

void Project::insertFilesAfterCopying(QString rdir)
{
  if ( rdir.right(1) != "/" ) rdir += "/";
  insertFiles( rdir, "*" );
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
    		i--;
    	}
  }
}

/** create new project */
void Project::newProject()
{
	wiz = new QWizard( 0, "new", true);
	wiz ->resize(500,400);
	
	png = new ProjectNewGeneral( wiz );
	
	stack = new QWidgetStack( wiz );
	
	pnl = new ProjectNewLocal( stack );
	pnw = new ProjectNewWeb  ( stack );
	pnf = new ProjectNewFinal( wiz );
	
	stack->addWidget( pnl, 0);
	stack->addWidget( pnw, 1 );
	
	wiz->addPage( png,   i18n("<b>General setting of project...</b>"));
	wiz->addPage( stack, i18n("<b>Insert files in project...</b>"));
	wiz->addPage( pnf,   i18n("<b>Some settings of project...</b>"));
	
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
  
  name = png->linePrjName->text();
	url  = KURL( basePath+png->linePrjFile->text());

  createEmptyDom();
	
	email = png->lineEmail->text();
	author = png->lineAuthor->text();
	basePath = png->linePrjDir->text();
	if ( basePath.right(1) != "/" )	basePath += "/";
	
	QExtFileInfo::createDir( basePath );
	
	name = png->linePrjName->text();
	url  = KURL( basePath+png->linePrjFile->text());
	
	if ( pnf->checkPrefix->isChecked() ) 
	{
		previewPrefix = pnf->linePrefix->text();
		if ( previewPrefix.right(1) != "/" ) previewPrefix+="/";
	}
	else
		previewPrefix = QString::null;
	
  QDomElement el;

  el = dom.firstChild().firstChild().toElement();
  el.setAttribute("type", png->type());
  el.setAttribute("name", name );
  el.setAttribute("preview", previewPrefix );

  el = dom.createElement("author");
  dom.firstChild().firstChild().appendChild( el );
  el.appendChild( dom.createTextNode( author ) );

  el = dom.createElement("email");
  dom.firstChild().firstChild().appendChild( el );
  el.appendChild( dom.createTextNode( email ) );

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
	
	emit closeFiles();
	emit setBasePath( basePath );
	emit setProjectName( name );
	emit reloadTree( fileNameList(), true, false );
	emit   showTree();
	
	saveProject();
}

void Project::options()
{
	QTabDialog *dlg = new QTabDialog(0L, i18n("Project options"), true);

	png = new ProjectNewGeneral( dlg );
	pnf = new ProjectNewFinal  ( dlg );
	
	png ->setMargin(10);
	pnf ->setMargin(10);
	
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
	png->lineAuthor ->setText( author );
	png->lineEmail  ->setText( email );
	
	pnf->linePrefix ->setText( previewPrefix );
	if ( !previewPrefix.isEmpty() ) pnf->checkPrefix->setChecked(true);
	
	if ( dlg->exec() )
	{
		name        	= png->linePrjName->text();
		author				= png->lineAuthor ->text();
		email					= png->lineEmail	->text();
		
		if ( pnf->checkPrefix->isChecked() && !pnf->linePrefix ->text().isEmpty() )
		{
			previewPrefix = pnf->linePrefix ->text();
			if ( previewPrefix.right(1) != "/") previewPrefix+="/";
		}
		else
			previewPrefix = QString::null;
		
		QDomElement el;

  	el = dom.firstChild().firstChild().toElement();
  	el.setAttribute("name",    name );
  	el.setAttribute("preview", previewPrefix );
		
		emit setProjectName( name );
	}
	
	delete dlg;
}

void Project::upload()
{
	emit saveAllFiles();
	
	ProjectUpload *dlg = new ProjectUpload(this, 0,i18n("Upload project's files..."), true);
	dlg->exec();
	delete dlg;
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
	}
	delete dlg;
}
