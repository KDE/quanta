/***************************************************************************
                          messageoutput.h  -  description
                             -------------------
    begin                : Thu Feb 24 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavski <pdima@mail.univ.kiev.ua>
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

#ifndef MESSAGEOUTPUT_H
#define MESSAGEOUTPUT_H

#include <qlistbox.h>

class KProcess;
class KPopupMenu;

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
  /**  */
  void insertItem(const QString& s );
  // Insert message for each line of string (EOL-separated).
  void showMessage(const QString& s, bool append = false);
  // Append string to the last item
  void addToLastItem(const QString& s );
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
