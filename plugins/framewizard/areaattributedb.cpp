/***************************************************************************
                          areaattributedb.cpp  -  description
                             -------------------
    begin                : gio mar 20 2003
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

#include "areaattributedb.h"

areaAttribute::areaAttribute(){
  name="";
  longdesc="";
  src="";
  scrolling="auto";
  id="";
  style="";
  title="";
  Class="";
  noresize=true;
  frameborder=true;
  marginwidth=10;
  marginheight=10;
}

areaAttribute::areaAttribute(areaAttribute* a){
  name = a->name;
  longdesc = a->longdesc;
  src = a->src;
  scrolling = a->scrolling;
  id = a->id;
  style = a->style;
  title = a->title;
  Class = a->Class;
  noresize = a->noresize;
  frameborder = a->frameborder;
  marginwidth = a->marginwidth;
  marginheight = a->marginheight;
}

QMap<QString,QString> areaAttribute::getAttributeMap(){
  QMap<QString,QString> map;

  map["name"] = getName();
  map["longdesc"] = getLongdesc();
  map["src"] = getSrc();
  map["scrolling"] = getScrolling();
  map["id"] = getId();
  map["style"] = getStyle();
  map["title"] = getTitle();
  map["class"] = getClass();
  map["noresize"] = getNoresize();
  map["frameborder"] = getFrameborder();
  map["marginwidth"] = getMarginwidth();
  map["marginheight"] = getMarginheight();

  return map;
}

void areaAttribute::setAllAttribute(QMap<QString,QString> map){
  setName(map["name"]);
  setLongdesc(map["longdesc"]);
  setSrc(map["src"]);
  setScrolling(map["scrolling"]);
  setId(map["id"]);
  setStyle(map["style"]);
  setTitle(map["title"]);
  setClass(map["class"]);
  if(map["noresize"] == "noresize" ) setNoresize(true);
  else setNoresize(false);
  if(map["frameborder"] == "1" ) setFrameborder(true);
  else setFrameborder(false);
  setMarginwidth(map["marginwidth"].toInt());
  setMarginheight(map["marginheight"].toInt());


}

#include "areaattributedb.moc"
