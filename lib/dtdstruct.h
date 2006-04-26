/***************************************************************************
    begin                : Tue Jun 28 2005
    copyright            : (C) 2005 by Jens Herden <jens@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/

#ifndef DTDSTRUCT_H
#define DTDSTRUCT_H

#include "qtag.h"

#include <qlist.h>
#include <qstringlist.h>
#include <qregexp.h>



//an internal representation of a DTD
typedef struct DTDStruct
{
  enum DTDFamily {Unknown = 0, Xml, Script}; ///< defined DTD families

  QString name;                    ///< DTD name
  QString nickName;                ///< DTD nickname
  bool loaded;                     ///< true = DTD is complet in memory
  QString url;                     ///< the url of the DTD definition file
  QString doctypeStr;              ///< the string that appears right after !doctype
  QString inheritsTagsFrom;        ///< Inherited DTD name
  QString defaultExtension;        ///< default extension when creating new files
  QStringList mimeTypes;
  bool caseSensitive;              ///< the tags&attributes in DTD are case sensitive or not
  DTDFamily family;                ///< xml, script type
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
  QList<StructTreeGroup> structTreeGroups;
  QMap<QString, XMLStructGroup> xmlStructTreeGroups;

/****************** END FOR THE NEW PARSER **********************/
  QStringList toolbars;

/* True if foo-foo2 should be considered as one word. False (default) otherwise. */
  bool minusAllowedInWord;

  mutable QChar tagAutoCompleteAfter;
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


  bool isSingleTag(const QString& tag) const
  {
    //!doctype is a common tag to all DTDs not listed in the tagsList
    if(tag.toLower() == "!doctype" || tag.toLower() == "?xml")
      return true;

    QTag* qtag = qtagForTag(tag);
    return qtag ? qtag->isSingle() : false;
  }


  bool isOptionalTag(const QString& tag) const
  {
    QTag* qtag = qtagForTag(tag);
    return qtag ? qtag->isOptional() : false;
  }


  bool isKnownTag(const QString& tag) const
  {
    if (!tag.isEmpty())
    {
      QString searchForTag = caseSensitive ? tag : tag.toUpper();
      return tagsList->contains(searchForTag);
    }
    return false;
  }


  AttributeList* tagAttributes(const QString& tag) const
  {
    QTag* qtag = qtagForTag(tag);
    return qtag ? qtag->attributes() : 0;
  }

private:

  QTag * qtagForTag(const QString& tag) const
  {
    if (tag.isEmpty())
      return 0;

    QString searchForTag = caseSensitive ? tag : tag.toUpper();
    return tagsList->value(searchForTag);
  }

};


#endif
