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

#include <QString>
#include <QHash>
#include <QList>

#include "areastruct.h"
#include "qtag.h"
#include "tagattr.h"
#include "quantaexport.h"

class QDomElement;

/**
  This is the a tag inside the document. It contains only the attributes and values as
 they are typed. It is used to build up the internal represantation of the document in
 a structure tree (build up from Node objects, which have pointers to Tag-s). A Tag is
 not necessary a valid and known DTD tag with valid attributes and values. It can be
 the parsed version of EXACTLY <foo foo_attr="foo"> in any XML like DTD.

  *@author Andras Mantia
  */



class LIBQUANTA_EXPORT Tag {
public:
  Tag();
  Tag(const Tag&);
  Tag(const AreaStruct &area, const QString &areaString, const DTDStruct *dtd = 0L, bool doParse = false);
  ~Tag();
  Tag operator = ( const Tag& );

  /** For Kafka copy/paste */
//   void save(QDomElement& element) const;
//   bool load(QDomElement const& element);

  /** Parse the p_tagStr and build up the tag's attributes and values */
  void parse (const QString &p_tagStr);

  /** Return the attribute at index*/
  QString attribute(int index) const {return (index < attrs.count()) ? attrs[index].name : "";};

  /** Return the attribute value at index*/
  QString attributeValue(int index) const {return (index < attrs.count()) ? attrs[index].value : "";};

  /** Return the value of attr*/
  QString attributeValue(const QString &attr, bool ignoreCase = false) const;

  /** Add an attribute */
  void addAttribute(TagAttr attr) {attrs.append(attr);}

  /** Get the attribute number index */
  TagAttr getAttribute(int index) const {return attrs[index];}

  /** Remove the attribute number index */
  void deleteAttribute(int index) {attrs.removeAt(index);}

  /** Insert a new Attribute, even if it already exists. Prefer using editAttribute.
    Return true if something was mdofied. */
  bool editAttribute(const QString& attrName, const QString& attrValue);

  /** Delete the attribute attrName */
  void deleteAttribute(const QString& attrName);

  /** Returns the quotation status of the attribute */
  bool isQuotedAttribute(int index) const {return attrs[index].quoted;}

  /** Check if this tag has the attr attribute defined */
  bool hasAttribute(const QString &attr, bool ignoreCase = false) const;

  /** set the coordinates of a tag attribute */
  void setAttributePosition(int index, int bLineName, int bColName, int bLineValue, int bColValue);

  /** Set the coordinates of tag inside the document */
  void setTagPosition(int bLine, int bCol, int eLine, int eCol) {m_area.set(bLine, bCol, eLine, eCol);};

  /** Set the coordinates of tag inside the document, but using an AreaStruct as argument */
  void setTagPosition(const AreaStruct &area) {m_area = area;};

  /** Where the tag begins in the document */
  void beginPos(int &bLine, int &bCol) const {bLine = m_area.bLine(); bCol = m_area.bCol();}

  /** Where the tag begins in the document */
  QPoint beginPos() const {return m_area.start;}

  /** Where the tag ends in the document */
  void endPos(int &eLine, int &eCol) const {eLine = m_area.eLine(); eCol = m_area.eCol();}

  /** Where the tag ends in the document */
  QPoint endPos() const {return m_area.end;}

  /** Return the tag area */
  AreaStruct area() const {return m_area;}

  /** Where the attr at index begins in the document */
  void attributeNamePos(int index, int &line, int &col) const;

  /** Where the attr value at index begins in the document */
  void attributeValuePos(int index, int &line, int &col) const;

  /** Set the internal string which is parsed */
  void setStr(const QString &p_tagStr);

  /** Get the tag in string format */
  QString tagStr() const {return m_tagStr;};

  /**
   * @param line
   * @param col
   * @return the index of attribute at (line, col) or -1 if not found
   */
  int attributeIndexAtPos(int line, int col) const;

  /**
   * @param line
   * @param col
   * @return the index of attributevalue at (line, col) or -1 if not found
   */
  int valueIndexAtPos(int line, int col) const;

  /**
   * @param attr
   * @return the index of attr or -1 if not found
   */
  int attributeIndex(const QString &attr) const;

//   void namePos(int &line, int &col);
  int attrCount() const {return attrs.count();}

  /** modify the attributes of tag according to the attrDict*/
  void modifyAttributes(QHash<QString, QString*> *attrDict);

  /** returns the tag as a string */
  QString toString() const;

