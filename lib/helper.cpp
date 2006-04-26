/***************************************************************************
    begin                : Tue Jul 12 2005
    copyright            : (C) 2005 by Jens Herden <jens@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/
#include "helper.h"

#include <kmessagebox.h>
#include <klocale.h>
#include <kglobal.h>
#include <kstandarddirs.h>

#include <qdir.h>
#include <qstring.h>


Helper::Helper()
{
}


Helper::~Helper()
{
}


int Helper::denyBinaryInsert()
{
  return KMessageBox::questionYesNo(0L, i18n("The file type is not recognized. \
  Opening binary files may confuse Quanta.\n Are you sure you want to open this file?"),
  i18n("Unknown Type"), KStdGuiItem::yes(), KStdGuiItem::no(), "Open Everything");
}


QString Helper::tmpFilePrefix()
{
  static const QString & tmpDir = KGlobal::staticQString(getTmpFolder());
  return tmpDir;
}


QString Helper::dataDir()
{
  static const QString & dataDir = KGlobal::staticQString(KGlobal::dirs()->findResourceDir("data", "quanta/pics/quantalogo.png"));  // FIXME kuanta is hardcoded!!
  return dataDir;
}


QString Helper::getTmpFolder()
{
  QString newFolder;
  QStringList tmpDirs = KGlobal::dirs()->resourceDirs("tmp");
  newFolder = tmpDirs[0];
  uint count = tmpDirs.count();
  for (uint i = 1; i < count; i++)
  {
    if (tmpDirs[i].contains("kde-"))
      newFolder = tmpDirs[i];  // TODO why not exit the loop? jens
  }
  QDir dir;
  dir.mkdir(newFolder + "quanta");
  newFolder += "quanta/quanta";
  return newFolder;
}
