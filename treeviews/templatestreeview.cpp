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
#include <qlayout.h>
#include <qtextedit.h>

// KDE includes
#include <krun.h>
#include <klocale.h>
#include <kiconloader.h>
#include <kopenwith.h>
#include <kstddirs.h>
#include <kmimetype.h>
#include <kmessagebox.h>
#include <kcombobox.h>
#include <kpropertiesdialog.h>

#include "templatestreeview.h"
#include "templatestreeview.moc"
#include "filestreefolder.h"
#include "filestreefile.h"
#include "newtemplatedirdlg.h"
#include "quantapropertiespagedlg.h"
#include "../resource.h"

extern QString globalDataDir;
const QString textMenu = i18n("Insert as Text");
const QString binaryMenu = i18n("Insert Link to File");
const QString docMenu = i18n("New Document Based on This");

TemplatesTreeView::TemplatesTreeView(const QString& projectBasePath, QWidget *parent, const char *name )
  : projectDir(0)
{
    QListView::QListView(parent,name);

    basePath = projectBasePath;

	fileMenu = new QPopupMenu();
	
	openId = fileMenu -> insertItem( UserIcon("open"),  i18n("&Open"), 		this ,SLOT(slotInsert()));
  fileMenu -> insertItem(i18n("Open for Editing"), 	this ,SLOT(slotOpen()));
//	fileMenu -> insertItem(i18n("Insert Tag"), 	this ,SLOT(slotInsertTag()));
//	fileMenu -> insertItem(i18n("Insert as Text in Document"), 	this ,SLOT(slotInsertInDocument()));
//	fileMenu -> insertItem(i18n("New Document Based on This"), 	this ,SLOT(slotNewDocument()));
	fileMenu -> insertSeparator();
	fileMenu -> insertItem( i18n("&New Directory..."), 		this ,SLOT(slotNewDir()));
	fileMenu -> insertItem( UserIcon("copy"),  i18n("&Copy"), 		this ,SLOT(slotCopy()));
	fileMenu -> insertItem( UserIcon("paste"), i18n("&Paste"),		this ,SLOT(slotPaste()));
	fileMenu -> insertItem( UserIcon("delete"),i18n("&Delete..."),   this ,SLOT(slotDelete()));
	fileMenu -> insertItem( i18n("Properties..."),   this ,SLOT(slotProperties()));
	fileMenu -> insertSeparator();
	fileMenu -> insertItem( i18n("Reload"),   this ,SLOT(slotReload()));


	folderMenu = new QPopupMenu();

//	folderMenu -> insertItem( i18n("Add Folder to Top"), this ,SLOT(slotAddToTop()), 0, ID_TOP, 0);
//	folderMenu -> insertSeparator();
	folderMenu -> insertItem( i18n("&New Directory..."), 		this ,SLOT(slotNewDir()));
	folderMenu -> insertItem( UserIcon("copy"),  i18n("&Copy"), 		this ,SLOT(slotCopy()));
	folderMenu -> insertItem( UserIcon("paste"), i18n("&Paste"),		this ,SLOT(slotPaste()));
	deleteMenuId = folderMenu -> insertItem( UserIcon("delete"),i18n("&Delete..."),   this ,SLOT(slotDelete()));
	folderMenu -> insertItem( i18n("Properties..."),   this ,SLOT(slotProperties()));
	folderMenu -> insertSeparator();
	folderMenu -> insertItem( i18n("Reload"),   this ,SLOT(slotReload()));

	QString dir = globalDataDir +"quanta/templates/";
 	globalDir = new FilesTreeFolder( this , i18n("Global Templates"), dir);
	globalDir->setPixmap( 0, SmallIcon("folder"));
	globalDir->setOpen( true );

	dir = locateLocal("data","quanta/templates/");
 	localDir = new FilesTreeFolder( this , i18n("Local Templates"),dir);
	localDir->setPixmap( 0, SmallIcon("folder"));
	localDir->setOpen( true );

/*
	if (! basePath.isEmpty())
	{
 		projectDir = new FilesTreeFolder( this , i18n("Project Templates"), basePath+"templates/");
		projectDir->setPixmap( 0, SmallIcon("folder"));
		projectDir->setOpen( true );
	} else
	{
		projectDir = 0L;
	}
*/
	
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
  setSelectionMode(QListView::Single);
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
/*   if (confirmInsert() != KMessageBox::Yes)
  {
    return;
  } */
   denyBinaryInsert();
   return;
 }

 emit insertFile(currentFileName());
}

