/***************************************************************************
                          filestreefile.h  -  description
                             -------------------
    begin                : Wed May 31 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
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

#ifndef FILESTREEFILE_H
#define FILESTREEFILE_H

#include <qlistview.h>
#include <kurl.h>

class FilesTreeFolder;
/**
  *@author Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
  */

class FilesTreeFile : public QListViewItem
{
public: 
	FilesTreeFile( FilesTreeFolder *parent, QString name, const KURL& p_url);
	FilesTreeFile( QListView *parent, QString name, const KURL& p_url);
	~FilesTreeFile();
	
	/** set icon for the item */
	void setDirIcon();
  void setIcon(const KURL& a_url);
  /** No descriptions */
  virtual void sortChildItems(int column, bool ascending);

  /** used for sorting */
//  virtual QString key ( int column, bool ascending ) const;

public:
	KURL url;
	bool isDir;
};

#endif
