/***************************************************************************
                          tag.cpp  -  description
                             -------------------
    begin                : Sun Sep 1 2002
    copyright            : (C) 2002, 2003 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/

#include <ctype.h>

#include <qdict.h>
#include <qstring.h>
#include <qcstring.h>
#include <qdom.h>

#include <kdebug.h>

#include "tag.h"
#include "document.h"
#include "quantacommon.h"
#include "resource.h"

#include "parser.h"
#include "node.h"


void TagAttr::save(QDomElement& element) const
{
    element.setAttribute("name", name);             // QString
    element.setAttribute("value", value);           // QString
    element.setAttribute("nameLine", nameLine);     // int
    element.setAttribute("nameCol", nameCol);       // int
    element.setAttribute("valueLine", valueLine);   // int
    element.setAttribute("valueCol", valueCol);     // int
    element.setAttribute("quoted", quoted);         // bool
    element.setAttribute("special", special);       // bool
}

bool TagAttr::load(QDomElement const& element)
{
    name = element.attribute("name");
    value = element.attribute("value");
    nameLine = QString(element.attribute("nameLine")).toInt();
    nameCol = QString(element.attribute("nameCol")).toInt();
    valueLine = QString(element.attribute("valueLine")).toInt();
    valueCol = QString(element.attribute("valueCol")).toInt();
    quoted = QString(element.attribute("quoted")).toInt();
    special = QString(element.attribute("special")).toInt();

    return true;
}


Tag::Tag()
{
  init();
}

Tag::Tag(const AreaStruct &area, Document *write, const DTDStruct *dtd, bool doParse)
{
  init();
  QString s = write->text(area);
  m_area = area;
  m_dtd = dtd;
  if (doParse)
  {
    parse(s, write);
  } else
  {
    m_write = write;
    m_tagStr = s;
    cleanStr = s;
  }
}

Tag::Tag( const Tag &t)
{
  name = t.name;
  nameSpace = t.nameSpace;
  m_dtd = t.m_dtd;
  single = t.single;
  closingMissing = t.closingMissing;
  m_area = t.m_area;
  m_tagStr = t.m_tagStr;
  cleanStr = t.cleanStr;
  m_write = t.m_write;
  type = t.type;
  structBeginStr = t.structBeginStr;
  m_nameLine = t.m_nameLine;
  m_nameCol = t.m_nameCol;
  attrs = t.attrs;
  validXMLTag = t.validXMLTag;
  m_cleanStrBuilt = t.m_cleanStrBuilt;
  m_indentationDone = t.m_indentationDone;
  m_notInTree = t.m_notInTree;
}

Tag::~Tag()
{
  attrs.clear();
}

void Tag::init()
{
  name = "";
  m_dtd = 0L;
  m_write = 0L;
  type = Unknown;
  single = false;
  closingMissing = false;
  structBeginStr = "";
  cleanStr = "";
  m_nameLine = -1;
  m_nameCol = -1;
  validXMLTag = true;
  m_cleanStrBuilt = true;
  m_indentationDone = true;
  m_notInTree = false;
}

void Tag::save(QDomElement& element) const
{
    element.setAttribute("name", name);                         // QString
    element.setAttribute("nameSpace", nameSpace);               // QString
    element.setAttribute("cleanStr", cleanStr);                 // QString
    element.setAttribute("type", type);                         // int
    element.setAttribute("single", single);                     // bool
    element.setAttribute("closingMissing", closingMissing);     // bool
    element.setAttribute("structBeginStr", structBeginStr);     // QString
    element.setAttribute("validXMLTag", validXMLTag);           // bool
    element.setAttribute("cleanStrBuilt", m_cleanStrBuilt);     // bool
    element.setAttribute("indentationDone", m_indentationDone); // bool
    element.setAttribute("notInTree", m_notInTree);             // bool
    element.setAttribute("nameLine", m_nameLine);               // int
    element.setAttribute("nameCol", m_nameCol);                 // int

    QValueList<TagAttr>::const_iterator it;
    for (it = attrs.begin(); it != attrs.end(); ++it)
    {
        QDomElement child_element = element.ownerDocument().createElement("tagAttr");
        element.appendChild(child_element);
        (*it).save(child_element);
    }

    element.setAttribute("tagStr", m_tagStr);                   // QString
}

bool Tag::load(QDomElement const& element)
{
    name = element.attribute("name");                         // QString
    nameSpace = element.attribute("nameSpace");               // QString
    cleanStr = element.attribute("cleanStr");                 // QString
    type = QString(element.attribute("type")).toInt();                         // int
    single = QString(element.attribute("single")).toInt();                     // bool
    closingMissing = QString(element.attribute("closingMissing")).toInt();     // bool
    structBeginStr = element.attribute("structBeginStr");     // QString
    validXMLTag = QString(element.attribute("validXMLTag")).toInt();           // bool
    m_cleanStrBuilt = QString(element.attribute("cleanStrBuilt")).toInt();     // bool
    m_indentationDone = QString(element.attribute("indentationDone")).toInt(); // bool
    m_notInTree = QString(element.attribute("notInTree")).toInt();             // bool
    m_nameLine = QString(element.attribute("nameLine")).toInt();               // int
    m_nameCol = QString(element.attribute("nameCol")).toInt();                 // int

    QDomNodeList list = element.childNodes();
    for (unsigned int i = 0; i != list.count(); ++i)
    {
        if (list.item(i).isElement())
        {
            QDomElement e = list.item(i).toElement();
            if (e.tagName() == "tagAttr")
            {
                TagAttr tag_attr;
                tag_attr.load(e);
                addAttribute(tag_attr);
            }
        }
    }

    m_tagStr = element.attribute("tagStr");                   // QString

    return true;
}

void Tag::parse(const QString &p_tagStr, Document *p_write)
{
 attrs.clear();
 m_tagStr = p_tagStr;
 uint strLength = m_tagStr.length();
 cleanStr = m_tagStr;
 m_write = p_write;
 if (!m_tagStr.startsWith("<"))
 {
   type = Text;
   return;
 }
 m_nameLine = m_area.bLine;
 m_nameCol = m_area.bCol + 1;
 uint pos = 1;
 while (pos < strLength &&
        !m_tagStr[pos].isSpace() && m_tagStr[pos] != '>' && m_tagStr[pos] != '<' && m_tagStr[pos] != '\n')
 {
   pos++;
 }
 name = m_tagStr.mid(1, pos - 1);
 int nameSpacePos = name.find(':');
 if (nameSpacePos != -1)
 {
   nameSpace = name.left(nameSpacePos);
   name = name.mid(++nameSpacePos);
   m_nameCol += nameSpacePos;
 }
 QString attrStr;
 TagAttr attr;
 attr.special = false; //by default non of the attributes are special
 while (pos < strLength && m_tagStr[pos].isSpace())
        pos++;
 int sPos = pos;
 int valueStartPos = 0;
 while (pos < strLength)
 {
    //find the attribute name
    while (pos < strLength &&
           !m_tagStr[pos].isSpace() && m_tagStr[pos] != '=')
    {
      pos++;
    }
    attr.name = m_tagStr.mid(sPos, pos - sPos);
    if (attr.name.endsWith(">") && pos == strLength)
    {
      attr.name = attr.name.left(attr.name.length() - 1).lower();
      if (!attr.name.stripWhiteSpace().isEmpty())
      {
        attr.nameLine = m_tagStr.left(sPos).contains('\n') + m_area.bLine;
        if (attr.nameLine == m_area.bLine)
            attr.nameCol = sPos + m_area.bCol;
        else
            attr.nameCol = m_tagStr.left(sPos).section('\n',-1).length();
        attr.value = (m_dtd != 0) ? m_dtd->booleanTrue : QString("checked");
        attr.valueCol = attr.nameCol;
        attr.valueLine = attr.nameLine;
        attr.quoted = false;
        attrs.append(attr);
      }
      break;
    }
    if (m_dtd && !m_dtd->caseSensitive)
        attr.name = attr.name.lower();
    attr.nameLine = m_tagStr.left(sPos).contains('\n') + m_area.bLine;
    if (attr.nameLine == m_area.bLine)
        attr.nameCol = sPos + m_area.bCol;
    else
        attr.nameCol = m_tagStr.left(sPos).section('\n',-1).length();

    while (pos < m_tagStr.length() && m_tagStr[pos].isSpace())
            pos++;
    //if the attribute is just listed and there is no value specified,
    //treate it as a "true" boolean
    if (m_tagStr[pos] != '=' || pos == strLength)
    {
      attr.value = (m_dtd != 0) ? m_dtd->booleanTrue : QString("checked");
      attr.valueCol = attr.nameCol;
      attr.valueLine = attr.nameLine;
      attr.quoted = false;
      pos--;
    } else
    {
      pos++;
      while (pos < strLength && m_tagStr[pos].isSpace())
            pos++;
      if (m_tagStr[pos] == '\'' || m_tagStr[pos] == '"')
      {
        attr.quoted = true;
        valueStartPos = pos + 1;
        QChar quotation = m_tagStr[pos];
        pos += 1;
        while (pos < strLength &&
               (m_tagStr[pos] != quotation ||
               (m_tagStr[pos] == quotation &&
                (m_tagStr[pos-1] == '\\'  || isInsideScript(m_tagStr.mid(valueStartPos, pos - valueStartPos)) ) )))
        {
          pos++;
        }
        attr.value = m_tagStr.mid(valueStartPos, pos - valueStartPos);
      } else
      {
        attr.quoted = false;
        valueStartPos = pos;
        while (pos < strLength && !m_tagStr[pos].isSpace())
          pos++;
        if (pos == strLength)
          pos--;
        attr.value = m_tagStr.mid(valueStartPos, pos - valueStartPos);
      }
      attr.valueLine = m_tagStr.left(valueStartPos).contains('\n') + m_area.bLine;
      if (attr.valueLine == m_area.bLine)
          attr.valueCol = valueStartPos + m_area.bCol;
      else
          attr.valueCol = m_tagStr.left(valueStartPos).section('\n',-1).length();
    }

    attrs.append(attr);
    //go to the first non-space char. This is where the next attribute name starts
    pos++;
    while (pos < strLength && m_tagStr[pos].isSpace())
          pos++;
    sPos = pos++;
 }

 //add the tag to the document usertag list if it's not present in the dtd
  if (m_tagStr.startsWith("<") && m_tagStr.endsWith(">") && m_dtd)
  {
    //QString tagName = (m_parsingDTD->caseSensitive) ? name : name.upper();
    QString tagName = name.lower();
    //add the new xml tags to the userTagList
    if ( !QuantaCommon::isKnownTag(m_dtd->name, tagName) &&
          name[0] != '/' )
    {
      QTag *newTag = m_write->userTagList.find(tagName);
      bool insertNew = !newTag;
      if (insertNew)
      {
        newTag = new QTag();
        newTag->setName(name);
        newTag->parentDTD = m_dtd;
      }
      for (int i = 0; i >attrCount(); i++)
      {
        Attribute *attr = new Attribute;
        attr->name = attribute(i);
        attr->values.append(attributeValue(i));
        newTag->addAttribute(attr);
        delete attr;
      }
      if (insertNew)
      {
        m_write->userTagList.replace(tagName, newTag);
      }
    }
  }
}


QString Tag::attribute(int index)
{
  QString attr="";
  if ( index != -1 && index < (int)attrs.count() )
  {
   attr = attrs[index].name;
  }
  return attr;
}

QString Tag::attributeValue(int index)
{
  QString val = "";
  if ( index != -1 && index < (int)attrs.count() )
  {
    val = attrs[index].value;
  }
  return val;
}

QString Tag::attributeValue(const QString &attr, bool ignoreCase)
{
 QString val = "";
 for (uint i = 0 ; i < attrs.count(); i++)
 {

  if ( attr == attrs[i].name ||
       ((!m_dtd->caseSensitive || ignoreCase) && attrs[i].name.lower() == attr.lower()))
  {
    val = attrs[i].value;
    break;
  }
 }
 return val;
}

/** Check if this tag has the attr attribute defined */
bool Tag::hasAttribute(const QString &attr, bool ignoreCase)
{
  for (uint i = 0; i < attrs.count(); i++)
  {
    if ( attrs[i].name ==  attr ||
         ((!m_dtd->caseSensitive || ignoreCase) && attrs[i].name.lower() == attr.lower()))
      return true;
  }
  return false;
}

