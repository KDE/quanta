/***************************************************************************
                          quantadoc.h  -  description
                             -------------------
    begin                : Втр Май  9 13:29:57 EEST 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon <pdima@users.sourceforge.net,yshurik@linuxfan.com,sequitur@easystreet.com>
                           (C) 2001-2003 Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QUANTADOC_H
#define QUANTADOC_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// include files for QT
#include <qptrlist.h>
#include <qdict.h>
#include <qobject.h>
#include <qstring.h>

#include <kurl.h>

// forward declaration of the Quanta classes
class Document;
class KConfig;

class QuantaDoc : public QObject
{
  Q_OBJECT

public:

  QuantaDoc(QWidget *parent, const char *name=0);
  ~QuantaDoc();

  bool newDocument(const KURL&, bool switchToExisting = true);
  void openDocument(const KURL&, const QString& a_encoding = QString::null, bool switchToExisting = true);
  bool saveDocument(const KURL&);
  void closeDocument();

  void prevDocument();
  void nextDocument();

  bool isModified();
  bool isModifiedAll();
  void setModified(bool flag = true);
  /** Check if url is opened or not. */
  Document* isOpened(const KURL& url);

  // "save modified" - asks the user
  // for saving if the document is modified
  bool saveModified();
  bool saveAll(bool dont_ask=true);

  // for kwrites
  void readConfig(KConfig *);
  void writeConfig(KConfig *);

  KURL::List openedFiles(bool noUntitled=true);
  void changeFileTabName(const KURL& newURL = KURL());

private:
  Document *write() const;
  Document *newWrite();

public slots:
  /** close documents. */
  void closeAll();
  /** show popup menu with list of attributes for current tag */
  void slotInsertAttrib( int id );
  void slotAttribPopup();

  void undoHistory();
  void invertSelect();
  /** Called when a file on the disk has changed. */
  void slotFileDirty(const QString& fileName);

signals:
  void newStatus();
  void title(QString);

private:
  KPopupMenu *attribMenu;
};

#endif // QUANTADOC_H
