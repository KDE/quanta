/***************************************************************************
                          messageoutput.cpp  -  description
                             -------------------
    begin                : Jan 12 2001
    copyright            : (C) 2001 by Dmitry Poplavsky <dima@kde.org>
                           (C) 2002-2004 Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "messageitem.h"
#include <qregexp.h>


MessageItem::MessageItem( QListBox * listbox, const QString &text, int line, const QString &fname )
  :QListBoxText(listbox,text),lineNumber(line),filename(fname)
{
  lineDefined = ( line!=-1 );
}

int MessageItem::line()
{
  if ( lineDefined ) {
    return lineNumber;
  }
  else {  // try to find first number in text
    QString s = text();
    int pos = s.find( QRegExp("[0-9]"));
    if ( pos == -1 )
      return lineNumber;
    int pos2 = pos;
    while ( s[pos2].isDigit() ) pos2++;

    QString snum = s.mid( pos, pos2-pos );
    return snum.toInt();

  }
}

int MessageItem::column()
{
  QString s = text();
  QRegExp exp("(?:\\D|^)(\\d{1,4})(?:\\D|$)");
  
  int pos = exp.search(s);
  if ( pos == -1 )
    return 0;
  pos = exp.search(s, pos + exp.cap(1).length());  
  if ( pos == -1 )
    return 0;
  s = exp.cap(1);
  return s.toInt();
}

void MessageItem::addText( QString t )
{
   setText( text() + t );
}

