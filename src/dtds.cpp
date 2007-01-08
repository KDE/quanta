/***************************************************************************
                             dtds.cpp
                             -------------------
    begin                : 12.02.2004  (extract from quanta_init and others)

    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev <pdima@users.sourceforge.net,yshurik@linuxfan.com>
                           (C) 2001-2003 by Andras Mantia <amantia@kde.org>
                           (C) 2000, 2003 by Eric Laffoon <sequitur@kde.org>
                           (C) 2004 by Jens Herden <jhe at epost.de>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

//qt includes
#include <qfile.h>
#include <qextfileinfo.h>
#include <qdom.h>
#include <qcursor.h>
#include <qtimer.h>

// include files for KDE
#include <kapplication.h>
#include <kcombobox.h>
#include <kconfig.h>
#include <kdialogbase.h>
#include <kurl.h>
#include <kurlrequester.h>
#include <kglobal.h>
#include <kmessagebox.h>
#include <klocale.h>
#include <kstandarddirs.h>
#include <kfiledialog.h>


// application specific includes
#include "quantacommon.h"
#include "resource.h"
#include "dtdparser.h"
#include "document.h"
#include "viewmanager.h"
#include "loadentitydlgs.h"

#include "dtds.h"

/** filename for the desciption of the dtd */
const QString m_rcFilename("description.rc");

/**
  This constructor reads the dictionary of known dtd's, the attributes and tags will be loaded
  on the first access to one dtd.
*/
DTDs::DTDs(QObject *parent)
  :QObject(parent)
{
  connect(this, SIGNAL(hideSplash()), parent, SLOT(slotHideSplash()));
  connect(this, SIGNAL(enableIdleTimer(bool)), parent, SLOT(slotEnableIdleTimer(bool)));
  connect(this, SIGNAL(loadToolbarForDTD(const QString&)), parent, SLOT(slotLoadToolbarForDTD(const QString&)));
//  kdDebug(24000) << "dtds::dtds" << endl;
  m_dict = new QDict<DTDStruct>(119, false); //optimized for max 119 DTD. This should be enough.
  m_dict->setAutoDelete(true);
  m_doc = new QDomDocument();

  QString localKDEdir = KGlobal::instance()->dirs()->localkdedir();
  QStringList tagsResourceDirs = KGlobal::instance()->dirs()->findDirs("appdata", "dtep");
  QStringList tagsDirs;
  QStringList::ConstIterator end = tagsResourceDirs.constEnd();
  for ( QStringList::ConstIterator it = tagsResourceDirs.constBegin(); it != end; ++it )
  {
    if ((*it).startsWith(localKDEdir))
    {
      QDir dir(*it);
      dir.setFilter(QDir::Dirs);
      QStringList subDirs = dir.entryList();
      QStringList::ConstIterator subitEnd = subDirs.constEnd();
      for ( QStringList::ConstIterator subit = subDirs.constBegin(); subit != subitEnd; ++subit )
      {
//         kdDebug(24000) << "dtds::dtds add:" << *it + *subit+"/" << endl;
        if ((*subit != ".") && (*subit != ".."))
          tagsDirs += *it + *subit + "/";
      }
    }
  }
  for ( QStringList::ConstIterator it = tagsResourceDirs.constBegin(); it != end; ++it )
  {
    if (!(*it).startsWith(localKDEdir))
    {
      QDir dir(*it);
      dir.setFilter(QDir::Dirs);
      QStringList subDirs = dir.entryList();
      QStringList::ConstIterator subitEnd = subDirs.constEnd();
      for ( QStringList::ConstIterator subit = subDirs.constBegin(); subit != subitEnd; ++subit )
      {
//         kdDebug(24000) << "dtds::dtds add2:" << *it + *subit+"/" << endl;
        if ((*subit != ".") && (*subit != ".."))
          tagsDirs += *it + *subit + "/";
      }
    }
  }
//  kdDebug(24000) << tagsDirs.count() << " folders found." << endl;
  QTime t;
  t.start();
  QStringList::ConstIterator tagsDirsEnd = tagsDirs.constEnd();
  for ( QStringList::ConstIterator it = tagsDirs.constBegin(); it != tagsDirsEnd; ++it )
  {
//     kdDebug(24000) << "read:" << *it  << endl;
    readTagDir(*it, false);  // read all tags, but only short form
  }
  kdDebug(24000) << "DTD reading: " << t.elapsed() << endl;
//load the mimetypes from the insideDTDs
  QDictIterator<DTDStruct> it(*m_dict);
  for( ; it.current(); ++it )
  {
    DTDStruct * dtd = it.current();
    for (uint i = 0; i < dtd->insideDTDs.count(); i++)
    {
      const DTDStruct *insideDTD = m_dict->find(dtd->insideDTDs[i]);  // search but don't load
      if (!insideDTD)
          insideDTD = m_dict->find(getDTDNameFromNickName(dtd->insideDTDs[i]));   // search but don't load
      if (insideDTD && !insideDTD->toplevel)
          dtd->mimeTypes += insideDTD->mimeTypes;
    }
  }

 //  kdDebug(24000) << "dtds::dtds constructed" << endl;
}

DTDs::~DTDs()
{
  QDictIterator<DTDStruct> it(*m_dict);
  for( ; it.current(); ++it )
  {
    removeDTD (it.current());
  }
  delete m_dict;
  delete m_doc;
}



void DTDs::removeDTD(DTDStruct *dtd)
{
  if (dtd)
  {
    delete dtd->tagsList;
    dtd->tagsList = 0L;
    delete dtd->commonAttrs;
    dtd->commonAttrs = 0L;
    m_dict->remove(dtd->name.lower());
  }
}


