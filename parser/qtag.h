/***************************************************************************
                          qtag.h  -  description
                             -------------------
    begin                : Thu Aug 15 2002
    copyright            : (C) 2002, 2003 by Andras Mantia
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
#include <qmap.h>
#include <qptrlist.h>
#include <qstringlist.h>
#include <qregexp.h>

//app includes

class QTag;

//an attribute of a tag looks like:
typedef struct Attribute{
        QString name;
        QString type;         //"input", "check", "list"
        QStringList values;   //the possible values it can have
        QString defaultValue; //the default value
        QString status;       // "optional", "required","implied"
      };

//the groups in structure tree are defined with the help of:
typedef struct StructTreeGroup{
        QString name;        //the name of the group
        QString noName;      //the text when there are no elements in the group
        QString icon;        //the icon of the group
        QRegExp searchRx;    //regular experssion to help us find the group - for pseudo DTDs
        bool hasSearchRx;    //true if searchRx should be used
        QRegExp clearRx;     //clear the text matched from the result of the searchRx search - for pseudo DTDs
        QString tag;         //tags belonging to this group - for real DTDs
        int tagType;     //the tag type for which this is valid
        QStringList attributes; //the attributes of the above tag to be displayed - for real DTDs
        QRegExp autoCompleteAfterRx;  //holds the char after the autocompletion box should be shown for this group elements. Null, if autocompletion shouldn't be used
        QRegExp removeFromAutoCompleteWordRx;
        bool hasFileName;  //the group contains filename(s)
        bool parseFile;   //parse the files belonging to this group
        QRegExp fileNameRx; //delete the matches of this regexp to obtain a filename (eg. linked, included file name)
      };

typedef struct XMLStructGroup {
          QString name;
          QString noName;
          QString icon;
          QStringList attributes;
          bool hasFileName;
          QRegExp fileNameRx;
        };

typedef QPtrList<Attribute> AttributeList;
typedef QDict<AttributeList> AttributeListDict;

typedef QDict<QTag> QTagList;

enum DTDFamily{Unknown = 0, Xml, Script};

#define MAX_STRUCTGROUPSCOUNT 10

//an internal representation of a DTD
typedef struct DTDStruct
    {
     QString name;                    //DTD name
     QString nickName;                //DTD nickname
     QString url;                     //the url of the DTD definition file
     QString doctypeStr;              //the string that appears right after !doctype
     QString inheritsTagsFrom;        //Inherited DTD name
     QString defaultExtension;        //default extension when creating new files
     QStringList mimeTypes;
     bool caseSensitive;              //the tags&attributes in DTD are case sensitive or not
     int family;                      //xml, script type
     bool toplevel;                 //true if the DTD can be the main DTD of a document. Always true for XML like DTD's
     QTagList* tagsList;              //the list of all defined tags in the DTD
     QString fileName;                //the DTD decription.rc with path
     AttributeListDict* commonAttrs;  //the attributes of the common groups

     QString booleanAttributes;       //simple or extended <tag booleanAttr> or <tag booleanAttr="1">
     QString booleanTrue;             //"true" or "1" or whatever
     QString booleanFalse;            //"false" or "0" or whatever
     QString singleTagStyle;          //"xml" or "html" (<tag/> or <tag>)
     QString defaultAttrType;         //"input", "string" or whatever

/****************** FOR THE NEW PARSER **********************/

/* Special, not to be parsed areas. It is the area of the nested DTD's
 (script, css) and special areas like comments. Special areas can be in form:
  <begin_str end_str> or they can be inside special tags, like
  <special_tag> </special_tag>.
*/

/* The starting and closing strings of a special area. For PHP the special areas
   are <? ?> and <* *>, so the entries are (<?,?>),(<*,*>).
*/
     QMap<QString, QString> specialAreas;

/* To which DTD this special area belongs. It may be a non-dtd name, like
   "comment", which is treated as a special case.
   Entries are in for of (<?,php) or (<!--,comment).
*/
     QMap<QString, QString> specialAreaNames;

/* A regular expression which matches the starting strings of all the
   possible special areas.
*/
     QRegExp     specialAreaStartRx;

/* For each special tag name, holds an attribute name. This attribute is used to
   figure out the DTD which is valid in the special tag area.
   E.g for the <script language="php">, the entry is "script"->"language".
   Special tags are skipped during parsing and parsed later according to
   their DTD definition.
*/
     QMap<QString, QString> specialTags;

/* A list of DTDs that can be present inside the DTD.
   For each DTD specified here the definitionAreaBegin/definitionAreaEnd is
   copied to specialAreaBegin/specialAreaEnd (and the specialAreaStartRx is
   updated) and the definitionTags are added to the specialTags.
   Basically this means that we use the DTD definitions when building
   the special area and tag definitions.
*/
     QStringList insideDTDs;

/* The definition tags for this DTD in the same for as the above. */
     QMap<QString, QString> definitionTags;

/* The beginning and end string of the definition areas for this DTD.
   It is stored in (area_begin_str,area_end_str) pairs. E.g (<?,?>)
*/
     QMap<QString, QString> definitionAreas;

/* Start/end pairs for comments. Eg. (//,\n); (<!--,-->)  */
     QMap<QString, QString> comments;

/* Regular experssion to match the start of the comments (//, <!--)*/
     QRegExp commentsStartRx;

/* How does a structure starts in this DTD. Eg. "{" or "begin".*/
     QString structBeginStr;
/* How does a structure ends in this DTD. Eg. "}" or "end".*/
     QString structEndStr;
/* A regular experssion to match the structe begin or end. */
     QRegExp structRx;
/* Regular experssion to match the possible keywords that can appear before
   a structure, like "function", "for", etc. */
     QRegExp structKeywordsRx;
/* Regular expression containing the keywords that indicate that the groups
defined in the structure after the keyword have local scope */
     QRegExp localScopeKeywordsRx;

/* A list of structure tree groups definition */
     QValueList<StructTreeGroup> structTreeGroups;
     QMap<QString, XMLStructGroup> xmlStructTreeGroups;

/****************** END FOR THE NEW PARSER **********************/
     QStringList toolbars;
/*A list with abbreviations in the for of: <template templatename, code> */
     QMap<QString, QString> abbreviations;

/* True if foo-foo2 should be considered as one word. False (default) otherwise. */
     bool minusAllowedInWord;

     QChar tagAutoCompleteAfter;
     QChar attrAutoCompleteAfter;
     QChar attributeSeparator;
     QChar tagSeparator;
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
  /** Returns true if tag is a possible child of this tag, or if
  there are no children defined */
  bool isChild(const QString& tag);

  /** No descriptions */
  QString fileName();
  /** No descriptions */
  void setFileName(const QString& fileName);
  /** Returns the attribute with name, or 0 if the tag does not have any attribute with name. */
  Attribute* attribute(const QString& name);

  /** The tag belongs to this DTD */
  DTDStruct *parentDTD;
  /** The tag has the attributes of the above common groups */
  QStringList commonGroups;
  QStringList stoppingTags;
  QMap<QString, bool> childTags; //list of the possible child tags. If the value is true, the child is mandatory
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
