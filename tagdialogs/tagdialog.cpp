/***************************************************************************
                          tagdialog.cpp  -  description
                             -------------------
    begin                : Sat Apr 1 2000
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

// include qt headers
#include <qdir.h>
#include <qfileinfo.h>

// include kde headers
#include <kapp.h>
#include <klocale.h>

#include "../document.h"

#include "tagdialog.h"

#include "corewidgetdlg.h"
#include "eventswidgetdlg.h"

#include "tagwidget.h"

#include "tagadlg.h"
#include "tagbody.h"
#include "tagfontdlg.h"
#include "tagform.h"
#include "tagformbutton.h"
#include "tagformtext.h"
#include "tagformtextareadlg.h"
#include "tagforminput.h"
#include "tagimgdlg.h"
#include "tagtabledlg.h"
#include "tagtablehead.h"
#include "tagtablerow.h"

extern QStrList *tagsList; // list of known tags
extern QStrList *tagsCore; // list of tags, allowed core attributes ( id, class, id, title )
extern QStrList *tagsI18n; // list of tags, allowed i18 attribs.
extern QStrList *tagsScript; // list of tags, allowed script attribs ( onClicl, omMouseOver... )
extern QStrList *quotedAttribs; // list of attribs, that have quoted values ( alt, src ... )
extern QStrList *lCore;          // list of core attributes ( id, class, style ... )
extern QStrList *lI18n;
extern QStrList *lScript;
extern QStrList *singleTags; // tags without end  part </ >
extern QStrList *optionalTags; // tags with optional end part

extern bool tagsCapital; // use capital for tags
extern bool attrCapital; // use capital letters for attributes of tag
extern bool useCloseTag; // use close tag if optional


TagDialog::TagDialog( Document *write, const char *tag ,const char *attr, const char *val)
    : QTabDialog( 0L, tag)
{
  this->write = write;
  dict = new QDict<char>(1,false);

  if ( tag )
  {
    this->tag = tag;
    if ( attr && val ) dict->insert( attr , val );
    fEdit = false; // new tag
  }
  else {
    write->currentTag();
    this->tag = QString(write->getTagAttr(0));
    fEdit = true; // edit tag
  }

  mainDlg = 0L;
  parseTag();

  if ( mainDlg )   mainDlg->  writeAttributes( dict );
  if ( coreDlg )   coreDlg->  writeAttributes( dict );
  if ( eventsDlg ) eventsDlg->writeAttributes( dict );

  QString caption = i18n("Tag properties: ");
  caption += this->tag;
  setCaption( caption);

  this->resize(420,400);

  if ( mainDlg )  addTab( mainDlg,    i18n("Main") );
  if ( coreDlg )  addTab( coreDlg,    i18n("Core & i18n") );
  if ( eventsDlg )addTab( eventsDlg,  i18n("Events") );

  setOkButton();
  setCancelButton();

  connect( this, SIGNAL(applyButtonPressed()),  SLOT(slotAccept()) );
  connect( this, SIGNAL(cancelButtonPressed()), SLOT(reject()) );
}

TagDialog::~TagDialog(){
}

/**  */
void TagDialog::parseTag()
{
  QString t = tag.lower();

  if ( t == "a" )         mainDlg = new TagADlg( this);
  if ( t == "body" )      mainDlg = new TagBody( this);
  if ( t == "font" )      mainDlg = new TagFontDlg( this);
  if ( t == "basefont" )  mainDlg = new TagFontDlg( this);
  if ( t == "form" )      mainDlg = new TagForm( this);
  if ( t == "button" )    mainDlg = new TagFormButton( this);
  if ( t == "input" )     mainDlg = new TagFormInput( this);
  if ( t == "textarea" )  mainDlg = new TagFormTextareaDlg( this);
  if ( t == "img" )       mainDlg = new TagImgDlg( this);
  if ( t == "table" )     mainDlg = new TagTableDlg(this);
  if ( t == "th" )        mainDlg = new TagTableHead(this);
  if ( t == "tr" )        mainDlg = new TagTableRow(this);
  if ( t == "td" )        mainDlg = new TagTableHead(this);

  if ( fEdit ) {
    for (int i=1; i < write->tagAttrNum; i++ )
    {
      QString *attr = new QString(write->getTagAttr(i));
      QString *val = new QString(write->getTagAttrValue(i));

      dict->insert( attr->data() , val->data() );
    }
  }

  t = t.upper();
  coreDlg   = 0L;
  eventsDlg = 0L;

  if ( tagsCore->find(t)!=-1  || tagsI18n->find(t)!=-1 )
  {
    coreDlg   = new CoreWidgetDlg( this);

    if ( tagsCore->find(t) == -1 )
      coreDlg->disableCoreAttribs();
    if ( tagsI18n->find(t) == -1 )
      coreDlg->disableI18nAttribs();
  }

  if ( tagsScript->find(t) != -1 )
    eventsDlg = new EventsWidgetDlg( this);
}
/**  */
void TagDialog::slotAccept()
{

  if ( mainDlg )   mainDlg->readAttributes( dict );
  if ( coreDlg )   coreDlg->readAttributes( dict );
  if ( eventsDlg ) eventsDlg->readAttributes( dict );

  if ( fEdit )
    write->changeCurrentTag( dict );
  else
    formeTag();

  accept();
}

/** form new tag from dict */
void TagDialog::formeTag()
{
  QDictIterator<char> it( *dict );

  QString newTag("");

  while ( it.current() ) {
    QString attr  = it.currentKey();
    QString val   = it.current();

    bool trans;
    val.toInt( &trans );

    QString attrval = " ";  // attr=value

    if ( !val.isEmpty() ) {
      if ( trans )
        attrval += attrCase(attr)+"="+val;
      else
        attrval += attrCase(attr)+"=\""+val+"\"";
    } else
      attrval += attrCase(attr); // for checkboxes dont print =""

    newTag = attrval+newTag;

    ++it;
  }

  newTag = QString("<")+tagCase(tag)+newTag+">";

  QString secondPartOfTag = QString("</")+tagCase(tag)+">";

  if ( ( singleTags->find( tag.upper() )!= -1 ) ||
     ( ( optionalTags->find(tag.upper())!= -1 ) && (!useCloseTag)))
          secondPartOfTag = "";

  write->insertTag( newTag, secondPartOfTag);
}

/** convert tag to upper or lower case */
QString TagDialog::tagCase( const char*  tag)
{
  QString sTag = tag;

  if ( tagsCapital )
    sTag = sTag.upper();
  else
    sTag = sTag.lower();

  return sTag;
}

/** convert attr of tag to upper or lower case */
QString TagDialog::attrCase( const char*  attr)
{
  QString sAttr = attr;

  if ( attrCapital )
    sAttr = sAttr.upper();
  else
    sAttr = sAttr.lower();

  return sAttr;
}

/** return document path */
QString TagDialog::basePath()
{
	if ( write->hasFileName() )
	{
		QString name = write->fileName();
		if ( name.left(5) == "file:" ) name.remove(0,5);
		QFileInfo fileInfo( name );
		return fileInfo.dirPath()+"/";
	}
		
	return QDir::currentDirPath()+"/";
}
