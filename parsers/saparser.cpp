/***************************************************************************
                          saparser.cpp  -  description
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

//kde includes
#include <kdebug.h>
#include <ktexteditor/editinterface.h>

//own includes
#include "saparser.h"
#include "sagroupparser.h"
#include "document.h"
#include "dtds.h"
#include "node.h"
#include "parsercommon.h"
#include "qtag.h"
#include "quantacommon.h"
#include "resource.h"

//#define DEBUG_PARSER

SAParser::SAParser()
{
  m_write = 0L;
  m_baseNode = 0L;
  m_currentNode = 0L;
  m_quotesRx = QRegExp("\"|'");
  m_specialInsideXml = false;
  m_parsingEnabled = true;
  m_synchronous = true;
  m_parseOneLineTimer = new QTimer(this);
  connect(m_parseOneLineTimer, SIGNAL(timeout()), this, SLOT(slotParseOneLine()));
  m_parseInDetailTimer = new QTimer(this);
  connect(m_parseInDetailTimer, SIGNAL(timeout()), this, SLOT(slotParseNodeInDetail()));
}

SAParser::~SAParser()
{
}

void SAParser::init(Node *node, Document* write)
{
  m_baseNode = node;
  m_write = write;
  m_dtd = write->defaultDTD();
}


bool SAParser::slotParseOneLine()
{
  if ((!m_parsingEnabled || !baseNode) && !m_synchronous)
  {
#ifdef DEBUG_PARSER
    kdDebug(24001) << "slotParseOneLine - interrupted" << endl;
#endif
    return false;
  }
  if (s_line <= s_endLine)
  {
    s_contextFound = false;
    switch (s_currentContext.type)
    {
      case Group:
      case Text: {
        int areaEndPos = -1;
        int quotedStringPos = -1;
        int commentPos = -1;
        int groupKeywordPos = -1;
        if (s_searchContent || (s_parentNode && s_parentNode->tag->dtd()->family == Xml))
        {
          //search for different s_contexts
          if (s_searchContent) //search for quoted strings, comments, groups only in non-comment special areas
          {
            quotedStringPos = s_textLine.find(m_quotesRx, s_col); //quoted strings
            s_searchedString = s_textLine.left(quotedStringPos);
            commentPos = s_searchedString.find(s_dtd->commentsStartRx, s_col); //comments
            s_searchedString = s_textLine.left(commentPos);
            if (s_fullParse)
              groupKeywordPos = s_searchedString.find(s_dtd->structRx, s_col); //groups, like { }
          } else
            s_searchedString = s_textLine;
          int specialAreaPos = -1;
          if (s_searchForSpecialAreas)  //special area inside special area
          {
            s_searchedString = s_textLine.left(groupKeywordPos);
            specialAreaPos = s_searchedString.find(s_dtd->specialAreaStartRx, s_col);
          }
          if (s_searchForAreaEnd) //the end of the special area
          {
            s_searchedString = s_textLine.left(specialAreaPos);
            areaEndPos = s_searchedString.find(s_areaEndString, s_col);
          } else
          if (s_searchForForcedAreaEnd) //the end of the special area if a forcing string was specified
          {
            s_searchedString = s_textLine.left(specialAreaPos);
            areaEndPos = s_searchedString.find(s_forcedAreaRx, s_col);
            if (areaEndPos != -1)
              s_areaEndString = s_forcedAreaRx.cap();
          }
          //check which s_context was found first
          if (quotedStringPos != -1)  //is it a quoted string?
          {
            if ( (quotedStringPos < commentPos || commentPos == -1) &&
                (quotedStringPos < groupKeywordPos || groupKeywordPos == -1) &&
                (quotedStringPos < specialAreaPos || specialAreaPos == -1) &&
                (quotedStringPos < areaEndPos || areaEndPos == -1) )
            {
              s_context.type = QuotedString;
              s_context.area.bCol = quotedStringPos;
              s_context.startString = s_textLine.mid(quotedStringPos, 1);
              s_contextFound = true;
            }
          }
          if (!s_contextFound && commentPos != -1)  //is it a comment?
          {
            if ( (commentPos < groupKeywordPos || groupKeywordPos == -1) &&
                (commentPos < specialAreaPos || specialAreaPos == -1) &&
                (commentPos < areaEndPos || areaEndPos == -1) )
            {
              s_context.type = Comment;
              s_context.area.bCol = commentPos;
              s_context.startString = s_dtd->commentsStartRx.cap();
              s_contextFound = true;
            }
          }
          if (!s_contextFound && groupKeywordPos != -1) //is it a group structure?
          {
            if  ( (groupKeywordPos < specialAreaPos || specialAreaPos == -1) &&
                  (groupKeywordPos < areaEndPos || areaEndPos == -1) )
            {
              QString foundText = s_dtd->structRx.cap();
              if (foundText == s_dtd->structBeginStr)
              {
                s_context.type = Group;
                s_context.area.bCol = groupKeywordPos;
                s_context.startString = foundText;
                //create a text node until the struct. beginning
                s_currentContext.area.eLine = s_line;
                s_currentContext.area.eCol = groupKeywordPos + foundText.length() - 1;
                if (s_currentNode &&
                    (s_currentNode->tag->type == Tag::Text ||
                     s_currentNode->tag->type == Tag::Empty) )
                  ParserCommon::appendAreaToTextNode(m_write, s_currentContext.area, s_currentNode);
                else
                  s_currentNode = ParserCommon::createTextNode(m_write, s_currentNode, s_line, s_currentContext.area.eCol + 1, s_currentContext.parentNode);

                s_currentNode->tag->type = Tag::ScriptStructureBegin;
                s_currentNode->tag->single = false;
                s_currentNode->insideSpecial = true;
                s_currentNode->specialInsideXml = m_specialInsideXml;
                s_currentContext.lastNode = s_currentNode;

                s_contextStack.push(s_currentContext);
                s_currentContext.parentNode = s_currentNode;
                s_col = s_context.area.bCol + s_context.startString.length();
                s_currentContext.area.bLine = s_line;
                s_currentContext.area.bCol = s_col;
                s_currentContext.type = Group;
                if (m_synchronous)
                  //slotParseOneLine();
                  return true;
                else
                {
#ifdef DEBUG_PARSER
                  kdDebug(24001) << "Calling slotParseOneLine from parseArea (opening group struct)." << endl;
#endif
                  m_parseOneLineTimer->start(0, true);
                }
                return true;
              } else  //it's a closing group structure element (like "}")
              {
                if (s_currentContext.type != Group)
                {
                  s_col = groupKeywordPos + foundText.length();
                  if (m_synchronous)
                    //slotParseOneLine();
                    return true;
                  else
                  {
#ifdef DEBUG_PARSER
                    kdDebug(24001) << "Calling slotParseOneLine from parseArea (closing group struct)." << endl;
#endif
                    m_parseOneLineTimer->start(0, true);
                  }
                  return true;
                }
                s_currentContext.area.eLine = s_line;
                s_currentContext.area.eCol = groupKeywordPos - 1;
                //kdDebug(24000) << QString("Group Struct s_context: %1, %2, %3, %4").arg( s_currentContext.bLine).arg(s_currentContext.bCol).arg(s_currentContext.eLine).arg(s_currentContext.eCol) << endl;

                if (s_currentNode &&
                    (s_currentNode->tag->type == Tag::Text ||
                     s_currentNode->tag->type == Tag::Empty) )
                  ParserCommon::appendAreaToTextNode(m_write, s_currentContext.area, s_currentNode);
                else
                  s_currentNode = ParserCommon::createTextNode(m_write, s_currentNode, s_line, groupKeywordPos, s_currentContext.parentNode);
                if (s_currentNode)
                {
                  s_currentNode->insideSpecial = true;
                  s_currentNode->specialInsideXml = m_specialInsideXml;
                }
                s_previousContext = s_contextStack.pop();
                s_currentContext.parentNode = s_previousContext.parentNode;
                s_currentContext.lastNode = s_previousContext.lastNode;
                s_currentContext.type = s_previousContext.type;
                s_currentContext.area.bLine = s_line;
                s_currentContext.area.bCol =  groupKeywordPos + foundText.length();
                s_currentContext.area.eLine = s_currentContext.area.eCol = -1;
                s_currentContext.startString = s_previousContext.startString;
                s_col = s_currentContext.area.bCol;

                Tag *tag = new Tag();
                tag->name = foundText;
                tag->setStr(foundText);
                tag->setWrite(m_write);
                tag->setTagPosition(s_line, groupKeywordPos, s_line, s_col - 1);
                tag->setDtd(s_dtd);
                tag->type = Tag::ScriptStructureEnd;
                tag->single = true;
                Node *node = new Node(s_currentContext.parentNode);
                nodeNum++;
                node->tag = tag;
                node->insideSpecial = true;
                node->specialInsideXml = m_specialInsideXml;
                if (s_currentContext.parentNode && !s_currentContext.parentNode->child)
                {
                  s_currentContext.parentNode->child = node;
                }
                else if (s_currentContext.lastNode)
                {
                  node->prev = s_currentContext.lastNode;
                  s_currentContext.lastNode->next = node;
                }
                s_currentNode = node;

                if (m_synchronous)
                  return true;
                else
                {
#ifdef DEBUG_PARSER
                  kdDebug(24001) << "Calling slotParseOneLine from parseArea (group structure)." << endl;
#endif
                  m_parseOneLineTimer->start(0, true);
                }
                return true;
              }
            }
          }
          if (!s_contextFound && specialAreaPos != -1) //is it a special area?
          {
            if (specialAreaPos < areaEndPos || areaEndPos == -1)
            {
              QString foundText = s_dtd->specialAreaStartRx.cap();
              s_currentContext.area.eLine = s_line;
              s_currentContext.area.eCol = specialAreaPos - 1;
              if (s_fullParse)
              {
                if ( s_currentNode &&
                    (s_currentNode->tag->type == Tag::Text ||
                    s_currentNode->tag->type == Tag::Empty) )
                  ParserCommon::appendAreaToTextNode(m_write, s_currentContext.area, s_currentNode);
                else
                  s_currentNode = ParserCommon::createTextNode(m_write, s_currentNode, s_line, specialAreaPos, s_currentContext.parentNode);
                if (s_currentNode)
                {
                  s_currentNode->insideSpecial = true;
                  s_currentNode->specialInsideXml = m_specialInsideXml;
                }
              }
              //create a toplevel node for the included special area
              AreaStruct area(s_line, specialAreaPos, s_line, specialAreaPos + foundText.length() - 1);
              Node *node = ParserCommon::createScriptTagNode(m_write, area, foundText, s_dtd, s_currentContext.parentNode, s_currentNode);
#ifdef DEBUG_PARSER
              kdDebug(24001) << "Parsing a nested area." << endl;
#endif
              AreaStruct area2(s_line, specialAreaPos, s_endLine, s_endCol);
              SAParser *p = new SAParser();
              p->init(m_baseNode, m_write);
              s_currentNode = p->parseArea(area2, foundText, "", node, s_fullParse, true);
              s_line = p->lastParsedLine();
              s_col = p->lastParsedColumn();
              delete p;
              s_currentContext.area.bLine = s_line;
              s_currentContext.area.bCol = s_col + 1;
              s_textLine = ParserCommon::getLine(m_write, s_line, s_endLine, s_endCol);
              s_col++;
              if (m_synchronous)
              {
                return true;
              }
              else
              {
#ifdef DEBUG_PARSER
                kdDebug(24001) << "Calling slotParseOneLine from slotParseOneLine (nested area)." << endl;
#endif
                m_parseOneLineTimer->start(0, true);
                return true;
              }
            }
          }
        } else //when we look only for the area end string
        if (s_searchForAreaEnd)
        {
          areaEndPos = s_textLine.find(s_areaEndString, s_col);
        } else
        if (s_searchForForcedAreaEnd)
        {
          areaEndPos = s_textLine.find(s_forcedAreaRx, s_col);
          if (areaEndPos != -1)
            s_areaEndString = s_forcedAreaRx.cap();
        }

        if (!s_contextFound && areaEndPos != -1) //did we find the end of the special area?
        {
          m_lastParsedLine = s_line;
          m_lastParsedCol = areaEndPos + s_areaEndString.length() - 1;

          s_currentContext.area.eLine = s_line;
          s_currentContext.area.eCol = areaEndPos - 1;
          //Always create a node between the opening and closing special area nodes.
          //This fixes the "commnet loss" bug when editing in VPL and autocompletion
          //for simple special areas like <? a ?>
          if (s_fullParse || !s_parentNode->child)
          {
            if ( s_currentNode &&
                (s_currentNode->tag->type == Tag::Text ||
                s_currentNode->tag->type == Tag::Empty) )
              ParserCommon::appendAreaToTextNode(m_write, s_currentContext.area, s_currentNode);
            else
            {
              s_currentNode = ParserCommon::createTextNode(m_write, s_currentNode, s_line, areaEndPos, s_parentNode);
            }
            if (s_currentNode)
            {
              s_currentNode->insideSpecial = true;
              s_currentNode->specialInsideXml = m_specialInsideXml;
            }
          }
          //kdDebug(24000) << QString("Special area %1 ends at %2, %3").arg(s_dtd->name).arg(s_line).arg(lastCol) << endl;

          //create a closing node for the special area
          Tag *tag = new Tag();
          tag->setTagPosition(s_line, areaEndPos, s_line, m_lastParsedCol);
          tag->parse(s_areaEndString, m_write);
          tag->setDtd(s_dtd);
          tag->type = Tag::XmlTagEnd;
          tag->validXMLTag = false; //FIXME: this is more or less a workaround. We should introduce and handle Tag::ScriptTagEnd
          tag->single = true;
          //at this point s_parentNode = the opening node of the special area (eg. <?)
          //and it should always exist
          Node *node = new Node(s_parentNode->parent);
          nodeNum++;
          s_parentNode->next = node;
          node->prev = s_parentNode;
          node->tag = tag;
          node->closesPrevious = true;

          if (s_fullParse)
          {
            Node *g_node, *g_endNode;
            g_node = s_parentNode->child;
           /* g_endNode = s_currentNode;
            if (g_node && g_node == g_endNode)
              g_endNode = s_parentNode->next;*/
            g_endNode = node;
