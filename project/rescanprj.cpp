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
  
  KURL bu(basePath);
  this->basePath = basePath;
  this->prjFileList = prjFileList;
  
  if ( bu.isLocalFile() )
  {
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
	}
	else
	{
	  KIO::ListJob *job = KIO::listRecursive( bu, false );

    connect( job, SIGNAL(entries(KIO::Job *,const KIO::UDSEntryList &)), 
             this,SLOT  (addEntries(KIO::Job *,const KIO::UDSEntryList &)));
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

void RescanPrj::addEntries(KIO::Job *,const KIO::UDSEntryList &list)
{
  KIO::UDSEntryListConstIterator it  = list.begin();
  KIO::UDSEntryListConstIterator end = list.end();
  
  for (; it != end; ++it) 
  {
    KIO::UDSEntry::ConstIterator it2 = (*it).begin();
    
    bool isDir;
    QString name;
    unsigned long size = 0L;
    
    for( ; it2 != (*it).end(); it2++ ) 
    {
      switch( (*it2).m_uds ) {
        case KIO::UDS_NAME:
          name = (*it2).m_str;
          break;
        case KIO::UDS_SIZE:
          size = ((*it2).m_long);
          break;
        case KIO::UDS_FILE_TYPE:
          isDir = S_ISDIR((*it2).m_long);
          break;
        default:
          break;
      }
    }
    
    if ( !isDir && name != QString::fromLatin1("..") && 
         prjFileList.findIndex(name) == -1 )
    {
	    this->list.append(name);
	    
  	  QString s;
      s.sprintf( "%i", size );
  	    
  	  new QListViewItem(listView, name, s);
    }
  }
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
