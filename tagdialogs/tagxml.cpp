/***************************************************************************
                          tagxml.cpp  -  description
                             -------------------
    begin                : Пнд Сен 25 14:34:07 EEST 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
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

#include "tagxml.h"
#include "tagattr.h"
#include <klineedit.h>
#include <qcheckbox.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

Tagxml::Tagxml( QDomDocument &d, QWidget *parent, const char *name)
	:TagWidget(parent,name), QWidget(parent,name), doc(d)
{
   QGridLayout *grid = new QGridLayout( this );
   grid->setSpacing( 13 );
   grid->setMargin( 11 );

   for ( QDomNode n = d.firstChild().firstChild().firstChild(); !n.isNull(); n = n.nextSibling() )
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

     warning( "quanta: tooltip '%s'\n", (const char *) tip );
     warning( "quanta: whatsthis '%s'\n", (const char *) whatsThis );

     if ( n.nodeName() == "label" ) {
     	  QLabel *label = new QLabel(this);
     	  QDomElement ltext = findChild(n,"text").toElement();
     	  if ( !ltext.isNull() )
     	  	label->setText( ltext.text() );

          if ( tip != QString::null )
              QToolTip::add( label, tip );
          if ( whatsThis != QString::null )
              QWhatsThis::add( label, whatsThis );
     	  	
     	  grid->addMultiCellWidget( label, row, row+rowspan, col,  col+colspan );
     }

     if ( n.nodeName() == "attr" ) {

     		QDomElement el = n.toElement();
        QString type = el.attribute("type","input");

        if ( type == "input" ) {
        	 KLineEdit *w = new KLineEdit(this);
        	 grid->addMultiCellWidget( w, row, row+rowspan, col,  col+colspan );

             if ( tip != QString::null )
                 QToolTip::add( w, tip );
             if ( whatsThis != QString::null )
                 QWhatsThis::add( w, whatsThis );

        	 Attr_line *attr = new Attr_line(&el,w);
        	 attributes.append(attr);
        }

        if ( type == "check" ) {
        	 QCheckBox *w = new QCheckBox(this);
        	 grid->addMultiCellWidget( w, row, row+rowspan, col,  col+colspan );
        	
        	 QDomElement ltext = findChild(n,"text").toElement();
		     	 if ( !ltext.isNull() )
    	 	  	 w->setText( ltext.text() );
        	
             if ( tip != QString::null )
                 QToolTip::add( w, tip );
             if ( whatsThis != QString::null )
                 QWhatsThis::add( w, whatsThis );

        	 Attr_check *attr = new Attr_check(&el,w);
        	 attributes.append(attr);
        }

        if ( type == "list" ) {
        	 QComboBox *w = new QComboBox(true,this);
        	 grid->addMultiCellWidget( w, row, row+rowspan, col,  col+colspan );
        	
             if ( tip != QString::null )
                 QToolTip::add( w, tip );
             if ( whatsThis != QString::null )
                 QWhatsThis::add( w, whatsThis );

        	 Attr_list *attr = new Attr_list(&el,w);
        	 attributes.append(attr);
        }

        if ( type == "color" ) {
        	 ColorCombo *w = new ColorCombo(this);
        	 grid->addMultiCellWidget( w, row, row+rowspan, col,  col+colspan );
        	
             if ( tip != QString::null )
                 QToolTip::add( w, tip );
             if ( whatsThis != QString::null )
                 QWhatsThis::add( w, whatsThis );

        	 Attr_color *attr = new Attr_color(&el,w);
        	 attributes.append(attr);
        }

        if ( type == "url" ) {
        	 FileCombo *w = new FileCombo(basePath, this);
        	 grid->addMultiCellWidget( w, row, row+rowspan, col,  col+colspan );
        	
             if ( tip != QString::null )
                 QToolTip::add( w, tip );
             if ( whatsThis != QString::null )
                 QWhatsThis::add( w, whatsThis );

        	 Attr_file *attr = new Attr_file(&el,w);
        	 attributes.append(attr);
        }

     	
     }

     if ( n.nodeName() == "spacer") {
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

Tagxml::~Tagxml()
{
}


void Tagxml::readAttributes( QDict<QString> *d )
{
  QString name,value;

  Attr * attr = attributes.first();
 	while ( attr ) {
 	  name = attr->attrName();
 	  value = attr->value();
 	
 	  if ( value.isEmpty() ) d->remove(name);
    else {
      if ( dynamic_cast<Attr_check *>(attr) ) // checkbox
      	d->replace(name, new QString("") );
      else
	      d->replace(name, new QString(value) );
    }

 	   	
	  attr = attributes.next();
  }

}

void Tagxml::writeAttributes( QDict<QString> *d )
{
	QString name,value;

  Attr * attr = attributes.first();
 	while ( attr ) {
 		
 	  name = attr->attrName();
 	
 	  QString *v = d->find(name);
 	  if ( v ) {
 	  	if ( dynamic_cast<Attr_check *>(attr) ) // checkbox
      	value = "checked";
      else
		 	  value = *v;
		}
		else value = "";
	 	
 	
 	  attr->setValue( value );
 	   	
	  attr = attributes.next();
  }


}

