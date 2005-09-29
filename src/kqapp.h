/*
    knapplication.h

    KNode, the KDE newsreader
    Copyright (c) 1999-2001 the KNode authors.
    See file AUTHORS for details

    Rewritten for Quanta Plus: (C) 2002, 2003 Andras Mantia <amantia@kde.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software Foundation,
    Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, US
*/

#ifndef KQAPPLICATION_H
#define KQAPPLICATION_H

#include <qframe.h>
#include <kuniqueapplication.h>

class KCmdLineArgs;
class KSplashScreen;

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
    KQApplicationPrivate():splash(0L), sp(0L) {};
    ~KQApplicationPrivate() {};

  protected:
    void init();

    KSplash *splash;
    KCmdLineArgs *args;
    KSplashScreen *sp;
};

class KQApplication : public KApplication, KQApplicationPrivate
{
  Q_OBJECT

  public:
    KQApplication();
    ~KQApplication();
  public slots:
    void slotInit();
    void slotSplashTimeout();
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
    void slotInit();
    void slotSplashTimeout();
};

#endif
