/***************************************************************************
                          messageoutput.h  -  description
                             -------------------
    begin                : Jan 12 2001
    copyright            : (C) 2001 by Dmitry Poplavsky <dima@kde.org>
                           (C) 2002, 2004 Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef MESSAGEITEM_H
#define MESSAGEITEM_H

#include <qlistbox.h>

/**
  Like QListBoxText with storing of filename and line in it.
*/

class MessageItem : public QListBoxText {
  public:

    MessageItem( QListBox *listbox, const QString &text=QString::null, int line=-1, const QString &fname=QString::null);

    /** return line number for file , associated with this message */
    int line();
    /** return column number for file , associated with this message */
    int column();
    /** return name of file , associated with this message */
    QString fileName();

    void setLine(int line) { lineNumber = line; lineDefined=true; }
    void setFileName(const QString &fileName ) { filename = fileName; }
    void addText(const QString &text);
  private:

    // line number and filename , associated with this line
    int lineNumber;
    bool lineDefined;
    QString filename;

};

#endif

