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
#include <qfileinfo.h>
#include <qfiledialog.h>
#include <qstringlist.h>
/*
#include <qtabdialog.h>
#include <qlistview.h>
*/

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
#include "projectnewgetfiles.h"

extern QString fileMaskHtml;
extern QString fileMaskJava;
extern QString fileMaskText;
extern QString fileMaskImage;

Project::Project( QWidget *, const char *name )
        : QWidget(0L,name)
{
  projectFile = 0L;
}

Project::~Project()
{
  //saveProject();
}

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

/** load project from file: name */
void Project::loadProject(QString name)
{
  QFileInfo f( name );
  if ( !f.exists() ) return;

  projectFileName = name;
  basePath 		= f.dirPath();
  QStringList fileNList;
  QStringList ofileList;

  if ( basePath.right(1) )	basePath += "/";

  projectFile = new KConfig( name );

  projectFile	->setGroup("Ftp");
  host      	= projectFile->readEntry("Host");
  remoteDir 	= projectFile->readEntry("Remote Dir");
  login     	= projectFile->readEntry("Login");

  projectFile 	->setGroup("General Options");
  fileNList			= projectFile->readListEntry("Files");
  baseURL     	= projectFile->readEntry("Base URL");
  projectName 	= projectFile->readEntry("Name");
  ofileList			= projectFile->readListEntry("Open Files");


  QStringList::Iterator it;

  for ( it = fileNList.begin(); it != fileNList.end(); ++it )
	{
  	QString fname = basePath + *it;
    QFileInfo fi(fname);

    if ( fi.exists() && fi.isReadable() && fi.isFile() )
    {
	  	ProjectItem *file = new ProjectItem( basePath );

	    projectFile	->setGroup( *it );
  	  file->URL   = projectFile->readEntry("URL");
  	  file->time  = (time_t) projectFile->readLongNumEntry("Time",0);
  	  file->fname = *it;
  	
    	fileList.append( file );
    }
  }

  emit setBasePath		( basePath );
  emit setProjectName	( projectName );

	for ( it = ofileList.begin(); it != ofileList.end(); ++it )
    emit openFile( *it );

	emit reloadTree 		( fileNameList(), true, false );
}

/** save project file */
void Project::saveProject()
{

  ProjectItem *item;

  if ( projectFileName.isEmpty()  ) return;

  QFileInfo fi( projectFileName );
  QFile  pFile;

  if ( !fi.isWritable() || !fi.isFile() ) return;

  if ( !QFile::remove( projectFileName )) return;

  if ( !projectFile ) return;
  delete projectFile;

  projectFile = new KConfig( projectFileName );

  fileList.first();

  while ( ( item = fileList.current() ) )
  {
    projectFile->setGroup( item->fname );
    projectFile->writeEntry("URL", item->URL );
    projectFile->writeEntry("Time", (long int)item->time );

    fileList.next();
  }

  emit requestOpenedFiles();

  projectFile->setGroup("General Options");
  projectFile->writeEntry("Files", 			fileNameList() );
  projectFile->writeEntry("Base URL", 	baseURL );
  projectFile->writeEntry("Name", 			projectName );
  projectFile->writeEntry("Open Files", openedFiles );

  projectFile->setGroup("Ftp");
  projectFile->writeEntry( "Host", 			host);
  projectFile->writeEntry( "Remote Dir",remoteDir);
  projectFile->writeEntry( "Login", 		login);

  projectFile->sync();
}

/** list of all project's files */
QStringList Project::fileNameList()
{
	QStringList list;
	ProjectItem *i;
	
	for ( i = fileList.first(); i; i = fileList.next() )
		list.append( i->fname );
		
	return list;
}

/** close */
void Project::closeProject()
{
  if ( projectFile )
  {
    saveProject();
    delete projectFile;
  }

  fileList.clear();

  emit closeFiles();

  emit setBasePath		( basePath );
  emit setProjectName	( i18n( "No project" ) );
  emit reloadTree 		( fileNameList(), true, false );

  projectFile = 0L;
}

