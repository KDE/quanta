/***************************************************************************
                          tagxml.h  -  description
                             -------------------
    begin                : Пнд Сен 25 14:34:07 EEST 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev <pdima@users.sourceforge.net,yshurik@linuxfan.com>
                           (C) 2004 Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef TAGATTR_H
#define TAGATTR_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

//qt includes
#include <qstring.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qdom.h>

//kde includes
#include <klineedit.h>
#include <kurl.h>

//app includes
#include "colorcombo.h"
#include "filecombo.h"

class QDomElement;
class QWidget;
class QTag;


QDomNode findChild( QDomNode &parent, QString name );


class Attr
{
  public:
    Attr( QDomElement *el, QWidget *, QTag *dtdTag ){ domEl = el; name = domEl->attribute("name",""); m_dtdTag = dtdTag;};
    virtual ~Attr(){};

    virtual QString value()=0;
    virtual void setValue( QString )=0;

    QString attrName() const;
  
  protected:
     QDomElement *domEl;
     QString name;
     QTag *m_dtdTag;
};


class Attr_line : public Attr
{
  protected:
    QLineEdit *line;

  public:
    Attr_line( QDomElement *el, QWidget *w, QTag *dtdTag ) : Attr(el, w, dtdTag)
        {  line = (QLineEdit *)w; }
    virtual ~Attr_line(){};

    virtual QString value() { return line->text(); }
    virtual void setValue( QString s ) { line->setText(s); }
};


class Attr_color : public Attr
{
  protected:
    ColorCombo *color;

  public:
    Attr_color( QDomElement *el, QWidget *w, QTag *dtdTag ) : Attr(el, w, dtdTag)
        { color = (ColorCombo *)w; }
    virtual ~Attr_color(){};

    virtual QString value() {  return color->colorName(); }
    virtual void setValue( QString s ) { color->setColorName(s); }
};

class Attr_file : public Attr
{
  protected:
    FileCombo *file;

  public:
    Attr_file( QDomElement *el, QWidget *w , QTag * dtdTag ) : Attr(el, w, dtdTag)
        { file = (FileCombo *)w; }
    virtual ~Attr_file(){};

    virtual QString value() {  return file->text(); }
    virtual void setValue( QString s ) { file->setText(s); }
};

class Attr_list : public Attr
{
  protected:
    QComboBox *combo;

  public:
    Attr_list( QDomElement *el, QWidget *w, QTag *dtdTag );
    virtual ~Attr_list(){};

    virtual QString value() { return combo->currentText(); }
    virtual void setValue( QString s );
};


class Attr_check : public Attr
{
  protected:
    QCheckBox *check;

  public:
    Attr_check( QDomElement *el, QWidget *w, QTag *dtdTag ) : Attr(el, w, dtdTag)
        { check = (QCheckBox *)w; }
    virtual ~Attr_check(){};

    virtual QString value() { return check->isChecked() ? "checked" : "unchecked" ; }
    virtual void setValue( QString s ) { check->setChecked( !s.isEmpty() ); }
};



#endif
