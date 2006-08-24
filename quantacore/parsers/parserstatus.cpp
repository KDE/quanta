/***************************************************************************
    begin                : Aug 24 2006
    copyright            : (C) 2006 by Jens Herden <jens@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/

#include <QXmlInputSource>


#include "parserstatus.h"


ParserStatus::ParserStatus(QXmlInputSource *source) :
  QXmlReader(), m_source(source)
{
  Q_ASSERT(source != 0);
}


ParserStatus::~ParserStatus()
{
}


void ParserStatus::startParsing()
{
}

//kate: indent-mode cstyle; space-indent on; indent-width 2; replace-tabs on; mixedindent off; encoding utf-8
