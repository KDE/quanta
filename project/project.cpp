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
#include <qcheckbox.h>
#include <qfileinfo.h>
#include <qfiledialog.h>
#include <qtextstream.h>
#include <qstringlist.h>
#include <qradiobutton.h>
#include <qwidgetstack.h>

// include files for KDE
#include <kurl.h>
#include <kfile.h>
#include <kwizard.h>
#include <klocale.h>
#include <kprocess.h>
#include <kio/job.h>
#include <kfiledialog.h>

// application headers
#include "copyto.h"
#include "../qextfileinfo.h"
#include "projectnewgeneral.h"
#include "projectnewlocal.h"
#include "projectnewwebs.h"

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

QStringList Project::fileNameList()
{
	QStringList list;
		
	QDomNodeList nl = dom.firstChild().firstChild().childNodes();
	
	for ( unsigned int i=0; i<nl.count(); i++ )
  {
		QDomElement el = nl.item(i).toElement();
		if ( el.nodeName() == "item" )	list.append( el.attribute("url") );
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

    if ( dlg->exec() ) fname = dlg->copy( rname );
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
  f.open( IO_ReadWrite | IO_Truncate );

  QTextStream qts( &f );

  qts << "<!DOCTYPE webproject ><webproject>" << endl;
  qts << "\t<project name=\"" << projectName << "\">" << endl;
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
void Project::saveProject()
{
  if ( !hasProject()  ) return;

  QFile f( projectFileName );
  f.open( IO_ReadWrite | IO_Truncate );

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

  if ( !f.exists() ) return;
	if ( !f.open(IO_ReadOnly) ) return;
	if ( !dom.setContent( &f ) ) return;
	if ( (n_prj=dom.firstChild().firstChild()).isNull() )	return;
	if ( (projectName=n_prj.toElement().attribute("name")).isNull() ) return;
		
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
	emit reloadTree 		( fileNameList(), true, false );
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
	
	QString sdir = rdir;
	if ( sdir.left(5) == "file:" ) sdir.remove(0,5);

  sdir = QExtFileInfo::toRelative( sdir, basePath);
	
  if ( sdir.left(2) == ".." )
  {
  	CopyTo *dlg = new CopyTo( basePath, this, rdir+i18n(": copy to project...") );

    if ( dlg->exec() ) rdir = dlg->copy( rdir );
    else {
    	delete dlg;
      return;
    }
    delete dlg;
  }
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
	
	png = new ProjectNewGeneral( wiz );
	
	stack = new QWidgetStack( wiz );
	
	pnl = new ProjectNewLocal( stack );
	pnw = new ProjectNewWebS ( stack );
	
	stack->addWidget( pnl, 0);
	stack->addWidget( pnw, 1 );
	
	wiz->addPage( png,   i18n("General setting of project..."));
	wiz->addPage( stack, i18n("Insert files in project..."));
	
	wiz->setNextEnabled  ( png,   false );
	wiz->setBackEnabled  ( stack, true  );
	wiz->setFinishEnabled( stack, true  );
	
	connect( png, SIGNAL(enableNextButton(QWidget *,bool)),
					 wiz, SLOT(setNextEnabled(QWidget*,bool)));
	connect( png, SIGNAL(enableNextButton(QWidget *,bool)),
					 pnl, SLOT(setDestDir(QWidget*,bool)));
	connect( this,SIGNAL(setLocalFiles(bool)),
					 pnl, SLOT(setFiles(bool)));	
	
	connect( wiz, SIGNAL(selected(const QString &)),
					this, SLOT  (slotSelectProjectType(const QString &)));

	if ( wiz->exec() ) slotAcceptCreateProject();
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
  projectName = png->linePrjName->text();
	projectFileName = basePath+png->linePrjFile->text();

  createEmptyDom();
  openedFiles.clear();
	
	email = png->lineEmail->text();
	author = png->lineAuthor->text();
	basePath = png->linePrjDir->text();
	if ( basePath.right(1) != "/" )	basePath += "/";
	
	projectName = png->linePrjName->text();
	projectFileName = basePath+png->linePrjFile->text();
	
  QDomElement el;

  el = dom.firstChild().firstChild().toElement();
  el.setAttribute("type", png->type());
  el.setAttribute("name", projectName );

  el = dom.createElement("author");
  dom.firstChild().firstChild().appendChild( el );
  el.appendChild( dom.createTextNode( author ) );

  el = dom.createElement("email");
  dom.firstChild().firstChild().appendChild( el );
  el.appendChild( dom.createTextNode( email ) );

  QStringList list;
  if ( png->type() == "Local" ) list = pnl->files();

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

/** for ok button on new project wizard */
/*
void Project::acceptNewPrj()
{

    if ( !QString(pageNewPrj2->lineSiteUrl->text()).isEmpty() )
    {
      emit selectMessageWidget();
        
      chdir( basePath.data() );
      
      KProcess *proc = new KProcess();

      proc -> clearArguments();

      *proc << "wget";
      *proc << "-c" << "-np" << "-r" << "--level=5" << "-nH";
      *proc << pageNewPrj2->lineSiteUrl->text();

      connect( proc, SIGNAL(receivedStdout(   KProcess*,char*,int)), this,
                     SLOT(  slotGetWgetOutput(KProcess*,char*,int)));
      connect( proc, SIGNAL(receivedStderr(   KProcess*,char*,int)), this,
                     SLOT(  slotGetWgetOutput(KProcess*,char*,int)));
      connect( proc, SIGNAL(processExited(    KProcess*)), this,
                     SLOT(  slotGetWgetExited(KProcess*)));
      proc->start(KProcess::NotifyOnExit, KProcess::AllOutput);
    }

}
*/
void Project::slotGetWgetExited(KProcess*)
{
	emit disableMessageWidget();
}

void Project::slotGetWgetOutput(KProcess *, char *buffer, int buflen)
{
  QString output = buffer;
  output = output.left( buflen );

  emit messages(output);
  
  int pos;
  while ( (pos = output.find("saved")) != -1 )
  {
    int begName = output.findRev('`',pos);
    if ( begName == -1 ) {
      output = output.remove(0,pos+1);
      continue;
    }
    
    int endName = output.find('\'',begName);
    if ( endName == -1 || endName > pos ) {
      output = output.remove(0,pos+1);
      continue;
    }
  
    QString fileName = output.left(endName);
    fileName = fileName.right( endName - begName-1);
  
    insertFile( basePath+fileName );
  
    output = output.remove(0,pos+1);
  }

  emit reloadTree( fileNameList(), false, true );
}