#ifdef DEBUG_PARSER
            kdDebug(24001) << "Calling slotParseForScriptGroup from slotParseOneLine." << endl;
#endif
//            slotParseForScriptGroup();
            if (!m_synchronous)
            {
              bool parsingLastNode = true;
              Node *n = g_endNode;
              while (n)
              {
                n = n->nextSibling();
                if (n && n->insideSpecial)
                {
                  parsingLastNode = false;
                  break;
                }
              }
              SAGroupParser *groupParser = new SAGroupParser(this, write(), g_node, g_endNode, m_synchronous, parsingLastNode, true);
              connect(groupParser, SIGNAL(rebuildStructureTree(bool)), SIGNAL(rebuildStructureTree(bool)));
              connect(groupParser, SIGNAL(cleanGroups()), SIGNAL(cleanGroups()));
              connect(groupParser, SIGNAL(parsingDone(SAGroupParser*)), SLOT(slotGroupParsingDone(SAGroupParser*)));
              groupParser->slotParseForScriptGroup();
              m_groupParsers.append(groupParser);
            }
          }

          m_lastParsedNode = node;
          s_useReturnVars = true;
          if (!m_synchronous)
          {
#ifdef DEBUG_PARSER
            kdDebug(24001) << "Calling parsingDone from slotParseOneLine (area end found)." << endl;
#endif
            m_lastParsedNode = parsingDone();
          }
          return false; //parsing finished
        }
        if (s_contextFound)
        {
          s_context.area.bLine = s_line;
          s_context.area.eLine = s_context.area.eCol = -1;
          s_context.parentNode = s_currentContext.parentNode;
          s_currentContext.area.eLine = s_context.area.bLine;
          s_currentContext.area.eCol = s_context.area.bCol - 1;
       //   s_currentContext.parentNode = s_parentNode;
          s_contextStack.push(s_currentContext);
          if (s_fullParse)
          {
            if (s_currentNode &&
              (s_currentNode->tag->type == Tag::Text || s_currentNode->tag->type == Tag::Empty) )
            {
              ParserCommon::appendAreaToTextNode(m_write, s_currentContext.area, s_currentNode);
              s_currentNode->insideSpecial = true;
              s_currentNode->specialInsideXml = m_specialInsideXml;
            } else
            if (s_currentContext.area.bLine < s_currentContext.area.eLine ||
                (s_currentContext.area.bLine == s_currentContext.area.eLine &&
                 s_currentContext.area.bCol < s_currentContext.area.eCol))
            {
              //create a tag from the s_currentContext
              Tag *tag = new Tag(s_currentContext.area, m_write, s_dtd);
              QString tagStr = tag->tagStr();
              tag->cleanStr = tagStr;
              QuantaCommon::removeCommentsAndQuotes(tag->cleanStr, s_dtd);
              if (tagStr.simplifyWhiteSpace().isEmpty())
              {
                tag->type = Tag::Empty;
              } else
              {
                tag->type = Tag::Text;
              }
              tag->single = true;
              //create a node with the above tag
              Node *node = new Node(s_currentContext.parentNode);
              nodeNum++;
              node->tag = tag;
              node->insideSpecial = true;
              node->specialInsideXml = m_specialInsideXml;
              if (s_currentContext.parentNode && !s_currentContext.parentNode->child)
              {
                s_currentContext.parentNode->child = node;
              }
              else if (s_currentNode)
              {
                node->prev = s_currentNode;
                s_currentNode->next = node;
              }
              s_currentNode = node;
            }
          }
          //kdDebug(24000) << QString("%1 s_context: %2, %3, %4, %5").arg(s_currentContext.type).arg( s_currentContext.bLine).arg(s_currentContext.bCol).arg(s_currentContext.eLine).arg(s_currentContext.eCol) << endl;

          s_currentContext = s_context;
          s_col = s_context.area.bCol + s_context.startString.length();
        } else
        {
          s_line++;
          s_col = 0;
          s_textLine = ParserCommon::getLine(m_write, s_line, s_endLine, s_endCol);
        }
        break;
      }
     case QuotedString:
      {
        int pos = -1;
        int p = s_col;
        int l = s_textLine.length();
        while (p < l)
        {
          p = s_textLine.find(s_currentContext.startString, p);
          if (p != -1)
          {
            if (p >= 0)
            {
              int i = p - 1;
              int slahNum = 0;
              while (i > 0 && s_textLine[i] == '\\')
              {
                slahNum++;
                i--;
              }
              if (p == 0 || (slahNum % 2 == 0))
              {
                pos = p;
                break;
              }
            }
          } else
            break;
          p++;
        }
        if (pos != -1)
        {
      //    if (pos != 0) pos++;
          s_currentContext.area.eLine = s_line;
          s_currentContext.area.eCol = pos;
          //kdDebug(24000) << QString("Quoted String s_context: %1, %2, %3, %4").arg( s_currentContext.bLine).arg(s_currentContext.bCol).arg(s_currentContext.eLine).arg(s_currentContext.eCol) << endl;
          if (s_fullParse)
          {
            if ( s_currentNode  &&
                (s_currentNode->tag->type == Tag::Text ||
                s_currentNode->tag->type == Tag::Empty) )
              ParserCommon::appendAreaToTextNode(m_write, s_currentContext.area, s_currentNode);
            else
              s_currentNode = ParserCommon::createTextNode(m_write, 0L, s_line, pos, s_currentContext.parentNode);
            s_currentNode->insideSpecial = true;
            s_currentNode->specialInsideXml = m_specialInsideXml;
          }
          s_previousContext = s_contextStack.pop();
          s_currentContext.parentNode = s_previousContext.parentNode;
          s_currentContext.type = s_previousContext.type;
          s_currentContext.area.bLine = s_line;
          s_currentContext.area.bCol = pos + 1;
          s_currentContext.area.eLine = s_currentContext.area.eCol = -1;
          s_currentContext.startString = s_previousContext.startString;
          s_col = pos + 1;
        } else
        {
          s_line++;
          s_col = 0;
          s_textLine = ParserCommon::getLine(m_write, s_line, s_endLine, s_endCol);
        }
        break;
      }
     case Comment:
      {
        int pos = s_textLine.find(s_dtd->comments[s_currentContext.startString], s_col);
        if (pos == -1 && s_dtd->comments[s_currentContext.startString] == "\n")
        {
          int pos2 = s_textLine.find(s_areaEndString, s_col);
          if (pos2 != -1)
          {
            pos = pos2 - 1;
          } else
          {
            pos = s_textLine.length();
          }
        }
        if (pos != -1)
        {
          s_currentContext.area.eLine = s_line;
          s_currentContext.area.eCol = pos + s_dtd->comments[s_currentContext.startString].length() - 1;
          s_currentContext.type = s_previousContext.type;
          //kdDebug(24000) << QString("Comment s_context: %1, %2, %3, %4").arg( s_currentContext.bLine).arg(s_currentContext.bCol).arg(s_currentContext.eLine).arg(s_currentContext.eCol) << endl;

          if (s_fullParse)
          {
            //create a tag with the comment
            Tag *tag = new Tag(s_currentContext.area, m_write, s_dtd);
            tag->type = Tag::Comment;
            tag->single = true;
            //create a node with the above tag
            Node *node = new Node(s_currentContext.parentNode);
            nodeNum++;
            node->tag = tag;
            node->insideSpecial = true;
            node->specialInsideXml = m_specialInsideXml;
            if (s_currentNode && s_currentNode != node->parent)
            {
              s_currentNode->next = node;
              node->prev = s_currentNode;
            } else
            if (node->parent && !node->parent->child)
              node->parent->child = node;
            s_currentNode = node;
          }
          s_previousContext = s_contextStack.pop();
          s_currentContext.parentNode = s_previousContext.parentNode;
          s_currentContext.type = s_previousContext.type;
          s_currentContext.area.bLine = s_line;
          s_currentContext.area.bCol = s_currentContext.area.eCol + 1;
          s_currentContext.area.eLine = s_currentContext.area.eCol = -1;
          s_currentContext.startString = s_previousContext.startString;
          s_col = s_currentContext.area.bCol;
        } else
        {
          s_line++;
          s_col = 0;
          s_textLine = ParserCommon::getLine(m_write, s_line, s_endLine, s_endCol);
        }
        break;
      }
     default:
      {
        s_line++;
        s_col = 0;
        s_textLine = ParserCommon::getLine(m_write, s_line, s_endLine, s_endCol);
      }
   }
    if (m_synchronous)
    {
      //slotParseOneLine();
    }
    else
    {
#ifdef DEBUG_PARSER
      kdDebug(24001) << "Calling slotParseOneLine from slotParseOneLine." << endl;
#endif
      m_parseOneLineTimer->start(0, true);
    }
  } else
  {
    if (!m_synchronous)
    {
#ifdef DEBUG_PARSER
      kdDebug(24001) << "Calling parsingDone from slotParseOneLine." << endl;
#endif
      parsingDone();
    }
    return false; //parsing finished
  }
  return true;
}

