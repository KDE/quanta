/***************************************************************************
                          previewfontoptions.cpp  -  description
                             -------------------
    begin                : Mon Dec 4 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
    email                : pdima@users.sourceforge.net,yshurik@penguinpowered.com,sequitur@easystreet.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "previewfontoptions.h"

#include <kfontdialog.h>

PreviewFontOptions::PreviewFontOptions(QWidget *parent, const char *name ) : QTabWidget(parent,name)
{
	stdFont = new KFontChooser (this, "standard font", false);
	fixedFont = new KFontChooser (this, "fixed font", true);
	
	addTab( stdFont, "Standard");
	addTab( fixedFont, "Fixed");
}


PreviewFontOptions::~PreviewFontOptions()
{
}