/** Reads the tag files and the description.rc from tagDir in order to
    build up the internal DTD and tag structures. */
bool DTDs::readTagDir(const QString &dirName, bool loadAll)
{
 // kdDebug(24000) << "dtds::readTagDir:" << dirName << "  all:" << loadAll << endl;
  QString tmpStr = dirName + m_rcFilename;
  if (!QFile::exists(tmpStr))
     return false;
  KConfig *dtdConfig = new KConfig(tmpStr, true);
  dtdConfig->setGroup("General");
  QString dtdName = dtdConfig->readEntry("Name", "Unknown");
  if (m_dict->find(dtdName.lower()))
  {
    delete dtdConfig;
    kdDebug(24000) << "dtds::readTagDir from " << dirName
                   << " canceled, DTD " << dtdName << " found in memory" << endl;
    return false;
  }

  //read the general DTD info
  DTDStruct *dtd = new DTDStruct;
  dtd->fileName  = tmpStr;
  dtd->name      = dtdName;
  dtd->nickName  = dtdConfig->readEntry("NickName", dtdName);
  dtd->mimeTypes = dtdConfig->readListEntry("MimeTypes");
  for (uint i = 0; i < dtd->mimeTypes.count(); i++)
    dtd->mimeTypes[i] = dtd->mimeTypes[i].stripWhiteSpace();
  dtd->family    = dtdConfig->readNumEntry("Family", Xml);
  if (dtd->family != Xml)
      dtd->toplevel = dtdConfig->readBoolEntry("TopLevel", false);
  else
      dtd->toplevel = true;
  dtd->tagsList = 0L;
  dtd->commonAttrs = 0L;

  //Read the areas that define the areas
  dtdConfig->setGroup("Parsing rules");
  QStringList definitionAreaBorders = dtdConfig->readListEntry("AreaBorders");
  for (uint i = 0; i < definitionAreaBorders.count(); i++)
  {
    QStringList tmpStrList = QStringList::split(" ", definitionAreaBorders[i].stripWhiteSpace());
    dtd->definitionAreas[tmpStrList[0].stripWhiteSpace()] = tmpStrList[1].stripWhiteSpace();
  }
  //Read the tags that define this DTD
  QStringList tmpStrList = dtdConfig->readListEntry("Tags");
  for (uint i = 0; i < tmpStrList.count(); i++)
  {
    tmpStr = tmpStrList[i].stripWhiteSpace();
    int pos = tmpStr.find('(');
    dtd->definitionTags[tmpStr.left(pos).stripWhiteSpace()] = tmpStr.mid(pos+1, tmpStr.findRev(')')-pos-1).stripWhiteSpace();
  }
  //Which DTD can be present in this one?
  dtd->insideDTDs = dtdConfig->readListEntry("MayContain");
  for (uint i = 0; i < dtd->insideDTDs.count(); i++)
  {
    dtd->insideDTDs[i] = dtd->insideDTDs[i].stripWhiteSpace().lower();
  }


  m_dict->insert(dtdName.lower(), dtd); //insert the structure into the dictionary
  delete dtdConfig;

  if (!loadAll)
  {
    dtd->loaded = false;
    return true;
  }

  dtd->loaded = readTagDir2(dtd);
  return dtd->loaded;
}


/** Reads the tag files and the description.rc from dtd in order to
    build up the internal DTD and tag structures.
    */
