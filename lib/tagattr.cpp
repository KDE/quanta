/***************************************************************************

    begin                : Sun Sep 1 2002
    copyright            : (C) 2002, 2003 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/

#include "tagattr.h"
#include "settings.h"


#include <QString>
#include <qdom.h>

//#include <kdebug.h>


// void TagAttr::save(QDomElement& element) const
// {
//     element.setAttribute("name", name);             // QString
//     element.setAttribute("value", value);           // QString
//     element.setAttribute("nameLine", nameLine);     // int
//     element.setAttribute("nameCol", nameCol);       // int
//     element.setAttribute("valueLine", valueLine);   // int
//     element.setAttribute("valueCol", valueCol);     // int
//     element.setAttribute("quoted", quoted);         // bool
//     element.setAttribute("special", special);       // bool
// }

// bool TagAttr::load(QDomElement const& element)
// {
//     name = element.attribute("name");
//     value = element.attribute("value");
//     nameLine = QString(element.attribute("nameLine")).toInt();
//     nameCol = QString(element.attribute("nameCol")).toInt();
//     valueLine = QString(element.attribute("valueLine")).toInt();
//     valueCol = QString(element.attribute("valueCol")).toInt();
//     quoted = QString(element.attribute("quoted")).toInt();
//     special = QString(element.attribute("special")).toInt();
//
//     return true;
// }

/** convert attr of tag to upper or lower case */
QString TagAttr::convertCase( const QString& attr)
{
  switch (Settings::self()->attributeCase())
  {
    case 1: return attr.toLower();
    case 2: return attr.toUpper();
  }
  return attr;
}


QString TagAttr::quoteAttributeValue(const QString & value)
{
  QString quote = Settings::self()->attributeQuotationChar();
  return quote + value + quote;
}
