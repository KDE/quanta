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
  m_attributeMap["name"] = "";
  m_attributeMap["longdesc"] = "";
  m_attributeMap["src"] = "";
  m_attributeMap["scrolling"] = "auto"; // default value
  m_attributeMap["id"] = "";
  m_attributeMap["style"] = "";
  m_attributeMap["title"] = "";
  m_attributeMap["class"] = "";
  m_attributeMap["noresize"] = "noresize";
  m_attributeMap["frameborder"] = "1"; // default value
  m_attributeMap["marginwidth"] = "10";
  m_attributeMap["marginheight"] = "10";
}
#include "areaattributedb.moc"


