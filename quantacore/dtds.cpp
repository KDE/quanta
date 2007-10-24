/***************************************************************************
    begin                : 12.02.2004  (extract from quanta_init and others)

    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev <pdima@users.sourceforge.net,yshurik@linuxfan.com>
                           (C) 2001-2003 by Andras Mantia <amantia@kde.org>
                           (C) 2000, 2003 by Eric Laffoon <sequitur@kde.org>
                           (C) 2004 by Jens Herden <jens@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

// application specific includes
#include "dtds.h"
#include "quantacommon.h"
#include "dtdparser.h"
#include "helper.h"


//qt includes
#include <QFile>
#include <extfileinfo.h>
#include <qdom.h>
#include <QCursor>

// include files for KDE
#include <kapplication.h>
#include <kconfig.h>
#include <kdebug.h>
#include <kurl.h>
#include <kglobal.h>
#include <kmessagebox.h>
#include <klocale.h>
#include <kstandarddirs.h>
#include <kfiledialog.h>
#include <kio/copyjob.h>

DTDs *DTDs::m_ref = 0L;

/** filename for the desciption of the dtd */
const QString DTDs::m_rcFilename = "description.rc";
/**
  This constructor reads the dictionary of known dtd's, the attributes and tags will be loaded
  on the first access to one dtd.
*/
DTDs::DTDs(QObject *parent)
  :QObject(parent)
{
  kDebug(24000) << "Constructing DTDs";
//  connect(this, SIGNAL((bool)), parent, SLOT(slotEnableIdleTimer(bool)));
  connect(this, SIGNAL(loadToolbarForDTD(const QString&)), parent, SLOT(slotLoadToolbarForDTD(const QString&)));
//  kDebug(24000) << "dtds::dtds";
  m_dict = new QHash<QString,DTDStruct*>;
  m_globalEntities = new QHash<QString, QTag*>;
  m_doc = new QDomDocument();

  QStringList tagsResourceDirs = KGlobal::mainComponent().dirs()->findDirs("data", "quanta/dtep");
  QStringList tagsDirs;
  for ( QStringList::Iterator it = tagsResourceDirs.begin(); it != tagsResourceDirs.end(); ++it )
  {
    QDir dir(*it);
    if (QFile::exists(dir.path() + "/entities.tag"))
    {
      readTagFile(dir.path() + "/entities.tag", 0L, m_globalEntities);
    }
    dir.setFilter(QDir::Dirs);
    QStringList subDirs = dir.entryList();
    for ( QStringList::Iterator subit = subDirs.begin(); subit != subDirs.end(); ++subit )
    {
//      kDebug(24000) << "dtds::dtds add:" << *it + *subit+'/';
      if ((*subit != ".") && (*subit != ".."))
         tagsDirs += *it + *subit + '/';
    }
  }
//  kDebug(24000) << tagsDirs.count() << " folders found.";
  for ( QStringList::Iterator it = tagsDirs.begin(); it != tagsDirs.end(); ++it )
  {
    readTagDir(*it, false);  // read all tags, but only short form
  }
//  kDebug(24000) << "dtds::dtds constructed";
}

DTDs::~DTDs()
{
  QHashIterator<QString, DTDStruct*> it(*m_dict);
  while (it.hasNext())
  {
    it.next();
    removeDTD(it.value());
  }
  delete m_dict;
  delete m_doc;
  QHashIterator<QString, QTag*> it2(*m_globalEntities);
  while (it2.hasNext())
  {
    it2.next();
    delete it2.value();
  }
  delete m_globalEntities;
  m_ref = 0L;
}



void DTDs::removeDTD(DTDStruct *dtd)
{
  if (dtd)
  {
    if (dtd->loaded)
    {
      QHashIterator<QString, QTag*> tagsIt(*(dtd->tagsList));
      while (tagsIt.hasNext())
      {
        tagsIt.next();
        delete tagsIt.value();
      }
      delete dtd->tagsList;
      dtd->tagsList = 0L;
      QHashIterator<QString, AttributeList*> it(*dtd->commonAttrs);
      while (it.hasNext())
      {
        it.next();
        AttributeList* list = it.value();
        qDeleteAll(list->begin(), list->end());
        delete list;
      }
      delete dtd->commonAttrs;
      dtd->commonAttrs = 0L;
    }
    m_dict->take(dtd->name.toLower());
    delete dtd;
  }
}


