/***************************************************************************
                          sagroupparser.cpp  -  description
                             -------------------
    begin                : Wed Feb 11 2004
    copyright            : (C) 2004 Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
  ***************************************************************************/
//own includes
#include "sagroupparser.h"
#include "groupelement.h"
#include "groupstorage.h"
#include "saparser.h"
#include "node.h"
#include "parsercommon.h"
#include "parseresult.h"
#include "extfileinfo.h"
#include "quantacommon.h"
#include "tag.h"
#include "dtdstruct.h"
#include "editorsource.h"

//qt includes
#include <qtimer.h>
#include <qlist.h>

//kde includes
#include <kdebug.h>
#include <kdirwatch.h>
#include <kurl.h>

SAGroupParser::SAGroupParser(SAParser *parent, ParseResult *base, EditorSource *source, Node *startNode, Node *endNode, bool synchronous, bool parsingLastNode, bool paringLastGroup)
{
  g_node = startNode;
  g_endNode = endNode;
  m_synchronous = synchronous;
  m_lastGroupParsed = paringLastGroup;
  m_parsingLastNode = parsingLastNode;
  m_parent = parent;
  m_base = base;
  m_source= source;
  m_count = 0;
  m_parseForGroupTimer = new QTimer(this);
  connect(m_parseForGroupTimer, SIGNAL(timeout()), this, SLOT(slotParseForScriptGroup()));
}

void SAGroupParser::slotParseForScriptGroup()
{
#ifdef DEBUG_PARSER
    //kDebug(24001) << "slotParseForScriptGroup. Synch: " << m_synchronous << endl;
#endif
  if ((m_parent && !m_parent->parsingEnabled()) || (!m_base->baseNode && !m_synchronous))
  {
#ifdef DEBUG_PARSER
    kDebug(24001) << "slotParseForScriptGroup aborted. Synch: " << m_synchronous << endl;
#endif
    return;
  }

  if (g_node && g_node != g_endNode )
  {
    if (g_node->tag && (g_node->tag->isType(Tag::Text) || g_node->tag->isType(Tag::ScriptStructureBegin)))
      parseForScriptGroup(g_node);
    g_node = g_node->nextSibling();
    if (m_synchronous)
    {
      slotParseForScriptGroup();
      return;
    }
    else
    {
#ifdef DEBUG_PARSER
      //kDebug(24001) << "Calling slotParseForScriptGroup from slotParseForScriptGroup." << endl;
#endif
      m_parseForGroupTimer->start(0, true);
    }
  } else
  {
#ifdef DEBUG_PARSER
    kDebug(24001) << "slotParseForScriptGroup done." << endl;
#endif
    if (m_lastGroupParsed && m_parsingLastNode && !m_synchronous)
    {
      if (m_lastGroupParsed)
      {
#ifdef DEBUG_PARSER
        kDebug(24000) << "Calling cleanGroups from SAGroupParser::slotParseForScriptGroup" << endl;
        kDebug(24001) << m_count << " GroupElement created." << endl;
#endif
        emit cleanGroups();
        m_lastGroupParsed = false;
      }
#ifdef DEBUG_PARSER
        kDebug(24001) << "Emitting groupsParsed from slotParseForScriptGroup." << endl;
#endif
        emit groupsParsed(m_source, m_base);
    }
  }
}

