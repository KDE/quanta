/***************************************************************************
                    dcopwindowmanagerif.h  -  description
                             -------------------
    begin                : Wed Feb 5 2003
    copyright            : (C) 2003 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/

#ifndef DCOPWINDOWMANAGERIF_H
#define DCOPWINDOWMANAGERIF_H

#include <dcopobject.h>
#include <qstringlist.h>

class DCOPWindowManagerIf : virtual public DCOPObject
{
  K_DCOP

  k_dcop:

  virtual int currentEditorIfNum() const = 0;
  virtual QString currentURL() const = 0;
  virtual QString projectURL() const = 0;
  virtual QStringList openedURLs() const = 0;
  virtual QStringList tagAreas(const QString& tag, bool includeCoordinates, bool skipFoundContent) const = 0;
  virtual void newCursorPosition(QString file, int lineNumber, int columnNumber) = 0;
  virtual void newDebuggerPosition(QString file, int lineNumber) = 0;
  virtual void openFile(QString file, int lineNumber, int columnNumber) = 0;
  virtual QString saveCurrentFile() = 0;
  virtual QString documentFolderForURL(QString url) = 0;
};

#endif

