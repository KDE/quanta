#ifndef RESCANPRJ_H
#define RESCANPRJ_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <kio/job.h>

#include "rescanprjdir.h"

class RescanPrj : public RescanPrjDir  {
   Q_OBJECT
public: 
	RescanPrj(QStringList, QString, QWidget *parent=0, const char *name=0, bool modal = true);
	~RescanPrj();
	
public slots:
  void slotSelect();
  void slotDeselect();
  void slotInvert();
  
  virtual void resizeEvent( QResizeEvent * );

  QStringList files();
  
  void addEntries(KIO::Job *,const KIO::UDSEntryList &);
  
private:
  QString basePath;
  QStringList list;
  QStringList prjFileList;
};

#endif