/** dialog for add files */
void Project::addFiles()
{
	if ( !hasProject() ) return;
	
	QStringList files;
	KURL::List list = KFileDialog::getOpenURLs(
		basePath,	i18n("*"), this, i18n("Insert files in project..."));
		
	files = list.toStringList();
	
	if ( files.isEmpty() ) return;
	QStringList::Iterator it;
	
	it = files.begin();
	
	QString f = *it;
	if ( f.left(5) == "file:" ) f.remove(0,5);

  QString fn = QExtFileInfo::toRelative( f, basePath);
  QString sn = fn;
	
  if ( fn.left(2) == ".." )
  {
    QString caption = sn;
    caption += ": copy to project directory...";
    CopyTo *dlg = new CopyTo( basePath, this, caption );

    if ( dlg->exec() )
    {
    	QString path = dlg->lineDir->text();
      if ( path.right(1) != "/" ) path.append("/");

      QDir dir( path);
      int i = 10; while ( !dir.exists() && i-- ) dir.mkdir( path);

      QString dest = path;

      KURL::List  list( files );
      KIO ::copy( list, KURL( dest ) );

      for ( it = files.begin(); it != files.end(); ++it )
      {
      	QString fn = *it;
      	while ( (i = fn.find('/')) >= 0 ) fn.remove(0,i+1);
      	sn = fn;
      	
      	fn = QExtFileInfo::toRelative( dest+fn, basePath);
      	
      	ProjectItem *it = new ProjectItem( basePath );
			  it -> URL   = baseURL + fn;
			  it -> fname = fn;
			
			  insertFile( it );
      }
      emit reloadTree 		( fileNameList(), true, false );
      return;
    }
    else return;
  }
	
	for ( it = files.begin(); it != files.end(); ++it )
	{
		insertFile( *it );
	}
	emit reloadTree 		( fileNameList(), true, false );
}

void Project::addDirectory()
{
	if ( !hasProject() ) return;
	
	QString dir = KFileDialog::getExistingDirectory(
		basePath, this, i18n("Insert directory in project..."));
	
	addDirectory(dir);
}

void Project::addDirectory(QString dir)
{
	if ( !hasProject() ) return;
	if ( dir.isEmpty() ) return;
	
	QString f = dir;
	if ( f.left(5) == "file:" ) f.remove(0,5);

  QString fn = QExtFileInfo::toRelative( f, basePath);
  QString sn = fn;
	
  if ( fn.left(2) == ".." )
  {
    QString caption = sn;
    caption += ": copy to project directory...";
    CopyTo *dlg = new CopyTo( basePath, this, caption );

    if ( dlg->exec() )
    {
    	QString path = dlg->lineDir->text();
      if ( path.right(1) != "/" ) path.append("/");

      QDir dir( path);
      int i = 10; while ( !dir.exists() && i-- ) dir.mkdir( path);

      QString dest = path;
      if ( f.right(1) == "/" ) f.remove( f.length()-1, 1 );

      KIO::Job *job = KIO::copy( KURL( f ), KURL( dest ) );
      connect( job, SIGNAL( result( KIO::Job *) ), this , SLOT( slotInsertDirFinished( KIO::Job *)));

      while ( (i = f.find('/')) >= 0 ) f.remove(0,i+1);
      if ( f.right(1) != "/" ) f += "/";
      tmppath = dest + f;

      return;
    }
    else return;
  }
  if ( f.right(1) != "/" ) f += "/";
  insertFilesRecursive( f, false, 0 );
  emit reloadTree 		( fileNameList(), true, false );
}

void Project::slotInsertDirFinished( KIO::Job *)
{
	insertFilesRecursive( tmppath, false, 0 );
  emit reloadTree 		( fileNameList(), true, false );
}

/** for receive from quantadoc */
void Project::slotOpenedFiles(QStringList list)
{
	openedFiles = list;
}

/** create new project */
void Project::newProject()
{

  wizardNewPrj = new KWizard(0L,"New project", true);

  pageNewPrj1  = new ProjectNewGeneral (wizardNewPrj);
  pageNewPrj2  = new ProjectNewGetFiles(wizardNewPrj);
//  pageNewPrj3  = new PrjPropertiesFTP(wizardNewPrj);

  wizardNewPrj ->addPage( pageNewPrj1, "General");
  wizardNewPrj ->addPage( pageNewPrj2, "Project Files");

  pageNewPrj1->lineAuthor ->setText(author);
  pageNewPrj1->lineEmail  ->setText(email);
  pageNewPrj1->lineBaseURL->setText("http://");

  wizardNewPrj->setFinishEnabled( pageNewPrj2, true );

  connect( wizardNewPrj->finishButton(), SIGNAL(clicked()),
  				this, SLOT(acceptNewPrj()));

  wizardNewPrj->resize( 460,460 );
  wizardNewPrj->show();
}

