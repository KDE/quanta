/***************************************************************************
                          messageoutput.h  -  description
                             -------------------
    begin                : Thu Feb 24 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavski
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

#include <qwidget.h>
#include <keditcl.h>


/**class for displaing of
weblint output
  *@author Yacovlev Alexander & Dmitry Poplavski
  */

class MessageOutput : public KEdit  {
   Q_OBJECT
public: 
	MessageOutput(QWidget *parent=0, const char *name=0);
	~MessageOutput();
	
public slots:	
	void insertAtEnd(QString s);
	
protected:
  virtual void mouseReleaseEvent(QMouseEvent* event);
  virtual void keyPressEvent ( QKeyEvent* event);

  signals:
  /** emited, if the mouse was clicked over the widget*/
  void clicked();
  void keyPressed(int key);

};

#endif
