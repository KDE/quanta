/***************************************************************************
                          cssdialog.cpp  -  description
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
** Form implementation generated from reading ui file 'css_dialog.ui'
**
** Created: Thu Oct 5 08:23:22 2000
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "cssdialog.h"
#include "cssdialog.moc"
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qframe.h>
#include <qlabel.h>
#include <klineedit.h>
#include <qpushbutton.h>
#include <qtabwidget.h>
#include <qwidget.h>
#include "colorcombo.h"
#include "filecombo.h"
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qradiobutton.h>
#include <qbuttongroup.h>

#include <klocale.h>

/*
 *  Constructs a CSSDialog which is a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
CSSDialog::CSSDialog( QString basePath, QWidget* parent,  const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
	  this->basePath = basePath;
    if ( !name )
  		setName( "CSSDialog" );
    resize( 611, 437 );
    setCaption( i18n( "CSS"  ) );
    setSizeGripEnabled( TRUE );
    grid = new QGridLayout( this );
    grid->setSpacing( 6 );
    grid->setMargin( 11 );

    TextLabel26 = new QLabel( this, "TextLabel26" );
    TextLabel26->setText( i18n( "Selector : "  ) );

    grid->addWidget( TextLabel26, 0, 1 );
    QSpacerItem* spacer = new QSpacerItem( 113, 20, QSizePolicy::Expanding, QSizePolicy::Fixed );
    grid->addItem( spacer, 1, 4 );
    QSpacerItem* spacer_2 = new QSpacerItem( 113, 20, QSizePolicy::Expanding, QSizePolicy::Fixed );
    grid->addItem( spacer_2, 1, 0 );
    QSpacerItem* spacer_3 = new QSpacerItem( 113, 20, QSizePolicy::Expanding, QSizePolicy::Fixed );
    grid->addItem( spacer_3, 1, 3 );
    QSpacerItem* spacer_4 = new QSpacerItem( 113, 20, QSizePolicy::Expanding, QSizePolicy::Fixed );
    grid->addItem( spacer_4, 1, 1 );
    QSpacerItem* spacer_5 = new QSpacerItem( 113, 20, QSizePolicy::Expanding, QSizePolicy::Fixed );
    grid->addItem( spacer_5, 1, 2 );

    selector = new KLineEdit( this, "selector" );

    grid->addMultiCellWidget( selector, 0, 0, 2, 3 );

    hbox = new QHBoxLayout;
    hbox->setSpacing( 6 );
    hbox->setMargin( 0 );
    QSpacerItem* spacer_6 = new QSpacerItem( 415, 20, QSizePolicy::Expanding, QSizePolicy::Fixed );
    hbox->addItem( spacer_6 );

    buttonOk = new QPushButton( this, "buttonOk" );
    buttonOk->setText( i18n( "&OK"  ) );
    buttonOk->setAutoDefault( TRUE );
    buttonOk->setDefault( TRUE );
    hbox->addWidget( buttonOk );

    buttonCancel = new QPushButton( this, "buttonCancel" );
    buttonCancel->setText( i18n( "&Cancel"  ) );
    buttonCancel->setAutoDefault( TRUE );
    hbox->addWidget( buttonCancel );

    grid->addMultiCellLayout( hbox, 4, 4, 0, 4 );

    TabWidget2 = new QTabWidget( this, "TabWidget2" );

    QWidget* widget = new QWidget( TabWidget2, "widget" );
    grid_2 = new QGridLayout( widget );
    grid_2->setSpacing( 6 );
    grid_2->setMargin( 11 );

    TextLabel1_2_2 = new QLabel( widget, "TextLabel1_2_2" );
    TextLabel1_2_2->setText( i18n( "Font Variant"  ) );

    grid_2->addWidget( TextLabel1_2_2, 2, 0 );

    TextLabel1 = new QLabel( widget, "TextLabel1" );
    TextLabel1->setText( i18n( "Font Family"  ) );

    grid_2->addWidget( TextLabel1, 0, 0 );

    font_family = new QComboBox( FALSE, widget, "font_family" );
    font_family->insertItem( "serif" );
    font_family->insertItem( "sans-serif" );
    font_family->insertItem( "cursive" );
    font_family->insertItem( "fantasy" );
    font_family->insertItem( "monospace" );
    font_family->setAutoCompletion( FALSE );
    font_family->setEditable( TRUE );

    grid_2->addMultiCellWidget( font_family, 0, 0, 1, 2 );

    font_style = new QComboBox( FALSE, widget, "font_style" );
    font_style->insertItem( "normal" );
    font_style->insertItem( "italic" );
    font_style->insertItem( "oblique" );
    font_style->setAutoCompletion( FALSE );
    font_style->setEditable( TRUE );

    grid_2->addMultiCellWidget( font_style, 1, 1, 1, 2 );

    TextLabel1_2_3 = new QLabel( widget, "TextLabel1_2_3" );
    TextLabel1_2_3->setText( i18n( "Font Weight"  ) );

    grid_2->addWidget( TextLabel1_2_3, 3, 0 );

    TextLabel1_2 = new QLabel( widget, "TextLabel1_2" );
    TextLabel1_2->setText( i18n( "Font Style"  ) );

    grid_2->addWidget( TextLabel1_2, 1, 0 );

    font_variant = new QComboBox( FALSE, widget, "font_variant" );
    font_variant->insertItem( "normal" );
    font_variant->insertItem( "small-caps" );
    font_variant->setAutoCompletion( FALSE );
    font_variant->setEditable( TRUE );

    grid_2->addMultiCellWidget( font_variant, 2, 2, 1, 2 );

    font_weight = new QComboBox( FALSE, widget, "font_weight" );
    font_weight->insertItem( "normal" );
    font_weight->insertItem( "bold" );
    font_weight->insertItem( "bolder" );
    font_weight->insertItem( "lighter" );
    font_weight->insertItem( "100" );
    font_weight->insertItem( "200" );
    font_weight->insertItem( "300" );
    font_weight->insertItem( "400" );
    font_weight->insertItem( "500" );
    font_weight->insertItem( "600" );
    font_weight->insertItem( "700" );
    font_weight->insertItem( "800" );
    font_weight->insertItem( "900" );
    font_weight->setAutoCompletion( FALSE );
    font_weight->setEditable( TRUE );

    grid_2->addMultiCellWidget( font_weight, 3, 3, 1, 2 );

    font_size = new QComboBox( FALSE, widget, "font_size" );
    font_size->insertItem( "xx-small" );
    font_size->insertItem( "x-small" );
    font_size->insertItem( "small" );
    font_size->insertItem( "medium" );
    font_size->insertItem( "large" );
    font_size->insertItem( "x-large" );
    font_size->insertItem( "xx-large" );
    font_size->insertItem( "" );
    font_size->insertItem( "larger" );
    font_size->insertItem( "smaller" );
    font_size->setAutoCompletion( FALSE );
    font_size->setEditable( TRUE );

    grid_2->addMultiCellWidget( font_size, 4, 4, 1, 2 );

    TextLabel1_2_3_2 = new QLabel( widget, "TextLabel1_2_3_2" );
    TextLabel1_2_3_2->setText( i18n( "Font Size"  ) );

    grid_2->addWidget( TextLabel1_2_3_2, 4, 0 );

    check_font = new QCheckBox( widget, "check_font" );
    check_font->setText( i18n( "Insert as line"  ) );

    grid_2->addWidget( check_font, 5, 0 );
    QSpacerItem* spacer_7 = new QSpacerItem( 20, 88, QSizePolicy::Fixed, QSizePolicy::Expanding );
    grid_2->addItem( spacer_7, 7, 0 );
    QSpacerItem* spacer_8 = new QSpacerItem( 184, 20, QSizePolicy::Expanding, QSizePolicy::Fixed );
    grid_2->addItem( spacer_8, 6, 0 );
    QSpacerItem* spacer_9 = new QSpacerItem( 183, 20, QSizePolicy::Expanding, QSizePolicy::Fixed );
    grid_2->addItem( spacer_9, 6, 1 );
    QSpacerItem* spacer_10 = new QSpacerItem( 184, 20, QSizePolicy::Expanding, QSizePolicy::Fixed );
    grid_2->addItem( spacer_10, 6, 2 );
    TabWidget2->insertTab( widget, i18n( "Font" ) );

    QWidget* widget_2 = new QWidget( TabWidget2, "widget_2" );
    grid_3 = new QGridLayout( widget_2 );
    grid_3->setSpacing( 8 );
    grid_3->setMargin( 11 );

    TextLabel2 = new QLabel( widget_2, "TextLabel2" );
    TextLabel2->setText( i18n( "Color"  ) );

    grid_3->addWidget( TextLabel2, 0, 0 );

    color = new ColorCombo( widget_2, "color" );

    grid_3->addMultiCellWidget( color, 0, 0, 1, 2 );

    background_color = new ColorCombo( widget_2, "background_color" );

    grid_3->addMultiCellWidget( background_color, 1, 1, 1, 2 );

    background_image = new FileCombo( basePath, widget_2, "background_image" );

    grid_3->addMultiCellWidget( background_image, 2, 2, 1, 2 );

    TextLabel4 = new QLabel( widget_2, "TextLabel4" );
    TextLabel4->setText( i18n( "Background Image"  ) );

    grid_3->addWidget( TextLabel4, 2, 0 );

    TextLabel3 = new QLabel( widget_2, "TextLabel3" );
    TextLabel3->setText( i18n( "Background Color"  ) );

    grid_3->addWidget( TextLabel3, 1, 0 );

    TextLabel5 = new QLabel( widget_2, "TextLabel5" );
    TextLabel5->setText( i18n( "Background Repeat"  ) );

    grid_3->addWidget( TextLabel5, 3, 0 );

    TextLabel6 = new QLabel( widget_2, "TextLabel6" );
    TextLabel6->setText( i18n( "Background Attacment"  ) );

    grid_3->addWidget( TextLabel6, 4, 0 );

    background_attachment = new QComboBox( FALSE, widget_2, "background_attachment" );
    background_attachment->insertItem( "scroll" );
    background_attachment->insertItem( "fixed" );
    background_attachment->setEditable( TRUE );
    background_attachment->setAutoCompletion( TRUE );

    grid_3->addMultiCellWidget( background_attachment, 4, 4, 1, 2 );

    background_repeat = new QComboBox( FALSE, widget_2, "background_repeat" );
    background_repeat->insertItem( "repeat" );
    background_repeat->insertItem( "repeat-x" );
    background_repeat->insertItem( "repeat-y" );
    background_repeat->insertItem( "no-repeat" );
    background_repeat->setEditable( TRUE );
    background_repeat->setAutoCompletion( TRUE );

    grid_3->addMultiCellWidget( background_repeat, 3, 3, 1, 2 );

    TextLabel7 = new QLabel( widget_2, "TextLabel7" );
    TextLabel7->setText( i18n( "Background Position"  ) );

    grid_3->addWidget( TextLabel7, 5, 0 );

    background_position_y = new QComboBox( FALSE, widget_2, "background_position_y" );
    background_position_y->insertItem( "top" );
    background_position_y->insertItem( "center" );
    background_position_y->insertItem( "bottom" );
    background_position_y->insertItem( "20%" );
    background_position_y->insertItem( "1cm" );
    background_position_y->insertItem( "10px" );
    background_position_y->setAutoCompletion( FALSE );
    background_position_y->setEditable( TRUE );

    grid_3->addWidget( background_position_y, 5, 1 );

    background_position_x = new QComboBox( FALSE, widget_2, "background_position_x" );
    background_position_x->insertItem( "left" );
    background_position_x->insertItem( "center" );
    background_position_x->insertItem( "right" );
    background_position_x->insertItem( "20%" );
    background_position_x->insertItem( "1cm" );
    background_position_x->insertItem( "10px" );
    background_position_x->setAutoCompletion( FALSE );
    background_position_x->setEditable( TRUE );

    grid_3->addWidget( background_position_x, 5, 2 );

    check_bg = new QCheckBox( widget_2, "check_bg" );
    check_bg->setText( i18n( "Insert as line"  ) );

    grid_3->addWidget( check_bg, 6, 0 );
    QSpacerItem* spacer_11 = new QSpacerItem( 20, 65, QSizePolicy::Fixed, QSizePolicy::Expanding );
    grid_3->addItem( spacer_11, 8, 0 );
    QSpacerItem* spacer_12 = new QSpacerItem( 182, 20, QSizePolicy::Expanding, QSizePolicy::Fixed );
    grid_3->addItem( spacer_12, 7, 0 );
    QSpacerItem* spacer_13 = new QSpacerItem( 183, 20, QSizePolicy::Expanding, QSizePolicy::Fixed );
    grid_3->addItem( spacer_13, 7, 1 );
    QSpacerItem* spacer_14 = new QSpacerItem( 182, 20, QSizePolicy::Expanding, QSizePolicy::Fixed );
    grid_3->addItem( spacer_14, 7, 2 );
    TabWidget2->insertTab( widget_2, i18n( "Color and BG" ) );

    QWidget* widget_3 = new QWidget( TabWidget2, "widget_3" );
    grid_4 = new QGridLayout( widget_3 );
    grid_4->setSpacing( 8 );
    grid_4->setMargin( 11 );

    TextLabel1_3 = new QLabel( widget_3, "TextLabel1_3" );
    TextLabel1_3->setText( i18n( "Word Spacing"  ) );

    grid_4->addWidget( TextLabel1_3, 0, 0 );

    TextLabel2_2 = new QLabel( widget_3, "TextLabel2_2" );
    TextLabel2_2->setText( i18n( "Letter Spacing"  ) );

    grid_4->addWidget( TextLabel2_2, 1, 0 );

    TextLabel5_2 = new QLabel( widget_3, "TextLabel5_2" );
    TextLabel5_2->setText( i18n( "Text Transformation"  ) );

    grid_4->addWidget( TextLabel5_2, 4, 0 );

    TextLabel6_2 = new QLabel( widget_3, "TextLabel6_2" );
    TextLabel6_2->setText( i18n( "Text Alignment"  ) );

    grid_4->addWidget( TextLabel6_2, 5, 0 );

    TextLabel7_2 = new QLabel( widget_3, "TextLabel7_2" );
    TextLabel7_2->setText( i18n( "Text Indentation"  ) );

    grid_4->addWidget( TextLabel7_2, 6, 0 );

    TextLabel8 = new QLabel( widget_3, "TextLabel8" );
    TextLabel8->setText( i18n( "Line Height"  ) );

    grid_4->addWidget( TextLabel8, 7, 0 );

    TextLabel4_2 = new QLabel( widget_3, "TextLabel4_2" );
    TextLabel4_2->setText( i18n( "Vertical Alignment"  ) );

    grid_4->addWidget( TextLabel4_2, 3, 0 );

    TextLabel3_2 = new QLabel( widget_3, "TextLabel3_2" );
    TextLabel3_2->setText( i18n( "Text Decoration"  ) );

    grid_4->addWidget( TextLabel3_2, 2, 0 );

    word_spacing = new QComboBox( FALSE, widget_3, "word_spacing" );
    word_spacing->insertItem( "normal" );
    word_spacing->insertItem( "2em" );
    word_spacing->insertItem( "-0.1ex" );
    word_spacing->insertItem( "20px" );
    word_spacing->insertItem( "0.1in" );
    word_spacing->insertItem( "0.2cm" );
    word_spacing->insertItem( "2mm" );
    word_spacing->insertItem( "3pt" );
    word_spacing->insertItem( "10pc" );
    word_spacing->setEditable( TRUE );

    grid_4->addMultiCellWidget( word_spacing, 0, 0, 1, 2 );

    letter_spacing = new QComboBox( FALSE, widget_3, "letter_spacing" );
    letter_spacing->insertItem( "normal" );
    letter_spacing->insertItem( "2em" );
    letter_spacing->insertItem( "-0.1ex" );
    letter_spacing->insertItem( "20px" );
    letter_spacing->insertItem( "0.1in" );
    letter_spacing->insertItem( "0.2cm" );
    letter_spacing->insertItem( "2mm" );
    letter_spacing->insertItem( "3pt" );
    letter_spacing->insertItem( "10pc" );
    letter_spacing->setEditable( TRUE );

    grid_4->addMultiCellWidget( letter_spacing, 1, 1, 1, 2 );

    text_decoration = new QComboBox( FALSE, widget_3, "text_decoration" );
    text_decoration->insertItem( "none" );
    text_decoration->insertItem( "underline" );
    text_decoration->insertItem( "overline" );
    text_decoration->insertItem( i18n( "line through" ) );
    text_decoration->insertItem( "blink" );
    text_decoration->setEditable( TRUE );
    text_decoration->setAutoCompletion( TRUE );

    grid_4->addMultiCellWidget( text_decoration, 2, 2, 1, 2 );

    vertical_align = new QComboBox( FALSE, widget_3, "vertical_align" );
    vertical_align->insertItem( "baseline" );
    vertical_align->insertItem( "sub" );
    vertical_align->insertItem( "super" );
    vertical_align->insertItem( "top" );
    vertical_align->insertItem( "text-top" );
    vertical_align->insertItem( "middle" );
    vertical_align->insertItem( "bottom" );
    vertical_align->insertItem( "text-bottom" );
    vertical_align->insertItem( "20%" );
    vertical_align->setEditable( TRUE );
    vertical_align->setAutoCompletion( TRUE );

    grid_4->addMultiCellWidget( vertical_align, 3, 3, 1, 2 );

    text_transform = new QComboBox( FALSE, widget_3, "text_transform" );
    text_transform->insertItem( "none" );
    text_transform->insertItem( "capitalize" );
    text_transform->insertItem( "uppercase" );
    text_transform->insertItem( "lowercase" );
    text_transform->setAutoCompletion( FALSE );
    text_transform->setEditable( TRUE );

    grid_4->addMultiCellWidget( text_transform, 4, 4, 1, 2 );

    text_align = new QComboBox( FALSE, widget_3, "text_align" );
    text_align->insertItem( "left" );
    text_align->insertItem( "right" );
    text_align->insertItem( "center" );
    text_align->insertItem( "justify" );
    text_align->setAutoCompletion( FALSE );
    text_align->setEditable( TRUE );

    grid_4->addMultiCellWidget( text_align, 5, 5, 1, 2 );

    text_indent = new QComboBox( FALSE, widget_3, "text_indent" );
    text_indent->insertItem( "2em" );
    text_indent->insertItem( "-0.1ex" );
    text_indent->insertItem( "20px" );
    text_indent->insertItem( "0.1in" );
    text_indent->insertItem( "0.2cm" );
    text_indent->insertItem( "2mm" );
    text_indent->insertItem( "3pt" );
    text_indent->insertItem( "10pc" );
    text_indent->insertItem( "20%" );
    text_indent->setEditable( TRUE );

    grid_4->addMultiCellWidget( text_indent, 6, 6, 1, 2 );

    line_height = new QComboBox( FALSE, widget_3, "line_height" );
    line_height->insertItem( "normal" );
    line_height->insertItem( "2em" );
    line_height->insertItem( "-0.1ex" );
    line_height->insertItem( "20px" );
    line_height->insertItem( "0.1in" );
    line_height->insertItem( "0.2cm" );
    line_height->insertItem( "2mm" );
    line_height->insertItem( "3pt" );
    line_height->insertItem( "10pc" );
    line_height->insertItem( "20%" );
    line_height->setEditable( TRUE );

    grid_4->addMultiCellWidget( line_height, 7, 7, 1, 2 );
    QSpacerItem* spacer_15 = new QSpacerItem( 20, 11, QSizePolicy::Fixed, QSizePolicy::Expanding );
    grid_4->addItem( spacer_15, 9, 0 );
    QSpacerItem* spacer_16 = new QSpacerItem( 182, 20, QSizePolicy::Expanding, QSizePolicy::Fixed );
    grid_4->addItem( spacer_16, 8, 0 );
    QSpacerItem* spacer_17 = new QSpacerItem( 183, 20, QSizePolicy::Expanding, QSizePolicy::Fixed );
    grid_4->addItem( spacer_17, 8, 1 );
    QSpacerItem* spacer_18 = new QSpacerItem( 182, 20, QSizePolicy::Expanding, QSizePolicy::Fixed );
    grid_4->addItem( spacer_18, 8, 2 );
    TabWidget2->insertTab( widget_3, i18n( "Text" ) );

    QWidget* widget_4 = new QWidget( TabWidget2, "widget_4" );
    grid_5 = new QGridLayout( widget_4 );
    grid_5->setSpacing( 8 );
    grid_5->setMargin( 11 );

    TextLabel11_2 = new QLabel( widget_4, "TextLabel11_2" );
    TextLabel11_2->setText( i18n( "Top"  ) );

    grid_5->addMultiCellWidget( TextLabel11_2, 1, 1, 0, 1 );

    TextLabel12_2 = new QLabel( widget_4, "TextLabel12_2" );
    TextLabel12_2->setText( i18n( "Right"  ) );

    grid_5->addMultiCellWidget( TextLabel12_2, 2, 2, 0, 1 );

    TextLabel13_2 = new QLabel( widget_4, "TextLabel13_2" );
    TextLabel13_2->setText( i18n( "Bottom"  ) );

    grid_5->addMultiCellWidget( TextLabel13_2, 3, 3, 0, 1 );

    TextLabel14_2 = new QLabel( widget_4, "TextLabel14_2" );
    TextLabel14_2->setText( i18n( "Left"  ) );

    grid_5->addMultiCellWidget( TextLabel14_2, 4, 4, 0, 1 );

    TextLabel15 = new QLabel( widget_4, "TextLabel15" );
    TextLabel15->setText( i18n( "Width"  ) );

    grid_5->addWidget( TextLabel15, 0, 2 );

    TextLabel16 = new QLabel( widget_4, "TextLabel16" );
    TextLabel16->setText( i18n( "Color"  ) );

    grid_5->addWidget( TextLabel16, 0, 3 );

    TextLabel17 = new QLabel( widget_4, "TextLabel17" );
    TextLabel17->setText( i18n( "Style"  ) );

    grid_5->addWidget( TextLabel17, 0, 4 );

    border_top_width = new QComboBox( FALSE, widget_4, "border_top_width" );
    border_top_width->insertItem( "thin" );
    border_top_width->insertItem( "medium" );
    border_top_width->insertItem( "thick" );
    border_top_width->insertItem( "2em" );
    border_top_width->insertItem( "-0.1ex" );
    border_top_width->insertItem( "20px" );
    border_top_width->insertItem( "0.1in" );
    border_top_width->insertItem( "0.2cm" );
    border_top_width->insertItem( "2mm" );
    border_top_width->insertItem( "3pt" );
    border_top_width->insertItem( "10pc" );
    border_top_width->setEditable( TRUE );

    grid_5->addWidget( border_top_width, 1, 2 );

    border_right_width = new QComboBox( FALSE, widget_4, "border_right_width" );
    border_right_width->insertItem( "thin" );
    border_right_width->insertItem( "medium" );
    border_right_width->insertItem( "thick" );
    border_right_width->insertItem( "2em" );
    border_right_width->insertItem( "-0.1ex" );
    border_right_width->insertItem( "20px" );
    border_right_width->insertItem( "0.1in" );
    border_right_width->insertItem( "0.2cm" );
    border_right_width->insertItem( "2mm" );
    border_right_width->insertItem( "3pt" );
    border_right_width->insertItem( "10pc" );
    border_right_width->setEditable( TRUE );

    grid_5->addWidget( border_right_width, 2, 2 );

    border_bottom_width = new QComboBox( FALSE, widget_4, "border_bottom_width" );
    border_bottom_width->insertItem( "thin" );
    border_bottom_width->insertItem( "medium" );
    border_bottom_width->insertItem( "thick" );
    border_bottom_width->insertItem( "2em" );
    border_bottom_width->insertItem( "-0.1ex" );
    border_bottom_width->insertItem( "20px" );
    border_bottom_width->insertItem( "0.1in" );
    border_bottom_width->insertItem( "0.2cm" );
    border_bottom_width->insertItem( "2mm" );
    border_bottom_width->insertItem( "3pt" );
    border_bottom_width->insertItem( "10pc" );
    border_bottom_width->setEditable( TRUE );

    grid_5->addWidget( border_bottom_width, 3, 2 );

    border_left_width = new QComboBox( FALSE, widget_4, "border_left_width" );
    border_left_width->insertItem( "thin" );
    border_left_width->insertItem( "medium" );
    border_left_width->insertItem( "thick" );
    border_left_width->insertItem( "2em" );
    border_left_width->insertItem( "-0.1ex" );
    border_left_width->insertItem( "20px" );
    border_left_width->insertItem( "0.1in" );
    border_left_width->insertItem( "0.2cm" );
    border_left_width->insertItem( "2mm" );
    border_left_width->insertItem( "3pt" );
    border_left_width->insertItem( "10pc" );
    border_left_width->setEditable( TRUE );

    grid_5->addWidget( border_left_width, 4, 2 );

    border_color_top = new ColorCombo( widget_4, "border_color_top" );

    grid_5->addWidget( border_color_top, 1, 3 );

    border_color_right = new ColorCombo( widget_4, "border_color_right" );

    grid_5->addWidget( border_color_right, 2, 3 );

    border_color_bottom = new ColorCombo( widget_4, "border_color_bottom" );

    grid_5->addWidget( border_color_bottom, 3, 3 );

    border_color_left = new ColorCombo( widget_4, "border_color_left" );

    grid_5->addWidget( border_color_left, 4, 3 );

    border_style_top = new QComboBox( FALSE, widget_4, "border_style_top" );
    border_style_top->insertItem( "none" );
    border_style_top->insertItem( "dotted" );
    border_style_top->insertItem( "slashed" );
    border_style_top->insertItem( "solid" );
    border_style_top->insertItem( "double" );
    border_style_top->insertItem( "groove" );
    border_style_top->insertItem( "ridge" );
    border_style_top->insertItem( "inset" );
    border_style_top->insertItem( "outset" );
    border_style_top->setEditable( TRUE );
    border_style_top->setAutoCompletion( TRUE );

    grid_5->addWidget( border_style_top, 1, 4 );

    border_style_right = new QComboBox( FALSE, widget_4, "border_style_right" );
    border_style_right->insertItem( "none" );
    border_style_right->insertItem( "dotted" );
    border_style_right->insertItem( "slashed" );
    border_style_right->insertItem( "solid" );
    border_style_right->insertItem( "double" );
    border_style_right->insertItem( "groove" );
    border_style_right->insertItem( "ridge" );
    border_style_right->insertItem( "inset" );
    border_style_right->insertItem( "outset" );
    border_style_right->setEditable( TRUE );
    border_style_right->setAutoCompletion( TRUE );

    grid_5->addWidget( border_style_right, 2, 4 );

    border_style_bottom = new QComboBox( FALSE, widget_4, "border_style_bottom" );
    border_style_bottom->insertItem( "none" );
    border_style_bottom->insertItem( "dotted" );
    border_style_bottom->insertItem( "slashed" );
    border_style_bottom->insertItem( "solid" );
    border_style_bottom->insertItem( "double" );
    border_style_bottom->insertItem( "groove" );
    border_style_bottom->insertItem( "ridge" );
    border_style_bottom->insertItem( "inset" );
    border_style_bottom->insertItem( "outset" );
    border_style_bottom->setEditable( TRUE );
    border_style_bottom->setAutoCompletion( TRUE );

    grid_5->addWidget( border_style_bottom, 3, 4 );

    border_style_left = new QComboBox( FALSE, widget_4, "border_style_left" );
    border_style_left->insertItem( "none" );
    border_style_left->insertItem( "dotted" );
    border_style_left->insertItem( "slashed" );
    border_style_left->insertItem( "solid" );
    border_style_left->insertItem( "double" );
    border_style_left->insertItem( "groove" );
    border_style_left->insertItem( "ridge" );
    border_style_left->insertItem( "inset" );
    border_style_left->insertItem( "outset" );
    border_style_left->setEditable( TRUE );
    border_style_left->setAutoCompletion( TRUE );

    grid_5->addWidget( border_style_left, 4, 4 );
    QSpacerItem* spacer_19 = new QSpacerItem( 128, 20, QSizePolicy::Expanding, QSizePolicy::Fixed );
    grid_5->addItem( spacer_19, 6, 0 );
    QSpacerItem* spacer_20 = new QSpacerItem( 128, 20, QSizePolicy::Expanding, QSizePolicy::Fixed );
    grid_5->addItem( spacer_20, 6, 2 );
    QSpacerItem* spacer_21 = new QSpacerItem( 127, 20, QSizePolicy::Expanding, QSizePolicy::Fixed );
    grid_5->addItem( spacer_21, 6, 3 );
    QSpacerItem* spacer_22 = new QSpacerItem( 128, 20, QSizePolicy::Expanding, QSizePolicy::Fixed );
    grid_5->addItem( spacer_22, 6, 4 );
    QSpacerItem* spacer_23 = new QSpacerItem( 20, 28, QSizePolicy::Fixed, QSizePolicy::Expanding );
    grid_5->addItem( spacer_23, 6, 1 );

    border_group = new QButtonGroup( widget_4, "border_group" );
    border_group->setTitle( i18n( ""  ) );
    border_group->setLineWidth( 0 );
    border_group->setColumnLayout(0, Qt::Vertical );
    border_group->layout()->setSpacing( 0 );
    border_group->layout()->setMargin( 0 );
    QVBoxLayout *vbox = new QVBoxLayout( border_group->layout() );
    vbox->setAlignment( Qt::AlignTop );
    vbox->setSpacing( 6 );
    vbox->setMargin( 11 );

    check_border_property = new QRadioButton( border_group, "check_border_property" );
    check_border_property->setText( i18n( "Insert values per property"  ) );
    check_border_property->setChecked( TRUE );
    vbox->addWidget( check_border_property );

    check_border_side = new QRadioButton( border_group, "check_border_side" );
    check_border_side->setText( i18n( "Insert values per border side"  ) );
    vbox->addWidget( check_border_side );

    check_border_common = new QRadioButton( border_group, "check_border_common" );
    check_border_common->setText( i18n( "General values for each sides"  ) );
    vbox->addWidget( check_border_common );

    grid_5->addMultiCellWidget( border_group, 5, 5, 0, 4 );
    TabWidget2->insertTab( widget_4, i18n( "Border" ) );

    QWidget* widget_5 = new QWidget( TabWidget2, "widget_5" );
    grid_6 = new QGridLayout( widget_5 );
    grid_6->setSpacing( 8 );
    grid_6->setMargin( 11 );

    TextLabel22 = new QLabel( widget_5, "TextLabel22" );
    TextLabel22->setText( i18n( "Display"  ) );

    grid_6->addWidget( TextLabel22, 0, 0 );

    display = new QComboBox( FALSE, widget_5, "display" );
    display->insertItem( "block" );
    display->insertItem( "inline" );
    display->insertItem( "list-item" );
    display->insertItem( "none" );
    display->setEditable( TRUE );
    display->setAutoCompletion( TRUE );

    grid_6->addMultiCellWidget( display, 0, 0, 1, 2 );

    TextLabel23 = new QLabel( widget_5, "TextLabel23" );
    TextLabel23->setText( i18n( "Whitespace"  ) );

    grid_6->addWidget( TextLabel23, 1, 0 );

    white_space = new QComboBox( FALSE, widget_5, "white_space" );
    white_space->insertItem( "normal" );
    white_space->insertItem( "pre" );
    white_space->insertItem( "nowrap" );
    white_space->setAutoCompletion( FALSE );
    white_space->setEditable( TRUE );

    grid_6->addMultiCellWidget( white_space, 1, 1, 1, 2 );

    Line9 = new QFrame( widget_5, "Line9" );
    Line9->setFrameStyle( QFrame::HLine | QFrame::Sunken );

    grid_6->addMultiCellWidget( Line9, 2, 2, 0, 2 );

    list_style_type = new QComboBox( FALSE, widget_5, "list_style_type" );
    list_style_type->setEditable( TRUE );
    list_style_type->setAutoCompletion( TRUE );

    grid_6->addMultiCellWidget( list_style_type, 3, 3, 1, 2 );

    TextLabel24 = new QLabel( widget_5, "TextLabel24" );
    TextLabel24->setText( i18n( "List Style Type"  ) );

    grid_6->addWidget( TextLabel24, 3, 0 );

    TextLabel24_2 = new QLabel( widget_5, "TextLabel24_2" );
    TextLabel24_2->setText( i18n( "List Style Image"  ) );

    grid_6->addWidget( TextLabel24_2, 4, 0 );

    list_style_image = new FileCombo( basePath, widget_5, "list_style_image" );

    grid_6->addMultiCellWidget( list_style_image, 4, 4, 1, 2 );

    TextLabel25 = new QLabel( widget_5, "TextLabel25" );
    TextLabel25->setText( i18n( "List Style Position"  ) );

    grid_6->addWidget( TextLabel25, 5, 0 );

    check_list_style = new QCheckBox( widget_5, "check_list_style" );
    check_list_style->setText( i18n( "Insert as line"  ) );

    grid_6->addWidget( check_list_style, 6, 0 );
    QSpacerItem* spacer_24 = new QSpacerItem( 20, 70, QSizePolicy::Fixed, QSizePolicy::Expanding );
    grid_6->addItem( spacer_24, 8, 0 );
    QSpacerItem* spacer_25 = new QSpacerItem( 182, 20, QSizePolicy::Expanding, QSizePolicy::Fixed );
    grid_6->addItem( spacer_25, 7, 0 );
    QSpacerItem* spacer_26 = new QSpacerItem( 183, 20, QSizePolicy::Expanding, QSizePolicy::Fixed );
    grid_6->addItem( spacer_26, 7, 1 );
    QSpacerItem* spacer_27 = new QSpacerItem( 182, 20, QSizePolicy::Expanding, QSizePolicy::Fixed );
    grid_6->addItem( spacer_27, 7, 2 );

    list_style_position = new QComboBox( FALSE, widget_5, "list_style_position" );
    list_style_position->insertItem( "inside" );
    list_style_position->insertItem( "outside" );
    list_style_position->setAutoCompletion( FALSE );
    list_style_position->setEditable( TRUE );

    grid_6->addMultiCellWidget( list_style_position, 5, 5, 1, 2 );
    TabWidget2->insertTab( widget_5, i18n( "Classification" ) );

    QWidget* widget_6 = new QWidget( TabWidget2, "widget_6" );
    grid_7 = new QGridLayout( widget_6 );
    grid_7->setSpacing( 8 );
    grid_7->setMargin( 11 );

    TextLabel11 = new QLabel( widget_6, "TextLabel11" );
    TextLabel11->setText( i18n( "Top"  ) );

    grid_7->addWidget( TextLabel11, 1, 0 );

    TextLabel12 = new QLabel( widget_6, "TextLabel12" );
    TextLabel12->setText( i18n( "Right"  ) );

    grid_7->addWidget( TextLabel12, 2, 0 );

    TextLabel13 = new QLabel( widget_6, "TextLabel13" );
    TextLabel13->setText( i18n( "Bottom"  ) );

    grid_7->addWidget( TextLabel13, 3, 0 );

    TextLabel14 = new QLabel( widget_6, "TextLabel14" );
    TextLabel14->setText( i18n( "Left"  ) );

    grid_7->addWidget( TextLabel14, 4, 0 );

    TextLabel9 = new QLabel( widget_6, "TextLabel9" );
    TextLabel9->setText( i18n( "Margin"  ) );

    grid_7->addMultiCellWidget( TextLabel9, 0, 0, 1, 2 );
    QSpacerItem* spacer_28 = new QSpacerItem( 20, 81, QSizePolicy::Fixed, QSizePolicy::Expanding );
    grid_7->addMultiCell( spacer_28, 7, 7, 1, 2 );

    margin_right = new QComboBox( FALSE, widget_6, "margin_right" );
    margin_right->insertItem( "auto" );
    margin_right->insertItem( "2em" );
    margin_right->insertItem( "-0.1ex" );
    margin_right->insertItem( "20px" );
    margin_right->insertItem( "0.1in" );
    margin_right->insertItem( "0.2cm" );
    margin_right->insertItem( "2mm" );
    margin_right->insertItem( "3pt" );
    margin_right->insertItem( "10pc" );
    margin_right->setEditable( TRUE );

    grid_7->addMultiCellWidget( margin_right, 2, 2, 1, 2 );

    margin_top = new QComboBox( FALSE, widget_6, "margin_top" );
    margin_top->insertItem( "auto" );
    margin_top->insertItem( "2em" );
    margin_top->insertItem( "-0.1ex" );
    margin_top->insertItem( "20px" );
    margin_top->insertItem( "0.1in" );
    margin_top->insertItem( "0.2cm" );
    margin_top->insertItem( "2mm" );
    margin_top->insertItem( "3pt" );
    margin_top->insertItem( "10pc" );
    margin_top->setEditable( TRUE );

    grid_7->addMultiCellWidget( margin_top, 1, 1, 1, 2 );

    margin_bottom = new QComboBox( FALSE, widget_6, "margin_bottom" );
    margin_bottom->insertItem( "auto" );
    margin_bottom->insertItem( "2em" );
    margin_bottom->insertItem( "-0.1ex" );
    margin_bottom->insertItem( "20px" );
    margin_bottom->insertItem( "0.1in" );
    margin_bottom->insertItem( "0.2cm" );
    margin_bottom->insertItem( "2mm" );
    margin_bottom->insertItem( "3pt" );
    margin_bottom->insertItem( "10pc" );
    margin_bottom->setEditable( TRUE );

    grid_7->addMultiCellWidget( margin_bottom, 3, 3, 1, 2 );

    margin_left = new QComboBox( FALSE, widget_6, "margin_left" );
    margin_left->insertItem( "auto" );
    margin_left->insertItem( "2em" );
    margin_left->insertItem( "-0.1ex" );
    margin_left->insertItem( "20px" );
    margin_left->insertItem( "0.1in" );
    margin_left->insertItem( "0.2cm" );
    margin_left->insertItem( "2mm" );
    margin_left->insertItem( "3pt" );
    margin_left->insertItem( "10pc" );
    margin_left->setEditable( TRUE );

    grid_7->addMultiCellWidget( margin_left, 4, 4, 1, 2 );

    check_margin = new QCheckBox( widget_6, "check_margin" );
    check_margin->setText( i18n( "Insert as line"  ) );

    grid_7->addMultiCellWidget( check_margin, 5, 5, 1, 2 );

    Line1 = new QFrame( widget_6, "Line1" );
    Line1->setFrameStyle( QFrame::VLine | QFrame::Sunken );

    grid_7->addMultiCellWidget( Line1, 0, 6, 3, 3 );

    check_padding = new QCheckBox( widget_6, "check_padding" );
    check_padding->setText( i18n( "Insert as line"  ) );

    grid_7->addMultiCellWidget( check_padding, 5, 5, 4, 5 );

    padding_left = new QComboBox( FALSE, widget_6, "padding_left" );
    padding_left->insertItem( "auto" );
    padding_left->insertItem( "2em" );
    padding_left->insertItem( "-0.1ex" );
    padding_left->insertItem( "20px" );
    padding_left->insertItem( "0.1in" );
    padding_left->insertItem( "0.2cm" );
    padding_left->insertItem( "2mm" );
    padding_left->insertItem( "3pt" );
    padding_left->insertItem( "10pc" );
    padding_left->setEditable( TRUE );

    grid_7->addMultiCellWidget( padding_left, 4, 4, 4, 5 );

    padding_bottom = new QComboBox( FALSE, widget_6, "padding_bottom" );
    padding_bottom->insertItem( "auto" );
    padding_bottom->insertItem( "2em" );
    padding_bottom->insertItem( "-0.1ex" );
    padding_bottom->insertItem( "20px" );
    padding_bottom->insertItem( "0.1in" );
    padding_bottom->insertItem( "0.2cm" );
    padding_bottom->insertItem( "2mm" );
    padding_bottom->insertItem( "3pt" );
    padding_bottom->insertItem( "10pc" );
    padding_bottom->setEditable( TRUE );

    grid_7->addMultiCellWidget( padding_bottom, 3, 3, 4, 5 );

    padding_right = new QComboBox( FALSE, widget_6, "padding_right" );
    padding_right->insertItem( "auto" );
    padding_right->insertItem( "2em" );
    padding_right->insertItem( "-0.1ex" );
    padding_right->insertItem( "20px" );
    padding_right->insertItem( "0.1in" );
    padding_right->insertItem( "0.2cm" );
    padding_right->insertItem( "2mm" );
    padding_right->insertItem( "3pt" );
    padding_right->insertItem( "10pc" );
    padding_right->setEditable( TRUE );

    grid_7->addMultiCellWidget( padding_right, 2, 2, 4, 5 );

    padding_top = new QComboBox( FALSE, widget_6, "padding_top" );
    padding_top->insertItem( "auto" );
    padding_top->insertItem( "2em" );
    padding_top->insertItem( "-0.1ex" );
    padding_top->insertItem( "20px" );
    padding_top->insertItem( "0.1in" );
    padding_top->insertItem( "0.2cm" );
    padding_top->insertItem( "2mm" );
    padding_top->insertItem( "3pt" );
    padding_top->insertItem( "10pc" );
    padding_top->setEditable( TRUE );

    grid_7->addMultiCellWidget( padding_top, 1, 1, 4, 5 );

    TextLabel10 = new QLabel( widget_6, "TextLabel10" );
    TextLabel10->setText( i18n( "Padding"  ) );

    grid_7->addMultiCellWidget( TextLabel10, 0, 0, 4, 5 );
    QSpacerItem* spacer_29 = new QSpacerItem( 104, 20, QSizePolicy::Expanding, QSizePolicy::Fixed );
    grid_7->addItem( spacer_29, 6, 5 );
    QSpacerItem* spacer_30 = new QSpacerItem( 104, 20, QSizePolicy::Expanding, QSizePolicy::Fixed );
    grid_7->addItem( spacer_30, 6, 1 );
    QSpacerItem* spacer_31 = new QSpacerItem( 104, 20, QSizePolicy::Expanding, QSizePolicy::Fixed );
    grid_7->addItem( spacer_31, 6, 2 );
    QSpacerItem* spacer_32 = new QSpacerItem( 104, 20, QSizePolicy::Expanding, QSizePolicy::Fixed );
    grid_7->addItem( spacer_32, 6, 4 );
    QSpacerItem* spacer_33 = new QSpacerItem( 104, 20, QSizePolicy::Expanding, QSizePolicy::Fixed );
    grid_7->addItem( spacer_33, 6, 0 );
    TabWidget2->insertTab( widget_6, i18n( "Margin Padding" ) );

    QWidget* widget_7 = new QWidget( TabWidget2, "widget_7" );
    grid_8 = new QGridLayout( widget_7 );
    grid_8->setSpacing( 8 );
    grid_8->setMargin( 11 );

    TextLabel18 = new QLabel( widget_7, "TextLabel18" );
    TextLabel18->setText( i18n( "Width"  ) );

    grid_8->addWidget( TextLabel18, 0, 0 );

    TextLabel19 = new QLabel( widget_7, "TextLabel19" );
    TextLabel19->setText( i18n( "Height"  ) );

    grid_8->addWidget( TextLabel19, 1, 0 );

    width = new QComboBox( FALSE, widget_7, "width" );
    width->insertItem( "auto" );
    width->insertItem( "2em" );
    width->insertItem( "-0.1ex" );
    width->insertItem( "20px" );
    width->insertItem( "0.1in" );
    width->insertItem( "0.2cm" );
    width->insertItem( "2mm" );
    width->insertItem( "3pt" );
    width->insertItem( "10pc" );
    width->setEditable( TRUE );

    grid_8->addMultiCellWidget( width, 0, 0, 1, 2 );

    height = new QComboBox( FALSE, widget_7, "height" );
    height->insertItem( "auto" );
    height->insertItem( "2em" );
    height->insertItem( "-0.1ex" );
    height->insertItem( "20px" );
    height->insertItem( "0.1in" );
    height->insertItem( "0.2cm" );
    height->insertItem( "2mm" );
    height->insertItem( "3pt" );
    height->insertItem( "10pc" );
    height->setEditable( TRUE );

    grid_8->addMultiCellWidget( height, 1, 1, 1, 2 );

    TextLabel20 = new QLabel( widget_7, "TextLabel20" );
    TextLabel20->setText( i18n( "Float"  ) );

    grid_8->addWidget( TextLabel20, 2, 0 );

    Float = new QComboBox( FALSE, widget_7, "float" );
    Float->insertItem( "left" );
    Float->insertItem( "right" );
    Float->insertItem( "none" );
    Float->setAutoCompletion( FALSE );
    Float->setEditable( TRUE );

    grid_8->addMultiCellWidget( Float, 2, 2, 1, 2 );

    TextLabel21 = new QLabel( widget_7, "TextLabel21" );
    TextLabel21->setText( i18n( "Clear"  ) );

    grid_8->addWidget( TextLabel21, 3, 0 );

    clear = new QComboBox( FALSE, widget_7, "clear" );
    clear->insertItem( "none" );
    clear->insertItem( "left" );
    clear->insertItem( "right" );
    clear->insertItem( "both" );
    clear->setAutoCompletion( FALSE );
    clear->setEditable( TRUE );

    grid_8->addMultiCellWidget( clear, 3, 3, 1, 2 );
    QSpacerItem* spacer_34 = new QSpacerItem( 20, 131, QSizePolicy::Fixed, QSizePolicy::Expanding );
    grid_8->addItem( spacer_34, 5, 0 );
    QSpacerItem* spacer_35 = new QSpacerItem( 182, 20, QSizePolicy::Expanding, QSizePolicy::Fixed );
    grid_8->addItem( spacer_35, 4, 0 );
    QSpacerItem* spacer_36 = new QSpacerItem( 183, 20, QSizePolicy::Expanding, QSizePolicy::Fixed );
    grid_8->addItem( spacer_36, 4, 1 );
    QSpacerItem* spacer_37 = new QSpacerItem( 182, 20, QSizePolicy::Expanding, QSizePolicy::Fixed );
    grid_8->addItem( spacer_37, 4, 2 );
    TabWidget2->insertTab( widget_7, i18n( "Box" ) );

    grid->addMultiCellWidget( TabWidget2, 2, 2, 0, 4 );

    // signals and slots connections
    connect( buttonOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );

}

/*
 *  Destroys the object and frees any allocated resources
 */
CSSDialog::~CSSDialog()
{
    // no need to delete child widgets, Qt does it all for us
}