bool DTDs::readTagDir2(DTDStruct *dtd)
{
//  kdDebug(24000) << "dtds::readTagDir2:" << dtd->name << " at " << dtd->fileName << endl;

  if (!QFile::exists(dtd->fileName)) return false;

  kapp->setOverrideCursor( QCursor(Qt::WaitCursor) );

  KConfig *dtdConfig = new KConfig(dtd->fileName, true);

  //read the general DTD info
  dtdConfig->setGroup("General");
  dtd->commonAttrs = new AttributeListDict();
  dtd->commonAttrs->setAutoDelete(true);

  bool caseSensitive = dtdConfig->readBoolEntry("CaseSensitive");
  dtd->url = dtdConfig->readEntry("URL");
  dtd->doctypeStr = dtdConfig->readEntry("DoctypeString");
  if (dtd->doctypeStr.isEmpty())
  {
    dtd->doctypeStr = "PUBLIC \"" + dtd->name + "\"";
    if (!dtd->url.isEmpty())
        dtd->doctypeStr += " \"" + dtd->url + "\"";
  }
  dtd->doctypeStr.prepend(' ');
  dtd->inheritsTagsFrom = dtdConfig->readEntry("Inherits").lower();
  dtd->documentation = dtdConfig->readEntry("Documentation").lower();

  dtd->defaultExtension = dtdConfig->readEntry("DefaultExtension");
  dtd->caseSensitive = caseSensitive;
  int numOfTags = 0;
  QTagList *tagList = new QTagList(119, false); //max 119 tag in a DTD
  tagList->setAutoDelete(true);
  //read all the tag files
  KURL dirURL(dtd->fileName);
  dirURL.setFileName("");
  QString dirName = dirURL.path(1);
  if (QFile::exists(dirName + "common.tag"))
    readTagFile(dirName + "common.tag", dtd, 0L);
  //bool idleTimerStatus = quantaApp->slotEnableIdleTimer(false);
  emit enableIdleTimer(false);
  KURL::List files = QExtFileInfo::allFilesRelative(dirURL, "*.tag", 0L);
  //quantaApp->slotEnableIdleTimer(idleTimerStatus);
  emit enableIdleTimer(true);
  QString tmpStr;
  KURL::List::ConstIterator end_f = files.constEnd();
  for ( KURL::List::ConstIterator it_f = files.constBegin(); it_f != end_f; ++it_f )
  {
    tmpStr = (*it_f).path(-1);
    if (!tmpStr.isEmpty())
    {
      tmpStr.prepend(dirName);
      if (!tmpStr.endsWith("/common.tag"))
        numOfTags += readTagFile(tmpStr, dtd, tagList);
    }
  }

  //read the toolbars
  dtdConfig->setGroup("Toolbars");
  tmpStr = QuantaCommon::readPathEntry(dtdConfig, "Location"); //holds the location of the toolbars
  if (!tmpStr.endsWith("/") && !tmpStr.isEmpty())
  {
    tmpStr.append("/");
  }
  dtd->toolbars = dtdConfig->readListEntry("Names");
  for (uint i = 0; i < dtd->toolbars.count(); i++)
  {
    dtd->toolbars[i] = tmpStr + dtd->toolbars[i].stripWhiteSpace() + toolbarExtension;
  }

  //read the extra tags and their attributes
  dtdConfig->setGroup("Extra tags");
  dtd->defaultAttrType = dtdConfig->readEntry("DefaultAttrType","input");
  QStrList extraTagsList;
  dtdConfig->readListEntry("List",extraTagsList);
  QString option;
  QStrList optionsList;
  QStrList attrList;
  for (uint i = 0 ; i < extraTagsList.count(); i++)
  {
    QTag *tag = new QTag();
    tag->setName(QString(extraTagsList.at(i)).stripWhiteSpace());

    tmpStr = (dtd->caseSensitive) ? tag->name() : tag->name().upper();
    if (tagList->find(tmpStr)) //the tag is already defined in a .tag file
    {
      delete tag;
      continue; //skip this tag
    }
    tag->parentDTD = dtd;
    //read the possible stopping tags
    QStrList stoppingTags;
    dtdConfig->readListEntry(tag->name() + "_stoppingtags",stoppingTags);
    for (uint j = 0; j < stoppingTags.count(); j++)
    {
      QString stopTag = QString(stoppingTags.at(j)).stripWhiteSpace();
      if (!dtd->caseSensitive) stopTag = stopTag.upper();
      tag->stoppingTags.append(stopTag);
    }
    //read the possible tag options
    optionsList.clear();
    dtdConfig->readListEntry(tag->name() + "_options",optionsList);
    for (uint j = 0; j < optionsList.count(); j++)
    {
      option = QString(optionsList.at(j)).stripWhiteSpace();
      QDictIterator<AttributeList> it(*(dtd->commonAttrs));
      for( ; it.current(); ++it )
      {
        tmpStr = "has" + QString(it.currentKey()).stripWhiteSpace();
        if (option == tmpStr)
        {
          tag->commonGroups += QString(it.currentKey()).stripWhiteSpace();
        }
      }
      if (option == "single")
      {
        tag->setSingle(true);
      }
      if (option == "optional")
      {
        tag->setOptional(true);
      }
    }
    attrList.clear();
    dtdConfig->readListEntry(tag->name(), attrList);
    for (uint j = 0; j < attrList.count(); j++)
    {
      Attribute* attr = new Attribute;
      attr->name = QString(attrList.at(j)).stripWhiteSpace();
      attr->type = dtd->defaultAttrType;
      tag->addAttribute(attr);
      delete attr;
    }
    if (caseSensitive)
    {
      tagList->insert(tag->name(),tag);  //append the tag to the list for this DTD
    } else
    {
      tagList->insert(tag->name().upper(),tag);
    }
  }
  dtd->tagsList = tagList;
  dtd->tagsList->setAutoDelete(true);


  /**** Code for the new parser *****/

  dtdConfig->setGroup("Parsing rules");
  bool appendCommonRules = dtdConfig->readBoolEntry("AppendCommonSpecialAreas", true);
  //Read the special areas and area names
  QString rxStr = "";
  if (dtd->family == Xml && appendCommonRules)
  {
      dtd->specialAreas["<?xml"] = "?>";
      dtd->specialAreaNames["<?xml"] = "XML PI";
      dtd->specialAreas["<!--"] = "-->";
      dtd->specialAreaNames["<!--"] = "comment";
//      dtd->specialAreas["<!"] = ">";
  //    dtd->specialAreaNames["<!"] = "DTD";
      dtd->insideDTDs.append("dtd");
      tmpStr = "(<?xml)|(<!--)|(<!)|";
      rxStr = QuantaCommon::makeRxCompatible(tmpStr);
  }
  QStringList specialAreasList = dtdConfig->readListEntry("SpecialAreas");
  QStringList specialAreaNameList = dtdConfig->readListEntry("SpecialAreaNames");
  QStringList tmpStrList;
  for (uint i = 0; i < specialAreasList.count(); i++)
  {
    if (!specialAreasList[i].stripWhiteSpace().isEmpty())
    {
      tmpStrList = QStringList::split(" ",specialAreasList[i].stripWhiteSpace());
      tmpStr = tmpStrList[0].stripWhiteSpace();
      rxStr.append(QuantaCommon::makeRxCompatible(tmpStr)+"|");
      dtd->specialAreas[tmpStr] = tmpStrList[1].stripWhiteSpace();
      dtd->specialAreaNames[tmpStr] = specialAreaNameList[i];
    }
  }
  if (rxStr.isEmpty())
  {
    dtd->specialAreaStartRx.setPattern("");
  } else
  {
    dtd->specialAreaStartRx.setPattern(rxStr.left(rxStr.length() - 1));
  }
  //Read the special tags
  tmpStrList = dtdConfig->readListEntry("SpecialTags");
  for (uint i = 0; i < tmpStrList.count(); i++)
  {
    tmpStr = tmpStrList[i].stripWhiteSpace();
    int pos = tmpStr.find('(');
    dtd->specialTags[tmpStr.left(pos).stripWhiteSpace()] = tmpStr.mid(pos+1, tmpStr.findRev(')')-pos-1).stripWhiteSpace();
  }

  //static const QString quotationStr = "\\\\\"|\\\\'";
  rxStr = "\\\\\"|\\\\'|";
  QStringList commentsList = dtdConfig->readListEntry("Comments");
  if (dtd->family == Xml && appendCommonRules)
    commentsList.append("<!-- -->");
  QString tmpStr2;
  for (uint i = 0; i < commentsList.count(); i++)
  {
    tmpStrList = QStringList::split(" ",commentsList[i].stripWhiteSpace());
    tmpStr = tmpStrList[0].stripWhiteSpace();
    rxStr += QuantaCommon::makeRxCompatible(tmpStr);
    rxStr += "|";
    tmpStr2 = tmpStrList[1].stripWhiteSpace();
    if (tmpStr2 == "EOL")
        tmpStr2 = '\n';
    dtd->comments[tmpStr] = tmpStr2;
  }
  dtd->commentsStartRx.setPattern(rxStr.left(rxStr.length()-1));

  /**** End of code for the new parser *****/

  //read the definition of a structure, and the structure keywords
  QStringList structKeywords = dtdConfig->readListEntry("StructKeywords",',');
  if (structKeywords.count() !=0 )
  {
      tmpStr = "\\b(";
      for (uint i = 0; i < structKeywords.count(); i++)
      {
        tmpStr += structKeywords[i].stripWhiteSpace()+"|";
      }
      tmpStr.truncate(tmpStr.length()-1);
      tmpStr += ")\\b";
  } else
  {
    tmpStr = "\\b[\\d\\S\\w]+\\b";
  }
  dtd->structKeywordsRx.setPattern(tmpStr);

  structKeywords = dtdConfig->readListEntry("LocalScopeKeywords",',');
  if (structKeywords.count() !=0 )
  {
      tmpStr = "\\b(";
      for (uint i = 0; i < structKeywords.count(); i++)
      {
        tmpStr += structKeywords[i].stripWhiteSpace()+"|";
      }
      tmpStr.truncate(tmpStr.length()-1);
      tmpStr += ")\\b";
  } else
  {
    tmpStr = "\\b[\\d\\S\\w]+\\b";
  }
  dtd->localScopeKeywordsRx.setPattern(tmpStr);

  dtd->structRx.setPattern(dtdConfig->readEntry("StructRx","\\{|\\}").stripWhiteSpace());
  dtd->structBeginStr = dtdConfig->readEntry("StructBeginStr","{").stripWhiteSpace();
  dtd->structEndStr = dtdConfig->readEntry("StructEndStr","}").stripWhiteSpace();


  dtdConfig->setGroup("Extra rules");
  dtd->minusAllowedInWord = dtdConfig->readBoolEntry("MinusAllowedInWord", false);
  tmpStr = dtdConfig->readEntry("TagAutoCompleteAfter", "<").stripWhiteSpace();
  if (tmpStr.upper() == "NONE")
      dtd->tagAutoCompleteAfter = '\0';
  else
  if (tmpStr.upper() == "ALWAYS")
      dtd->tagAutoCompleteAfter = '\1';
  else
      dtd->tagAutoCompleteAfter = tmpStr.at(0);
  dtd->requestSpaceBeforeTagAutoCompletion = dtdConfig->readBoolEntry("RequestSpaceBeforeTagAutoCompletion", false);
  dtd->attrAutoCompleteAfter = dtdConfig->readEntry("AttributeAutoCompleteAfter","(").stripWhiteSpace().at(0);
  dtd->attributeSeparator = dtdConfig->readEntry("AttributeSeparator").stripWhiteSpace().at(0);
  if (dtd->attributeSeparator.isNull())
  {
    dtd->attributeSeparator = (dtd->family == Xml) ? '\"' : ',';
  }
  dtd->tagSeparator = dtdConfig->readEntry("TagSeparator").stripWhiteSpace().at(0);
  if (dtd->tagSeparator.isNull())
      dtd->tagSeparator = dtd->attributeSeparator;

  dtd->booleanAttributes = dtdConfig->readEntry("BooleanAttributes","extended");
  dtd->booleanTrue = dtdConfig->readEntry("BooleanTrue","true");
  dtd->booleanFalse = dtdConfig->readEntry("BooleanFalse","false");
  dtd->singleTagStyle = dtdConfig->readEntry("Single Tag Style", "xml").lower();
  dtd->variableGroupIndex = dtdConfig->readNumEntry("VariableGroupIndex", 0) - 1;
  dtd->functionGroupIndex = dtdConfig->readNumEntry("FunctionGroupIndex", 0) - 1;
  dtd->classGroupIndex = dtdConfig->readNumEntry("ClassGroupIndex", 0) - 1;
  if (dtd->classGroupIndex != -1)
  {
    tmpStr = dtdConfig->readEntry("MemberAutoCompleteAfter").stripWhiteSpace();
    dtd->memberAutoCompleteAfter.setPattern(tmpStr);
  }
  dtd->objectGroupIndex = dtdConfig->readNumEntry("ObjectGroupIndex", 0) - 1;

  //read the definition of different structure groups, like links, images, functions
  //classes, etc.
  uint structGroupsCount = dtdConfig->readNumEntry("StructGroupsCount", 0);
  if (structGroupsCount > MAX_STRUCTGROUPSCOUNT)
      structGroupsCount = MAX_STRUCTGROUPSCOUNT; //max. 10 groups

  if (dtd->family == Script)
  {
      StructTreeGroup group;
      QRegExp attrRx("\\([^\\)]*\\)");
      QString tagStr;
      for (uint index = 1; index <= structGroupsCount; index++)
      {
        dtdConfig->setGroup(QString("StructGroup_%1").arg(index));
        //new code
        group.name = dtdConfig->readEntry("Name").stripWhiteSpace();
        group.noName = dtdConfig->readEntry("No_Name").stripWhiteSpace();
        group.icon = dtdConfig->readEntry("Icon").stripWhiteSpace();
        tmpStr = dtdConfig->readEntry("DefinitionRx").stripWhiteSpace();
        group.definitionRx.setPattern(tmpStr);
        tmpStr = dtdConfig->readEntry("UsageRx").stripWhiteSpace();
        group.usageRx.setPattern(tmpStr);
        tmpStr = dtdConfig->readEntry("TypeRx").stripWhiteSpace();
        group.typeRx.setPattern(tmpStr);
        group.hasDefinitionRx = !group.definitionRx.pattern().isEmpty();
        group.isMinimalDefinitionRx = dtdConfig->readBoolEntry("DefinitionRx_Minimal", false);
        group.appendToTags = dtdConfig->readBoolEntry("AppendToTags", false);
        group.parentGroup = dtdConfig->readEntry("ParentGroup").stripWhiteSpace();
        tagStr = dtdConfig->readEntry("TagType", "Text").stripWhiteSpace();
        if (tagStr == "XmlTag")
            group.tagType = Tag::XmlTag;
        else if (tagStr == "XmlTagEnd")
            group.tagType = Tag::XmlTagEnd;
        else if (tagStr == "Text")
            group.tagType = Tag::Text;
        else if (tagStr == "Comment")
            group.tagType = Tag::Comment;
        else if (tagStr == "CSS")
            group.tagType = Tag::CSS;
        else if (tagStr == "ScriptTag")
            group.tagType = Tag::ScriptTag;
        else if (tagStr == "ScriptStructureBegin")
            group.tagType = Tag::ScriptStructureBegin;
        else if (tagStr == "ScriptStructureEnd")
            group.tagType = Tag::ScriptStructureEnd;
        else group.tagType = -1;
        tmpStr = dtdConfig->readEntry("AutoCompleteAfter").stripWhiteSpace();
        group.autoCompleteAfterRx.setPattern(tmpStr);
        tmpStr = dtdConfig->readEntry("RemoveFromAutoCompleteWord").stripWhiteSpace();
        group.removeFromAutoCompleteWordRx.setPattern(tmpStr);
        group.hasFileName = dtdConfig->readBoolEntry("HasFileName", false);
        group.parseFile = dtdConfig->readBoolEntry("ParseFile", false);
        tmpStr = dtdConfig->readEntry("FileNameRx").stripWhiteSpace();
        group.fileNameRx.setPattern(tmpStr);
        dtd->structTreeGroups.append(group);
      }
    } else
    {
      XMLStructGroup group;
      QRegExp attrRx("\\([^\\)]*\\)");
      QString tagName;
      for (uint index = 1; index <= structGroupsCount; index++)
      {
        dtdConfig->setGroup(QString("StructGroup_%1").arg(index));
        group.name = dtdConfig->readEntry("Name").stripWhiteSpace();
        group.noName = dtdConfig->readEntry("No_Name").stripWhiteSpace();
        group.icon = dtdConfig->readEntry("Icon").stripWhiteSpace();
        group.appendToTags = dtdConfig->readBoolEntry("AppendToTags", false);
        group.parentGroup = dtdConfig->readEntry("ParentGroup").stripWhiteSpace();
        QString tagStr = dtdConfig->readEntry("Tag").stripWhiteSpace();
        if (!tagStr.isEmpty())
        {
          attrRx.search(tagStr);
          tmpStr = attrRx.cap();
          tmpStrList = QStringList::split(',', tmpStr.mid(1, tmpStr.length()-2));
          tagName = tagStr.left(tagStr.find('(')).lower();
          group.attributes.clear();
          for (uint i = 0; i < tmpStrList.count(); i++)
            group.attributes += tmpStrList[i].stripWhiteSpace();
          group.hasFileName = dtdConfig->readBoolEntry("HasFileName", false);
          tmpStr = dtdConfig->readEntry("FileNameRx").stripWhiteSpace();
          group.fileNameRx.setPattern(tmpStr);
          dtd->xmlStructTreeGroups.insert(tagName, group);
        }
      }
    }

  delete dtdConfig;
  dtd->loaded = true;
  resolveInherited(dtd);
  kapp->restoreOverrideCursor();
  return true;
}


