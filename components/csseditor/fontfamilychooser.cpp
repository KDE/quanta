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
#include <qwhatsthis.h>

#include<kdebug.h>

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
  
  if( families.count() != 0 ) lbAvailable->insertStringList(families);
  
       
  QIconSet iconSet = SmallIconSet(QString::fromLatin1("forward"));
  QPixmap pixMap = iconSet.pixmap( QIconSet::Small, QIconSet::Normal );
  pbAdd->setIconSet(iconSet);
  pbAdd->setFixedSize( pixMap.width()+8, pixMap.height()+8 );
  
  iconSet = SmallIconSet(QString::fromLatin1("back"));
  pbRemove->setIconSet(iconSet);
  pbRemove->setFixedSize( pixMap.width()+8, pixMap.height()+8 );
  
   iconSet = SmallIconSet(QString::fromLatin1("up"));
  pbMoveUp->setIconSet(iconSet);
  pbMoveUp->setFixedSize( pixMap.width()+8, pixMap.height()+8 );
  
  iconSet = SmallIconSet(QString::fromLatin1("down"));
  pbMoveDown->setIconSet(iconSet);
  pbMoveDown->setFixedSize( pixMap.width()+8, pixMap.height()+8 );

  connect(pbAdd, SIGNAL(clicked()), this ,SLOT( addFont() ));
  connect( lbAvailable, SIGNAL( highlighted( const QString& ) ), this, SLOT( updatePreview( const QString&) ) );
  connect( lbAvailable, SIGNAL( highlighted( const QString& ) ), this, SLOT( setCurrentSelectedAvailableFamilyFont( const QString&) ) );
  connect( lbGeneric, SIGNAL( highlighted( const QString& ) ), this, SLOT( updatePreview( const QString&) ) );
  connect( lbGeneric, SIGNAL( highlighted( const QString& ) ), this, SLOT( setCurrentSelectedGenericFamilyFont( const QString&) ) );
  connect( lbSelected, SIGNAL( highlighted( const QString& ) ), this, SLOT( updatePreview( const QString&) ) );
  connect( lbSelected, SIGNAL( highlighted( int ) ), this, SLOT( setCurrentSelectedFont( int ) ) );
  connect( lbSelected, SIGNAL( highlighted( const QString& ) ), this, SLOT( setCurrentSelectedFont( const QString&) ) );
  connect( pbRemove, SIGNAL( clicked() ), this, SLOT( removeFont() ) );
  connect( pbMoveUp, SIGNAL( clicked() ), this, SLOT( moveFontUp() ) );
  connect( pbMoveDown, SIGNAL( clicked() ), this, SLOT( moveFontDown() ) );
  
  QWhatsThis::add(lbAvailable,i18n("These are the names of the available fonts on your system"));
  QWhatsThis::add(lbGeneric,i18n("These are the names of the generic fonts "));
  QWhatsThis::add(lbSelected,i18n("These are the names of the generic fonts you have selected  "));
  QWhatsThis::add(pbAdd,i18n("Click this to add a font to your style sheet"));
  QWhatsThis::add(pbRemove,i18n("Click this to remove a font from your style sheet"));
  QWhatsThis::add(pbMoveUp,i18n("Click this to make the font more preferable than the preceeding one"));
  QWhatsThis::add(pbMoveDown,i18n("Click this to make the font less preferable than the following one"));
  
}
 
fontFamilyChooser::~fontFamilyChooser(){} 

void fontFamilyChooser::updatePreview(const QString& s){
  lePreview->setFont(QFont(s,20));
}

void fontFamilyChooser::addFont(){
  lbSelected->insertItem( m_currentSelectedFont );
  switch(m_fontOrigin) {
    case available: lbAvailable->removeItem(lbAvailable->index(lbAvailable->findItem(m_currentSelectedFont)));
                             break;
    case generic : lbGeneric->removeItem(lbGeneric->index(lbGeneric->findItem(m_currentSelectedFont)));
                             break;
  }
}

void fontFamilyChooser::setCurrentSelectedAvailableFamilyFont(const QString& f){
  m_fontOrigin = available;
  m_currentSelectedFont = f;
   m_selectedFontMap[f] = available;
}

void fontFamilyChooser::setCurrentSelectedGenericFamilyFont(const QString& f){
  m_fontOrigin = generic;
  m_currentSelectedFont =f;
  m_selectedFontMap[f] = generic;
}

void  fontFamilyChooser::moveFontUp(){ 
  if(m_currentSelectedFontIndex == 0) return;
  int dummyIndex = m_currentSelectedFontIndex; 
  lbSelected->insertItem( lbSelected->text(m_currentSelectedFontIndex ), dummyIndex -1); 
  lbSelected->removeItem(dummyIndex + 1);
  lbSelected->setSelected( dummyIndex -1, true);                            
}

void  fontFamilyChooser::moveFontDown(){
  if((unsigned int)m_currentSelectedFontIndex == lbSelected->count()) return;
  int dummyIndex = m_currentSelectedFontIndex;
  lbSelected->insertItem( lbSelected->text(m_currentSelectedFontIndex ), dummyIndex + 2);
  lbSelected->removeItem(dummyIndex);
  lbSelected->setSelected(dummyIndex +1, true);
}

void fontFamilyChooser::removeFont(){
  QString dummyFont(m_currentSelectedFont);// since removeItem emits highlighted signal, after
                                                                           //  removeItem call the value of m_currentSelectedFont
                                                                           // is actually the font after m_currentSelectedFont and so
                                                                           // we must save m_currentSelectedFont value in dummyFont
  lbSelected->removeItem( m_currentSelectedFontIndex );
  switch(m_selectedFontMap[dummyFont]) {
    case available: lbAvailable->insertItem(dummyFont);
                             lbAvailable->sort();
                             break;
    case generic : lbGeneric->insertItem(dummyFont);
                            lbGeneric->sort();
                            break;
  }
  
}

QStringList fontFamilyChooser::fontList(){
  QStringList list;
  QListBoxItem *item = lbSelected->firstItem();
  while( item != 0 ){
    if( item->text().contains( QRegExp("\\W") ) ) list.append( "'" + item->text() + "'" );
    else list.append( item->text() );
    item = item->next();  
  }
  return list;
}

void fontFamilyChooser::setInitialValue(const QString& s){
  QStringList familyList = QStringList::split(",",s);                 
  for ( QStringList::Iterator it = familyList.begin(); it != familyList.end(); ++it ) {
    (*it).remove("'");
    (*it).remove("\"");
    lbSelected->insertItem((*it).stripWhiteSpace());
  }  
}

#include "fontfamilychooser.moc"
