/***************************************************************************
                          parser.cpp  -  description
                             -------------------
    begin                : Sun Sep 1 2002
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

//qt includes
#include <qstring.h>
#include <qregexp.h>
#include <qcstring.h>
#include <qstringlist.h>
#include <qstrlist.h>
#include <qdatetime.h>

//standard library includes
#include <stdio.h>
#include <ctype.h>
#include <iostream.h>

//app includes
#include "parser.h"
#include "node.h"
#include "tag.h"
#include "../resource.h"
#include "../quantacommon.h"
#include "../document.h"

//kde includes
#include <kapplication.h>
#include <kdebug.h>
#include <klocale.h>

Parser::Parser()
{
  m_text = QString::null;
  m_node = 0L;
  oldMaxLines = 0;
}

Parser::~Parser()
{
}

/** Searches for scripts inside the text from startNode. It looks only for the
script begin/and delimiters, and not for the <script> or other special tags.
Useful when parsing for script inside scripts, or inside the quoted attribute 
values of the xml tags. 
 Returns: true if a script area is found, false if the parsed text does not
contain any scripts. */
bool Parser::scriptParser(Node *startNode)
{
  bool found = false;
  DTDStruct *dtd = startNode->tag->dtd;
  if (dtd->specialAreas.count())
  {
    QString foundText;
    QString s;
    QString specialEndStr;
    QString text = startNode->tag->tagStr();

    int pos = 0;
    int pos2;
    int col = startNode->tag->structBeginStr.length();
    int bl, bc, el, ec;
    int node_bl, node_bc, node_el, node_ec;
    int n;
    startNode->tag->beginPos(node_bl, node_bc);
    startNode->tag->endPos(node_el, node_ec);
    Node *currentNode = 0L;

    while (pos != -1)
    {
      pos = text.find(dtd->specialAreaStartRx, col);
      if (pos != -1)
      {
        foundText = dtd->specialAreaStartRx.cap();
        //Calculate the beginning coordinates
        s = text.left(pos);
        n = s.contains('\n');
        bl = node_bl + n;
        if (n > 0)
        {
          bc = pos - s.findRev("\n") - 1;
        } else
        {
          bc = node_bc + pos;
        }
        //What is the closing string?
        specialEndStr = dtd->specialAreas[foundText];
        pos2 = text.find(specialEndStr, pos);
        if (pos2 != -1)
        {
          //Calculate the end coordinates
          s = text.left(pos2);
          n = s.contains('\n');
          el = node_bl + n;
          pos2 += specialEndStr.length() - 1;
          if (n > 0)
          {
            ec = pos2 - s.findRev("\n") - 1;
          } else
          {
            ec = node_bc + pos2;
          }
          s = text.mid(pos, pos2 - pos + 1);
          //build the tag
          Tag *tag = new Tag();
          tag->setWrite(write);
          tag->setStr(s);
          tag->setTagPosition(bl, bc, el, ec);
          tag->single = true;
          tag->dtd = dtds->find(dtd->specialAreaNames[foundText]);
          if (!tag->dtd)
              tag->dtd = m_dtd;  //fallback

          tag->name = i18n("%1 block").arg(dtd->specialAreaNames[foundText].upper());
          tag->structBeginStr = foundText;
          Node *node = new Node(startNode);
          node->tag = tag;
          node->tag->type = Tag::NeedsParsing;
          node->insideSpecial = true;
          //Insert the node into the tree under startNode
          if (!currentNode)
          {
            startNode->child = node;
          } else
          {
            currentNode->next = node;
            node->prev = currentNode;
          }
          currentNode = node;
          found = true;

          currentNode = specialAreaParser(node); //Why??
          //specialAreaParser(node);
          col = pos2 + 1;
        } else
          pos = -1;
      }
    }
  }
  return found;
}