void DTDs::resolveInherited (DTDStruct *dtd)
{
  //Resolve the inheritence
  if (!dtd->inheritsTagsFrom.isEmpty())
  {
    DTDStruct *parent = (DTDStruct *) find(dtd->inheritsTagsFrom);  // this loads the dtd, if not present in memory
    QDictIterator<QTag> tag_it(*(parent->tagsList));
    for ( ; tag_it.current(); ++tag_it)
    {
      QTag *tag = tag_it.current();
      QString searchForTag = (dtd->caseSensitive) ? tag->name() : tag->name().upper();
      if (!dtd->tagsList->find(searchForTag))
      {
        QTag *newTag = new QTag(*tag);
        dtd->tagsList->insert(searchForTag, newTag);
      }
    }
  }

//Read the pseudo DTD area definition strings (special area/tag string)
//from the DTD's which may be present in the DTD (May_Contain setting)
  QMap<QString, QString>::ConstIterator mapIt;
  QString specialAreaStartRxStr = dtd->specialAreaStartRx.pattern();
  if (!specialAreaStartRxStr.isEmpty())
      specialAreaStartRxStr += "|";
  for (uint i = 0; i < dtd->insideDTDs.count(); i++)
  {
    const DTDStruct *insideDTD = m_dict->find(dtd->insideDTDs[i]);  // search but don't load
    if (!insideDTD)
        insideDTD = m_dict->find(getDTDNameFromNickName(dtd->insideDTDs[i]));   // search but don't load
    if (insideDTD)
    {
      for (mapIt = insideDTD->definitionAreas.begin(); mapIt != insideDTD->definitionAreas.end(); ++mapIt)
      {
        QString tmpStr = mapIt.key();
        dtd->specialAreas[tmpStr] = mapIt.data();
        dtd->specialAreaNames[tmpStr] = dtd->insideDTDs[i];
        specialAreaStartRxStr.append("(?:" +  QuantaCommon::makeRxCompatible(tmpStr) + ")|");
      }

      for (mapIt = insideDTD->definitionTags.begin(); mapIt != insideDTD->definitionTags.end(); ++mapIt)
      {
        dtd->specialTags[mapIt.key()] = mapIt.data();
      }
    }
  dtd->specialAreaStartRx.setPattern(specialAreaStartRxStr.left(specialAreaStartRxStr.length() - 1));
  };
}



