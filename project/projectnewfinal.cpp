/***************************************************************************
                          projectnewgeneral.cpp  -  description
                             -------------------
    begin                : Fri Oct 27 2000
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

// qt includes
#include <qlabel.h>
#include <qlayout.h>

// kde includes
#include <kiconloader.h>

// app includes
#include "projectnewfinal.h"
#include "projectnewfinal.moc"

ProjectNewFinal::ProjectNewFinal(QWidget *parent, const char *name )
  : ProjectNewFinalS(parent,name)
{
  imagelabel->setPixmap( UserIcon("wiznewprjfin") );
}

ProjectNewFinal::~ProjectNewFinal(){
}

void ProjectNewFinal::setMargin(int i)
{
  layout()->setMargin(i);
}