Node* SAParser::parseArea(const AreaStruct &specialArea,
                          const QString &areaStartString,
                          const QString &forcedAreaEndString,
                          Node *parentNode,
                          bool fullParse, bool synchronous)
{
  m_synchronous = synchronous;
  s_parentNode = parentNode;
  s_fullParse = fullParse;
#ifdef DEBUG_PARSER
  kdDebug(24001) << "parseArea full: " << s_fullParse << "  synch: " << m_synchronous <<endl;
#endif

  int s_startLine = specialArea.bLine;
  int s_startCol = specialArea.bCol;
  s_endLine = specialArea.eLine;
  s_endCol = specialArea.eCol;
  //kdDebug(24000) << QString("Starting to parse at %1, %2 for %3").arg(s_startLine).arg(s_startCol).arg(areaStartString) << endl;

  s_searchForAreaEnd = false;
  s_searchForForcedAreaEnd = false;
  s_dtd = 0L;
  if (s_parentNode && !areaStartString.isEmpty())
  {
    const DTDStruct *parentDTD = m_dtd;
    if (s_parentNode->parent)
      parentDTD = s_parentNode->parent->tag->dtd();
    s_dtd = DTDs::ref()->find(parentDTD->specialAreaNames[areaStartString]);
    s_areaEndString = parentDTD->specialAreas[areaStartString];
    s_searchForAreaEnd = true;
  }
  if (!forcedAreaEndString.isEmpty())
  {
    s_forcedAreaRx.setPattern(forcedAreaEndString);
    s_forcedAreaRx.setCaseSensitive(m_dtd->caseSensitive);
    s_searchForForcedAreaEnd = true;
    s_searchForAreaEnd = false;
    if (s_parentNode)
      s_dtd = s_parentNode->tag->dtd();
  }
  s_searchContent = true;
  if (s_parentNode && s_parentNode->tag->type == Tag::Comment)
    s_searchContent = false;
  if (!s_dtd)
  {
    if (s_parentNode)
      s_dtd = s_parentNode->tag->dtd(); //fallback, usually when the special area is a comment
    else
      s_dtd = m_dtd; //fallback when there is no s_parentNode
  }
  m_write->addDTEP(s_dtd->name);
  s_searchForSpecialAreas = (s_dtd->specialAreas.count() > 0);
  if (s_parentNode && s_parentNode->tag->type == Tag::Comment)
    s_searchForSpecialAreas = false;
  s_col = s_startCol + areaStartString.length();
  s_line = s_startLine;
  s_textLine = m_write->text(s_startLine, 0, s_startLine, m_write->editIf->lineLength(s_startLine));
  if (s_line == s_endLine)
  {
    if (s_endCol > 0)
      s_textLine.truncate(s_endCol + 1);
    else
      s_textLine = "";
  }

  s_previousContext.type = Unknown;
  s_currentContext.type = Text;
  s_currentContext.area.bLine = s_line;
  s_currentContext.area.bCol = s_col;
  s_currentContext.area.eLine = s_currentContext.area.eCol = -1;
  s_currentContext.parentNode = s_parentNode;
  s_currentNode = s_parentNode;
  m_lastParsedNode = 0L;
  s_useReturnVars = false;
  if (s_line <= s_endLine)
  {
    if (m_synchronous)
    {
      while (slotParseOneLine()); //actually this parses the whole area, as synchronous == true
      if (s_useReturnVars) //this is true if the special area end was found
      {
        return m_lastParsedNode;
      }
    }
    else
    {
#ifdef DEBUG_PARSER
      kdDebug(24001) << "Calling slotParseOneLine from parseArea." << endl;
#endif
      m_parseOneLineTimer->start(0, true);
      return 0L;
    }
  }
  if (m_synchronous) //if the special area end was not found and we are in synchronous mode
  {
    s_next = 0L;
#ifdef DEBUG_PARSER
    kdDebug(24001) << "Calling parsingDone from parseArea." << endl;
#endif
    s_currentNode = parsingDone();
    return s_currentNode;
  }
  return 0L;
}

