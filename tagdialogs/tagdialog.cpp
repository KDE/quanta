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
#include <kstandarddirs.h>

#include "../document.h"

#include "tagdialog.h"
#include "tagdialog.moc"
#include "corewidgetdlg.h"
#include "eventswidgetdlg.h"

#include "tagwidget.h"
#include "tagimgdlg.h"
#include "tagxml.h"

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


TagDialog::TagDialog( Document *write, QString tag ,QString attr,bool insertInLine )
    : QTabDialog( 0L, tag, true)
{
  this->write = write;
  dict = new QDict<QString>(1,false);
  this->insertInLine = insertInLine;

  if ( tag )
  {
    this->tag = tag;
    if ( !attr.isNull() ) {
      parseAttributes(attr);
    }
    fEdit = false; // new tag
  }
  else {
    write->currentTag();
    this->tag = QString(write->getTagAttr(0));
    fEdit = true; // edit tag
  }

  mainDlg = 0L;
  parseTag();

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

  bool findXMLConfig = false;

  QStringList tagsDirs = KGlobal::instance()->dirs()->findDirs("appdata", "tags");

  for ( QStringList::Iterator it = tagsDirs.begin(); it != tagsDirs.end(); ++it ) {
  	QString tagDir = *it;
  	QDir dir(tagDir, "*.tag");
  	QStringList files = dir.entryList();

  	for ( QStringList::Iterator it_f = files.begin(); it_f != files.end(); ++it_f ) {

  	   if ( *it_f != t+".tag" )
  	   	 continue;

  		 QString fname = tagDir + *it_f ;
  		 QFile f( fname );

			 f.open( IO_ReadOnly );

		   QDomDocument doc;
		   if ( !doc.setContent( &f ) )
		   	 continue;

  		 mainDlg = new Tagxml( doc, this );
  		 findXMLConfig = true;
  		 break;
  	}

  	if ( findXMLConfig )
  		break;
  }


  if ( !findXMLConfig ) {
     if ( t == "img" )       mainDlg = new TagImgDlg( this);
  }

  if ( fEdit ) {
    for (int i=1; i < write->tagAttrNum; i++ )
    {
      QString *attr = new QString(write->getTagAttr(i));
      QString *val = new QString(write->getTagAttrValue(i));

      dict->insert( *attr , val );
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

  if ( !findXMLConfig ) {
  if ( t.lower() == "img" ) ((TagImgDlg *)mainDlg)->writeAttributes( dict ); }
  else 											((Tagxml    *)mainDlg)->writeAttributes( dict );
}
/**  */
void TagDialog::slotAccept()
{

  if ( mainDlg )   ((Tagxml *)mainDlg)->readAttributes( dict );
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
  QDictIterator<QString> it( *dict );

  QString newTag("");

  while ( it.current() ) {
    QString attr  = it.currentKey();
    QString val   = *it.current();

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

  if ( !insertInLine ) {
    QString space="";
  	space.fill( ' ', write->currentColumn() );
    newTag += "\n" + space + "  ";
    secondPartOfTag = "\n" + space + secondPartOfTag;
  }

  if ( ( singleTags->find( tag.upper() )!= -1 ) ||
     ( ( optionalTags->find(tag.upper())!= -1 ) && (!useCloseTag)))
          secondPartOfTag = "";

  write->insertTag( newTag, secondPartOfTag);
}

/** convert tag to upper or lower case */
QString TagDialog::tagCase( QString  tag)
{
  QString sTag = tag;

  if ( tagsCapital )
    sTag = sTag.upper();
  else
    sTag = sTag.lower();

  return sTag;
}

/** convert attr of tag to upper or lower case */
QString TagDialog::attrCase( QString  attr)
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
	if ( !write->isUntitled() )
	{
		QString name = write->url().url();
		if ( name.left(5) == "file:" ) name.remove(0,5);
		QFileInfo fileInfo( name );
		return fileInfo.dirPath()+"/";
	}

	return QDir::currentDirPath()+"/";
}

void TagDialog::parseAttributes( QString attrs )
{
  QString t = attrs;

  t = t.stripWhiteSpace();

  while ( !t.isEmpty() ) {

  	int i = 0;
  	while ( !t[i].isSpace() && !t[i].isNull() && t[i] != '=' )	i++;

  	QString attr = t.left(i);
  	QString *value = new QString();

  	t = t.remove(0,i).stripWhiteSpace();

  	if ( t[0] == '=' ) {
  		t = t.remove(0,1).stripWhiteSpace();

  		if ( t[0] == '"' ) {
  			i = 1;
  			while ( t[i] != '"' && !t[i].isNull() ) i++;
  			if ( t[i] == '"' )
  				*value = t.mid(1,i-1);
  			else
  			  *value = t.mid(1,i);
  			t = t.remove(0,i+1).stripWhiteSpace();
  		}
  		else
  		if ( t[0] == '\'' ) {
  			i = 1;
  			while ( t[i] != '\'' && !t[i].isNull() ) i++;
  			if ( t[i] == '\'' )
  				*value = t.mid(1,i-1);
  			else
  			  *value = t.mid(1,i);
  			t = t.remove(0,i+1).stripWhiteSpace();
  		}
  		else {

    		i=0;
  	  	while ( !t[i].isSpace() && !t[i].isNull() )	i++;

    		*value = t.left(i);
    		t = t.remove(0,i).stripWhiteSpace();
  		}

  		// debug ( name+" , "+attr[attrcount]+"="+value[attrcount]+";" );




   }
   qDebug("attr :%s; value :%s;",attr.data(),value->data() );
   dict->insert( attr , value );
 }

}

