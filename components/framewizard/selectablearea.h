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

//#include <qwidget.h>
#include <khtml_part.h>
#include <khtmlview.h>

/**a QTextBrowser that can be selected
  *@author gulmini luciano
  */


class SelectableArea : public KHTMLPart  {
   Q_OBJECT

   public :
     SelectableArea(QWidget *parent=0, const char *name=0);
     ~SelectableArea();
     QString idLabel() const { return m_idLabel; }
     void setIdLabel(const QString &i) { m_idLabel = i; }
     void setSource(const QString& s) { if(!s.isEmpty()) openURL( KURL(s) ); }

   protected :
     virtual bool eventFilter(QObject*, QEvent*);

   private :
      QString m_idLabel;

    signals :
      void selected(const QString &);
      void Resized(QRect);
};



#endif
