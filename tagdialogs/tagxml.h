/***************************************************************************
                          tagxml.h  -  description
                             -------------------
    begin                : Пнд Сен 25 14:34:07 EEST 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
                           (C) 2003 by Andras Mantia <amantia@freemail.hu>
    email                : pdima@users.sourceforge.net,yshurik@linuxfan.com,sequitur@easystreet.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef TAGXML_H
#define TAGXML_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <qwidget.h>
#include <qdom.h>
#include <qptrlist.h>

#include "tagattr.h"
#include "tagwidget.h"
#include "../parser/qtag.h"

class Tagxml : public QWidget, TagWidget {
  Q_OBJECT
  public:
    /** construtor */
    Tagxml(QDomDocument &d, DTDStruct *dtd, QWidget* parent=0, const char *name=0);
    /** destructor */
    ~Tagxml();

    QPtrList<Attr> attributes;

    void readAttributes( QDict<QString> *d);
    void writeAttributes( QDict<QString> *d );
    void focusToFirstItem();

  private:
    QDomDocument doc;
    DTDStruct *m_dtd;
    QWidget *m_firstItem;

};



#endif
