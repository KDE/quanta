/***************************************************************************
                          tagquickstart.h  -  description
                             -------------------
    begin                : Sat Nov 27 1999
    copyright            : (C) 1999 by Yacovlev Alexander & Dmitry Poplavsky
                           (C) 2002 Andras Mantia
    email                : pdima@mail.univ.kiev.ua, amantia@freemail.hu
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef TAGQUICKSTART_H
#define TAGQUICKSTART_H

#include "tagquickstarts.h"

class KURL;

class TagQuickStart : public TagQuickStartS  {
   Q_OBJECT
public: 
  TagQuickStart( const KURL& a_baseURL, QWidget *parent=0, const char *name=0);
  ~TagQuickStart();

private:
  KURL baseURL;
  
public slots: // Public slots
  
  void slotFileSelect();
  
  void slotBGColor   (const QColor &newColor);
  void slotTextColor (const QColor &newColor);
  void slotLinkColor (const QColor &newColor);
  void slotALinkColor(const QColor &newColor);
  void slotVLinkColor(const QColor &newColor);
  
  void slotColorBG   (const QString &newColor);
  void slotColorText (const QString &newColor);
  void slotColorLink (const QString &newColor);
  void slotColorALink(const QString &newColor);
  void slotColorVLink(const QString &newColor);

};

#endif
