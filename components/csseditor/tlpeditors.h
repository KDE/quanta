/***************************************************************************
                          tlpeditors.h  -  description
                             -------------------
    begin                : gio apr 1 2004
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

#ifndef TLPEDITORS_H
#define TLPEDITORS_H

#include <qhbox.h>
class KPushButton;
class QLineEdit;
class QLabel;
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
    virtual void setButtonIcon(QString);
    void setToolTip(QString);
    void setLabelText(QString);
    QLineEdit* lineEdit() const { return m_le; }
    KPushButton* button() const { return m_pb; }

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
    enum mode{ Multi, Single };
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


#endif
