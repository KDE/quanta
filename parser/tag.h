/***************************************************************************
                          tag.h  -  description
                             -------------------
    begin                : Sun Sep 1 2002
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

#ifndef TAG_H
#define TAG_H

#include <qstring.h>
#include <qdict.h>

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

#define MAX_ATTR_NUM 100

struct TagAttr {
  QString name;           //attr name
  QString value;          //attr value
  int nameLine, nameCol;  //where the attr name begins
  int valueLine, valueCol;//where the attr value begins
  bool quoted;            //quote or not the attribute
};

class Tag {
public: 
	Tag();
	Tag( const Tag&);
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
  /** Check if this tag has the attr attribute defined */
  bool hasAttribute( const QString &attr );
  /** Set the coordinates of tag inside the document */
  void setTagPosition(int bLine, int bCol, int eLine, int eCol);
  /** Where the tag begins in the document */
  void beginPos(int &bLine, int &bCol) {bLine = beginLine; bCol = beginCol;}
  /** Where the tag ends in the document */
  void endPos(int &eLine, int &eCol) {eLine = endLine; eCol = endCol;}
  /** Where the attr at index begins in the document */
  void attributeNamePos(int index, int &line, int &col) {line = attrs[index].nameLine; col = attrs[index].nameCol;}
  /** Where the attr value at index begins in the document */
  void attributeValuePos(int index, int &line, int &col) {line = attrs[index].valueLine; col = attrs[index].valueCol;}
  /** Set the internal string which is parsed */
  void setStr(const QString &p_tagStr) {m_tagStr = p_tagStr;}
  /** Get the tag in string format */
  QString tagStr() {return m_tagStr;};
  /** Get the document where the tag lies */
  Document *write() {return m_write;}
  /** Set the document where the tag lies */
  void setWrite(Document *p_write) {m_write = p_write;}
	
  enum TokenType {Unknown = 0, XmlTag,  XmlTagEnd, Text, Comment, ScriptStructureBegin, ScriptStructureEnd }; // types of token
	QString name;
  int type;   //one of the TokenType
	bool single; // tags like <tag />
  bool closingMissing; //closing tag is optional and missing
  int attrCount;
private:
  int beginLine; //where the tag begins in the doc
  int beginCol;
  int endLine;   //where the tag ends in the doc
  int endCol;
  TagAttr attrs[MAX_ATTR_NUM];  //attributes in a tag
  QString m_tagStr;   //the tag in string format (as it is in the document)
  Document *m_write;  //the document

  /** Parse the attributes in the string and build the attrs vector */
  void parseAttr( QString text, int &line, int &col);
};

#endif