/** Reads the tags for the tag files. Returns the number of read tags. */
uint DTDs::readTagFile(const QString& fileName, DTDStruct* parentDTD, QTagList *tagList)
{
//  kdDebug(24000) << "dtds::readTagFile:" << fileName << endl;
 QFile f(fileName);
 if (! f.exists())
   kdError() << "dtds::readTagFile file does not exist:" << fileName << endl;
 else
 {
   bool result = f.open( IO_ReadOnly );
   if (! result)
     kdError() << "dtds::readTagFile unable to open:" << fileName
                                << " Status: " << f.status() << endl;
 }
 QString errorMsg;
 int errorLine, errorCol;
 if (!m_doc->setContent( &f, &errorMsg, &errorLine, &errorCol ))
 {
   emit hideSplash();
   KMessageBox::error(0L, i18n("<qt>The DTD tag file %1 is not valid.<br> The error message is: <i>%2 in line %3, column %4.</i></qt>").arg(fileName).arg(errorMsg).arg(errorLine).arg(errorCol),
   i18n("Invalid Tag File"));
   kdWarning() << fileName << ": " << errorMsg << ": " << errorLine << "," << errorCol << endl;
 }

 f.close();
 QDomNodeList nodeList = m_doc->elementsByTagName("tag");
 uint numOfTags = nodeList.count();
 for (uint i = 0; i < numOfTags; i++)
 {
    QDomNode n = nodeList.item(i);
    QDomElement e = n.toElement();
    if (e.attribute("type") == "class")
    {
      QString extends = e.attribute("extends");
      QString name = e.attribute("name");
      if (!name.isEmpty() && !extends.isEmpty())
        parentDTD->classInheritance[name] = extends;
      continue;
    }
    QTag *tag = new QTag();
    tag->setName(e.attribute("name"));
    tag->setFileName(fileName);
    tag->parentDTD = parentDTD;
    bool common = false;
    setAttributes(&n, tag, common);
    if (common)
    {
      QString groupName = e.attribute("name");
      AttributeList *attrs = tag->attributes();
      attrs->setAutoDelete(false);
      AttributeList *commonAttrList = new AttributeList;      //no need to delete it
      commonAttrList->setAutoDelete(true);
      *commonAttrList = *attrs;
      //delete tag;
      parentDTD->commonAttrs->insert(groupName, commonAttrList);
    } else
    {
      if (parentDTD->caseSensitive)
      {
        tagList->replace(tag->name(), tag);  //append the tag to the list for this DTD
      } else
      {
        tagList->replace(tag->name().upper(), tag);
      }
    }
 }
 return numOfTags;
}


