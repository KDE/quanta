/***************************************************************************
                          cssselectoreditor.cpp  -  description
                             -------------------
    begin                : dom ago 25 2002
    copyright            : (C) 2002 by Andras Mantia, Andrea Bergia
    email                : amantia@freemail.hu, andreabergia@yahoo.it
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

// QT includes
#include <qstringlist.h>
#include <qregexp.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include <qradiobutton.h>

// KDE includes
#include <klocale.h>
#include <kapp.h>
#include <kdebug.h>
#include <kurl.h>
#include <kpushbutton.h>
#include <klineedit.h>
#include <kcombobox.h>

// App includes
#include "cssselectoreditor.h"
#include "colorcombo.h"
#include "filecombo.h"


CSSSelectorEditor::CSSSelectorEditor(QString code, bool inlin,
	QWidget *parent, const char *name)
	: CSSEditSelectorS (parent, name), code_inline (inlin)
{
	// UI things
	setCaption(name);

	connect(buttonOk, SIGNAL(clicked()), SLOT(accept()));
	connect(buttonCancel, SIGNAL(clicked()), SLOT(reject()));

	// Insert the widgets value from the css code
	widgetFromCode(code);
}

CSSSelectorEditor::~CSSSelectorEditor()
{
}

/** Get the CSS code based on the widget's values */
QString CSSSelectorEditor::code()
{
	// We'll put in properties all the CSS properties which values aren't
	// the default
	QStringList properties;

	// Font
	QString f_family = comboFontFamily->currentText();
	QString f_size = comboFontSize->currentText();
	QString f_style = comboFontStyle->currentText();
	QString f_variant = comboFontVariant->currentText();
	QString f_weight = comboFontWeight->currentText();

	if ( checkFontInline->isChecked() ) {
		// Special case for inline rule
		QString temp = "font:";

		if (f_style != "normal" && f_style != "")
			temp += " " + f_style;
		if (f_variant != "normal" && f_variant != "")
			temp += " " + f_variant;
		if (f_weight != "normal" && f_weight != "")
			temp += " " + f_weight;

		// If the previous properties aren't set, and the size and the family are blank or the
		// default, we don't insert the font rule at all
		if ((f_size == "normal" || f_size == "") && f_family == "" && temp == "font:")
			temp = "";
		else
			temp += " " + f_size + " " + f_family + ";\n";

		properties += temp;
	} else {
		// Every properties is splitted on a different line
		if (f_family != "" && f_family != "")
			properties += "font-family: " + f_family;
		if (f_size != "medium" && f_size != "")
			properties += "font-size: " + f_size;
		if (f_style != "normal" && f_style != "")
			properties += "font-style: " + f_style;
		if (f_variant != "normal" && f_variant != "")
			properties += "font-variant: " + f_variant;
		if (f_weight != "normal" && f_weight != "")
			properties += "font-weight: " + f_weight;
	}

	// Text color
	if (ccomboColor->colorName() != "")
		properties += "color: " + ccomboColor->colorName();

	// Background
	QString b_color = ccomboBGColor->colorName();
	QString b_img = fcomboBGImage->text();
	QString b_repeat = comboBGRepeat->currentText();
	QString b_att = comboBGAttachment->currentText();
	QString b_pos_x = comboBGPositionLeft->currentText();
	QString b_pos_y = comboBGPositionTop->currentText();

	// Create the background position, but leave it empty if it is the default
	QString b_pos = "";
	if ( b_pos_x != "" && b_pos_x != "left" && b_pos_x != "0" && b_pos_x != "0 %" ) {
		b_pos += b_pos_y + " " + b_pos_x;
	}

 	// The same of the font: insert the rules only if these are different from
  	// the default values
	if ( checkBGInline->isChecked() ) {
     	// Generate the inline rule
		QString temp = "background: ";
		if (b_color != "transparent" && b_color != "")
			temp += " " + b_color;
		if (b_img != "none" && b_img != "")
			temp += " " + b_img;
		if (b_repeat != "repeat" && b_repeat != "")
			temp += " " + b_repeat;
		if (b_att != "scroll" && b_att != "")
			temp += " " + b_att;
		if (b_pos != "")
			temp += " " + b_pos;
		if (temp != "background: ")
			properties += temp + ";\n";
	} else {
     	// One line for background rule
		if (b_color != "transparent" && b_color != "")
			properties += "background-color: " + b_color;
		if (b_img != "none" && b_img != "")
			properties += "background-image: " + b_img;
		if (b_repeat != "repeat" && b_repeat != "")
			properties += "background-repeat: " + b_repeat;
		if (b_att != "scroll" && b_att != "")
			properties += "background-attachment: " + b_att;
		if (b_pos != "")
			properties += "background-position: " + b_pos;
	}

	// Let's add some others text properties
	QString t;
	t = comboWordSpacing->currentText();
	if (t != "normal" && t != "")
		properties +=  "word-spacing: " + t;
	t = comboLetterSpacing->currentText();
	if (t != "normal" && t != "")
		properties += "letter-spacing: " + t;
	t = comboTextDecoration->currentText();
	if (t != "none" && t != "")
		properties += " text-decoration: " + t;
	t = comboTextTransform->currentText();
	if (t != "none" && t != "")
		properties += " text-transform: " + t;
	t = comboTextAlign->currentText();
	if (t != "left" && t != "")
		properties += "text-align: " + t;
	t = comboVerticalAlign->currentText();
	if (t != "baseline" && t != "")
		properties += "vertical-align: " + t;
	t = comboTextIndent->currentText();
	if (t != "0" && t != "")
		properties += "text-indent: " + t;
	t = comboLineHeight->currentText();
	if (t != "normal" && t != "")
		properties += "line-height: " + t;

	// Now, let's add the margin
	QString mt = comboTopMargin->currentText();
	QString mr = comboRightMargin->currentText();
	QString mb = comboBottomMargin->currentText();
	QString ml = comboLeftMargin->currentText();
	if ( checkMarginInline->isChecked() ) {
		QString temp = "margin: " + mt + " " + mr + " " + mb + " " + ml;
		temp = " " + temp.simplifyWhiteSpace();

		// We can specify only some values if there are repetitions, so let's
		// check for it
		if (mt == mb && mb == mr && mr == ml)
			properties += "margin: " + mt + ";\n";
		else if (mt == mb && ml == mr)
			properties += "margin: " + mt + " " + mr + ";\n";
		else if (ml == mr)
			properties += "margin: " + mt + " " + mr + " " + mb + ";\n";
		else if (temp != "margin: " && temp != "margin: 0 0 0 0")
			properties += temp + ";\n";
	} else {
		if (mt != "0" && mt != "")
			properties += "margin-top: " + mt;
		if (mr != "0" && mr != "")
			properties += "margin-right: " + mr;
		if (mb != "0" && mb != "")
			properties += "margin-bottom: " + mb;
		if (ml != "0" && ml != "")
		properties += "margin-left: " + ml;
	}

	// The padding
	QString pt = comboTopPadding->currentText();
	QString pr = comboRightPadding->currentText();
	QString pb = comboBottomPadding->currentText();
	QString pl = comboLeftPadding->currentText();
	if ( checkPaddingInline->isChecked() ) {
		QString temp = "padding: " + pt + " " + pr + " " + pb + " " + pl;
		temp = temp.simplifyWhiteSpace();
		if (pt == pb && pb == pr && pr == pl)
			properties += "padding: " + pt + ";\n";
		else if (pt == pb && pl == pr)
			properties += "padding: " + pt + " " + pr + ";\n";
		else if (pl == pr)
			properties += "padding: " + pt + " " + pr + " " + pb + ";\n";
		else if (temp != "padding: " && temp != "padding: 0 0 0 0")
			properties += temp + ";\n";
	} else {
		if (pt != "0" && pt != "")
			properties += "padding-top: " + pt;
		if (pr != "0" && pr != "")
			properties += "padding-right: " + pr;
		if (pb != "0" && pb != "")
			properties += "padding-bottom: " + pb;
		if (pl != "0" && pl != "")
			properties += "padding-left: " + pl;
	}

	//Now is the time to insert the border
	QString b_w_t = comboBorderTopWidth->currentText();
	QString b_w_r = comboBorderRightWidth->currentText();
	QString b_w_b = comboBorderBottomWidth->currentText();
	QString b_w_l = comboBorderLeftWidth->currentText();

	QString b_s_t = comboBorderTopStyle->currentText();
	QString b_s_r = comboBorderRightStyle->currentText();
	QString b_s_b = comboBorderBottomStyle->currentText();
	QString b_s_l = comboBorderLeftStyle->currentText();

	QString b_c_t = ccomboBorderTopColor->colorName();
	QString b_c_r = ccomboBorderRightColor->colorName();
	QString b_c_b = ccomboBorderBottomColor->colorName();
	QString b_c_l = ccomboBorderLeftColor->colorName();

	QString b_w = QString(b_w_t + " " + b_w_r + " " + b_w_b + " "
		+ b_w_l).simplifyWhiteSpace();
	QString b_s = QString(b_s_t + " " + b_s_r + " " + b_s_b + " "
		+ b_s_b).simplifyWhiteSpace();
	QString b_c = QString(b_c_t + " " + b_c_r + " " + b_c_b + " "
		+ b_c_l).simplifyWhiteSpace();

	QString b_t = QString(b_w_t + " " + b_s_t + " " + b_c_t).simplifyWhiteSpace();
	QString b_r = QString(b_w_r + " " + b_s_r + " " + b_c_r).simplifyWhiteSpace();
	QString b_b = QString(b_w_b + " " + b_s_b + " " + b_c_b).simplifyWhiteSpace();
	QString b_l = QString(b_w_l + " " + b_s_l + " " + b_c_l).simplifyWhiteSpace();

	if ( radioBorderWCS->isChecked() ) {
		// 3 rules: border-width, border-style, border-color
		if ( b_w != "medium medium medium medium" && b_w != "" )
			properties += "border-width: " + b_w;
		if ( b_s != "none none none none" && b_s != "" )
			properties += "border-style: " + b_s;
		if ( b_c != "" )
			properties += "border-color: " + b_c;
	} else if ( radioBorderSides->isChecked() ) {
		// 4 rules: top, right, bottom, left -border
		if ( b_t != "medium none" && b_t != "" )
			properties += "top-border: " + b_t;
		if ( b_r != "medium none" && b_r != "" )
			properties += "right-border: " + b_r;
		if ( b_b != "medium none" && b_b != "" )
			properties += "bottom-border: " + b_b;
		if ( b_l != "medium none" && b_l != "" )
			properties += "left-border: " + b_l;
	} else if ( radioBorderOnly->isChecked() ) {
		// 1 rule: border
		if ( b_t != "medium none" && b_t != "" )
			properties += "border: " + b_t;
	}

	// Some others properties
	if (comboWidth->currentText() != "auto" && comboWidth->currentText() != "")
		properties += "width: " + comboWidth->currentText();
	if (comboHeight->currentText() != "auto" && comboHeight->currentText() != "")
		properties += "height: " + comboHeight->currentText();
	if (comboFloat->currentText() != "none" && comboFloat->currentText() != "")
		properties += "float: " + comboFloat->currentText();
	if (comboClear->currentText() != "none" && comboClear->currentText() != "")
		properties += "clear: " + comboClear->currentText();
	if (comboDisplay->currentText() != "block" && comboDisplay->currentText() != "")
		properties += "display: " + comboDisplay->currentText();
	if (comboWhiteSpace->currentText() != "normal" &&
		comboWhiteSpace->currentText() != "")
		properties += "white-space: " + comboWhiteSpace->currentText();

	// List style
	if ( checkListStyleInline->isChecked() ) {
		// All in one rule
		QString temp, t;
		t = comboListStyleType->currentText();
		if (t != "disc" && t != "")
			temp += " " + t;
		t = comboListStylePosition->currentText();
		if (t != "outside" && t != "")
			temp += " " + t;
		t = fcomboListStyleImage->text();
		if (t != "")
			temp += " " + t;
		temp = temp.simplifyWhiteSpace();
		if (temp != "")
			properties += "list-style: " + temp + ";\n";
	} else {
		// 3 rules
		QString t;
		t = comboListStyleType->currentText();
		if (t != "disc" && t != "")
			properties += "list-style-type: " + t;
		t = comboListStylePosition->currentText();
		if (t != "outside" && t != "")
			properties += "list-style-position: " + t;
		t = fcomboListStyleImage->text();
		if (t != "")
			properties += "list-style-image: " + t;
	}

	// Let's create the final CSS code
	if (properties.size() == 0)
		return "";
	QString text="";
	if (!code_inline) {
		bool ind = checkCodeInline->isChecked();

		text = lineSelector->text() + "{";
		if (ind)
			text += "\n";
		for (QStringList::Iterator it = properties.begin();
			it != properties.end(); it++) {
			if (ind)
				text += "\t";
			text += *it;
			if (ind)
      {
				text += "\n";
      } else
      {
        text += "; ";
      }
		}
		text += "}";
		if (ind)
			text += "\n";
	} else {
		for (QStringList::Iterator it = properties.begin();
		it != properties.end(); ++it) {
			text += *it + "; ";
		}
	}
	return text;
}

