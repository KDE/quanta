/***************************************************************************
                          tagwidget.h  -  description
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

#ifndef TAGWIDGET_H
#define TAGWIDGET_H

#include <qwidget.h>
#include <qdict.h>

class QComboBox;
class QLineEdit;
class QSpinBox;
class QCheckBox;
class KColorButton;

/**base class for tag widgets
  *@author Yacovlev Alexander & Dmitry Poplavsky
  */

class TagWidget : public QWidget  {
   Q_OBJECT
public: 
	TagWidget(QWidget *parent=0, const char *name=0);
	~TagWidget();
	
  virtual void readAttributes( QDict<char> * ) {};
  virtual void writeAttributes( QDict<char> * ) {};
  /**  */
  void updateDict( const char *attr, QComboBox *combo );
  void updateDict( const char *attr, QLineEdit *line );
  void updateDict( const char *attr, QSpinBox *spin );
  void updateDict( const char *attr, QCheckBox *check );

  void setValue( const char *val, QComboBox *combo);
  void setValue( const char *val, QLineEdit *line);
  void setValue( const char *val, QSpinBox  *spin);
  void setValue( const char *val, KColorButton *button);

protected:
  QDict<char> *dict; // dictionary of attributes and values
  QString basePath;
};

#endif
