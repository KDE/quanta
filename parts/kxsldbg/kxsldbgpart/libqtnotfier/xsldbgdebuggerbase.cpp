/***************************************************************************
                          xsldbgdebuggerbase.cpp  -  The base class from
                                                      which a debugger
                                                      could be built

                             -------------------
    begin                : Fri Feb 1 2001
    copyright            : (C) 2001 by Keith Isdale
    email                : k_isdale@tpg.com.au
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <qapplication.h>
#include <qglobal.h>
#include <qstringlist.h>
#include <kurl.h>

#include "xsldbgdebuggerbase.h"

XsldbgDebuggerBase::XsldbgDebuggerBase()
  : QObject(0L, "XsldbgDebuggerBase")
{
  initialized = false;
  updateTimerID = -1;
}


XsldbgDebuggerBase::~XsldbgDebuggerBase()
{
  /* empty*/
}



QString XsldbgDebuggerBase::fromUTF8(const char *text)
{
  QString result;
  if (text != 0L)
    result = (const char*)text;
  return result;
}


QString XsldbgDebuggerBase::fromUTF8(const xmlChar *text)
{
  QString result;
  if (text != 0L)
    result = QString::fromUtf8((const char*)text);
  return result;
}


QString XsldbgDebuggerBase::fromUTF8FileName(const char *text)
{
  QString result;
  if (text != 0L){
    KURL url(((const char*)text));
    if (url.isLocalFile())
	result = QString("file:") + url.path();
    else 
	result = url.prettyURL(); 
  }
  return result;
}


QString XsldbgDebuggerBase::fromUTF8FileName(const xmlChar *text)
{
  QString result;
  if (text != 0L){
    KURL url(QString::fromUtf8((const char*)text));
    if (url.isLocalFile())
	result = QString("file:") + url.path();
    else 
	result = url.prettyURL(); 
  }
  return result;
}

#include "xsldbgdebuggerbase.moc"
