/***************************************************************************
                          tagdialog.cpp  -  description
                             -------------------
    begin                : Sat Apr 1 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavsky
                           (C) 2002-2003 by Andras Mantia
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
#include <qevent.h>

// include kde headers
#include <klocale.h>
#include <kstandarddirs.h>
#include <ktexteditor/viewcursorinterface.h>
#include <kconfig.h>
#include <kurl.h>

#include "tagdialog.h"
#include "tagwidget.h"
#include "tagimgdlg.h"
#include "tagxml.h"

#include "../document.h"
#include "../qextfileinfo.h"
#include "../quantacommon.h"
#include "../resource.h"
#include "../parser/qtag.h"

TagDialog::TagDialog(QTag* dtdTag, Tag *tag, KURL a_baseURL)
    : QTabDialog( 0L, "tagdialog", true)
{
  init(dtdTag, a_baseURL);

  m_tag = tag;
  if (m_tag)
  {
    for (int i = 0; i < m_tag->attrCount(); i++)
    {
    dict->insert(m_tag->attribute(i), new QString(m_tag->attributeValue(i)));
    }
  }
  mainDlg = 0L;
  parseTag();

}

TagDialog::TagDialog(QTag* dtdTag, QString attrs, KURL a_baseURL)
    : QTabDialog( 0L, "tagdialog", true)
{
  init(dtdTag, a_baseURL);
  if ( !attrs.isNull() )
  {
    parseAttributes(attrs);
  }
  mainDlg = 0L;
  m_tag = 0L;
  parseTag();
}

TagDialog::~TagDialog()
{
  if (deleteTag)
  {
    delete dtdTag;
  }
  delete dict;
}


void TagDialog::init(QTag *dtdTag, KURL a_baseURL)
{
  setOkButton(i18n("&OK"));
  setCancelButton(i18n("&Cancel"));

  connect( this, SIGNAL(applyButtonPressed()),  SLOT(slotAccept()) );
  connect( this, SIGNAL(cancelButtonPressed()), SLOT(reject()) );

  if (!dtdTag)   //the tag is invalid, let's create a default one
  {
    this->dtdTag = new QTag();
    this->dtdTag->setName(i18n("Unknown tag"));
    deleteTag = true;
  } else
  {
    this->dtdTag = dtdTag;
    deleteTag = false;
  }
  dict = new QDict<QString>(1,false);
  dict->setAutoDelete(true);
  m_baseURL = a_baseURL;

  QString caption = i18n("Tag Properties: ");
  caption += this->dtdTag->name();
  setCaption( caption);
  this->resize(420,400);
}
/**  */
void TagDialog::parseTag()
{
  if (dtdTag->name() != i18n("Unknown tag")) //read from the extra tags
  {
    QDomDocument doc;
    if (dtdTag->name().lower() == "img") //NOTE: HTML specific code!
    {
       mainDlg = new TagImgDlg( this);
      ((TagImgDlg *)mainDlg)->writeAttributes( dict );
    }
    //read the tag file it is available
    else if (QFileInfo(dtdTag->fileName()).exists())
    {
       QFile f( dtdTag->fileName() );
       f.open( IO_ReadOnly );
       if ( doc.setContent( &f ) )
       {
       QString tagName = dtdTag->name();
       QDomNodeList nodeList = doc.elementsByTagName("tag");
       for ( uint i = 0; i < nodeList.count(); i++)
       {
           QDomNode n = nodeList.item(i);
          QString nodeTagName = n.toElement().attribute("name");
          if (!dtdTag->parentDTD->caseSensitive)
              nodeTagName = nodeTagName.upper();
          if (nodeTagName == tagName) //read a tag
          {
            mainDlg = new Tagxml( n, dtdTag->parentDTD, this );
            ((Tagxml    *)mainDlg)->writeAttributes( dict );
            break;
          }
       }
       }
       f.close();
    }
    else
    {
      QString docString = "<!DOCTYPE TAGS>\n<TAGS>\n";
      docString += QString("<tag name=\"%1\">\n").arg(dtdTag->name());
      docString += QuantaCommon::xmlFromAttributes(dtdTag->attributes());
      docString += "</tag>\n</TAGS>\n";
      doc.setContent(docString);
      QDomNode domNode2 = doc.firstChild().firstChild();
      mainDlg = new Tagxml( domNode2, dtdTag->parentDTD, this );
    }

  if ( mainDlg )
  {
    addTab( mainDlg, i18n("Main") );
  }

  KConfig *dtdConfig = new KConfig(dtdTag->parentDTD->fileName);
  dtdConfig->setGroup("General");
  int numOfPages = dtdConfig->readNumEntry("NumOfPages");
  extraPageList = new QPtrList<Tagxml>();
  extraPageList->setAutoDelete(true);
  for (int i = 1; i <= numOfPages; i++)
  {
    Tagxml *extraPage = 0L;
    dtdConfig->setGroup(QString("Page%1").arg(i));
    QString title = dtdConfig->readEntry("Title");
    QStrList groupList;
    dtdConfig->readListEntry("Groups", groupList);
    QDomDocument extraDoc; //build an internal tag XML for the groups
    bool addPage = false;
    QString docString = "<!DOCTYPE TAGS>\n<TAGS>\n";
    docString += QString("<tag name=\"Page%1\">\n").arg(i);
    AttributeList *attrs = new AttributeList;
    for (uint j = 0; j < groupList.count(); j++)
    {
      QString groupName = QString(groupList.at(j)).stripWhiteSpace();
      if (dtdTag->commonGroups.contains(groupName)) //add the attributes of this common tag to a new tab
      {
        AttributeList *groupAttrs = dtdTag->parentDTD->commonAttrs->find(groupName);
        for (uint k = 0; k < groupAttrs->count(); k++)
        {
          attrs->append(groupAttrs->at(k));
        }
        addPage = true;
      }
    }
    docString += QuantaCommon::xmlFromAttributes(attrs);
    docString += "</tag>\n</TAGS>\n";
    if (addPage)
    {
      extraDoc.setContent(docString);
      QDomNode domNode = extraDoc.firstChild().firstChild();
      extraPage = new Tagxml( domNode, dtdTag->parentDTD, this );
      extraPage->writeAttributes( dict );
      addTab( extraPage, i18n(title) );
      extraPageList->append(extraPage);
    }
    delete attrs;
  }
 }
}

