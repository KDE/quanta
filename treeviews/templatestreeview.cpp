/***************************************************************************
                          templatestreeview.cpp  -  description
                             -------------------
    begin                : Thu Dec 20 2001
    copyright            : (C) 2001 by Andras Mantia
    email                : amantia@freemail.hu
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
// QT includes
#include <qcheckbox.h>
#include <qdir.h>
#include <qpixmap.h>
#include <qheader.h>
#include <qdragobject.h>
#include <qpoint.h>

// KDE includes
#include <krun.h>
#include <klocale.h>
#include <kiconloader.h>
#include <kopenwith.h>
#include <kstddirs.h>
#include <kmimetype.h>
#include <kmessagebox.h>
#include <kcombobox.h>

#include "templatestreeview.h"
#include "filestreefolder.h"
#include "filestreefile.h"
#include "newtemplatedirdlg.h"

const QString textMenu = i18n("Insert as text");
const QString binaryMenu = i18n("Insert binary (as link)");
const QString docMenu = i18n("New document based on this");

TemplatesTreeView::TemplatesTreeView(const QString& projectBasePath, QWidget *parent, const char *name )
{
    QListView::QListView(parent,name);

    basePath = projectBasePath;

	fileMenu = new QPopupMenu();
	
	openId = fileMenu -> insertItem( UserIcon("open"),  i18n("&Open"), 		this ,SLOT(slotInsert()));
  fileMenu -> insertItem(i18n("Open for editing"), 	this ,SLOT(slotOpen()));
//	fileMenu -> insertItem(i18n("Insert tag"), 	this ,SLOT(slotInsertTag()));
//	fileMenu -> insertItem(i18n("Insert as text in document"), 	this ,SLOT(slotInsertInDocument()));
//	fileMenu -> insertItem(i18n("New document based on this"), 	this ,SLOT(slotNewDocument()));
	fileMenu -> insertSeparator();
	fileMenu -> insertItem( i18n("&New directory"), 		this ,SLOT(slotNewDir()));
	fileMenu -> insertItem( UserIcon("copy"),  i18n("&Copy"), 		this ,SLOT(slotCopy()));
	fileMenu -> insertItem( UserIcon("paste"), i18n("&Paste"),		this ,SLOT(slotPaste()));
	fileMenu -> insertItem( UserIcon("delete"),i18n("&Delete"),   this ,SLOT(slotDelete()));
	fileMenu -> insertItem( i18n("Properties"),   this ,SLOT(slotProperties()));
	fileMenu -> insertSeparator();
	fileMenu -> insertItem( i18n("Reload"),   this ,SLOT(slotReload()));
	
	
	folderMenu = new QPopupMenu();
	
//	folderMenu -> insertItem( i18n("Add folder to top"), this ,SLOT(slotAddToTop()), 0, ID_TOP, 0);
//	folderMenu -> insertSeparator();
	folderMenu -> insertItem( i18n("&New directory"), 		this ,SLOT(slotNewDir()));
	folderMenu -> insertItem( UserIcon("copy"),  i18n("&Copy"), 		this ,SLOT(slotCopy()));
	folderMenu -> insertItem( UserIcon("paste"), i18n("&Paste"),		this ,SLOT(slotPaste()));
	deleteMenuId = folderMenu -> insertItem( UserIcon("delete"),i18n("&Delete"),   this ,SLOT(slotDelete()));
	folderMenu -> insertItem( i18n("Properties"),   this ,SLOT(slotProperties()));
	folderMenu -> insertSeparator();
	folderMenu -> insertItem( i18n("Reload"),   this ,SLOT(slotReload()));

	QString dir = KGlobal::dirs()->findResourceDir("data","quanta/toolbar/quantalogo.png")+"quanta/templates/";
 	globalDir = new FilesTreeFolder( this , i18n("Global templates"), dir);
	globalDir->setPixmap( 0, SmallIcon("folder"));
	globalDir->setOpen( true );

	dir = locateLocal("data","quanta/templates/");
 	localDir = new FilesTreeFolder( this , i18n("Local templates"),dir);
	localDir->setPixmap( 0, SmallIcon("folder"));
	localDir->setOpen( true );

	if (! basePath.isEmpty())
	{
 		projectDir = new FilesTreeFolder( this , i18n("Project templates"), basePath+"templates/");
		projectDir->setPixmap( 0, SmallIcon("folder"));
		projectDir->setOpen( true );
	} else
	{
		projectDir = 0L;
	}
	
	this->dirList = 0L;	
	
	setRootIsDecorated( true );
    header()->hide();
    setSorting( 0 );

    setFrameStyle( Panel | Sunken );
    setLineWidth( 2 );
    addColumn( "Name" );

	setFocusPolicy(QWidget::ClickFocus);

	rootDir = 0L;
 	homeDir = 0L;
	
	connect(  this, SIGNAL(doubleClicked(QListViewItem *)),
						this, SLOT(slotSelectFile(QListViewItem *)));
	connect(  this, SIGNAL(selectionChanged(QListViewItem *)),
						this, SLOT(slotSelectImage(QListViewItem *)));
	connect(	this, SIGNAL(returnPressed(QListViewItem *)),
						this, SLOT(slotSelectFile(QListViewItem *)));
						
	connect( this, SIGNAL(rightButtonPressed(QListViewItem*, const QPoint&, int)),
					 this, SLOT(slotMenu(QListViewItem*, const QPoint&, int)));
					
	connect(	this, SIGNAL(open(QListViewItem *)),
						this,	SLOT(slotSelectFile(QListViewItem *)));
						
  setAcceptDrops(true);
  viewport()->setAcceptDrops(true);
}

TemplatesTreeView::~TemplatesTreeView()
{
}
/** No descriptions */
void TemplatesTreeView::slotInsertInDocument()
{
 QString mimetype = KMimeType::findByFileContent(currentFileName())->name();

 if (! mimetype.contains("text"))
 {
   if (KMessageBox::questionYesNo(this,i18n("This file may be a binary file, thus cannot be \
inserted correctly.\n Do you still want to insert it?"),i18n("Wrong type")) != KMessageBox::Yes)
  {
    return;
  }
 }

 emit insertFile(currentFileName());
}

