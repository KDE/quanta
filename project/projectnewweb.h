/***************************************************************************
                          projectnewweb.h  -  description
                             -------------------
    begin                : Fri Oct 27 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
    email                : pdima@users.sourceforge.net,yshurik@penguinpowered.com,sequitur@easystreet.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef PROJECTNEWWEB_H
#define PROJECTNEWWEB_H

#include <qwidget.h>
#include <qstringlist.h>
#include <projectnewwebs.h>

/**
  *@author Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
  */
  
class KProcess;

class ProjectNewWeb : public ProjectNewWebS  {
   Q_OBJECT
public: 
	ProjectNewWeb(QWidget *parent=0, const char *name=0);
	~ProjectNewWeb();
	
public slots:
  void enableStart(const QString&);
	void setCommandL(const QString&);
	void setBasePath(QString);
	
	void slotStart();
	
	QStringList files();
	
	void slotGetWgetExited(KProcess *);
  void slotGetWgetOutput(KProcess *proc, char *buffer, int buflen);
	
  virtual void resizeEvent( QResizeEvent * );
  
signals:

  void enableMessages();
  void disableMessages();
  
  void messages( QString );
  /** No descriptions */
  void enableNextButton(QWidget *,bool);

private:
  QString basePath;
  
  bool start;
  
  KProcess *proc;
  
  QStringList filesList;
  
};

#endif
