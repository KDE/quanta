/***************************************************************************
                          propertysetter.h  -  description
                             -------------------
    begin                : gio lug 24 2003
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

#ifndef PROPERTYSETTER_H
#define PROPERTYSETTER_H

#include <qwidget.h>
#include <qhbox.h>
#include <kurlrequester.h>
#include <qptrlist.h>
#include "doubleeditors.h"



class QComboBox;
class QLabel;
class QLineEdit;
class QSpinBox;
class QStringList;
class KPushButton;
class angleEditor;
class colorRequester;
class frequencyEditor;
class lengthEditor;
class specialSB;
class timeEditor;



/**
  *@author gulmini luciano
  */

class multipleSpinBox : public QHBox{
   Q_OBJECT

   private:
     QPtrList<QSpinBox> SBList;
     
   public:
      multipleSpinBox(QWidget *parent, const char* name=0, int n=2);
     ~multipleSpinBox();
      QSpinBox* getSpinBox(unsigned int i) { return SBList.at(i); }
      void setSuffix(const QString&);
   public slots:
     void valueChangedSlot(const QString&);   
   signals:
     void valueChanged(const QString&);    
};

 
enum mode{ Multi, Single };
    
class URIEditor : public QHBox {
  Q_OBJECT

   private:
     QStringList sFiles;
     QLineEdit *le;
     QLabel *label;
     KPushButton *pb;

     mode Mode;
     
   public:
     URIEditor(QWidget *parent, const char* name=0);
     ~URIEditor();
     KPushButton * button() const { return pb;}
     void setMode(const mode& m) { Mode = m ; }
     
   public slots:
     void URI(const QString&);
     void openFileDialog();
   signals:
     void valueChanged(const QString&);   
};
   
class percentageEditor : public QHBox  {
     Q_OBJECT
   private:
     QSpinBox *sb;
   public:
           percentageEditor(QWidget *parent=0, const char *name=0);
           ~percentageEditor();
   signals:
     void valueChanged(const QString&);  
};



class propertySetter : public QHBox  {
   Q_OBJECT
   private:
     unsigned int ind;
     QPtrList<QWidget> list;
     QComboBox *cb;
     QLineEdit *le;
     QSpinBox  *sb;
     lengthEditor *lE;
     doubleLengthEditor *dlE;
     doublePercentageEditor *dpe;
     doubleComboBoxEditor *dcbe;
     percentageEditor *pe;
     URIEditor *ue;
     frequencyEditor *fe;
     timeEditor *te;
     angleEditor *ae;
     colorRequester *cr;
     KPushButton *pb;     
   public:
           propertySetter(QWidget *parent=0, const char *name=0);
           ~propertySetter();
     void setComboBox();
     void setSpinBox(const QString& min="0", const QString& max="9999", const QString& s=QString::null);
     void setLineEdit();
     void setLengthEditor();
     void setDoubleLengthEditor();
     void setDoublePercentageEditor();
     void setDoubleComboBoxEditor();
     void setPercentageEditor();
     void setURIEditor();
     void setFrequencyEditor();
     void setTimeEditor();
     void setAngleEditor();
     void setColorRequester();
 
     void reset();

     void addButton();
     QComboBox* getComboBox() const { return cb; }
     doubleComboBoxEditor* getDoubleComboBoxEditor() const { return dcbe; }
     URIEditor* getURIEditor() const { return ue; }
     
  public slots:
    void Show();  
    
  signals:
    void valueChanged(const QString&);
};

#endif
