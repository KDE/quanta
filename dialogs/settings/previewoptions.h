/***************************************************************************
                          previewoptions.h  -  description
                             -------------------
    begin                : Fri Aug 4 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon <pdima@users.sourceforge.net,yshurik@penguinpowered.com,sequitur@easystreet.com>
                           (C) 2002, 2004 Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef PREVIEWOPTIONS_H
#define PREVIEWOPTIONS_H

#include "previewoptionss.h"

class KConfig;

class PreviewOptions : public PreviewOptionsS  {
   Q_OBJECT
public:
  PreviewOptions(QWidget *parent=0, const char *name=0);
  ~PreviewOptions();

  QString position();
  QString layout();
  QString closeButtons();
  QString docPosition();
  uint toolviewTabs();
  bool hiddenFiles();
  bool saveTrees();
  void setPosition(const QString&);
  void setWindowLayout(const QString&);
  void setCloseButtons(const QString&);
  void setToolviewTabs(uint);
  void setDocPosition(const QString&);
  void setHiddenFiles(bool show);
  void setSaveTrees(bool show);

private:
  uint m_toolviewTabs;
};

#endif
