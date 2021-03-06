/***************************************************************************
                          tag.h  -  description
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

#ifndef TAG_H
#define TAG_H

#include <qstring.h>
#include <qdict.h>
#include <qvaluelist.h>

#include "qtag.h"

/**
  This is the a tag inside the document. It contains only the attributes and values as
 they are typed. It is used to build up the internal represantation of the document in
 a structure tree (build up from Node objects, which have pointers to Tag-s). A Tag is
 not necessary a valid and known DTD tag with valid attributes and values. It can be
 the parsed version of EXACTLY <foo foo_attr="foo"> in any XML like DTD.

  *@author Andras Mantia
  */

/**
  Declare Kate::Document before ::Document.
  Otherwise --enable-final could break by #including this file
  before <kate/document.h>.
  */
namespace Kate {
  class Document;
}

class Document;

struct TagAttr {
  TagAttr() {nameLine = nameCol = valueLine = valueCol = 0; quoted = true; special = false;}
  QString name;           //attr name
  QString value;          //attr value
  int nameLine, nameCol;  //where the attr name begins
  int valueLine, valueCol;//where the attr value begins
  bool quoted;            //quote or not the attribute
  bool special;           //true if the attribute is not a real one, instead it's only part 
                          //of a special area that's present in the tag. Example: 
                          //Tag: <a <? echo $a ?> href="x">, <?, echo, $a and ?> are special attributes
                          //This is important, otherwise they would be wrongly treated as booleans.
};

struct AreaStruct{
   AreaStruct() {bLine = bCol = eLine = eCol = -1;}
   AreaStruct(int bl, int bc, int el, int ec) {bLine = bl; bCol = bc; eLine = el; eCol = ec;}
   int bLine;
   int bCol;
   int eLine;
   int eCol;
};


class Tag {
public:
  Tag();
  Tag(const Tag&);
  Tag(const AreaStruct &area, Document *write, DTDStruct *dtd = 0L, bool doParse = false);
  ~Tag();
  Tag operator = ( const Tag& );

  /** Parse the p_tagStr in p_write and build up the tag's attributes and values */
  void parse (const QString &p_tagStr, Document *p_write);
  /** Return the attribute at index*/
  QString attribute(int index);
  /** Return the attribute value at index*/
  QString attributeValue(int index);
  /** Return the value of attr*/
  QString attributeValue(QString attr);
  /** Add an attribute */
  void addAttribute(TagAttr attr) {attrs.append(attr);}
  /** Get the attribute number index */
  TagAttr getAttribute(uint index) const {return attrs[index];}
  /** Remove the attribute number index */
  void deleteAttribute(uint index) {attrs.remove(attrs.at(index));}
  /** Insert a new Attribute, even if it already exists. Prefer using editAttribute */
  void editAttribute(const QString& attrName, const QString& attrValue);
  /** Delete the attribute attrName */
  void deleteAttribute(const QString& attrName);
  /** Returns the quotation status of the attribute */
  bool isQuotedAttribute(int index) const {return attrs[index].quoted;}
  /** Check if this tag has the attr attribute defined */
  bool hasAttribute( const QString &attr );
  /** set the coordinates of a tag attribute */
  void setAttributePosition(int index, int bLineName, int bColName, int bLineValue, int bColValue);
  /** Set the coordinates of tag inside the document */
  void setTagPosition(int bLine, int bCol, int eLine, int eCol);
  /** Set the coordinates of tag inside the document, but using an AreaStruct as argument */
  void setTagPosition(const AreaStruct &area);
  /** Where the tag begins in the document */
  void beginPos(int &bLine, int &bCol) const {bLine = m_area.bLine; bCol = m_area.bCol;}
  /** Where the tag ends in the document */
  void endPos(int &eLine, int &eCol) const {eLine = m_area.eLine; eCol = m_area.eCol;}
  /** Return the tag area */
  AreaStruct area() const {return m_area;}
  /** Where the attr at index begins in the document */
  void attributeNamePos(int index, int &line, int &col);
  /** Where the attr value at index begins in the document */
  void attributeValuePos(int index, int &line, int &col);
  /** Set the internal string which is parsed */
  void setStr(const QString &p_tagStr);
  /** Get the tag in string format */
  QString tagStr() const {return m_tagStr;};
  /** Get the document where the tag lies */
  Document *write() {return m_write;}
  /** Set the document where the tag lies */
  void setWrite(Document *p_write) {m_write = p_write;}
  /** Returns the index of attribute at (line,col). */
  int attributeIndexAtPos(int line, int col);
  /** Returns the index of attributevalue at (line,col). */
  int valueIndexAtPos(int line, int col);
  /** Return the index of attr. */
  int attributeIndex(QString attr);

  void namePos(int &line, int &col);
  int attrCount() const {return attrs.count();}
  /** modify the attributes of tag according to the attrDict*/
  void modifyAttributes(QDict<QString> *attrDict);
  /** returns the tag as a string */
  QString toString();
  /** returns true if the tag is a closing tag (name or namespace starts with "/") */
  bool isClosingTag();
  /** Sets the special flag of attribute at index*/
  void setAttributeSpecial(int index, bool special);

  int size();

  enum TokenType {
    Unknown = 0,
    XmlTag,
    XmlTagEnd,
    Text,
    Comment,
    CSS,
    ScriptTag,
    ScriptStructureBegin,
    ScriptStructureEnd,
    LocalVariable,
    GlobalVariable,
    NeedsParsing = 500,
    Empty,
    Skip = 1000 }; // types of token

  //TODO: write setting/retrieving methods for the below attributes, and add
  //them the m_ prefix
  QString name;
  QString nameSpace;
  QString cleanStr;
  int type;   //one of the TokenType
  bool single; // tags like <tag />
  bool closingMissing; //closing tag is optional and missing
  DTDStruct* dtd; //the tag belongs to this DTD
  QString structBeginStr; //if it's a special block, contains the block beginning definition string (like <? or <style language="foo">)
  bool validXMLTag; //false if the closing ">" was not found
#ifdef BUILD_KAFKAPART
  //specifies if we need to build the clean tag string from the attrs
  // or the text without entities. This is the most important bool of kafka!
  // if true, the markup is already generated.
  // if false, it is not, we need to generate it.
  bool cleanStrBuilt;
  //specifies if this tag are just here to provide a DOM::Node <-> Node correspondance
  // but are not in the Node Tree.
  bool notInTree;
#endif

private:
  void init();

  AreaStruct m_area; //where the tag is in the doc
  int m_nameLine;//where the tag name begins
  int m_nameCol;

  QValueList<TagAttr> attrs;  //attributes in a tag
  QString m_tagStr;   //the tag in string format (as it is in the document)
  Document *m_write;  //the document
};


#endif
