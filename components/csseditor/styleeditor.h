/***************************************************************************
                           styleeditor  -  description
    begin              : Wed Apr 7 2004
    copyright          : (C) 2004 by Luciano Gulmini <e.gulmini@tiscali.it>
 ***************************************************************************/

/***************************************************************************
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; version 2 of the License.
 *
 ***************************************************************************/

#ifndef STYLEEDITOR_H
#define STYLEEDITOR_H

//qt includes
#include <qhbox.h>

//kde includes

//app includes

//forward declarations
class QLineEdit;
class KPushButton;

class StyleEditor : public QHBox{
  Q_OBJECT

public:
    StyleEditor(QWidget *parent=0, const char* name=0);
    ~StyleEditor();
    QLineEdit* lineEdit() const { return m_le; }
    KPushButton* button() const { return m_pb; }

public slots:
    void openCSSEditor();

private:
    QLineEdit *m_le;
    KPushButton *m_pb;
};


#endif //STYLEEDITOR_H
