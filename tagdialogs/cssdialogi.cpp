/***************************************************************************
                          cssdialogi.cpp  -  description
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

#include "cssdialogi.h"
#include "cssdialogi.moc"
#include <klineedit.h>
#include <qcombobox.h>
#include <qcheckbox.h>
#include <qradiobutton.h>
#include "filecombo.h"
#include "colorcombo.h"


/*
 *  Constructs a CSSDialogI which is a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
CSSDialogI::CSSDialogI( QString basePath, QWidget* parent,  const char* name, bool modal, WFlags fl )
    : CSSDialog( basePath, parent, name, modal, fl )
{
}

/*
 *  Destroys the object and frees any allocated resources
 */
CSSDialogI::~CSSDialogI()
{
    // no need to delete child widgets, Qt does it all for us
}

#define ADD_CSS_STR( var, str ) if ( !var.isEmpty() ) text += QString(str) + var +";\n";
#define ADD_CSS_VAR( var ) if ( !var.isEmpty() ) text += QString(" ") + var;


QString CSSDialogI::data()
{
  QString text = "";

  QString f_family = font_family ->currentText();
  QString f_size   = font_size   ->currentText();
  QString f_style  = font_style  ->currentText();
  QString f_variant= font_variant->currentText();
  QString f_weight = font_weight ->currentText();

  if ( check_font->isChecked() ) {
  	text += " font:";
  	ADD_CSS_VAR( f_style )
  	ADD_CSS_VAR( f_variant )
  	ADD_CSS_VAR( f_weight )

  	text += QString(" ") + f_size + " " + f_family+";\n";
  }
  else {
  	ADD_CSS_STR( f_family ," font-family: ")
  	ADD_CSS_STR( f_size ," font-size: ")
  	ADD_CSS_STR( f_style ," font-style: ")
  	ADD_CSS_STR( f_variant ," font-variant: ")
  	ADD_CSS_STR( f_weight ," font-weight: ")
  }


  ADD_CSS_STR( color->colorName()," color: " )

  QString b_color  = background_color->colorName();
  QString b_img    = background_attachment->currentText();
  QString b_repeat = background_repeat->currentText();
  QString b_att    = background_attachment->currentText();
  QString b_pos_x  = background_position_x->currentText();
  QString b_pos_y  = background_position_y->currentText();

  QString b_pos = b_pos_y;
  if ( !b_pos_x.isEmpty() )
  	b_pos += " " + b_pos_x;


  if ( check_bg->isChecked() ) {
  	text += " background: ";
  	ADD_CSS_VAR( b_color )
  	ADD_CSS_VAR( b_img )
  	ADD_CSS_VAR( b_repeat )
  	ADD_CSS_VAR( b_att )
  	ADD_CSS_VAR( b_pos )
  	text += ";\n";
  }
  else {
  	ADD_CSS_STR( b_color, " background-color: " )
  	ADD_CSS_STR( b_img, " background-image: " )
  	ADD_CSS_STR( b_repeat," background-repeat: " )
  	ADD_CSS_STR( b_att, " background_attachment: " )
  	ADD_CSS_STR( b_pos, " background-position: " )
  }

  ADD_CSS_STR ( word_spacing->currentText(), " word-spacing: " )
  ADD_CSS_STR ( letter_spacing->currentText(), " letter-spacing: " )
  ADD_CSS_STR ( text_decoration->currentText(), " text-decoration: " )
  ADD_CSS_STR ( word_spacing->currentText(), " word-spacing: " )
  ADD_CSS_STR ( vertical_align->currentText(), " vertical-alignment: " )
  ADD_CSS_STR ( text_transform->currentText(), " text-transformation: " )
  ADD_CSS_STR ( text_align->currentText(), " text-alignment: " )
  ADD_CSS_STR ( text_indent->currentText(), " text-indentation: " )
  ADD_CSS_STR ( line_height->currentText(), " line-height: " )

  if ( check_margin->isChecked() ) {
  	text += " margin: ";
  	ADD_CSS_VAR( margin_top->currentText() )
  	ADD_CSS_VAR( margin_right->currentText() )
  	ADD_CSS_VAR( margin_bottom->currentText() )
  	ADD_CSS_VAR( margin_left->currentText() )
  	text += QString(";\n");
  }
  else {
  	ADD_CSS_STR( margin_top->currentText(),   " margin-top: " )
  	ADD_CSS_STR( margin_right->currentText(), " margin-right: " )
  	ADD_CSS_STR( margin_bottom->currentText()," margin-bottom: " )
  	ADD_CSS_STR( margin_left->currentText(),  " margin-left: " )
  }

  if ( check_padding->isChecked() ) {
  	text += " padding: ";
  	ADD_CSS_VAR( padding_top->currentText() )
  	ADD_CSS_VAR( padding_right->currentText() )
  	ADD_CSS_VAR( padding_bottom->currentText() )
  	ADD_CSS_VAR( padding_left->currentText() )
  	text += QString(";\n");
  }
  else {
  	ADD_CSS_STR( padding_top->currentText(),   " padding-top: " )
  	ADD_CSS_STR( padding_right->currentText(), " padding-right: " )
  	ADD_CSS_STR( padding_bottom->currentText()," padding-bottom: " )
  	ADD_CSS_STR( padding_left->currentText(),  " padding-left: " )
  }

  QString b_w_t = border_top_width->currentText();
  QString b_w_r = border_right_width->currentText();
  QString b_w_b = border_bottom_width->currentText();
  QString b_w_l = border_left_width->currentText();

  QString b_s_t = border_style_top->currentText();
  QString b_s_r = border_style_right->currentText();
  QString b_s_b = border_style_bottom->currentText();
  QString b_s_l = border_style_left->currentText();

  QString b_c_t = border_color_top->colorName();
  QString b_c_r = border_color_right->colorName();
  QString b_c_b = border_color_bottom->colorName();
  QString b_c_l = border_color_left->colorName();

  QString b_w = b_w_t+" "+b_w_r+" "+b_w_b+" "+b_w_l;
  QString b_s = b_s_t+" "+b_s_r+" "+b_s_b+" "+b_s_l;
  QString b_c = b_c_t+" "+b_c_r+" "+b_c_b+" "+b_c_l;

  QString b_t = b_w_t+" "+b_s_t+" "+b_c_t;
  QString b_r = b_w_r+" "+b_s_r+" "+b_c_r;
  QString b_b = b_w_b+" "+b_s_b+" "+b_c_b;
  QString b_l = b_w_l+" "+b_s_l+" "+b_c_l;

  if ( check_border_property->isChecked() ) {
  	if ( b_w != "   " )
  		text += " border-width: " + b_w + ";\n";
  	if ( b_s != "   " )
  		text += " border-style: " + b_s + ";\n";
  	if ( b_c != "   " )
  		text += " border-color: " + b_c + ";\n";
  }

  if ( check_border_side->isChecked() ) {
  	if ( b_t != "  " )
  		text += " top-border: " + b_t + ";\n";
  	if ( b_r != "  " )
  		text += " right-border: " + b_r + ";\n";
  	if ( b_b != "  " )
  		text += " bottom-border: " + b_b + ";\n";
  	if ( b_l != "  " )
  		text += " left-border: " + b_l + ";\n";
  }

  if ( check_border_common->isChecked() ) {
  	if ( b_t != "  " )
  		text += " border: " + b_t + ";\n";
  }

  ADD_CSS_STR( width->currentText(),   " width: " )
  ADD_CSS_STR( height->currentText(),   " height: " )
  ADD_CSS_STR( Float->currentText(),   " float: " )
  ADD_CSS_STR( clear->currentText(),   " clear: " )

  ADD_CSS_STR( display->currentText(),   " display: " )
  ADD_CSS_STR( white_space->currentText(),   " white-space: " )

  if ( check_list_style->isChecked() ) {
  	text+=" list-style: ";
  	ADD_CSS_VAR( list_style_type->currentText() )
  	ADD_CSS_VAR( list_style_position->currentText() )
  	ADD_CSS_VAR( list_style_image->text() )
  }
  else {
  	ADD_CSS_STR( list_style_type->currentText(), " list-style-type: " )
  	ADD_CSS_STR( list_style_position->currentText(), " list-style-position: " )
  	ADD_CSS_STR( list_style_image->text(), " list-style-image: " )
  	text += ";\n";
  }

	return ( selector->text() + "\n{\n" + text + "}\n"  );

}

#undef ADD_CSS_STR
#undef ADD_CSS_VAR




