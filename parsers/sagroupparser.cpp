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
//qt includes
#include <qtimer.h>
#include <qvaluelist.h>

//kde includes
#include <kdebug.h>
#include <kdirwatch.h>
#include <kurl.h>

//own includes
#include "sagroupparser.h"
#include "saparser.h"
#include "document.h"
#include "node.h"
#include "parsercommon.h"
#include "qextfileinfo.h"
#include "quantacommon.h"
#include "resource.h"
#include "tag.h"

extern GroupElementMapList globalGroupMap;

SAGroupParser::SAGroupParser(SAParser *parent, Node *startNode, Node *endNode, bool synchronous, bool parsingLastNode, bool paringLastGroup)
{
  g_node = startNode;
  g_endNode = endNode;
  m_synchronous = synchronous;
  m_lastGroupParsed = paringLastGroup;
  m_parsingLastNode = parsingLastNode;
  m_parent = parent;
  m_count = 0;
}

void SAGroupParser::slotParseForScriptGroup()
{
#ifdef DEBUG_PARSER
  //kdDebug(24001) << "slotParseForScriptGroup. Synch: " << m_synchronous << endl;
#endif
  if (!m_parent->parsingEnabled() || (!baseNode && !m_synchronous))
{
#ifdef DEBUG_PARSER
    kdDebug(24001) << "slotParseForScriptGroup aborted. Synch: " << m_synchronous << endl;
#endif
    return;
}

  if (g_node && g_node != g_endNode )
{
  if (g_node->tag && (g_node->tag->type == Tag::Text || g_node->tag->type == Tag::ScriptStructureBegin))
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
      //  kdDebug(24001) << "Calling slotParseForScriptGroup from slotParseForScriptGroup." << endl;
#endif
      QTimer::singleShot(0, this, SLOT(slotParseForScriptGroup()));
  }
} else
{
#ifdef DEBUG_PARSER
    kdDebug(24001) << "slotParseForScriptGroup done." << endl;
#endif
    if (m_lastGroupParsed && m_parsingLastNode && !m_synchronous)
{
  if (m_lastGroupParsed)
  {
#ifdef DEBUG_PARSER
        kdDebug(24000) << "Calling cleanGroups from SAGroupParser::slotParseForScriptGroup" << endl;
        kdDebug(24000) << m_count << " GroupElement created." << endl;
#endif
        emit cleanGroups();
        m_lastGroupParsed = false;
  }
#ifdef DEBUG_PARSER
      kdDebug(24001) << "Emitting rebuildStructureTree from slotParseForScriptGroup." << endl;
#endif
      emit rebuildStructureTree(true);
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
  GroupElementList* groupElementList;
  KURL baseURL = QExtFileInfo::path(m_parent->write()->url());
  QString str = node->tag->cleanStr;
  QString tagStr = node->tag->tagStr();
  const DTDStruct* dtd = node->tag->dtd();
  node->tag->beginPos(bl, bc);
  QValueList<StructTreeGroup>::ConstIterator it;
  for (it = dtd->structTreeGroups.begin(); it != dtd->structTreeGroups.end(); ++it)
  {
    group = *it;
    if (!group.hasDefinitionRx ||
         node->tag->type == Tag::XmlTag ||
         node->tag->type == Tag::XmlTagEnd ||
         node->tag->type == Tag::Comment ||
         node->tag->type == Tag::Empty ||
         ( group.tagType != Tag::Text && node->tag->type != group.tagType)
       )
      continue;
    pos = 0;
    group.definitionRx.setMinimal(group.isMinimalDefinitionRx);
    while (pos != -1)
    {
      pos = group.definitionRx.search(str, pos);
      if (pos != -1) //the Node is part of this group
      {
        title = tagStr.mid(pos, group.definitionRx.matchedLength());
        node->tag->beginPos(bl, bc);
        tmpStr = tagStr.left(pos);
        int newLines = tmpStr.contains('\n');
        bl += newLines;
        int l = tmpStr.findRev('\n'); //the last EOL
        bc = (l == -1) ? bc + pos : pos - l - 1;
        newLines = title.contains('\n');
        l = title.length();
        el = bl + newLines;
        ec = (newLines > 0) ? l - title.findRev('\n') : bc + l - 1;
        pos += l;
        AreaStruct area(bl, bc, el, ec);
        //get the list of elements which are present in this group and
        //have the same title. For example get the list of all group
        //element which are variable and the matched string was "$i"
        int cap1Pos = str.find(group.definitionRx.cap(1));
        QString s = tagStr.mid(cap1Pos, group.definitionRx.cap(1).length());
        groupElementList = & (globalGroupMap[group.name + "|" + s]);
        //Create a new tag which point to the exact location of the matched string.
        //For example when the group defined PHP variables it
        //points to "$i" in a node which originally contained "print $i + 1"
        Tag *newTag = new Tag(*node->tag);
        newTag->setTagPosition(area);
        newTag->setStr(title);
        newTag->name = s;

        if (dtd->name == "PHP" && group.name == "Functions")
        {
          QTag *qTag = m_parent->write()->userTagList.find(s.lower());
          if (!qTag)
          {
            QTag *qTag = new QTag();
            qTag->setName(s.left(s.find('(')));
            m_parent->write()->userTagList.insert(s.lower(), qTag);
          }
        }

        groupElement = new GroupElement;
        groupElement->deleted = false;
        groupElement->tag = newTag;
        groupElement->node = node;
        groupElement->group = const_cast<StructTreeGroup*>(&(*it));
        //Find out if the current node is inside a script structure or not.
        //This is used to define local/global scope of the group elements.
        Node *tmpNode = node;
        while (tmpNode && tmpNode->tag->dtd() == dtd && tmpNode->tag->type != Tag::ScriptStructureBegin)
        {
          tmpNode = tmpNode->parent;
        }
        if (tmpNode && tmpNode->tag->type == Tag::ScriptStructureBegin)
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
          if ( tmpNode->tag->type == Tag::ScriptStructureBegin && tmpNode->tag->dtd()->localScopeKeywordsRx.search(tmpNode->tag->cleanStr) != -1)
          {
            groupElement->global = false;
            groupElement->parentNode = tmpNode;
            break;
          }
          tmpNode = tmpNode->parent;
        }
        if (!group.typeRx.pattern().isEmpty() && group.typeRx.search(title) != -1)
          groupElement->type = group.typeRx.cap(1);
#ifdef DEBUG_PARSER
       kdDebug(24001) << "GroupElement created: " <<groupElement << " "<< groupElement->tag->area().bLine << " " << groupElement->tag->area().bCol << " "<< groupElement->tag->area().eLine << " "<< groupElement->tag->area().eCol << " " << groupElement->tag->tagStr() << " " << groupElement->type << endl;
#endif
        //store the pointer to the group element list where this node was put
        //used to clear the corresponding entry from the group element lists
        //when the node is deleted (eg. $i was deleted, so it should be deleted
        //from the "variables | $i" group element list as well)
        node->m_groupElements.append(groupElement);
        groupElementList->append(groupElement);
        m_count++;
        //if a filename may be present in the title, extract it
        if (group.hasFileName && group.parseFile)
        {
          s.remove(group.fileNameRx);
          KURL url;
          QuantaCommon::setUrl(url, s.stripWhiteSpace());
          url = QExtFileInfo::toAbsolute(url, baseURL);
          ParserCommon::includedFiles += url.path();
          ParserCommon::includedFilesDTD.append(dtd);
          ParserCommon::includeWatch->addFile(url.path());
        }
      }
    }
  }
#ifdef DEBUG_PARSER
 if (t.elapsed() > 10)
     kdDebug(24001) << "Done: " << t.elapsed() << endl;
#endif
}

#include "sagroupparser.moc"