void Tag::setAttributePosition(int index, int bLineName, int bColName, int bLineValue, int bColValue)
{
  TagAttr attr;
  attr = attrs[index];
  attr.nameLine = bLineName;
  attr.nameCol = bColName;
  attr.valueLine = bLineValue;
  attr.valueCol = bColValue;
  attrs.remove(attrs.at(index));
  //attrs.append(attr);
  attrs.insert(attrs.at(index) ,attr);
}

/** Set the coordinates of tag inside the document */
void Tag::setTagPosition(int bLine, int bCol, int eLine, int eCol)
{
  m_area.bLine = bLine;
  m_area.bCol = bCol;
  m_area.eLine = eLine;
  m_area.eCol = eCol;
}

/** Set the coordinates of tag inside the document, but using an AreaStruct as argument */
void Tag::setTagPosition(const AreaStruct &area)
{
  m_area = area;
}

/** Return the index of attr. */
int Tag::attributeIndex(const QString &attr)
{
  int index = -1;
  uint i = 0;
  do{
    if (attrs[i].name == attr ||
       (!m_dtd->caseSensitive && attrs[i].name == attr.lower()))
       index = i;
    i++;
  } while (index == -1 && i < attrs.count());
  return index;
}
/** Returns the index of attribute at (line,col). */
int Tag::attributeIndexAtPos(int line, int col)
{
 int index = -1;
 uint i = 0;
 do
 {
   if (attrs[i].nameLine == line)
   {
     if (attrs[i].nameCol <= col &&
         (int) (attrs[i].nameCol + attrs[i].name.length()) >=col)
     {
        index = i;
     }
   }
   i++;
 } while (i < attrs.count() && index == -1);
 return index;
}

