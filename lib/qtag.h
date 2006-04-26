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

//#include "dtdstruct.h"


/**Quanta style tag (XML tag or code tag), as they are defined in the DTD. Contains
  all the possible attributes and the possible values for the attributes. Do not
  confund with the Tag class, which can change as the user types other attributes and
  changes their values.

  *@author Andras Mantia
  */

//app includes

//qt includes
#include <qhash.h>
#include <qmap.h>
#include <qlist.h>
#include <qstringlist.h>
#include <qregexp.h>


//class QTag;
class Node;
struct DTDStruct;


//an attribute of a tag looks like:
typedef struct Attribute
{
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
      
typedef QList<Attribute*> AttributeList;

typedef QHash<QString, AttributeList*> AttributeListDict;



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
class StructTreeGroup : public XMLStructGroup {
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



#define MAX_STRUCTGROUPSCOUNT 10


class QTag {
public:
  QTag();
  QTag(  QTag&);
  ~QTag();
  QTag operator = (QTag& );
  /** Add an attribute to the tag. */
  void addAttribute(Attribute* attr);
  /** Returns the number of attributes for the tag. */
  int attributeCount() const;
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
  QList<QTag*> parents();

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
  
  /** Returns the QTag object for the tag "tag" from the DTD named "dtdname". 
   * 
   * @param dtdName 
   * @param tag 
   * @return pointer to @see QTag or 0
   */
//  static QTag* tagFromDTD(const QString& dtdName, const QString& tag);
  
  /** Returns the QTag object for the tag "tag" from the DTD. 
   * 
   * @param dtd 
   * @param tag 
   * @return pointer to @see QTag or 0
   */
  static QTag* tagFromDTD(const DTDStruct* dtd, const QString& tag);
  /** Returns the QTag object for the node "node" from node's DTD.
   *
   * @param node 
   * @return pointer to @see QTag or 0
   */
  static QTag* tagFromDTD(Node *node);

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

typedef QHash<QString, QTag*> QTagList;


#endif

// kate: space-indent on; indent-width 2; mixedindent off; indent-mode cstyle;
