/***************************************************************************
                          copytodlg.h  -  description
                             -------------------
    begin                : Mon Mar 27 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavsky
    								  (C) 2001 by Andras Mantia
    email                : pdima@mail.univ.kiev.ua
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

// app includes
#include "copytos.h"
/**
  *@author Yacovlev Alexander & Dmitry Poplavsky
  */

class CopyTo : public CopyToS  {
Q_OBJECT
public: 
	CopyTo(QString dir, QWidget *parent=0, const char *name=0);
	~CopyTo();

	QString copy( QString rname );
	QStringList copy( QStringList rfiles );
	QString fname; 

public slots: // Public slots
	void endCopy( KIO::Job *,const KURL&,const KURL&,bool,bool);
	void slotResult( KIO::Job *);

signals:
	void addFilesToProject(QString,CopyTo*);

private:
	QString mInitialDir;
};

#endif
