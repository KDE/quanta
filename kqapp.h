/*
    knapplication.h

    KNode, the KDE newsreader
    Copyright (c) 1999-2001 the KNode authors.
    See file AUTHORS for details

    Rewritten for Quanta Plus: (C) 2002 Andras Mantia <amantia@kde.org>

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

#include <kuniqueapplication.h>

class KCmdLineArgs;

class KSplash : public QFrame
{
  Q_OBJECT

  public:
    KSplash();
    ~KSplash();

};

class KQApplicationPrivate
{
  public:
    KQApplicationPrivate() {};
    ~KQApplicationPrivate() {};
    
  protected:
    void init();
   
    KSplash *splash;
    KCmdLineArgs *args;
};

class KQApplication : public KApplication, KQApplicationPrivate
{
  Q_OBJECT

  public:
    KQApplication();
    ~KQApplication();
  public slots:  
    /** No descriptions */
    void slotInit();

};

class KQUniqueApplication : public KUniqueApplication, KQApplicationPrivate
{
  Q_OBJECT

  public:
    KQUniqueApplication();
    ~KQUniqueApplication();

    /** Create new instance of Quanta. Make the existing
        main window active if Quanta is already running */
    int newInstance();
  public slots:
    /** No descriptions */
    void slotInit();

};

#endif
