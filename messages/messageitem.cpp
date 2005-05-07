/***************************************************************************
                          messageoutput.cpp  -  description
                             -------------------
    begin                : Jan 12 2001
    copyright            : (C) 2001 by Dmitry Poplavsky <dima@kde.org>
                           (C) 2002-2005 Andras Mantia <amantia@kde.org>
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
#include <klocale.h>

MessageItem::MessageItem( QListBox * listbox, const QString &text, int line, int column, const QString &fname )
  :QListBoxText(listbox,text),lineNumber(line),columnNumber(column),filename(fname)
{
}

int MessageItem::line()
{
  if ( lineNumber != -1 ) {
    return lineNumber;
  }
  else {  // try to find first number in text
    QString s = text();
    int pos = s.find( QRegExp("[0-9]"));
    if ( pos == -1 )
      return lineNumber;
    int pos2 = pos;
    while ( s[pos2].isDigit() ) pos2++;

    QString snum = s.mid(pos, pos2-pos);
    return snum.toInt();

  }
}

int MessageItem::column()
{
  if (columnNumber != -1) 
  {
    return columnNumber;
  } else
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
}

QString MessageItem::fileName()
{
  if (filename.isEmpty())
  {
    QString fname;
    QString s = text();
    int pos = s.find(i18n("File: "));
    if (pos != -1)
    {
      int pos2 = s.find(i18n(", "), pos);
      if (pos2 != -1)
        fname = s.mid(pos, pos2-pos);
      else
        fname = s.mid(pos);  
      fname.remove(i18n("File: "));
    }
    return fname;
  } else
  return filename;
}

void MessageItem::addText(const QString &t)
{
   setText( text() + t );
}

