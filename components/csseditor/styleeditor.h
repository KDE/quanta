/***************************************************************************
                           styleeditor  -  description
    begin              : Wed Apr 7 2004
    copyright          : (C) 2004 by Luciano Gulmini <gulmini.luciano@student.unife.it>
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
#include "tlpeditors.h"
//kde includes

//app includes

//forward declarations
class propertySetter;
class StyleEditor : public TLPEditor{
  Q_OBJECT
  private:
    int m_iconWidth,
        m_iconHeight;
public:
    StyleEditor(QWidget *parent=0, const char* name=0);
    virtual void setButtonIcon(int width, int height);

public slots:
    void openCSSEditor();
    virtual void connectToPropertySetter(propertySetter* /*p*/){}
    virtual void setInitialValue(const QString& /*s*/){}
};


#endif //STYLEEDITOR_H