/**
 Parse the dom document and retrieve the tag attributes
*/
void DTDs::setAttributes(QDomNode *dom, QTag* tag, bool &common)
{
 common = false;
 Attribute *attr;

 QDomElement el = dom->toElement();
 QString tmpStr;

 tmpStr = el.attribute("common");
 if ((tmpStr != "1" && tmpStr != "yes")) //in case of common tags, we are not interested in these options
 {
    if (tag->parentDTD->commonAttrs)
    {
      QDictIterator<AttributeList> it(*(tag->parentDTD->commonAttrs));
      for( ; it.current(); ++it )
      {
        QString lookForAttr = "has" + QString(it.currentKey()).stripWhiteSpace();
        tmpStr = el.attribute(lookForAttr);
        if (tmpStr == "1" || tmpStr == "yes")
        {
          tag->commonGroups += QString(it.currentKey()).stripWhiteSpace();
        }
      }
    }

    tmpStr = el.attribute("single");
    if (tmpStr == "1" || tmpStr == "yes")
    {
      tag->setSingle(true);
    }

    tmpStr = el.attribute("optional");
    if (tmpStr == "1" || tmpStr == "yes")
    {
      tag->setOptional(true);
    }

    tmpStr = el.attribute("scope");
    tag->setScope(tmpStr);

    tag->type = el.attribute("type", "xmltag");
    tag->returnType = el.attribute("returnType", "");
    tag->className = el.attribute("class", "");
    tag->comment = el.attribute("comment", "");
    if (!tag->comment.isEmpty())
      tag->comment = " [" + i18n(tag->comment) + "] ";
    tag->comment.prepend(el.attribute("version"));
 } else
 {
   common = true;
 }
 QString attrList;
 for ( QDomNode n = dom->firstChild(); !n.isNull(); n = n.nextSibling() )
 {
   tmpStr = n.nodeName();
   if (tmpStr == "children")
   {
     QDomElement el = n.toElement();
     QDomElement item = el.firstChild().toElement();
     while ( !item.isNull() )
     {
        tmpStr = item.tagName();
        if (tmpStr == "child")
        {
            QString childTag = item.attribute("name");
            if (!tag->parentDTD->caseSensitive)
                childTag = childTag.upper();
            tag->childTags.insert(childTag, item.attribute("usage") == "required");
        }
        item = item.nextSibling().toElement();
     }
   } else
   if (tmpStr == "stoppingtags") //read what tag can act as closing tag
   {
     QDomElement el = n.toElement();
     QDomElement item = el.firstChild().toElement();
     while ( !item.isNull() )
     {
       if (item.tagName() == "stoppingtag")
       {
         QString stopTag = item.attribute("name");
         if (!tag->parentDTD->caseSensitive)
            stopTag = stopTag.upper();
         tag->stoppingTags.append(stopTag);
       }
       item = item.nextSibling().toElement();
     }
   } else
   if (tmpStr == "attr") //an attribute
   {
     QDomElement el = n.toElement();
     attr = new Attribute;
     attr->name = el.attribute("name");
     attr->source = el.attribute("source");
     attr->interface = el.attribute("interface");
     attr->method = el.attribute("method");
     attr->arguments = el.attribute("arguments");

     attr->type = el.attribute("type",tag->parentDTD->defaultAttrType);
     attr->defaultValue = el.attribute("defaultValue");
     attr->status = el.attribute("status");

     if ( attr->type == "list" ) {
       for ( QDomElement attrEl = el.firstChild().toElement(); !attrEl.isNull(); attrEl = attrEl.nextSibling().toElement() ) {
         if ( attrEl.tagName() == "items" ) {
           QDomElement item = attrEl.firstChild().toElement();
           while ( !item.isNull() ) {
             attr->values.append( item.text() );
             item = item.nextSibling().toElement();
           }
         }
       }
     } else if ( attr->type == "check" ) {
       attr->values.append("true");
       attr->values.append("false");
     } else if ( attr->type == "color" ) {
       attr->values.append("Black");
       attr->values.append("Silver");
       attr->values.append("Gray");
       attr->values.append("White");
       attr->values.append("Maroon");
       attr->values.append("Red");
       attr->values.append("Purple");
       attr->values.append("Fuchsia");
       attr->values.append("Green");
       attr->values.append("Lime");
       attr->values.append("Olive");
       attr->values.append("Yellow");
       attr->values.append("Navy");
       attr->values.append("Blue");
       attr->values.append("Teal");
       attr->values.append("Aqua");
     } else if ( attr->type == "url" ) {
       //not treated yet
     } else if ( attr->type == "input" ) {
       //not treated yet
     }

     if (tag->type == "function" || tag->type == "method")
     {
      if (attr->status == "optional")
      {
        attrList = attrList + "["+attr->type +" "+attr->name +"], ";
      } else
      {
        attrList = attrList + attr->type +" "+attr->name +", ";
      }
     }
     if (!attr->name.isEmpty())
     {
       tag->addAttribute(attr);
     }
     delete attr;
   }
 }
 if (!attrList.isEmpty())
   tag->comment.prepend(attrList.left(attrList.length() - 2) + "; ");
}


