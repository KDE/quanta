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

Parser::Parser()
{
  m_text = QString::null;
  m_node = 0L;
}

Parser::~Parser()
{
}

/** Parse the whole text from Document w and build the internal structure tree from Nodes */
Node *Parser::parse(Document *w)
{
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

  return m_node;
}

/** Print the doc structure tree to the standard output.
    Only for debugging purposes. */
void Parser::coutTree(Node *node, int indent)
{
 while (node)
 {
   for (int i =0; i < indent; i++)
    cout << " ";
   if (node->tag->type != Tag::Text) cout << node->tag->name;
   else cout << node->tag->tagStr();
   cout << " (" << node->tag->type << ")\n";
   if (node->child) coutTree(node->child, indent + 4);
   node = node->next;
 }
}

void Parser::parse2()
{
 Node * node = m_node;
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


DTDNode *Parser::subParseForDTD(DTDNode *parent, int& line, int& col)
{
  DTDNode *node;
  QString text;
  QString foundText;
  int pos;
  Tag *tag;
  bool found ;
  int nextLine, nextCol;

  while (line < maxLines)
  {
    found = false;
    text = write->editIf->textLine(line);
    pos = scriptBeginRx.search(text, col);
    if (pos != -1) //a script DTD has been found
    {
      foundText = scriptBeginRx.cap(0).lower();
      if (foundText.startsWith("<script"))
      {
        tag = write->findXMLTag(line, pos, true);
        if (tag)
        {
          foundText = tag->attributeValue("language");
          tag->endPos(nextLine, nextCol);
          delete tag;
        }
      } else
      {
         QDictIterator<DTDStruct> it(*dtds);
         for( ; it.current(); ++it )
         {
            DTDStruct *dtd = it.current();
            if (dtd->family == Script)
            {
               int index = dtd->scriptTagStart.findIndex(foundText);
               if (index !=-1)
               {
                 foundText = dtd->name;
                 break;
               }
            }
         }
       }
       found = true;
       node = new DTDNode;
       node->bLine = line;
       node->bCol = pos;
       node->name = foundText.lower();
       node->parent = parent;
//       node->child = subParseForDTD(node, nextLine, nextCol+1);
     }  //if (pos !=-1)

  } //while
  return node;
}


/** Builds an internal tree to reflect the areas where each real & pseudo dtd is active. */
void Parser::parseForDTD(Document *w)
{
 write = w;
 uint line, col;
 line = 0;
 int pos = 0;
 maxLines = write->editIf->numLines();
 QString text = write->editIf->text();
 uint length = text.length();
 QString foundText;
 col = 0;
 Tag *tag;
 QRegExp rx = scriptBeginRx;
 rx.setPattern(scriptBeginRx.pattern()+"|\\!doctype");
 DTDListNode dtdNode;
 dtdList.clear();
 while (col < length && pos != -1)
 {
   pos = rx.search(text, col);
   if (pos != -1) //a script definition was found
   {
     line = text.left(pos).contains("\n");
     foundText = rx.cap(0).lower();
     col = pos + foundText.length();
     if (foundText.startsWith("<script"))
     {
       tag = write->findXMLTag(line, pos, true);
       if (tag)
       {
         foundText = tag->attributeValue("language").lower();
         delete tag;
       }
     } else
     {
       if (foundText.startsWith("!doctype"))
       {
         //TODO:
       } else
       {
         QDictIterator<DTDStruct> it(*dtds);
         for( ; it.current(); ++it )
         {
            DTDStruct *dtd = it.current();
            if (dtd->family == Script)
            {
               int index = dtd->scriptTagStart.findIndex(foundText);
               if (index !=-1)
               {
                 foundText = dtd->name.lower();
                 break;
               }
            }
         }
       }
     }
     dtdNode.line = line;
     dtdNode.col = col;
     dtdNode.name = foundText;
     dtdList.append(dtdNode);
   }
 }


}
