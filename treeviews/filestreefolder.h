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
#include <qstringlist.h>
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
    virtual QString fullName();

    virtual void setOpen( bool );
    void setup();
  /** No descriptions */
  virtual bool acceptDrop(const QMimeSource *mime) const;

public:
    QFile file;
    FilesTreeFolder * parentFolder;
    QString folderName;
    QStringList openedList;

    bool readable;
    bool opened;
    bool showall;

public slots: // Public slots
  	/** reload file list */
  	void reloadList();
private: // Private methods
  /** No descriptions */
  void restoreOpenStatus();
private: // Private methods
  /** No descriptions */
  void saveOpenStatus();
protected: // Protected methods
  /** No descriptions */
  virtual void dropped(QDropEvent *e);
};


#endif