void TemplatesTreeView::slotMenu(QListViewItem *item, const QPoint &point, int)
{
	if ( !item ) return;
	setSelected(item, true);
	
	FilesTreeFolder *d = dynamic_cast<FilesTreeFolder *>( item);
	if ( d )
	{
	  if ( d->text(0) == i18n("Global Templates") || d->text(0) == i18n("Local Templates" )
	       || d->text(0) == i18n("Project Templates") )
	        folderMenu ->setItemEnabled( deleteMenuId, false);
	  else  folderMenu ->setItemEnabled( deleteMenuId, true );
	
	  folderMenu ->popup     ( point);
	} else
  {
   readDirInfo();
	
   QString menuText = "";

   if (dirInfo.mimeType.upper().contains("TEXT")) menuText = textMenu;
   if (dirInfo.mimeType.upper().contains("FILE")) menuText = binaryMenu;
   if (dirInfo.mimeType.upper().contains("TEMPLATE")) menuText = docMenu;

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
}
/** No descriptions */
void TemplatesTreeView::slotNewDocument()
{
 QString mimetype = KMimeType::findByFileContent(currentFileName())->name();

 if (! mimetype.contains("text"))
 {
/*   if (confirmInsert() != KMessageBox::Yes)
  {
    return;
  } */
   denyBinaryInsert();
   return;
 }

	QListViewItem *item = currentItem();
  if (item )
	{
  	FilesTreeFolder *parent = (FilesTreeFolder *) item->parent();
	  if ( !parent ) return;
	  if ( dynamic_cast<FilesTreeFolder *>(item) ) return;
    QString fileName = currentFileName(); // store, because openFile changes current item
    emit openFile(KURL(), defaultEncoding);
	  emit insertFile(fileName);
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

/** No descriptions */
void TemplatesTreeView::slotNewDir()
{
  NewTemplateDirDlg *createDirDlg = new NewTemplateDirDlg(this,i18n("Create New Template Directory"));
  createDirDlg->typesCombo->insertItem("text/all");
  createDirDlg->typesCombo->insertItem("file/all");
  createDirDlg->typesCombo->insertItem("template/all");

  readDirInfo();

   if (dirInfo.mimeType.isEmpty())
   {
    createDirDlg->parentAttr->setText(i18n("&Inherit parent attribute (nothing)"));
   } else
   {
     createDirDlg->parentAttr->setText(i18n("&Inherit parent attribute (%1)").arg(dirInfo.mimeType));
   }
   if (createDirDlg->exec())
   {
    QDir dir;

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
   startDir = QFileInfo(startDir).dirPath();
   if (!dir.mkdir(startDir+"/"+createDirDlg->dirName->text()))
   {
      KMessageBox::error(this,i18n("Error while creating the new directory.\n \
                   Maybe you don't have permission to write in the %1 directory.").arg(startDir));
      return;
   }
   if (! createDirDlg->parentAttr->isChecked())
   {
      dirInfo.mimeType = createDirDlg->typesCombo->currentText();
      dirInfo.preText = "";
      dirInfo.postText = "";
      writeDirInfo(startDir+"/"+createDirDlg->dirName->text()+"/.dirinfo");
   }
   slotReload();
  }
}
/** No descriptions */

QDragObject * TemplatesTreeView::dragObject ()
{
  readDirInfo();
  if(!dirInfo.mimeType.isEmpty()) // only drag when the template type is specified
  {
    QUriDrag *drag = new QUriDrag(this, 0);
    QStringList uriList(currentFileName());
    drag->setFileNames(uriList);
    return drag;
  }
  return 0;
}

/*
void TemplatesTreeView::startDrag()
{
  QUriDrag *drag = new QUriDrag(this, 0);
  QStringList uriList(currentFileName());
  drag->setFileNames(uriList);
  drag->drag();
 }
*/

/** No descriptions */
void TemplatesTreeView::contentsDropEvent(QDropEvent *e)
{
 QString source;
 QString dest = "";
// QTextDrag::decode(e,source);
 QStringList fileList;

 QUriDrag::decodeLocalFiles(e, fileList);
 if(fileList.empty()) return;

 source = fileList.front();

 QListViewItem *item = itemAt(contentsToViewport(e->pos()));

 if (item)
 {
  FilesTreeFolder *parent = dynamic_cast<FilesTreeFolder *> (item->parent());
	
	if ( !parent ) // top level element
  {
		dest = ((FilesTreeFolder *)item)->fullName();
	}
  else
  {
   	dest = parent->fullName()+item->text(0);
    KIO::Job *job = KIO::copy(source,dest);
    connect( job, SIGNAL( result( KIO::Job *) ), this , SLOT( slotJobFinished( KIO::Job *) ) );
  }
 }

}
/** No descriptions */
void TemplatesTreeView::contentsDragEnterEvent(QDragEnterEvent *event)
{
 if (QUriDrag::canDecode(event))
 {
    event->accept();
 }
}

/** Reads a .dirinfo file from the selected item's path */
void TemplatesTreeView::readDirInfo(QString startDir)
{
  QListViewItem *item = currentItem();

  if (startDir.isEmpty())
  {
    FilesTreeFile *f = dynamic_cast<FilesTreeFile *>( item);
  	if ( f )
    {
      startDir = currentFileName();
    } else
    {
     startDir = currentFileName() + "/dummy_file";
    }
  }

  QFileInfo dotFileInfo(QFileInfo(startDir).dirPath()+"/.dirinfo");

  while ((!dotFileInfo.exists()) && (dotFileInfo.dirPath() != "/"))
  {
   dotFileInfo.setFile(QFileInfo(dotFileInfo.dirPath()).dirPath()+"/.dirinfo");
  }

  KConfig *config = new KConfig(dotFileInfo.filePath());
  dirInfo.mimeType = config->readEntry("Type");
  dirInfo.preText = config->readEntry("PreText");
  dirInfo.postText = config->readEntry("PostText");
  dirInfo.usePrePostText = config->readBoolEntry("UsePrePostText", false);

  delete config;
}

/** Writes a .dirinfo file from the selected item's path */
void TemplatesTreeView::writeDirInfo(QString dirInfoFile)
{
  QListViewItem *item = currentItem();
  QString startDir = "";

  if (dirInfoFile.isEmpty())
  {
	  FilesTreeFile *f = dynamic_cast<FilesTreeFile *>( item);
	  if ( f )
    {
      startDir = currentFileName();
    } else
    {
      startDir = currentFileName() + "/dummy_file";
    }
  } else
  {
    startDir = dirInfoFile;
  }

  QFileInfo dotFileInfo(QFileInfo(startDir).dirPath()+"/.dirinfo");

  KConfig *config = new KConfig(dotFileInfo.filePath());
  config->writeEntry("Type", dirInfo.mimeType);
  config->writeEntry("PreText", dirInfo.preText);
  config->writeEntry("PostText", dirInfo.postText);
  config->writeEntry("UsePrePostText", dirInfo.usePrePostText);
  config->sync();

  delete config;
}

void TemplatesTreeView::slotProperties()
{
  if ( !currentItem() ) return;

  KPropertiesDialog *propDlg = new KPropertiesDialog( KURL( currentFileName() ), this, 0L, false, false);

//Always add the Quanta directory page
  QFrame *quantaDirPage = propDlg->dialog()->addPage(i18n("Quanta Directory"));
  QVBoxLayout *topLayout = new QVBoxLayout( quantaDirPage);
  quantaProperties = new QuantaPropertiesPageDlg( quantaDirPage, i18n("Quanta") );

  quantaProperties->typesCombo->insertItem("text/all");
  quantaProperties->typesCombo->insertItem("file/all");
  quantaProperties->typesCombo->insertItem("template/all");

  readDirInfo();

  quantaProperties->typesCombo->setCurrentItem(dirInfo.mimeType);

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
  if (!dotFileInfo.exists()) quantaProperties->parentAttr->setChecked(true);
  if (dirInfo.mimeType.isEmpty())
   {
    quantaProperties->parentAttr->setText(i18n("&Inherit parent attribute (nothing)"));
   } else
   {
    quantaProperties->parentAttr->setText(i18n("&Inherit parent attribute (%1)").arg(dirInfo.mimeType));
   }
   quantaProperties->preTextEdit->setText(dirInfo.preText);
   quantaProperties->postTextEdit->setText(dirInfo.postText);
   if (dirInfo.usePrePostText)
   {
    quantaProperties->usePrePostText->setChecked(true);
   }

   topLayout->addWidget( quantaProperties );
//   connect( propDlg, SIGNAL( applied() ), this , SLOT( slotPropertiesApplied) );


//If the item is a file, add the Quanta file info page
  addFileInfoPage(propDlg);
  if (propDlg->exec() == QDialog::Accepted)
   {
    slotPropertiesApplied();
    slotReload();
   }

  delete propDlg;
}


/** No descriptions */
void TemplatesTreeView::slotPropertiesApplied()
{
  DirInfo localDirInfo;

  if (!quantaProperties->parentAttr->isChecked())
  {
    localDirInfo.mimeType = quantaProperties->typesCombo->currentText();
  } else
  {
    localDirInfo.mimeType = dirInfo.mimeType;
  }

  localDirInfo.usePrePostText = quantaProperties->usePrePostText->isChecked();
  localDirInfo.preText = quantaProperties->preTextEdit->text();
  localDirInfo.postText = quantaProperties->postTextEdit->text();

  if ( (dirInfo.mimeType != localDirInfo.mimeType) ||
       (dirInfo.preText != localDirInfo.preText) ||
       (dirInfo.postText != localDirInfo.postText))
  {
    dirInfo.mimeType = localDirInfo.mimeType;
    dirInfo.preText = localDirInfo.preText;
    dirInfo.postText = localDirInfo.postText;
    dirInfo.usePrePostText = localDirInfo.usePrePostText;
    writeDirInfo();
  }


}

/** No descriptions */
void TemplatesTreeView::slotInsertTag()
{
 if (!currentItem()) return;

 readDirInfo();
 emit insertTag( currentFileName(), dirInfo);
}

void TemplatesTreeView::slotDragInsert(QDropEvent *e)
{
  QStringList fileList;
  QUriDrag::decodeLocalFiles(e, fileList);

  if(fileList.empty())
   return;

  QString localFileName = fileList.front();

  readDirInfo(localFileName);
  QString mimeType = KMimeType::findByPath(localFileName)->name();

  /* First, see if the type of the file is specified in the .dirinfo file */
  if(!dirInfo.mimeType.isEmpty())
  {
    if(dirInfo.mimeType == "text/all") // default to inserting in document
    {
     if(!mimeType.contains("text", false))
     {
       denyBinaryInsert();
       return;
     }
     emit insertFile(localFileName);
    }

    if(dirInfo.mimeType == "file/all")
    {
      // whatever this is, insert it with a tag (image or link or prefix/postfix)
      emit insertTag(localFileName, dirInfo);
    }
    else
    if(dirInfo.mimeType == "template/all")
    {
      if(!mimeType.contains("text", false))
      {
/*        if(confirmInsert() != KMessageBox::Yes)
          return;*/
        denyBinaryInsert();
        return;
      }
      emit openFile(KURL(), defaultEncoding);
      emit insertFile(localFileName);
    }
  }
}

int TemplatesTreeView::confirmInsert()
{
  return KMessageBox::questionYesNo(this,i18n("This file may be a binary file, thus cannot be \
used as a base file correctly.\n Do you still want to continue?"),i18n("Wrong type")) ;
}
/** Sets the project template directory */
void TemplatesTreeView::slotSetTemplateDir(const QString &Dir)
{
  if(projectDir)
    delete projectDir;
  projectDir = 0;

  if(!Dir.isEmpty())
  {
    projectDir = new FilesTreeFolder(this, i18n("Project Templates"), Dir+"/");
    projectDir->setPixmap(0, SmallIcon("folder"));
    projectDir->setOpen(true);
  }

  slotReload();
}
/** No descriptions */
int TemplatesTreeView::denyBinaryInsert()
{
  KMessageBox::sorry(this, "Can't insert binary file as text", "Wrong type",
FALSE);

 return 1; //not used yet
}
