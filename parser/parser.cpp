/***************************************************************************
                          parser.cpp  -  description
                             -------------------
    begin                : Sun Sep 1 2002
    copyright            : (C) 2002 by Andras Mantia
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

//app includes
#include "parser.h"
#include "node.h"
#include "tag.h"
#include "../resource.h"
#include "../quantacommon.h"
#include "../document.h"

//kde includes
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
 
  QString foundText;
  QString s;
  QString specialEndStr;
  QString text = startNode->tag->tagStr();
  
  int pos = 0;
  int pos2;
  int parsingDTDIndex = 0;
  int col = startNode->tag->offset;
  int bl, bc, el, ec;
  int node_bl, node_bc, node_el, node_ec;
  int n;
  startNode->tag->beginPos(node_bl, node_bc);
  startNode->tag->endPos(node_el, node_ec);
  Node *currentNode = 0L;
  bool found = false;
  
  while (pos != -1)
  {
    pos = text.find(m_dtd->specialAreaStartRx, col); //FIXME: m_dtd nem jo...
    if (pos != -1)
    {
      foundText = m_dtd->specialAreaStartRx.cap();
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
      for (uint i = 0; i < m_dtd->specialAreaBegin.count(); i++)
      {
        if (m_dtd->specialAreaBegin[i] == foundText)
        {
          specialEndStr = m_dtd->specialAreaEnd[i];
          parsingDTDIndex = i;
          break;
        }
      }
      pos2 = text.find(specialEndStr, pos);
      if (pos2 != -1) 
      {          
        //Calculate the end coordinates
        s = text.left(pos2);
        n = s.contains('\n');
        el = node_bl + n;
        if (n > 0)
        {
          ec = pos2 - s.findRev("\n");
        } else
        {
          ec = node_bc + pos2 + specialEndStr.length() - 1;
        }
        s = text.mid(pos, pos2 - pos);
        //build the tag
        Tag *tag = new Tag;
        tag->setWrite(write);
        tag->setStr(s);
        tag->setTagPosition(bl, bc, el, ec);
        tag->single = true;
        tag->parsingDTDName = m_dtd->specialAreaNames[parsingDTDIndex];
        tag->name = i18n("%1 block").arg(tag->parsingDTDName.upper());
        tag->offset = foundText.length();
        Node *node = new Node(startNode);
        node->tag = tag;
        node->tag->type = Tag::ScriptTag;
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

        parseInside(node); 

        col = pos2 + 1;           
      } else 
        pos = -1;
    }
  }
  
  return found;
}

Node *Parser::newParse(Document *w)
{
  QTime t;
  t.start();

  write = w;
  m_dtd = dtds->find(w->parsingDTD()); 
  //first parse as an XML document
  QString textLine;
  int line, col;
  line = col = 0;
  int tagStartCol, tagStartLine, tagEndLine, tagEndCol;
  int tagStartPos, specialStartPos;
  int maxLine = w->editIf->numLines() - 1;
  int lastLineLength = w->editIf->lineLength(maxLine);
  int parsingDTDIndex = -1; //the index inside specialAreaNames or specialTagNames
  bool nodeFound = false;
  bool goUp;
  Node *rootNode = 0;
  Node *parentNode = 0L;
  Node *currentNode = 0L;
  Tag *tag;
  textLine = w->editIf->textLine(line);
  while (line < maxLine)
  {
    nodeFound = false;
    goUp = false;
    tagStartPos = textLine.find('<', col);
    specialStartPos = textLine.find(m_dtd->specialAreaStartRx, col);
    if ( specialStartPos != -1 && 
         (specialStartPos <= tagStartPos || tagStartPos == -1) ) //a special tag beginning was found
    {
      tagStartLine = line;
      QString foundText = m_dtd->specialAreaStartRx.cap();
      QString specialEndStr;
      for (uint i = 0; i < m_dtd->specialAreaBegin.count(); i++)
      {
        if (m_dtd->specialAreaBegin[i] == foundText)
        {
          specialEndStr = m_dtd->specialAreaEnd[i];
          parsingDTDIndex = i;
          break;
        }
      }
      
      int pos = specialStartPos + foundText.length();
      tagEndCol = lastLineLength;
      while (line < maxLine)
      {
        textLine = w->editIf->textLine(line);
        pos = textLine.find(specialEndStr, pos);
        if (pos != -1)
        {
          tagEndLine = line;
          tagEndCol = pos + specialEndStr.length() - 1;
          break;
        } else
        {
          line++;
          pos = 0;
        }
      }
      tagStartCol = specialStartPos ;
      col = tagEndCol;
      nodeFound = true;

      //build a special node here
      tag = new Tag();
      tag->setTagPosition(tagStartLine, tagStartCol, tagEndLine, tagEndCol);
      QString tagStr = w->text(tagStartLine, tagStartCol, tagEndLine, tagEndCol);
      tag->setStr(tagStr);
      tag->type = Tag::NeedsParsing;
      tag->setWrite(w);
      tag->single = true;
      tag->parsingDTDName = m_dtd->specialAreaNames[parsingDTDIndex];
      tag->name = i18n("%1 block").arg(tag->parsingDTDName.upper());
      tag->offset = foundText.length();
      
      goUp = (parentNode && parentNode->tag->single);
    }
    
    if ( tagStartPos != -1 && 
         (tagStartPos < specialStartPos || specialStartPos == -1) ) //do we found a tag?
    {
      int openNum = 1;
      tagStartLine = line;
      tagEndLine = maxLine;
      tagEndCol = w->editIf->lineLength(maxLine);
      int sCol = tagStartPos + 1;
      while (line < maxLine && openNum > 0)
      {
        textLine = w->editIf->textLine(line);
        for (uint i = sCol; i < textLine.length(); i++)
        {
           if (textLine[i] == '<') openNum++;
           if (textLine[i] == '>') openNum--;
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
      
      col = tagEndCol;
      nodeFound = true;
      //build an xml tag node here
      tag = new Tag();
      tag->setTagPosition(tagStartLine, tagStartPos, tagEndLine, tagEndCol);
      QString tagStr = w->text(tagStartLine, tagStartPos, tagEndLine, tagEndCol);
      tag->parse(tagStr , w);
      tag->type = Tag::XmlTag;
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
      
      int tagPos = m_dtd->specialTags.findIndex(tag->name.lower());
      if (tagPos != -1)
      {
        QRegExp endRx;
        endRx.setPattern("/"+tag->name+"\\s*>");
        endRx.setCaseSensitive(m_dtd->caseSensitive);
        int bl, bc, el, ec;
        if (! w->find(endRx, line, tagEndCol, bl, bc, el, ec).isEmpty())
        {
          tagEndLine = el;
          tagEndCol = ec;
          QString s = tag->attributeValue(m_dtd->specialTagNames[tagPos]);
          if (s.isEmpty())
              s = i18n("unknown");
          delete tag;
          tag = new Tag();
          tag->setTagPosition(tagStartLine, tagStartPos, tagEndLine, tagEndCol);
          QString tagStr = w->text(tagStartLine, tagStartPos, tagEndLine, tagEndCol);
          tag->type = Tag::NeedsParsing;
          tag->setStr(tagStr);
          tag->setWrite(w);
          tag->single = true;
          tag->parsingDTDName = s; 
          tag->name = i18n("%1 block").arg(tag->parsingDTDName.upper());
          line = tagEndLine;
          col = tagEndCol;
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
      QString s = w->text(el, ec + 1, tagStartLine, tagStartPos -1);
      if (!s.simplifyWhiteSpace().isEmpty() &&
          (el !=0 || ec !=0) )
      {
        textTag = new Tag();
        textTag->setStr(s);
        textTag->setTagPosition(el, ec+1, tagStartLine, tagStartPos -1);
        textTag->setWrite(w);
        textTag->type = Tag::Text;
        textTag->single = true;
        
        if (parentNode && parentNode->tag->single)
        {
          node = new Node(parentNode->parent);
          node->prev = parentNode;
          parentNode->next = node;
          parentNode = parentNode->parent;
          goUp = false;
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
        
        node->tag = textTag;
        currentNode = node;
        if (!rootNode) 
            rootNode = node;
      }
      
      if (goUp)
      {
        node = new Node(parentNode->parent);
        node->prev = parentNode;
        parentNode->next = node;
        parentNode = parentNode->parent;
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
        if (!tag->single)
            parentNode = node;
      }
      
      node->tag = tag;
      
      if (tag->type == Tag::NeedsParsing)
      {
        if (tag->parsingDTDName != "comment")
        {
          parseInside(node);
          node->tag->type = Tag::ScriptTag;
        } else
        {
          node->tag->type = Tag::Comment;
        }
      }       
      else if (tag->type == Tag::XmlTag)
           {
             scriptParser(node);
           }
      
      currentNode = node;
      if (!rootNode) 
          rootNode = node;
    } else
    {
      line++;
      col = 0;
      textLine = w->editIf->textLine(line);
    }
    
  }
  
  m_text = w->editIf->text();
  kdDebug(24000) << "New parser ("<< maxLine << " lines): " << t.elapsed() << " ms\n";
 // coutTree(rootNode, 2);
  return rootNode;
}

void Parser::parseInside(Node *startNode)
{

 const QString quotationStr = "\\\\\"|\\\\'";
  
 QRegExp commentsRx;
 DTDStruct *dtd = dtds->find(startNode->tag->parsingDTDName);
 if (!dtd) 
    dtd = m_dtd;  //fallback
 if (dtd->commentsRxStr.isEmpty())
 {
   commentsRx.setPattern("("+quotationStr+")");
 } else
 {
   commentsRx.setPattern("("+dtd->commentsRxStr+"|"+quotationStr+")");
 }
 
 QString str = startNode->tag->tagStr();
 QString tagStr = str;
 //Replace all the commented strings and the escaped quotation marks (\", \')
 // with spaces so they will not mess up our parsing
 int pos = 0;
 int l;

 /* parse this special node for scripts. If a script is found inside, then
 replace the script area with spaces, so it won't mess up our block searching.
 */
 if (scriptParser(startNode))
 {
    int col = 0;
    QString specialEndStr;
      while (pos != -1)
      {
        pos = str.find(m_dtd->specialAreaStartRx, col); //FIXME: m_dtd nem jo...
        if (pos != -1)
        {
          QString foundText = m_dtd->specialAreaStartRx.cap();
          for (uint i = 0; i < m_dtd->specialAreaBegin.count(); i++)
          {
            if (m_dtd->specialAreaBegin[i] == foundText)
            {
              specialEndStr = m_dtd->specialAreaEnd[i];
              break;
            }
          }
          l = str.find(specialEndStr, pos);
          for (int j = pos; j < l ; j++)
          {
            str[j] = ' ';
          }
          col = l;
        }
      }
  }
     
 pos = 0; 
 while (pos != -1)
 {
   pos = commentsRx.search(str, pos);
   if (pos != -1)
   {
    l = commentsRx.matchedLength();
    for (int i = pos; i < pos + l ; i++)
    {
      str[i] = ' ';
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
   }
 }
 
 //kdDebug(24000) << "str: " << str << "\n";
 
  QRegExp keywordRx(dtd->structKeywordsRx);
  QRegExp startEndRx(dtd->structRx);
  QString s, name;
  pos = 0;
  int lastPos = 0;
  int lastPos2;
  int bLine, bCol, eLine, eCol;
  int el, ec; //the end coordinates of the root node
  int startPos; // where the sturcture really starts (e.g in "function {}")
  int matchNum;
  startNode->tag->beginPos(bLine, bCol);
  Tag *tag;
  Node *node;
  Node *currentNode = 0L;
  Node *rootNode = startNode;
  bool findStruct; //true if there is a need to search for the structure end
  int n;
  
  while (pos != -1)
  {
    pos  = startEndRx.search(str, lastPos);
    rootNode->tag->endPos(el, ec);
    if (pos != -1)
    {
      if (startEndRx.cap() == dtd->structBeginStr)
      {   
        startPos = keywordRx.searchRev(str, pos);
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
      if (!s.simplifyWhiteSpace().isEmpty())
      {
        tag = new Tag();
        tag->setStr(s.replace(commentsRx,"").stripWhiteSpace());
        tag->setWrite(write);
        tag->setTagPosition(bLine, bCol, eLine, eCol);
        tag->type = Tag::Text;
        tag->single = true;
       
        if ( bLine > el ||               //the beginning of the tag is after the end of the
            (bLine == el && bCol > ec) ) //root, so go up one level
        {
          currentNode = rootNode;
          rootNode = rootNode->parent;
          rootNode->tag->endPos(el, ec); //get the end coordinates for the new root node
        } 
        node = new Node(rootNode);
        if (!rootNode->child)
        {
          rootNode->child = node;
          node->prev = 0L;
        } else
        {
          if (!currentNode)
          {
            currentNode = rootNode->child;
            while (currentNode->next)
               currentNode = currentNode->next;
          }
          node->prev = currentNode;
          currentNode->next = node;
        }
        node->tag = tag;
        
        //scriptParser(node);
        
        currentNode = node;
      } 
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
          pos = startEndRx.search(str, lastPos2);
          if (pos != -1)
          {
            s = startEndRx.cap();       
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
        bLine += name.contains("\n");
        pos = 0;
        if (matchNum != 0) 
            lastPos2 = str.length();

        tag = new Tag();
        s = tagStr.mid(startPos, lastPos2 - startPos);
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
        tag->setStr(s);
        tag->setWrite(write);
        tag->setTagPosition(bLine, bCol, eLine, eCol);
        tag->type = Tag::ScriptStructureBegin;
        tag->single = true;

        if ( bLine > el ||                //the beginning of the tag is after the end of the
            (bLine == el && bCol > ec) )  //root, so go up one level
        {
          currentNode = rootNode;
          rootNode = rootNode->parent;
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
        
        currentNode = node;
        rootNode = node;
      } 
      
      bCol += name.length() + 1;
    }
  }

}


/** Parse the whole text from Document w and build the internal structure tree from Nodes */
Node *Parser::parse(Document *w)
{
  QTime t;
  t.start();
  
  write = w;
  m_dtdName = w->parsingDTD();
  m_dtd = dtds->find(m_dtdName);
  if (!m_dtd) m_dtd = dtds->find(qConfig.defaultDocType); //we should set this to projectDTD
  m_text = w->editIf->text();
//  if (m_node) delete m_node;
  int line = 0;
  int col = 0;
  maxLines = write->editIf->numLines();
  m_node = subParse(0L, line, col);

  parse2();

//  coutTree(m_node,0); //debug printout
  
  kdDebug(24000) << "Old parser: " << t.elapsed() << " ms\n";

  return m_node;
}

/** Print the doc structure tree to the standard output.
    Only for debugging purposes. */
void Parser::coutTree(Node *node, int indent)
{
 QString output;
 while (node)
 {
	output="";
   for (int i =0; i < indent; i++) output +=" ";
   if (node->tag->type != Tag::Text) output += node->tag->name;
   else output+= node->tag->tagStr();
   kdDebug(24000) <<output<<" (" << node->tag->type << ")"<<endl;
   if (node->child) coutTree(node->child, indent + 4);
   node = node->next;
 }
}

void Parser::parse2()
{
// Node * node = m_node;
}

/** Recursive parsing algorithm. Actually this does the parsing and tree building. */
Node * Parser::subParse( Node * parent, int &line, int &col )
{
  Node * node = 0L;
  Node * prevNode = 0L;
  Node * firstNode = 0L;
  while (line < maxLines)
  {
    Tag *tag = write->tagAt(m_dtd, line, col,true);
    if (tag)
    {
      switch ( tag->type )
      {
        case Tag::XmlTag :
             {
               if (prevNode && !prevNode->tag->name.startsWith("/"))
               {
                 prevNode->tag->closingMissing = true; //prevNode is single...
               }
               if ( parent )   // check if this tag stop area of previous
               {
                 QTag *qTag = QuantaCommon::tagFromDTD(m_dtd, parent->tag->name);
                 if ( qTag )
                 {
                   QString searchFor = (m_dtd->caseSensitive)?tag->name:tag->name.upper();
                   if ( qTag->stoppingTags.contains( searchFor ) )
                   {
                     parent->tag->closingMissing = true; //parent is single...
                     col--;
                     return firstNode;
                   }
                 }
               } //if (parent)

               Node *tnode = new Node( parent );
               if ( !firstNode ) firstNode = tnode;

               tnode->tag = tag;
               tnode->next = 0L;
               if ( prevNode ) prevNode->next = tnode;
               tnode->prev = prevNode;
               prevNode = tnode;

               if ( !tag->single && !QuantaCommon::isSingleTag(m_dtdName, tag->name) )  // not single tag
               {
                 tag->endPos(line, col);
                 nextPos(line, col);
                 tnode->child = subParse( tnode , line, col ); //tag can have other elements inside
               } else
               {
                 tag->endPos(line, col);
                }
               if (tnode->next) prevNode = tnode->next;
               break;
             } //case XMLTag
      case Tag::XmlTagEnd:
             {
               if (parent)
               {
//                 DTDStruct *dtd = dtds->find(m_dtdName);
                 QString startName = (m_dtd->caseSensitive) ? parent->tag->name: parent->tag->name.upper();
                 QString endName = (m_dtd->caseSensitive) ? tag->name: tag->name.upper();
                 if ("/"+startName == endName)
                 {
                   Node *tnode = new Node( parent->parent );
                   tnode->tag = tag;
                   parent->next = tnode;
                   tnode->prev = parent;
                   tag->endPos(line, col);
                   return firstNode;
                 }
               }
               break;
             }

      case Tag::Comment:
      case Tag::Text:
           {
             if (parent &&
                 parent->tag->name.lower() == "style" &&
                 parent->tag->attributeValue("type").lower() == "text/css")
             {
               node = new Node( parent );
               if ( !firstNode )
                   firstNode = node;
               if ( prevNode )
                   prevNode->next = node;
               tag->type = Tag::CSS;    
               node->tag = tag;
               node->prev = prevNode;
               node->next = 0L;
               prevNode = node;
               tag->endPos(line, col);
             } else
             {
               node = new Node( parent );
               if ( !firstNode )
                   firstNode = node;
               if ( prevNode )
                   prevNode->next = node;
               node->tag = tag;
               node->prev = prevNode;
               node->next = 0L;
               prevNode = node;
               tag->endPos(line, col);
             }
             break;
           }
      case Tag::ScriptStructureBegin:
           {
             node = new Node(parent);
             if ( !firstNode )
                 firstNode = node;
             if ( prevNode )
                 prevNode->next = node;
             node->tag = tag;
             node->prev = prevNode;
             node->next = 0L;
             prevNode = node;
             tag->endPos(line, col);
             nextPos(line, col);
             node->child = subParse( node , line, col );
             if (node->next) prevNode = node->next;
             break;
           }
      case Tag::ScriptStructureEnd:
           {
             if (parent)
             {
               Node *node = new Node( parent->parent );
               node->tag = tag;
               parent->next = node;
               node->prev = parent;
               tag->endPos(line, col);
               return firstNode;
             }
             break;
           }
      case Tag::Skip:
           {
             tag->endPos(line, col);
             delete tag;
             break;
           }

      }

    } //if tag;
    nextPos(line, col);
  } // while
  return firstNode;
}

/** Delete the internal m_node */
void Parser::deleteNode()
{
  if (m_node)
  {
    delete m_node;
    m_node = 0L;
  }
}

/** Go to the next column, or to the next line if we are at the end of line */
void Parser::nextPos(int &line, int &col)
{
  if (col < write->editIf->lineLength(line) - 1)
  {
      col++;
  } else
  {
      col = 0;
      line++;
  }
}
/** Clear the parser internal text, thus forcing the reparsing. */
void Parser::clear()
{
  m_text = "";
}

void Parser::rebuildDTDList()
{
  int delta = maxLines - oldMaxLines;
  uint line, col;
  write->viewCursorIf->cursorPositionReal(&line, &col);
  QString start, end;
  DTDListNode dtdNode;  
  for (uint i = 0; i < dtdList.count(); i++)
  {
    dtdNode = dtdList[i];
    if (dtdNode.bLine >= (int)line) dtdNode.bLine += delta;
    if (dtdNode.eLine >= (int)line) dtdNode.eLine += delta;
    if (dtdNode.startText.startsWith("<script") ||
        dtdNode.startText.startsWith("!doctype") )
    {
      start = write->editIf->textLine(dtdNode.bLine).remove(0, dtdNode.bCol - dtdNode.startText.length()).lower();
    } else
    {
      start = write->editIf->textLine(dtdNode.bLine).remove(0, dtdNode.bCol).lower();
    }
    end = write->editIf->textLine(dtdNode.eLine).remove(0, dtdNode.eCol).lower();
    if (!start.startsWith(dtdNode.startText) || !end.startsWith(dtdNode.endText) )
    {
      parseForDTD(write, true);
      break;
    }
    dtdList[i] = dtdNode;
  }
  oldMaxLines = maxLines;
}

/** Builds an internal tree to reflect the areas where each real & pseudo dtd is active. */
void Parser::parseForDTD(Document *w, bool force)
{
 write = w;
 maxLines = write->editIf->numLines();
 if (!force)
 {
   if (oldMaxLines != maxLines) rebuildDTDList();
   return;
 }
 oldMaxLines = maxLines;
 uint line = 0;
 uint col = 0;
 int pos = 0;
 uint startCol = 0;
 QString text = write->editIf->text();
 uint length = text.length();
 QString foundText;
 Tag *tag;
 QRegExp rx = scriptBeginRx;
 rx.setPattern(scriptBeginRx.pattern()+"|"+scriptEndRx.pattern()+"|\\!doctype");
 rx.setCaseSensitive(false);
 DTDListNode dtdNode;
 dtdList.clear();
 while (startCol < length && pos != -1)
 {
   pos = rx.search(text, startCol);
   if (pos != -1)
   {
     line = text.left(pos).contains("\n");
     foundText = rx.cap(0).lower();
     startCol = pos + foundText.length();
     col = pos - text.left(pos).findRev("\n")-1;
     if (foundText == "<script") //script begin
     {
       tag = write->findXMLTag(line, col, true);
       if (tag)
       {
         int eLine, eCol;
         dtdNode.startText = tag->tagStr().lower();
         foundText = tag->attributeValue("language").lower();
         tag->endPos(eLine, eCol);
         delete tag;
         dtdNode.dtd = dtds->find(foundText);
         dtdNode.eLine = -1;
         dtdNode.bLine = eLine;
         dtdNode.bCol = eCol+1;
         dtdList.append(dtdNode);
       }
     } else
       if (foundText == "/script>") //script end
       {
         for (int i = dtdList.count() -1; i >=0; i--)     //search for the first non-closed <script> tag
         {
           if (dtdList[i].startText.startsWith("<script") && dtdList[i].eLine == -1)
           {
             dtdList[i].eLine = line;
             dtdList[i].eCol = col;
             dtdList[i].endText = foundText;
             break;
           }
         }
       } else
       {
         if (foundText == "!doctype")
         {
           for (int i = dtdList.count() -1; i >=0; i--)     //search for the first non-closed <script> tag
           {
             if (dtdList[i].startText.startsWith("!doctype") && dtdList[i].eLine == -1)
             {
               dtdList[i].endText = "";
               dtdList[i].eLine = line;
               dtdList[i].eCol = col - 1;
               break;
             }
           }
           pos = write->editIf->textLine(line).findRev("<", col);
           tag = write->findXMLTag(line, pos, true);
           if (tag)
           {
             QString docText = tag->tagStr();
             dtdNode.startText = docText.lower();
             pos = docText.find("public",0,false);
             if (pos == -1) //if no PUBLIC info, use the word after !DOCTYPE as the doc.type
             {
               foundText = tag->attribute(0);
             } else
             {             //use the quoted string after PUBLIC as doc. type
               pos = docText.find("\"", pos+1);
               if (pos !=-1)
               {
                 int endPos = docText.find("\"",pos+1);
                 foundText = docText.mid(pos+1, endPos-pos-1);
               }
             }
             int eLine, eCol;
             tag->endPos(eLine, eCol);
             delete tag;
             dtdNode.dtd = dtds->find(foundText.lower());
             dtdNode.eLine = -1;
             dtdNode.bLine = eLine;
             dtdNode.bCol = eCol;
             dtdList.append(dtdNode);
           } // if (tag)
         } else
         {
           bool found = false;
           QDictIterator<DTDStruct> it(*dtds);
           for( ; it.current(); ++it )
           {
              DTDStruct *dtd = it.current();
              if (dtd->family == Script)
              {
                 int index = dtd->scriptTagStart.findIndex(foundText);
                 if (index !=-1)          //script begin
                 {
                   dtdNode.startText = foundText;
                   dtdNode.dtd = dtd;
                   dtdNode.eLine = -1;
                   dtdNode.bLine = line;
                   dtdNode.bCol = col;
                   dtdList.append(dtdNode);
                   found = true;
                   break;
                 }
                 index = dtd->scriptTagEnd.findIndex(foundText); //script end, search for the first non-closed script begin
                 if (index != -1)
                 {
                   for (int i = dtdList.count(); i >=0; i--)
                   {
                     //FIXME: Valgrind says there is an uninitalized variable used here
                     if (dtdList[i].dtd == dtd && dtdList[i].eLine == -1) 
                     {
                       dtdList[i].eLine = line;
                       dtdList[i].eCol = col;
                       dtdList[i].endText = foundText;
                       found = true;
                       break;
                     }
                   } //for i
                 }
              } // if (dtd.Family == Script)
             if (found) break;
           } //for iterator
         } //else
       } //else
   } //if (pos != -1)
 } //while
 for (int i = dtdList.count() -1; i >=0; i--)     //search for the first non-closed <script> tag
 {
   if (dtdList[i].eLine == -1)
   {
     dtdList[i].eLine = maxLines;
     dtdList[i].eCol = 0;
   }
 }
}

/** No descriptions */
DTDStruct * Parser::currentDTD(int line, int col)
{
  DTDStruct *dtd = 0L;
  DTDListNode dtdNode;
  for (uint i = 0; i < dtdList.count(); i++)
  {
    dtdNode = dtdList[i];
    if (QuantaCommon::isBetween(line, col, dtdNode.bLine,dtdNode.bCol, dtdNode.eLine, dtdNode.eCol) == 0)
    {
      dtd = dtdNode.dtd;
    }
  }

  return dtd;
}