void DTDs::slotLoadDTD()
{
  KURL url = KFileDialog::getOpenURL("", i18n("*.dtd|DTD Definitions"), 0L);
  if (!url.isEmpty())
  {
    DTDParser dtdParser(url, KGlobal::dirs()->saveLocation("data") + resourceDir + "dtep");
    if (dtdParser.parse())
    {
      QString dirName = dtdParser.dirName();
      KConfig dtdcfg(dirName + m_rcFilename, true);
      dtdcfg.setGroup("General");
      QString dtdName = dtdcfg.readEntry("Name");
      QString nickName = dtdcfg.readEntry("NickName", dtdName);
      DTDStruct * dtd = m_dict->find(dtdName) ;
      if (dtd &&
          KMessageBox::warningYesNo(0L, i18n("<qt>Do you want to replace the existing <b>%1</b> DTD?</qt>").arg(nickName), QString::null, i18n("Replace"), i18n("Do Not Replace")) == KMessageBox::No)
      {
        return;
      }
      removeDTD(dtd);
      if (readTagDir(dirName))
      {
          QString family = dtdcfg.readEntry("Family", "1");
          Document *w = ViewManager::ref()->activeDocument();
          if (family == "1" && w &&
              KMessageBox::questionYesNo(0L, i18n("<qt>Use the newly loaded <b>%1</b> DTD for the current document?</qt>").arg(nickName), i18n("Change DTD"), i18n("Use"), i18n("Do Not Use")) == KMessageBox::Yes)
          {
            w->setDTDIdentifier(dtdName);
            emit loadToolbarForDTD(w->getDTDIdentifier());
            emit forceReparse();
          }
      }
    }
  }
}