void SAGroupParser::parseForScriptGroup(Node *node)
{
#ifdef DEBUG_PARSER
  QTime t;
  t.start();
#endif

  int bl, bc, el, ec;
  int pos;
  QString title;
  QString tmpStr;
  StructTreeGroup group;
  GroupElement *groupElement;
 // GroupElementList* groupElementList;
  KUrl baseURL = ExtFileInfo::path(m_source->url());
  QString str = node->tag->cleanStr;
  QString tagStr = node->tag->tagStr();
  const DTDStruct* dtd = node->tag->dtd();
  node->tag->beginPos(bl, bc);
  QList<StructTreeGroup>::ConstIterator end(dtd->structTreeGroups.end());
  for (QList<StructTreeGroup>::ConstIterator it = dtd->structTreeGroups.begin(); it != end; ++it)
  {
    group = *it;
    if (!group.hasDefinitionRx ||
         node->tag->isType(Tag::XmlTag) ||
         node->tag->isType(Tag::XmlTagEnd) ||
         node->tag->isType(Tag::Comment) ||
         node->tag->isType(Tag::Empty) ||
         ( group.tagType != Tag::Text && !node->tag->isType(static_cast<Tag::TokenType>(group.tagType)))
       )
      continue;
    pos = 0;
    group.definitionRx.setMinimal(group.isMinimalDefinitionRx);
//    kDebug(24000) << "Analyzing\n\n " << str << endl;
    while (pos != -1)
    {
      pos = group.definitionRx.indexIn(str, pos);
      if (pos != -1) //the Node is part of this group
      {
 //       kDebug(24000) << "Found from\n\n " << str.mid(pos) << endl;
        title = tagStr.mid(pos, group.definitionRx.matchedLength());
        node->tag->beginPos(bl, bc);
        tmpStr = tagStr.left(pos);
        int newLines = tmpStr.count('\n');
        bl += newLines;
        int l = tmpStr.lastIndexOf('\n'); //the last EOL
        bc = (l == -1) ? bc + pos : pos - l - 1;
        newLines = title.count('\n');
        l = title.length();
        el = bl + newLines;
        ec = (newLines > 0) ? l - title.lastIndexOf('\n') : bc + l - 1;
        AreaStruct area(bl, bc, el, ec);
        int cap1Pos = str.indexOf(group.definitionRx.cap(1), pos);
        pos += l;
        QString s = tagStr.mid(cap1Pos, group.definitionRx.cap(1).length());
        //Create a new tag which point to the exact location of the matched string.
        //For example when the group defined PHP variables it
        //points to "$i" in a node which originally contained "print $i + 1"
        Tag *newTag = new Tag(*node->tag);
        newTag->setTagPosition(area);
        newTag->setStr(title);
        newTag->setName(s);

        groupElement = new GroupElement;
        groupElement->deleted = false;
        groupElement->tag = newTag;
        groupElement->node = node;
        groupElement->group = const_cast<StructTreeGroup*>(&(*it));
        //Find out if the current node is inside a script structure or not.
        //This is used to define local/global scope of the group elements.
        Node *tmpNode = node;
        while (tmpNode && tmpNode->tag->dtd() == dtd && !tmpNode->tag->isType(Tag::ScriptStructureBegin))
        {
          tmpNode = tmpNode->parent;
        }
        if (tmpNode && tmpNode->tag->isType(Tag::ScriptStructureBegin))
        {
          groupElement->parentNode = tmpNode;
        } else
        {
          groupElement->parentNode = 0L;
        }
        groupElement->global = true;
        tmpNode = node->parent;
        while (tmpNode && tmpNode->tag->dtd() == dtd)
        {
          if ( tmpNode->tag->isType(Tag::ScriptStructureBegin) && tmpNode->tag->dtd()->localScopeKeywordsRx.indexIn(tmpNode->tag->cleanStr) != -1)
          {
            groupElement->global = false;
            groupElement->parentNode = tmpNode;
            break;
          }
          tmpNode = tmpNode->parent;
        }

        if (group.appendToTags)
        {
          QTag *qTag = new QTag();
          qTag->setName(s.left(s.indexOf('(')));
          qTag->className = "";
          if (groupElement->parentNode)
          {
            const GroupElementList elements = m_base->groupStorage->elementsForNode(groupElement->parentNode);
            GroupElementList::ConstIterator end(elements.constEnd());
            for (GroupElementList::ConstIterator it2 = elements.constBegin(); it2 != end; ++it2)
            {
              if ((*it2)->group->name == group.parentGroup)
              {
                qTag->className = (*it2)->tag->name();
                break;
              }
            }
          }
          m_base->userTags.insert(s.toLower(), qTag);
        }


        if (!group.typeRx.pattern().isEmpty() && group.typeRx.indexIn(title) != -1)
          groupElement->type = group.typeRx.cap(1);
#ifdef DEBUG_PARSER
       kDebug(24001) << "GroupElement created: " <<groupElement << " "<< groupElement->tag->area().bLine() << " " << groupElement->tag->area().bCol() << " "<< groupElement->tag->area().eLine() << " "<< groupElement->tag->area().eCol() << " " << groupElement->tag->tagStr() << " " << groupElement->type << endl;
#endif
        //store the pointer to the group element list where this node was put
        //used to clear the corresponding entry from the group element lists
        //when the node is deleted (eg. $i was deleted, so it should be deleted
        //from the "variables | $i" group element list as well)
/*        groupElementList = & (globalGroupMap[group.name + "|" + s]);
        groupElementList->append(groupElement);
        node->m_groupElements.append(groupElement);*/

        m_base->groupStorage->insert(group.name, s, groupElement);

        m_count++;
        //if a filename may be present in the title, extract it
        if (group.hasFileName && group.parseFile)
        {
          s.remove(group.fileNameRx);
          KUrl url;
          ExtFileInfo::setUrl(url, s.trimmed());
          url = ExtFileInfo::toAbsolute(url, baseURL);
          ParserCommon::includedFiles += url.path();
          ParserCommon::includedFilesDTD.append(dtd);
          ParserCommon::includeWatch->addFile(url.path());
        }
      }
    }
  }
#ifdef DEBUG_PARSER
 if (t.elapsed() > 10)
     kDebug(24001) << "Done: " << t.elapsed() << endl;
#endif
}

#include "sagroupparser.moc"