/** Parse a string, using as start position sLine, sCol. */
Node *Parser::parseArea(int startLine, int startCol, int endLine, int endCol, Node **lastNode, Node *a_node)
{
  //first parse as an XML document
  QString textLine;
  textLine.fill(' ', startCol);
  int line = startLine;
  int col = 0;;
  int tagStartCol, tagStartLine, tagEndLine, tagEndCol;
  int tagStartPos, specialStartPos;
  int lastLineLength = write->editIf->lineLength(endLine);
  int specialAreaCount = m_dtd->specialAreas.count();
  bool nodeFound = false;
  bool goUp;
  Node *rootNode = 0L;
  Node *parentNode = a_node;
  Node *currentNode = a_node;
  if (currentNode && (currentNode->tag->type != Tag::XmlTag ||
      currentNode->tag->single))
      parentNode = currentNode->parent;
  Tag *tag;
  textLine.append(write->text(startLine, startCol, startLine, write->editIf->lineLength(startLine)));
  while (line <= endLine)
  {
    if (line == endLine)
    {
      if (endCol > 0) 
        textLine.truncate(endCol);
      else
        textLine = "";
    }
    nodeFound = false;
    goUp = false;
    tagStartPos = textLine.find('<', col);
    specialStartPos = specialAreaCount ? textLine.find(m_dtd->specialAreaStartRx, col): -1;
    if ( specialStartPos != -1 &&
         (specialStartPos <= tagStartPos || tagStartPos == -1) ) //a special tag beginning was found
    {
      tagStartLine = line;
      QString foundText = m_dtd->specialAreaStartRx.cap();
      QString specialEndStr = m_dtd->specialAreas[foundText];
      int pos = specialStartPos + foundText.length();
      tagEndCol = lastLineLength;
      tagEndLine = endLine;
      bool endFound = false;
      while (line <= endLine)
      {
        textLine = write->editIf->textLine(line);
        if (line == endLine)
        {
          if (endCol >0)
            textLine.truncate(endCol);
          else
            textLine = "";
        }
        pos = textLine.find(specialEndStr, pos);
        if (pos != -1)
        {
          tagEndLine = line;
          tagEndCol = pos + specialEndStr.length() - 1;
          endFound = true;
          break;
        } else
        {
          line++;
          pos = 0;
        }
      }
      if (a_node && !endFound)
          return 0L;

      tagStartCol = specialStartPos;
      col = tagEndCol;
      nodeFound = true;

      //build a special node here
      tag = new Tag();
      tag->setTagPosition(tagStartLine, tagStartCol, tagEndLine, tagEndCol);
      QString tagStr = write->text(tagStartLine, tagStartCol, tagEndLine, tagEndCol);
      tag->setStr(tagStr);
      tag->type = Tag::NeedsParsing;
      tag->setWrite(write);
      tag->single = true;
      tag->dtd = dtds->find(m_dtd->specialAreaNames[foundText]);
      if (!tag->dtd)
           tag->dtd = m_dtd;  //fallback
      tag->name = i18n("%1 block").arg(m_dtd->specialAreaNames[foundText].upper());
      tag->structBeginStr = foundText;

      goUp = (parentNode && parentNode->tag->single);
    }

    if ( tagStartPos != -1 &&
         (tagStartPos < specialStartPos || specialStartPos == -1) ) //do we found a tag?
    {
      int openNum = 1;
      tagStartLine = line;
      tagEndLine = endLine;
      tagEndCol = lastLineLength;
      int sCol = tagStartPos + 1;
      int firstStartCol = lastLineLength + 1;
      int firstStartLine = endLine;
      bool firstOpenFound = false;
      bool insideSingleQuotes = false;
      bool insideDoubleQuotes = false;
      while (line <= endLine && openNum > 0)
      {
        textLine = write->editIf->textLine(line);
        if (line == endLine)
        {
          if (endCol > 0)
            textLine.truncate(endCol);
          else
            textLine = "";
        }
        for (uint i = sCol; i < textLine.length(); i++)
        {
           if (i > 0 && textLine[i-1] != '\\')
           {
             if (textLine[i] == '\'' && !insideDoubleQuotes)
               insideSingleQuotes = !insideSingleQuotes;
             if (textLine[i] == '"' && !insideSingleQuotes)
               insideDoubleQuotes = !insideDoubleQuotes;
           }
           if (!insideSingleQuotes && !insideDoubleQuotes)
           {
              if (textLine[i] == '<')
              {
                openNum++;
                if (!firstOpenFound)
                {
                  firstStartCol = i;
                  firstStartLine = line;
                  firstOpenFound = true;
                }

              } else
              if (textLine[i] == '>') openNum--;
           }
           if (openNum == 0)
           {
             tagEndCol = i;
             tagEndLine = line;
             break;
           }
        }
        sCol = 0;
        if (openNum != 0)
            line++;
      }
      if (openNum != 0)
      {
        tagEndLine = firstStartLine;
        tagEndCol = firstStartCol - 1;
        if (tagEndCol < 0)
        {
          tagEndLine--;
          if (tagEndLine < 0)
              tagEndLine = 0;
          tagEndCol = write->editIf->lineLength(tagEndLine);
        }
        line = tagEndLine;
        textLine = write->editIf->textLine(line);
      }
      col = tagEndCol;
      nodeFound = true;
      //build an xml tag node here
      tag = new Tag();
      tag->setTagPosition(tagStartLine, tagStartPos, tagEndLine, tagEndCol);
      QString tagStr = write->text(tagStartLine, tagStartPos, tagEndLine, tagEndCol);
      tag->parse(tagStr , write);
      tag->type = Tag::XmlTag;
      tag->dtd = m_dtd;
      tag->single = QuantaCommon::isSingleTag(m_dtd->name, tag->name);
      if (tag->name[0] == '/')
      {
        tag->type = Tag::XmlTagEnd;
        tag->single = true;
      }
      if (tagStr.right(2) == "/>")
      {
        tag->single = true;
        if (tag->name.endsWith("/"))
            tag->name.truncate(tag->name.length() - 1);
      }

      if (m_dtd->specialTags.contains(tag->name.lower()))
      {
        QRegExp endRx;
        endRx.setPattern("/"+tag->name+"\\s*>");
        endRx.setCaseSensitive(m_dtd->caseSensitive);
        int bl, bc, el, ec;
        if (! write->find(endRx, line, tagEndCol, bl, bc, el, ec).isEmpty())
        {
          QString structBeginStr = tagStr;
          tagEndLine = el;
          tagEndCol = ec;
          QString s = tag->attributeValue(m_dtd->specialTags[tag->name.lower()]);
          delete tag;
          tag = new Tag();
          tag->setTagPosition(tagStartLine, tagStartPos, tagEndLine, tagEndCol);
          QString tagStr = write->text(tagStartLine, tagStartPos, tagEndLine, tagEndCol);
          tag->type = Tag::NeedsParsing;
          tag->setStr(tagStr);
          tag->setWrite(write);
          tag->single = true;
          tag->dtd = dtds->find(s);
          if (!tag->dtd)
              tag->dtd = m_dtd;  //fallback
          tag->name = i18n("%1 block").arg(s);
          tag->structBeginStr = structBeginStr;
          line = tagEndLine;
          col = tagEndCol;
          textLine = write->editIf->textLine(line);
        } else
        {
          if (a_node)
              return 0L;
        }

      }

      goUp = ( parentNode &&
               ( (tag->type == Tag::XmlTagEnd &&
                  "/"+parentNode->tag->name.lower() == tag->name.lower() ) ||
                  parentNode->tag->single )
             );
      if (parentNode && !goUp)
      {
        QTag *qTag = QuantaCommon::tagFromDTD(m_dtd, parentNode->tag->name);
        if ( qTag )
        {
          QString searchFor = (m_dtd->caseSensitive)?tag->name:tag->name.upper();
          if ( qTag->stoppingTags.contains( searchFor ) )
          {
            parentNode->tag->closingMissing = true; //parent is single...
            goUp = true;
          }
        }
      }
    }

    col++;
    if (nodeFound)
    {
      //build also the text node which is between two tag nodes
      Tag *textTag = 0L;
      Node *node = 0L;
      int el = 0;
      int ec = 0;
      if (currentNode)
      {
        currentNode->tag->endPos(el, ec);
      }
      tagStartPos--;
      QString s = write->text(el, ec + 1, tagStartLine, tagStartPos);

      if (el !=0 || ec !=0)
      {
        textTag = new Tag();
        textTag->setStr(s);
        textTag->setTagPosition(el, ec+1, tagStartLine, tagStartPos);
        textTag->setWrite(write);
        textTag->single = true;
        textTag->dtd = m_dtd;
        if (s.simplifyWhiteSpace().isEmpty())
        {
          textTag->type = Tag::Empty;
        } else
        {
          textTag->type = Tag::Text;
        }

        if (parentNode && parentNode->tag->single)
        {
          node = new Node(parentNode->parent);
          node->prev = parentNode;
          parentNode->next = node;
          parentNode = parentNode->parent;
          goUp = false;
        } else
        {
          if ( currentNode &&
               ( currentNode->tag->type == Tag::Empty ||
                 currentNode->tag->type == Tag::Text) )     //merge two consquenttext or empty nodes
          {
            currentNode->tag->beginPos(el, ec);
            currentNode->tag->setTagPosition(el, ec, tagStartLine, tagStartPos);
            s = write->text(el, ec, tagStartLine, tagStartPos);
            currentNode->tag->setStr(s);
            if (s.simplifyWhiteSpace().isEmpty())
            {
              currentNode->tag->type = Tag::Empty;
            } else
            {
              currentNode->tag->type = Tag::Text;
            }
            delete textTag;
            textTag = 0L;
          } else
          {
            node = new Node(parentNode);
            if (currentNode && currentNode != parentNode)
            {
              currentNode->next = node;
              node->prev = currentNode;
            } else
            {
              if (parentNode)
                  parentNode->child = node;
            }
            if (!textTag->single)
                parentNode = node;
          }
        }
        if (textTag)
        {
          node->tag = textTag;
          currentNode = node;
          if (!rootNode)
              rootNode = node;
        }
      }

      if (goUp)
      {
        node = new Node(parentNode->parent);
        node->prev = parentNode;
        parentNode->next = node;
        parentNode = parentNode->parent;
        node->closesPrevious = true;
      } else
      {
        node = new Node(parentNode);
        if (currentNode && currentNode != parentNode)
        {
          currentNode->next = node;
          node->prev = currentNode;
        } else
        {
          if (parentNode)
              parentNode->child = node;
        }
      }
      if (!tag->single)
          parentNode = node;

      node->tag = tag;

      if (tag->type == Tag::NeedsParsing)
      {
        if (tag->name.lower().startsWith("comment"))
        {
          node->tag->type = Tag::Comment;
        } else
        {
          //specialAreaParser(node);
          if (!rootNode)
              rootNode = node;
          node = specialAreaParser(node); //Why was so?
        }
      }
      else if (tag->type == Tag::XmlTag)
           {
             //search for scripts inside the XML tag
             scriptParser(node);
           }

      currentNode = node;
      if (!rootNode)
          rootNode = node;
    } else
    {
      line++;
      col = 0;
      textLine = write->editIf->textLine(line);
    }

  }

//create a text node from the last tag until the end of file
  Tag *textTag = 0L;
  Node *node = 0L;
  int el = 0;
  int ec = 0;
  if (currentNode)
  {
    currentNode->tag->endPos(el, ec);
  }

  QString s = write->text(el, ec + 1, endLine, endCol);
  if ( el !=0 || ec !=0 )
  {
    textTag = new Tag();
    textTag->setStr(s);
    textTag->setTagPosition(el, ec+1, endLine, endCol);
    textTag->setWrite(write);
    textTag->single = true;
    textTag->dtd = m_dtd;
    if (s.simplifyWhiteSpace().isEmpty()) //create a node with Empty tag type
    {
      textTag->type = Tag::Empty;
    } else
    {
      textTag->type = Tag::Text;
    }
    if (parentNode && parentNode->tag->single)
    {
      node = new Node(parentNode->parent);
      node->prev = parentNode;
      parentNode->next = node;
      parentNode = parentNode->parent;
      //node->closesPrevious = true;
      goUp = false;
    } else
    {
      if ( currentNode &&
          ( currentNode->tag->type == Tag::Empty ||
            currentNode->tag->type == Tag::Text) )     //merge two consquenttext or empty nodes
      {
        currentNode->tag->beginPos(el, ec);
        currentNode->tag->setTagPosition(el, ec, endLine, endCol);
        s = write->text(el, ec, endLine, endCol);
        currentNode->tag->setStr(s);
        if (s.simplifyWhiteSpace().isEmpty())
        {
          currentNode->tag->type = Tag::Empty;
        } else
        {
          currentNode->tag->type = Tag::Text;
        }
        delete textTag;
        textTag = 0L;
      } else
      {
        node = new Node(parentNode);
        if (currentNode && currentNode != parentNode)
        {
          currentNode->next = node;
          node->prev = currentNode;
        } else
        {
          if (parentNode)
              parentNode->child = node;
        }
        if (!textTag->single)
            parentNode = node;
      }
    }
    if (textTag)
    {
      node->tag = textTag;
      currentNode = node;
      if (!rootNode)
          rootNode = node;
    }
  }

  *lastNode = currentNode;
  return rootNode;
}

