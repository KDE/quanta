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
class KWrite;
class Document;

class KConfig;


class QuantaDoc : public QObject
{
  Q_OBJECT

  friend class QuantaApp;
  friend class QuantaView;

  public:
    
    QuantaDoc( QuantaApp *app, QWidget *parent, const char *name=0);
    ~QuantaDoc();
    
    void openDocument (const KURL&);
    void saveDocument (const KURL&);
    void closeDocument();    
        
    void prevDocument();
    void nextDocument();
    
    KURL    url();
		QString basePath();
    
    bool isModified();
    bool isModifiedAll();
    void setModified(bool flag = true);
    
    /// "save modified" - asks the user 
    /// for saving if the document is modified
    bool saveModified();	
    bool saveAll(bool dont_ask=true);

    Document *write();
		
		// for kwrites
		void  readConfig( KConfig * );
		void writeConfig( KConfig * );
		
		QStringList openedFiles(bool noUntitled=true);
		
  private:
    bool newDocument (const KURL&);
    void changeFileTabName(QString oldUrl, QString newUrl = QString::null );
    Document *newWrite(QWidget *parent);
	
  public slots:
    /** close documents. */
    void closeAll();
    
    void finishLoadURL(KWrite *);
    void finishSaveURL(KWrite *);

    /** show popup menu with list of attributes for current tag */
    void slotInsertAttrib      ( int id );
    void slotInsertCoreAttrib  ( int id );
    void slotInsertEventsAttrib( int id );
    void slotAttribPopup();
    
    void editorOptions();
    void highlightings();
    
    void cut();
    void copy();
    void paste();
    void undo();
    void redo();
    void undoHistory();
    void selectAll();
    void deselectAll();
    void find();
    void findAgain();
    void replace();
    void invertSelect();
    void verticalSelect();
    void indent();
    void unindent();
    void cleanIndent();
    void gotoLine();

    void setHl (int);
    void setEol(int);
 	
  signals:
    void newStatus();
    void title(QString);

  private:
  	
  	QuantaApp *app;
    /** list with documents( kwrites ) */
    QDict<Document> *docList;
    /** manager for bookmarks */
    //KWriteManager *writeManager;
/*
    KPopupMenu *attribMenu;
    QPopupMenu *attribCoreMenu;
    QPopupMenu *attribEventsMenu;
*/    
};

#endif // QUANTADOC_H