/** Insert an attribute to dict*/
void TagDialog::insertAttribute(QString *attr, QString *value)
{
  dict->insert( *attr , value );
}


/** Return the attributes in QDict<QString> format*/
QDict<QString> * TagDialog::getAttributes()
{
  return dict;
}


/** Return all the attributes in one string*/
QString TagDialog::getAttributeString()
{
  QDictIterator<QString> it( *dict );

  QString attrStr("");

  while ( it.current() ) {
    QString attr  = it.currentKey();
    QString val   = *it.current();

    QString attrval = " ";  // attr=value

    if ( !val.isEmpty() ) {
        attrval += QuantaCommon::attrCase(attr) + "=" + qConfig.attrValueQuotation + val + qConfig.attrValueQuotation;
    } else
      attrval += QuantaCommon::attrCase(attr); // for checkboxes dont print =""

    attrStr = attrval+attrStr;

    ++it;
  }
  return attrStr;
}

/**Return the value of the attribute specified by attr. */
QString TagDialog::getAttribute(const QString& attr)
{
 QString attrStr = getAttributeString()+" ";
 int pos = attrStr.upper().find(attr.upper());
 if (pos != -1)
 {
   pos = attrStr.find("=",pos+1) + 1;
   return attrStr.mid(pos, attrStr.find(" ",pos) - pos);
 }
 else
 {
  return QString::null;
 }

}

/**  */
void TagDialog::slotAccept()
{

  if (dtdTag->name() != i18n("Unknown tag"))
  {
    if ( mainDlg )
       ((Tagxml *)mainDlg)->readAttributes( dict );
    for (uint i = 0; i < extraPageList->count(); i++)
    {
      extraPageList->at(i)->readAttributes( dict );
    }
    delete extraPageList;
  }
  accept();
}

/** return document path */
KURL TagDialog::baseURL()
{
 return m_baseURL;
}


void TagDialog::parseAttributes( QString attrs )
{
  QString t = attrs;

  t = t.stripWhiteSpace();

  while ( !t.isEmpty() )
  {
    int i = 0;
    while ( !t[i].isSpace() && !t[i].isNull() && t[i] != '=' )  i++;

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
        while ( !t[i].isSpace() && !t[i].isNull() )  i++;

        *value = t.left(i);
        t = t.remove(0,i).stripWhiteSpace();
      }

      // debug ( name+" , "+attr[attrcount]+"="+value[attrcount]+";" );




   }
   // qDebug("attr :%s; value :%s;",attr.data(),value->data() );
   dict->insert( attr , value );
 }

}

/** Insert the new tag into the Document*/
void TagDialog::insertTag(Document *w, bool insertInLine)
{
  if (dtdTag->name() != i18n("Unknown tag"))
  {
    QString newTag = getAttributeString();
    newTag = QString("<")+QuantaCommon::tagCase(dtdTag->name())+newTag;

    if ( dtdTag->parentDTD->singleTagStyle == "xml" &&
          (dtdTag->isSingle() || (!qConfig.closeOptionalTags && dtdTag->isOptional()))
        )
    {
      newTag.append(" /");
    }
    newTag.append(">");

    QString secondPartOfTag = QString("</")+QuantaCommon::tagCase(dtdTag->name())+">";

    if ( !insertInLine )
    {
      QString space="";
      space.fill( ' ',w->viewCursorIf->cursorColumnReal() );
      newTag += "\n" + space + "  ";
      secondPartOfTag = "\n" + space + secondPartOfTag;
    }

    if ( (!qConfig.closeTags && !dtdTag->isSingle()) ||
          (dtdTag->isSingle()) ||
          (!qConfig.closeOptionalTags && dtdTag->isOptional()) )
    {
      secondPartOfTag = "";
    }

    w->insertTag( newTag, secondPartOfTag);
  }
}

void TagDialog::showEvent(QShowEvent *ev)
{
  QTabDialog::showEvent(ev);
  if (dynamic_cast<Tagxml*>(mainDlg))
     static_cast<Tagxml*>(mainDlg)->focusToFirstItem();
}

#include "tagdialog.moc"
