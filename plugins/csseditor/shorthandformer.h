/***************************************************************************
 *   Copyright (C) 2003 by Gulmini Luciano                                          *
 *   gulmini.luciano@student.unife.it                                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef SHORTHANDFORMER_H
#define SHORTHANDFORMER_H

/**
@author Gulmini Luciano
*/
#include <qmap.h>
class QString;
class QStringList;

class ShorthandFormer{
public:
    ShorthandFormer();
    ShorthandFormer( QMap<QString,QString> m );
    ~ShorthandFormer();
    QString compress();
    QMap<QString,QString> expand(QString propertyName, QString propertyValue);
    static QStringList SHFormList();
     
private:
    QMap<QString,QString> m_properties;
    QString cue_after,
               cue_before,
               pause_before,
               pause_after,
               background_color,
               background_image,
               background_repeat,
               background_attachment,
               background_position,
               border_top_style,
               border_top_color,
               border_top_width,
               border_left_style,
               border_left_color,
               border_left_width,
               border_right_style,
               border_right_color,
               border_right_width,
               border_bottom_style,
               border_bottom_color,
               border_bottom_width,
               outline_style,
               outline_color,
               outline_width,
               list_style_type,
               list_style_image,
               list_style_position,
               font_style,
               font_variant,
               font_weight,
               font_size,
               line_height,
               font_family,
               margin_top,
               margin_bottom,
               margin_left,
               margin_right,
               padding_top,
               padding_bottom,
               padding_left,
               padding_right;
    
private:
  QString compressCueProp();
  QString compressPauseProp();
  QString compressPaddingProp();
  QString compressMarginProp();
  QString compressFontProp();
  QString compressBackgroundProp();
  QString compressOutlineProp();
  QString compressListStyleProp(); 
  QString compressBorderStyleProp();
  QString compressBorderWidthProp();
  QString compressBorderColorProp(); 
  QString compressBorderProp(); 
  
  QString compressImplementation( QString prop, QString t, QString b, QString r, QString l, QString defValue);
  QString compressImplementation2( QString prop, QString after, QString before, QString defValue);
  QString compressImplementation3( QString prop, QString p1, QString p2, QString p3);
  
  QMap<QString,QString> expandCueProp(QStringList l);
  QMap<QString,QString> expandPauseProp(QStringList l);
  QMap<QString,QString> expandBackgroundProp(QStringList l);
  QMap<QString,QString> expandFontProp(QStringList l);
  QMap<QString,QString> expandOutlineProp(QStringList l);
  QMap<QString,QString> expandListstyleProp(QStringList l);
  QMap<QString,QString> expandBoxSide(QString subPropName, QStringList l);
  QMap<QString,QString> expandBorderProp(QStringList l);
  QMap<QString,QString> expandBox(QString subPropName, QStringList l);
  QMap<QString,QString> expandPaddingProp(QStringList l);
  QMap<QString,QString> expandMarginProp(QStringList l);
 
  QMap<QString,QString> expandImplementation(QString propertyName, QStringList l);
  QMap<QString,QString> expandImplementation2(QString propertyName, QStringList l);
 };

#endif
