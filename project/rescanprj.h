#ifndef RESCANPRJ_H
#define RESCANPRJ_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <kio/job.h>

#include "rescanprjdir.h"
#include "../treeviews/uploadtreeview.h"


struct URLListEntry{
   KURL url;
   QString date;
   QString size;
 };

class RescanPrj : public RescanPrjDir  {
   Q_OBJECT
public: 
	RescanPrj(KURL::List p_prjFileList, const KURL& p_baseURL, QWidget *parent=0, const char *name=0, bool modal = true);
	~RescanPrj();
	
public slots:
  void slotSelect();
  void slotDeselect();
  void slotInvert();
  void slotExpand();
  void slotCollapse();
  
  virtual void resizeEvent( QResizeEvent * );

  KURL::List files();
  
  void addEntries(KIO::Job *,const KIO::UDSEntryList &);
  
private:
  KURL baseURL;
  KURL::List prjFileList;
  QValueList<URLListEntry> urlList;
protected slots: // Protected slots
  /** No descriptions */
  void slotListDone(KIO::Job *);
};

#endif

