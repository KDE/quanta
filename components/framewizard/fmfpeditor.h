/***************************************************************************
                          fmfpeditor.h  -  description
                             -------------------
    begin                : mer giu 4 2003
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

#ifndef FMFPEDITOR_H
#define FMFPEDITOR_H

#include <fmfpeditors.h>

/**
  *@author Gulmini Luciano
  */

class fmFPeditor : public fmFPeditorS  {
  Q_OBJECT
  public: 
	  fmFPeditor();
	  ~fmFPeditor();
  public slots:  
    QString getIdValue();
    QString getSrcValue();
    QString getClassValue();
    QString getLongdescValue();
    QString getTitleValue();
    QString getStyleValue();
    QString getNameValue();
    QString getNoresizeValue();
    QString getScrollingValue(); 
    QString getFrameborderValue(); 
    QString getMarginwidthValue();
    QString getMarginheightValue(); 
    void setup(QMap<QString,QString>);
    QMap<QString,QString> getAttributeMap();

};

#endif
