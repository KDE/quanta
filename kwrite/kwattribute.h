/*
  $Id$

   Copyright (C) 1998, 1999 Jochen Wilhelmy
                            digisnap@cs.tu-berlin.de

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
*/

#ifndef _KWATTRIBUTE_H_
#define _KWATTRIBUTE_H_

#include <qstring.h>
#include <qcolor.h>
#include <qfont.h>
#include <qfontmetrics.h>

class Attribute {
  public:
    Attribute();
//    Attribute(const char *aName, const QColor &, const QColor &, const QFont &);
//    QString name;
    QColor col;
    QColor selCol;
    void setFont(const QFont &);
    QFont font;
    QFontMetrics fm;
    //workaround for slow QFontMetrics::width()
    int width(QChar c) {return (fontWidth < 0) ? fm.width(c) : fontWidth;}
    int width(QString s) {return (fontWidth < 0) ? fm.width(s) : s.length()*fontWidth;}
  protected:
    int fontWidth;
};

#endif //_KWATTRIBUTE_H_
