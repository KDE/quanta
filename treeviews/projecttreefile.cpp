/***************************************************************************
                          projectfile.cpp  -  description
                             -------------------
    begin                : Wed Mar 15 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavsky
                           (C) 2001, 2002 Andras Mantia
    email                : pdima@mail.univ.kiev.ua, amantia@freemail.hu
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

// QT includes
#include <qdir.h>
#include <qfileinfo.h>

// KDE includes
#include <kiconloader.h>

// app includes
#include "projecttreefile.h"
#include "projecttreefolder.h"
#include "../resource.h"

ProjectTreeFile::ProjectTreeFile( ProjectTreeFolder *parent, QString name, const KURL& p_url)
    : FilesTreeFile( parent, name, p_url)
{
  url = p_url;
}

ProjectTreeFile::~ProjectTreeFile(){
}

void ProjectTreeFile::paintCell(QPainter *p, const QColorGroup &cg, int column, int width, int alignment)
{
  QColorGroup mycg(cg);
  QListViewItem::paintCell(p,mycg,column,width,alignment);
}

