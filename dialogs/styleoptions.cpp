/***************************************************************************
                          styleoptions.cpp  -  description
                             -------------------
    begin                : Fri Apr 7 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavsky
    email                : pdima@mail.univ.kiev.ua
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "styleoptions.h"

StyleOptions::StyleOptions(QWidget *parent, const char *name) : QWidget(parent,name){
	initDialog();
}

StyleOptions::~StyleOptions(){
}
