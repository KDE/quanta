#include "rescanprj.h"
#include "rescanprj.moc"
// qt includes
#include <qlistview.h>
#include <qpushbutton.h>

// kde includes
#include <kurl.h>
#include <klocale.h>

// app includes
#include "rescanprj.h"
#include "../qextfileinfo.h"
#include "../quantacommon.h"

//TODO Redo with KURLs and KDIRLister.

RescanPrj::RescanPrj(KURL::List p_prjFileList, const KURL& p_baseURL, QWidget *parent, const char *name, bool modal )
	: RescanPrjDir(parent,name,modal)
{
  setCaption(name);

  listView->setColumnText(1, i18n("Add"));
  listView->removeColumn(3); //TODO: Date column removed as date is not read...
  baseURL = p_baseURL;
  baseURL.adjustPath(1);

  prjFileList = p_prjFileList;

  KIO::ListJob *job = KIO::listRecursive( baseURL, false );

  connect( job, SIGNAL(entries(KIO::Job *,const KIO::UDSEntryList &)),
           this,SLOT  (addEntries(KIO::Job *,const KIO::UDSEntryList &)));


	connect( buttonSelect,   SIGNAL(clicked()),
	         this,           SLOT(slotSelect()));
	connect( buttonDeselect, SIGNAL(clicked()),
	         this,           SLOT(slotDeselect()));
	connect( buttonInvert,   SIGNAL(clicked()),
	         this,           SLOT(slotInvert()));
	connect( buttonExpand,   SIGNAL(clicked()),
	         this,           SLOT(slotExpand()));
	connect( buttonCollapse, SIGNAL(clicked()),
	         this,           SLOT(slotCollapse()));
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

    //TODO: Get also the file date
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

    KURL u = baseURL;
    QuantaCommon::setUrl(u, name);
    if ( !isDir && name != QString::fromLatin1("..") &&
         prjFileList.findIndex(u) == -1 )
    {
	    this->list.append(u);

  	  QString s = QString("%1").arg( size );

      
//  	  new QListViewItem(listView, name, s);
      listView->addItem(name, s, "");
    }
  }

  slotSelect();
}

void RescanPrj::resizeEvent ( QResizeEvent *t )
{
  RescanPrjDir::resizeEvent(t);
//  listView->setColumnWidth(0,listView->width()-listView->columnWidth(1)-20);
}

void RescanPrj::slotSelect()
{
  listView->selectAll(true);
  listView->slotSelectFile();
}

void RescanPrj::slotDeselect()
{
  listView->selectAll(false);
  listView->slotSelectFile();
}

void RescanPrj::slotInvert()
{
  listView->invertAll();
  listView->slotSelectFile();
}

void RescanPrj::slotExpand()
{
  listView->expandAll();
}

void RescanPrj::slotCollapse()
{
  listView->collapseAll();
}

KURL::List RescanPrj::files()
{
	KURL::List r;

  QListViewItem *item;
  QListViewItemIterator it(listView);
  for ( ; it.current(); ++it )
  {
   item = it.current();
   if ( listView->isSelected( item ))
   {
     KURL u = baseURL;
     u.setPath(baseURL.path(1)+item->text(0));
     r.append(u);
   }
  }
  return r;
}