void TemplatesTreeView::slotMenu(QListViewItem *item, const QPoint &point, int)
{
	if ( !item ) return;
	setSelected(item, true);
	
	FilesTreeFile *f = dynamic_cast<FilesTreeFile *>( item);
	if ( f )
  {
   QFileInfo fileInfo(currentFileName());
   QFileInfo dotFileInfo(fileInfo.dirPath()+"/.dirinfo");

   while ((!dotFileInfo.exists()) && (dotFileInfo.dirPath() != "/"))
   {
    dotFileInfo.setFile(QFileInfo(dotFileInfo.dirPath()).dirPath()+"/.dirinfo");
   }
   QFile dotFile(dotFileInfo.filePath());
   dotFile.open(IO_ReadOnly);

   QString s;
   QString menuText = "";

   dotFile.readLine(s,100);
   if (s.upper().contains("TEXT")) menuText = textMenu;
   if (s.upper().contains("IMAGE")) menuText = binaryMenu;
   if (s.upper().contains("TEMPLATE")) menuText = docMenu;

   if (menuText.isEmpty())
   {
     fileMenu->setItemEnabled(openId, false);
   } else
   {
     fileMenu->setItemEnabled(openId, true);
     fileMenu->changeItem(openId, menuText);
   }

   fileMenu->popup( point);
  }
	
	FilesTreeFolder *d = dynamic_cast<FilesTreeFolder *>( item);
	if ( d )
	{
	  if ( d->text(0) == i18n("Global templates") || d->text(0) == i18n("Local templates" )
	       || d->text(0) == i18n("Project templates") )
	        folderMenu ->setItemEnabled( deleteMenuId, false);
	  else  folderMenu ->setItemEnabled( deleteMenuId, true );
	
	  folderMenu ->popup     ( point);
	}
}
/** No descriptions */
void TemplatesTreeView::slotNewDocument()
{
 QString mimetype = KMimeType::findByFileContent(currentFileName())->name();

 if (! mimetype.contains("text"))
 {
   if (KMessageBox::questionYesNo(this,i18n("This file may be a binary file, thus cannot be \
used as a base file correctly.\n Do you still want to continue?"),i18n("Wrong type")) != KMessageBox::Yes)
  {
    return;
  }
 }

	QListViewItem *item = currentItem();
  if (item )
	{
  	FilesTreeFolder *parent = (FilesTreeFolder *) item->parent();
	  if ( !parent ) return;
	  if ( dynamic_cast<FilesTreeFolder *>(item) ) return;
	  emit openFile( KURL());
	  emit insertFile(currentFileName());
	}
}

