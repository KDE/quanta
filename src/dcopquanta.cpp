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
  const QRegExp rx("\\.|\\#|\\:");
  QStringList selectorList;
  GroupElementMapList::Iterator it;
  for ( it = globalGroupMap.begin(); it != globalGroupMap.end(); ++it )
  {
    QString key = it.key();
    if (key.startsWith("Selectors|"))
    {
      QString selectorName = key.mid(10);
      int index = selectorName.find(':');
      if (index != -1)
        selectorName = selectorName.mid(0, index);
      QString tmpStr;
      index = selectorName.find(rx);
      if (index != -1)
      {
        tmpStr = selectorName.left(index).lower();
      } else
      {
        tmpStr = selectorName;
      }
      if (tmpStr.isEmpty() || tag.lower() == tmpStr || tmpStr == "*")
      {
        tmpStr = selectorName.mid(index + 1).replace('.',' ');
        if (!tmpStr.isEmpty() && !selectorList.contains(tmpStr))
          selectorList << tmpStr;
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

QStringList DCOPQuanta::groupElements(const QString& group)
{
  QStringList elementList;
  GroupElementMapList::Iterator it;
  for ( it = globalGroupMap.begin(); it != globalGroupMap.end(); ++it )
  {
    QString key = it.key();
    if (key.startsWith(group + "|"))
    {
      QString name = key.mid(10);
      int index = name.find(':');
      if (index != -1)
        name = name.mid(0, index);
      QString tmpStr;
      index = name.find("|");
      if (index != -1)
      {
        tmpStr = name.left(index).lower();
      } else
      {
        tmpStr = name;
      }
    
      elementList << tmpStr;
     }
  }
  return elementList;
}
