/***************************************************************************
                          areaattributedb.h  -  description
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

#ifndef AREAATTRIBUTEDB_H
#define AREAATTRIBUTEDB_H
#include <qrect.h>
#include <qstring.h>
#include <qmap.h>
#include <qobject.h>
/**this is
  *@author gulmini luciano
  */

class areaAttribute : public QObject{
    Q_OBJECT

    private:
      QRect geometry;
      QMap<QString,QString> attributeMap;     //tag specific attributes

    public:
      areaAttribute();
      areaAttribute(areaAttribute* a){ attributeMap = a->getAttributeMap(); }
      ~areaAttribute(){};
      void setAttribute(const QString& name, const QString& value){ attributeMap[name] = value; }
      void setAllAttributes(QMap<QString,QString> map){ attributeMap = map; }
      void resetAttributes();
      QRect getGeometry() const { return geometry; }
      QString getSrc() const{ return attributeMap["src"]; }
      QString getAttributeValue(QString l) { return getAttributeMap()[l];}
      QMap<QString,QString> getAttributeMap() { return attributeMap; }
      void setGeometry(QRect g) { geometry = g; }

    public slots:
      void setGeometry(QRect g,QString l) { geometry = g; }
};

#endif
