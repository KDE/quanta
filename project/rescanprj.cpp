#include "rescanprj.h"

// qt includes
#include <qlistview.h>
#include <qstringlist.h>
#include <qpushbutton.h>

// kde includes

// app includes
#include "rescanprj.h"
#include "../qextfileinfo.h"

RescanPrj::RescanPrj(QStringList prjFileList, QString basePath, QWidget *parent, const char *name, bool modal )
	: RescanPrjDir(parent,name,modal)
{
  setCaption(name);
  
  listView->setColumnAlignment(1,Qt::AlignRight);
  
  if ( basePath.right(1) != "/" ) basePath += "/";
  
  this->basePath = basePath;
  
  QStringList::Iterator it;
	QStringList r = QExtFileInfo::allFilesRelative( basePath, "*");
	
	for ( it = r.begin(); it != r.end(); ++it )
	{
	  if ( prjFileList.findIndex(*it) == -1 )
	  {
	    list.append( *it );
	    QFileInfo fi( basePath+*it );
	    
	    QString size;
      size.sprintf( "%i", fi.size() );
	    
	    new QListViewItem(listView, *it, size);
	  }
	}
	
	connect( buttonSelect,   SIGNAL(clicked()),
	         this,           SLOT(slotSelect()));
	connect( buttonDeselect, SIGNAL(clicked()),
	         this,           SLOT(slotDeselect()));
	connect( buttonInvert,   SIGNAL(clicked()),
	         this,           SLOT(slotInvert()));
}

RescanPrj::~RescanPrj(){
}

void RescanPrj::resizeEvent ( QResizeEvent *t )
{
  RescanPrjDir::resizeEvent(t);
  listView->setColumnWidth(0,listView->width()-listView->columnWidth(1)-20);
}

void RescanPrj::slotSelect()
{
  QListViewItem *item;
  
  for ( item = listView->firstChild(); item; item = item->nextSibling())
  {
    listView->setSelected( item, true );
  }
}

void RescanPrj::slotDeselect()
{
  listView->clearSelection();
}

void RescanPrj::slotInvert()
{
  QListViewItem *item;
  
  for ( item = listView->firstChild(); item; item = item->nextSibling())
  {
    listView->setSelected( item, !listView->isSelected( item ));
  }
}

QStringList RescanPrj::files()
{
	QStringList r;
	
  QListViewItem *item;
  
  for ( item = listView->firstChild(); item; item = item->nextSibling())
  {
    if ( listView->isSelected( item ))
    {
      r.append( basePath+item->text(0) );
    }
  }
  return r;
}
