/***************************************************************************
                          renameitem.h  -  description
                             -------------------
    begin                : Sat Nov 24 2001
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

#ifndef RENAMEITEM_H
#define RENAMEITEM_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


// app includes
#include "renamedialog.h"

// kde includes
#include <kio/jobclasses.h>

/**
  *@author Andras Mantia
  */

class RenameItem : public RenameDialog  {
Q_OBJECT
public: 
	RenameItem(QWidget *parent=0,const char *name=0);
	QString renameFile( QString oldName );
	QString renameFolder( QString oldName );
	~RenameItem();

public slots: // Public slots
  void slotResult( KIO::Job *);
signals: // Signals
  /** No descriptions */
  void renameFinished(RenameItem*);
	
};

#endif
