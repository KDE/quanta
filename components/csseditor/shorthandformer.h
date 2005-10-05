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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.             *
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
    ShorthandFormer(){}
    ShorthandFormer( QMap<QString,QString> m );
    ~ShorthandFormer(){}
    QString compress();
    QMap<QString,QString> expand(const QString& propertyName, const QString& propertyValue);
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

  QString compressImplementation( const QString& prop, const QString& t, const QString& b, const QString& r, const QString& l, const QString& defValue);
  QString compressImplementation2( const QString& prop, const QString& after, const QString& before, const QString& defValue);
  QString compressImplementation3( const QString& prop, const QString& p1, const QString& p2, const QString& p3);

  QMap<QString,QString> expandCueProp(const QStringList& l);
  QMap<QString,QString> expandPauseProp(const QStringList& l);
  QMap<QString,QString> expandBackgroundProp(const QStringList& l);
  QMap<QString,QString> expandFontProp(const QStringList& l);
  QMap<QString,QString> expandOutlineProp(const QStringList& l);
  QMap<QString,QString> expandListstyleProp(const QStringList& l);
  QMap<QString,QString> expandBoxSide(const QString& subPropName, const QStringList& l);
  QMap<QString,QString> expandBorderProp(const QStringList& l);
  QMap<QString,QString> expandBox(const QString& subPropName, const QStringList& l);
  QMap<QString,QString> expandPaddingProp(const QStringList& l);
  QMap<QString,QString> expandMarginProp(const QStringList& l);

  QMap<QString,QString> expandImplementation(const QString& propertyName, const QStringList& l);
  QMap<QString,QString> expandImplementation2(const QString& propertyName, const QStringList& l);
 };

#endif