void TemplatesTreeView::slotReload()
{
  globalDir->reloadList();
  localDir->reloadList();
  if (projectDir)
    projectDir->reloadList();
}

/** Insert the template as text, image, new document. */
void TemplatesTreeView::slotInsert()
{
 QString menuText = fileMenu->text(openId);

 if (menuText == textMenu) slotInsertInDocument();
 if (menuText == binaryMenu) slotInsertTag();
 if (menuText == docMenu) slotNewDocument();
}


#include "templatestreeview.moc"
/** No descriptions */
void TemplatesTreeView::slotNewDir()
{
  NewTemplateDirDlg *createDirDlg = new NewTemplateDirDlg(this,i18n("Create new template directory"));
  createDirDlg->typesCombo->insertItem("text/all");
  createDirDlg->typesCombo->insertItem("image/all");
  createDirDlg->typesCombo->insertItem("template/all");

  QListViewItem *item = currentItem();
  QString startDir = "";
	FilesTreeFile *f = dynamic_cast<FilesTreeFile *>( item);
	if ( f )
  {
   startDir = currentFileName();
  } else
  {
   startDir = currentFileName() + "/dummy_file";
  }

   QFileInfo dotFileInfo(QFileInfo(startDir).dirPath()+"/.dirinfo");

   while ((!dotFileInfo.exists()) && (dotFileInfo.dirPath() != "/"))
   {
    dotFileInfo.setFile(QFileInfo(dotFileInfo.dirPath()).dirPath()+"/.dirinfo");
   }
   QFile dotFile(dotFileInfo.filePath());
   dotFile.open(IO_ReadOnly);

   QString s;
   dotFile.readLine(s,100);

   if (s.isEmpty())
   {
    createDirDlg->parentAttr->setText(i18n("&Inherit parrent attribute (nothing)"));
   } else
   {
     createDirDlg->parentAttr->setText(i18n("&Inherit parrent attribute (%1)").arg(s));
   }
   if (createDirDlg->exec())
   {
    QDir dir;

    startDir = QFileInfo(startDir).dirPath();
    if (!dir.mkdir(startDir+"/"+createDirDlg->dirName->text()))
    {
      KMessageBox::error(this,i18n("Error while creating the new directory.\n \
                   Maybe you don't have permission to write in the %1 directory.").arg(startDir));
      return;
    }
    if (! createDirDlg->parentAttr->isChecked())
    {
     QFile dirFile(startDir+"/"+createDirDlg->dirName->text()+"/.dirinfo");
     dirFile.open(IO_WriteOnly);
     QTextStream dirStream(&dirFile);
     dirStream << createDirDlg->typesCombo->currentText();
     dirFile.flush();
     dirFile.close();
    }
    slotReload();
   }
}
/** No descriptions */
QDragObject * TemplatesTreeView::dragObject ()
{
  QDragObject *drag = new QTextDrag(currentFileName(), this);
  return drag;
}
/** No descriptions */
void TemplatesTreeView::contentsDropEvent(QDropEvent *e)
{
 QString source;
 QString dest = "";
 QTextDrag::decode(e,source);

  QListViewItem *item = itemAt( contentsToViewport(e->pos()));

	FilesTreeFolder *parent = dynamic_cast<FilesTreeFolder *> (item->parent());
	
	if ( !parent ) // top level element
  {
		dest = ((FilesTreeFolder *)item)->fullName();
	} else
  {
   	dest = parent->fullName()+item->text(0);
    KIO::Job *job = KIO::copy(source,dest);
    connect( job, SIGNAL( result( KIO::Job *) ), this , SLOT( slotJobFinished( KIO::Job *) ) );
  }	

}
/** No descriptions */
void TemplatesTreeView::contentsDragEnterEvent(QDragEnterEvent *event)
{
 if (QTextDrag::canDecode(event))
 {
    event->accept();
 }
}
