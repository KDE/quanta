/***************************************************************************
                          fmfpeditor.cpp  -  description
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

#include "fmfpeditor.h"
#include <qspinbox.h>
#include <qlineedit.h>
#include <qbuttongroup.h>
#include <qregexp.h>
#include <kurlrequester.h>
#include <qstringlist.h>
#include <kfiledialog.h>
#include <klocale.h>

fmFPeditor::fmFPeditor() : fmFPeditorS (){ 
  sbLeft->setWrapping(true);
  sbTop->setWrapping(true);
 
  QString htmlFiles = i18n("*.html *.htm|HTML Files");
  QString phpFiles = i18n("*.php|PHP Files"); 
  QString xmlFiles = i18n("*.xml|XML Files"); 
  QString xhtmlFiles = i18n("*xhtml|XHTML Files");
  QString allFiles = i18n("*.*|All Files"); 
  
  fc->setFilter(htmlFiles+"\n"+phpFiles+"\n"+xmlFiles+"\n"+xhtmlFiles+"\n"+allFiles);

}
fmFPeditor::~fmFPeditor(){
}


QString fmFPeditor::getIdValue()
{
    return leId->text();
}


QString fmFPeditor::getSrcValue()
{
    return fc->url();
}


QString fmFPeditor::getClassValue()
{
    return leClass->text();
}

QString fmFPeditor::getLongdescValue()
{
    return leLongdesc->text();
}

QString fmFPeditor::getTitleValue()
{
    return leTitle->text();
}

QString fmFPeditor::getStyleValue()
{
    return leStyle->text();
}

QString fmFPeditor::getNameValue() {
    return leName->text();
}

QString fmFPeditor::getNoresizeValue() {
    switch(bgNoresize->id(bgNoresize->selected())){
        case 0:return "";break;
	default:return "noresize";
    }

}
QString fmFPeditor::getScrollingValue() {
    switch(bgScrolling->id(bgScrolling->selected())){
        case 0:return "yes";break;
	case 2:return "no";break;
	default:return "auto";
    }
}
QString fmFPeditor::getFrameborderValue() {
    switch(bgBorder->id(bgBorder->selected())){
	case 1:return "0";break;
	default:return "1";
    }
}
QString fmFPeditor::getMarginwidthValue() {
    return QString::number( sbLeft->value(),10 );
}
QString fmFPeditor::getMarginheightValue() {
    return QString::number( sbTop->value(),10 );
}


void fmFPeditor::setup(QMap<QString,QString> m){
    leId->setText(m["id"]);
    fc->setURL(m["src"]);
    leClass->setText(m["class"]);
    leLongdesc->setText(m["longdesc"]);
    leTitle->setText(m["title"]);
    leStyle->setText(m["style"]);
    leName->setText(m["name"]);

    if(m["noresize"]!="noresize") bgNoresize->setButton(0);
    else bgNoresize->setButton(1);

    if(m["scrolling"]=="yes") bgScrolling->setButton(0);
      else if(m["scrolling"]=="no") bgScrolling->setButton(2);

     if(m["frameborder"]=="0") bgBorder->setButton(1);

    sbLeft->setValue(m["marginwidth"].toInt());
    sbTop->setValue(m["marginheight"].toInt());

}

QMap<QString,QString> fmFPeditor::getAttributeMap(){
  QMap<QString,QString> map;

  map["name"] = getNameValue();
  map["longdesc"] = getLongdescValue();
  map["src"] = getSrcValue();
  map["scrolling"] = getScrollingValue();
  map["id"] = getIdValue();
  map["style"] = getStyleValue();
  map["title"] = getTitleValue();
  map["class"] = getClassValue();
  map["noresize"] = getNoresizeValue();
  map["frameborder"] = getFrameborderValue();
  map["marginwidth"] = getMarginwidthValue();
  map["marginheight"] = getMarginheightValue();

  return map;
}

