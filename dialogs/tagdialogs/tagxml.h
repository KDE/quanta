/***************************************************************************
                          tagxml.h  -  description
                             -------------------
    begin                : � � 25 14:34:07 EEST 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon <pdima@users.sourceforge.net,yshurik@penguinpowered.com,sequitur@easystreet.com>
                           (C) 2003, 2004 by Andras Mantia <amantia@kde.org>
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

#include <qwidget.h>
#include <qdom.h>
#include <qptrlist.h>

#include "tagattr.h"
#include "tagwidget.h"
#include "qtag.h"

class Tagxml : public QWidget, TagWidget {
  Q_OBJECT
  public:
    /** construtor */
    Tagxml(QDomNode &d, QTag *dtdTag, QWidget* parent=0, const char *name=0);
    Tagxml(QDomNode &d, QTag *dtdTag, const QString& selection, QWidget* parent=0, const char *name=0);
    /** destructor */
    ~Tagxml();

    QPtrList<Attr> attributes;

    void readAttributes( QDict<QString> *d);
    void writeAttributes( QDict<QString> *d );
    void focusToFirstItem();

  private:
    void initialize(QDomNode &d, QTag *dtdTag);
      
    QDomNode doc;
    const DTDStruct *m_dtd;
    QWidget *m_firstItem;
    QTag *m_dtdTag;
    QString m_selection;
};



#endif
