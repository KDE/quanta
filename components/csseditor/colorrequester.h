/***************************************************************************
                          colorrequester.h  -  description
                             -------------------
    copyright            : (C) 2004 by gulmini luciano
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
#ifndef COLORREQUESTER_H
#define COLORREQUESTER_H

#include <kpushbutton.h>
#include <qcolor.h>

#include "minieditor.h"

class KLineEdit;
class KColorDialog;
class colorRequester : public miniEditor{
    Q_OBJECT
  public:
    colorRequester(QWidget *parent, const char* name=0);
    ~colorRequester();
    KLineEdit * lineEdit() const;
    KPushButton * button() const { return myButton; }
    virtual void connectToPropertySetter(propertySetter* p);
    //void setInitialValue(const QString&);
  public slots:
    void openColorDialog();
    void setInitialValue(/*const QString&*/);
  protected:
    void init();
  signals:
    void selectedColor(QColor);
    void textChanged(const QString&);
  private:
    mutable KColorDialog * myColorDialog;
    KPushButton *myButton;
    class colorRequesterPrivate;
          colorRequesterPrivate *d;
    QColor m_initialValue;
};
#endif
