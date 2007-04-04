/***************************************************************************
 *   Copyright (C) 2005 by Jens Herden                                     *
 *   jens@kdewebdev.org                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "htmlpart.h"
#include "htmlpreviewpart.h"

#include <kurl.h>
#include <klibloader.h>
#include <kparts/part.h>
#include <kstandarddirs.h>
#include <khtmlview.h>

//kdevelop includes
#include <core.h>
#include <idocumentcontroller.h>


HTMLPart::HTMLPart(HTMLPreviewPart *part)
  : KHTMLPart(), m_part(part)
{
  
  //FIXME: ?
//  setOptions(CanOpenInNewWindow);
}


HTMLPart::~HTMLPart()
{}


void HTMLPart::slotDuplicate( )
{
  //FIXME: how to show the docs?
//  KDevelop::Core::self()->documentController()->showDocumentation(url(), true);
}


void HTMLPart::slotOpenInNewWindow( const KUrl & url )
{
  //FIXME: how to show the docs?
  //KDevelop::Core::self()->documentController()->showDocumentation(url, true);
}

#include "htmlpart.moc"

// kate: space-indent on; indent-width 2; mixedindent off; indent-mode cstyle;