/** Parse the whole text from Document w and build the internal structure tree
    from Nodes */
Node *Parser::parse(Document *w)
{
  QTime t;
  t.start();

  if (baseNode)
  {
    delete baseNode;
    baseNode = 0L;
    m_node = 0L;
  }
  Node *lastNode;
  write = w;
  m_dtd = w->defaultDTD();
  m_text = w->editIf->text();
  maxLines = w->editIf->numLines() - 1;
  if (maxLines >= 0)
      m_node = parseArea(0, 0, maxLines, w->editIf->lineLength(maxLines), &lastNode);
  kdDebug(24000) << "New parser ("<< maxLines << " lines): " << t.elapsed() << " ms\n";
 // coutTree(rootNode, 2); -1
  return m_node;
}

/** Parses the found special (like script, css and such) areas.*/
Node* Parser::specialAreaParser(Node *startNode)
{
 DTDStruct *dtd = startNode->tag->dtd;
 QString str = startNode->tag->tagStr();
 QString tagStr = str;
 QString s, name;
 int pos = 0;
 int l;
 bool goUp = false;
 /* parse this special node for scripts. If a script is found inside, then
 replace the script area with spaces, so it won't mess up our block searching.
 */
 if (scriptParser(startNode))
 {
    int col = 0;
    QString specialEndStr;
    while (pos != -1)
    {
      pos = str.find(dtd->specialAreaStartRx, col);
      if (pos != -1)
      {
        specialEndStr = dtd->specialAreas[dtd->specialAreaStartRx.cap()];
        l = str.find(specialEndStr, pos);
        for (int j = pos; j < (int)(l + specialEndStr.length()) ; j++)
        {
          str[j] = ' ';
        }
        col = l;
      }
    }
  }

 //Replace all the commented strings and the escaped quotation marks (\", \')
 // with spaces so they will not mess up our parsing
 pos = 0;
 while (pos != -1)
 {
   pos = dtd->commentsStartRx.search(str, pos);
   if (pos != -1)
   {
     s = dtd->commentsStartRx.cap();
     if (s == "\\\"" || s == "\\'")
     {
       str[pos] = ' ';
       str[pos+1] = ' ';
       pos += 2;
     } else
     {
       s = dtd->comments[s];
       l = str.find(s, pos);
       l = (l == -1) ? str.length() : l;
       for (int i = pos; i < l ; i++)
       {
         str[i] = ' ';
       }
       pos = l + s.length();
     }
   }
 }

 //Now replace the quoted strings with spaces
 const QRegExp strRx("(\"[^\"]*\"|'[^']*')");
 pos = 0;
 while (pos != -1)
 {
   pos = strRx.search(str, pos);
   if (pos != -1)
   {
    l = strRx.matchedLength();
    for (int i = pos; i < pos + l ; i++)
    {
      str[i] = ' ';
    }
    pos += l;
   }
 }

  pos = 0;
  int lastPos = startNode->tag->structBeginStr.length();
  int lastPos2;
  int bLine, bCol, eLine, eCol;
  int el, ec; //the end coordinates of the root node
  int startPos; // where the sturcture really starts (e.g in "function {}")
  int matchNum;
  int n;
  startNode->tag->beginPos(bLine, bCol);
  n = startNode->tag->structBeginStr.contains("\n");
  bLine += n;
  bCol += lastPos;
  if (n > 0)
  {
    bCol -= startNode->tag->structBeginStr.findRev("\n");
  }

  Tag *tag;
  Node *node;
  Node *currentNode = startNode->child;
  Node *rootNode = startNode;
  Node *nextToRoot = rootNode;
  if (rootNode->next)
      nextToRoot = rootNode->next;
  bool findStruct; //true if there is a need to search for the structure end

  if (currentNode)
  {
    while (currentNode->next)
      currentNode = currentNode->next;
  }


  while (pos != -1)
  {
    goUp = false;
    pos  = dtd->structRx.search(str, lastPos);
    nextToRoot->tag->endPos(el, ec);
    if (pos != -1)
    {
      if (dtd->structRx.cap() == dtd->structBeginStr)
      {
        startPos = dtd->structKeywordsRx.searchRev(str, pos);
        findStruct = true;
      } else
      {
        startPos = -1;
        findStruct = false;
      }
      if (startPos == -1 || startPos <= lastPos)
          startPos = pos;
      //create a Text node from the string between lastPos and pos
      s = tagStr.mid(lastPos, startPos - lastPos);
      eLine = bLine + s.contains('\n');
      n = s.findRev('\n');
      if (n == -1)
      {
        eCol = startPos - lastPos + bCol - 1;
      } else
      {
        eCol = startPos - n - 2 - lastPos;
      }
      tag = new Tag();
      tag->setStr(s);//s.replace(dtd->commentsRx,"").stripWhiteSpace());
      tag->cleanStr = str.mid(lastPos, startPos - lastPos);
      tag->setWrite(write);
      tag->setTagPosition(bLine, bCol, eLine, eCol);
      if (s.simplifyWhiteSpace().isEmpty())
      {
        tag->type = Tag::Empty;
      } else
      {
        tag->type = Tag::Text;
      }
      tag->single = true;
      tag->dtd = dtd;

      while ( bLine > el ||               //the beginning of the tag is after the end of the
            (bLine == el && bCol > ec) ) //root, so go up one level
      {
        currentNode = nextToRoot;
        rootNode = rootNode->parent;
        nextToRoot = rootNode;
        if (rootNode->next)
          nextToRoot = rootNode->next;
        nextToRoot->tag->endPos(el, ec); //get the end coordinates for the new root node
        goUp = true;
      }
      node = new Node(rootNode);
      if (!rootNode->child)
      {
        rootNode->child = node;
      } else
      {
        node->prev = currentNode;
        currentNode->next = node;
      }
      node->tag = tag;
      node->insideSpecial = true;
      if (goUp)
      {
        node->closesPrevious = true;
        goUp = false;
      }
      currentNode = node;

      lastPos = pos + 1;
      name = "";
      bLine = eLine;
      bCol = eCol + 1;
      if (findStruct)
      {
        //find the matching structure end string (})
        lastPos2 = pos + 1;
        matchNum = 1;
        name = tagStr.mid(startPos, pos - startPos);
        pos = 0;
        while (matchNum != 0 && pos != -1)
        {
          pos = dtd->structRx.search(str, lastPos2);
          if (pos != -1)
          {
            s = dtd->structRx.cap();
            if (s == dtd->structBeginStr)
            {
              matchNum++;
            } else
            {
              matchNum--;
            }
            lastPos2 = pos + s.length();
          }
        }
        pos = 0;
        if (matchNum != 0)
            lastPos2 = str.length();

        tag = new Tag();
        //s = tagStr.mid(startPos, lastPos2 - startPos);
        s = str.mid(startPos, lastPos - startPos - 1);
        eLine = bLine + s.contains('\n');
        n = s.findRev('\n');
        if (n == -1)
        {
          eCol = lastPos2 - startPos + bCol - 1;
        } else
        {
          eCol = lastPos2 - n - 2 - startPos;
        }
        tag->name = name.stripWhiteSpace();
        //tag->setStr(name);
        tag->setStr(s);
        tag->cleanStr = str.mid(startPos, lastPos - startPos - 1);
        tag->setWrite(write);
        tag->setTagPosition(bLine, bCol, eLine, eCol);
        tag->type = Tag::ScriptStructureBegin;
        //tag->single = true;
        tag->dtd = dtd;

        bLine += name.contains("\n");
        if ( bLine > el ||                //the beginning of the tag is after the end of the
            (bLine == el && bCol > ec) )  //root, so go up one level
        {
          currentNode = nextToRoot;
          rootNode = rootNode->parent;
          if (rootNode->next)
              nextToRoot = rootNode->next;
          goUp = true;
        }

        node = new Node(rootNode);
        if (!rootNode->child)
        {
          rootNode->child = node;
        } else
        {
          node->prev = currentNode;
          currentNode->next = node;
        }
        node->tag = tag;
        node->insideSpecial = true;
        if (goUp)
            node->closesPrevious = true;

        int l = name.length();
        s = tagStr.mid(startPos + l + 1, lastPos2 - startPos - l - 1);
        eLine = bLine + s.contains('\n');
        n = s.findRev('\n');
        if (n == -1)
        {
          eCol = lastPos2 - startPos + bCol - 1;
        } else
        {
          eCol = lastPos2 - n - 3 - startPos  - l;
        }
        tag = new Tag();
        tag->type = Tag::ScriptStructureEnd;
        n = dtd->structEndStr.length();
        s = s.right(n);
        tag->setStr(s);
        tag->setTagPosition(eLine, eCol - n + 1, eLine, eCol);
        tag->setWrite(write);
        tag->dtd = dtd;
        tag->single = true;

        nextToRoot = new Node(rootNode);
        nextToRoot->prev = node;
        node->next = nextToRoot;
        nextToRoot->tag = tag;
        nextToRoot->closesPrevious = true;
        nextToRoot->insideSpecial = true;

        currentNode = node;
        rootNode = node;

      }

      bCol += name.length() + 1;
    } else    //create a text node from the last structure until the end of special area
    {
      startNode->tag->endPos(eLine, eCol);
      tag = new Tag();
      s = tagStr.mid(lastPos);
      tag->setStr(s);//s.replace(dtd->commentsRx,"").stripWhiteSpace());
      tag->cleanStr = str.mid(lastPos);
      tag->setWrite(write);
      tag->setTagPosition(bLine, bCol, eLine, eCol);
      if (s.simplifyWhiteSpace().isEmpty())
      {
        tag->type = Tag::Empty;
      } else
      {
        tag->type = Tag::Text;
      }
      tag->single = true;
      tag->dtd = dtd;
      while ( bLine > el ||               //the beginning of the tag is after the end of the
            (bLine == el && bCol > ec) ) //root, so go up one level
      {
        currentNode = nextToRoot;
        rootNode = rootNode->parent;
        nextToRoot = rootNode;
        if (rootNode->next)
            nextToRoot = rootNode->next;
        nextToRoot->tag->endPos(el, ec); //get the end coordinates for the new root node
        goUp = true;
      }
      node = new Node(rootNode);
      if (!rootNode->child)
      {
        rootNode->child = node;
      } else
      {
        node->prev = currentNode;
        currentNode->next = node;
      }
      node->tag = tag;
      node->insideSpecial = true;
      if (goUp)
          node->closesPrevious = true;
      currentNode = node;
    }
  }

  rootNode = startNode;


  int bl, bc;
  startNode->tag->beginPos(bLine, bCol);
  startNode->tag->endPos(el, ec);

  //modify the starting node, so it contains only the beginning of the special area
  bl = bLine;
  bc = bCol;
  n = startNode->tag->structBeginStr.contains("\n");
  bLine += n;
  bCol += startNode->tag->structBeginStr.length();
  if (n > 0)
  {
    bCol -= startNode->tag->structBeginStr.findRev("\n");
  }
  s = write->text(bl, bc, bLine, bCol - 1);
  startNode->tag->setTagPosition(bl, bc, bLine, bCol - 1);
  startNode->tag->setStr(s);

  eLine = eCol = -1;
  //create a node with the end of special area
  QString specialEndStr = dtd->specialAreas[startNode->tag->structBeginStr];
  if (specialEndStr.isEmpty())
  {
    tag = new Tag(*startNode->tag);
    tag->parse(startNode->tag->structBeginStr, write);
    name = tag->name;
    delete tag;
    tag = 0L;
    if (!name.isEmpty())
    {
      name.prepend('/');
      s = currentNode->tag->tagStr();
      pos = s.findRev(name);
      if (pos != -1)
      {
        s = s.left(pos);
        pos = s.findRev('<');
        if (pos !=-1)
        {
          s = s.left(pos);
          n = s.contains('\n');
          currentNode->tag->beginPos(eLine, eCol);
          eLine +=n;
          eCol = s.length();
          if (n >0)
          {
            eCol -= s.findRev('\n');
            eCol -=2;
          }
        }

      }
    }
  } else
  {
    eLine = el;
    eCol = ec - specialEndStr.length();
  }
  if (eLine != -1)
  {
    Tag *tag = new Tag();
    s = write->text(eLine, eCol+1, el, ec);
    tag->setStr(s);
    tag->setTagPosition(eLine, eCol+1, el, ec);
    tag->type = Tag::XmlTagEnd;
    tag->single = true;
    tag->dtd = dtd;
    tag->setWrite(write);

    node = new Node(startNode->parent);
    node->tag = tag;
    node->insideSpecial = true;
    node->closesPrevious = true;
    node->prev = startNode;
    startNode->next = node;
    startNode->tag->single = false;

    rootNode = node;
  }

//if the block has no nodes inside, create a Text node with its content.
  if (currentNode == startNode->child)
  {
    tag = new Tag(*startNode->tag);
    s = write->text(bLine, bCol, eLine, eCol);
    tag->setStr(s);
    tag->setTagPosition(bLine, bCol, eLine, eCol);
    tag->type = Tag::Text;
    tag->single = true;
    node = new Node(startNode);
    startNode->child = node;
    node->tag = tag;
    node->insideSpecial = true;
    tag->dtd = dtd;
  } else
  {
    currentNode->tag->beginPos(bLine, bCol);
    s = write->text(bLine, bCol, eLine, eCol);
    if (s.simplifyWhiteSpace().isEmpty())
    {
      currentNode->tag->type = Tag::Empty;
    } else
    {
      currentNode->tag->type = Tag::Text;
    }
    currentNode->tag->setStr(s);
    currentNode->tag->setTagPosition(bLine, bCol, eLine, eCol);
  }

  startNode->tag->type = Tag::ScriptTag;
  startNode->insideSpecial = true;
  return rootNode;
}

