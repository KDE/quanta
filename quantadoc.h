/***************************************************************************
                          quantadoc.h  -  description
                             -------------------
    begin                : ��� ���  9 13:29:57 EEST 2000
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
#include <qobject.h>
#include <qstring.h>
#include <qlist.h>
#include <qdict.h>

// forward declaration of the Quanta classes
class QuantaApp;
class QuantaView;
class QPopupMenu;
class KPopupMenu;

class KConfig;
class Document;
class KWriteManager;

/**	QuantaDoc provides a document object for a document-view model.
  *
  * The QuantaDoc class provides a document object that can be used in
  * conjunction with the classes QuantaApp and QuantaView to create a
  * document-view model for standard KDE applications based on
  * KApplication and KTMainWindow. Thereby, the document object is
  * created by the QuantaApp instance and contains the document
  * structure with the according methods for manipulation of the
  * document data by QuantaView objects. Also, QuantaDoc contains the
  * methods for serialization of the document data from and to files.
  *
  * @author Source Framework Automatically Generated by KDevelop, (c) The KDevelop Team. 	
  * @version KDevelop version 0.4 code generation */
class QuantaDoc : public QObject
{
  Q_OBJECT

  friend class QuantaApp;
  friend class QuantaView;

  public:
    /** Constructor for the fileclass of the application */
    QuantaDoc( QuantaApp *app, QWidget *parent, const char *name=0);
    /** Destructor for the fileclass of the application */
    ~QuantaDoc();

    /** adds a view to the document which represents the document
        contents. Usually this is your main view. */
    void addView(QuantaView *view);
    /** removes a view from the list of currently connected views */
    void removeView(QuantaView *view);
    /** sets the modified flag for the document after a modifying
        action on the view connected to the document.*/
    void setModified(bool _m=true);
    /** returns if the document is modified or not. Use this to
        determine if your document needs saving by the user on
        closing.*/
    bool isModified();
    bool isModifiedAll();
    /** "save modified" - asks the user for saving if the document is modified */
    bool saveModified();	
    bool saveAll(bool dont_ask=true);
    /** deletes the document's contents */
    void deleteContents();
    /** initializes the document generally */
    bool newDocument( const char* name = 0L);
    /** closes the acutal document */
    void closeDocument();
    /** loads the document by filename and format and emits the updateViews() signal */
    bool openDocument(const QString &filename, const char *format=0);
    /** saves the document under filename and format.*/	
    bool saveDocument(const QString &filename, const char *format=0);
    /** returns the pathname of the current document file*/
        /** navigation between opened files */
    void nextDocument();
    void prevDocument();

    QString getAbsFilePath();
    /** sets the filename of the document */
    void setTitle(const QString &_t);
    /** returns the title of the document */
    QString getTitle();
    /** returns the kwrite document */
    Document *write();
    /** create new write classa */
		Document *newWrite(QWidget *parent);
		/** base path of document */
		QString basePath();
		/** return bool need repaint preview or not */
		bool needRepaintPreview();
		
		/** change file name in dict and tabbar  from old ones to current */
		void changeFileName(QString oldname);
		
		/** write Config */
		void writeConfig( KConfig * );
		
		/** set RB menu for KWrite */
		void setRBMenu( QPopupMenu * rbMenu ) { this->rbMenu = rbMenu; }

	
  public slots:
    /** calls repaint() on all views connected to the document object
     * and is called by the view by which the document has been
     * changed.  As this view normally repaints itself, it is excluded
     * from the paintEvent.  */
    void slotUpdateAllViews(QuantaView *sender);
    /** close documents. */
    void closeAll();
    /** for exchange signals */
    void slotRequestOpenedFiles();

    /** show popup menu with list of attributes for current tag */
    void slotInsertAttrib( int id );
    void slotInsertCoreAttrib( int id );
    void slotInsertEventsAttrib( int id );
    void slotAttribPopup();
 	
  signals:
  	void openedFiles(QStringList);

  public:	
    /** the list of the views currently connected to the document */
    static QList<QuantaView> *pViewList;	

  private:
  	/** Quanta classes */
  	QuantaApp *app;
  	/** doc's attributes */
    QString title;
    /** list with documents( kwrites ) */
    QDict<Document> *docList;
    /** manager for bookmarks */
    KWriteManager *writeManager;
    QPopupMenu *rbMenu;

    KPopupMenu *attribMenu;
    QPopupMenu *attribCoreMenu;
    QPopupMenu *attribEventsMenu;
};

#endif // QUANTADOC_H