/** for ok button on new project wizard */
void Project::acceptNewPrj()
{
  QString fname = pageNewPrj1->linePrjFile->text();

  if ( !fname.isEmpty() )
  {
    basePath        = pageNewPrj1->linePrjDir->text();
    baseURL         = pageNewPrj1->lineBaseURL->text();

    if ( basePath.right(1) != "/" ) basePath.append("/");

    fname = basePath + fname;

    QDir prjDir( basePath );

    if ( !prjDir.exists() )
      prjDir.mkdir( basePath );

    closeProject();
    projectFile     = new KConfig( fname );
    projectName     = pageNewPrj1->linePrjName->text();
    projectFileName = fname;

    projectFile->setGroup("General Options");
	  projectFile->writeEntry("Name", 			projectName );
    projectFile->sync();
/*
    host        = pageNewPrj3->lineHost->text();
    remoteDir   = pageNewPrj3->lineRDir->text();
    login       = pageNewPrj3->lineLogin->text();
*/
    if ( pageNewPrj2->checkInsertFiles->isChecked() )
      insertFilesRecursive( basePath, pageNewPrj2->checkWebFiles->isChecked() );

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

    emit setBasePath		( basePath );
	  emit setProjectName	( projectName );
    
    emit reloadTree( fileNameList(), true, false );

    saveProject();
  }
}

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

/**  */
void Project::insertFile( QString name )
{
  if ( !hasProject() ) return;

  int pos;
  if ( name.left(5) == "file:" ) name.remove(0,5);

  QString fname = QExtFileInfo::toRelative( name, basePath);
  QString shortname = fname;

  while ( ( pos = shortname.find('/') ) >= 0 ) shortname.remove(0,pos+1);

  if ( fname.left(2) == ".." )
  {
    QString caption = shortname;
    caption += ": copy to project directory...";
    CopyTo *dlg = new CopyTo( basePath, this, caption );

    if ( dlg->exec() )
    {
      QString path = dlg->lineDir->text();
      if ( path.right(1) != "/" ) path.append("/");

      QDir dir( path);
      int i = 10;

      while ( !dir.exists() && i-- )
        dir.mkdir( path);

      fname = path + shortname;

      if ( name != fname ) KIO::copy( KURL( name ), KURL( fname ) );

      fname = QExtFileInfo::toRelative( fname, basePath);
    }
    else {
      delete dlg;
      return;
    }
    delete dlg;
  }

  fileList.first();
  while ( fileList.current() )
  {
    if ( fileList.current()->fname == fname ) return;
    fileList.next();
  }

  ProjectItem *file = new ProjectItem( basePath );
  file->URL = baseURL + fname;
  file->fname = fname;

  insertFile( file );
}

/**  */
void Project::insertFile( ProjectItem *file )
{
  fileList.insert(0,file );
}

/** insert files from dir recursive */
void Project::insertFilesRecursive( QString path, bool webFiles, int level )
{

  if ( level > 10 )
    return;

  if ( !hasProject() ) return;

	QDir dir( path );
	QStringList dirList;
	QStringList fileList;
	
  if ( !dir.isReadable() ) return;
  if ( !dir.exists() ) return;

  dir.setSorting( QDir::Name);
	dir.setFilter ( QDir::Dirs);
	
	QStringList::Iterator it;
	
	dirList = dir.entryList();
		
	dirList.remove(".");
	dirList.remove("..");
		
	dir.setFilter( QDir::Files);
	
	fileList = dir.entryList();
		
	fileList.first();
		
	for ( it = fileList.begin(); it != fileList.end(); ++it )
	{
	  QString name = *it;
	
	  QFileInfo fi( dir, *it );
	  if ( fi.isSymLink() ) continue;
		
	  if ( QDir::match( fileMaskHtml , name) || QDir::match( fileMaskImage, name) || !webFiles)
	  {
	    QString fname = path+name;
	    insertFile( fname );
		}
	}
	
	for ( it = dirList.begin(); it != dirList.end(); ++it )
	{
		QString name = *it;
		
	  QFileInfo fi( dir, *it );
	  if ( fi.isSymLink() ) continue;
		
		insertFilesRecursive( path+name+"/", webFiles, level+1 );
  }
}

