/***************************************************************************
                          previewfontoptions.h  -  description
                             -------------------
    begin                : Mon Dec 4 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon <pdima@users.sourceforge.net,yshurik@penguinpowered.com,sequitur@easystreet.com>
                           (C) 2001-2002 Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef PREVIEWFONTOPTIONS_H
#define PREVIEWFONTOPTIONS_H

#include <qwidget.h>
#include <qtabwidget.h>
#include <qfont.h>

/**
  *@author Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
  */

class KFontChooser;

class PreviewFontOptions : public QTabWidget  {
   Q_OBJECT
public:
  PreviewFontOptions(QWidget *parent=0, const char *name=0);
  ~PreviewFontOptions();

  KFontChooser *stdFont, *fixedFont;
};

#endif
