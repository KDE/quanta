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
#include <qmap.h>
class QStringList;

class fontFamilyChooser : public fontFamilyChooserS
{
  Q_OBJECT
  private:
    enum FontOrigin { available, generic };
    QString m_currentSelectedFont;
    FontOrigin m_fontOrigin;
    int  m_currentSelectedFontIndex;
    QMap<QString,FontOrigin> m_selectedFontMap;
  
  private slots:
    void updatePreview(const QString &);
    void setCurrentSelectedAvailableFamilyFont(const QString&); 
    void setCurrentSelectedGenericFamilyFont(const QString&);
    void setCurrentSelectedFont( int i) { m_currentSelectedFontIndex = i; }
    void setCurrentSelectedFont( const QString& f ) { m_currentSelectedFont = f; }
    void addFont();
    void removeFont();
    void moveFontUp();
    void moveFontDown();
    
  public:    
    fontFamilyChooser(QWidget* parent, const char *name=0);
    ~fontFamilyChooser(); 
    QStringList fontList();
    void setInitialValue(const QString& s);
};

#endif
