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
#include <qptrlist.h>
#include <qrect.h>
#include <qstring.h>
#include <qmap.h>
#include <qdict.h>
#include <qobject.h>
/**this is
  *@author gulmini luciano
  */

class areaAttribute : public QObject{
    Q_OBJECT
    private:
    QRect geometry;

    //tag specific attributes
    QString name,
            longdesc,
            src,
            scrolling,
            id,
            style,
            title,
            Class;
    bool noresize,
         frameborder;
    int marginwidth,
        marginheight;
    //end tag specific attributes

    public:
    areaAttribute();
    areaAttribute(areaAttribute*);
    ~areaAttribute(){};

    void setSrc(QString s) { src = s; }
    void setName(QString s) { name = s; }
    void setLongdesc(QString s) { longdesc = s; }
    void setScrolling(QString s) { scrolling = s; }
    void setId(QString s) { id = s; }
    void setStyle(QString s) { style = s; }
    void setTitle(QString s) { title = s; }
    void setClass(QString s) { Class = s; }
    void setNoresize(bool b) { noresize = b; }
    void setFrameborder(bool b) { frameborder = b; }
    void setMarginwidth(int i) { marginwidth = i; }
    void setMarginheight(int i) { marginheight = i; }
    void setAllAttribute(QMap<QString,QString>);
    QRect getGeometry() const { return geometry; }

    QString getSrc() const{ return src; }
    QString getName() const { return name; }
    QString getLongdesc() const { return longdesc; }
    QString getScrolling() const { return scrolling; }
    QString getId() const { return id; }
    QString getStyle() const { return style; }
    QString getTitle() const { return title; }
    QString getClass() const { return Class; }
    QString getNoresize() const { if(noresize) return "noresize";return ""; }
    QString getFrameborder() const { if(frameborder) return "1";return "0"; }
    QString getMarginwidth() const { return QString::number(marginwidth,10); }
    QString getMarginheight() const { return QString::number(marginheight,10); }
    QMap<QString,QString> getAttributeMap();
    
    public slots:
      void setGeometry(QRect g) { geometry = g; }
};
typedef QDict<areaAttribute> areaAttributeDB;


#endif
