/***************************************************************************
                          tag.h  -  description
                             -------------------
    begin                : Sun Apr 16 2000
    copyright            : (C) 2000 by Dmitry Poplavsky
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

#ifndef TAG_H
#define TAG_H

#include <qstring.h>
#include <qdict.h>

/**
  *@author Dmitry Poplavsky
  */

class Document;

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

  bool hasAttribute( const QString &attr );
  /** No descriptions */
  void setTagPosition(int bLine, int bCol, int eLine, int eCol);
  void getTagBeginPos(int &bLine, int &bCol) {bLine = beginLine; bCol = beginCol;}
  void getTagEndPos(int &eLine, int &eCol) {eLine = endLine; eCol = endCol;}
  void getAttrNamePos(int index, int &line, int &col) {line = attrs[index].nameLine; col = attrs[index].nameCol;}
  void getAttrValuePos(int index, int &line, int &col) {line = attrs[index].valueLine; col = attrs[index].valueCol;}
  void setStr(const QString &p_tagStr) {m_tagStr = p_tagStr;}
  QString tagStr() {return m_tagStr;};
  Document *write() {return m_write;}
  void setWrite(Document *p_write) {m_write = p_write;}
	
	QString name;
  QString type;   //"xmltag", "script", "text"
	bool single; // tags like <tag />
  int attrCount;
private:
  int beginLine; //where the tag begins in the doc
  int beginCol;
  int endLine;   //where the tag ends in the doc
  int endCol;
  TagAttr attrs[50];  //max 50 attributes in a tag
  QString m_tagStr;
  Document *m_write;

  void parseAttr( QString text, int &line, int &col);
};

#endif
