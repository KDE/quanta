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

//kde includes
#include <kurl.h>

#include "quantacommon.h"
#include "resource.h"


uint tagsCase;
uint attrsCase;
QString attrsQuotation;
bool useCloseTag;
bool useAutoCompletion;
QString globalDataDir;
QString defaultDocType;

QDict<AttributeList> *tagsDict;

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
  QTag *qtag = 0;

  DTDStruct* dtd = dtds->find(dtdName);
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
  AttributeList* attrs = tagAttributes(dtdName, tag); 
  Attribute *attr;
  if (attrs)
  {
    for ( attr = attrs->first(); attr; attr = attrs->next() )
    {
      if (attr->name == attribute)
      {
        return &attr->values;
      }
    }
  }
  return 0L;
}