  /** returns true if the tag is a closing tag */
  bool isClosingTag() const {return m_closing;} ;

  /** Sets the special flag of attribute at index*/
  void setAttributeSpecial(int index, bool special);

#ifdef DEBUG_PARSER
  int size() const;
#endif

  const DTDStruct* dtd() const {return m_dtd;}

  void setDtd(const DTDStruct *dtd) {m_dtd = dtd;};

  enum TokenType {
    Unknown = 0,
    XmlTag, //1 Represent a Tag e.g. <tag>, <tag />
    XmlTagEnd, //2 Represenjt a closing tag e.g. <tag/>
    Text, //3 Represent a portion of text. There can't be two adjacent Texts.
    Comment, //4 Represent a XML comment : "<!--", "-->" is a XmlTagEnd (not sure, please correct).
    CSS, //5
    ScriptTag, //6 Represent a Script e.g. "<?php", "?>" is a XmlTagEnd (not sure, please correct).
    ScriptStructureBegin, //7
    ScriptStructureEnd, //8
    LocalVariable, //9
    GlobalVariable, //10
    NeedsParsing = 500,
    Empty, //501
    Skip = 1000 }; // types of token

  bool isType(Tag::TokenType type) const {return type == m_type;};
  void setType(Tag::TokenType type) {m_type = type;};

  Tag::TokenType type() const {return m_type;};

  void setName(const QString & name) {m_name = name;};
  QString name(bool caseSensitive = true) const {return caseSensitive ? m_name : m_name.toUpper();};

  QString nameSpace(bool caseSensitive = true) const {return caseSensitive ? m_nameSpace : m_nameSpace.toUpper();};

  QString nameAndNS(bool showClosing = true) const
  {
    QString s = m_nameSpace.isEmpty() ? m_name : m_nameSpace + ":" + m_name;
    return (showClosing && m_closing) ? s.prepend('/') : s;
  }

  //TODO: write setting/retrieving methods for the below attributes, and add
  //them the m_ prefix
  QString cleanStr;
  bool single; // tags like <tag />
  bool closingMissing; //closing tag is optional and missing
  QString structBeginStr; //if it's a special block, contains the block beginning definition string (like <? or <style language="foo">)
  bool validXMLTag; //false if the closing ">" was not found

  bool cleanStrBuilt() {return m_cleanStrBuilt;}
  void setCleanStrBuilt(bool cleanStrBuilt) {m_cleanStrBuilt = cleanStrBuilt;}
  bool indentationDone() {return m_indentationDone;}
  void setIndentationDone(bool indentationDone) {m_indentationDone = indentationDone;}
  bool notInTree() {return m_notInTree;}
  void setNotInTree(bool notInTree) {m_notInTree = notInTree;}

  /**Returns true if this is the closing part of a Tag. It's namespace aware.*/
  bool doesClose(const Tag *tag) const;

  /**Returns true if tag2 is the closing pair of tag1. It's namespace aware.*/
//   static bool closesTag(QString namespaceName, QString tagName, bool caseSensitive,
//                         QString namespaceName2, QString tagName2, bool caseSensitive2);

  /** convert tag to upper or lower case */
  static QString convertCase(const QString& tag);


private:
  Tag::TokenType m_type;   //one of the TokenType
  QString m_name;
  QString m_nameSpace;

  //specifies if we need to build the clean tag string from the attrs
  // or the text without entities. This "clean" string will be inserted in the source view.
  // if true, the markup is already generated.
  // if false, it is not, we need to generate it.
  bool m_cleanStrBuilt;

  //Specify if the indentation has been applied to this Node : added spaces to text and empty Nodes,
  // added empty Node around for other Node.
  bool m_indentationDone;

  // specifies if this tag is just conencted to a DOM::Node but isn't part of the Node tree.
  bool m_notInTree;


  void init();

  /** Verifies if the last char from @param str is inside a script area or not */
  bool isInsideScript(const QString& str);

  AreaStruct m_area; //where the tag is in the doc
//   int m_nameLine;//where the tag name begins
//   int m_nameCol;
  const DTDStruct* m_dtd; //the tag belongs to this DTD

  QList<TagAttr> attrs;  //attributes in a tag
  QString m_tagStr;   //the tag in string format (as it is in the document)
  bool m_closing; // true is this is a closing tag
};


#endif

// kate: space-indent on; indent-width 2; mixedindent off; indent-mode cstyle;
