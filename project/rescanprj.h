/***************************************************************************
                          rescanprj.h  -  description
                             -------------------
    begin                : ?
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon <pdima@users.sourceforge.net,yshurik@penguinpowered.com,sequitur@easystreet.com>
                           (C) 2002,2003 Andras Mantia <amantia@kde.org>
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

#include <qregexp.h>
#include <kio/job.h>


#include "rescanprjdir.h"
#include "projecturl.h"

class KFileItem;

struct URLListEntry{
   KURL url;
   KFileItem* fileItem;
 };

class RescanPrj : public RescanPrjDir  {
   Q_OBJECT
public:
  RescanPrj(ProjectUrlList p_prjFileList, const KURL& p_baseURL, QRegExp &p_excludeRx, QWidget *parent=0, const char *name=0, bool modal = true);
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
  ProjectUrlList prjFileList;
  QRegExp excludeRx;
  QValueList<URLListEntry> urlList;
protected slots: // Protected slots
  /** No descriptions */
  void slotListDone(KIO::Job *);
};

#endif