Node *SAParser::parsingDone()
{
#ifdef DEBUG_PARSER
  kdDebug(24001) << "parsingDone. Use return values:" << s_useReturnVars << endl;
#endif
  if (s_useReturnVars)
  {
    if (s_fullParse)
    {
        Node *n = m_lastParsedNode;
        if (m_useNext)
        {
//           kdDebug(24000) << "m_lastParsedNode: " << m_lastParsedNode << " " << m_lastParsedNode->tag->tagStr() << endl;
          n->next = s_next;
          if (s_next)
          {
            s_next->prev = n;
          }
          n->prev = s_parentNode;
        }
        m_currentNode = n->nextSibling();
        if (m_currentNode)
        {
#ifdef DEBUG_PARSER
          kdDebug(24001) << "Calling slotParseNodeInDetail from parsingDone (use return values)" << endl;
#endif
          m_parseInDetailTimer->start(0, true);
          return m_lastParsedNode;
        }
        else
        {
          m_parsingEnabled = true;
#ifdef DEBUG_PARSER
          kdDebug(24001) << "Emitting rebuildStructureTree from parsingDone (use return values). Enable parsing." << endl;
#endif
          emit rebuildStructureTree(false);
#ifdef DEBUG_PARSER
//           kdDebug(24000) << "Calling cleanGroups from SAParser::parsingDone" << endl;
#endif
          emit cleanGroups();
        }
    }
    m_currentNode = 0L;
    return m_lastParsedNode;
  }
  if (!s_currentNode)
  {
    s_currentNode = ParserCommon::createTextNode(m_write, s_parentNode, s_endLine, s_endCol, s_parentNode);
    if (s_currentNode)
    {
      s_currentNode->insideSpecial = true;
      s_currentNode->specialInsideXml = m_specialInsideXml;
    }
  }
  else if (s_parentNode && !s_parentNode->next)
  {
    s_currentNode = ParserCommon::createTextNode(m_write, s_currentNode, s_endLine, s_endCol, s_parentNode);
    s_currentNode->insideSpecial = true;
    s_currentNode->specialInsideXml = m_specialInsideXml;
  }
  if (s_fullParse)
  {
    Node *n;
    if (s_parentNode)
    {
      n = s_parentNode;//->child;
    } else
    {
      n = s_currentNode;
      while (n && n->prev)
        n = n->prev;
      s_currentNode = n;
    }
    Node *g_node = n;
#ifdef DEBUG_PARSER
    kdDebug(24001) << "Calling slotParseForScriptGroup from parsingDone. Synch:" << m_synchronous << endl;
#endif
    //parse for groups only when doing aynchronous detailed parsing
    if (!m_synchronous)
    {
      SAGroupParser *groupParser = new SAGroupParser(this, write(), g_node, 0L, m_synchronous, true /*last node*/, true);
      connect(groupParser, SIGNAL(rebuildStructureTree(bool)), SIGNAL(rebuildStructureTree(bool)));
      connect(groupParser, SIGNAL(cleanGroups()), SIGNAL(cleanGroups()));
      connect(groupParser, SIGNAL(parsingDone(SAGroupParser*)), SLOT(slotGroupParsingDone(SAGroupParser*)));
      groupParser->slotParseForScriptGroup();
      m_groupParsers.append(groupParser);
     }
  }

  m_lastParsedLine = s_endLine;
  m_lastParsedCol = s_endCol + 1;

  if (s_fullParse && m_currentNode)
  {
      if (m_useNext && s_currentNode)
      {
//           kdDebug(24000) << "s_currentNode: " << s_currentNode << endl;
        Node *n = s_currentNode;
        n->next = s_next;
        if (s_next)
          s_next->prev = n;
      }
      m_currentNode = m_currentNode->nextSibling();
      if (m_currentNode)
      {
#ifdef DEBUG_PARSER
        kdDebug(24001) << "Calling slotParseNodeInDetail from parsingDone." << endl;
#endif
        m_parseInDetailTimer->start(0, true);
        emit rebuildStructureTree(false);
      }
      else
      {
        m_parsingEnabled = true;
#ifdef DEBUG_PARSER
        kdDebug(24001) << "Emitting detailedParsingDone from parsingDone. Enable parsing." << endl;
#endif
        emit rebuildStructureTree(false);
      }
  }
  m_currentNode = 0L;
  return s_currentNode;
}

