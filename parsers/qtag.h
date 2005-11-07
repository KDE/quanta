/***************************************************************************
                          qtag.h  -  description
                             -------------------
    begin                : Thu Aug 15 2002
    copyright            : (C) 2002, 2003 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
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
class Node;

//an attribute of a tag looks like:
typedef struct Attribute{
        QString name;
        QString type;         //"input", "check", "list"
        QStringList values;   //the possible values it can have
        QString defaultValue; //the default value
        QString status;       // "optional", "required","implied"
        QString source;
        QString interface;
        QString method;
        QString arguments;
      };

class XMLStructGroup {
  public:
    QString name;   ///<the name of the group
    QString noName; ///<the text when there are no elements in the group
    QString icon;   ///<the icon of the group
    QStringList attributes; ///<the attributes of the above tag to be displayed
    bool hasFileName;     ///<the group contains filename(s)
    QRegExp fileNameRx;   ///<delete the matches of this regexp to obtain a filename (eg. linked, included file name)
    bool appendToTags; ///<true if the group elements must be used as normal tags in autocompletion
    QString parentGroup; ///<if the group element can be a child of another group (eg. member function of a class), holds the parent name. Makes sense only if appentToTags is true
};


//the groups in structure tree are defined with the help of:
class StructTreeGroup:public XMLStructGroup {
  public:
    QRegExp definitionRx; //regular expression to help us find the group element definition - for pseudo DTDs
    QRegExp usageRx; //regexp to find the usage of a group element in the document
    bool hasDefinitionRx;    //true if searchRx should be used
    bool isMinimalDefinitionRx; // true if the searchRx should be non-greedy
    QRegExp typeRx;   //regular expression to help us find the group element type from the definition string - for pseudo DTDs
    int tagType;     //the tag type for which this is valid
    QRegExp autoCompleteAfterRx;  //holds the char after the autocompletion box should be shown for this group elements. Null, if autocompletion shouldn't be used
    QRegExp removeFromAutoCompleteWordRx;
    bool parseFile;   //parse the files belonging to this group
};


typedef QPtrList<Attribute> AttributeList;
typedef QDict<AttributeList> AttributeListDict;

typedef QDict<QTag> QTagList;

enum DTDFamily{Unknown = 0, Xml, Script};

#define MAX_STRUCTGROUPSCOUNT 10

//an internal representation of a DTD
typedef struct DTDStruct
    {
     QString name;                    ///< DTD name
     QString nickName;                ///< DTD nickname
     bool loaded;                     ///< true = DTD is complet in memory
     QString url;                     ///< the url of the DTD definition file
     QString doctypeStr;              ///< the string that appears right after !doctype
     QString inheritsTagsFrom;        ///< Inherited DTD name
     QString defaultExtension;        ///< default extension when creating new files
     QStringList mimeTypes;
     bool caseSensitive;              ///< the tags&attributes in DTD are case sensitive or not
     int family;                      ///< xml, script type
     bool toplevel;                   ///< true if the DTD can be the main DTD of a document. Always true for XML like DTD's
     QString documentation;           ///< the name of the documentation package 
     QTagList* tagsList;              ///< the list of all defined tags in the DTD
     QString fileName;                ///< the DTD decription.rc with path
     AttributeListDict* commonAttrs;  ///< the attributes of the common groups

     QString booleanAttributes;       ///< simple or extended <tag booleanAttr> or <tag booleanAttr="1">
     QString booleanTrue;             ///< "true" or "1" or whatever
     QString booleanFalse;            ///< "false" or "0" or whatever
     QString singleTagStyle;          ///< "xml" or "html" (<tag/> or <tag>)
     QString defaultAttrType;         ///< "input", "string" or whatever

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
     mutable QRegExp     specialAreaStartRx;

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

/* Regular expression to match the start of the comments (//, <!--)*/
     mutable QRegExp commentsStartRx;

/* How does a structure starts in this DTD. Eg. "{" or "begin".*/
     QString structBeginStr;
/* How does a structure ends in this DTD. Eg. "}" or "end".*/
     QString structEndStr;