/** rescan project directory dialog and insert new files */
/*
void Project::rescanDirectory()
{
  if ( !hasProject() ) return;

  RescanPrjFiles *dlg = new RescanPrjFiles( &fileList, basePath, this, "New files...");

  if ( dlg->exec() ){
      QListViewItem *item = dlg->list->firstChild();
      while ( item ) {
        if ( item->isSelected())
           insertFile( item->text(0) );
        item = item->nextSibling() ;
      }
      reloadTree();

  }
  delete dlg;
}
*/

/**  */
bool Project::hasProject(){
  return ( projectFile != 0L );
}


/** upload current project */
/*
void Project::uploadProject()
{
  if ( !hasProject() ) return;

  ProjectUploadDlg *dlg = new ProjectUploadDlg( &fileList, this, "Upload Files");

  dlg->lineHost->setText(  host.data() );
  dlg->lineRoot->setText(  remoteDir.data() );
  dlg->lineLogin->setText( login.data() );

  if ( dlg->exec() )
  {
    host = dlg->lineHost->text();
    remoteDir = dlg->lineRoot->text();
    login = dlg->lineLogin->text();

    QString pass = dlg->linePassword->text();
    QList<ProjectFile> *list = dlg->selectedFiles();

    UploadProgress *pDlg = new UploadProgress( host, remoteDir, login, pass, list, dlg, "Upload..." );
    pDlg->exec();
  }

  delete dlg;
}
*/

/** project properties */
/*
void Project::properties()
{
  QTabDialog *dlg = new QTabDialog( 0L);

  mainDlg = new PrjPropertiesMain( dlg);
  mainDlg->lineProjectName->setText( projectName.data() );
  mainDlg->lineBaseURL    ->setText( baseURL.data() );
  mainDlg->lineAuthor     ->setText( author.data() );
  mainDlg->lineEMail      ->setText( email.data() );

  ftpDlg = new PrjPropertiesFTP( dlg);
  ftpDlg->lineHost ->setText( host.data() );
  ftpDlg->lineRDir ->setText( remoteDir.data() );
  ftpDlg->lineLogin->setText( login.data() );

  dlg->setCaption("Project properties");
  //dlg->setGeometry(300,200,420,300);
  dlg->setGeometry( QApplication::desktop()->width( )/2-210,
                    QApplication::desktop()->height()/2-150,
                    420,300);

  dlg->addTab( mainDlg, "General");
  dlg->addTab(  ftpDlg, "FTP");

  dlg->setOkButton();
  dlg->setCancelButton();

  connect( dlg, SIGNAL(applyButtonPressed()),  SLOT(acceptProperties()) );
  connect( dlg, SIGNAL(cancelButtonPressed()), dlg, SLOT(reject()) );

  dlg->show();
}
*/

/** setup properties */
/*
void Project::acceptProperties()
{
  projectName = mainDlg->lineProjectName->text();
  baseURL     = mainDlg->lineBaseURL->text();
  author      = mainDlg->lineAuthor->text();
  email       = mainDlg->lineEMail->text();

  host        = ftpDlg->lineHost->text();
  remoteDir   = ftpDlg->lineRDir->text();
  login       = ftpDlg->lineLogin->text();

  reloadTree();
}
*/

void Project::slotRemoveFolder(QString fname)
{
	debug( fname );
	fileList.first();
	while ( fileList.current() )
	{
		ProjectItem *item = fileList.current();
		
		if ( !qstrncmp( fname, item->fullName(), fname.length() ) )
		{
			debug( fname );
			debug( item->fullName() );
			fileList.remove();
			continue;
		}
		fileList.next();
	}

}

void Project::slotRemoveFile(QString fname)
{
	fileList.first();
	while ( fileList.current() )
	{
		ProjectItem *item = fileList.current();
		
		if ( item->fullName() == fname )
		{
			debug( fname );
			debug( item->fullName() );
			fileList.remove();
			continue;
		}
		fileList.next();
	}
}
