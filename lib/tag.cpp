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

#include "tag.h"
#include "tagattr.h"
#include "node.h"
#include "dtdstruct.h"
#include "settings.h"

// #include <ctype.h>

#include <QString>
#include <qdom.h>

#include <kdebug.h>


Tag::Tag()
{
  init();
}


Tag::Tag(const AreaStruct &area, const QString &areaString, const DTDStruct *dtd, bool doParse)
{
  init();
  m_area = area;
  m_dtd = dtd;
  if (doParse)
  {
    parse(areaString);
  } else
  {
    m_tagStr = areaString;
    cleanStr = areaString;
  }
}


Tag::Tag(const Tag &t)
{
  m_name = t.m_name;
  m_nameSpace = t.m_nameSpace;
  m_dtd = t.m_dtd;
  single = t.single;
  closingMissing = t.closingMissing;
  m_area = t.m_area;
  m_tagStr = t.m_tagStr;
  cleanStr = t.cleanStr;
  m_type = t.m_type;
  structBeginStr = t.structBeginStr;
//   m_nameLine = t.m_nameLine;
//   m_nameCol = t.m_nameCol;
  attrs = t.attrs;
  validXMLTag = t.validXMLTag;
  m_cleanStrBuilt = t.m_cleanStrBuilt;
  m_indentationDone = t.m_indentationDone;
  m_notInTree = t.m_notInTree;
  m_closing = t.isClosingTag();
}

Tag::~Tag()
{
}

void Tag::init()
{
  m_name = "";
  m_nameSpace = "";
  m_dtd = 0L;
  m_type = Tag::Unknown;
  single = false;
  closingMissing = false;
  structBeginStr = "";
  cleanStr = "";
//   m_nameLine = -1;
//   m_nameCol = -1;
  validXMLTag = true;
  m_cleanStrBuilt = true;
  m_indentationDone = true;
  m_notInTree = false;
  m_area = AreaStruct();
  m_closing = false;
}

// void Tag::save(QDomElement& element) const
// {
//     element.setAttribute("name", m_name);                         // QString
//     element.setAttribute("nameSpace", m_nameSpace);               // QString
//     element.setAttribute("cleanStr", cleanStr);                 // QString
//     element.setAttribute("type", m_type);                         // int
//     element.setAttribute("single", single);                     // bool
//     element.setAttribute("closingMissing", closingMissing);     // bool
//     element.setAttribute("structBeginStr", structBeginStr);     // QString
//     element.setAttribute("validXMLTag", validXMLTag);           // bool
//     element.setAttribute("cleanStrBuilt", m_cleanStrBuilt);     // bool
//     element.setAttribute("indentationDone", m_indentationDone); // bool
//     element.setAttribute("notInTree", m_notInTree);             // bool
// //     element.setAttribute("nameLine", m_nameLine);               // int
// //     element.setAttribute("nameCol", m_nameCol);                 // int
//
//     QValueList<TagAttr>::const_iterator end(attrs.end());
//     for (QValueList<TagAttr>::const_iterator it = attrs.begin(); it != end; ++it)
//     {
//         QDomElement child_element = element.ownerDocument().createElement("tagAttr");
//         element.appendChild(child_element);
//         (*it).save(child_element);
//     }
//     element.setAttribute("tagStr", m_tagStr);                   // QString
// }

// bool Tag::load(QDomElement const& element)
// {
//     m_name = element.attribute("name");                         // QString
//     m_nameSpace = element.attribute("nameSpace");               // QString
//     cleanStr = element.attribute("cleanStr");                 // QString
//     m_type = static_cast<TokenType>(QString(element.attribute("type")).toInt()); // int
//     single = QString(element.attribute("single")).toInt();                     // bool
//     closingMissing = QString(element.attribute("closingMissing")).toInt();     // bool
//     structBeginStr = element.attribute("structBeginStr");     // QString
//     validXMLTag = QString(element.attribute("validXMLTag")).toInt();           // bool
//     m_cleanStrBuilt = QString(element.attribute("cleanStrBuilt")).toInt();     // bool
//     m_indentationDone = QString(element.attribute("indentationDone")).toInt(); // bool
//     m_notInTree = QString(element.attribute("notInTree")).toInt();             // bool
// //     m_nameLine = QString(element.attribute("nameLine")).toInt();               // int
// //     m_nameCol = QString(element.attribute("nameCol")).toInt();                 // int
//
//     QDomNodeList list = element.childNodes();
//     uint count = list.count();
//     for (uint i = 0; i != count; ++i)
//     {
//         if (list.item(i).isElement())
//         {
//             QDomElement e = list.item(i).toElement();
//             if (e.tagName() == "tagAttr")
//             {
//                 TagAttr tag_attr;
//                 tag_attr.load(e);
//                 addAttribute(tag_attr);
//             }
//         }
//     }
//
//     m_tagStr = element.attribute("tagStr");                   // QString
//
//     return true;
// }

