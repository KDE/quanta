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
#include <qmessagebox.h>

// include files for KDE
#include <kurl.h>
#include <kfile.h>
#include <kwizard.h>
#include <klocale.h>
#include <kfiledialog.h>

// application headers
#include "copyto.h"
#include "../qextfileinfo.h"
#include "projectnewgeneral.h"
#include "projectnewlocal.h"
#include "projectnewweb.h"
#include "projectnewfinals.h"
#include "projectupload.h"
#include "rescanprj.h"

extern QString fileMaskHtml;
extern QString fileMaskJava;
extern QString fileMaskText;
extern QString fileMaskImage;

Project::Project( QWidget *, const char *name )
        : QWidget(0L,name)
{
}

Project::~Project()
{
  if ( hasProject() ) saveProject();
}

bool Project::hasProject(){
  return ( !projectFileName.isNull() );
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
		    if ( fi.exists() ) list.append( el.attribute("url") );
		  }
		}
  }
	return list;
}

void Project::insertFile( QString rname, bool repaint )
{
	if ( !hasProject() ) return;
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
	if ( !hasProject() ) return;
	insertFiles( QExtFileInfo::allFiles( path, mask ));
}

/** insert files */
void Project::insertFiles( QStringList files )
{
  if ( !hasProject() ) return;

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
	if ( !hasProject()  ) return;

  QFile f( projectFileName );
  if ( !f.open( IO_WriteOnly ) )
  {
    QMessageBox::critical( this, i18n( "ERROR" ), i18n("Can't open file for IO_ReadWrite | IO_Truncate"));
    return;
  }

  QTextStream qts( &f );

  qts << "<!DOCTYPE webproject ><webproject>" << endl;
  qts << "\t<project name=\"" << projectName << "\">" << endl;
  qts << "\t\t<upload />" << endl;
  qts << "\t</project>" << endl;
  qts << "</webproject>" << endl;

  f.close();
  
  loadProject( projectFileName );
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
// SLOTS implementation
//

/** open project file */
void Project::openProject()
{
  QString fileToOpen = KFileDialog::getOpenFileName( QDir::homeDirPath(),
                       i18n("*.webprj|Project files\n*|All files"), this, i18n("Open Project..."));

  if( !fileToOpen.isEmpty() )
  {
    closeProject();
    loadProject( fileToOpen );
    emit addRecentProject( fileToOpen );
  }
}

/** save project file */
bool Project::saveProject()
{
  if ( !hasProject()  ) return;

  QFile f( projectFileName );
  if ( !f.open( IO_ReadWrite | IO_Truncate ) )
  {
    return false;
  }

  QTextStream qts( &f );

  emit requestOpenedFiles();

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
	// insert new opened files
	QStringList::Iterator it;
  for ( it = openedFiles.begin(); it != openedFiles.end(); ++it )
	{
  	el = dom.createElement("openfile");
  	el.setAttribute("url", *it );

  	dom.firstChild().firstChild().appendChild( el );
	}
	
  dom.save( qts, 0);
  return true;
}

/** close project and edited files */
void Project::closeProject()
{
  if ( hasProject() ) saveProject();

  dom.clear();

  emit closeFiles();

  emit setBasePath		( basePath );
  emit setProjectName	( i18n( "No project" ) );
  emit reloadTree 		( fileNameList(), true, false );

  projectFileName = QString::null;
}

/** load project from file: name */
void Project::loadProject(QString fname)
{
  QFile f( fname );
  QFileInfo fi( fname );
  QDomNode n_prj;
  QDomElement el;

  basePath = fi.dirPath();
  if ( basePath.right(1) != "/" )	basePath += "/";

  if ( !f.exists() ) { QMessageBox::critical( this, i18n( "ERROR" ), i18n("File don't exists") );return;}
	if ( !f.open(IO_ReadOnly) ) { QMessageBox::critical( this, i18n( "ERROR" ), i18n("Can't open for IO_ReadOnly") );return;}
	if ( !dom.setContent( &f ) ) { QMessageBox::critical( this, i18n( "ERROR" ), i18n("Not found XML info in file") );return;}
	if ( (n_prj=dom.firstChild().firstChild()).isNull() )	{ QMessageBox::critical( this, i18n( "ERROR" ), i18n("Wrong project's file") );return;}
	if ( (projectName=n_prj.toElement().attribute("name")).isNull() ) { QMessageBox::critical( this, i18n( "ERROR" ), i18n("Wrong project's file") );return;}
	
	previewPrefix = n_prj.toElement().attribute("preview");
	if ( previewPrefix.right(1) != "/" && !previewPrefix.isEmpty() ) previewPrefix+="/";
		
	QDomNodeList nl = dom.firstChild().firstChild().childNodes();
	
	for ( unsigned int i=0; i<nl.count(); i++ )
  {
		el = nl.item(i).toElement();
		
  	if ( el.nodeName() == "openfile" )
  		emit openFile( el.attribute("url") );
  		
  	if ( el.nodeName() == "item" )
  	{
  		QFileInfo fi( basePath+el.attribute("url") );
  		if ( !fi.exists() ) {
  			el.parentNode().removeChild( el );
    		i--;
  		}
  	}
  }

  projectFileName = fname;

  emit setBasePath		( basePath );
  emit setProjectName	( projectName );
	emit reloadTree 		( fileNameList(true), true, false );
}

// slot for insert file
void Project::insertFile( QString name )
{
  insertFile( name, true );
}

/** dialog for add files */
void Project::addFiles()
{
	if ( !hasProject() ) return;
	
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
	if ( !hasProject() ) return;
	
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

/** for receive from quantadoc */
void Project::slotOpenedFiles(QStringList list)
{
	openedFiles = list;
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
	
	pnf = new ProjectNewFinalS( wiz );
	pnf ->imagelabel->setPixmap( UserIcon("wiznewprjfin") );
	
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
  
  projectName = png->linePrjName->text();
	projectFileName = basePath+png->linePrjFile->text();

  createEmptyDom();
  openedFiles.clear();
	
	email = png->lineEmail->text();
	author = png->lineAuthor->text();
	basePath = png->linePrjDir->text();
	if ( basePath.right(1) != "/" )	basePath += "/";
	
	QExtFileInfo::createDir( basePath );
	
	projectName = png->linePrjName->text();
	projectFileName = basePath+png->linePrjFile->text();
	
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
  el.setAttribute("name", projectName );
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
	emit setProjectName( projectName );
	emit reloadTree( fileNameList(), true, false );
	
	saveProject();
}

void Project::options()
{
	if ( !hasProject() ) return;
	
	QTabDialog *dlg = new QTabDialog(0L, i18n("Project options"), true);

	png = new ProjectNewGeneral( dlg );
	pnf = new ProjectNewFinalS ( dlg );
	pnf ->imagelabel->setPixmap( UserIcon("wiznewprjfin") );
		
	dlg->addTab( png, i18n("General") );
	dlg->addTab( pnf, i18n("Network") );
	
	dlg->setOkButton();
	dlg->setCancelButton();
	
	png->linePrjDir ->setEnabled( false );
	png->linePrjFile->setEnabled( false );
	png->buttonDir  ->setEnabled( false );
	
	delete png->ButtonGroup1_2;
	
	png->linePrjDir ->setText( basePath );
	png->linePrjName->setText( projectName );
	png->linePrjFile->setText( projectFileName );
	png->lineAuthor ->setText( author );
	png->lineEmail  ->setText( email );
	
	pnf->linePrefix ->setText( previewPrefix );
	if ( !previewPrefix.isEmpty() ) pnf->checkPrefix->setChecked(true);
	
	if ( dlg->exec() )
	{
		projectName 	= png->linePrjName->text();
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
  	el.setAttribute("name",    projectName );
  	
  	el.setAttribute("preview", previewPrefix );
		
		emit setProjectName( projectName );
	}
	
	delete dlg;
}

void Project::upload()
{
	if ( !hasProject() ) return;
	
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
  if ( !hasProject() ) return;
	
	RescanPrj *dlg = new RescanPrj( fileNameList(), basePath, this, i18n("New files in project's dir..."));
	if ( dlg->exec() )
	{
	  insertFiles( dlg->files() );
	}
	delete dlg;
}
