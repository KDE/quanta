/***************************************************************************
                          whtmlpart.cpp  -  description
                             -------------------
    begin                : Fri Aug 18 2000
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

#include "whtmlpart.h"

WHTMLPart::WHTMLPart(QWidget *parent, const char *name )
	: KHTMLPart(parent,name)
{
   hpos = 0;
}

WHTMLPart::~WHTMLPart()
{
}

void WHTMLPart::urlSelected( const QString &url, int, int, const QString & )
{
  KURL cURL = completeURL( url );
  openURL( cURL ) ;
  addToHistory( cURL.url() );
}


void WHTMLPart::forward()
{
  if ( forwardEnable() ) {
  	hpos++;
  	openURL( history.at(hpos) );
  	
  	emit updateStatus( backEnable() , forwardEnable() );
  }
}


void WHTMLPart::back()
{
  if ( backEnable() ) {
  	hpos--;
  	openURL( history.at(hpos) );
  	
  	emit updateStatus( backEnable() , forwardEnable() );
  }
}


void WHTMLPart::addToHistory( QString url )
{

   if ( history.count() > 0 )
	   while ( hpos < history.count()-1  )	
  	 		history.removeLast();
   		
   if ( !history.isEmpty() ) hpos++;

   history.append(url);

   hpos = history.count()-1;

   emit updateStatus( backEnable() , forwardEnable() );

}


bool WHTMLPart::backEnable()
{
   return hpos > 0;
}

	
bool WHTMLPart::forwardEnable()
{
   return hpos < history.count()-1;
}