/** Returns the index of attributevalue at (line,col). */
int Tag::valueIndexAtPos(int line, int col)
{
 int index = -1;
 uint i = 0;
 do
 {
   if  (attrs[i].valueLine == line &&
       (attrs[i].valueLine != attrs[i].nameLine || attrs[i].valueCol != attrs[i].nameCol))
   {
     if (attrs[i].valueCol <= col &&
         (int) (attrs[i].valueCol + attrs[i].value.length()) >=col)
     {
        index = i;
     }
   }
   i++;
 } while (i < attrs.count() && index == -1);
 return index;
}

void Tag::namePos(int &line, int &col)
{
  line = m_nameLine;
  col = m_nameCol;
}

void Tag::setStr(const QString &p_tagStr)
{
  m_tagStr = p_tagStr; cleanStr = m_tagStr;
}

int Tag::size()
{
  int l = sizeof(name) +  8*sizeof(int) + 2*sizeof(bool);
  l += sizeof(cleanStr) + sizeof(m_tagStr);
  l += sizeof(structBeginStr) + sizeof(attrs);
  l += sizeof(m_dtd) + sizeof(m_write);

  return l;
}

void Tag::attributeNamePos(int index, int &line, int &col)
{
 line = -1;
 col = -1;
 if ( index != -1 && index < (int)attrs.count() )
 {
  line = attrs[index].nameLine;
  col = attrs[index].nameCol;
 }
}