/** Reads the tag files and the description.rc from tagDir in order to
    build up the internal DTD and tag structures. */
bool DTDs::readTagDir(const QString &dirName, bool loadAll)
{
 // kDebug(24000) << "dtds::readTagDir:" << dirName << "  all:" << loadAll;
  QString tmpStr = dirName + m_rcFilename;
  if (!QFile::exists(tmpStr))
     return false;
  KConfig dtdConfig(tmpStr);
  KConfigGroup grp(&dtdConfig, "General");
  QString dtdName = grp.readEntry("Name", "Unknown");
  if (m_dict->contains(dtdName.toLower()))
  {
    kDebug(24000) << "dtds::readTagDir from " << dirName
                   << " canceled, DTD " << dtdName << " found in memory" << endl;
    return false;
  }
  //read the general DTD info
  DTDStruct *dtd = new DTDStruct;
  dtd->fileName  = tmpStr;
  dtd->name      = dtdName;
  dtd->nickName  = grp.readEntry("NickName", dtdName);
  dtd->mimeTypes = grp.readEntry("MimeTypes", QStringList() );
  for (int i = 0; i < dtd->mimeTypes.count(); i++)
    dtd->mimeTypes[i] = dtd->mimeTypes[i].trimmed();
  dtd->family = static_cast<DTDStruct::DTDFamily>(grp.readEntry("Family", /*DTDStruct::Xml*/ 1)); //FIXME convert back to enum
  if (dtd->family != DTDStruct::Xml)
      dtd->toplevel = grp.readEntry("TopLevel", false);
  else
      dtd->toplevel = true;
  dtd->tagsList = 0L;
  dtd->commonAttrs = 0L;

  //Read the areas that define the areas
  grp.changeGroup("Parsing rules");
  QStringList definitionAreaBorders = grp.readEntry("AreaBorders", QStringList());
  for (int i = 0; i < definitionAreaBorders.count(); i++)
  {
    QStringList tmpStrList = definitionAreaBorders[i].trimmed().split(" ");
    dtd->definitionAreas[tmpStrList[0].trimmed()] = tmpStrList[1].trimmed();
  }
  //Read the tags that define this DTD
  QStringList tmpStrList = grp.readEntry("Tags", QStringList());
  for (int i = 0; i < tmpStrList.count(); i++)
  {
    tmpStr = tmpStrList[i].trimmed();
    int pos = tmpStr.indexOf('(');
    dtd->definitionTags[tmpStr.left(pos).trimmed()] = tmpStr.mid(pos+1, tmpStr.lastIndexOf(')')-pos-1).trimmed();
  }

  kDebug(24000) << "DTD loaded: " << dtdName << " pointer " << dtd;

  m_dict->insert(dtdName.toLower(), dtd); //insert the structure into the dictionary

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
//  kDebug(24000) << "dtds::readTagDir2:" << dtd->name << " at " << dtd->fileName;

  if (!QFile::exists(dtd->fileName)) return false;

  kapp->setOverrideCursor( QCursor(Qt::WaitCursor) );

  KConfig dtdConfig(dtd->fileName);
  KConfigGroup grp(&dtdConfig, "General");
  //read the general DTD info
  dtd->commonAttrs = new AttributeListDict();

  bool caseSensitive = grp.readEntry("CaseSensitive", false);
  dtd->url = grp.readEntry("URL");
  dtd->doctypeStr = grp.readEntry("DoctypeString");
  if (dtd->doctypeStr.isEmpty())
  {
    dtd->doctypeStr = "PUBLIC \"" + dtd->name + "\"";
    if (!dtd->url.isEmpty())
        dtd->doctypeStr += " \"" + dtd->url + "\"";
  }
  dtd->doctypeStr.prepend(' ');
  dtd->inheritsTagsFrom = grp.readEntry("Inherits").toLower();
  dtd->documentation = grp.readEntry("Documentation").toLower();

  dtd->defaultExtension = grp.readEntry("DefaultExtension", "html");
  dtd->caseSensitive = caseSensitive;
  int numOfTags = 0;
  QTagList *tagList = new QTagList;
  //read all the tag files
  KUrl dirURL = KUrl(dtd->fileName);
  dirURL.setFileName("");
  QString dirName = dirURL.path(KUrl::AddTrailingSlash);
  if (QFile::exists(dirName + "common.tag"))
    readTagFile(dirName + "common.tag", dtd, 0L);
  //bool idleTimerStatus = quantaApp->slotEnableIdleTimer(false);
//  emit enableIdleTimer(false);
  KUrl::List files = ExtFileInfo::allFilesRelative(dirURL, "*.tag");
  //quantaApp->slotEnableIdleTimer(idleTimerStatus);
//  emit enableIdleTimer(true);
  QString tmpStr;
  for ( KUrl::List::Iterator it_f = files.begin(); it_f != files.end(); ++it_f )
  {
    tmpStr = (*it_f).fileName();
    if (!tmpStr.isEmpty())
    {
      tmpStr.prepend(dirName);
      if (!tmpStr.endsWith("/common.tag"))
        numOfTags += readTagFile(tmpStr, dtd, tagList);
    }
  }

  //read the toolbars
  grp.changeGroup("Toolbars");
  tmpStr = grp.readPathEntry("Location"); //holds the location of the toolbars
  if (!tmpStr.endsWith('/') && !tmpStr.isEmpty())
  {
    tmpStr.append('/');
  }
  dtd->toolbars = grp.readEntry("Names", QStringList());
  for (int i = 0; i < dtd->toolbars.count(); i++)
  {
    dtd->toolbars[i] = tmpStr + dtd->toolbars[i].trimmed() + Helper::toolbarExtension();
  }

  //read the extra tags and their attributes
  grp.changeGroup("Extra tags");
  dtd->defaultAttrType = grp.readEntry("DefaultAttrType","input");
  QStringList extraTagsList = grp.readEntry("List", QStringList());
  QString option;
  QStringList optionsList;
  QStringList attrList;
  for (int i = 0 ; i < extraTagsList.count(); i++)
  {
    QTag *tag = new QTag();
    tag->setName(QString(extraTagsList.at(i)).trimmed());

    tmpStr = tag->name(dtd->caseSensitive);
    if (tagList->contains(tmpStr)) //the tag is already defined in a .tag file
    {
      delete tag;
      continue; //skip this tag
    }
    tag->parentDTD = dtd;
    //read the possible stopping tags
    QStringList stoppingTags = grp.readEntry(tag->name() + "_stoppingtags", QStringList());
    for (int j = 0; j < stoppingTags.count(); j++)
    {
      QString stopTag = QString(stoppingTags.at(j)).trimmed();
      if (!dtd->caseSensitive) stopTag = stopTag.toUpper();
      tag->stoppingTags.append(stopTag);
    }
    //read the possible tag options
    optionsList = grp.readEntry(tag->name() + "_options", QStringList());
    for (int j = 0; j < optionsList.count(); j++)
    {
      option = QString(optionsList.at(j)).trimmed();
      QHashIterator<QString, AttributeList*> it(*(dtd->commonAttrs));
      while (it.hasNext())
      {
        it.next();
        tmpStr = "has" + QString(it.key()).trimmed();
        if (option == tmpStr)
        {
          tag->commonGroups += QString(it.key()).trimmed();
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
    attrList = grp.readEntry(tag->name(), QStringList());
    for (int j = 0; j < attrList.count(); j++)
    {
      Attribute* attr = new Attribute;
      attr->name = QString(attrList.at(j)).trimmed();
      attr->type = dtd->defaultAttrType;
      tag->addAttribute(attr);
      delete attr;
    }
    tagList->insert(tag->name(caseSensitive), tag);  //append the tag to the list for this DTD
  }
  dtd->tagsList = tagList;


  /**** Code for the new parser *****/

  grp.changeGroup("Parsing rules");
  //Which DTD can be present in this one?
  dtd->insideDTDs = grp.readEntry("MayContain", QStringList());
  for (int i = 0; i < dtd->insideDTDs.count(); i++)
  {
    dtd->insideDTDs[i] = dtd->insideDTDs[i].trimmed().toLower();
  }
  bool appendCommonRules = grp.readEntry("AppendCommonSpecialAreas", true);
  //Read the special areas and area names
  QString rxStr = "";
  if (dtd->family == DTDStruct::Xml && appendCommonRules)
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
  QStringList specialAreasList = grp.readEntry("SpecialAreas", QStringList());
  QStringList specialAreaNameList = grp.readEntry("SpecialAreaNames", QStringList());
  QStringList tmpStrList;
  for (int i = 0; i < specialAreasList.count(); i++)
  {
    if (!specialAreasList[i].trimmed().isEmpty())
    {
      tmpStrList = specialAreasList[i].trimmed().split(" ");
      tmpStr = tmpStrList[0].trimmed();
      rxStr.append(QuantaCommon::makeRxCompatible(tmpStr) + '|');
      dtd->specialAreas[tmpStr] = tmpStrList[1].trimmed();
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
  tmpStrList = grp.readEntry("SpecialTags", QStringList());
  for (int i = 0; i < tmpStrList.count(); i++)
  {
    tmpStr = tmpStrList[i].trimmed();
    int pos = tmpStr.indexOf('(');
    dtd->specialTags[tmpStr.left(pos).trimmed()] = tmpStr.mid(pos+1, tmpStr.lastIndexOf(')')-pos-1).trimmed();
  }

  //static const QString quotationStr = "\\\\\"|\\\\'";
  rxStr = "\\\\\"|\\\\'|";
  QStringList commentsList = grp.readEntry("Comments", QStringList());
  if (dtd->family == DTDStruct::Xml && appendCommonRules)
    commentsList.append("<!-- -->");
  QString tmpStr2;
  for (int i = 0; i < commentsList.count(); i++)
  {
    tmpStrList = commentsList[i].trimmed().split(" ");
    tmpStr = tmpStrList[0].trimmed();
    rxStr += QuantaCommon::makeRxCompatible(tmpStr);
    rxStr += '|';
    tmpStr2 = tmpStrList[1].trimmed();
    if (tmpStr2 == "EOL")
        tmpStr2 = '\n';
    dtd->comments[tmpStr] = tmpStr2;
  }
  dtd->commentsStartRx.setPattern(rxStr.left(rxStr.length()-1));

  /**** End of code for the new parser *****/

  //read the definition of a structure, and the structure keywords
  QStringList structKeywords = grp.readEntry("StructKeywords", QStringList(), ',');
  if (structKeywords.count() !=0 )
  {
      tmpStr = "\\b(";
      for (int i = 0; i < structKeywords.count(); i++)
      {
        tmpStr += structKeywords[i].trimmed()+'|';
      }
      tmpStr.truncate(tmpStr.length()-1);
      tmpStr += ")\\b";
  } else
  {
    tmpStr = "\\b[\\d\\S\\w]+\\b";
  }
  dtd->structKeywordsRx.setPattern(tmpStr);

  structKeywords = grp.readEntry("LocalScopeKeywords", QStringList(),',');
  if (structKeywords.count() !=0 )
  {
      tmpStr = "\\b(";
      for (int i = 0; i < structKeywords.count(); i++)
      {
        tmpStr += structKeywords[i].trimmed()+'|';
      }
      tmpStr.truncate(tmpStr.length()-1);
      tmpStr += ")\\b";
  } else
  {
    tmpStr = "\\b[\\d\\S\\w]+\\b";
  }
  dtd->localScopeKeywordsRx.setPattern(tmpStr);

  dtd->structRx.setPattern(grp.readEntry("StructRx","\\{|\\}").trimmed());
  dtd->structBeginStr = grp.readEntry("StructBeginStr","{").trimmed();
  dtd->structEndStr = grp.readEntry("StructEndStr","}").trimmed();


  grp.changeGroup("Extra rules");
  dtd->minusAllowedInWord = grp.readEntry("MinusAllowedInWord", false);
  tmpStr = grp.readEntry("TagAutoCompleteAfter", "<").trimmed();
  if (tmpStr.toUpper() == "NONE")
      dtd->tagAutoCompleteAfter = '\0';
  else
  if (tmpStr.toUpper() == "ALWAYS")
      dtd->tagAutoCompleteAfter = '\1';
  else
      dtd->tagAutoCompleteAfter = tmpStr.at(0);
  tmpStr = grp.readEntry("AttributeAutoCompleteAfter","(").trimmed();
  if (tmpStr.isEmpty())
  {
    dtd->attributeSeparator = (dtd->family == DTDStruct::Xml) ? '\"' : ',';
  } else
    dtd->attributeSeparator = tmpStr.at(0);

  tmpStr = grp.readEntry("TagSeparator").trimmed();
  if (tmpStr.isEmpty())
  {
    dtd->tagSeparator = dtd->attributeSeparator;
  } else
  {
    dtd->tagSeparator = tmpStr.at(0);
  }
  dtd->booleanAttributes = grp.readEntry("BooleanAttributes","extended");
  dtd->booleanTrue = grp.readEntry("BooleanTrue","true");
  dtd->booleanFalse = grp.readEntry("BooleanFalse","false");
  dtd->singleTagStyle = grp.readEntry("Single Tag Style", "xml").toLower();
  dtd->variableGroupIndex = grp.readEntry("VariableGroupIndex", 0) - 1;
  dtd->functionGroupIndex = grp.readEntry("FunctionGroupIndex", 0) - 1;
  dtd->classGroupIndex = grp.readEntry("ClassGroupIndex", 0) - 1;
  if (dtd->classGroupIndex != -1)
  {
    tmpStr = grp.readEntry("MemberAutoCompleteAfter").trimmed();
    dtd->memberAutoCompleteAfter.setPattern(tmpStr);
  }
  dtd->objectGroupIndex = grp.readEntry("ObjectGroupIndex", 0) - 1;

  //read the definition of different structure groups, like links, images, functions
  //classes, etc.
  uint structGroupsCount = grp.readEntry("StructGroupsCount", 0);
  if (structGroupsCount > MAX_STRUCTGROUPSCOUNT)
      structGroupsCount = MAX_STRUCTGROUPSCOUNT; //max. 10 groups

  if (dtd->family == DTDStruct::Script)
  {
      StructTreeGroup group;
      QRegExp attrRx("\\([^\\)]*\\)");
      QString tagStr;
      for (uint index = 1; index <= structGroupsCount; index++)
      {
        grp.changeGroup(QString("StructGroup_%1").arg(index));
        //new code
        group.name = grp.readEntry("Name").trimmed();
        group.noName = grp.readEntry("No_Name").trimmed();
        group.icon = grp.readEntry("Icon").trimmed();
        tmpStr = grp.readEntry("DefinitionRx").trimmed();
        group.definitionRx.setPattern(tmpStr);
        tmpStr = grp.readEntry("UsageRx").trimmed();
        group.usageRx.setPattern(tmpStr);
        tmpStr = grp.readEntry("TypeRx").trimmed();
        group.typeRx.setPattern(tmpStr);
        group.hasDefinitionRx = !group.definitionRx.pattern().isEmpty();
        group.isMinimalDefinitionRx = grp.readEntry("DefinitionRx_Minimal", false);
        group.appendToTags = grp.readEntry("AppendToTags", false);
        group.parentGroup = grp.readEntry("ParentGroup").trimmed();
        tagStr = grp.readEntry("TagType", "Text").trimmed();
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
        tmpStr = grp.readEntry("AutoCompleteAfter").trimmed();
        group.autoCompleteAfterRx.setPattern(tmpStr);
        tmpStr = grp.readEntry("RemoveFromAutoCompleteWord").trimmed();
        group.removeFromAutoCompleteWordRx.setPattern(tmpStr);
        group.hasFileName = grp.readEntry("HasFileName", false);
        group.parseFile = grp.readEntry("ParseFile", false);
        tmpStr = grp.readEntry("FileNameRx").trimmed();
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
        grp.changeGroup(QString("StructGroup_%1").arg(index));
        group.name = grp.readEntry("Name").trimmed();
        group.noName = grp.readEntry("No_Name").trimmed();
        group.icon = grp.readEntry("Icon").trimmed();
        group.appendToTags = grp.readEntry("AppendToTags", false);
        group.parentGroup = grp.readEntry("ParentGroup").trimmed();
        QString tagStr = grp.readEntry("Tag").trimmed();
        if (!tagStr.isEmpty())
        {
          attrRx.indexIn(tagStr);
          tmpStr = attrRx.cap();
          tmpStrList = tmpStr.mid(1, tmpStr.length()-2).split(',');
          tagName = tagStr.left(tagStr.indexOf('(')).toLower();
          group.attributes.clear();
          for (int i = 0; i < tmpStrList.count(); i++)
            group.attributes += tmpStrList[i].trimmed();
          group.hasFileName = grp.readEntry("HasFileName", false);
          tmpStr = grp.readEntry("FileNameRx").trimmed();
          group.fileNameRx.setPattern(tmpStr);
          dtd->xmlStructTreeGroups.insert(tagName, group);
        }
      }
    }

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
    QHashIterator<QString, QTag*> tag_it(*(parent->tagsList));
    while (tag_it.hasNext())
    {
      tag_it.next();
      QTag *tag = tag_it.value();
      QString searchForTag = tag->name(dtd->caseSensitive);
      if (!dtd->tagsList->contains(searchForTag))
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
      specialAreaStartRxStr += '|';
  for (int i = 0; i < dtd->insideDTDs.count(); i++)
  {
    const DTDStruct *insideDTD = m_dict->value(dtd->insideDTDs[i]);  // search but don't load
    if (!insideDTD)
        insideDTD = m_dict->value(getDTDNameFromNickName(dtd->insideDTDs[i]));   // search but don't load
    if (insideDTD)
    {
      for (mapIt = insideDTD->definitionAreas.begin(); mapIt != insideDTD->definitionAreas.end(); ++mapIt)
      {
        QString tmpStr = mapIt.key();
        dtd->specialAreas[tmpStr] = mapIt.value();
        dtd->specialAreaNames[tmpStr] = dtd->insideDTDs[i];
        specialAreaStartRxStr.append("(?:" +  QuantaCommon::makeRxCompatible(tmpStr) + ")|");
      }

      for (mapIt = insideDTD->definitionTags.begin(); mapIt != insideDTD->definitionTags.end(); ++mapIt)
      {
        dtd->specialTags[mapIt.key()] = mapIt.value();
      }
    }
  dtd->specialAreaStartRx.setPattern(specialAreaStartRxStr.left(specialAreaStartRxStr.length() - 1));
  };
}



/** Reads the tags for the tag files. Returns the number of read tags. */
uint DTDs::readTagFile(const QString& fileName, DTDStruct* parentDTD, QTagList *tagList)
{
//  kDebug(24000) << "dtds::readTagFile:" << fileName;
 QFile f(fileName);
 if (! f.exists())
   kError() << "dtds::readTagFile file does not exist:" << fileName << endl;
 else
 {
   bool result = f.open( IO_ReadOnly );
   if (! result)
     kError() << "dtds::readTagFile unable to open:" << fileName
                                << " Status: " << f.error() << endl;
 }
 QString errorMsg;
 int errorLine, errorCol;
 if (!m_doc->setContent( &f, &errorMsg, &errorLine, &errorCol ))
 {
//   emit hideSplash();
   KMessageBox::error(0L, i18n("<qt>The DTD tag file %1 is not valid.<br />The error message is: <i>%2 in line %3, column %4.</i></qt>", fileName, errorMsg, errorLine, errorCol),
   i18n("Invalid Tag File"));
   kWarning() << fileName << ": " << errorMsg << ": " << errorLine << "," << errorCol;
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
    if (common && parentDTD)
    {
      QString groupName = e.attribute("name");
      AttributeList *attrs = tag->attributes();
      AttributeList *commonAttrList = new AttributeList;      //no need to delete it
      *commonAttrList = *attrs;
      //delete tag;
      parentDTD->commonAttrs->insert(groupName, commonAttrList);
    } else
    {
      tagList->insert(tag->name((!parentDTD || parentDTD->caseSensitive)), tag);  //append the tag to the list for this DTD
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
 if ((tmpStr != "1" && tmpStr != "yes") ) //in case of common tags, we are not interested in these options
 {
    if (tag->parentDTD)
    {
      QHashIterator<QString, AttributeList*> it(*(tag->parentDTD->commonAttrs));
      while (it.hasNext())
      {
        it.next();
        QString lookForAttr = "has" + QString(it.key()).trimmed();
        tmpStr = el.attribute(lookForAttr);
        if (tmpStr == "1" || tmpStr == "yes")
        {
          tag->commonGroups += QString(it.key()).trimmed();
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
      tag->comment = " [" + i18n(tag->comment.toUtf8()) + "] ";
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
            if (tag->parentDTD && !tag->parentDTD->caseSensitive)
                childTag = childTag.toUpper();
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
         if (tag->parentDTD && !tag->parentDTD->caseSensitive)
            stopTag = stopTag.toUpper();
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

     if (tag->parentDTD)
      attr->type = el.attribute("type",tag->parentDTD->defaultAttrType);
     else
      attr->type = el.attribute("type", "input");
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
        attrList = attrList + '[' + attr->type + ' ' + attr->name + "], ";
      } else
      {
        attrList = attrList + attr->type + ' ' + attr->name + ", ";
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
  KUrl url = KFileDialog::getOpenUrl(KUrl(), "*.dtd", 0L);
  if (!url.isEmpty())
  {
    DTDParser dtdParser(url, KGlobal::dirs()->saveLocation("data") + Helper::resourceDir() + "dtep");
    if (dtdParser.parse())
    {
      QString dirName = dtdParser.dirName();
      KConfig dtdcfg(dirName + m_rcFilename);
      KConfigGroup grp(&dtdcfg, "General");
      QString dtdName = grp.readEntry("Name");
      QString nickName = grp.readEntry("NickName", dtdName);
      DTDStruct * dtd = m_dict->value(dtdName) ;
      if (dtd &&
          KMessageBox::warningYesNo(0L, i18n("<qt>Do you want to replace the existing <b>%1</b> DTD?</qt>", nickName)) == KMessageBox::No)
      {
        return;
      }
      removeDTD(dtd);
      if (readTagDir(dirName))
      {
/*FIXME          QString family = grp.readEntry("Family", "1");
          Document *w = ViewManager::ref()->activeDocument();
          if (family == "1" && w &&
              KMessageBox::questionYesNo(0L, i18n("<qt>Use the newly loaded <b>%1</b> DTD for the current document?</qt>",nickName), i18n("Change DTD")) == KMessageBox::Yes)
          {
            w->setDTDIdentifier(dtdName);
            emit loadToolbarForDTD(w->getDTDIdentifier());
            emit forceReparse();
          }*/
      }
    }
  }
}

void DTDs::slotLoadDTEP(const QString &_dirName, bool askForAutoload)
{
  QString dirName = _dirName;
  if (!dirName.endsWith('/'))
    dirName += '/';
  KConfig dtdcfg(dirName + m_rcFilename);
  KConfigGroup grp(&dtdcfg, "General");
  QString dtdName = grp.readEntry("Name");
  QString nickName = grp.readEntry("NickName", dtdName);
  DTDStruct * dtd = m_dict->value(dtdName) ;
  if ( dtd &&
      KMessageBox::warningYesNo(0L, i18n("<qt>Do you want to replace the existing <b>%1</b> DTD?</qt>", nickName)) == KMessageBox::No)
  {
    return;
  }
  removeDTD(dtd);
  if (!readTagDir(dirName))
  {
    KMessageBox::error(0L, i18n("<qt>Cannot read the DTEP from <b>%1</b>. Check that the folder contains a valid DTEP (<i>description.rc and *.tag files</i>).</qt>", dirName), i18n("Error Loading DTEP"));
  } else
  {
    QString family = grp.readEntry("Family", "1");
    if (askForAutoload && KMessageBox::questionYesNo(0L, i18n("<qt>Autoload the <b>%1</b> DTD in the feature?</qt>", nickName)) == KMessageBox::Yes)
    {
      KUrl src;
      src.setPath(dirName);
      KUrl target;
      QString destDir = KGlobal::dirs()->saveLocation("data") + Helper::resourceDir() + "dtep/";
      target.setPath(destDir + src.fileName());
      KIO::copy( src, target, KIO::HideProgressInfo); //don't care about the result
    }
/*FIXME    Document *w = ViewManager::ref()->activeDocument();
    if (family == "1" && w &&
        KMessageBox::questionYesNo(0L, i18n("<qt>Use the newly loaded <b>%1</b> DTD for the current document?</qt>").arg(nickName), i18n("Change DTD")) == KMessageBox::Yes)
    {
      w->setDTDIdentifier(dtdName);
      emit loadToolbarForDTD(w->getDTDIdentifier());
      emit forceReparse();
    }*/
  }
}


/** Returns the DTD name (identifier) corresponding to the DTD's nickname */
QString DTDs::getDTDNameFromNickName(const QString& nickName)
{
  QHashIterator<QString, DTDStruct*> it(*m_dict);
  while (it.hasNext())
  {
    it.next();
    DTDStruct *dtd = it.value();
    if (dtd->nickName.toLower() == nickName.toLower())
    {
      return dtd->name;
    }
  }
  return nickName;
}

/** returns the known nick names */
QStringList DTDs::nickNameList(bool topLevelOnly)
{
  QStringList nickList;
  QHashIterator<QString, DTDStruct*> it(*m_dict);
  while (it.hasNext())
  {
    it.next();
    DTDStruct *dtd = it.value();
    if (!topLevelOnly || dtd->toplevel)
    {
      nickList << dtd->nickName;
    }
  }
  nickList.sort();
  return nickList;
}


/** returns the known names */
QStringList DTDs::nameList(bool topLevelOnly)
{
  QStringList nameList;
  QHashIterator<QString, DTDStruct*> it(*m_dict);
  while (it.hasNext())
  {
    it.next();
    DTDStruct *dtd = it.value();
    if (!topLevelOnly || dtd->toplevel)
    {
      nameList << dtd->name;
    }
  }
  nameList.sort();
  return nameList;
}

QStringList DTDs::fileNameList(bool topLevelOnly)
{
  QStringList nameList;
  QHashIterator<QString, DTDStruct*> it(*m_dict);
  while (it.hasNext())
  {
    it.next();
    DTDStruct *dtd = it.value();
    if (!topLevelOnly || dtd->toplevel)
    {
      nameList << (dtd->name + '|' + dtd->fileName);
    }
  }
  return nameList;
}


/** find a DTD for a given mimetype */
const DTDStruct * DTDs::DTDfromMimeType(const QString &mimetype)
{
  QHashIterator<QString, DTDStruct*> it(*m_dict);
  while (it.hasNext())
  {
    it.next();
    DTDStruct *dtd = it.value();
    if (dtd->toplevel && dtd->mimeTypes.contains(mimetype))
    {
      return dtd;
    }
  }
  return 0L;
}


#include "dtds.moc"
