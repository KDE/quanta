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

 
//enum mode{ multi, single };
//enum URIResourceType{ audio, image, mousePointer };
    
class URIEditor : public QHBox {
  Q_OBJECT
  public:
    enum mode{ multi, single };
    enum URIResourceType{ audio, image, mousePointer };
      
  private:
    QStringList m_sFiles;
    QLineEdit *m_le;
    QLabel *m_label;
    KPushButton *m_pb;
    mode m_Mode;
    URIResourceType m_resourceType;
     
  public:
    URIEditor(QWidget *parent, const char* name=0);
    ~URIEditor();
    KPushButton * button() const { return m_pb;}
    void setMode(const mode& m) { m_Mode = m ; }
    void setResourceType(const  URIResourceType& r) { m_resourceType = r ; }
     
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
    unsigned int m_ind;
    QPtrList<QWidget> m_list;
    QComboBox *m_cb;
    QLineEdit *m_le;
    QSpinBox  *m_sb;
    lengthEditor *m_lE;
    doubleLengthEditor *m_dlE;
    doublePercentageEditor *m_dpe;
    doubleComboBoxEditor *m_dcbe;
    percentageEditor *m_pe;
    URIEditor *m_ue;
    frequencyEditor *m_fe;
    timeEditor *m_te;
    angleEditor *m_ae;
    colorRequester *m_cr;
    KPushButton *m_pb;  
        
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
    void setUriEditor();
    void setFrequencyEditor();
    void setTimeEditor();
    void setAngleEditor();
    void setColorRequester();
    void reset();
    void addButton();
    QComboBox* ComboBox() const { return m_cb; }
    doubleComboBoxEditor* DoubleComboBoxEditor() const { return m_dcbe; }
    URIEditor* UriEditor() const { return m_ue; }
     
  public slots:
    void Show();  
    
  signals:
    void valueChanged(const QString&);
};

#endif
