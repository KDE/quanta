/***************************************************************************
                          tagwidget.cpp  -  description
                             -------------------
    begin                : Sat Apr 1 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavsky
    email                : pdima@mail.univ.kiev.ua
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

// QT files
#include <qcombobox.h>
#include <klineedit.h>
#include <qstring.h>
#include <qspinbox.h>
#include <qcheckbox.h>

// KDE files
#include <kcolorbtn.h>

// application's headers
#include "tagwidget.h"
#include "tagdialog.h"

TagWidget::TagWidget(QWidget *parent, const char *name )
  : QWidget(parent,name)
{
	this->basePath = ((TagDialog *)parent)->basePath();
}

TagWidget::~TagWidget(){
}

void TagWidget::updateDict( QString attr, QComboBox *combo )
{
  QString *s = new QString(combo->currentText());
  if ( s->isEmpty() ) dict->remove(attr);
  else dict->replace(attr, s );
}

void TagWidget::setValue( QString val, QComboBox *combo)
{
  bool found = false;
  QString value = val;
  int num = combo->count();

  for ( int i=0;i<num;i++) {
    if ( value == combo->text( i) ) {
      combo->setCurrentItem( i);
        found = true;
    }
  }

  if ( !found ) combo->setEditText( val);
}

void TagWidget::setValue( QString val, KLineEdit *line)
{
  line->setText( val);
}

void TagWidget::setValue( QString val, QSpinBox *spin)
{
  QString value = val;
  int valint = value.toInt();

  spin->setValue( valint);
}

void TagWidget::setValue( QString val, KColorButton *button)
{
  QString value = val;

  button->setColor( value.data() );
}

void TagWidget::updateDict( QString attr, KLineEdit *line )
{
  QString *s = new QString(line->text());
  if ( s->isEmpty() ) dict->remove(attr);
  else dict->replace(attr, s );
}

void TagWidget::updateDict( QString attr, QSpinBox *spin )
{
  QString *s = new QString(spin->text());
  if ( s->isEmpty() ) dict->remove(attr);
  else dict->replace(attr, s );
}

void TagWidget::updateDict( QString attr, QCheckBox *check )
{
  if ( !check->isChecked() )
    dict->remove( attr );
  else {
    if ( !dict->find(attr) )
      dict->insert(attr, new QString("") );
  }
}
