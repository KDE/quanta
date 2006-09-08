/***************************************************************************
                          specialsb.h  -  description
                             -------------------
    begin                : dom ago 3 2003
    copyright            : (C) 2003 by Gulmini Luciano
    email                : gulmini.luciano@student.unife.it
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef SPECIALSB_H
#define SPECIALSB_H

#include "minieditor.h"
#include <qcombobox.h>
class mySpinBox;
class KLineEdit;

/**
  *@author gulmini luciano
  */

class specialSB : public miniEditor {
    Q_OBJECT
  protected:
    QComboBox *m_cb;
    mySpinBox *m_sb;
    KLineEdit *m_lineEdit;

   public:
     specialSB(QWidget *parent=0, const char *name=0, bool useLineEdit = false);
     ~specialSB();
     void insertItem(const QString& s){ m_cb->insertItem(s); }
     void setInitialValue(const QString& s);
     QStringList cbValueList();
     virtual void connectToPropertySetter(propertySetter* p);

   public slots:
     void cbValueSlot(const QString&);
     void sbValueSlot(const QString&);
     void lineEditValueSlot(const QString&);

   signals:
     void valueChanged(const QString&);

};

class angleEditor : public specialSB {
     Q_OBJECT
   public:
           angleEditor(QWidget *parent=0, const char *name=0);
};

class frequencyEditor : public specialSB {
     Q_OBJECT
   public:
           frequencyEditor(QWidget *parent=0, const char *name=0);
};

class timeEditor : public specialSB {
     Q_OBJECT
   public:
           timeEditor(QWidget *parent=0, const char *name=0);
};

class lengthEditor : public specialSB {
     Q_OBJECT
   public:
           lengthEditor(QWidget *parent=0, const char *name=0);
};

#endif
