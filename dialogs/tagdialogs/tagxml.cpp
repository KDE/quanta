/***************************************************************************
                          tagxml.cpp  -  description
                             -------------------
    begin                : � � 25 14:34:07 EEST 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon <pdima@users.sourceforge.net,yshurik@penguinpowered.com,sequitur@easystreet.com>
                           (C) 2002-2004 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "tagxml.h"
#include "tagattr.h"
#include <klineedit.h>
#include <kdebug.h>
#include <klocale.h>

#include <qcheckbox.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

#include "styleeditor.h"

Tagxml::Tagxml( QDomNode &d, QTag *dtdTag, QWidget *parent, const char *name)
    :QWidget(parent,name), TagWidget(parent,name), doc(d)
{
    initialize(d, dtdTag);
}

Tagxml::Tagxml( QDomNode &d, QTag *dtdTag, const QString& selection, QWidget *parent, const char *name)
    : QWidget(parent,name), TagWidget(parent,name), doc(d), m_selection(selection)
{
    initialize(d, dtdTag);
}

Tagxml::~Tagxml()
{
}

void Tagxml::initialize(QDomNode &d, QTag *dtdTag)
{
    m_dtd = dtdTag->parentDTD;
    m_dtdTag = dtdTag;
    QGridLayout *grid = new QGridLayout( this );
    grid->setSpacing( 13 );
    grid->setMargin( 11 );
    m_firstItem = 0L;
//parse the tag XML file, in order to build up the dialog
    for ( QDomNode n = d.firstChild(); !n.isNull(); n = n.nextSibling() )
    {
        QDomNode location = findChild(n,"location");
        if ( location.isNull() )
            continue;

     //debug( n.nodeName() );

        QDomElement el = location.toElement();

        int row = el.attribute("row","0").toInt();
        int col = el.attribute("col","0").toInt();
        int colspan = el.attribute("colspan","1").toInt()-1;
        int rowspan = el.attribute("rowspan","1").toInt()-1;

        location = findChild(n,"textlocation");
        el = location.toElement();
        int textrow = el.attribute("row","0").toInt();
        int textcol = el.attribute("col","0").toInt();
        int textcolspan = el.attribute("colspan","1").toInt()-1;
        int textrowspan = el.attribute("rowspan","1").toInt()-1;

     // qDebug("%s col:%d row:%d cs:%d, rs:%d", n.nodeName().data(), col,row,colspan,rowspan );

        QString tip;
        QDomNode tooltip = findChild( n ,"tooltip" );
        if ( !tooltip.isNull() ) {
            if ( tooltip.firstChild().isText() ) {
                QDomText text = tooltip.firstChild().toText();
                tip = text.data();
            }
        }
        QString whatsThis;
        QDomNode what = findChild( n ,"whatsthis" );
        if ( !what.isNull() ) {
            if ( what.firstChild().isText() ) {
                QDomText text = what.firstChild().toText();
                whatsThis = text.data();
            }
        }

        if ( n.nodeName() == "label" ) //a static label
        {
            QLabel *label = new QLabel(this);
            QDomElement ltext = findChild(n,"text").toElement();
            if ( !ltext.isNull() )
                label->setText( ltext.text().isEmpty() ? QString("") : (ltext.text()+":") );

            if ( !tip.isNull() )
                QToolTip::add( label, tip );
            if ( !whatsThis.isNull() )
                QWhatsThis::add( label, whatsThis );

            grid->addMultiCellWidget( label, row, row+rowspan, col,  col+colspan );
        }

        if ( n.nodeName() == "attr" ) //an attribute
        {

            QDomElement el(n.toElement());
            QString type = el.attribute("type","input");

            QDomElement ltext = findChild(n,"text").toElement();
            if ( !ltext.isNull() && (type != "check") ) //if there is a text label for the attribute
            {
                QLabel *label = new QLabel(this);
                label->setText( ltext.text().isEmpty() ? QString("") : (ltext.text()+":") );

                if ( !tip.isNull() )
                    QToolTip::add( label, tip );
                if ( !whatsThis.isNull() )
                    QWhatsThis::add( label, whatsThis );
                grid->addMultiCellWidget( label, textrow, textrow+textrowspan, textcol,  textcol+textcolspan );
            }


        //look for the different attribute types
            if ( type == "input" )
            {
                KLineEdit *w = new KLineEdit(this);
                grid->addMultiCellWidget( w, row, row+rowspan, col,  col+colspan );
           
                if ( !tip.isNull() )
                    QToolTip::add( w, tip );
                if ( !whatsThis.isNull() )
                    QWhatsThis::add( w, whatsThis );

                Attr_line *attr = new Attr_line(el, w, m_dtdTag);
                attributes.append(attr);
                if (!m_firstItem)
                    m_firstItem = w;
            } 
            else
                if ( type == "check" )
            {
                QCheckBox *w = new QCheckBox(this);
                grid->addMultiCellWidget( w, row, row+rowspan, col,  col+colspan );

                QDomElement ltext = findChild(n,"text").toElement();
                if ( !ltext.isNull() )
                    w->setText( ltext.text() );
                if ( !tip.isNull() )
                    QToolTip::add( w, tip );
                if ( !whatsThis.isNull() )
                    QWhatsThis::add( w, whatsThis );

                Attr_check *attr = new Attr_check(el, w, m_dtdTag);
                attributes.append(attr);
                if (!m_firstItem)
                    m_firstItem = w;
            } else
                if ( type == "list" )
                {
                    QComboBox *w = new QComboBox(true, this);
                    grid->addMultiCellWidget( w, row, row+rowspan, col,  col+colspan );
                    if ( !tip.isNull() )
                        QToolTip::add( w, tip );
                    if ( !whatsThis.isNull() )
                        QWhatsThis::add( w, whatsThis );
                    Attr_list *attr = new Attr_list(el, w, dtdTag);
                    attributes.append(attr);
                    if (!m_firstItem)
                        m_firstItem = w;
                } else
                    if ( type == "color" )
                    {
                        ColorCombo *w = new ColorCombo(this);
                        grid->addMultiCellWidget( w, row, row+rowspan, col,  col+colspan );

                        if ( !tip.isNull() )
                            QToolTip::add( w, tip );
                        if ( !whatsThis.isNull() )
                            QWhatsThis::add( w, whatsThis );

                        Attr_color *attr = new Attr_color(el, w, m_dtdTag);
                        attributes.append(attr);
                        if (!m_firstItem)
                            m_firstItem = w;
                    } else
                        if ( type == "url" )
                        {
                            FileCombo *w = new FileCombo(baseURL, this);
                            grid->addMultiCellWidget( w, row, row+rowspan, col,  col+colspan );
           
                            if ( !tip.isNull() )
                                QToolTip::add( w, tip );
                            if ( !whatsThis.isNull() )
                                QWhatsThis::add( w, whatsThis );
                            Attr_file *attr = new Attr_file(el, w, m_dtdTag);
                            attributes.append(attr);
                            if (!m_firstItem)
                                m_firstItem = w;
                        }
        
                        else
                            if ( type == "css-style" )
                        {
                            StyleEditor *w = new StyleEditor(this);
                            grid->addMultiCellWidget( w, row, row+rowspan, col,  col+colspan );

                            if ( !tip.isNull() )
                                QToolTip::add( w, tip );
                            if ( !whatsThis.isNull() )
                                QWhatsThis::add( w, whatsThis );

                            Attr_line *attr = new Attr_line(el, w->lineEdit(), m_dtdTag);
                            attributes.append(attr);
           
                            if (!m_firstItem)
                                m_firstItem = w;
           
                        }
        
        }

        if ( n.nodeName() == "spacer")
        {
            QDomElement el = n.toElement();

            QSpacerItem* spacer;

            if ( el.attribute("orientation","v") == "v" )
                spacer = new QSpacerItem( 5, 10, QSizePolicy::Fixed, QSizePolicy::Expanding );
            else
                spacer = new QSpacerItem( 10, 5, QSizePolicy::Expanding, QSizePolicy::Fixed );
            grid->addItem(spacer,row,col);
        }

    }
}

void Tagxml::focusToFirstItem()
{
  if(m_firstItem)
    m_firstItem->setFocus();
}

void Tagxml::readAttributes( QDict<QString> *d )
{
  QString name,value;

  Attr * attr = attributes.first();
   while ( attr ) {
     name = attr->attrName();
     value = attr->value();

     if ( value.isEmpty() )
        d->remove(name);
     else
     {
      if ( dynamic_cast<Attr_check *>(attr) ) // checkbox
      {      
        if (value == "checked")
        {
          if (m_dtd->booleanAttributes == "simple")
          {
            d->replace(name, new QString("") );
          } else
          {
            d->replace(name, new QString(m_dtd->booleanTrue)); //it seems that browsers don't like <input disabled="false">, so if a checkbox is false, don't put in the tag at all
          }
        }  
        else  
        {
          d->remove(name);
        }
/*        } else
        {
          value = (value == "checked")?m_dtd->booleanTrue:m_dtd->booleanFalse;
          d->replace(name, new QString(value));
        } */
      } else
      if (dynamic_cast<Attr_file *>(attr))
      {
//        value = KURL::encode_string(value);
        d->replace(name, new QString(value));
      } else {
        value.replace(QRegExp("&(?!amp;)"), "&amp;");
        d->replace(name, new QString(value) );
      }
    }


    attr = attributes.next();
  }

}

void Tagxml::writeAttributes( QDict<QString> *d )
{
    QString name,value;

    Attr * attr = attributes.first();
  
    int count = 0;
    while ( attr ) {

        name = attr->attrName();

        QString *v = d->find(name);
        if ( v ) {
            v->replace("&amp;","&");
            if ( dynamic_cast<Attr_check *>(attr) ) // checkbox
                value = "checked";
            else
                if ( dynamic_cast<Attr_file *>(attr))
                    value = KURL::decode_string(*v);
            else
                value = *v;
        }
        else
        {
            value = "";
        
            Attribute* attrib = m_dtdTag->attribute(name);
            if(attrib && attrib->source.lower() == "selection")
                value = m_selection;
        }

        attr->setValue( value );

        attr = attributes.next();
        ++count;
    }
}

#include "tagxml.moc"
