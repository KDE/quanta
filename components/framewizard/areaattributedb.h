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
#include <qmap.h>
#include <qobject.h>
/**this is
  *@author gulmini luciano
  */

class areaAttribute : public QObject{
    Q_OBJECT

    private:
      QRect m_geometry;
      QMap<QString,QString> m_attributeMap;     //tag specific attributes

    public:
      areaAttribute();
      ~areaAttribute(){};
      void setAttribute(const QString& name, const QString& value){ m_attributeMap[name] = value; }
      void setAllAttributes(QMap<QString,QString> map){ m_attributeMap = map; }
      void resetAttributes();
      QRect geometry() const { return m_geometry; }
      QString src() const{ return m_attributeMap["src"]; }
      QString attributeValue(QString l) const { return attributeMap()[l];}
      QMap<QString,QString> attributeMap() const { return m_attributeMap; }

    public slots:
      void setGeometry(QRect g) { m_geometry = g; }
};

#endif