/** Print the doc structure tree to the standard output.
    Only for debugging purposes. */
void Parser::coutTree(Node *node, int indent)
{
 QString output;
 while (node)
 {
   output = "";
   output.fill('.', indent);
   if (node->tag->type != Tag::Text)
       output += node->tag->name.replace(QRegExp("\n")," ");
   else
       output+= node->tag->tagStr().replace(QRegExp("\n")," ");
   cout << output <<" (" << node->tag->type << ")\n";
   if (node->child)
       coutTree(node->child, indent + 4);
   node = node->next;
 }
}

/** Clear the parser internal text, thus forcing the reparsing. */
void Parser::clear()
{
  m_text = "";
}

/** No descriptions */
DTDStruct * Parser::currentDTD(int line, int col)
{
  DTDStruct *dtd = m_dtd;
  Node *node = nodeAt(line, col);
  if (node)
  {
    dtd = node->tag->dtd;
  }

  return dtd;
}

/** Returns the node for position (line, column). As more than one node can
contain the same area, it return the "deepest" node. */
Node *Parser::nodeAt(int line, int col, bool findDeepest)
{
  Node *node = m_node;
  int bl, bc, el, ec;
  int result;

  while (node)
  {
    node->tag->beginPos(bl, bc);
    if (node->next)
    {
      node->next->tag->beginPos(el, ec);
    } else
    {
      el = write->editIf->numLines();
      ec = 0;
    }
    result = QuantaCommon::isBetween(line, col, bl, bc, el, ec);
    if ( result == 0)
    {
      if (node->child)
      {
        node = node->child;
      } else
      {
        break; //we found the node
      }
    } else
    if (result == -1)
    {
      node = node->parent;
      break; //we found the node
    } else
    {
      node = node->next;
    }
  }

  if (findDeepest && node && node->tag->type == Tag::Empty)
  {
     if (node->parent)
     {
       node = node->parent;
     } else
     if (node->prev)
     {
       node = node->prev;
     }
  }
  return node;
}

