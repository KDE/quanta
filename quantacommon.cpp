/***************************************************************************
                          quantacommon.cpp  -  description
                             -------------------
    begin                : Sat Jul 27 2002
    copyright            : (C) 2002 by Andras Mantia
    email                : amantia@freemail.hu
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
//qt includes
#include <qstring.h>
#include <qdict.h>
#include <qdir.h>

//kde includes
#include <kurl.h>
#include <kdirwatch.h>

#include "quantacommon.h"

#include "quanta.h"
#include "quantadoc.h"
#include "document.h"
#include "project/project.h"

uint tagsCase;
uint attrsCase;
QString attrsQuotation;
bool closeOptionalTags;
bool closeTags;
bool useAutoCompletion;
uint refreshFrequency;
QString globalDataDir;
QString defaultDocType;
QuantaApp *QuantaCommon::quantaApp;

QDict<AttributeList> *tagsDict;
QDict<DTDStruct> *dtds; //holds all the known tags with attributes for each DTD.
QValueList<QString> pluginsList; //holds the names of available plugins

Node *baseNode;
Parser *parser;
KDirWatch *fileWatcher;

QString defaultEncoding;

QuantaCommon::QuantaCommon(){
}

QuantaCommon::~QuantaCommon(){
}

/** convert tag to upper or lower case */
QString QuantaCommon::tagCase( const QString tag)
{
  QString sTag = tag;

  switch (tagsCase)
  {
    case 1: sTag = tag.lower();
            break;
    case 2: sTag = tag.upper();
  }
  return sTag;
}

/** convert attr of tag to upper or lower case */
QString QuantaCommon::attrCase( const QString attr)
{
  QString sAttr = attr;

  switch (attrsCase)
  {
    case 1: sAttr = attr.lower();
    case 2: sAttr = attr.upper();
  }
  return sAttr;
}

/** Set's up the url correctly from urlString. */
void QuantaCommon::setUrl(KURL &url, QString urlString)
{
 if (urlString.at(0) == '/')
 {
   url.setPath(urlString);
 }
 else
 {
   url = urlString;
 }
}
/** No descriptions */
bool QuantaCommon::isSingleTag(QString dtdName, QString tag)
{
  bool single = false;

  DTDStruct* dtd = dtds->find(dtdName);
  if (dtd)
  {
    QString searchForTag = (dtd->caseSensitive) ? tag : tag.upper();
    QTag* qtag = dtd->tagsList->find(searchForTag);
    if (qtag)
        single = qtag->isSingle();
  }

  return single;
}

/** No descriptions */
bool QuantaCommon::isOptionalTag(QString dtdName, QString tag)
{
  bool optional = false;

  DTDStruct* dtd = dtds->find(dtdName);
  if (dtd)
  {
    QString searchForTag = (dtd->caseSensitive) ? tag : tag.upper();
    QTag* qtag = dtd->tagsList->find(searchForTag);
    if (qtag)
        optional = qtag->isOptional();
  }

  return optional;
}
/** No descriptions */
bool QuantaCommon::isKnownTag(QString dtdName, QString tag)
{
  bool known = false;

  DTDStruct* dtd = dtds->find(dtdName);
  if (dtd)
  {
    QString searchForTag = (dtd->caseSensitive) ? tag : tag.upper();
    if (dtd->tagsList->find(searchForTag))
        known = true;
  }

  return known;
}

AttributeList*  QuantaCommon::tagAttributes(QString dtdName, QString tag)
{
  AttributeList* attrs = 0L;

  DTDStruct* dtd = dtds->find(dtdName);
  if (dtd)
  {
    QString searchForTag = (dtd->caseSensitive) ? tag : tag.upper();
    QTag* qtag = dtd->tagsList->find(searchForTag);
    if (qtag)
        attrs = qtag->attributes();
  }

  return attrs;
}
/** Returns the QTag object for the tag "tag" from the DTD named "dtdname". */
QTag* QuantaCommon::tagFromDTD(QString dtdName, QString tag)
{
  DTDStruct* dtd = dtds->find(dtdName);
  return tagFromDTD(dtd, tag);
}
/** Returns the QTag object for the tag "tag" from the DTD. */
QTag* QuantaCommon::tagFromDTD(DTDStruct *dtd, QString tag)
{
  QTag *qtag = 0;
  if (dtd)
  {
    QString searchForTag = (dtd->caseSensitive) ? tag : tag.upper();
    qtag = dtd->tagsList->find(searchForTag);
  }

 return qtag;
}

