/***************************************************************************
                          selectablearea.h  -  description
                             -------------------
    begin                : mer mar 5 2003
    copyright            : (C) 2003 by gulmini luciano
    email                : gulmini.luciano@student.unife.it
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef SELECTABLEAREA_H
#define SELECTABLEAREA_H

#include <qwidget.h>
#include <qtextbrowser.h>

/**a QTextBrowser that can be selected
  *@author gulmini luciano
  */

class SelectableArea : public QTextBrowser  {
   Q_OBJECT
    public :
	SelectableArea(QWidget *parent=0, const char *name=0);
	~SelectableArea();
	QString getIdLabel() const { return idLabel; }
        void setIdLabel(QString i) { idLabel = i; }
    protected :
	    void mousePressEvent( QMouseEvent *);
            void focusOutEvent ( QFocusEvent * ); 
            void resizeEvent ( QResizeEvent * );
    private :
            QString idLabel;
    signals :	
	   void selected(QString);
           void Resized(QRect);
};

#endif
