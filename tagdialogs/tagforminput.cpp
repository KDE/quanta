/***************************************************************************
                          tagforminput.cpp  -  description
                             -------------------
    begin                : Sun Apr 2 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavsky
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
#include "tagforminput.h"
#include "../qextfileinfo.h"

#include <kapp.h>
#include <kiconloader.h>
#include <kfiledialog.h>


TagFormInput::TagFormInput(QWidget *parent, const char *name)
  : TagWidget(parent,name)
{
	initDialog();
	
	buttonSrc ->setPixmap( UserIcon("open") );
	
	connect( buttonSrc, SIGNAL(clicked()), this, SLOT(slotSrcSelect()) );
	connect( comboType, SIGNAL(activated(int)), this, SLOT(slotTypeChange(int)) );
	
}

TagFormInput::~TagFormInput(){
}


void TagFormInput::readAttributes( QDict<char> *d )
{
  dict = d;

  updateDict("type",      comboType );
  updateDict("name",      lineName );
  updateDict("value",     lineValue);
  updateDict("size",      lineSize);
  updateDict("maxlength", lineMaxLen);
  updateDict("src",       lineSrc);
  updateDict("alt",       lineAlt);
  updateDict("align",     comboAlign);
  updateDict("checked",   checkCheck);
  updateDict("disabled",  checkDisabled);
  updateDict("readonly",  checkReadOnly);
  updateDict("onfocus",   lineOnFocus);
  updateDict("onblur",    lineOnBlur);
  updateDict("onselect",  lineOnSelect);
  updateDict("onchange",  lineOnChange);
}

void TagFormInput::writeAttributes( QDict<char> *d )
{
  dict = d;
  char *t;

  if (( t=d->find("type") ))      setValue(t,  comboType);
  if (( t=d->find("name") ))      setValue(t,  lineName);
  if (( t=d->find("value") ))     setValue(t,  lineValue);
  if (( t=d->find("size") ))      setValue(t,  lineSize);
  if (( t=d->find("maxlength") )) setValue(t,  lineMaxLen);
  if (( t=d->find("src") ))       setValue(t,  lineSrc);
  if (( t=d->find("alt") ))       setValue(t,  lineAlt);
  if (( t=d->find("align") ))     setValue(t,  comboAlign);
  if (( t=d->find("onfocus") ))   setValue(t,  lineOnFocus);
  if (( t=d->find("onblur") ))    setValue(t,  lineOnBlur);
  if (( t=d->find("onselect") ))  setValue(t,  lineOnSelect);
  if (( t=d->find("onchange") ))  setValue(t,  lineOnChange);

  checkCheck    -> setChecked( d->find("checked") != 0 );
  checkDisabled -> setChecked( d->find("disabled") != 0 );
  checkReadOnly -> setChecked( d->find("readonly") != 0 );


  int type = comboType->currentItem();
  slotTypeChange( type );
}

void TagFormInput::slotSrcSelect()
{
		QString fileName = KFileDialog::getOpenFileName( basePath, "*|All files");
		if (fileName.isEmpty()) return;
		
		QExtFileInfo file(fileName);
		file.convertToRelative( basePath );
		QString shortName = file.filePath();
		lineSrc->setText(shortName);
}

/**  */
void TagFormInput::slotTypeChange(int i)
{
    bool attrTable[10][7] = {
        {1,1,1,0,0,0,1},      // text
        {1,1,1,0,0,0,1},      // password
        {0,0,0,0,0,1,0},      // checkbox
        {0,0,0,0,0,1,0},      // radio
        {0,0,0,0,0,0,0},      // submit
        {0,0,0,0,0,0,0},      // reset
        {0,0,1,0,0,0,1},      // file
        {0,0,0,0,0,0,0},      // hidden
        {1,0,1,1,1,0,0},      // image
        {0,0,0,0,0,1,0}};     // button
// size, maxlen, src, alt, align , checked, readonly
    lineSize   -> setEnabled( attrTable[i][0] );
    lineMaxLen -> setEnabled( attrTable[i][1] );
    lineSrc    -> setEnabled( attrTable[i][2] );
    buttonSrc  -> setEnabled( attrTable[i][2] );
    lineAlt    -> setEnabled( attrTable[i][3] );
    comboAlign -> setEnabled( attrTable[i][4] );
    checkCheck -> setEnabled( attrTable[i][5] );
    checkReadOnly -> setEnabled( attrTable[i][6] );
}

