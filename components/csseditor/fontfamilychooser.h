/***************************************************************************
                          fontfamilychooser.h  -  description
                             -------------------
    begin                : mer lug 23 11:20:17 CEST 2003
    copyright            : (C) |YEAR| by si2003    email                : gulmini.luciano@student.unife.it
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef FONTFAMILYCHOOSER_H
#define FONTFAMILYCHOOSER_H

#include "fontfamilychoosers.h"

class QStringList;

class fontFamilyChooser : public fontFamilyChooserS
{
  Q_OBJECT
  private:
    QString m_currentSelectedFont;
    int  m_currentSelectedFontIndex;
  
  private slots:
    void updatePreview(const QString &);
    void setCurrentSelectedFont( const QString&);
    void setCurrentSelectedFont( int );
    void addFont();
    void removeFont();
    
  public:
    
    fontFamilyChooser(QWidget* parent, const char *name=0);
    ~fontFamilyChooser(); 
    QStringList fontList();

  signals:
    //void selectedFont(const QString&);

};

#endif