/** From the code, insert the values on the widgets */;
void CSSSelectorEditor::widgetFromCode(QString code)
{
	QStringList values;
	if (code_inline) {
		// Parse the selector name
		QString sel = code;
		sel.truncate(sel.find("{"));
		lineSelector->setText(sel.simplifyWhiteSpace());

		// Split the code into a list of pairs property: value
		QString values_code = code.mid(sel.length() + 1, code.find("}") - 1).simplifyWhiteSpace();
		values = QStringList::split( ";", values_code);
	} else {
		lineSelector->setEnabled (false);
		values = QStringList::split( ";", code);
	}

	// Now iter on each pair, and insert it into the correct widget
	QStringList::Iterator it = values.begin();
	for (; it != values.end(); it++) {
		QString p = *it;
		// In pname goes the property name, in pvalue goes the property value
		QString pname = p.mid(p.find("{") + 1, p.find(":")).simplifyWhiteSpace();
		QString pvalue = p.mid(p.find(":") + 1).simplifyWhiteSpace();

		// Remove chars that could be in the string (errors)
		pname = pname.replace(QRegExp(":"), "");
		pname = pname.replace(QRegExp("\\{"), "");
		pname = pname.replace(QRegExp("\\}"), "");
		pvalue = pvalue.replace(QRegExp(":"), "");
		pvalue = pvalue.replace(QRegExp("\\{"), "");
		pvalue = pvalue.replace(QRegExp("\\}"), "");

		// Now the bigger if even done in my life: let's insert all the
		// properties into their widgets

		// Font properties
		if (pname == "font-family")
			comboFontFamily->setCurrentText(pvalue);
		else if (pname == "font-style")
			comboFontStyle->setCurrentText(pvalue);
		else if (pname == "font-variant")
			comboFontVariant->setCurrentText(pvalue);
		else if (pname == "font-weight")
			comboFontWeight->setCurrentText(pvalue);
		else if (pname == "font-size")
			comboFontSize->setCurrentText(pvalue);
		else if (pname == "font") {
			// Now things are more difficult: we must sub - parse the string
			// and place all the currect values into the combo boxes
			QStringList fp = QStringList::split( " ", pvalue);

			// __TODO__
		}

		// Color & Background properties
		else if (pname == "color")
			ccomboColor->setColorName(pvalue);
		else if (pname == "background-color")
			ccomboBGColor->setColorName(pvalue);
		else if (pname == "background-image")
			fcomboBGImage->setText(pvalue);
		else if (pname == "background-repeat")
			comboBGRepeat->setCurrentText(pvalue);
		else if (pname == "background-attachment")
			comboBGAttachment->setCurrentText(pvalue);
		else if (pname == "background-position") {
			QStringList bp = QStringList::split(pvalue, " ");
			comboBGPositionTop->setCurrentText(bp[0]);
			if (bp.size() == 2)
				comboBGPositionLeft->setCurrentText(bp[1]);
		} else if (pname == "background") {
			// __TODO__
		}

		// Text properties
		else if (pname == "word-spacing")
			comboWordSpacing->setCurrentText(pvalue);
		else if (pname == "letter-spacing")
			comboLetterSpacing->setCurrentText(pvalue);
		else if (pname == "text-decoration")
			comboTextDecoration->setCurrentText(pvalue);
		else if (pname == "text-align")
			comboTextAlign->setCurrentText(pvalue);
		else if (pname == "vertical-align")
			comboVerticalAlign->setCurrentText(pvalue);
		else if (pname == "text-transform")
			comboTextTransform->setCurrentText(pvalue);
		else if (pname == "text-indent")
			comboTextIndent->setCurrentText(pvalue);
		else if (pname == "line-height")
			comboLineHeight->setCurrentText(pvalue);

		// Border properties
		else if (pname == "border-top-width")
			comboBorderTopWidth->setCurrentText(pvalue);
		else if (pname == "border-right-width")
			comboBorderRightWidth->setCurrentText(pvalue);
		else if (pname == "border-bottom-width")
			comboBorderBottomWidth->setCurrentText(pvalue);
		else if (pname == "border-left-width")
			comboBorderLeftWidth->setCurrentText(pvalue);
		else if (pname == "border-top") {
			// __TODO__
		} else if (pname == "border-right") {
			// __TODO__
		} else if (pname == "border-bottom") {
			// __TODO__
		} else if (pname == "border-left") {
			// __TODO__
		} else if (pname == "border-width") {
			// Get the top, right, bottom and left values
			QString t, r, b, l;
			splitValueTRBL(pvalue, t, r, b, l);

			// And put them into the currect combo boxes
			comboBorderTopWidth->setCurrentText( t );
			comboBorderRightWidth->setCurrentText( r );
			comboBorderBottomWidth->setCurrentText( b );
			comboBorderLeftWidth->setCurrentText( l );
		} else if (pname == "border-color") {
			// Get the top, right, bottom and left values
			QString t, r, b, l;
			splitValueTRBL(pvalue, t, r, b, l);

			// And put them into the currect combo boxes
			ccomboBorderTopColor->setColorName( t );
			ccomboBorderRightColor->setColorName( r );
			ccomboBorderBottomColor->setColorName( b );
			ccomboBorderLeftColor->setColorName( l );
		} else if (pname == "border-style") {
			// Get the top, right, bottom and left values
			QString t, r, b, l;
			splitValueTRBL(pvalue, t, r, b, l);

			// And put them into the currect combo boxes
			comboBorderTopStyle->setCurrentText( t );
			comboBorderRightStyle->setCurrentText( r );
			comboBorderBottomStyle->setCurrentText( b );
			comboBorderLeftStyle->setCurrentText( l );
		} else if (pname == "border") {
			// __TODO__
		}

		// Margin & padding properties
		else if (pname == "margin-top")
			comboTopMargin->setCurrentText(pvalue);
		else if (pname == "margin-right")
			comboRightMargin->setCurrentText(pvalue);
		else if (pname == "margin-bottom")
			comboBottomMargin->setCurrentText(pvalue);
		else if (pname == "margin-left")
			comboLeftMargin->setCurrentText(pvalue);
		else if (pname == "margin") {
			// Get the top, right, bottom and left values
			QString t, r, b, l;
			splitValueTRBL(pvalue, t, r, b, l);

			// And put them into the currect combo boxes
			comboTopMargin->setCurrentText( t );
			comboRightMargin->setCurrentText( r );
			comboBottomMargin->setCurrentText( b );
			comboLeftMargin->setCurrentText( l );
		} else if (pname == "padding-top")
			comboTopPadding->setCurrentText(pvalue);
		else if (pname == "padding-right")
			comboRightPadding->setCurrentText(pvalue);
		else if (pname == "padding-bottom")
			comboBottomPadding->setCurrentText(pvalue);
		else if (pname == "padding-left")
			comboLeftPadding->setCurrentText(pvalue);
		else if (pname == "padding") {
			// Get the top, right, bottom and left values
			QString t, r, b, l;
			splitValueTRBL(pvalue, t, r, b, l);

			// And put them into the currect combo boxes
			comboTopPadding->setCurrentText( t );
			comboRightPadding->setCurrentText( r );
			comboBottomPadding->setCurrentText( b );
			comboLeftPadding->setCurrentText( l );
		}

		// Box properties
		else if (pname == "width")
			comboWidth->setCurrentText(pvalue);
		else if (pname == "height")
			comboHeight->setCurrentText(pvalue);
		else if (pname == "float")
			comboFloat->setCurrentText(pvalue);
		else if (pname == "clear")
			comboClear->setCurrentText(pvalue);

		// Classification properties
		else if (pname == "display")
			comboDisplay->setCurrentText(pvalue);
		else if (pname == "white-space")
			comboWhiteSpace->setCurrentText(pvalue);

		// List style
		else if (pname == "list-style-type")
			comboListStyleType->setCurrentText(pvalue);
		else if (pname == "list-style-image")
			fcomboListStyleImage->setText(pvalue);
		else if (pname == "list-style-position")
			comboListStylePosition->setCurrentText(pvalue);
		else if (pname == "list-style") {
			// __TODO__
		}

		else {
			// Finally, if we get here, there's an error somewhere in the CSS code
			kdWarning() << "Error while parsing CSS code. Unknowen property name.\n"
				<< "Property: " << pname << "\nValue: " << pvalue << "\n";
		}
	}
}

/** Split a string in the form top [right]? [bottom]? [left]? into the
	four variables */
void CSSSelectorEditor::splitValueTRBL(QString value, QString& t,
	QString& r, QString& b, QString& l)
{
	QStringList ls = QStringList::split(value, " ");
	switch (ls.size()) {
		case 1:
			t = r = b = l = ls[0];
			break;
		case 2:
			t = b = ls[0];
			r = l = ls[1];
			break;
		case 3:
			t = ls[0];
			r = l = ls[1];
			b = ls[2];
			break;
		case 4:
			t = ls[0];
			r = ls[1];
			b = ls[2];
			l = ls[3];
			break;
	}
}

#include "cssselectoreditor.moc"