/* A regular expression to match the structe begin or end. */
     mutable QRegExp structRx;
/* Regular expression to match the possible keywords that can appear before
   a structure, like "function", "for", etc. */
     mutable QRegExp structKeywordsRx;
/* Regular expression containing the keywords that indicate that the groups
defined in the structure after the keyword have local scope */
     mutable QRegExp localScopeKeywordsRx;

/* A list of structure tree groups definition */
     mutable QValueList<StructTreeGroup> structTreeGroups;
     QMap<QString, XMLStructGroup> xmlStructTreeGroups;

/****************** END FOR THE NEW PARSER **********************/
     QStringList toolbars;

/* True if foo-foo2 should be considered as one word. False (default) otherwise. */
     bool minusAllowedInWord;

     mutable QChar tagAutoCompleteAfter;
     bool requestSpaceBeforeTagAutoCompletion;
     QChar attrAutoCompleteAfter;
     QChar attributeSeparator;
     QChar tagSeparator;

     /* Script language related items */
     int variableGroupIndex; ///< the index of the structure tree group holding the variables. -1 if there is no such group.
     int functionGroupIndex; ///< the index of the structure tree group holding the functions. -1 if there is no such group.
     int classGroupIndex; ///< the index of the structure tree group holding the classes. -1 if there is no such group.
     int objectGroupIndex; ///< the index of the structure tree group holding the classes. -1 if there is no such group.
     mutable QRegExp memberAutoCompleteAfter; ///< the regular expression after which a list with the existing member methods and variables for a class should be shown. Makes sense only if the language supports classes.
     QMap<QString, QString> classInheritance; ///<stores the inheritance tree

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
  /** Returns true if the attribute exists */
  bool isAttribute(const QString &attrName);
  /** No descriptions */
  void setSingle(bool isSingle);
  /** No descriptions */
  void setOptional(bool isOptional);
  /** No descriptions */
  void setName(const QString& theName);
  /** No descriptions */
  QString name(bool doNotConvert = false);
  /** No descriptions */
  bool isSingle();
  /** No descriptions */
  bool isOptional();
  
  /**
   * This property is used to determine the scope of a tag action.
   * For example, if the user is in the midle of a word and press the bold button,
   * the scope is a "word", i.e., the whole word will be affected by the action. 
   * Instead, if center is pressed, all surrounding inline nodes will be affected by the new tag.
   */
  QString const& scope() const {return m_scope;}
  void setScope(QString const& scope) {m_scope = scope;}
  
  /** Returns true if tag is a possible child of this tag, or if
  there are no children defined and if trueIfNoChildsDefined is set to true. */
  bool isChild(const QString& tag, bool trueIfNoChildsDefined = true);
  //prefer using this variant, it handle Text, Empty, XmlTagEnd nodes!
  bool isChild(Node *node, bool trueIfNoChildsDefined = true, bool treatEmptyNodesAsText = false);
  /*** Returns the list of parent of this tag. */
  QPtrList<QTag> parents();

  /** No descriptions */
  QString fileName();
  /** No descriptions */
  void setFileName(const QString& fileName);
  /** Returns the attribute with name, or 0 if the tag does not have any attribute with name. */
  Attribute* attribute(const QString& name);

  /** The tag belongs to this DTD */
  const DTDStruct *parentDTD;
  /** The tag has the attributes of the above common groups */
  QStringList commonGroups;
  QStringList stoppingTags;
  QMap<QString, bool> childTags; ///<list of the possible child tags. If the value is true, the child is mandatory
  QString type; ///<function, class, xmltag, etc.
  QString returnType;  ///<useful is type="function"; may be int, string or whatever
  QString className; ///< the name of the class where the tag belongs to. Used only for script DTEP tags
  QString comment; ///< comment associated to the tag. Will appear as a tooltip in the autocompletion box. Useful for specifying version information (eg. since PHP5)

protected: // Protected attributes
  /** List of the possible attributes */
  AttributeList attrs;
  bool single;
  bool optional;
  QString tagName;
  QString m_scope;
  /** The path to the tag file. Null if there is no tag file for the tag. */
  QString m_fileName;
};


#endif
