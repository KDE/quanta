/***************************************************************************
                          previewoptions.cpp  -  description
                             -------------------
    begin                : Fri Aug 4 2000
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
#include "previewoptions.h"

// KDE includes
#include <kconfig.h>


PreviewOptions::PreviewOptions(QWidget *parent, const char *name) : QWidget(parent,name){
	initDialog();
}

PreviewOptions::~PreviewOptions(){
}

void PreviewOptions::setPosition( QString position )
{
	radioBottom->setChecked(true);
	
	if ( position == "Disabled") radioDisabled->setChecked(true);
	if ( position == "Bottom"  ) radioBottom->setChecked(true);
	if ( position == "Right"   ) radioRight->setChecked(true);
	if ( position == "FWSpace" ) radioFullworkspace->setChecked(true);
}

QString PreviewOptions::position()
{
	QString position = "Bottom";
	
	if ( radioBottom->isChecked() 				) position = "Bottom";
	if ( radioRight->isChecked()					) position = "Right";
	if ( radioFullworkspace->isChecked() 	) position = "FWSpace";
	if ( radioDisabled->isChecked()				) position = "Disabled";
	
	return position;
}
