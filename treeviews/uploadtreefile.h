/***************************************************************************
                          uploadtreefile.h  -  description
                             -------------------
    begin                : Sun Aug 25 2002
    copyright            : (C) 2002 by Andras Mantia
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

#ifndef UPLOADTREEFILE_H
#define UPLOADTREEFILE_H

#include <qlistview.h>
#include "uploadtreefolder.h"

/**
  *@author George Vilches
  */

class UploadTreeFile : public QListViewItem  {
public: 
	UploadTreeFile( UploadTreeFolder *parent, QString, QString, QString);
	UploadTreeFile( QListView *parent, QString, QString, QString );
	~UploadTreeFile();
	
	/** set icon for the item */
	void setDirIcon();
	void setIcon( QString name );
  void setWhichPixmap( QString );

	/** used for sorting */
	virtual QString key ( int column, bool ascending ) const;

public:
	QString fname;
	bool isDir;
};

#endif
