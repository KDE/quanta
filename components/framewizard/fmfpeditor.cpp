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
//#include <kpreviewwidgetbase.h> 

/*class PreviewWidget : public KPreviewWidgetBase {
};*/

fmFPeditor::fmFPeditor() : fmFPeditorS (){ 
  sbLeft->setWrapping(true);
  sbTop->setWrapping(true);
 
  QString htmlFiles = i18n("*.html *.htm|HTML Files");
  QString phpFiles = i18n("*.php|PHP Files"); 
  QString xmlFiles = i18n("*.xml|XML Files"); 
  QString xhtmlFiles = i18n("*xhtml|XHTML Files");
  QString allFiles = i18n("*|All Files"); 
  
  fc->setFilter(htmlFiles+"\n"+phpFiles+"\n"+xmlFiles+"\n"+xhtmlFiles+"\n"+allFiles);

}
fmFPeditor::~fmFPeditor(){
}

QString fmFPeditor::noresizeValue() {
  switch(bgNoresize->id(bgNoresize->selected())){
    case 0:return "";break;
    default:return "noresize";
  }
}

QString fmFPeditor::scrollingValue() {
  switch(bgScrolling->id(bgScrolling->selected())){
    case 0:return "yes";break;
    case 2:return "no";break;
    default:return "auto";
  }
}

QString fmFPeditor::frameborderValue() {
  switch(bgBorder->id(bgBorder->selected())){
    case 1:return "0"; break;
    default:return "1";
  }
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
  else 
  if(m["scrolling"]=="no") bgScrolling->setButton(2);

  if(m["frameborder"]=="0") bgBorder->setButton(1);

  sbLeft->setValue(m["marginwidth"].toInt());
  sbTop->setValue(m["marginheight"].toInt());
}

QMap<QString,QString> fmFPeditor::attributeMap(){
  QMap<QString,QString> map;

  map["name"] = leName->text();
  map["longdesc"] = leLongdesc->text();
  map["src"] = fc->url();
  map["scrolling"] = scrollingValue();
  map["id"] = leId->text();
  map["style"] = leStyle->text();
  map["title"] = leTitle->text();
  map["class"] = leClass->text();
  map["noresize"] = noresizeValue();
  map["frameborder"] = frameborderValue();
  map["marginwidth"] = QString::number( sbLeft->value(),10 );
  map["marginheight"] = QString::number( sbTop->value(),10 );

  return map;
}

#include "fmfpeditor.moc"
