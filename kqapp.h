/*
    knapplication.h

    KNode, the KDE newsreader
    Copyright (c) 1999-2001 the KNode authors.
    See file AUTHORS for details

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software Foundation,
    Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, US
*/

#ifndef KQAPPLICATION_H
#define KQAPPLICATION_H

#include <kuniqueapp.h>


class KSplash : public QFrame
{
  Q_OBJECT

  public:
    KSplash();
    ~KSplash();

};

class KQApplication : public KApplication
{
  Q_OBJECT

  public:
    KQApplication();
    ~KQApplication();

};

class KQUniqueApplication : public KUniqueApplication
{
  Q_OBJECT

  public:
    KQUniqueApplication();
    ~KQUniqueApplication();

    /** Create new instance of KNode. Make the existing
        main window active if KNode is already running */
    int newInstance();

};

#endif
