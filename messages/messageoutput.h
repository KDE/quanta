/***************************************************************************
                          messageoutput.h  -  description
                             -------------------
    begin                : Thu Feb 24 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavski
                           (C) 2002 Andras Mantia <amantia@freemail.hu>
    email                : pdima@mail.univ.kiev.ua
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

/**class for displaing of
weblint output
  *@author Yacovlev Alexander & Dmitry Poplavski
  */

class MessageOutput : public QListBox  {
   Q_OBJECT
public: 
	MessageOutput(QWidget *parent=0, const char *name=0);
	~MessageOutput();
	
public slots:	
  /**  */
	void insertItem( QString s );
	/** insert message for each line of s */
	void showMessage( QString s );
	void addToLastItem( QString s );
	
	/** php3 debugger stuff */
	void phpDebug(QString s);
	void newPhpConnect();
	void endPhpConnect();
	
	/** php4 debugger stuff */
	void php4Debug(QString s);
	
	
	/** weblint stuff */
	void processWebLint( KProcess *, char *, int );
	void weblintFinished();
	
	
public:
  uint maxItems() { return max_items; }	
  void setMaxItems( uint items ) {  max_items = items; }
  /** remove first items , if count > max_items  */
  void checkMaxItems();
  
signals:
  void clicked(QString fname, int line );
  
protected slots:
  void clickItem( QListBoxItem *);  
  
private:
  uint max_items;
};

#endif
