/***************************************************************************
                          csseditor_globals.h  -  description
                             -------------------
    begin                : dom ago 3 2003
    copyright            : (C) 2003 by gulmini luciano
    email                : gulmini.luciano@student.unife.it
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef CSSEDITOR_GLOBALS_H
#define CSSEDITOR_GLOBALS_H
#include <qspinbox.h>
#include <qregexp.h>
#include <qstringlist.h>

namespace CSSEditorGlobals {
const QStringList HTMLColors(QStringList::split(",",QString("aliceblue,antiquewhite,aqua,aquamarine,azure,beige,bisque,black,blanchedalmond,"
                                   "blue,blueviolet,brown,burlywood,cadetblue,chartreuse,chocolate,coral,cornflowerblue,"
                                   "cornsilk,crimson,cyan,darkblue,darkcyan,darkgoldenrod,darkgray,darkgreen,"
                                   "darkkhaki,darkmagenta,darkolivegreen,darkorange,darkorchid,darkred,darksalmon,"
                                   "darkseagreen,darkslateblue,darkslategray,darkturquoise,darkviolet,deeppink,"
                                   "deepskyblue,dimgray,dodgerblue,firebrick,floralwhite,forestgreen,fuchsia,gainsboro,"
                                   "ghostwhite,gold,goldenrod,gray,green,greenyellow,honeydew,hotpink,indianred,"
                                   "indigo,ivory,khaki,lavender,lavenderblush,lawngreen,lemonchiffon,lightblue,lightcoral,"
                                   "lightcyan,lightgoldenrodyellow,lightgreen,lightgrey,lightpink,lightsalmon,lightseagreen,"
                                   "lightskyblue,lightslategray,lightsteelblue,lightyellow,lime,limegreen,linen,magenta,"
                                   "maroon,mediumaquamarine,mediumblue,mediumorchid,mediumpurple,mediumseagreen,"
                                   "mediumslateblue,mediumspringgreen,mediumturquoise,mediumvioletred,midnightblue,"
                                   "mintcream,mistyrose,moccasin,navajowhite,navy,oldlace,olive,olivedrab,orange,"
                                   "orangered,orchid,palegoldenrod,palegreen,paleturquoise,palevioletred,papayawhip,"
                                   "peachpuff,peru,pink,plum,powderblue,purple,red,rosybrown,royalblue,saddlebrown,"
                                   "salmon,sandybrown,seagreen,seashell,sienna,silver,skyblue,slateblue,slategray,snow,"
                                   "springgreen,steelblue,tan,teal,thistle,tomato,turquoise,violet,wheat,white,whitesmoke,"
                                   "yellow,yellowgreen")));
                                   

  const QStringList lengthUnits(QStringList::split(",",QString("cm,em,ex,in,mm,pc,pt,px"))); 
  const QStringList frequencyUnits(QStringList::split(",",QString("Hz,kHz")));
  const QStringList angleUnits(QStringList::split(",",QString("deg,rad,grad")));
  const QStringList timeUnits(QStringList::split(",",QString("s,ms")));
}

                                                                   
class mySpinBox : public QSpinBox{
    Q_OBJECT
  public:
    mySpinBox(QWidget * parent = 0, const char * name = 0 );
    mySpinBox( int minValue, int maxValue, int step = 1, QWidget * parent = 0, const char * name = 0 );
    ~mySpinBox();
  public slots:
    void checkSuffix(const QString&);  
};

                                                          
#endif
