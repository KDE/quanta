/***************************************************************************
                          tagwidget.h  -  description
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

#ifndef TAGWIDGET_H
#define TAGWIDGET_H

#include <qwidget.h>
#include <qdict.h>

class QComboBox;
class QLineEdit;
class QSpinBox;
class QCheckBox;
class KColorButton;
class KURL;

/**base class for tag widgets
  *@author Yacovlev Alexander & Dmitry Poplavsky
  */

class TagWidget {
public:
  TagWidget(QObject *parent=0, const char *name=0);
  virtual ~TagWidget();

  virtual void readAttributes(QDict<QString> *) {};
  virtual void writeAttributes(QDict<QString> *) {};
  void updateDict(const QString &attr, QComboBox *combo);
  void updateDict(const QString &attr, QLineEdit *line);
  void updateDict(const QString &attr, QSpinBox *spin);
  void updateDict(const QString &attr, QCheckBox *check);

  void setValue(const QString &val, QComboBox *combo);
  void setValue(const QString &val, QLineEdit *line);
  void setValue(const QString &val, QSpinBox *spin);
  void setValue(const QString &val, KColorButton *button);

protected:
  QDict<QString> *dict; // dictionary of attributes and values
  KURL baseURL;
};

#endif
