/***************************************************************************
                          dcopquanta.cpp  -  description
                             -------------------
    begin                : Thu Mar 4 2004
    copyright            : (C) 2004 Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

//kde includes

//qt includes
#include <qregexp.h>

//app includes
#include "dcopquanta.h"
#include "node.h"

extern GroupElementMapList globalGroupMap;

DCOPQuanta::DCOPQuanta() : DCOPObject("QuantaIf")
{
}

QStringList DCOPQuanta::selectors(const QString& tag)
{
  QStringList selectorList;
  GroupElementMapList::Iterator it;
  for ( it = globalGroupMap.begin(); it != globalGroupMap.end(); ++it )
  {
    QString key = it.key();
    if (key.startsWith("Selectors|"))
    {
      QString selectorName = key.mid(10);
      QString tmpStr;
      int index = selectorName.find(QRegExp("\\.|\\#|\\:"));
      if (index != -1)
      {
        tmpStr = selectorName.left(index).lower();
      } else
      {
        tmpStr = selectorName;
      }
      if (tmpStr.isEmpty() || tag.lower() == tmpStr || tmpStr == "*")
      {
        selectorList << selectorName.mid(index + 1).replace('.',' ');
      }
     }
  }
  return selectorList;
}

QStringList DCOPQuanta::idSelectors()
{
  QStringList selectorList;
  GroupElementMapList::Iterator it;
  for ( it = globalGroupMap.begin(); it != globalGroupMap.end(); ++it )
  {
    QString key = it.key();
    if (key.startsWith("Selectors|"))
    {
      QString selectorName = key.mid(10);
      QString tmpStr;
      if (selectorName.startsWith("#"))
      {
          selectorList << selectorName.mid(1);
      }
     }
  }
  return selectorList;
}
