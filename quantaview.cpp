/***************************************************************************
                          quantaview.cpp  -  description
                             -------------------
    begin                : ��� ���  9 13:29:57 EEST 2000
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

// include files for Qt
#include <qprinter.h>
#include <qpainter.h>

// application specific includes
#include "document.h"
#include "quantaview.h"
#include "quantadoc.h"
#include "quanta.h"
#include "resource.h"
#include "quantacommon.h"


/////////////////////////////////////////////////////////////////////
// SLOT CALLBACK IMPLEMENTATION
/////////////////////////////////////////////////////////////////////

/**  */
void QuantaView::insertTag( const char *tag)
{

  QString startTag = QuantaCommon::tagCase(tag);

  if ( ( singleTags->find( startTag.upper() )!= -1 ) ||
     ( ( optionalTags->find(startTag.upper())!= -1 ) && (!useCloseTag)))
      write()->insertTag( QString("<")+startTag+">" );
  else
      write()->insertTag( QString("<")+startTag+">", QString("</")+startTag+">");
}
#include "quantaview.moc"
