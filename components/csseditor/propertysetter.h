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
#include <qptrlist.h>
#include <qcombobox.h>


class miniEditor;
class KPushButton;


/**
  *@author gulmini luciano
  */
  
class propertySetter : public QHBox  {
  Q_OBJECT

  private:
    unsigned int m_ind;
    QPtrList<QWidget> m_list;
    QComboBox *m_cb;
    KPushButton *m_pb;

  public:
    propertySetter(QWidget *parent=0, const char *name=0);
    ~propertySetter();
    
    void installMiniEditor(miniEditor *m); 
    
    void setComboBox();
    void setSpinBox(const QString& initialValue="0", const QString& min="0", const QString& max="9999", const QString& s=QString::null);
    void setLineEdit();
    void setPredefinedColorListEditor();
    void reset();
    void addButton();
    QComboBox* ComboBox() const { return m_cb; }

  public slots:
    void Show();

  signals:
    void valueChanged(const QString&);
};

#endif
