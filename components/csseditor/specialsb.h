/***************************************************************************
                          specialsb.h  -  description
                             -------------------
    begin                : dom ago 3 2003
    copyright            : (C) 2003 by simone
    email                : simone@localhost
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

#include <qhbox.h>
#include <qcombobox.h>

/**
  *@author simone
  */
class QSpinBox;
  
class specialSB : public QHBox {
    Q_OBJECT
  protected:
    QComboBox *m_cb;
    QSpinBox  *m_sb;
 
  private:
    QString m_cbValue,
                 m_sbValue;

   public:
           specialSB(QWidget *parent=0, const char *name=0);
           ~specialSB();
     void insertItem(QString s){ m_cb->insertItem(s); }
   public slots:
     void cbValueSlot(const QString&);
     void sbValueSlot(const QString&);
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
