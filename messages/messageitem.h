/***************************************************************************
                          messageoutput.h  -  description
                             -------------------
    begin                : Jan 12 2001
    copyright            : (C) 2001 by Dmitry Poplavsky
    email                : dima@kde.org
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <qlistbox.h>

/**
  Like QListBoxText with storing of filename and line in it.
*/

class MessageItem : public QListBoxText {
  public:
  
    MessageItem( QListBox *listbox, const QString &text=QString::null, int line=-1, const QString &fname=QString::null);
    
    /** return line number for file , associated with this message */
    int line();
    /** return name of file , associated with this message */
    QString fileName() { return filename; }
    
    void setLine( int line ) { lineNumber = line; lineDefined=true;  }
    void setFileName( QString fileName ) { filename = fileName; }
  private:
  
    // line number and filename , associated with this line
    int lineNumber;
    bool lineDefined;
    QString filename;
  
};

#endif

