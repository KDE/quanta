/***************************************************************************
                          quantadoc.h  -  description
                             -------------------
    begin                : Втр Май  9 13:29:57 EEST 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
    email                : pdima@users.sourceforge.net,yshurik@linuxfan.com,sequitur@easystreet.com
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
#include <qlist.h>
#include <qdict.h>
#include <qobject.h>
#include <qstring.h>

#include <kurl.h>

// forward declaration of the Quanta classes
class QuantaApp;
class QuantaView;
class Document;

class KConfig;
class KDirWatch;

class QuantaDoc : public QObject
{
  Q_OBJECT

friend class QuantaApp;
friend class QuantaView;

public:
    
  QuantaDoc( QWidget *parent, const char *name=0);
  ~QuantaDoc();

  void openDocument (const KURL&, QString encoding = QString::null);
  void saveDocument (const KURL&);
  void closeDocument();

  void prevDocument();
  void nextDocument();

  bool isModified();
  bool isModifiedAll();
  void setModified(bool flag = true);

  /// "save modified" - asks the user
  /// for saving if the document is modified
  bool saveModified();	
  bool saveAll(bool dont_ask=true);

  Document *write();
  Document *newWrite();
		
	// for kwrites
	void  readConfig( KConfig * );
	void writeConfig( KConfig * );
		
	KURL::List openedFiles(bool noUntitled=true);
  /** No descriptions */
  QDict<Document> *docList() {return m_docList;}
  void changeFileTabName(const KURL& oldURL, KURL newURL = KURL() );
		
private:
  bool newDocument (const KURL&);
	
public slots:
  /** close documents. */
  void closeAll();
  /** show popup menu with list of attributes for current tag */
  void slotInsertAttrib      ( int id );
  void slotAttribPopup();

  void undoHistory();
  void invertSelect();
  /** Called when a file on the disk has changed. */
  void slotFileDirty(const QString& fileName);
 	
signals:
  void newStatus();
  void title(QString);

private:
    /** list with documents( kwrites ) */
    QDict<Document> *m_docList;

    KPopupMenu *attribMenu;

};

#endif // QUANTADOC_H
