/***************************************************************************
                          rescanprj.h  -  description
                             -------------------
    begin                : ?
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
                           (C) 2002,2003 Andras Mantia <amantia@freemail.hu>
    email                : pdima@users.sourceforge.net,yshurik@penguinpowered.com,sequitur@easystreet.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef RESCANPRJ_H
#define RESCANPRJ_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <qregexp.h>
#include <kio/job.h>

#include "rescanprjdir.h"

struct URLListEntry{
   KURL url;
   QString date;
   QString size;
 };

class RescanPrj : public RescanPrjDir  {
   Q_OBJECT
public:
  RescanPrj(KURL::List p_prjFileList, const KURL& p_baseURL, QRegExp &p_excludeRx, QWidget *parent=0, const char *name=0, bool modal = true);
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
  QRegExp excludeRx;
  QValueList<URLListEntry> urlList;
protected slots: // Protected slots
  /** No descriptions */
  void slotListDone(KIO::Job *);
};

#endif