void Tag::attributeValuePos(int index, int &line, int &col)
{
 line = -1;
 col = -1;
 if ( index != -1 && index < (int)attrs.count() )
 {
  line = attrs[index].valueLine;
  col = attrs[index].valueCol;
 }
}

bool Tag::editAttribute(const QString& attrName, const QString& attrValue)
{
  TagAttr attr;

  for (uint i = 0 ; i < attrs.count(); i++)
  {
    if ( attrName == attrs[i].name ||
      (!m_dtd->caseSensitive && attrs[i].name.lower() == attrName.lower()))
    {
      if(attr.value == attrValue)
        return false;

      attr = attrs[i];
      attr.value = attrValue;
      attrs.remove(attrs.at(i));
      attrs.append(attr);
      return true;
    }
  }
  //attrName not found, creating the attr, if attrValue not empty
  if(!attrValue.isEmpty())
  {
    attr.name = attrName;
    attr.value = attrValue;
    attr.quoted = true;
    attrs.append(attr);
    return true;
  }

  return false;
}

void Tag::deleteAttribute(const QString& attrName)
{
  for (uint i = 0 ; i < attrs.count(); i++)
  {
    if ( attrName == attrs[i].name ||
      (!m_dtd->caseSensitive && attrs[i].name.lower() == attrName.lower()))
    {
      attrs.remove(attrs.at(i));
    }
  }
}