void DTDs::slotLoadDTEP(const QString &_dirName, bool askForAutoload)
{
  QString dirName = _dirName;
  if (!dirName.endsWith("/"))
    dirName += "/";
  KConfig dtdcfg(dirName + m_rcFilename, true);
  dtdcfg.setGroup("General");
  QString dtdName = dtdcfg.readEntry("Name");
  QString nickName = dtdcfg.readEntry("NickName", dtdName);
  DTDStruct * dtd = m_dict->find(dtdName) ;
  if ( dtd &&
      KMessageBox::warningYesNo(0L, i18n("<qt>Do you want to replace the existing <b>%1</b> DTD?</qt>").arg(nickName), QString::null, i18n("Replace"), i18n("Do Not Replace")) == KMessageBox::No)
  {
    return;
  }
  removeDTD(dtd);
  if (!readTagDir(dirName))
  {
    KMessageBox::error(0L, i18n("<qt>Cannot read the DTEP from <b>%1</b>. Check that the folder contains a valid DTEP (<i>description.rc and *.tag files</i>).</qt>").arg(dirName), i18n("Error Loading DTEP"));
  } else
  {
    QString family = dtdcfg.readEntry("Family", "1");
    if (askForAutoload && KMessageBox::questionYesNo(0L, i18n("<qt>Autoload the <b>%1</b> DTD in the future?</qt>").arg(nickName), QString::null, i18n("Load"), i18n("Do Not Load")) == KMessageBox::Yes)
    {
      KURL src;
      src.setPath(dirName);
      KURL target;
      QString destDir = KGlobal::dirs()->saveLocation("data") + resourceDir + "dtep/";
      target.setPath(destDir + src.fileName());
      KIO::copy( src, target, false); //don't care about the result
    }
    Document *w = ViewManager::ref()->activeDocument();
    if (family == "1" && w &&
        KMessageBox::questionYesNo(0L, i18n("<qt>Use the newly loaded <b>%1</b> DTD for the current document?</qt>").arg(nickName), i18n("Change DTD"), i18n("Use"), i18n("Do Not Use")) == KMessageBox::Yes)
    {
      w->setDTDIdentifier(dtdName);
      emit loadToolbarForDTD(w->getDTDIdentifier());
      emit forceReparse();
    }
  }
}

void DTDs::slotLoadEntities()
{
  KDialogBase dlg(0L, "loadentities", true, i18n("Load DTD Entities Into DTEP"), KDialogBase::Ok | KDialogBase::Cancel);
  LoadEntityDlgS entitiesWidget(&dlg);
  QStringList lst(DTDs::ref()->nickNameList(true));
  entitiesWidget.targetDTEPCombo->insertStringList(lst);
  Document *w = ViewManager::ref()->activeDocument();
  if (w)
  {
    QString nickName = DTDs::ref()->getDTDNickNameFromName(w->getDTDIdentifier());
    entitiesWidget.targetDTEPCombo->setCurrentItem(lst.findIndex(nickName));
  }
  dlg.setMainWidget(&entitiesWidget);
  if (dlg.exec())
  {
    DTDStruct * dtd = m_dict->find(getDTDNameFromNickName(entitiesWidget.targetDTEPCombo->currentText()));
    DTDParser dtdParser(KURL::fromPathOrURL(entitiesWidget.sourceDTDRequester->url()), KGlobal::dirs()->saveLocation("data") + resourceDir + "dtep");
    QString dtdDir = QFileInfo(dtd->fileName).dirPath();
    if (dtdParser.parse(dtdDir, true))
    {
      readTagFile(dtdDir + "/entities.tag", dtd, dtd->tagsList);
    }
  }
}


/** Returns the DTD name (identifier) corresponding to the DTD's nickname */
QString DTDs::getDTDNameFromNickName(const QString& nickName)
{
  QDictIterator<DTDStruct> it(*m_dict);
  for( ; it.current(); ++it )
  {
    if (it.current()->nickName.lower() == nickName.lower())
    {
      return it.current()->name;
    }
  }
  return nickName;
}

/** returns the known nick names */
QStringList DTDs::nickNameList(bool topLevelOnly)
{
  QStringList nickList;
  QDictIterator<DTDStruct> it(*m_dict);
  for( ; it.current(); ++it )
  {
    if (!topLevelOnly || it.current()->toplevel)
    {
      nickList << it.current()->nickName;
    }
  }
  nickList.sort();
  return nickList;
}


/** returns the known names */
QStringList DTDs::nameList(bool topLevelOnly)
{
  QStringList nameList;
  QDictIterator<DTDStruct> it(*m_dict);
  for( ; it.current(); ++it )
  {
    if (!topLevelOnly || it.current()->toplevel)
    {
      nameList << it.current()->name;
    }
  }
  nameList.sort();
  return nameList;
}

QStringList DTDs::fileNameList(bool topLevelOnly)
{
  QStringList nameList;
  QDictIterator<DTDStruct> it(*m_dict);
  for( ; it.current(); ++it )
  {
    if (!topLevelOnly || it.current()->toplevel)
    {
      nameList << (it.current()->name + "|" + it.current()->fileName);
    }
  }
  return nameList;
}


const DTDStruct * DTDs::DTDforURL(const KURL &url)
{
  QValueList<DTDStruct*> foundList;
  QDictIterator<DTDStruct> it(*m_dict);
  for( ; it.current(); ++it )
  {
    if (it.current()->toplevel && canHandle(it.current(), url))
    {
      foundList.append(it.current());
    }
  }
  if (foundList.isEmpty())
    return find("empty");
  else
  {
    QString path = url.path();
    for (uint i = 0; i < foundList.count(); i++)
    {
      if (path.endsWith('.' + foundList[i]->defaultExtension))
        return foundList[i];
    }
    return foundList[0];
  }
}

bool DTDs::canHandle(const DTDStruct *dtd, const KURL &url)
{
  QString mimetype = KMimeType::findByURL(url)->name();
  if (dtd->mimeTypes.contains(mimetype))
    return true;
  if (url.path().endsWith('.' + dtd->defaultExtension))
    return true;
  return false;
}

#include "dtds.moc"