void SAParser::parseInDetail(bool synchronous)
{
//  synchronous = true; //for testing. Uncomment to test the parser in synchronous mode
//  return; //for testing. Uncomment to disable the detailed parser
#ifdef DEBUG_PARSER
  kdDebug(24001) << "parseInDetail. Enabled: " << m_parsingEnabled << endl;
#endif
  if (!m_parsingEnabled)
  {
    m_currentNode = m_baseNode;
    m_parsingEnabled = true;
    m_synchronous = synchronous;
    if (m_currentNode)
    {
#ifdef DEBUG_PARSER
      kdDebug(24001) << "Calling slotParseNodeInDetail from parseInDetail." << endl;
#endif
      slotParseNodeInDetail();
    }
  }
}

void SAParser::slotParseNodeInDetail()
{
#ifdef DEBUG_PARSER
  kdDebug(24001) << "slotParseNodeInDetail. Enabled: " << m_parsingEnabled << " Synch: " << m_synchronous << endl; //this is really heavy debug information, enable only when really needed
#endif
  if (m_currentNode && m_parsingEnabled && baseNode)
  {
    if (m_currentNode->insideSpecial &&
        m_currentNode->tag->type != Tag::Comment &&
        m_currentNode->tag->type != Tag::Text &&
        m_currentNode->tag->type != Tag::Empty)
    {
      Node::deleteNode(m_currentNode->child);
      m_currentNode->child = 0L;
      AreaStruct area(m_currentNode->tag->area());
      s_next = 0L;
      m_useNext = false;
      //FIXME: Find out why can the tag become 0L
      if (m_currentNode->next && m_currentNode->next->tag)
      {
        AreaStruct area2(m_currentNode->next->tag->area());
        area.eLine = area2.eLine;
        area.eCol = area2.eCol + 1;
        s_next = m_currentNode->next->next;
        if (m_currentNode->next->closesPrevious)
        {
          m_currentNode->next->removeAll = false;
          Node *secondNext = m_currentNode->next->next;
          if (secondNext)
            secondNext->prev = m_currentNode;
          Node::deleteNode(m_currentNode->next);
          m_currentNode->next = secondNext;
          m_useNext = true;
        }
      } else
      {
        area.eLine = m_write->editIf->numLines() - 1;
        area.eCol = m_write->editIf->lineLength(area.eLine);
      }
#ifdef DEBUG_PARSER
      kdDebug(24001) << "Calling parseArea from slotParseNodeInDetail." << endl;
#endif
      QString areaStartString = m_currentNode->tag->tagStr();
      if (m_currentNode->tag->dtd()->specialAreaNames[areaStartString].isEmpty())
      {
        AreaStruct area2(m_currentNode->tag->area());
        area.bLine = area2.eLine;
        area.bCol = area2.eCol + 1;
        parseArea(area, "", "</"+m_currentNode->tag->name+"\\s*>", m_currentNode, true, m_synchronous);
      }
      else
        parseArea(area, m_currentNode->tag->tagStr(), "", m_currentNode, true, m_synchronous);
    } else
    {
//       Node *node = m_currentNode;
      m_currentNode = m_currentNode->nextSibling();
      if (m_currentNode)
      {
#ifdef DEBUG_PARSER
//        kdDebug(24001) << "Calling slotParseNodeInDetail from slotParseNodeInDetail." << endl; //this is really heavy debug information, enable only when really needed
#endif
        m_parseInDetailTimer->start(0, true);
      } else
      {
#ifdef DEBUG_PARSER
        kdDebug(24001) << "Emitting rebuildStructureTree from slotParseNodeInDetail." << endl;
#endif
        emit rebuildStructureTree(false);
      }
    }
  }
}


void SAParser::setParsingEnabled(bool enabled)
{
#ifdef DEBUG_PARSER
  kdDebug(24001) << "Parsing enabled: " << enabled << endl;
#endif
  m_parsingEnabled = enabled;
  if (!enabled)
  {
    m_parseOneLineTimer->stop();
    m_parseInDetailTimer->stop();
    for (QValueList<SAGroupParser*>::Iterator it = m_groupParsers.begin(); it != m_groupParsers.end(); ++it)
    {
      (*it)->m_parseForGroupTimer->stop();
      delete (*it);
    }
    m_groupParsers.clear();
  }
}

void SAParser::slotGroupParsingDone(SAGroupParser *groupParser)
{
  m_groupParsers.remove(groupParser);
  delete groupParser;
}


#include "saparser.moc"