Node *Parser::rebuild(Document *w)
{
 QTime t;
 t.start();
 if (w != write || !m_node)
 {
   return parse(w);
 } else
 {
   oldMaxLines = maxLines;
   maxLines = w->editIf->numLines() - 1;
   int lineDiff = maxLines - oldMaxLines;
   int bl, bc, el, ec;
   int bLine, bCol, eLine, eCol; //the coordinates of the invalid area
   bLine = bCol = 0;
   eLine = maxLines;
   eCol = w->editIf->lineLength(maxLines);
   QString text;
   QString tagStr;
   bool moveNodes = false;
   uint line, col;
   m_text = w->editIf->text();
   QStringList textLines = QStringList::split('\n', m_text, true);
   w->viewCursorIf->cursorPositionReal(&line, &col);
   Node *node = nodeAt(line, col, false);
   Node *startNode = node;
   if (node)
   {
     node->tag->beginPos(bl, bc);
     bLine = bl;
     bCol = bc;
   }

   Node *firstNode = node;
   Node *lastNode = 0L;
//find the first unchanged (non empty) node and store it as firstNode
   while (node)
   {
     node->tag->beginPos(bl, bc);
     node->tag->endPos(el, ec);
     text = w->text(bl, bc, el, ec);
     tagStr = node->tag->tagStr();
     if ( tagStr != text ||
          node->tag->type == Tag::Empty ||
          node->insideSpecial
        )
     {
       node = node->previousSibling();
     } else
     {
       firstNode = node;
       break;
     }
   }

//find the last unchanged node and store it as lastNode;
//move the nodes if they were shifted
   node = startNode;
   while (node)
   {
     node->tag->beginPos(bl, bc);
     node->tag->endPos(el, ec);
     if (!moveNodes)
     {
        text = w->text(bl + lineDiff, bc, el + lineDiff, ec);
        tagStr = node->tag->tagStr();
        if (tagStr == text && node->tag->type != Tag::Empty && !node->insideSpecial)
        {
          if (!lastNode)
              lastNode = node;

          if (lineDiff != 0)
          {
            moveNodes = true;
            node->tag->setTagPosition(bl + lineDiff, bc, el + lineDiff, ec);
          } else
          {
            break;
          }
        }
     } else
     {
       node->tag->setTagPosition(bl + lineDiff, bc, el + lineDiff, ec);
     }
     node = node->nextSibling();
   }

   if (firstNode && !firstNode->insideSpecial)
   {
     node = firstNode->nextSibling();
   } else
   {
//     node = m_node;
     return parse(write);
   }
   if (node)
       node->tag->beginPos(bLine, bCol);
   if (lastNode)
   {
     lastNode->tag->beginPos(eLine, eCol);
     eCol--;
   }

   //delete all the nodes between the firstNode and lastNode
   while (node && node != lastNode )
   {
     Node *nextNode = node->nextSibling();
     node->removeAll = false;
     Node *child = node->child;
     Node *parent = node->parent;
     Node *next = node->next;
     Node *prev = node->prev;
     bool closesPrevious = node->closesPrevious;
     if (nextNode && nextNode->prev == node)
     {
       nextNode->prev = prev;
     }
     if (next)
      next->prev = prev;
     if (prev && prev->next == node)
     {
        prev->next = next;
     }
     if (next && next->closesPrevious)
      next->closesPrevious = false;

     delete node;
     node = 0L;
     if (child)
     {
       Node *n = child;
       Node *m = child;
       while (n)
       {
         m = n;
         n->parent = parent;
         n = n->next;
       }
       if (prev)
       {
         child->prev = prev;
         prev->next = child;
       } else
       {
         if (parent)
            parent->child = child;
       }
       prev = m;
     }

     if (prev)
     {
       node = prev;
       node->next = next;
       if (next)
          next->prev = prev;
       while (node->child)
       {
         node = node->child;
         while (node->next)
              node = node->next;
       }
       if (next && closesPrevious)
       {
         if (prev->child)
         {
            next->prev = node;
            node->next = next;
         } else
         {
           prev->child = next;
           next->prev = 0L;
         }
         prev->next = 0L;
         Node *n = next;
         while (n)
         {
           n->parent = prev;
           n = n->next;
         }
       }
     } else
     {
       if (next && closesPrevious)
       {
         Node *n = next;
         while (n)
         {
           n->parent = prev;
           n = n->next;
         }
         next->prev = 0L;
       }
       node = parent;
       if (node)
           node->child = next;
     }
     node = nextNode;
   }

   QString invalidStr = QString("Invalid area: %1,%2,%3,%4").arg(bLine).arg(bCol).arg(eLine).arg(eCol);
   kdDebug(24000) << invalidStr << "\n";

   //something strange has happened, like moving text with D&D inside the editor
   if (eLine < bLine || (eLine == bLine && eCol <= bCol))
   {
      return parse(w);
   }

   Node *lastInserted = 0L;
   node = parseArea(bLine, bCol, eLine, eCol, &lastInserted, firstNode);

   //another stange case: the parsed area contains a special area without end
   if (!node)
   {
      if (lastNode)
      {
        if (lastNode->prev )
            lastNode->prev->next = 0L;
        if (lastNode->parent && lastNode->parent->child == lastNode)
            lastNode->parent->child = 0L;
      }
     delete lastNode;
     lastNode = 0L;
     return parse(w);
   }

   bool goUp;
   if (lastNode && lastInserted)
   {
      //merge the nodes if they are both of type Text or Empty
      if ( (lastInserted->tag->type == Tag::Empty || lastInserted->tag->type == Tag::Text) &&
            (lastNode->tag->type == Tag::Empty || lastNode->tag->type == Tag::Text))
      {
        lastNode->prev = lastInserted->prev;
        if (lastInserted->prev)
            lastInserted->prev->next = lastNode;
        lastNode->parent = lastInserted->parent;
        lastInserted->tag->beginPos(bLine, bCol);
        lastNode->tag->endPos(eLine, eCol);
        lastNode->tag->setTagPosition(bLine, bCol, eLine, eCol);
        QString s = write->text(bLine, bCol, eLine, eCol);
        lastNode->tag->setStr(s);
        if (!s.simplifyWhiteSpace().isEmpty())
        {
          lastNode->tag->type = Tag::Text;
        } else
        {
          lastNode->tag->type = Tag::Empty;
        }
        if (lastInserted->parent && lastInserted->parent->child == lastInserted)
            lastInserted->parent->child = lastInserted->next;
        lastInserted->removeAll = false;
        delete lastInserted;
        lastInserted = lastNode;
        if (lastNode->next)
        {
          lastNode = lastNode->next;
        } else
        {
          while (lastNode)
          {
            if (lastNode->parent && lastNode->parent->next)
            {
              lastNode = lastNode->parent->next;
              break;
            } else
            {
              lastNode = lastNode->parent;
            }
          }
        }
      }

    node = lastInserted;
    while (node && lastNode)
    {
      goUp = ( node->parent &&
               ( (lastNode->tag->type == Tag::XmlTagEnd &&
                  "/"+node->parent->tag->name.lower() == lastNode->tag->name.lower() ) ||
                  node->parent->tag->single )
             );
      if (node->parent && !goUp)
      {
        QTag *qTag = QuantaCommon::tagFromDTD(m_dtd, node->parent->tag->name);
        if ( qTag )
        {
          QString searchFor = (m_dtd->caseSensitive)?lastNode->tag->name:lastNode->tag->name.upper();
          if ( qTag->stoppingTags.contains( searchFor ) )
          {
            node->parent->tag->closingMissing = true; //parent is single...
            goUp = true;
          }
        }
      }

    if (goUp)
    {
      if (lastNode->prev)
        lastNode->prev->next = 0L;
      if (lastNode->parent && lastNode->parent->child == lastNode)
          lastNode->parent->child = 0L;
      node->parent->next = lastNode;
      lastNode->prev = node->parent;
      if (node->parent)
        lastNode->parent = node->parent->parent;
      else
        lastNode->parent = 0L;
      node->next = 0L;
      lastNode->closesPrevious = true;
    } else
    {
      if (lastNode->prev && lastNode->prev->next == lastNode)
          lastNode->prev->next = 0L;
      node->next = lastNode;
      lastNode->prev = node;
      lastNode->parent = node->parent;
    }
    node = lastNode;
    if (lastNode->next)
    {
      lastNode = lastNode->next;
    } else
    {
      while (lastNode)
      {
        if (lastNode->parent && lastNode->parent->next)
        {
          lastNode = lastNode->parent->next;
          break;
        } else
        {
          lastNode = lastNode->parent;
        }
      }
    }

    }
   }
 }
 //coutTree(m_node, 2);
 //cout << "\n";
 kdDebug(24000) << "Rebuild: " << t.elapsed() << " ms \n";
 return m_node;
}