/** Returns an XML style string containing the GUI for attributes. */
QString QuantaCommon::xmlFromAttributes(AttributeList* attributes)
{
 QString xmlStr;
 QTextStream stream( &xmlStr, IO_WriteOnly );
 if (attributes)
 {
  int row = 0;
  for ( uint i = 0; i< attributes->count();i++)
  {
    Attribute *attribute = attributes->at(i);
    QString name = attribute->name.left(1).upper()+attribute->name.right(attribute->name.length()-1);

    stream << "  <attr name=\"" + attribute->name +"\" type=\""+attribute->type+"\"";
    if (!attribute->defaultValue.isEmpty())
            stream  << " defaultValue=\"" + attribute->defaultValue + "\"";
    if (!attribute->status.isEmpty())
            stream  << " status=\"" + attribute->status + "\"";

    stream << ">" << endl;
    stream << "    <text>" << name << "</text>" << endl;

    if (attribute->type != "check")
    {
      stream  << "    <textlocation col=\"0\" row=\"" << row << "\" />" << endl;
    }
    stream  << "    <location col=\"1\" row=\"" << row << "\" />" << endl;

    if (attribute->type == "list")
    {
      stream << "    <items>" << endl;
      for (uint j = 0; j < attribute->values.count(); j++)
      {
        stream << "      <item>" << attribute->values[j] << "</item>" << endl;
      }
      stream << "    </items>" << endl;
    }
    stream  << "  </attr>" << endl << endl ;
    row++;
  } //for
 } //if

 return xmlStr;
}

/** Returns list of values for attribute */
QStringList* QuantaCommon::tagAttributeValues(QString dtdName, QString tag, QString attribute)
{
  QStringList *values = 0L;
  
  AttributeList* attrs = tagAttributes(dtdName, tag); 
  Attribute *attr;
  if (attrs)
  {
    for ( attr = attrs->first(); attr; attr = attrs->next() )
    {
      if (attr->name == attribute)
      {
        if (attr->type == "url") {
          Project *project = quantaApp->getProject();
          if (project->hasProject()) {
            values = new QStringList(project->fileNameList(true));
            values->append("mailto:" + project->email);
          } else {
            QDir dir = QDir(quantaApp->getDoc()->write()->url().directory());
            values = new QStringList(dir.entryList());            
          }
          break;
        } else {
          values = &attr->values;
          break;
        }
      }
    }
  }
  
  return values;
}


/** Returns the DTD name (identifier) corresponding to the DTD's nickname */
QString QuantaCommon::getDTDNameFromNickName(QString nickName)
{
  QString name = nickName;
  QDictIterator<DTDStruct> it(*dtds);
  for( ; it.current(); ++it )
  {
    if (it.current()->nickName == nickName)
    {
     name = it.current()->name;
     break;
    }
  }

 return name;
}

/** Returns the DTD iddentifier from the given nickname */
QString QuantaCommon::getDTDNickNameFromName(QString name)
{
  QString nickName = name;
  QDictIterator<DTDStruct> it(*dtds);
  for( ; it.current(); ++it )
  {
    if (it.current()->name == name)
    {
      nickName = it.current()->nickName;
      break;
    }
  }

  return nickName;
}

  /** Returns 0 if the (line,col) is inside the area specified by the other arguments,
      -1 if it is before the area and 1 if it is after. */
int QuantaCommon::isBetween(int line, int col, int bLine, int bCol, int eLine, int eCol)
{
  int pos = 0;
  if (line < bLine || (line == bLine && (col < bCol) )) pos = -1; //it is before
  if (line > eLine || (line == eLine && (col > eCol) )) pos = 1;  //it is after
/*  if ( ( line > bLine && line < eLine ) ||
       ( line == bLine && line < eLine && col >= bCol) ||
       ( line > bLine && line == eLine && col <= eCol) ||
        ( line == bLine && line == eLine && col >=bCol && col <=eCol) )
 {
   result = 0;  //between
 }*/
 return pos;
}


/** Read property of QDict<bool> pluginsAvailable. */
bool QuantaCommon::pluginAvailable(const QString& name)
{
	bool result = false;
  for (uint i=0; i < pluginsList.count(); i++)
  {
    if (pluginsList[i] == name) result = true;
  }

  return result;
}
