/***************************************************************************
                          dirview.h  -  description
                             -------------------
    begin                : Mon Feb 21 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavski
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

#ifndef FILESVIEW_H
#define FILESVIEW_H

#include <qlistview.h>
#include <qpopupmenu.h>
#include <qstring.h>
#include <qfile.h>

class FilesTreeFolder: public QListViewItem
{
public:
    FilesTreeFolder( QListView * parent, const char * name, const char *_dir);
    FilesTreeFolder( FilesTreeFolder * parent, const char * name );
    virtual ~FilesTreeFolder();

    QString text( int column ) const;

  	/** used for sorting */
  	virtual QString key ( int column, bool ascending ) const;
    virtual QString fullName(QListViewItem *item = 0);

    void setOpen( bool );
    void setup();

public:
    QFile file;
    FilesTreeFolder * parentFolder;
    QString folderName;

    bool readable;
    bool opened;
    bool showall;

public slots: // Public slots
  	/** reload file list */
  	void reloadList();
};


#endif
