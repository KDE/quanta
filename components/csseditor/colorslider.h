/***************************************************************************
                          colorslider.h  -  description
                             -------------------
    begin                : lun  ago 9 2004
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

#ifndef COLORSLIDER_H
#define COLORSLIDER_H

#include <qslider.h>

#include "minieditor.h"



/**
  *@author gulmini luciano
  */

class colorSlider : public miniEditor  {
     Q_OBJECT
   private :
    QString m_functionName;  
   protected:
     QSlider *m_leftValue,
                  *m_centerValue,
                  *m_rightValue;
   public:
           colorSlider(const QString& functionName,const QString& l=QString::null,const QString& c=QString::null,const QString& r=QString::null,QWidget *parent=0, const char *name=0);
           virtual ~colorSlider();
           virtual void connectToPropertySetter(propertySetter* p);
  
   private slots: 
     void convertLeftValue(int i);  
     void convertCenterValue(int i);
     void convertRightValue(int i);  
              
   signals:
     void valueChanged(const QString&);
};  

class RGBcolorSlider : public colorSlider  {
     Q_OBJECT
   public:
           RGBcolorSlider(QWidget *parent=0, const char *name=0);
           ~RGBcolorSlider(){}         
};

//FOR CSS3
/*class HSLcolorSlider : public colorSlider  {
     Q_OBJECT
   public:
           HSLcolorSlider(QWidget *parent=0, const char *name=0);
           ~HSLcolorSlider();             
};*/
  

#endif
