/***************************************************************************
                          colorslider.cpp  -  description
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

#include <qvbox.h>
#include <qlabel.h>

#include <klocale.h>

#include "colorslider.h"
#include "propertysetter.h"

/**
  *@author gulmini luciano
  */

  

colorSlider::colorSlider(const QString& fn,const QString& l,const QString& c,const QString& r,QWidget *parent, const char *name) : miniEditor(parent,name)  { 
  m_functionName = fn;
  QVBox *leftBox = new QVBox(this);
  QVBox *centerBox = new QVBox(this);
  QVBox *rightBox = new QVBox(this);
  QLabel *leftLabel = new QLabel("<b>" +  l +"</b>",leftBox);
  QLabel *centerLabel = new QLabel(("<b>" +  c +"</b>"),centerBox);
  QLabel *rightLabel = new QLabel(("<b>" +  r +"</b>"),rightBox);
  leftLabel->setAlignment(Qt::AlignHCenter);
  centerLabel->setAlignment(Qt::AlignHCenter);
  rightLabel->setAlignment(Qt::AlignHCenter);  
  leftLabel->setTextFormat (Qt::RichText ) ;
  centerLabel->setTextFormat ( Qt::RichText ) ;
  rightLabel->setTextFormat (Qt::RichText  ) ;
  m_leftValue = new QSlider ( 0, 255, 1, 0, Qt::Horizontal , leftBox);
  m_centerValue = new QSlider ( 0, 255, 1, 0, Qt::Horizontal , centerBox);
  m_rightValue = new QSlider ( 0, 255, 1, 0, Qt::Horizontal , rightBox);
  setSpacing(10);
  connect(m_leftValue, SIGNAL(valueChanged ( int)), this, SLOT(convertLeftValue(int)));
  connect(m_centerValue, SIGNAL(valueChanged ( int)), this, SLOT(convertCenterValue(int)));
  connect(m_rightValue, SIGNAL(valueChanged ( int)), this, SLOT(convertRightValue(int)));
}

colorSlider::~colorSlider(){
  //delete m_redValue;
  //delete m_greenValue;
  //delete m_blueValue; 
}

void colorSlider::connectToPropertySetter(propertySetter* p){
  connect( this, SIGNAL(valueChanged(const QString&)), p, SIGNAL(valueChanged(const QString&)));
}

void colorSlider::convertLeftValue(int i){
  emit valueChanged(m_functionName + "(" + QString::number(i,10) + "," + QString::number(m_centerValue->value(),10) + "," +QString::number(m_rightValue->value(),10) +")");
}

void colorSlider::convertCenterValue(int i){
  emit valueChanged(m_functionName + "(" +  QString::number(m_leftValue->value(),10) + "," + QString::number(i,10) + "," + QString::number(m_rightValue->value(),10) +")");
}

void colorSlider::convertRightValue(int i){
  emit valueChanged(m_functionName + "(" + QString::number(m_leftValue->value(),10) + "," + QString::number(m_centerValue->value(),10) + "," + QString::number(i,10) +")");
}

RGBcolorSlider::RGBcolorSlider(QWidget *parent, const char *name) : colorSlider("rgb",i18n("Red"),i18n("Green"),i18n("Blue"),parent,name){
}

//FOR CSS3
/*HSLcolorSlider::HSLcolorSlider(QWidget *parent, const char *name) : colorSlider("hsl",i18n("Hue"),i18n("Saturation"),i18n("Lightness"),parent,name){
}
*/

#include "colorslider.moc"
