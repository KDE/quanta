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
   resetAttributes();
}

void areaAttribute::resetAttributes(){
  attributeMap["name"] = "";
  attributeMap["longdesc"] = "";
  attributeMap["src"] = "";
  attributeMap["scrolling"] = "auto"; // default value
  attributeMap["id"] = "";
  attributeMap["style"] = "";
  attributeMap["title"] = "";
  attributeMap["class"] = "";
  attributeMap["noresize"] = "noresize";
  attributeMap["frameborder"] = "1"; // default value
  attributeMap["marginwidth"] = "10";
  attributeMap["marginheight"] = "10";
}
#include "areaattributedb.moc"


