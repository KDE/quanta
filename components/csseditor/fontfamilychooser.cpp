/***************************************************************************
                          fontfamilychooser.cpp  -  description
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


#include "fontfamilychooser.h"
 
#include <qfontdatabase.h>
#include <qstringlist.h>
#include <qlistbox.h>
#include <qfont.h>
#include <klocale.h>
#include <qiconset.h>
#include <qpixmap.h>
#include <kiconloader.h>
#include <kpushbutton.h>
#include <kglobalsettings.h>
#include <qregexp.h>
#include <qlineedit.h>

fontFamilyChooser::fontFamilyChooser(QWidget* parent, const char *name) : fontFamilyChooserS(parent,name){

  QFont tmpFont( KGlobalSettings::generalFont().family(), 64, QFont::Black );
  lePreview->setMinimumHeight( lePreview->fontMetrics().lineSpacing() );
  lePreview->setAlignment(Qt::AlignCenter);
  QFont font;
  font.setPointSize(20);
  lePreview->setFont(font);
  lePreview->setText(i18n("The Quick Brown Fox Jumps Over The Lazy Dog"));
  
  QFontDatabase fdb;
  QStringList families = fdb.families();
  for ( QStringList::Iterator it = families.begin(); it != families.end(); ++it ) {
    if( (*it).contains('[') !=0  ) 
      it = families.remove(it);
  }
  if( families.count() != 0 )
  {
    lbAvailable->insertStringList(families);
  }
       
  QIconSet iconSet = SmallIconSet(QString::fromLatin1("1rightarrow"));
  QPixmap pixMap = iconSet.pixmap( QIconSet::Small, QIconSet::Normal );
  pbAddAvailable->setIconSet(iconSet);
  pbAddAvailable->setFixedSize( pixMap.width()+8, pixMap.height()+8 );
  pbAddGeneric->setIconSet(iconSet);
  pbAddGeneric->setFixedSize( pixMap.width()+8, pixMap.height()+8 );

  connect(pbAddAvailable, SIGNAL(clicked()), this ,SLOT( addFont() ));
  connect(pbAddGeneric, SIGNAL(clicked()), this ,SLOT( addFont() ));
  connect( lbAvailable, SIGNAL( highlighted( const QString& ) ), this, SLOT( updatePreview( const QString&) ) );
  connect( lbAvailable, SIGNAL( highlighted( const QString& ) ), this, SLOT( setCurrentSelectedFont( const QString&) ) );
  connect( lbGeneric, SIGNAL( highlighted( const QString& ) ), this, SLOT( updatePreview( const QString&) ) );
  connect( lbGeneric, SIGNAL( highlighted( const QString& ) ), this, SLOT( setCurrentSelectedFont( const QString&) ) );
  connect( lbSelected, SIGNAL( highlighted( const QString& ) ), this, SLOT( updatePreview( const QString&) ) );
  connect( lbSelected, SIGNAL( highlighted( int ) ), this, SLOT( setCurrentSelectedFont( int ) ) );
  connect( pbRemove, SIGNAL( clicked() ), this, SLOT( removeFont() ) );
}
 
fontFamilyChooser::~fontFamilyChooser(){} 

void fontFamilyChooser::updatePreview(const QString& s){
  lePreview->setFont(QFont(s,20));
}

void fontFamilyChooser::setCurrentSelectedFont( const QString& s){
  m_currentSelectedFont = s;
}

void fontFamilyChooser::setCurrentSelectedFont( int i){
  m_currentSelectedFontIndex = i;
}

void fontFamilyChooser::addFont(){
  lbSelected->insertItem( m_currentSelectedFont );
}

void fontFamilyChooser::removeFont(){
  lbSelected->removeItem( m_currentSelectedFontIndex );
}

QStringList fontFamilyChooser::fontList(){
  QStringList list;
  QListBoxItem *item = lbSelected->firstItem();
  while( item != 0L ){
    if( item->text().contains( QRegExp("\\W") ) )
      list.append( "'" + item->text() + "'" );
    else 
      list.append( item->text() );
    item = item->next();  
  }
  return list;
}


#include "fontfamilychooser.moc"
