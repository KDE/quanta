/***************************************************************************
                          tagwidget.cpp  -  description
                             -------------------
    begin                : Sat Apr 1 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavsky <pdima@mail.univ.kiev.ua>
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
#include <kcolorbutton.h>
#include <kurl.h>

// application's headers
#include "tagwidget.h"
#include "tagdialog.h"
#include "resource.h"

TagWidget::TagWidget(QObject *parent, const char *)
{
  baseURL = ((TagDialog *)parent)->baseURL();
}

TagWidget::~TagWidget(){
}

void TagWidget::updateDict(const QString &attr, QComboBox *combo )
{
  QString *s = new QString(combo->currentText());
  if (s->isEmpty())
  {
    dict->remove(attr);
    delete s;
  }
  else
    dict->replace(attr, s);
}

void TagWidget::setValue(const QString &val, QComboBox *combo)
{
  bool found = false;
  int num = combo->count();

  for ( int i = 0; i < num; i++)
  {
    if (val == combo->text(i))
    {
      combo->setCurrentItem(i);
      found = true;
    }
  }

  if (!found)
    combo->setEditText(val);
}

void TagWidget::setValue(const QString &val, QLineEdit *line)
{
  line->setText(val);
}

void TagWidget::setValue(const QString &val, QSpinBox *spin)
{
  spin->setValue(val.toInt());
}

void TagWidget::setValue(const QString &val, KColorButton *button)
{
  button->setColor(val);
}

void TagWidget::updateDict(const QString &attr, QLineEdit *line )
{
  QString *s = new QString(line->text());
  if (s->isEmpty())
  {
    dict->remove(attr);
    delete s;
  }
  else
    dict->replace(attr, s);
}

void TagWidget::updateDict(const QString &attr, QSpinBox *spin )
{
  QString *s = new QString(spin->text());
  if (s->isEmpty())
  {
    dict->remove(attr);
    delete s;
  } 
  else
    dict->replace(attr, s);
}

void TagWidget::updateDict(const QString &attr, QCheckBox *check )
{
  if (!check->isChecked())
    dict->remove(attr);
  else
  {
    if (!dict->find(attr))
      dict->insert(attr, new QString(""));
  }
}