void Tag::parse(const QString &p_tagStr)
{
  attrs.clear();
  m_tagStr = p_tagStr;
  int strLength = m_tagStr.length();
  cleanStr = m_tagStr;
  if (!m_tagStr.startsWith("<"))
  {
    m_type = Tag::Text;
    return;
  }
//   m_nameLine = m_area.bLine();
//   m_nameCol = m_area.bCol() + 1;
  m_closing = m_tagStr.startsWith("</");
  int pos;
  if (m_closing)
    pos = 2;
  else
    pos = 1;
  while (pos < strLength &&
          !m_tagStr[pos].isSpace() && m_tagStr[pos] != '>' && m_tagStr[pos] != '<' && m_tagStr[pos] != '\n')
  {
    pos++;
  }
  int nameStartPos = m_closing ? 2 : 1;
  m_name = m_tagStr.mid(nameStartPos, pos - nameStartPos);
  int nameSpacePos = m_name.indexOf(':');
  if (nameSpacePos != -1)
  {
    m_nameSpace = m_name.left(nameSpacePos);
    m_name = m_name.mid(++nameSpacePos);
//     m_nameCol += nameSpacePos;
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
      attr.name = attr.name.left(attr.name.length() - 1).toLower();
      if (!attr.name.trimmed().isEmpty())
      {
        attr.nameLine = m_tagStr.left(sPos).count('\n') + m_area.bLine();
        if (attr.nameLine == m_area.bLine())
            attr.nameCol = sPos + m_area.bCol();
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
        attr.name = attr.name.toLower();
    attr.nameLine = m_tagStr.left(sPos).count('\n') + m_area.bLine();
    if (attr.nameLine == m_area.bLine())
        attr.nameCol = sPos + m_area.bCol();
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
      attr.valueLine = m_tagStr.left(valueStartPos).count('\n') + m_area.bLine();
      if (attr.valueLine == m_area.bLine())
          attr.valueCol = valueStartPos + m_area.bCol();
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
}


QString Tag::attributeValue(const QString &attr, bool ignoreCase) const
{
  int count = attrs.count();
  for (int i = 0 ; i < count; i++)
  {
    if ( attr == attrs[i].name ||
       ((!m_dtd->caseSensitive || ignoreCase) && attrs[i].name.toLower() == attr.toLower()))
    {
      return attrs[i].value;
    }
  }
  return "";
}


/** Check if this tag has the attr attribute defined */
bool Tag::hasAttribute(const QString &attr, bool ignoreCase) const
{
  int count = attrs.count();
  for (int i = 0; i < count; i++)
  {
    if ( attrs[i].name ==  attr ||
         ((!m_dtd->caseSensitive || ignoreCase) && attrs[i].name.toLower() == attr.toLower()))
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
  attrs.removeAt(index);
  //attrs.append(attr);
  attrs.insert(index, attr);
}


/** Return the index of attr. */
int Tag::attributeIndex(const QString &attr) const
{
  int count = attrs.count();
  for (int i = 0; i < count; i++)
  {
    if (attrs[i].name == attr ||
       (!m_dtd->caseSensitive && attrs[i].name == attr.toLower()))
       return i;
  }
  return -1;
}


/** Returns the index of attribute at (line,col). */
int Tag::attributeIndexAtPos(int line, int col) const
{
 int index = -1;
 int i = 0;
 do
 {
   if (attrs[i].nameLine == line)
   {
     if (attrs[i].nameCol <= col &&
         (int) (attrs[i].nameCol + attrs[i].name.length()) >= col)
     {
        index = i;
     }
   }
   i++;
 } while (i < attrs.count() && index == -1);
 return index;
}


/** Returns the index of attributevalue at (line,col). */
int Tag::valueIndexAtPos(int line, int col) const
{
 int index = -1;
 int i = 0;
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

// void Tag::namePos(int &line, int &col)
// {
//   line = /*m_nameLine*/;
//   col = m_nameCol;
// }

void Tag::setStr(const QString &p_tagStr)
{
  m_tagStr = p_tagStr; cleanStr = m_tagStr;
}

#ifdef DEBUG_PARSER
int Tag::size() const
{
  int l = sizeof(m_name) + 8 * sizeof(int) + 2 * sizeof(bool);
  l += sizeof(cleanStr) + sizeof(m_tagStr);
  l += sizeof(structBeginStr) + sizeof(attrs);
  l += sizeof(m_dtd);

  return l;
}
#endif


void Tag::attributeNamePos(int index, int &line, int &col) const
{
 line = -1;
 col = -1;
 if ( index != -1 && index < (int)attrs.count() )
 {
  line = attrs[index].nameLine;
  col = attrs[index].nameCol;
 }
}


void Tag::attributeValuePos(int index, int &line, int &col) const
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
  int count = attrs.count();
  for (int i = 0 ; i < count; i++)
  {
    if ( attrName == attrs[i].name ||
      (!m_dtd->caseSensitive && attrs[i].name.toLower() == attrName.toLower()))
    {
      if(attr.value == attrValue)
        return false;

      attr = attrs[i];
      attr.value = attrValue;
      attrs.removeAt(i);
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
  int count = attrs.count();
  for (int i = 0 ; i < count; i++)
  {
    if ( attrName == attrs[i].name ||
      (!m_dtd->caseSensitive && attrs[i].name == attrName.toLower()))
    {
      attrs.removeAt(i);
    }
  }
}


void Tag::modifyAttributes(QHash<QString, QString*> *attrDict)
{
  QTag *qTag = QTag::tagFromDTD(m_dtd, m_name);
  QString attribute;
  QString value;
  QHashIterator<QString, QString*> it(*attrDict);
  it.next();
  while (it.hasNext())
  {
    attribute = it.key();
    value = *(it.value());
    if (qTag && qTag->parentDTD->singleTagStyle == "xml" && attribute=="/")
    {
     it.next();
     continue;
    }
    editAttribute(attribute, value);
    it.next();
  }
  int count = attrs.count();
  for (int i = 0 ; i < count; i++)
  {
    if ( !attrDict->contains(attrs[i].name) )
    {
      attrs.removeAt(i);
    }
  }
}


QString Tag::toString() const
{
  QTag *qTag = QTag::tagFromDTD(m_dtd, m_name);
  TagAttr attr;
  QString attrString;
  QString tagString;
  QList<TagAttr>::ConstIterator end(attrs.constEnd());
  for (QList<TagAttr>::ConstIterator it = attrs.constBegin(); it != end; ++it)
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
        attrString.append(Settings::self()->attributeQuotationChar());
      attrString.append(attr.value);
      if (!attr.value.endsWith("\\\"") && !attr.value.endsWith("\\\'"))
        attrString.append(Settings::self()->attributeQuotationChar());
    }
    tagString.prepend(attrString);
  }
  attrString = m_closing ? "<" : "</";
  if (!m_nameSpace.isEmpty())
    attrString += m_nameSpace + ":";
  attrString.append(convertCase(m_name));
  tagString.prepend(attrString);

  if (attrs.isEmpty() && qTag && qTag->parentDTD->singleTagStyle == "xml" &&
      (qTag->isSingle() ||
      (!Settings::self()->closeOptionalTags() && qTag->isOptional()) || single)
     )
  {
    tagString.append(" /");
  }
  tagString.append(">");

  return tagString;
}


void Tag::setAttributeSpecial(int index, bool special)
{
 if ( index != -1 && index < (int)attrs.count() )
 {
   attrs[index].special = special;
 }
}


bool Tag::isInsideScript(const QString &str)
{
  if (!m_dtd)
    return false; //happens when the DTD is not known yet, e.g in Document::findDTDName

  //This detects if the last char from str is inside a script area or not, to
  //treat cases like <a href="<? echo "foo" ?>"> correctly
  //TODO: speed up if you can...
  if (str.indexOf(m_dtd->specialAreaStartRx) != -1)
  {
    QString foundString = m_dtd->specialAreaStartRx.cap();
    if (str.indexOf(m_dtd->specialAreas[foundString]) == -1)
    {
      return true;
    }
  }
  return false;
}


bool Tag::doesClose(const Tag *openTag) const
{
  if (!m_closing || (nameSpace(dtd()->caseSensitive) != openTag->nameSpace(dtd()->caseSensitive)))
    return false;

  return (name(dtd()->caseSensitive) == openTag->name(dtd()->caseSensitive));
}

// bool Tag::closesTag(QString namespaceName, QString tagName, bool caseSensitive,
//                     QString namespaceName2, QString tagName2, bool caseSensitive2)
// {
//     QString tag1Name, tag2Name;
//     if(namespaceName.isEmpty())
//     {
//       if(!namespaceName.isEmpty())
//         return false;//namespace missmatch
//
//       tag1Name = caseSensitive ? tagName : tagName.toUpper();
//       tag2Name = caseSensitive2 ? tagName2 : tagName2.toUpper();
//       if("/" + tag1Name != tag2Name)
//         return false;//not the closing tag
//     }
//     else
//     {
//       if(namespaceName2.isEmpty())
//         return false;//namespace missmatch
//
//       tag1Name = caseSensitive ? (namespaceName + tagName) : (namespaceName.toUpper() + tagName.toUpper());
//       tag2Name = caseSensitive2 ? (namespaceName2 + tagName2) : (namespaceName2.toUpper() + tagName2.toUpper());
//       if("/" + tag1Name != tag2Name)
//         return false; //namespace missmatch or not the closing tag
//     }
//     return true;
// }


/** convert tag to upper or lower case */
QString Tag::convertCase(const QString& tag)
{
  switch (Settings::self()->tagCase())
  {
    case 1: return tag.toLower();
    case 2: return tag.toUpper();
  }
  return tag;
}


// kate: space-indent on; indent-width 2; mixedindent off; indent-mode cstyle;
