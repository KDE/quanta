/***************************************************************************
                          messageoutput.h  -  description
                             -------------------
    begin                : Thu Feb 24 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavski <pdima@mail.univ.kiev.ua>
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

#ifndef MESSAGEOUTPUT_H
#define MESSAGEOUTPUT_H

#include <qlistbox.h>

class KProcess;
class KPopupMenu;

class MessageItem;

/**class for displaing of
weblint output
  *@author Yacovlev Alexander & Dmitry Poplavski
  */

class MessageOutput : public QListBox  {
   Q_OBJECT
public:
  MessageOutput(QWidget *parent=0, const char *name=0);
  ~MessageOutput();
  // Return whole content as a single string with EOLs
  QString content();

public slots:
  /**  Inserts an item with the content s to the end of the list*/
  MessageItem *insertItem(const QString& s);
  
  /** Insert message for each line of string (EOL-separated).
    @param line The line this message refers to. -1 if it's not specified (the first number is taken from the message string)
    @param column The column this message refers to. -1 if it's not specified (the second number is taken from the message string)
    @param fileName The URL of the file this message refers to. If empty, the string following the "File: " string in the message is used.
    @param message The message itself
    @param append Append the message to the previous one or start a new line.  Default is false, start a new line.
  */
  void showMessage(int line, int column, const QString &fileName, const QString& message, bool append = false);
  /** Insert message for each line of string (EOL-separated).
    @param message The message itself
    @param append Append the message to the previous one or start a new line. Default is false, start a new line.
  */
  void showMessage(const QString& message, bool append = false);
  /** Append message string to the last item
    @param line The line this message refers to. -1 if it's not specified (the first number is taken from the message string)
    @param column The column this message refers to. -1 if it's not specified (the second number is taken from the message string)
    @param fileName The URL of the file this message refers to. If empty, the string following the "File: " string in the message is used.
    @param message The message itself
  */
  void addToLastItem(const QString& message);
  // Copy message content to clipboard
  void copyContent();
  // Save message content to clipboard
  void saveContent();


public:
  uint maxItems() const { return m_maxItems; }
  void setMaxItems( uint items ) {  m_maxItems = items; }
  /** remove first items , if count > m_maxItem  */
  void checkMaxItems();

signals:
  void clicked(const QString& fname, int line, int col);

protected slots:
  void clickItem( QListBoxItem *);
  // Show context menu
  void showMenu(QListBoxItem*, const QPoint&);

private:
  uint m_maxItems;
  KPopupMenu* m_popupMenu;
};

#endif
