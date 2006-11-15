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

#ifndef TAGATTR_H
#define TAGATTR_H

#include "quantaexport.h"

#include <QString>

class QDomElement;


class KDEVQUANTA_EXPORT TagAttr {
  
public:
  TagAttr() 
    {
      nameLine = nameCol = valueLine = valueCol = 0; 
      quoted = true; 
      special = false;
    }

  /** For Kafka copy/paste */
//   void save(QDomElement& element) const;
//   bool load(QDomElement const& element);

  /** convert attr to upper or lower case */
  static QString convertCase(const QString& tag);


  /** returns the attribute value in quoted form, by taking care of the
  quotation setting
  */
  static QString quoteAttributeValue(const QString & value);
  
  QString name;           //attr name
  QString value;          //attr value
  int nameLine, nameCol;  //where the attr name begins
  int valueLine, valueCol;//where the attr value begins
  bool quoted;            //quote or not the attribute
  bool special;           //true if the attribute is not a real one, instead it's only part
                          //of a special area that's present in the tag. Example:
                          //Tag: <a <? echo $a ?> href="x">, <?, echo, $a and ?> are special attributes
                          //This is important, otherwise they would be wrongly treated as booleans.

};


#endif