void Tag::modifyAttributes(QDict<QString> *attrDict)
{
  QTag *qTag = QuantaCommon::tagFromDTD(m_dtd, name);
  QDictIterator<QString> it(*attrDict);
  QString attribute;
  QString value;
  while ( it.current() )
  {
    attribute = it.currentKey();
    value = *(it.current());
    if (qTag && qTag->parentDTD->singleTagStyle == "xml" && attribute=="/")
    {
     ++it;
     continue;
    }
    editAttribute(attribute, value);
    ++it;
  }
  for (uint i = 0 ; i < attrs.count(); i++)
  {
    if ( !attrDict->find(attrs[i].name) )
    {
      attrs.remove(attrs.at(i));
    }
  }
}

QString Tag::toString()
{
  QTag *qTag = QuantaCommon::tagFromDTD(m_dtd, name);
  QValueList<TagAttr>::Iterator it;
  TagAttr attr;
  QString attrString;
  QString tagString;
  for (it = attrs.begin(); it != attrs.end(); ++it)
  {
    attr = *it;
    attrString = " ";
    if (attr.value.isEmpty() || attr.name == "/")
    {
      attrString.append(attr.name);
    } else
    {
      attrString.append(attr.name + "=");
      if (!attr.value.startsWith("\\\"") && !attr.value.startsWith("\\\'"))
          attrString.append(qConfig.attrValueQuotation);
      attrString.append(attr.value);
      if (!attr.value.endsWith("\\\"") && !attr.value.endsWith("\\\'"))
          attrString.append(qConfig.attrValueQuotation);
    }
    tagString.prepend(attrString);
  }
  attrString = "<";
  if (!nameSpace.isEmpty())
    attrString += nameSpace + ":";
  attrString.append(QuantaCommon::tagCase(name));
  tagString.prepend(attrString);

  if (attrs.isEmpty() && qTag && qTag->parentDTD->singleTagStyle == "xml" &&
      (qTag->isSingle() ||
      (!qConfig.closeOptionalTags && qTag->isOptional()) || single)
     )
  {
    tagString.append(" /");
  }
  tagString.append(">");

  return tagString;
}

bool Tag::isClosingTag()
{
  return (name[0] == '/' || nameSpace[0] == '/');
}

void Tag::setAttributeSpecial(int index, bool special)
{
 if ( index != -1 && index < (int)attrs.count() )
 {
   attrs[index].special = special;
 }
}

void Tag::setDtd(const DTDStruct *dtd)
{
  m_dtd = dtd;
}

bool Tag::isInsideScript(const QString &str)
{
  if (!m_dtd)
    return false; //happens when the DTD is not known yet, e.g in Document::findDTDName

  //This detects if the last char from str is inside a script area or not, to
  //treat cases like <a href="<? echo "foo" ?>"> correctly
  //TODO: speed up if you can...
  if (str.find(m_dtd->specialAreaStartRx) != -1)
  {
    QString foundString = m_dtd->specialAreaStartRx.cap();
    if (str.find(m_dtd->specialAreas[foundString]) == -1)
    {
      return true;
    }
  }
  return false;
}
