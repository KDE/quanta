/***************************************************************************
                          cssdialogi.h  -  description
                             -------------------
    begin                : Thu Oct 5 2000
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
#ifndef CSSDIALOGI_H
#define CSSDIALOGI_H
#include "cssdialog.h"

class CSSDialogI : public CSSDialog
{ 
    Q_OBJECT

public:
    CSSDialogI( QString basePath, QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~CSSDialogI();

    QString data();

};

#endif // CSSDIALOGI_H
