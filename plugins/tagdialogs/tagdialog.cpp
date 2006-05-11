/***************************************************************************
                          tagdialog.cpp  -  description
                             -------------------
    begin                : Sat Apr 1 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavsky <pdima@mail.univ.kiev.ua>
                           (C) 2002-2004 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

//app includes

#include "tagdialog.h"
#include "attributes.h"
#include "dtdstruct.h"
// #include "extfileinfo.h"
#include "qtag.h"
#include "settings.h"
#include "tag.h"
#include "tagpair.h"
#include "tagwidget.h"
#include "tagimgdlg.h"
#include "tagxml.h"


// include qt headers
#include <QDir>
#include <QFileInfo>
#include <QEvent>
#include <QTextStream>

// include kde headers
#include <klocale.h>
#include <kstandarddirs.h>
#include <ktexteditor/view.h>
#include <kconfig.h>
#include <kurl.h>
#include <kstdguiitem.h>
#include <kdebug.h>

TagDialog::TagDialog(QTag* dtdTag, Tag *tag, const KUrl& a_baseURL)
    : Q3TabDialog( 0L, "tagdialog", true)
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

TagDialog::TagDialog(QTag* dtdTag, Tag *tag, const QString& selection, const KUrl& a_baseURL)
    : Q3TabDialog( 0L, "tagdialog", true), m_selection(selection)
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

TagDialog::TagDialog(QTag* dtdTag, const QString& attrs, const KUrl& a_baseURL)
    : Q3TabDialog( 0L, "tagdialog", true)
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

TagDialog::TagDialog(QTag* dtdTag, const QString& selection, const QString& attrs, const KUrl& a_baseURL)
    : Q3TabDialog( 0L, "tagdialog", true), m_selection(selection)
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
  QHashIterator<QString, QString *> it(*dict);
  while (it.hasNext())
  {
    it.next();
    delete it.value();
  }
  delete dict;
}


void TagDialog::init(QTag *a_dtdTag, const KUrl& a_baseURL)
{
  setOkButton(KStdGuiItem::ok().text());
  setCancelButton(KStdGuiItem::cancel().text());

  connect( this, SIGNAL(applyButtonPressed()),  SLOT(slotAccept()) );
  connect( this, SIGNAL(cancelButtonPressed()), SLOT(reject()) );

  if (!a_dtdTag)   //the tag is invalid, let's create a default one
  {
    dtdTag = new QTag();
    dtdTag->setName(i18n("Unknown tag"));
    deleteTag = true;
  } else
  {
    dtdTag = a_dtdTag;
    deleteTag = false;
  }
  dict = new QHash<QString, QString*>;
  m_baseURL = a_baseURL;

  QString caption = i18n("Tag Properties: ");
  caption += dtdTag->name();
  setWindowTitle(caption);
  resize(420, 400);
}
/**  */
void TagDialog::parseTag()
{
  if (dtdTag->name() != i18n("Unknown tag")) //read from the extra tags
  {
    QDomDocument doc;
    if (dtdTag->name().toLower() == "img") //NOTE: HTML specific code!
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
       for ( int i = 0; i < nodeList.count(); i++)
       {
          QDomNode n = nodeList.item(i);
          QString nodeTagName = n.toElement().attribute("name");
          if (!dtdTag->parentDTD->caseSensitive)
              nodeTagName = nodeTagName.toUpper();
          if (nodeTagName == tagName && n.toElement().elementsByTagName("attr").count() > 0) //read a tag
          {
            mainDlg = new Tagxml( n, dtdTag, m_selection, this );
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
      //FIXME docString += QuantaCommon::xmlFromAttributes(dtdTag->attributes());
      docString += "</tag>\n</TAGS>\n";
      doc.setContent(docString);
      QDomNode domNode2 = doc.firstChild().firstChild();
      mainDlg = new Tagxml( domNode2, dtdTag, m_selection, this );
    }

  if ( mainDlg )
  {
    addTab( mainDlg, i18n("Main") );
  }

  KConfig *dtdConfig = new KConfig(dtdTag->parentDTD->fileName);
  dtdConfig->setGroup("General");
  int numOfPages = dtdConfig->readEntry("NumOfPages", 0);
  extraPageList = new QList<Tagxml*>;
  for (int i = 1; i <= numOfPages; i++)
  {
    Tagxml *extraPage = 0L;
    dtdConfig->setGroup(QString("Page%1").arg(i));
    QString title = dtdConfig->readEntry("Title");
    QStringList groupList = dtdConfig->readEntry("Groups", QStringList());
    QDomDocument extraDoc; //build an internal tag XML for the groups
    bool addPage = false;
    QString docString = "<!DOCTYPE TAGS>\n<TAGS>\n";
    docString += QString("<tag name=\"Page%1\">\n").arg(i);
    for (int j = 0; j < groupList.count(); j++)
    {
       groupList[j] = groupList[j].trimmed();
    }
    QDomDocument commonDoc;
    QString commonFileName = QFileInfo(dtdTag->fileName()).path() + "/common.tag";
    if (QFile(commonFileName).exists())
    {
      QFile commonFile(commonFileName);
      commonFile.open(IO_ReadOnly);
      if (doc.setContent(&commonFile))
      {
        int row = 0;
        QDomNodeList nodeList = doc.elementsByTagName("tag");
        for ( int j = 0; j < nodeList.count(); j++)
        {
          QDomNode node = nodeList.item(j);
          QString nodeTagName = node.toElement().attribute("name");
          if (groupList.contains(nodeTagName) && dtdTag->commonGroups.contains(nodeTagName)) //add the attributes of this common tag to a new tab
          {
            QString s;
            QTextStream str(&s, IO_ReadWrite);
            QString s2;
            QTextStream str2(&s2, IO_ReadWrite);
            for ( QDomNode n = node.firstChild(); !n.isNull(); n = n.nextSibling() )
            {
              if (n.nodeName() == "attr")
              {
                QDomElement el = n.toElement();
                str.reset();
                str << "<attr name=\"" << el.attribute("name") << "\" type=\"" << el.attribute("type", "input");
                str << "\" source=\"" << el.attribute("source");
                str << "\" interface=\"" << el.attribute("interface");
                str << "\" method=\"" << el.attribute("method");
                str << "\" arguments=\"" << el.attribute("arguments");
                str << "\">" << endl;
                str << "    <text>" << el.attribute("name") << "</text>" << endl;
                str << "    <textlocation col=\"0\" row=\"" << row << "\" />" << endl;
                str << "    <location col=\"1\" row=\"" << row << "\" />" << endl;
                QDomNodeList childNodes = n.childNodes();
                for ( int k = 0; k < childNodes.count(); k++)
                {
                  QDomNode childNode = childNodes.item(j);
                  childNode.save(str2, 2);
                  str << s2;
                }
                str << "</attr>" << endl;
                row++;
              }

            }
            docString += s;
            addPage = true;
          }
        }
        commonFile.close();
      }
    }

    docString += "</tag>\n</TAGS>\n";
    if (addPage)
    {
      extraDoc.setContent(docString);
      QDomNode domNode = extraDoc.firstChild().firstChild();
      extraPage = new Tagxml( domNode, dtdTag, m_selection, this );
      extraPage->writeAttributes( dict );
      addTab( extraPage, i18n(title.toUtf8()) );
      extraPageList->append(extraPage);
    }
//    delete attrs;
  }
 }
}

/** Insert an attribute to dict*/
void TagDialog::insertAttribute(QString *attr, QString *value)
{
  dict->insert( *attr , value );
}

/** Return the attributes in QDict<QString> format*/
QHash<QString, QString*> * TagDialog::getAttributes()
{
  return dict;
}


/** Return all the attributes in one string*/
QString TagDialog::getAttributeString()
{
  QHashIterator<QString, QString*> it( *dict );

  QString attrStr;

  while (it.hasNext())
  {
    it.next();
    QString attr  = TagAttr::convertCase(it.key());
    QString val   = *(it.value());

    QString attrval = " ";  // attr=value

    if ( val.isEmpty()) {
      attrval += attr; // for checkboxes dont print =""
    }
    else {
        attrval += attr + "=" + Settings::self()->attributeQuotationChar() + val + Settings::self()->attributeQuotationChar();
    }
    attrStr = attrval + attrStr;

  }
  return attrStr;
}

/**Return the value of the attribute specified by attr. */
QString TagDialog::getAttribute(const QString& attr)
{
 QString attrStr = getAttributeString()+" ";
 int pos = attrStr.toUpper().indexOf(attr.toUpper());
 if (pos != -1)
 {
   pos = attrStr.indexOf("=", pos+1) + 1;
   return attrStr.mid(pos, attrStr.indexOf(" ", pos) - pos);
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
    for (int i = 0; i < extraPageList->count(); i++)
    {
      extraPageList->at(i)->readAttributes( dict );
    }
    qDeleteAll(extraPageList->begin(), extraPageList->end());
    delete extraPageList;
  }
  accept();
}

/** return document path */
KUrl TagDialog::baseURL()
{
 return m_baseURL;
}


void TagDialog::parseAttributes(const QString &attrs)
{
  QString t = attrs;

  t = t.trimmed();

  while ( !t.isEmpty() )
  {
    int i = 0;
    while ( !t[i].isSpace() && !t[i].isNull() && t[i] != '=' )  i++;

    QString attr = t.left(i);
    QString *value = new QString();

    t = t.remove(0, i).trimmed();

   if ( t[0] == '=' ) {
      t = t.remove(0, 1).trimmed();

      if ( t[0] == '"' ) {
        i = 1;
        while ( t[i] != '"' && !t[i].isNull() ) i++;
        if ( t[i] == '"' )
          *value = t.mid(1,i-1);
        else
          *value = t.mid(1,i);
        t = t.remove(0, i+1).trimmed();
      }
      else
      if ( t[0] == '\'' ) {
        i = 1;
        while ( t[i] != '\'' && !t[i].isNull() ) i++;
        if ( t[i] == '\'' )
          *value = t.mid(1, i-1);
        else
          *value = t.mid(1, i);
        t = t.remove(0, i+1).trimmed();
      }
      else {

        i=0;
        while ( !t[i].isSpace() && !t[i].isNull() )  i++;

        *value = t.left(i);
        t = t.remove(0, i).trimmed();
      }

      // debug ( name+" , "+attr[attrcount]+"="+value[attrcount]+";" );




   }
   // qDebug("attr :%s; value :%s;",attr.data(),value->data() );
   dict->insert( attr , value );
 }

}

/** Insert the new tag into the Document*/
TagPair TagDialog::getNewlyCreatedTag()
{
  TagPair tagPair;

  if (dtdTag->name() == i18n("Unknown tag"))
    return tagPair;

  QString newTag = Tag::convertCase(dtdTag->name()) + getAttributeString();

  if (dtdTag->parentDTD->singleTagStyle == "xml" && (dtdTag->isSingle() || (!Settings::self()->closeOptionalTags() && dtdTag->isOptional())))
  {
    tagPair.setOpening(newTag, true);
    return tagPair;
  }

  tagPair.setOpening(newTag);

  if ( (!dtdTag->isSingle()) &&
       (Settings::self()->closeTags()) &&
       (Settings::self()->closeOptionalTags() || !dtdTag->isOptional()) )
  {
    tagPair.setClosing(Tag::convertCase(dtdTag->name()));
  }
  return tagPair;
}

/*FIXME
Node *TagDialog::buildNode(Document *w)
{
	Node *node;
	QString attrStr("");
	QDictIterator<QString> it( *dict );


	node = kafkaCommon::createNode(dtdTag->name(), "", Tag::XmlTag, w);

	while (it.current())
	{
		node->tag->editAttribute(QuantaCommon::attrCase(it.currentKey()), *it.current());
		++it;
	}

	return node;
}
*/

void TagDialog::showEvent(QShowEvent *ev)
{
  Q3TabDialog::showEvent(ev);
  if (dynamic_cast<Tagxml*>(mainDlg))
     static_cast<Tagxml*>(mainDlg)->focusToFirstItem();
}

#include "tagdialog.moc"
