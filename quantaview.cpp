/***************************************************************************
                          quantaview.cpp  -  description
                             -------------------
    begin                : Втр Май  9 13:29:57 EEST 2000
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

  Document *w = write();
  QString tagStr = QuantaCommon::tagCase(tag);
  DTDStruct *dtd = w->currentDTD(true);
  bool single = QuantaCommon::isSingleTag(dtd->name, tagStr);
  bool optional = QuantaCommon::isOptionalTag(dtd->name, tagStr);

  QString startTag = tagStr;
  startTag.prepend("<");
  if ( dtd->singleTagStyle == "xml" &&
       ( single || (optional && !qConfig.closeOptionalTags))
     )
  {
    startTag.append(" /");
  }
  startTag.append(">");
  
  if ( (qConfig.closeTags && !single && !optional) ||
       (qConfig.closeOptionalTags && optional) )
  {
    w->insertTag( startTag, QString("</")+tagStr+">");
  }
  else
  {
    w->insertTag(startTag);
  }
}
#include "quantaview.moc"
