/***************************************************************************
                          qtag.h  -  description
                             -------------------
    begin                : Thu Aug 15 2002
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

#ifndef QTAG_H
#define QTAG_H


/**Quanta style tag (XML tag or code tag), as they are defined in the DTD. Contains
  all the possible attributes and the possible values for the attributes. Do not
  confund with the Tag class, which can change as the user types other attributes and
  changes their values.

  *@author Andras Mantia
  */
//qt includes
#include <qdict.h>
#include <qptrlist.h>
#include <qstringlist.h>

//app includes

//an attribute of a tag looks like:
typedef struct Attribute{
        QString name;
        QString type;         //"input", "check", "list"
        QStringList values;   //the possible values it can have
        QString defaultValue; //the default value
        QString status;       // "optional", "required","implied"
      };

typedef QPtrList<Attribute> AttributeList;
typedef QDict<AttributeList> AttributeListDict;

class QTag;
typedef QDict<QTag> QTagList;

enum DTDFamily{Unknown = 0, Xml, Script};

//an internal representation of a DTD
typedef struct DTDStruct
    {
     QString name;                    //DTD name
     QString nickName;                //DTD nickname
     QString url;                     //the url of the DTD definition file
     QString inheritsTagsFrom;        //Inherited DTD name
     QString defaultExtension;        //default extension when creating new files
     bool caseSensitive;              //the tags&attributes in DTD are case sensitive or not
     int family;                      //xml, script type
     QTagList* tagsList;              //the list of all tags in the DTD
     QString fileName;                //the DTD decription.rc with path
     AttributeListDict* commonAttrs;  //the attributes of the common groups
     QString scriptName;              //the name that can be the value of <script language=""> attribute
     QString scriptRegExpStr;
     QStringList scriptTagStart;
     QStringList scriptTagEnd;
     QString booleanAttributes;       //simple or extended <tag booleanAttr> or <tag booleanAttr="1">
     QString booleanTrue;             //"true" or "1" or whatever
     QString booleanFalse;            //"false" or "0" or whatever
     QString singleTagStyle;          //"xml" or "html" (<tag/> or <tag>)
     QString defaultAttrType;         //"input", "string" or whatever

     QStringList toolbars;
    };

class QTag {
public: 
	QTag();
	QTag(  QTag&);
	~QTag();
	QTag operator = ( QTag& );
  /** Add an attribute to the tag. */
  void addAttribute(Attribute* attr);
  /** Returns the number of attributes for the tag. */
  int attributeCount();
  /** Returns the attribute at index. */
  Attribute* attributeAt(int index);
  AttributeList *attributes() { return &attrs;}
  /** No descriptions */
  void setSingle(bool isSingle);
  /** No descriptions */
  void setOptional(bool isOptional);
  /** No descriptions */
  void setName(const QString& theName);
  /** No descriptions */
  QString name();
  /** No descriptions */
  bool isSingle();
  /** No descriptions */
  bool isOptional();
  /** No descriptions */
  QString fileName();
  /** No descriptions */
  void setFileName(QString& fileName);
  /** Returns the attribute with name, or 0 if the tag does not have any attribute with name. */
  Attribute* attribute(QString& name);

  /** The tag belongs to this DTD */
  DTDStruct *parentDTD;
  /** The tag has the attributes of the above common groups */
  QStringList commonGroups;
  QStringList stoppingTags;
  QString type; //function, class, xmltag, etc.
  QString returnType;  //useful is type="function"; may be int, string or whatever

protected: // Protected attributes
  /** List of the possible attributes */
  AttributeList attrs;
  bool single;
  bool optional;
  QString tagName;
  /** The path to the tag file. Null if there is no tag file for the tag. */
  QString m_fileName;
};


#endif
