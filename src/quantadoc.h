/***************************************************************************
                          quantadoc.h  -  description
                             -------------------
    begin                : ���� 9 13:29:57 EEST 2000
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

// include files for QT

// forward declaration of the Quanta classes
class Document;
class KConfig;
class KURL;

class QuantaDoc : public QObject
{
  Q_OBJECT

public:

  QuantaDoc(QWidget *parent, const char *name=0);
  ~QuantaDoc();

  bool newDocument(const KURL&, bool switchToExisting = true);
  void openDocument(const KURL&, const QString& a_encoding = QString::null, bool switchToExisting = true, bool readOnly = false);

public slots:
  /** close documents. */
  /** show popup menu with list of attributes for current tag */
  void slotInsertAttrib( int id );
  void slotAttribPopup();

  void slotOpeningCompleted(const KURL &url);
  void slotOpeningFailed(const KURL &url);

signals:
  void newStatus();
  void hideSplash();
  void eventHappened(const QString&, const QString&, const QString& );

private:
  KPopupMenu *attribMenu;
  bool m_eventLoopStarted;
};

#endif // QUANTADOC_H
