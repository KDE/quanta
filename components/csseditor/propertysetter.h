/***************************************************************************
                          propertysetter.h  -  description
                             -------------------
    begin                : gio lug 24 2003
    copyright            : (C) 2003 by gulmini luciano
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

#ifndef PROPERTYSETTER_H
#define PROPERTYSETTER_H

#include <qhbox.h>
#include <kurlrequester.h>
#include <qptrlist.h>
#include "doubleeditors.h"
#include <qcombobox.h>
class QLabel;
class QLineEdit;
class mySpinBox;
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


class TLPEditor : public QHBox { //editor with a line text and a button calling a dialog
  Q_OBJECT

  protected:
    QLineEdit *m_le;
    QLabel *m_label;
    KPushButton *m_pb;

  public:
    TLPEditor(QWidget *parent, const char* name=0);
    virtual ~TLPEditor();
    KPushButton *button() const { return m_pb;}
    void setButtonIcon(QString);
    void setToolTip(QString);
    void setLabelText(QString);

  signals:
    void valueChanged(const QString&);
};

class fontEditor : public TLPEditor{
  Q_OBJECT

  public:
    fontEditor(QWidget *parent, const char* name=0);

  public slots:
    void openFontChooser();
};

class URIEditor : public TLPEditor {
  Q_OBJECT
  public:
    enum mode{ multi, single };
    enum URIResourceType{ audio, image, mousePointer };

  private:
    QStringList m_sFiles;
    mode m_Mode;
    URIResourceType m_resourceType;

  public:
    URIEditor(QWidget *parent, const char* name=0);
    void setMode(const mode& m) { m_Mode = m ; }
    void setResourceType(const  URIResourceType& r) { m_resourceType = r ; }

  public slots:
    void URI(const QString&);
    void openFileDialog();
};

class percentageEditor : public QHBox  {
     Q_OBJECT
   private:
     mySpinBox *m_sb;
   public:
           percentageEditor(const QString& initialValue="0",QWidget *parent=0, const char *name=0);
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
    mySpinBox  *m_sb;
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
    QComboBox *m_pcb;
    KPushButton *m_pb;
    fontEditor *m_ftE;

  public:
    propertySetter(QWidget *parent=0, const char *name=0);
    ~propertySetter();
    void setFontEditor();
    void setComboBox();
    void setSpinBox(const QString& initialValue="0", const QString& min="0", const QString& max="9999", const QString& s=QString::null);
    void setLineEdit();
    void setLengthEditor(QString s);
    void setDoubleLengthEditor(QString s);
    void setDoublePercentageEditor(QString s);
    void setDoubleComboBoxEditor();
    void setPercentageEditor(QString s);
    void setUriEditor();
    void setFrequencyEditor(QString s);
    void setTimeEditor(QString s);
    void setAngleEditor(QString s);
    void setColorRequester();
    void setPredefinedColorListEditor();
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
