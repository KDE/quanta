/***************************************************************************
                          cssdialog.h  -  description
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
/****************************************************************************
** Form interface generated from reading ui file 'css_dialog.ui'
**
** Created: Thu Oct 5 08:22:31 2000
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef CSSDIALOG_H
#define CSSDIALOG_H

#include <qdialog.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class ColorCombo;
class FileCombo;
class QCheckBox;
class QComboBox;
class QRadioButton;
class QFrame;
class QLabel;
class QLineEdit;
class QPushButton;
class QTabWidget;
class QWidget;
class QButtonGroup;

class CSSDialog : public QDialog
{ 
    Q_OBJECT

public:
    CSSDialog( QString basePath, QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~CSSDialog();

    QLabel* TextLabel26;
    QLineEdit* selector;
    QPushButton* buttonOk;
    QPushButton* buttonCancel;
    QTabWidget* TabWidget2;
    QLabel* TextLabel1_2_2;
    QLabel* TextLabel1;
    QComboBox* font_family;
    QComboBox* font_style;
    QLabel* TextLabel1_2_3;
    QLabel* TextLabel1_2;
    QComboBox* font_variant;
    QComboBox* font_weight;
    QComboBox* font_size;
    QLabel* TextLabel1_2_3_2;
    QCheckBox* check_font;
    QLabel* TextLabel2;
    ColorCombo* color;
    ColorCombo* background_color;
    FileCombo* background_image;
    QLabel* TextLabel4;
    QLabel* TextLabel3;
    QLabel* TextLabel5;
    QLabel* TextLabel6;
    QComboBox* background_attachment;
    QComboBox* background_repeat;
    QLabel* TextLabel7;
    QComboBox* background_position_y;
    QComboBox* background_position_x;
    QCheckBox* check_bg;
    QLabel* TextLabel1_3;
    QLabel* TextLabel2_2;
    QLabel* TextLabel5_2;
    QLabel* TextLabel6_2;
    QLabel* TextLabel7_2;
    QLabel* TextLabel8;
    QLabel* TextLabel4_2;
    QLabel* TextLabel3_2;
    QComboBox* word_spacing;
    QComboBox* letter_spacing;
    QComboBox* text_decoration;
    QComboBox* vertical_align;
    QComboBox* text_transform;
    QComboBox* text_align;
    QComboBox* text_indent;
    QComboBox* line_height;
    QLabel* TextLabel11_2;
    QLabel* TextLabel12_2;
    QLabel* TextLabel13_2;
    QLabel* TextLabel14_2;
    QLabel* TextLabel15;
    QLabel* TextLabel16;
    QLabel* TextLabel17;
    QComboBox* border_top_width;
    QComboBox* border_right_width;
    QComboBox* border_bottom_width;
    QComboBox* border_left_width;
    ColorCombo* border_color_top;
    ColorCombo* border_color_right;
    ColorCombo* border_color_bottom;
    ColorCombo* border_color_left;
    QComboBox* border_style_top;
    QComboBox* border_style_right;
    QComboBox* border_style_bottom;
    QComboBox* border_style_left;
    QRadioButton* check_border_property;
    QRadioButton* check_border_side;
    QRadioButton* check_border_common;
    QLabel* TextLabel22;
    QComboBox* display;
    QLabel* TextLabel23;
    QComboBox* white_space;
    QFrame* Line9;
    QComboBox* list_style_type;
    QLabel* TextLabel24;
    QLabel* TextLabel24_2;
    FileCombo* list_style_image;
    QLabel* TextLabel25;
    QCheckBox* check_list_style;
    QComboBox* list_style_position;
    QLabel* TextLabel11;
    QLabel* TextLabel12;
    QLabel* TextLabel13;
    QLabel* TextLabel14;
    QLabel* TextLabel9;
    QComboBox* margin_right;
    QComboBox* margin_top;
    QComboBox* margin_bottom;
    QComboBox* margin_left;
    QCheckBox* check_margin;
    QFrame* Line1;
    QCheckBox* check_padding;
    QComboBox* padding_left;
    QComboBox* padding_bottom;
    QComboBox* padding_right;
    QComboBox* padding_top;
    QLabel* TextLabel10;
    QLabel* TextLabel18;
    QLabel* TextLabel19;
    QComboBox* width;
    QComboBox* height;
    QLabel* TextLabel20;
    QComboBox* Float;
    QLabel* TextLabel21;
    QComboBox* clear;
    QButtonGroup* border_group;

    QString basePath;

protected:
    QHBoxLayout* hbox;
    QGridLayout* grid;
    QGridLayout* grid_2;
    QGridLayout* grid_3;
    QGridLayout* grid_4;
    QGridLayout* grid_5;
    QGridLayout* grid_6;
    QGridLayout* grid_7;
    QGridLayout* grid_8;
};

#endif // CSSDIALOG_H
