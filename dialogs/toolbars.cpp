/***************************************************************************
                          toolbars.cpp  -  description
                             -------------------
    begin                : Fri Oct 13 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
    email                : pdima@users.sourceforge.net,yshurik@penguinpowered.com,sequitur@easystreet.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "toolbars.h"
#include <kiconloader.h>
#include <qbuffer.h>
#include <qimage.h>


ToolBars::ToolBars( QDomDocument doc )
{
	d = doc;
	dtoolbars = d.namedItem("tabbars").childNodes();
}

ToolBars::~ToolBars(){
}


QDomElement ToolBars::action( int id )
{
  for ( unsigned t=0; t < dtoolbars.count(); t++ )
  {
    QDomNodeList dactions = dtoolbars.item(t).childNodes();
    for ( unsigned int i=0; i < dactions.count(); i++ )
    {
      QDomElement el = dactions.item(i).toElement();
      if ( el.attribute("id","-1").toInt() == id  )
        return el;
    }
  }

  return QDomElement();
}

QPixmap ToolBars::actionIcon( int id )
{
  QString sicon = action(id).attribute("icon","ball");
  return UserIcon(sicon);
/*  QDomElement el = action(id).namedItem("icon").toElement();
  QString itype = el.attribute("type","userIcon");
  QString idata = el.attribute("data","ball");

  if ( itype == "png" ) {
      QPixmap img;
      char *ba = new char[ idata.length() / 2 ];
      for ( int i = 0; i < (int)idata.length() / 2; ++i ) {
      	char h = idata[ 2 * i ].latin1();
      	char l = idata[ 2 * i  + 1 ].latin1();
      	uchar r = 0;
      	if ( h <= '9' )
      	    r += h - '0';
      	else
      	    r += h - 'a' + 10;
      	r = r << 4;
      	if ( l <= '9' )
  	        r += l - '0';
      	else
      	    r += l - 'a' + 10;
      	ba[ i ] = r;
      }
    	img.loadFromData( (const uchar*)ba, idata.length() / 2, "png" );
      delete [] ba;
      return img;
  }
  else {
      return  UserIcon( idata );
  }
*/
}

/*void ToolBars::setActionIcon( int id, QString icon)
{
  action(id).setAttribute("icon",icon);
  QDomElement el = d.createElement("icon");
  el.setAttribute("type","png");

  QByteArray ba;
  QBuffer buf( ba );
  buf.open( IO_WriteOnly );
  QImageIO iio( &buf, "PNG" );
  iio.setImage( icon.convertToImage() );
  iio.write();
  buf.close();

  QString s = "";
  static const char hexchars[] = "0123456789abcdef";


  for ( int i = 0; i < (int)ba.size(); i++  ) {
    uchar ch = ba[i];
    s += hexchars[ch >> 4];
    s += hexchars[ch & 0x0f];
  }

  el.setAttribute("data",s);

  QDomElement iaction = action(id);
  QDomElement oldel = iaction.namedItem("icon").toElement();
  if ( !oldel.isNull() )
    iaction.removeChild(oldel);

  iaction.appendChild(el);

}
*/

