/***************************************************************************
                          copytodlg.h  -  description
                             -------------------
    begin                : Mon Mar 27 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavsky <dima@mail.univ.kiev.ua>
                           (C) 2001-2003 by Andras Mantia
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef COPYTO_H
#define COPYTO_H

// kde includes
#include <kio/jobclasses.h>


/**
  *@author Yacovlev Alexander & Dmitry Poplavsky
  */

class KURL;

class CopyTo : public QObject{
Q_OBJECT
public:
  CopyTo(const KURL& dirURL);
  ~CopyTo();

  KURL copy( const KURL& sourceURL, const KURL &destination);
  KURL::List copy(const KURL::List& sourceList, const KURL &destination );

public slots: // Public slots
  void slotResult( KIO::Job *);

signals:
  void addFilesToProject(const KURL::List&);
  void deleteDialog(CopyTo*);

private:
  KURL m_InitialDirUrl;
  bool m_listCopy;
  KURL::List m_destList;
};

#endif
