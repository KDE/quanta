/***************************************************************************
                               kafkacommon.cpp
                             -------------------

    copyright            : (C) 2003, 2004 - Nicolas Deschildre
    email                : ndeschildre@kdewebdev.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <qptrdict.h>

#include <kdebug.h>
#include <dom/dom_exception.h>
#include <dom/dom_doc.h>
#include <dom/dom_element.h>
#include <dom/dom_text.h>

#include "node.h"
#include "tag.h"
#include "document.h"
#include "resource.h"
#include "quantacommon.h"

#include "kafkacommon.h"
#include "wkafkapart.h"
#include "undoredo.h"
#include "cursors.h"

#include <cassert>

Node *kafkaCommon::getNextNode(Node *node, bool &goUp, Node *endNode)
{
    //goto next node, my favorite part :)
    if(!node || node == endNode)
        return 0L;
    if(goUp)
    {
        if(node->next)
        {
            goUp = false;
            if(node->next == endNode)
                return 0L;
            return node->next;
        }
        else
        {
            if(node->parent == endNode)
                return 0L;
            return getNextNode(node->parent, goUp);
        }
    }
    else
    {
        if(node->child)
        {
            if(node->child == endNode)
                return 0L;
            return node->child;
        }
        else if(node->next)
        {
            if(node->next == endNode)
                return 0L;
            return node->next;
        }
        else
        {
            goUp = true;
            if(node->parent == endNode)
                return 0L;
            return getNextNode(node->parent, goUp);
        }
    }
}

Node* kafkaCommon::getNextNodeNE(Node *node, bool &goUp, Node *endNode)
{
    Node *n = node;
    n = getNextNode(n, goUp, endNode);
    while(n && n->tag->type == Tag::Empty)
        n = getNextNode(n, goUp, endNode);
    return n;
}

Node* kafkaCommon::getPrevNode(Node *node, Node *endNode)
{
    Node *n = node;

    if(!node)
        return 0L;

    if(n->prev && n->prev->child)
    {
        n = n->prev;
        if(n == endNode)
            return 0L;
        while(n->child)
        {
            n = n->child;
            while(n && n->next)
                n = n->next;
            if(n == endNode)
                return 0L;
        }
    }
    else if(n->prev)
    {
        n = n->prev;
        if(n == endNode)
            return 0L;
    }
    else
    {
        n = n->parent;
        if(n == endNode)
            return 0L;
    }
    return n;
}

Node* kafkaCommon::getPrevNodeNE(Node *node, Node *endNode)
{
    Node *n = node;
    n = getPrevNode(node, endNode);
    while(n && n->tag->type == Tag::Empty)
        n = getPrevNode(n, endNode);
    return n;
}

Node* kafkaCommon::DTDGetCommonParent(Node* startNode, Node* endNode,
                                      QValueList<int>& commonParentStartChildLocation,
                                      QValueList<int>& commonParentEndChildLocation, Node* nodeSubtree)
{
    // look for commonParent
    Node* commonParent = 0;
    Node* commonParentStartChild = 0, *commonParentEndChild = 0;
    int locOffset = 1;
    QValueList<int> startNodeLocation = getLocation(startNode);
    QValueList<int> endNodeLocation = getLocation(endNode);
    QValueList<int>::iterator itStart = startNodeLocation.begin();
    QValueList<int>::iterator itEnd = endNodeLocation.begin();
    while(itStart != startNodeLocation.end() && itEnd != endNodeLocation.end() &&
          (*itStart) == (*itEnd))
    {
        commonParent = getNodeFromSubLocation(startNodeLocation, locOffset, nodeSubtree);
        itStart++;
        itEnd++;
        locOffset++;
    }

    //look for commonParentStartChild and commonParentEndChild
    if(itStart != startNodeLocation.end())
        commonParentStartChild = getNodeFromSubLocation(startNodeLocation, locOffset, nodeSubtree);
    else
        commonParentStartChild = commonParent;

    if(itEnd != endNodeLocation.end())
        commonParentEndChild = getNodeFromSubLocation(endNodeLocation, locOffset, nodeSubtree);
    else
        commonParentEndChild = commonParent;

    //If commonParent isn't inline, move commonParent to the closest non inline node
    if(commonParent && (commonParent->tag->type == Tag::Text || commonParent->tag->type == Tag::Empty))
    {
        Node* oldCommonParent = commonParent;
        commonParent = commonParent->parent;
        commonParentStartChild = oldCommonParent;
        commonParentEndChild = oldCommonParent;
    }
    //startNode or endNode can't be the commonParent.
    else if(commonParent && (itStart == startNodeLocation.end() || itEnd == endNodeLocation.end()))
        commonParent = commonParent->parent;

    commonParentStartChildLocation = getLocation(commonParentStartChild);
    commonParentEndChildLocation = getLocation(commonParentEndChild);

    return commonParent;
}

Node* kafkaCommon::DTDGetNonInlineCommonParent(Node* startNode, Node* endNode,
                                               QValueList<int>& commonParentStartChildLocation,
                                               QValueList<int>& commonParentEndChildLocation, Node* nodeSubtree)
{
    // look for commonParent
    Node* commonParent = 0;
    Node* commonParentStartChild = 0, *commonParentEndChild = 0;
    int locOffset = 1;
    QValueList<int> startNodeLocation = getLocation(startNode);
    QValueList<int> endNodeLocation = getLocation(endNode);
    QValueList<int>::iterator itStart = startNodeLocation.begin();
    QValueList<int>::iterator itEnd = endNodeLocation.begin();
    while(itStart != startNodeLocation.end() && itEnd != endNodeLocation.end() &&
          (*itStart) == (*itEnd))
    {
        commonParent = getNodeFromSubLocation(startNodeLocation, locOffset, nodeSubtree);
        itStart++;
        itEnd++;
        locOffset++;
    }

    //look for commonParentStartChild and commonParentEndChild
    if(itStart != startNodeLocation.end())
        commonParentStartChild = getNodeFromSubLocation(startNodeLocation, locOffset, nodeSubtree);
    else
        commonParentStartChild = commonParent;

    if(itEnd != endNodeLocation.end())
        commonParentEndChild = getNodeFromSubLocation(endNodeLocation, locOffset, nodeSubtree);
    else
        commonParentEndChild = commonParent;

    //If commonParent isn't inline, move commonParent to the closest non inline node
    if(commonParent && (isInline(commonParent->tag->name) ||
       commonParent->tag->type == Tag::Text || commonParent->tag->type == Tag::Empty))
    {
        Node* oldCommonParent = commonParent;
        commonParent = commonParent->parent;
        while(commonParent && isInline(commonParent->tag->name))
        {
            oldCommonParent = commonParent;
            commonParent = commonParent->parent;
        }
        commonParentStartChild = oldCommonParent;
        commonParentEndChild = oldCommonParent;
    }
    //startNode or endNode can't be the commonParent.
    else if(commonParent && (itStart == startNodeLocation.end() || itEnd == endNodeLocation.end()))
        commonParent = commonParent->parent;

    commonParentStartChildLocation = getLocation(commonParentStartChild);
    commonParentEndChildLocation = getLocation(commonParentEndChild);

    return commonParent;
}

DOM::Node kafkaCommon::getNextDomNode(DOM::Node node, bool &goUp, bool returnParentNode, DOM::Node endNode)
{
    if(node.isNull())
        return 0L;
    if(node.hasChildNodes() && !goUp)
    {
        if(endNode == node.firstChild())
            return 0L;
        else
            return node.firstChild();
    }
    else if(!node.nextSibling().isNull())
    {
        goUp = false;
        if(endNode == node.nextSibling())
            return 0L;
        else
            return node.nextSibling();
    }
    else
    {
        goUp = true;
        if(node.parentNode().isNull() || endNode == node.parentNode())
            return 0L;
        if(returnParentNode)
            return node.parentNode();
        else
            return getNextDomNode(node.parentNode(), goUp, returnParentNode, endNode);
    }
}

DOM::Node kafkaCommon::getPrevDomNode(DOM::Node node, DOM::Node endNode)
{
    DOM::Node n = node;

    if(node.isNull())
        return 0L;

    if(!n.previousSibling().isNull() && !n.previousSibling().firstChild().isNull())
    {
        n = n.previousSibling();
        if(n == endNode)
            return 0L;
        while(!n.firstChild().isNull())
        {
            n = n.firstChild();
            while(!n.isNull() && !n.nextSibling().isNull())
                n = n.nextSibling();
            if(n == endNode)
                return 0L;
        }
    }
    else if(!n.previousSibling().isNull())
    {
        n = n.previousSibling();
        if(n == endNode)
            return 0L;
    }
    else
    {
        n = n.parentNode();
        if(n == endNode)
            return 0L;
    }
    return n;
}

Node* kafkaCommon::getCorrectStartNode(Node* startNode, int& start_offset)
{
    Node* start_node = startNode;

    while(start_node && (start_node->tag->type != Tag::Text || (uint)start_offset == start_node->tag->tagStr().length()))
    {
        start_node = start_node->nextSibling();
        if(start_node->tag->type == Tag::Text || start_node->tag->type == Tag::Empty)
        {
            start_offset = 0;
            break;
        }
    }

    return start_node;
}

Node* kafkaCommon::getCorrectEndNode(Node* endNode, int& end_offset)
{
    Node* end_node = endNode;

    while(end_node && (end_node->tag->type != Tag::Text || end_offset == 0))
    {
        end_node = end_node->previousSibling();
        if(end_node && end_node->tag->type == Tag::Text)
        {
            end_offset = end_node->tag->tagStr().length();
            break;
        }
    }

    return end_node;
}

Node* kafkaCommon::getCommonParentChild(Node* node, Node* commonParent)
{
    assert(node && commonParent);

    Node* aux = commonParent->child;
    assert(aux);

    while(aux && aux != node)
    {
        if(aux->hasForChild(node))
            return aux;
        aux = aux->nextSibling();
    }
    return aux;
}


void kafkaCommon::applyIndentation(Node *node, int nbOfSpaces, int nbOfTabs, NodeModifsSet* modifs, bool inlineNodeIndentation)
{
#ifdef LIGHT_DEBUG
    kdDebug(25001)<< "kafkaCommon::applyIndentation()" << endl;
#endif

    Node *parent, *nextNE, *prevNE, *realPrevNE, *realNextNE, *realPrev, *realNext, *prev, *next;
    int nonInlineDepth = 0, nonInlineDepth2 = 0, i;
    bool b = false;
    QString indentation1, indentation2, text;

    if(!node)
        return;

    prev = node->previousSibling();
    next = node->nextSibling();
    prevNE = getPrevNodeNE(node);
    nextNE = getNextNodeNE(node, b);
    realPrevNE = node->prevNE();
    realNextNE = node->nextNE();
    realPrev = node->prev;
    realNext = node->next;

    if(inlineNodeIndentation &&
       !node->prev && getNodeDisplay(node->parent, true) == kafkaCommon::blockDisplay)
    {
        AreaStruct node_area = node->tag->area();
        AreaStruct parent_area = node->parent->tag->area();

        if(node_area.bLine == parent_area.bLine)
        {
            node->tag->setIndentationDone(true);
            return;
        }
    }

    //First remove all the indentation
    if(node->tag->type == Tag::Text)
        setTagString(node, removeUnnecessaryWhitespaces(node->tag->tagStr()), modifs);

    //compute the "non-inline depth" of the Node and of the next NE (not Empty) Node
    // i.e. we count how many non-inline parent they have.
    parent = node->parent;
    while(parent)
    {
        if(getNodeDisplay(parent, true) == kafkaCommon::blockDisplay)
            ++nonInlineDepth;
        parent = parent->parent;
    }

    //compute the "non-inline depth" of the next non-empty Node.
    if (nextNE)
        parent = nextNE->parent;
    else
        parent = 0L;
    while(parent)
    {
        if(getNodeDisplay(parent, true) == kafkaCommon::blockDisplay)
            ++nonInlineDepth2;
        parent = parent->parent;
    }

    parent = node->parent;

    if(!parent || getNodeDisplay(parent, true) == kafkaCommon::blockDisplay)
    {
        //prepare the indentation
        indentation1 = "\n";
        indentation2 = "\n";

        if(nbOfSpaces == 0) // tabs are used
        {
            indentation1 += QString().fill('\t', nbOfTabs * nonInlineDepth);
            indentation2 += QString().fill('\t', nbOfTabs * nonInlineDepth2);
        }
        else // spaces are used
        {
            indentation1 += QString().fill(' ', nbOfSpaces * nonInlineDepth);
            indentation2 += QString().fill(' ', nbOfSpaces * nonInlineDepth2);
        }

        //test and add indentations if necessary
        if(!prevNE || (prevNE && getNodeDisplay(node, true) ==
                       kafkaCommon::blockDisplay) ||
                (prevNE && getNodeDisplay(node, true) == kafkaCommon::inlineDisplay &&
                 getNodeDisplay(prevNE, true) == kafkaCommon::blockDisplay))
        {
            if(node->tag->type == Tag::Text && !hasParent(node, "pre"))
            {
                setTagStringAndFitsNodes(node, indentation1 + node->tag->tagStr(), modifs);
            }
            else if(prev && prev->tag->type == Tag::Empty)
            {
                setTagStringAndFitsNodes(prev, indentation1, modifs);
            }
            //The indentation is always done at the left because we apply this function "from left to right"
            else if(prev && prev->tag->type == Tag::Text /** && prev->tag->indentationDone() */)
            {
                //Remove the indentation at the right of the text Node
                text = prev->tag->tagStr();
                for(i = 0; (unsigned)i < text.length(); ++i)
                {
                    if(!text[i].isSpace())
                        break;
                }
                if(i == 0)
                    prev->tag->setStr(removeUnnecessaryWhitespaces(text));
                else
                    prev->tag->setStr(text.mid(0, i) + removeUnnecessaryWhitespaces(text, true));
                setTagStringAndFitsNodes(prev, prev->tag->tagStr() + indentation1, modifs);
            }
        }

        if(!nextNE || (nextNE && getNodeDisplay(node, true) ==
                       kafkaCommon::blockDisplay) ||
                (nextNE && getNodeDisplay(node, true) == kafkaCommon::inlineDisplay &&
                 getNodeDisplay(nextNE, true) == kafkaCommon::blockDisplay))
        {
            if(node->tag->type == Tag::Text && !hasParent(node, "pre"))
            {
                setTagStringAndFitsNodes(node, node->tag->tagStr() + indentation2, modifs);
            }
            else if(next && next->tag->type == Tag::Empty)
            {
                setTagStringAndFitsNodes(next, indentation2, modifs);
            }
            //If next's cleanStrBuilt is not true, the next node to be processed will be this
            //one and the indentation spaces will be handled as real spaces.
            else if(next && next->tag->type == Tag::Text && next->tag->indentationDone())
            {
                //Remove the indentation at the left of the text Node
                text = next->tag->tagStr();
                for(i = text.length() - 1; i <= 0; i--)
                {
                    if(!text[i].isSpace())
                        break;
                }
                if((unsigned)i == text.length() - 1)
                    next->tag->setStr(removeUnnecessaryWhitespaces(text));
                else
                    next->tag->setStr(removeUnnecessaryWhitespaces(text, false, true) +
                                      text.mid(i + 1));
                setTagStringAndFitsNodes(next, indentation2 + next->tag->tagStr(), modifs);
            }
        }
    }
    else
    {
        //The parent is inline, so no indentation.
        //Nothing to do.
    }
    node->tag->setIndentationDone(true);
}

void kafkaCommon::fitIndentationNodes(Node *n1, Node *n2, NodeModifsSet *modifs)
{
#ifdef LIGHT_DEBUG
    kdDebug(25001)<< "kafkaCommon::fitIndentationNodes()" << endl;
#endif

    Node *parent, *child, *node, *emptyNode = 0L, *emptyNode2 = 0L;
    int nbEmptyNodes = 0, n1Depth, n2Depth;
    bool lastChild = false, firstChild = false;

    if(!n1 || !n2 || n1 == n2 || n1->tag->type == Tag::Empty || n2->tag->type == Tag::Empty)
        return;

    n1Depth = nodeDepth(n1);
    n2Depth = nodeDepth(n2);

    if(n1Depth != n2Depth)
    {
        if(n1Depth > n2Depth)
        {
            child = n1;
            parent = n2;
        }
        else
        {
            child = n2;
            parent = n1;
        }
        if(child->parent->firstChildNE() == child)
            firstChild = true;
        if(child->parent->lastChildNE() == child)
            lastChild = true;

        //counting the Empty Nodes and deleting them to have only one empty node.
        if(firstChild)
        {
            node = child->prev;
            while(node)
            {
                if(node->tag->type == Tag::Empty)
                    nbEmptyNodes++;
                node = node->prev;
            }
            node = child->prev;
            while(nbEmptyNodes > 1)
            {
                extractAndDeleteNode(node, modifs, false, false, false);
                nbEmptyNodes--;
                node = child->prev;
            }
            if(nbEmptyNodes == 1)
                emptyNode = child->prev;
        }

        nbEmptyNodes = 0;
        if(lastChild)
        {
            node = child->next;
            while(node)
            {
                if(node->tag->type == Tag::Empty)
                    nbEmptyNodes++;
                node = node->next;
            }
            node = child->next;
            while(nbEmptyNodes > 1)
            {
                extractAndDeleteNode(node, modifs, false, false, false);
                nbEmptyNodes--;
                node = child->next;
            }
            if(nbEmptyNodes == 1)
                emptyNode2 = child->next;
        }

        //adding/deleting a empty node if necessary
        if(firstChild)
        {
            if(getNodeDisplay(parent, true) == kafkaCommon::blockDisplay)
            {
                if(child->tag->type != Tag::Text && !emptyNode)
                {
                    createAndInsertNode("", "", Tag::Empty, n2->tag->write(), child->parent,
                                        child, child, modifs);
                }
            }
            else
            {
                if(child->tag->type == Tag::Text && emptyNode)
                {
                    extractAndDeleteNode(emptyNode, modifs, false, false, false);
                }
            }
        }

        if(lastChild)
        {
            if(getNodeDisplay(parent, true) == kafkaCommon::blockDisplay)
            {
                if(child->tag->type != Tag::Text && !emptyNode2)
                {
                    createAndInsertNode("", "", Tag::Empty, n2->tag->write(), child->parent,
                                        0L, 0L, modifs);
                }
            }
            else
            {
                if(child->tag->type == Tag::Text && emptyNode2)
                {
                    extractAndDeleteNode(emptyNode2, modifs, false, false, false);
                }
            }
        }
    }
    else
    {
        if(n1->next != n2)
        {
            //counting the Empty Nodes and deleting them to have only one empty node.
            node = n1->next;
            while(node && node != n2)
            {
                if(node->tag->type == Tag::Empty)
                    nbEmptyNodes++;
                node = node->next;
            }
            node = n1->next;
            while(nbEmptyNodes > 1 || (nbEmptyNodes > 0 && n1->getClosingNode() == n2))
            {
                extractAndDeleteNode(node, modifs, false, false, false);
                nbEmptyNodes--;
                node = n1->next;
            }
            if(nbEmptyNodes == 1)
                emptyNode = n1->next;

            if(n1->getClosingNode() == n2 && n1->child && n1->child->tag->type == Tag::Empty)
                emptyNode = n1->child;
        }

        //adding/deleting a empty node if necessary
        parent = n1->parent;
        if(!parent || getNodeDisplay(parent, true) == kafkaCommon::blockDisplay)
        {
            if(getNodeDisplay(n1, true) == kafkaCommon::blockDisplay &&
                    n1->tag->type != Tag::Text)
            {
                if(n2->tag->type == Tag::Text && emptyNode)
                {
                    extractAndDeleteNode(emptyNode, modifs, false, false, false);
                }
                else if(n2->tag->type != Tag::Text && !emptyNode)
                {
                    if(n1->getClosingNode() == n2)
                    {
                        createAndInsertNode("", "", Tag::Empty, n2->tag->write(), n1, 0L, 0L, modifs);
                    }
                    else
                    {
                        createAndInsertNode("", "", Tag::Empty, n2->tag->write(), parent, n2, n2, modifs);
                    }
                }
            }
            else
            {
                if((n2->tag->type == Tag::Text ||
                        getNodeDisplay(n2, true) == kafkaCommon::inlineDisplay) &&
                        emptyNode)
                {
                    extractAndDeleteNode(emptyNode, modifs, false, false, false);
                }
                else if(n2->tag->type != Tag::Text &&
                        getNodeDisplay(n2, true) == kafkaCommon::blockDisplay &&
                        n1->tag->type != Tag::Text && !emptyNode)
                {
                    if(n1->getClosingNode() == n2)
                    {
                        createAndInsertNode("", "", Tag::Empty, n2->tag->write(), n1, 0L, 0L, modifs);
                    }
                    else
                    {
                        createAndInsertNode("", "", Tag::Empty, n2->tag->write(), parent, n2, n2, modifs);
                    }
                }
            }
        }
        else
        {
            if(emptyNode)
                extractAndDeleteNode(emptyNode, modifs, false, false, false);
        }
    }
}

void kafkaCommon::fitsNodesPosition(Node* startNode, int colMovement, int lineMovement, int colEnd, int lineEnd)
{
    bool b = false;
    int j, SNbeginLine, SNbeginCol/**, SNlastLine, SNlastCol*/;
    int beginLine, beginCol, lastLine, lastCol;
    Node *node = startNode;

    if(!node)
        return;

    node->tag->beginPos(SNbeginLine, SNbeginCol);
    //node->tag->endPos(SNlastLine, SNlastCol);

    while(node)
    {
        node->tag->beginPos(beginLine, beginCol);
        node->tag->endPos(lastLine, lastCol);
        if(beginLine >= lineEnd && beginCol >= colEnd &&
                colEnd != -2 && lineEnd != -2)
            return;
        if(beginLine == SNbeginLine && lastLine == SNbeginLine)
            node->tag->setTagPosition(beginLine + lineMovement,
                                      beginCol + colMovement, lastLine + lineMovement,
                                      lastCol + colMovement);
        else if(beginLine == SNbeginLine)//&&lastLine != SNbeginLine
            node->tag->setTagPosition(beginLine + lineMovement,
                                      beginCol + colMovement, lastLine + lineMovement,
                                      lastCol);
        else
            node->tag->setTagPosition(beginLine + lineMovement,
                                      beginCol, lastLine + lineMovement, lastCol);
        for(j = 0; j < node->tag->attrCount(); ++j)
        {
            if(node->tag->getAttribute(j).nameLine == SNbeginLine)
            {
                node->tag->getAttribute(j).nameLine += lineMovement;
                node->tag->getAttribute(j).nameCol += colMovement;
                node->tag->getAttribute(j).valueLine += lineMovement;
                node->tag->getAttribute(j).valueCol += colMovement;
            }
            else
            {
                node->tag->getAttribute(j).nameLine += lineMovement;
                node->tag->getAttribute(j).valueLine += lineMovement;
            }
        }
        node = getNextNode(node, b);
    }
}

int kafkaCommon::getNodeDisplay(Node *node, bool closingNodeToo)
{
    QString nodeName;

    if(!node)
        return kafkaCommon::errorDisplay;

    if(node->tag->type == Tag::Text)
        return kafkaCommon::inlineDisplay;
    else if(node->tag->type == Tag::XmlTag || (node->tag->type == Tag::XmlTagEnd &&
            closingNodeToo))
    {
        //If we areusing a non (X)HTML DTD, make everything blockDisplay by default
        if(node->tag->dtd() && node->tag->dtd()->name.contains("HTML", false) == 0)
            return kafkaCommon::blockDisplay;

        nodeName = node->tag->name.lower();
        if(closingNodeToo && nodeName.startsWith("/"))
            nodeName = nodeName.mid(1);
        if(nodeName == "html" || nodeName == "head" || nodeName == "meta" ||
                nodeName == "link" || nodeName == "style" || nodeName == "option" ||
                nodeName == "optgroup" || nodeName == "area" || nodeName == "param" ||
                nodeName == "thead" || nodeName == "tbody" || nodeName == "dt" ||
                nodeName == "tfoot" || nodeName == "col" || nodeName == "colgroup" ||
                nodeName == "tr" || nodeName == "td" || nodeName == "th" || nodeName == "caption" ||
                nodeName == "ins" || nodeName == "legend")
            //Ok right, but this is only for indentation...
            //return kafkaCommon::noneDisplay;
            return kafkaCommon::blockDisplay;
        else if(nodeName == "body" || nodeName == "p" || nodeName == "div" ||
                nodeName == "blockquote" || nodeName == "isindex" ||
                nodeName == "center" || nodeName == "hr" || nodeName == "h1" ||
                nodeName == "h2" || nodeName == "h3" || nodeName == "h4" || nodeName == "h5" ||
                nodeName == "h6" || nodeName == "table" ||
                nodeName == "ul" || nodeName == "menu" || nodeName == "dir" || nodeName == "ol" ||
                nodeName == "li" || nodeName == "ul" || nodeName == "dd" || nodeName == "dl" ||
                nodeName == "form" || nodeName == "fieldset" ||
                nodeName == "pre" || nodeName == "noscript" || nodeName == "noframes" ||
                nodeName == "frameset" || nodeName == "frame" ||
                nodeName == "address" || nodeName == "del" || nodeName == "br")
            return kafkaCommon::blockDisplay;
        else if(nodeName == "q" || nodeName == "u" || nodeName == "i" || nodeName == "b" ||
                nodeName == "cite" || nodeName == "em" || nodeName == "var" || nodeName == "em" ||
                nodeName == "tt" || nodeName == "code" || nodeName == "kbd" || nodeName == "samp" ||
                nodeName == "big" || nodeName == "small" || nodeName == "s" || nodeName == "strike" ||
                nodeName == "sub" || nodeName == "sup" || nodeName == "abbr" || nodeName == "title" ||
                nodeName == "acronym" || nodeName == "a" || nodeName == "bdo" ||
                nodeName == "font" || nodeName == "#text" || nodeName == "strong" || nodeName == "dfn" ||
                nodeName == "img" ||  nodeName == "applet" ||  nodeName == "object" ||  nodeName == "basefont"  ||  nodeName == "script" ||  nodeName == "map" || nodeName == "span" ||
                nodeName == "iframe" || nodeName == "input" || nodeName == "select" || nodeName == "textarea" ||
                nodeName == "label" || nodeName == "button" )
            return kafkaCommon::inlineDisplay;
        else
        {
#ifdef LIGHT_DEBUG
            kdDebug(25001)<< "kafkaCommon::getNodeType() - ERROR " << nodeName <<
            " not found" << endl;
#endif

            return kafkaCommon::noneDisplay;
        }
    }
    return kafkaCommon::errorDisplay;
}

QString kafkaCommon::removeUnnecessaryWhitespaces(const QString &string,
        bool removeAllSpacesAtTheLeft, bool removeAllSpacesAtTheRight)
{
    /**QString newString;
    int i;

    if(string.length() == 0)
    	return "";

    newString = string[0];
    for(i = 1; (unsigned)i < string.length(); ++i)
    {
    	if(!string[i - 1].isSpace() || !string[i].isSpace())
    		newString += string[i];
    }

    if(removeAllSpacesAtTheLeft && newString.length() > 0 && newString[0].isSpace())
    	newString = newString.mid(1);
    if(removeAllSpacesAtTheRight && newString.length() > 0 &&
    	newString[newString.length() - 1].isSpace())
    	newString = newString.mid(0, newString.length() - 1);

    return newString;*/
    QString newString;
    bool hasLeftWhiteSpaces, hasRightWhiteSpaces;

    if(string.length() == 0)
        return QString();

    hasLeftWhiteSpaces = (string[0].isSpace());
    hasRightWhiteSpaces = (string[string.length() - 1].isSpace());

    newString = string.stripWhiteSpace();
    if(hasLeftWhiteSpaces && !removeAllSpacesAtTheLeft)
        newString.insert(0, " ");
    if(hasRightWhiteSpaces && !removeAllSpacesAtTheRight)
        newString.insert(newString.length(), " ");

    return newString;
}

Node* kafkaCommon::createNode(const QString &nodeName, const QString &tagString, int nodeType, Document *doc)
{
    Node *node;

    //create the Node.
    node = new Node(0L);

    //Create the corresponding Tag.
    node->tag = new Tag();
    if(doc)
        node->tag->setDtd(doc->defaultDTD());
    else
        node->tag->setDtd(0L);
    node->tag->setWrite(doc);
    node->tag->type = nodeType;
    node->tag->name = QuantaCommon::tagCase(nodeName);
    if(doc)
        node->tag->single = QuantaCommon::isSingleTag(doc->defaultDTD()->name, nodeName);
    else
        node->tag->single = false;
    node->tag->setStr(tagString);
    node->tag->setCleanStrBuilt(false);
    node->tag->setIndentationDone(false);
    return node;
}

void kafkaCommon::restorePastedNode(Node* node, Document* doc)
{
    if(doc)
        node->tag->setDtd(doc->defaultDTD());
    else
        node->tag->setDtd(0L);

    node->tag->setWrite(doc);

}

Node *kafkaCommon::createDoctypeNode(Document *doc)
{
    Node *node, *child, *closingNode;

    if(!doc)
        return 0L;

    //Build the script Tag
    node = kafkaCommon::createNode("DTD block", "", Tag::ScriptTag, doc);
    closingNode = kafkaCommon::createNode("", "", Tag::XmlTagEnd, doc);
    node->next = closingNode;
    closingNode->prev = node;

    //Then build the Script tag which will be child of the above node.
    child = kafkaCommon::createNode("#text", "DOCTYPE" + doc->defaultDTD()->doctypeStr, Tag::Text, doc);
    child->tag->setCleanStrBuilt(true);
    child->insideSpecial = true;
    insertNode(child, node, 0L, 0L, false);

    return node;
}

Node *kafkaCommon::createXmlDeclarationNode(Document *doc, const QString &encoding)
{
    Node *node, *child, *closingNode;
    QString text;

    if(!doc)
        return 0L;

    //build the script Tag
    node = kafkaCommon::createNode("XML PI block" ,"", Tag::ScriptTag, doc);
    closingNode = kafkaCommon::createNode("", "", Tag::XmlTagEnd, doc);
    node->next = closingNode;
    closingNode->prev = node;

    //Then build the Text tag which will be child of the above node.
    text = " encoding=\"" + encoding + "\" version=\"1.0\"";
    child = kafkaCommon::createNode("#text", text, Tag::Text, doc);
    child->tag->setCleanStrBuilt(true);
    child->insideSpecial = true;
    insertNode(child, node, 0L, 0L, false);

    return node;
}

Node* kafkaCommon::createMandatoryNodeSubtree(Node *node, Document *doc)
{
    QTag *nodeQTag, *oldNodeQTag;
    bool searchForMandatoryNode;
    Node *currentParent;
    QMap<QString, bool>::iterator it;

    if(!node)
        return 0L;

    nodeQTag = QuantaCommon::tagFromDTD(node);
    if(!nodeQTag)
        return false;

    searchForMandatoryNode = true;
    currentParent = node;
    while(searchForMandatoryNode)
    {
        oldNodeQTag = nodeQTag;
        for(it = nodeQTag->childTags.begin(); it != nodeQTag->childTags.end(); ++it)
        {
            if(it.data())
            {
                nodeQTag = QuantaCommon::tagFromDTD(nodeQTag->parentDTD, it.key());
                if(!nodeQTag)
                    return node;
                currentParent = createAndInsertNode(nodeQTag->name(), "", Tag::XmlTag, doc,
                                                    currentParent, 0L, 0L, (NodeModifsSet*)0L);
                break;
            }
        }
        if(oldNodeQTag == nodeQTag)
            searchForMandatoryNode = false;
    }

    return currentParent;
}

Node* kafkaCommon::insertNode(Node *node, Node* parentNode, Node* nextSibling,
                              NodeModifsSet *modifs, bool merge)
{
#ifdef LIGHT_DEBUG
    kdDebug(25001)<< "kafkaCommon::insertNode()" << endl;
#endif

    NodeModif* modif;
    Node *n, *closingNode;
    bool nodeIsFirstChild = false, b;

    if(!node)
        return 0L;

    //Reset the listviews items pointers for node and its children
    n = node;
    b = false;
    while(n)
    {
        /**node->mainListItem = 0L;
        node->listItems.clear();
        node->groupElementLists.clear();*/
        n = getNextNode(n, b);
    }

    //place the new Node.
    if(parentNode)
        n = parentNode->child;
    else
        n = baseNode;
    while(n && n->next)
        n = n->next;

    if(!parentNode && (!baseNode || (nextSibling && !nextSibling->prev)))
    {
        nodeIsFirstChild = true;
        baseNode = node;
        parser->setRootNode(baseNode);
    }
    if(parentNode && (!parentNode->child || nextSibling == parentNode->child))
    {
        nodeIsFirstChild = true;
        parentNode->child = node;
    }
    node->parent = parentNode;

    if(nextSibling && nextSibling->prev)
    {
        nextSibling->prev->next = node;
        node->prev = nextSibling->prev;
    }
    else if(n && !nodeIsFirstChild)
    {
        n->next = node;
        node->prev = n;
    }

    if(nextSibling)
        nextSibling->prev = node;
    node->next = nextSibling;

    //log this.
    if(modifs)
    {
        modif = new NodeModif();
        if(node->child)
            modif->setType(NodeModif::NodeAndChildsAdded);
        else
            modif->setType(NodeModif::NodeAdded);
        modif->setLocation(getLocation(node));
        modifs->addNodeModif(modif);
    }

    //Then try to merge with the siblings
    if(merge)
    {
        if(node->prev)
        {
            n = node->prev;
            if(mergeNodes(node->prev, node, modifs))
                node = n;
        }
        if(node->next)
        {
            mergeNodes(node, node->next, modifs);
        }
    }

    //update the closesPrevious switch
    closingNode = node->getClosingNode();
    if(closingNode)
        closingNode->closesPrevious = true;

#ifdef HEAVY_DEBUG

    coutTree(baseNode, 2);
#endif

    return node;
}

Node* kafkaCommon::insertNode(Node *node, Node* parentNode, Node* nextSibling, NodeSelection& cursorHolder,
                              NodeModifsSet *modifs, bool merge)
{
#ifdef LIGHT_DEBUG
    kdDebug(25001)<< "kafkaCommon::insertNode()" << endl;
#endif

    NodeModif* modif;
    Node *n, *closingNode;
    bool nodeIsFirstChild = false, b;

    if(!node)
        return 0L;

    //Reset the listviews items pointers for node and its children
    n = node;
    b = false;
    while(n)
    {
        /**node->mainListItem = 0L;
        node->listItems.clear();
        node->groupElementLists.clear();*/
        n = getNextNode(n, b);
    }

    //place the new Node.
    if(parentNode)
        n = parentNode->child;
    else
        n = baseNode;
    while(n && n->next)
        n = n->next;

    if(!parentNode && (!baseNode || (nextSibling && !nextSibling->prev)))
    {
        nodeIsFirstChild = true;
        baseNode = node;
        parser->setRootNode(baseNode);
    }
    if(parentNode && (!parentNode->child || nextSibling == parentNode->child))
    {
        nodeIsFirstChild = true;
        parentNode->child = node;
    }
    node->parent = parentNode;

    if(nextSibling && nextSibling->prev)
    {
        nextSibling->prev->next = node;
        node->prev = nextSibling->prev;
    }
    else if(n && !nodeIsFirstChild)
    {
        n->next = node;
        node->prev = n;
    }

    if(nextSibling)
        nextSibling->prev = node;
    node->next = nextSibling;

    //log this.
    if(modifs)
    {
        modif = new NodeModif();
        if(node->child)
            modif->setType(NodeModif::NodeAndChildsAdded);
        else
            modif->setType(NodeModif::NodeAdded);
        modif->setLocation(getLocation(node));
        modifs->addNodeModif(modif);
    }

    //Then try to merge with the siblings
    if(merge)
    {
        if(node->prev)
        {
            n = node->prev;
            if(mergeNodes(node->prev, node, cursorHolder, modifs))
                node = n;
        }
        if(node->next)
        {
            mergeNodes(node, node->next, cursorHolder, modifs);
        }
    }

    //update the closesPrevious switch
    closingNode = node->getClosingNode();
    if(closingNode)
        closingNode->closesPrevious = true;

#ifdef HEAVY_DEBUG

    coutTree(baseNode, 2);
#endif

    return node;
}

Node *kafkaCommon::insertNode(Node *newNode, Node *parent, Node *nextSibling, Node *nextEndSibling,
                              NodeModifsSet *modifs, bool merge)
{
#ifdef LIGHT_DEBUG
    kdDebug(25001)<< "kafkaCommon::insertNode()1" << endl;
#endif

    Node *n, *nodeEnd = 0;

    if(!newNode)
        return 0L;

    //place the new Node.
    newNode = insertNode(newNode, parent, nextSibling, modifs, merge);

    if(!newNode->tag->single && newNode->tag->type == Tag::XmlTag)
    {
        //create the new closing Node.
        nodeEnd = createNode("/" + newNode->tag->name, "", Tag::XmlTagEnd, newNode->tag->write());
        nodeEnd->closesPrevious = true;

        //place the new closing Node.
        nodeEnd = insertNode(nodeEnd, parent, nextEndSibling, modifs, merge);
    }

    //If nextSibling != nextEndSibling, move all Nodes between node and nodeEnd as child of node
    if(nextSibling != nextEndSibling)
    {
        n = newNode->next;
        while(newNode->next && newNode->next != nodeEnd)
            moveNode(newNode->next, newNode, 0L, modifs);
    }

    return newNode;
}

Node* kafkaCommon::insertNode(Node *newNode, Node *parent, Node *startNodeToSurround,
                              Node *endNodeToSurround, int startOffset, int endOffset, NodeModifsSet *modifs)
{
#ifdef LIGHT_DEBUG
    kdDebug(25001)<< "kafkaCommon::insertNode()2" << endl;
#endif

    if(!newNode || !startNodeToSurround || !endNodeToSurround)
        return 0L;

    //first split the Nodes.
    if(splitNode(startNodeToSurround, startOffset, modifs))
    {
        if(endNodeToSurround == startNodeToSurround)
        {
            endNodeToSurround = endNodeToSurround->next;
            endOffset -= startOffset;
        }
        startNodeToSurround = startNodeToSurround->next;
    }
    if(splitNode(endNodeToSurround, endOffset, modifs))
        endNodeToSurround = endNodeToSurround->next;

    //Then create and insert the new Node.
    return insertNode(newNode, parent, startNodeToSurround,
                      endNodeToSurround, modifs);
}

Node* kafkaCommon::insertNodeSubtree(Node *node, Node* parentNode, Node* nextSibling,
                                     NodeModifsSet *modifs, bool merge)
{
    Node *nextNode, *currentNode;

    if(!node || (node && node->prev))
        return 0L;

    //insert the node subtree
    currentNode = node;
    while(currentNode)
    {
        nextNode = currentNode->next;
        if(currentNode == node)
            node = insertNode(currentNode, parentNode, nextSibling, nextSibling, modifs, merge);
        else
            insertNode(currentNode, parentNode, nextSibling, nextSibling, modifs, merge);

        currentNode = nextNode;
    }

    return node;
}

Node* kafkaCommon::insertNodeSubtree(Node *node, Node* parentNode, Node* nextSibling,
                                     Node* nextEndSibling, NodeModifsSet *modifs, bool merge)
{
    Node *nextNode, *currentNode, *currentParent;

    if(!node || (node && node->prev))
        return 0L;

    //insert the node subtree.
    currentNode = node;
    currentParent = parentNode;
    while(currentNode)
    {
        nextNode = currentNode->child;
        currentNode->child = 0L;

        //If the closing tag of currentNode is present, let's delete it
        if(currentNode->next && QuantaCommon::closesTag(currentNode->tag, currentNode->next->tag))
            delete extractNode(currentNode->next, 0L);

        //insert the node and its closing tag if necessary.
        if(currentNode == node)
        {
            currentParent = insertNode(currentNode, currentParent, nextSibling,
                                       nextEndSibling, modifs, merge);
            node = currentParent;
        }
        else
            currentParent = insertNode(currentNode, currentParent, nextSibling,
                                       0L, modifs, merge);

        currentNode = nextNode;
    }

    return node;
}

Node* kafkaCommon::DTDInsertNodeSubtree(Node *newNode, NodeSelectionInd& selection,
                                        Node **cursorNode, long& cursorOffset, NodeModifsSet *modifs)
{
    Q_ASSERT(!selection.hasSelection());

    Node* startNode = 0;
    if(!(*cursorNode)) // see KafkaDocument::slotPaste()
        startNode = getNodeFromLocation(selection.cursorNode());
    else
        startNode = *cursorNode;

    if(!startNode)
    {
        kdError() << "NULL startNode in kafkaCommon::DTDInsertNodeSubtree given by NodeSelectionInd::cursorNode()" << endl;
        return 0;
    }

    Node* endNode = 0;
    if(!cursorNode)
        return 0;
    //int startOffset = selection.cursorOffset();
    int startOffset = cursorOffset;

    /**
     * TODO : Optionnal for the moment : move the cursor coordinates so that we have good locations.
     * e.g. <b>boo|</b>baa should be translated to <b>boo</b>|baa
     */

    if(cursorOffset == (signed)startNode->tag->tagStr().length())
    {
        while(startNode && startNode->tag->type != Tag::Text)
            startNode = startNode->nextSibling();
        if(!startNode)
        {
            insertNodeSubtree(newNode, baseNode->child, 0, modifs, true);
            return newNode;
        }
        else
            cursorOffset = 0;
    }

    // look for commonParent
    QValueList<int> commonParentStartChildLocation;
    QValueList<int> commonParentEndChildLocation;

    Node* commonParent = DTDGetNonInlineCommonParent(startNode, startNode,
            commonParentStartChildLocation, commonParentEndChildLocation, 0);

    Node* commonParentStartChild = getNodeFromLocation(commonParentStartChildLocation);

    //OK now, we are sure the node can be inserted. Start the work by splitting
    //startNode if necessary
    if(cursorOffset != 0)
    {
        if(startNode->tag->type == Tag::Text || startNode->tag->type == Tag::Empty)
        {
            if(splitNode(startNode, startOffset, modifs))
            {
            //</TEMPORARY>
                if(startNode == commonParentStartChild)
                    commonParentStartChild = commonParentStartChild->nextSibling();
                endNode = startNode->nextSibling();
            }
            else if(startOffset == (signed)startNode->tag->tagStr().length())
            {
            //No need to update endNode. If endNode == startNode && startOffset == endOffset,
            //we'll catch this later.
                if(startNode == commonParentStartChild)
                    commonParentStartChild = commonParentStartChild->nextSibling();
                startNode = startNode->nextSibling();
            }
        }
    }

    if(newNode->tag->type == Tag::Text || newNode->tag->type == Tag::Empty)
    {
        *cursorNode = newNode;
        cursorOffset = newNode->tag->tagStr().length();
        return insertNodeSubtree(newNode, startNode->parent, endNode, modifs);
    }

    //Then we "split" the lastValidStartParent - startNode subtree into two : the first part is untouched
    // and the second will be surrounded by the new Node. Same thing for endNode.
    Node* node = startNode;
    Node* parentNode = startNode->parent;
    Node* newParentNode = 0, *child = 0, *next = 0;
    while(parentNode && commonParent && parentNode != commonParent)
    {
        if(true/*node != parentNode->firstChild()*/)
        {
            //node is not the first Child of parentNode, we have to duplicate parentNode, and put node and
            //all its next sibling as child of the new parentNode.
            /**newParentNode = insertNode(parentNode->tag->name, parentNode->tag->tagStr(),
            parentNode->tag->type, parentNode->tag->write(), parentNode->parentNode(),
            parentNode, parentNode, modifs);*/
            newParentNode = duplicateNode(parentNode);
            insertNode(newParentNode, parentNode->parentNode(), parentNode, parentNode, modifs);
            child = parentNode->firstChild();
            if(cursorOffset != 0)
            {
                while(child && (child != endNode) && !child->hasForChild(endNode))
                {
                    next = child->next;
                    moveNode(child, newParentNode, 0L, modifs);
                    child = next;
                }
            }
            else
            {
                while(child)
                {
                    next = child->next;
                    moveNode(child, newParentNode, 0L, modifs, true, true);
                    if(child == startNode || child->hasForChild(startNode))
                        break;

                    child = next;
                }
            }
        }
        //commonParentStartChild = parentNode;
        node = parentNode;
        parentNode = parentNode->parent;
    }

    if(endNode)
    {
        node = endNode;
        parentNode = endNode->parent;
        while(parentNode && commonParent && parentNode != commonParent)
        {
            if(true/*node != parentNode->firstChild()*/)
            {
            //node is not the first Child of parentNode, we have to duplicate parentNode, and put node and
            //all its next sibling as child of the new parentNode.
            /**newParentNode = insertNode(parentNode->tag->name, parentNode->tag->tagStr(),
                parentNode->tag->type, parentNode->tag->write(), parentNode->parentNode(),
                parentNode, parentNode, modifs);*/
                newParentNode = duplicateNode(parentNode);
                insertNode(newParentNode, parentNode->parentNode(), parentNode, parentNode, modifs);
                child = parentNode->firstChild();
                while(child /*&& child == endNode*/ &&
                      (child == endNode || child->hasForChild(endNode)/* ||
                      (child->prev && child->prev->hasForChild(endNode) && child->closesPrevious)*/))
                {
                    next = child->next;
                    moveNode(child, newParentNode, 0L, modifs, true, true);
                    child = next;
                }
            }
            commonParentStartChild = newParentNode;
            node = parentNode;
            Node* aux = parentNode;
            parentNode = parentNode->parent;
            // Remove node subtree if empty
            if(!aux->hasChildNodes())
                extractAndDeleteNode(aux, modifs);
        }
    }
    if(newNode->next && QuantaCommon::closesTag(newNode->tag, newNode->next->tag))
        delete extractNode(newNode->next, 0L);

    Node* nextSibling = commonParentStartChild;
    /*
    if(cursorOffset == 0)
        nextSibling = nextSibling->SNext();
    */
    return insertNodeSubtree(newNode, commonParent, nextSibling/*, nextSibling*/, modifs);

    //mergeInlineNode(commonParent, commonParent->next, cursorNode, cursorOffset, modifs);
    //return newNode;
}

Node* kafkaCommon::DTDInsertNodeSubtree(Node* newNode, Node* parentNode, Node* nextSibling,
                                        NodeSelection& /*cursorHolder*/, NodeModifsSet *modifs)
{
    QTag* nodeQTag = QuantaCommon::tagFromDTD(parentNode);
    if(!nodeQTag || !nodeQTag->isChild(newNode))
        return 0;
    else
        return insertNodeSubtree(newNode, parentNode, nextSibling, modifs);
}

bool kafkaCommon::DTDinsertNode(Node *newNode, Node *startNode, int startOffset, Node *endNode,
                                int endOffset, Document *doc, Node **cursorNode, long &cursorOffset, NodeModifsSet *modifs)
{
#ifdef LIGHT_DEBUG
    kdDebug(25001)<< "kafkaCommon::DTDinsertNode()" << endl;
#endif

    QValueList<int> startNodeLocation, endNodeLocation;
    QValueList<int>::iterator itStart, itEnd;
    Node *commonParent = 0L, *commonParentStartChild, *commonParentEndChild, *parentNode, *node;
    Node *lastValidStartParent = 0L, *lastValidEndParent = 0L, *newParentNode, *child, *next;
    Node *oldCommonParent, *lastNewNode, *oldParentNode;
    QTag *parentNodeQTag = 0, *newNodeQTag, *lastNewNodeQTag;
    NodeModif modif;
    int locOffset = 1;
    bool newNodeIsInline, isAfter;

    if(!startNode || !endNode || !newNode || !doc)
    {
        Node::deleteNode(newNode);
        return false;
    }

    //FIrst get the mandatory Nodes if necessary, and get the qTag of the first and last Node.
    lastNewNode = createMandatoryNodeSubtree(newNode, doc);
    lastNewNodeQTag = QuantaCommon::tagFromDTD(lastNewNode);
    newNodeQTag = QuantaCommon::tagFromDTD(newNode);
    if(!newNodeQTag || !lastNewNodeQTag)
    {
      Node::deleteNode(newNode);
      return false;
    }

    //Then search for the common parent of startNode and endNode (commonParent)
    //and for the childs of commonParent which are parent of startNode and endNode
    //(commonParentStartChild && commonParentEndChild)
    //CommonParent will be the limit (startNode -- commonNode) where Nodes can
    //be splitted in order to insert the newNode.
    startNodeLocation = getLocation(startNode);
    endNodeLocation = getLocation(endNode);
    itStart = startNodeLocation.begin();
    itEnd = endNodeLocation.begin();
    while(itStart != startNodeLocation.end() && itEnd != endNodeLocation.end() &&
            (*itStart) == (*itEnd))
    {
        commonParent = getNodeFromSubLocation(startNodeLocation, locOffset);
        itStart++;
        itEnd++;
        locOffset++;
    }
    //look for commonParentStartChild and commonParentEndChild
    if(itStart != startNodeLocation.end())
        commonParentStartChild = getNodeFromSubLocation(startNodeLocation, locOffset);
    else
        commonParentStartChild = commonParent;
    if(itEnd != endNodeLocation.end())
        commonParentEndChild = getNodeFromSubLocation(endNodeLocation, locOffset);
    else
        commonParentEndChild = commonParent;
    //If newNode isn't inline, move commonParent to the closest non inline node
    newNodeIsInline = isInline(newNode->tag->name);
    if(!newNodeIsInline && commonParent && (isInline(commonParent->tag->name) ||
                                            commonParent->tag->type == Tag::Text || commonParent->tag->type == Tag::Empty))
    {
        oldCommonParent = commonParent;
        commonParent = commonParent->parent;
        while(commonParent && isInline(commonParent->tag->name))
        {
            oldCommonParent = commonParent;
            commonParent = commonParent->parent;
        }
        commonParentStartChild = oldCommonParent;
        commonParentEndChild = oldCommonParent;
    }
    //startNode or endNode can't be the commonParent.
    else if(commonParent && (itStart == startNodeLocation.end() || itEnd == endNodeLocation.end()))
        commonParent = commonParent->parent;

    //Now look if at least one of the parent Nodes between startNode and commonParent
    //can have nodeName as child. If so for startNode and endNode, let's find the last
    //parent Nodes which can have nodeName as child.
    parentNode = startNode->parent;
    oldParentNode = startNode;
    while(parentNode && commonParent && parentNode != commonParent->parent)
    {
        parentNodeQTag = QuantaCommon::tagFromDTD(parentNode);
        if(parentNodeQTag && parentNodeQTag->isChild(newNode) &&
                lastNewNodeQTag->isChild(oldParentNode))
            lastValidStartParent = parentNode;
        else if(newNodeIsInline || !isInline(parentNode->tag->name))
            break;
        //else if(!newNodeIsInline && isInline(parentNode)), we continue : BLOCK element can
        //cut some inline tag in order to be inserted.
        oldParentNode = parentNode;
        parentNode = parentNode->parent;
    }
    parentNode = endNode->parent;
    oldParentNode = endNode;
    while(parentNode && commonParent &&  parentNode != commonParent->parent)
    {
        parentNodeQTag = QuantaCommon::tagFromDTD(parentNode);
        if(parentNodeQTag && parentNodeQTag->isChild(newNode) &&
                lastNewNodeQTag->isChild(oldParentNode))
            lastValidEndParent = parentNode;
        else if(newNodeIsInline || !isInline(parentNode->tag->name))
            break;
        //else if(!newNodeIsInline && isInline(parentNode)), we continue : BLOCK element can
        //cut some inline tag in order to be inserted.
        oldParentNode = parentNode;
        parentNode = parentNode->parent;
    }

    /**if(!lastValidEndParent || !lastValidStartParent)
    {
      Node::deleteNode(newNode);
      return false;
    }*/

    //OK now, we are sure the node can be inserted. Start the work by splitting
    //startNode and endNode if necessary
    if(startNode->tag->type == Tag::Text || startNode->tag->type == Tag::Empty)
    {
        if(splitNode(startNode, startOffset, modifs))
        {
            //<TEMPORARY>
            if(startNode == (*cursorNode) && cursorOffset > startOffset)
            {
                (*cursorNode) = (*cursorNode)->nextSibling();
                cursorOffset -= startOffset;
            }
            //</TEMPORARY>
            if(startNode == commonParentStartChild)
                commonParentStartChild = commonParentStartChild->nextSibling();
            if(startNode == endNode)
            {
                endNode = endNode->nextSibling();
                endOffset -= startOffset;
            }
            startNode = startNode->nextSibling();
            startOffset = 0;
        }
        else if(startOffset == (signed)startNode->tag->tagStr().length())
        {
            //No need to update endNode. If endNode == startNode && startOffset == endOffset,
            //we'll catch this later.
            if(startNode == commonParentStartChild)
                commonParentStartChild = commonParentStartChild->nextSibling();
            startNode = startNode->nextSibling();
        }
    }
    if(endNode->tag->type == Tag::Text || endNode->tag->type == Tag::Empty)
    {
        if(!splitNode(endNode, endOffset, modifs) && endOffset == 0)
        {
            //No need to update startNode. If startNode == endNode && startOffset == endOffset,
            //we'll catch this later.
            if(endNode == commonParentEndChild)
                commonParentEndChild = commonParentEndChild->previousSibling();
            if (endNode->previousSibling())
              endNode = endNode->previousSibling();
        }
    }

    //Then we "split" the lastValidStartParent - startNode subtree into two : the first part is untouched
    // and the second will be surrounded by the new Node. Same thing for endNode.
    node = startNode;
    if (!startNode) //Andras: it can happen.
      return false;
    parentNode = startNode->parent;
    while(lastValidStartParent && parentNode && parentNode != lastValidStartParent)
    {
        if(node != parentNode->firstChild())
        {
            //node is not the first Child of parentNode, we have to duplicate parentNode, and put node and
            //all its next sibling as child of the new parentNode.
            /**newParentNode = insertNode(parentNode->tag->name, parentNode->tag->tagStr(),
            	parentNode->tag->type, parentNode->tag->write(), parentNode->parentNode(),
            	parentNode, parentNode, modifs);*/
            newParentNode = duplicateNode(parentNode);
            insertNode(newParentNode, parentNode->parentNode(), parentNode, parentNode, modifs);
            child = parentNode->firstChild();
            while(child && child != startNode && !child->hasForChild(startNode))
            {
                next = child->next;
                moveNode(child, newParentNode, 0L, modifs);
                child = next;
            }
        }
        node = parentNode;
        parentNode = parentNode->parent;
    }
    node = endNode;
    parentNode = endNode->parent;
    while(lastValidEndParent && parentNode && parentNode != lastValidEndParent)
    {
        if(node != parentNode->lastChild())
        {
            //node is not the last Child of parentNode, we have to duplicate parentNode, and put all
            //the next sibling of node as child of the new parentNode
            /**newParentNode = insertNode(parentNode->tag->name, parentNode->tag->tagStr(),
            	parentNode->tag->type, parentNode->tag->write(), parentNode->parentNode(),
            	parentNode, parentNode, modifs);*/
            newParentNode = duplicateNode(parentNode);
            insertNode(newParentNode, parentNode->parentNode(), parentNode, parentNode, modifs);
            if(parentNode == commonParentStartChild)
                commonParentStartChild = newParentNode;
            if(parentNode == commonParentEndChild)
                commonParentEndChild = newParentNode;
            child = parentNode->firstChild();
            while(child)
            {
                next = child->next;
                moveNode(child, newParentNode, 0L, modifs);
                if(child == endNode || child->hasForChild(endNode))
                {
                    if(QuantaCommon::closesTag(child->tag, next->tag))
                        moveNode(next, newParentNode, 0L, modifs);
                    break;
                }
                child = next;
            }
        }
        node = parentNode;
        parentNode = parentNode->parent;
    }

    //Now if startNode is after endNode, this means that a selectionless insertion is being done.
    //(This is due to the text splitting)
    //Let's insert it and return
    isAfter = (compareNodePosition(startNode, endNode) == kafkaCommon::isAfter);
    if(isAfter || (startNode == endNode && startOffset == endOffset &&
                   (signed)startNode->tag->tagStr().length() == startOffset))
    {
        if(isAfter)
            parentNodeQTag = QuantaCommon::tagFromDTD(commonParent);
        else if((signed)startNode->tag->tagStr().length() == startOffset && startNode->tag->type == Tag::XmlTag)
            parentNodeQTag = QuantaCommon::tagFromDTD(startNode);
        else if((signed)startNode->tag->tagStr().length() == startOffset && startNode->tag->type == Tag::XmlTagEnd)
            parentNodeQTag = QuantaCommon::tagFromDTD(startNode->parent);
        if(!parentNodeQTag || (parentNodeQTag && parentNodeQTag->isChild(newNode)))
        {
            if(isAfter)
                insertNodeSubtree(newNode, commonParent, commonParentStartChild, modifs);
            else if((signed)startNode->tag->tagStr().length() == startOffset && startNode->tag->type == Tag::XmlTag)
                insertNodeSubtree(newNode, startNode, 0L, modifs);
            else if((signed)startNode->tag->tagStr().length() == startOffset && startNode->tag->type == Tag::XmlTagEnd)
                insertNodeSubtree(newNode, startNode->parent, startNode->next, modifs);
            //<TEMPORARY>
            (*cursorNode) = lastNewNode;
            cursorOffset = 0;
            //</TEMPORARY>
            return true;
        }
        else
        {
          Node::deleteNode(newNode);
          return false;
        }
    }
    else
    {
        //Else we apply the recursive function to add the new Node when necessary/possible.
        bool addingStarted = false;
        bool examinationStarted = false;
        bool nodeInserted = false;
        int level = 0;
        addNodeRecursively(newNode, lastNewNode,
                           (compareNodePosition(lastValidStartParent, commonParentStartChild) ==
                            kafkaCommon::isAfter)?lastValidStartParent:commonParentStartChild,
                           (compareNodePosition(lastValidEndParent, commonParentEndChild) ==
                            kafkaCommon::isAfter)?lastValidEndParent:commonParentEndChild,
                           startNode, endNode, commonParentStartChild, examinationStarted,
                           addingStarted, nodeInserted, level, modifs);

        //And we merge if necessary some identical inline Nodes.
        mergeInlineNode(startNode, endNode, cursorNode, cursorOffset, modifs);
        return nodeInserted;
    }
}

bool kafkaCommon::DTDinsertRemoveNode(Node *newNode, Node *startNode, int startOffset,
                                      Node *endNode, int endOffset, Document *doc, Node **cursorNode, long &cursorOffset,
                                      NodeModifsSet *modifs)
{
    int result;

    if(!newNode || !startNode || !endNode || !doc)
        return false;

    //First try to remove the Nodes. If unsuccessfull, try to insert it.
    result = DTDExtractNode(newNode->tag->name, doc, startNode, startOffset, endNode, endOffset,
                            cursorNode, cursorOffset, modifs);
    if(result == kafkaCommon::nothingExtracted || result == kafkaCommon::extractionBadParameters)
    {
        return DTDinsertNode(newNode, startNode, startOffset, endNode, endOffset, doc, cursorNode,
                             cursorOffset, modifs);
    }
    else
        return true;
    //else if result == kafkaCommon::extractionStoppedDueToBadNodes,
    //what should we do?
}

Node *kafkaCommon::createAndInsertNode(const QString &nodeName, const QString &tagString,
                                       int nodeType, Document *doc, Node* parent, Node* nextSibling, NodeModifsSet *modifs,
                                       bool merge)
{
#ifdef LIGHT_DEBUG
    kdDebug(25001)<< "kafkaCommon::createAndInsertNode() - nodeName :" << nodeName <<
    " - tagStr :" << tagString << " - nodeType :" << nodeType << endl;
#endif

    Node *node;

    //create the new Node.
    node = createNode(nodeName, tagString, nodeType, doc);

    //insert the new Node.
    insertNode(node, parent, nextSibling, modifs, merge);

    return node;
}

Node *kafkaCommon::createAndInsertNode(const QString &nodeName, const QString &tagString,
                                       int nodeType, Document *doc, Node *parent, Node *nextSibling, Node *nextEndSibling,
                                       NodeModifsSet *modifs)
{
#ifdef LIGHT_DEBUG
    kdDebug(25001)<< "kafkaCommon::createAndInsertNode()2- nodeName :" << nodeName <<
    " - tagStr :" << tagString << " - nodeType :" << nodeType << endl;
#endif

    Node *node;

    //create the new Node.
    node = createNode(nodeName, tagString, nodeType, doc);

    //insert the new Node.
    insertNode(node, parent, nextSibling, nextEndSibling, modifs);

    return node;
}

Node *kafkaCommon::createAndInsertNode(const QString &nodeName, const QString &tagString,
                                       int nodeType, Document *doc, Node *parent, Node *startNodeToSurround,
                                       Node *endNodeToSurround, int startOffset, int endOffset, NodeModifsSet *modifs)
{
#ifdef LIGHT_DEBUG
    kdDebug(25001)<< "kafkaCommon::createAndInsertNode()3 - nodeName :" << nodeName <<
    " - tagStr :" << tagString << " - nodeType :" << nodeType << endl;
#endif

    Node *node;

    if(!startNodeToSurround || !endNodeToSurround)
        return 0L;

    //create the new Node.
    node = createNode(nodeName, tagString, nodeType, doc);

    //insert the new Node.
    insertNode(node, parent, startNodeToSurround, endNodeToSurround, startOffset, endOffset,
               modifs);

    return node;

}

bool kafkaCommon::DTDcreateAndInsertNode(const QString &nodeName, const QString &tagString,
        int nodeType, Document *doc, Node *startNode, int startOffset, Node *endNode, int endOffset,
        Node **cursorNode, long &cursorOffset, NodeModifsSet *modifs)
{
#ifdef LIGHT_DEBUG
    kdDebug(25001)<< "kafkaCommon::DTDcreateAndInsertNode()2 - nodeName : " << nodeName <<
    " - tagStr" <<tagString << endl;
#endif

    Node *node;

    if(!startNode || !endNode)
        return false;

    //create the new Node.
    node = createNode(nodeName, tagString, nodeType, doc);

    //insert the new Node.
    return DTDinsertNode(node, startNode, startOffset, endNode, endOffset, doc, cursorNode,
                         cursorOffset, modifs);

}

bool kafkaCommon::addNodeRecursively(Node *newNode, Node *leafNode,
                                     Node *startExaminationNode, Node *endExaminationNode, Node* startNode, Node *endNode,
                                     Node* currentNode, bool &examinationStarted, bool &addingStarted, bool &nodeInserted, int level,
                                     NodeModifsSet *modifs)
{

    QTag *leafNodeQTag, *currentNodeParentQTag;
    Node *startSelection = 0L, *endSelection = 0L, *oldCurrentNode, *copyNewNode;
    bool selectionInProgress = false, validCurNodeParent = false;

    leafNodeQTag = QuantaCommon::tagFromDTD(leafNode);
    if(!leafNodeQTag)
        return false;

    if(currentNode && currentNode->parent)
    {
        currentNodeParentQTag = QuantaCommon::tagFromDTD(currentNode->parent);
        if(currentNodeParentQTag && currentNodeParentQTag->isChild(newNode))
            validCurNodeParent = true;
    }

    while(currentNode)
    {
#ifdef HEAVY_DEBUG
        kdDebug(25001)<< "kafkaCommon::addNodeRevursively() [" << level << "] - currentNode :" <<
        currentNode->tag->name << "("<< currentNode->tag->type << ")(" << currentNode << ")" << endl;
#endif
        //If currentNode is the startExaminationNode, let's start to examine Nodes (=> search the startNode)
        if(currentNode == startExaminationNode)
            examinationStarted = true;

        //If currentNode is the startNode, let's start to try to add Nodes.
        if(currentNode == startNode)
            addingStarted = true;

        //If the currentNode is text or XmlTag, and if it is DTD valid to insert the node Subtree and
        //if the examination has started and currentNode doesn't have endExaminationNode as
        //child, let's start/extend the selection over this node.
        if((currentNode->tag->type == Tag::XmlTag || currentNode->tag->type == Tag::Text) &&
                leafNodeQTag->isChild(currentNode) && validCurNodeParent && examinationStarted &&
                !currentNode->hasForChild(endExaminationNode))
        {
#ifdef HEAVY_DEBUG
            kdDebug(25001)<< "kafkaCommon::addNodeRevursively() [" << level <<
            "] -  Valid Child : " << currentNode->tag->name << endl;
#endif
            //extend the selection to this node.
            if(currentNode->tag->type == Tag::XmlTag && currentNode->getClosingNode())
                endSelection = currentNode->getClosingNode();
            else
                endSelection = currentNode;

            //If this Node is, or has for child startNode, let's start to add newNode
            if(currentNode->hasForChild(startNode) || currentNode == startNode)
            {
#ifdef HEAVY_DEBUG
                kdDebug(25001)<< "kafkaCommon::addNodeRevursively() [" << level <<
                "] -  This Node has the startNode as Child : " << currentNode->tag->name << endl;
#endif

                addingStarted = true;
            }

            //If there isn't a previously started selection, let's start it now.
            if(!selectionInProgress && addingStarted)
            {
#ifdef HEAVY_DEBUG
                kdDebug(25001)<< "kafkaCommon::addNodeRevursively() [" << level <<
                "] - selection started at Node " << currentNode->tag->name << endl;
#endif

                selectionInProgress = true;
                startSelection = currentNode;
            }
        }
        else if(currentNode->tag->type == Tag::XmlTag || currentNode->tag->type == Tag::Text)
        {
#ifdef HEAVY_DEBUG
            kdDebug(25001)<< "kafkaCommon::addNodeRevursively() [" << level <<
            "] -  Invalid Child : " << currentNode->tag->name << endl;
#endif
            //the current Node can't handle newNode as a child, let's stop the selection
            // here and surround the current selection with newNode
            endSelection = currentNode->prev;
            if(selectionInProgress)
            {
#ifdef HEAVY_DEBUG
                kdDebug(25001)<< "kafkaCommon::addNodeRevursively() [" << level <<
                "] - selection ended(2) at Node " << currentNode->tag->name << endl;
#endif

                selectionInProgress = false;
                if(addingStarted)
                {
                    while(startSelection && startSelection->tag->type == Tag::Empty)
                        startSelection = startSelection->next;
                    while(endSelection && endSelection->tag->type == Tag::Empty)
                        endSelection = endSelection->prev;
                    if (startSelection && endSelection)
                    {
                      /**copyNewNode = duplicateNode(newNode);
                      insertNode(copyNewNode, startSelection->parentNode(), startSelection,
                        endSelection->next, modifs);*/
                      copyNewNode = duplicateNodeSubtree(newNode);
                      insertNodeSubtree(copyNewNode, startSelection->parentNode(), startSelection,
                                        endSelection->next, modifs);
                      nodeInserted = true;
                    }
                }
            }

            //TESTING: If this Node is, or has for child startNode, let's start to add newNode
            /**if(currentNode->hasForChild(startNode) || currentNode == startNode)
            {
            #ifdef HEAVY_DEBUG
                kdDebug(25001)<< "kafkaCommon::addNodeRevursively() [" << level <<
                "] -  This Node has the startNode as Child : " << currentNode->tag->name << endl;
            #endif

                addingStarted = true;
            }*/

            //Let's try to surround some of the childs of currentNode.
            if(currentNode->child)
            {
                addNodeRecursively(newNode, leafNode, startExaminationNode,
                                   endExaminationNode, startNode, endNode, currentNode->child,
                                   examinationStarted, addingStarted, nodeInserted, level + 1, modifs);
            }
        }
        //If the currentNode is XmlTagEnd, Empty or whatever but not XmlTag and Text,
        // we will surround them with newNode if a selection was started.
        else
        {
            if(selectionInProgress)
            {
                if((currentNode->tag->type == Tag::XmlTag || currentNode->tag->type == Tag::ScriptTag) &&
                        currentNode->getClosingNode())
                    endSelection = currentNode->getClosingNode();
                else
                    endSelection = currentNode;
            }
            //If this Node is, or has for child startNode, let's start to add newNode
            if((currentNode->hasForChild(startNode) || currentNode == startNode) &&
                    examinationStarted)
            {
#ifdef HEAVY_DEBUG
                kdDebug(25001)<< "kafkaCommon::addNodeRevursively() [" << level <<
                "] -  This Node has the startNode as Child : " << currentNode->tag->name << endl;
#endif

                addingStarted = true;
            }
        }

        //If the current Node is, or has for child endNode, or if currentNode is
        //endExaminationNode or if examination is stopped, let's stop the current selection.
        if(currentNode->hasForChild(endNode) || currentNode == endNode ||
                currentNode == endExaminationNode)
        {
#ifdef HEAVY_DEBUG
            kdDebug(25001)<< "kafkaCommon::addNodeRevursively() [" << level <<
            "] -  This Node has the endNode as Child : " << currentNode->tag->name << endl;
#endif

            addingStarted = false;
            examinationStarted = false;
            if(selectionInProgress)
            {
#ifdef HEAVY_DEBUG
                kdDebug(25001)<< "kafkaCommon::addNodeRevursively() [" << level <<
                "] - selection ended at Node " << currentNode->tag->name << endl;
#endif

                selectionInProgress = false;
                while(startSelection && startSelection->tag->type == Tag::Empty)
                    startSelection = startSelection->next;
                while(endSelection && endSelection->tag->type == Tag::Empty)
                    endSelection = endSelection->prev;
                if (startSelection && endSelection)
                {
                  /**copyNewNode = duplicateNode(newNode);
                  insertNode(copyNewNode, startSelection->parentNode(), startSelection,
                    endSelection->next, modifs);*/
                  copyNewNode = duplicateNodeSubtree(newNode);
                  insertNodeSubtree(copyNewNode, startSelection->parentNode(), startSelection,
                                    endSelection->next, modifs);
                  nodeInserted = true;
                }
            }
        }

        oldCurrentNode = currentNode;
        currentNode = currentNode->next;
    }

    if(selectionInProgress)
    {
#ifdef HEAVY_DEBUG
        kdDebug(25001)<< "kafkaCommon::addNodeRevursively() [" << level <<
        "] - selection ended(3) at Node " << oldCurrentNode->tag->name << endl;
#endif

        selectionInProgress = false;
        endSelection = oldCurrentNode;
        if(addingStarted)
        {
            while(startSelection && startSelection->tag->type == Tag::Empty)
                startSelection = startSelection->next;
            while(endSelection && endSelection->tag->type == Tag::Empty)
                endSelection = endSelection->prev;
            /**copyNewNode = duplicateNode(newNode);
            insertNode(copyNewNode, startSelection->parentNode(), startSelection,
            	endSelection->next, modifs);*/
            copyNewNode = duplicateNodeSubtree(newNode);
            insertNodeSubtree(copyNewNode, startSelection->parentNode(), startSelection,
                              endSelection->next, modifs);
            nodeInserted = true;
        }
    }

    //The newNode was a template, let's delete it now.
    if(level == 0)
      Node::deleteNode(newNode);
    return true;
}

Node *kafkaCommon::duplicateNode(Node *node)
{
    Node *newNode;

    if(!node)
        return 0L;

    newNode = new Node(0L);
    (*newNode) = node;
    newNode->tag->setCleanStrBuilt(false);
    newNode->tag->setIndentationDone(false);

    return newNode;
}

typedef struct boo
{
    boo()
    {
        m_n1 = m_n2 = 0L;
    }
    boo(Node *n1, Node *n2)
    {
        m_n1 = n1;
        m_n2 = n2;
    }
    Node *m_n1;
    Node *m_n2;
}
NodeLink;

Node* kafkaCommon::getLastChild(Node* node)
{
    assert(node);

    Node* end_node = node->getClosingNode();
    if(!end_node && node->hasChildNodes())
        end_node = node->lastChildNE();
    else if(!end_node)
        end_node = node;

    assert(end_node);

    return end_node;
}

Node *kafkaCommon::duplicateNodeSubtree(Node *node, bool childAndClosingTagOnly)
{
    QPtrList<NodeLink> nodeLinkList;
    bool goUp = false;
    Node *currentNode, *currentNewNode, *newRootNode = 0, *newNext, *newParent, *newPrev;
    NodeLink *link;
    Node* endNode = 0;
    if(!node)
      return 0L;

    if(childAndClosingTagOnly)
        endNode = getLastChild(node);


    nodeLinkList.setAutoDelete(true);
    currentNode = node;
    while(currentNode)
    {
        currentNewNode = duplicateNode(currentNode);
        nodeLinkList.append(new NodeLink(currentNode, currentNewNode));

        newNext = 0L;
        newParent = 0L;
        newPrev = 0L;
        for(link = nodeLinkList.first(); link; link = nodeLinkList.next())
        {
            if(link->m_n1 == currentNode->parent)
                newParent = link->m_n2;
            else if(link->m_n1 == currentNode->next)
                newNext = link->m_n2;
            else if(link->m_n1 == currentNode->prev)
                newPrev = link->m_n2;
        }

        if(!newParent && !newPrev)
            newRootNode = currentNewNode;
        else if(!newParent)
        {
            //Temporary, insertNode would rely on baseNode which can be dangerous
            currentNewNode->prev = newPrev;
            newPrev->next = currentNewNode;
        }
        else
            insertNode(currentNewNode, newParent, newNext, 0L, false);

        if(childAndClosingTagOnly)
            currentNode = getNextNode(currentNode, goUp, endNode);
        else
            currentNode = getNextNode(currentNode, goUp, node);
    }

    return newRootNode;
}

Node* kafkaCommon::extractNode(Node *node, NodeModifsSet *modifs, bool extractChildren,
                               bool extractClosingTag)
{
    NodeModif *modif = 0, *modifChild;
    Node *lastChild, *curNode;
    Node *parent, *next, *child, *n;
    //Node *prev;
    bool isSingle;
    int type;
    QString namespaceName, nodeName, caseSensitive;
    QString closingNamespaceName, closingNodeName, closingCaseSensitive;
    QValueList<int> location;

    if(!node)
        return 0L;

    if(!node->child)
        extractChildren = true;

    parent = node->parent;
    next = node->next;
    //prev = node->prev; //Should this be used at all?
    child = node->child;
    lastChild = node->lastChild();
    isSingle = node->tag->single;
    type = node->tag->type;
    namespaceName = node->tag->nameSpace;
    nodeName = node->tag->name;
    caseSensitive = node->tag->dtd()->caseSensitive;

    //logging
    if(modifs)
    {
        modif = new NodeModif();
        if(extractChildren)
            modif->setType(NodeModif::NodeAndChildsRemoved);
        else
            modif->setType(NodeModif::NodeRemoved);
        modif->setLocation(getLocation(node));

        //log the children move if we don't extract the children
        if(!extractChildren)
        {
          location = getLocation(node);
          location.last()++;
          n = lastChild;
          while(n)
          {
            modifChild = new NodeModif();
            modifChild->setType(NodeModif::NodeAndChildsMoved);
            modifChild->setLocation(getLocation(n));
            modifChild->setFinalLocation(location);
            modifs->addNodeModif(modifChild);
            n = n->prev;
          }
        }
    }

    //starting to extract.
    if(node == baseNode)
    {
        if(extractChildren)
            baseNode = 0L;
        else
            baseNode = node->child;
        parser->setRootNode(baseNode);
    }
    if(!extractChildren)
    {
        curNode = node->child;
        while(curNode)
        {
            curNode->parent = node->parent;
            curNode = curNode->next;
        }
    }
    if(node->parent && node->parent->child == node)
    {
        if(extractChildren)
            node->parent->child = node->next;
        else
            node->parent->child = node->child;
    }
    node->parent = 0L;
    if(node->prev)
    {
        if(extractChildren)
            node->prev->next = node->next;
        else
        {
            node->prev->next = node->child;
            node->child->prev = node->prev;
        }
    }
    if(node->next)
    {
        if(extractChildren)
            node->next->prev = node->prev;
        else
        {
            /**lastChild = node->child;
            while(lastChild->next)
                lastChild = lastChild->next;*/
            node->next->prev = lastChild;
            lastChild->next = node->next;
        }
    }
    node->prev = 0L;
    node->next = 0L;
    if(!extractChildren)
        node->child = 0L;

    if(modifs)
    {
        modif->setNode(0/*node*/); // this deletes the node!!???
        modifs->addNodeModif(modif);
    }

    //extract the closing Tag
    if(extractClosingTag && type == Tag::XmlTag && !isSingle && next)
    {
        while(next && next->tag->type == Tag::Empty)
            next = next->next;
        if(next)
        {
            closingNamespaceName = next->tag->nameSpace;
            closingNodeName = next->tag->name;
            closingCaseSensitive = next->tag->dtd()->caseSensitive;
            if(QuantaCommon::closesTag(namespaceName, nodeName, caseSensitive,
                                       closingNamespaceName, closingNodeName, closingCaseSensitive))
                extractNode(next, modifs, false, false);
        }
    }

#ifdef HEAVY_DEBUG
    coutTree(baseNode, 2);
#endif

    return node;
}

Node* kafkaCommon::DTDExtractNodeSubtree(Node *startNode, int startOffset, Node *endNode, int endOffset,
                                         Node **cursorNode, long &cursorOffset, NodeModifsSet *modifs, bool extractInlineParentNodes)
{
#ifdef LIGHT_DEBUG
    kdDebug(25001) << "kafkaCommon::extractNodeSubtree()" << endl;
#endif

    if(!startNode || !endNode)
        return 0;

    QValueList<int> commonParentStartChildLocation;
    QValueList<int> commonParentEndChildLocation;

    Node* commonParent = 0;

    NodeSelection cursorHolder;
    cursorHolder.setCursorNode(*cursorNode);
    cursorHolder.setCursorOffset(cursorOffset);

    splitStartAndEndNodeSubtree(startNode, startOffset, endNode, endOffset, commonParent,
                                commonParentStartChildLocation, commonParentEndChildLocation,
                                cursorHolder, 0, modifs, extractInlineParentNodes);

    *cursorNode = cursorHolder.cursorNode();
    cursorOffset = cursorHolder.cursorOffset();
    Node* commonParentStartChild = getNodeFromLocation(commonParentStartChildLocation);
    Node* commonParentEndChild = getNodeFromLocation(commonParentEndChildLocation);

    if(startNode == endNode)
    {
        Q_ASSERT(startNode->tag->type == Tag::Text || startNode->tag->type == Tag::Empty);

        Node* prev = startNode->prev;
        Node* next = startNode->next;

        Node* aux = extractNode(startNode, modifs);

        mergeInlineNode(prev, next, cursorNode, cursorOffset, modifs);

        return aux;
    }

    // now let us extract the subtree

    if(!commonParentEndChild)
        commonParentEndChild = endNode;
    extractNodeSubtreeAux(commonParentStartChild, commonParentEndChild, modifs);

    // merge identical nodes
    Node* commonParentEndChild_next = commonParentEndChild->SNext();
    mergeInlineNode(commonParent, commonParentEndChild_next, cursorNode, cursorOffset, modifs);
    mergeInlineNode(commonParentStartChild, commonParentEndChild, cursorNode, cursorOffset, modifs);

#ifdef LIGHT_DEBUG
    coutTree(commonParentStartChild, 3);
#endif

    return commonParentStartChild;
}

Node* kafkaCommon::DTDExtractNodeSubtree(Node *startNode, int startOffset, Node *endNode, int endOffset,
                                         Node* nodeSubtree, NodeModifsSet* modifs, bool extractInlineParentNodes)
{
#ifdef LIGHT_DEBUG
    kdDebug(25001) << "kafkaCommon::extractNodeSubtree()" << endl;
#endif

    if(!startNode || !endNode)
        return 0;

    QValueList<int> commonParentStartChildLocation;
    QValueList<int> commonParentEndChildLocation;

    Node* commonParent = 0;
    if(extractInlineParentNodes)
    {
        commonParent = DTDGetNonInlineCommonParent(startNode, endNode,
                commonParentStartChildLocation, commonParentEndChildLocation, nodeSubtree);
    }
    else
    {
        commonParent = DTDGetCommonParent(startNode, endNode,
                                          commonParentStartChildLocation, commonParentEndChildLocation, nodeSubtree);
    }
    assert(commonParent == nodeSubtree);

    NodeSelection selection;
    splitStartAndEndNodeSubtree(startNode, startOffset, endNode, endOffset, commonParent,
                                commonParentStartChildLocation, commonParentEndChildLocation,
                                selection, nodeSubtree, modifs);

    Node* cursorNode = selection.cursorNode();
    long cursorOffset = selection.cursorOffset();
    Node* commonParentStartChild = getNodeFromLocation(commonParentStartChildLocation, nodeSubtree);
    Node* commonParentEndChild = getNodeFromLocation(commonParentEndChildLocation, nodeSubtree);

    if(startNode == endNode)
    {
        Q_ASSERT(startNode->tag->type == Tag::Text || startNode->tag->type == Tag::Empty);
        return extractNode(startNode, modifs);
    }

    // now let us extract the subtree
    commonParentStartChild = getNodeFromLocation(commonParentStartChildLocation, commonParent);
    commonParentEndChild = getNodeFromLocation(commonParentEndChildLocation, commonParent);

    if(!commonParentEndChild)
        commonParentEndChild = endNode;
    extractNodeSubtreeAux(commonParentStartChild, commonParentEndChild, modifs);

    //merge identical nodes
/*    Node* cursorNode = 0;
    int cursorOffset = 0;*/
    Node* commonParentEndChild_next = commonParentEndChild->SNext();
    mergeInlineNode(commonParent, commonParentEndChild_next, &cursorNode, cursorOffset, modifs);
    mergeInlineNode(commonParentStartChild, commonParentEndChild, &cursorNode, cursorOffset, modifs);

#ifdef LIGHT_DEBUG
    coutTree(commonParentStartChild, 3);
#endif

    return commonParentStartChild;
}

Node* kafkaCommon::extractNodeSubtreeAux(Node* commonParentStartChild, Node* commonParentEndChild, NodeModifsSet* modifs)
{
    Node* node = commonParentStartChild;
    Node* prev_node = 0;
    Node* next_node = 0;
    Node* significant_next_node = 0;
    Node* node_extracted = 0;

    Node* commonParentEndChild_next = commonParentEndChild->SNext();

    while(node && node != commonParentEndChild_next)
    {
        next_node = node->next;
        significant_next_node = node->SNext();
        node_extracted = extractNode(node, modifs, true, true);
        if(node_extracted)
        {
            node_extracted->prev = prev_node;
            if(significant_next_node != commonParentEndChild_next || (next_node && next_node->closesPrevious))
                node_extracted->next = next_node;
            if(next_node && next_node->closesPrevious)
            {
                next_node->prev = node_extracted;
                node_extracted->_closingNode = next_node;
            }
        }
        prev_node = node_extracted;
        node = significant_next_node;
    }

    return commonParentStartChild;
}

Node* kafkaCommon::getNodeSubtree(Node *startNode, int startOffset, Node *endNode, int endOffset, bool extractInlineParentNodes)
{
#ifdef LIGHT_DEBUG
    kdDebug(25001) << "kafkaCommon::getNodeSubtree()" << endl;
#endif

    if(!startNode || !endNode)
        return 0;

    QValueList<int> commonParentStartChildLocation;
    QValueList<int> commonParentEndChildLocation;

    Node* commonParent = 0;
    if(extractInlineParentNodes)
        commonParent = DTDGetNonInlineCommonParent(startNode, endNode,
            commonParentStartChildLocation, commonParentEndChildLocation, 0);
    else
        commonParent = DTDGetCommonParent(startNode, endNode,
                                          commonParentStartChildLocation, commonParentEndChildLocation, 0);

    // get the subtree to operate
    Node* newStartNode = 0;
    Node* newEndNode = 0;

    Node* newCommonParent = duplicateNodeSubtree(commonParent, true);

    QValueList<int> const startNodeLocation = getLocation(startNode);
    QValueList<int> const commonParentLocation = getLocation(commonParent);
    uint const commonParentDepth = commonParentLocation.size();
    uint const newStartNodeDepth = startNodeLocation.size() - commonParentDepth + 1;
    uint const newEndNodeDepth = startNodeLocation.size() - commonParentDepth + 1;

    QValueList<int> newStartNodeLocation, newEndNodeLocation;
    newStartNodeLocation.push_back(1);
    newEndNodeLocation.push_back(1);

    for(uint i = 1; i != newStartNodeDepth; ++i)
        newStartNodeLocation.push_back(startNodeLocation[i + commonParentDepth - 1]);

    QValueList<int> const endNodeLocation = getLocation(endNode);
    for(uint i = 1; i != newEndNodeDepth; ++i)
        newEndNodeLocation.push_back(endNodeLocation[i + commonParentDepth - 1]);

    newStartNode = getNodeFromLocation(newStartNodeLocation, newCommonParent);
    newEndNode = getNodeFromLocation(newEndNodeLocation, newCommonParent);

    return DTDExtractNodeSubtree(newStartNode, startOffset, newEndNode, endOffset, newCommonParent, 0);
}

Node* kafkaCommon::DTDRemoveSelection(NodeSelectionInd& selection,
                                      Node **cursorNode, long& cursorOffset, NodeModifsSet *modifs, bool extractInlineParentNodes)
{
    Q_ASSERT(selection.hasSelection());

    int startOffset = selection.cursorOffset();
    int endOffset = selection.cursorOffsetEndSel();
    Node* startNode = getNodeFromLocation(selection.cursorNode());
    Node* endNode = getNodeFromLocation(selection.cursorNodeEndSel());

    return DTDExtractNodeSubtree(startNode, startOffset, endNode, endOffset, cursorNode, cursorOffset, modifs, extractInlineParentNodes);
}

void kafkaCommon::extractAndDeleteNode(Node *node, NodeModifsSet *modifs, bool deleteChildren,
                                       bool deleteClosingTag, bool mergeAndFormat)
{
    NodeModif modif;
    Node  *curNode, *nodePrev, *nodeNext, *nodeNext2, *n, *n2;
    QString nodeName, closingNodeName, namespaceName, namespaceName2;
    bool isSingle, caseSensitive, caseSensitive2;

    if(!node)
        return;

    isSingle = node->tag->single;
    nodeName = node->tag->name;
    namespaceName = node->tag->nameSpace;
    caseSensitive = node->tag->dtd()->caseSensitive;
    nodePrev = node->prev;
    nodeNext = node->next;
    if(!node->child)
        deleteChildren = true;
    node = extractNode(node, modifs, deleteChildren);

    //delete the closing Tag
    if(!isSingle && deleteClosingTag && nodeNext)
    {
        curNode = nodeNext;
        while(curNode && curNode->tag->type == Tag::Empty)
            curNode = curNode->next;
        if(curNode)
        {
            closingNodeName = curNode->tag->name;
            namespaceName2 = curNode->tag->nameSpace;
            caseSensitive2 = curNode->tag->dtd()->caseSensitive;
            if(QuantaCommon::closesTag(namespaceName, nodeName, caseSensitive,
                                       namespaceName2, closingNodeName, caseSensitive2))
            {
                curNode = nodeNext;
                while(curNode)
                {
                    nodeNext2 = curNode->next;
                    closingNodeName = curNode->tag->name;
                    namespaceName2 = curNode->tag->nameSpace;
                    caseSensitive2 = curNode->tag->dtd()->caseSensitive;
                    curNode = extractNode(curNode, modifs, deleteChildren);
                    curNode = nodeNext2;
                    if(QuantaCommon::closesTag(namespaceName, nodeName, caseSensitive,
                                               namespaceName2, closingNodeName, caseSensitive2))
                        break;
                }
                nodeNext = curNode;
            }
        }
    }

    //merge the next and prev Nodes if they are both of type Text or Empty
    if(mergeAndFormat && nodePrev)
    {
        n = nodePrev;
        n2 = nodePrev->next;
        while(n && n2 && n2->prev != nodeNext)
        {
            if(!mergeNodes(n, n2, modifs))
                break;
            n2 = n->next;
        }
    }
}

int kafkaCommon::DTDExtractNode(const QString &nodeName, Document *doc, Node *startNode,
                                int startOffset, Node *endNode, int endOffset, Node **cursorNode, long &cursorOffset,
                                NodeModifsSet *modifs)
{
    QTag *nodeNameQTag, *parentQTag;
    Node *node, *lastNodeNameStartNode, *lastNodeNameEndNode;
    Node *parentNode, *newParentNode, *child, *next;
    bool goUp, nodesRemoved = false, DTDError = false, result;
    bool startNodeSplitted = false, endNodeSplitted = false;

    if(!doc || !startNode || !endNode)
        return kafkaCommon::extractionBadParameters;

    //First check that nodeName is really inline and that an area is selected.
    nodeNameQTag = QuantaCommon::tagFromDTD(doc->defaultDTD(), nodeName);
    if(!nodeNameQTag)
        return kafkaCommon::extractionBadParameters;
    if(!isInline(nodeName))
        return kafkaCommon::extractionBadParameters;
    if(startNode->tag->type == Tag::Text && startOffset == (signed)startNode->tag->tagStr().length())
    {
        startOffset = 0;
        while(startNode && startNode->nextSibling())
        {
            startNode = startNode->nextSibling();
            if(startNode == endNode || startNode->tag->type == Tag::Text)
                break;
        }
    }
    if(startNode == endNode && startOffset == endOffset)
        return kafkaCommon::extractionBadParameters;

    //Then, process startNode and endNode : look if a nodeName parent is one of
    //startNode/endNode's inline parents and if it is the case, split the necessary Nodes.
    //The comparaison is made in lowercase, even in xml : it could be strange, for an user, to have
    //its nodes not removed because there are in the wrong case.
    node = startNode;
    lastNodeNameStartNode = 0L;
    while(node && (isInline(node->tag->name) || node->tag->type == Tag::Text))
    {
        if(node->tag->name.lower() == nodeName.lower())
            lastNodeNameStartNode = node;
        node = node->parent;
    }
    node = endNode;
    lastNodeNameEndNode = 0L;
    while(node && (isInline(node->tag->name) || node->tag->type == Tag::Text))
    {
        if(node->tag->name.lower() == nodeName.lower())
            lastNodeNameEndNode = node;
        node = node->parent;
    }

    if(startNode->tag->type == Tag::Text)
    {
        if(splitNode(startNode, startOffset, modifs))
        {
            startNodeSplitted = true;
            //<TEMPORARY>
            if(startNode == (*cursorNode) && cursorOffset > startOffset)
            {
                (*cursorNode) = (*cursorNode)->nextSibling();
                cursorOffset -= startOffset;
            }
            //</TEMPORARY>
            if(startNode == endNode)
            {
                endNode = endNode->nextSibling();
                endOffset -= startOffset;
            }
            startNode = startNode->nextSibling();
        }
    }
    if(endNode->tag->type == Tag::Text)
    {
        result = splitNode(endNode, endOffset, modifs);
        if(result)
            endNodeSplitted = true;
        else if(!result && endOffset == 0)
            endNode = endNode->previousSibling();
    }

    if(lastNodeNameStartNode)
    {
        node = startNode;
        parentNode = startNode->parent;
        while(parentNode && parentNode != lastNodeNameStartNode->parent)
        {
            if(node != parentNode->firstChild())
            {
                newParentNode = duplicateNode(parentNode);
                insertNode(newParentNode, parentNode->parentNode(), parentNode, parentNode, modifs);
                child = parentNode->firstChild();
                while(child && child != startNode && !child->hasForChild(startNode))
                {
                    next = child->next;
                    moveNode(child, newParentNode, 0L, modifs);
                    child = next;
                }
            }
            node = parentNode;
            parentNode = parentNode->parent;
        }
    }
    if(lastNodeNameEndNode)
    {
        node = endNode;
        parentNode = endNode->parent;
        while(parentNode && parentNode != lastNodeNameEndNode->parent)
        {
            if(node != parentNode->SLastChild())
            {
                newParentNode = duplicateNode(parentNode);
                insertNode(newParentNode, parentNode->parentNode(), parentNode, parentNode, modifs);
                if(parentNode == lastNodeNameStartNode)
                    lastNodeNameStartNode = newParentNode;
                child = parentNode->firstChild();
                while(child)
                {
                    next = child->next;
                    moveNode(child, newParentNode, 0L, modifs);
                    if(child == endNode || child->hasForChild(endNode))
                    {
                        if(QuantaCommon::closesTag(child->tag, next->tag))
                            moveNode(next, newParentNode, 0L, modifs);
                        break;
                    }
                    child = next;
                }
            }
            node = parentNode;
            parentNode = parentNode->parent;
        }
    }

    //Now delete the nodeName Nodes when possible from lastNodeNameStartParent to endNode.
    node = lastNodeNameStartNode?lastNodeNameStartNode:startNode;
    goUp = false;
    while(node && !DTDError)
    {
        next = getNextNode(node, goUp);
        if(node->tag->type == Tag::XmlTag && node->tag->name.lower() == nodeName.lower())
        {
            parentQTag = QuantaCommon::tagFromDTD(node->parent);
            if(parentQTag)
            {
                child = node->firstChild();
                while(child)
                {
                    if(!parentQTag->isChild(child))
                        DTDError = true;
                    child = child->next;
                }
                if(!DTDError)
                {
                    extractNode(node, modifs, false, true);
                    nodesRemoved = true;
                }
            }
        }
        if(node == endNode)
            break;
        node = next;
    }

    //TODO: merge the unnecessary splitted Nodes.
    if(endNode && endNodeSplitted)
        mergeNodes(endNode, endNode->nextSibling(), modifs, true);
    if(startNode && startNodeSplitted)
    {
        node = startNode->previousSibling();
        result = mergeNodes(startNode->previousSibling(), startNode, modifs, true);
        startNode = node;
        //<TEMPORARY>
        if(result)
        {
            (*cursorNode) = node;
            cursorOffset += startOffset;
        }
        //</TEMPORARY>
    }

    if(DTDError)
        return kafkaCommon::extractionStoppedDueToBadNodes;
    else if(!nodesRemoved)
        return kafkaCommon::nothingExtracted;
    else
    {
        //merge when necessary some text/identical inlines.
        mergeInlineNode(startNode, endNode, cursorNode, cursorOffset, modifs);

        return kafkaCommon::extractionDone;
    }
}


void kafkaCommon::moveNode(Node *nodeToMove, Node *newParent, Node *newNextSibling,
                           NodeModifsSet *modifs, bool merge, bool moveClosingNode)
{
    NodeModif *modif = 0;
    Node *newNode, *closingNode;
    closingNode = nodeToMove->getClosingNode();

    //DON'T log the removal and addition of the same Node!! When spliting the undoRedo stack
    //it will delete the remove NodeModif and thus the Node inside which is the Node inserted.
    if(modifs)
    {
        modif = new NodeModif();
        modif->setType(NodeModif::NodeAndChildsMoved);
        modif->setLocation(getLocation(nodeToMove));
    }

    //extract the old Node.
    newNode = extractNode(nodeToMove, 0L, true);

    //insert the new Node.
    insertNode(newNode, newParent, newNextSibling, 0L, merge);
    if(modifs)
        modif->setFinalLocation(getLocation(newNode));

    if(moveClosingNode && closingNode)
        moveNode(closingNode, newParent, newNextSibling,
                 modifs, merge, false);

    if(modifs)
        modifs->addNodeModif(modif);
}

void kafkaCommon::moveNode(Node *nodeToMove, Node *newParent, Node *newNextSibling, NodeSelection& cursorHolder,
                           NodeModifsSet *modifs, bool merge, bool moveClosingNode)
{
    NodeModif *modif = 0;
    Node *newNode, *closingNode;
    closingNode = nodeToMove->getClosingNode();

    //DON'T log the removal and addition of the same Node!! When spliting the undoRedo stack
    //it will delete the remove NodeModif and thus the Node inside which is the Node inserted.
    if(modifs)
    {
        modif = new NodeModif();
        modif->setType(NodeModif::NodeAndChildsMoved);
        modif->setLocation(getLocation(nodeToMove));
    }

    //extract the old Node.
    newNode = extractNode(nodeToMove, 0L, true);
    cursorHolder.setCursorNode(newNode);

    //insert the new Node.
    insertNode(newNode, newParent, newNextSibling, cursorHolder, 0L, merge);
    if(modifs)
        modif->setFinalLocation(getLocation(newNode));

    if(moveClosingNode && closingNode)
        moveNode(closingNode, newParent, newNextSibling,
                 modifs, merge, false);

    if(modifs)
        modifs->addNodeModif(modif);
}

bool kafkaCommon::splitNode(Node *n, int offset, NodeModifsSet *modifs)
{
    NodeModif *modif;
    Tag *tag;
    QString tagStr;
    Node *node;

    if(!n || (n->tag->type != Tag::Text && n->tag->type != Tag::Empty) || offset <= 0 || offset >=
            (signed)n->tag->tagStr().length())
        return false;

    //logging
    if(modifs)
    {
        tag = new Tag(*(n->tag));
        modif = new NodeModif();
        modif->setType(NodeModif::NodeModified);
        modif->setTag(tag);
        modif->setLocation(getLocation(n));
        modifs->addNodeModif(modif);
    }

    tagStr = n->tag->tagStr();
    n->tag->setStr(tagStr.left(offset));

    if(n->tag->type == Tag::Text)
        node = createAndInsertNode("#text", tagStr.right(tagStr.length() - offset), Tag::Text, n->tag->write(),
                                   n->parent, n->next, modifs, false);
    else
        node = createAndInsertNode("", tagStr.right(tagStr.length() - offset), Tag::Empty, n->tag->write(),
                                   n->parent, n->next, modifs, false);

    //Node's string is a part of n's clean string
    node->tag->setCleanStrBuilt(true);
    node->tag->setIndentationDone(true);
    return true;
}

void kafkaCommon::splitStartNodeSubtree(Node* startNode, Node* commonParent,
                                        QValueList<int>& commonParentStartChildLocation, NodeModifsSet* modifs)
{
    //Then we "split" the lastValidStartParent - startNode subtree into two : the first part is untouched
    // and the second will be surrounded by the new Node. Same thing for endNode.
    Node* node = startNode;
    Node* parentNode = startNode->parent;
    Node* commonParentStartChild = 0;
    while(parentNode && commonParent && parentNode != commonParent)
    {
        if(node != parentNode->firstChild())
        {
            Node* newParentNode = duplicateNode(parentNode);
            insertNode(newParentNode, parentNode->parentNode(), parentNode, parentNode, modifs);
            Node* child = parentNode->firstChild();
            while(child && child != startNode && !child->hasForChild(startNode))
            {
                Node* next = child->next;
                moveNode(child, newParentNode, 0L, modifs);
                child = next;
            }
        }
        commonParentStartChild = parentNode;
        node = parentNode;
        parentNode = parentNode->parent;
    }

    if(commonParentStartChild)
        commonParentStartChildLocation = getLocation(commonParentStartChild);
}

void kafkaCommon::splitEndNodeSubtree(Node* endNode, Node* commonParent,
                                      QValueList<int>& commonParentStartChildLocation,
                                      QValueList<int>& commonParentEndChildLocation,
                                      bool subTree, NodeModifsSet* modifs)
{
    Node* node = endNode;
    Node* parentNode = endNode->parent;

    Node* aux = 0;
    if(subTree)
        aux = commonParent;
    else
        aux = baseNode;
    Node* commonParentStartChild = getNodeFromLocation(commonParentStartChildLocation, aux);
    Node* commonParentEndChild = getNodeFromLocation(commonParentEndChildLocation, aux);
    while(parentNode && commonParent && parentNode != commonParent)
    {
        if(node != parentNode->lastChild())
        {
            Node* newParentNode = duplicateNode(parentNode);
            insertNode(newParentNode, parentNode->parentNode(), parentNode, parentNode, modifs);
            if(parentNode == commonParentStartChild)
                commonParentStartChild = newParentNode;
            if(parentNode == commonParentEndChild)
                commonParentEndChild = newParentNode;
            Node* child = parentNode->firstChild();
            while(child)
            {
                Node* next = child->next;
                moveNode(child, newParentNode, 0L, modifs);
                if(child == endNode || child->hasForChild(endNode))
                {
                    if(QuantaCommon::closesTag(child->tag, next->tag))
                        moveNode(next, newParentNode, 0L, modifs);
                    break;
                }
                child = next;
            }
        }
        node = parentNode;
        parentNode = parentNode->parent;
    }
    commonParentStartChildLocation = getLocation(commonParentStartChild);
    commonParentEndChildLocation = getLocation(commonParentEndChild);
}

void kafkaCommon::splitStartAndEndNodeSubtree(Node*& startNode, int startOffset, Node*& endNode, int endOffset, Node*& commonParent,
                                              QValueList<int>& commonParentStartChildLocation,
                                              QValueList<int>& commonParentEndChildLocation,
                                              NodeSelection& cursorHolder,
                                              Node* subTree, NodeModifsSet* modifs, bool extractInlineParentNodes)
{
    assert(startNode && endNode);
    assert(startOffset >= 0);
    assert(endOffset >= 0);

    // get correct start and end nodes and offsets
    startNode = getCorrectStartNode(startNode, startOffset);
    endNode = getCorrectEndNode(endNode, endOffset);

    // look for common parent
    if(!commonParent)
    {
        if(extractInlineParentNodes)
            // get the non inline common parent
            commonParent =
                    DTDGetNonInlineCommonParent(startNode, endNode, commonParentStartChildLocation, commonParentEndChildLocation, subTree);
        else
            commonParent =
                    DTDGetCommonParent(startNode, endNode, commonParentStartChildLocation, commonParentEndChildLocation, subTree);
    }
    else
    {
        assert(commonParent->hasForChild(startNode));
        assert(commonParent->hasForChild(endNode));
        assert(!commonParentStartChildLocation.empty());
        assert(!commonParentEndChildLocation.empty());
    }

    Node* commonParentStartChild = kafkaCommon::getNodeFromLocation(commonParentStartChildLocation, subTree);
    Node* commonParentEndChild = kafkaCommon::getNodeFromLocation(commonParentEndChildLocation, subTree);

    Node* cursorNode = cursorHolder.cursorNode();
    int cursorOffset = cursorHolder.cursorOffset();

    // split start and end node
    if(splitNode(startNode, startOffset, modifs))
    {
        if(startNode == cursorNode && cursorOffset > startOffset)
        {
            cursorNode = cursorNode->nextSibling();
            cursorOffset -= startOffset;
        }
        if(startNode == commonParentStartChild)
            commonParentStartChild = commonParentStartChild->nextSibling();

        if(startNode == endNode)
        {
            endNode = endNode->nextSibling();
            endOffset -= startOffset;
        }
        startNode = startNode->nextSibling();
        startOffset = 0;
    }
    splitNode(endNode, endOffset, modifs);

    // split start and end nodes subtree in function of common parent
    commonParentStartChildLocation = kafkaCommon::getLocation(commonParentStartChild);
    splitStartNodeSubtree(startNode, commonParent, commonParentStartChildLocation, modifs);

    commonParentEndChildLocation = kafkaCommon::getLocation(commonParentEndChild);
    splitEndNodeSubtree(endNode, commonParent, commonParentStartChildLocation, commonParentEndChildLocation, subTree, modifs);

    cursorHolder.setCursorNode(cursorNode);
    cursorHolder.setCursorOffset(cursorOffset);
}

bool kafkaCommon::mergeNodes(Node *n, Node *n2, NodeModifsSet *modifs, bool mergeTextOnly)
{
    NodeModif *modif;
    Tag *tag;
    if(!n || !n2)
        return false;

    if(((n->tag->type == Tag::Empty && !mergeTextOnly) || n->tag->type == Tag::Text) &&
            ((n2->tag->type == Tag::Empty && !mergeTextOnly) || n2->tag->type == Tag::Text))
    {
        tag = new Tag(*(n->tag));

        //logging
        if(modifs)
        {
            modif = new NodeModif();
            modif->setType(NodeModif::NodeModified);
            modif->setTag(tag);
            modif->setLocation(getLocation(n));
            modifs->addNodeModif(modif);
        }

        // have in consideration two spaces in a row
        QString nStr(n->tag->tagStr());
        QString n2Str(n2->tag->tagStr());
        if(nStr[nStr.length() - 1] == ' ' && n2Str[0] == ' ')
        {
            nStr = nStr.left(nStr.length() - 1);
            nStr.append("&nbsp;");
            n->tag->setStr(nStr);

            n2Str = n2Str.right(n2Str.length() - 1);
            n2Str.prepend("&nbsp;");
            n2->tag->setStr(n2Str);
        }

        if((n->tag->type == Tag::Text && n2->tag->type == Tag::Text) ||
                (n->tag->type == Tag::Empty && n2->tag->type == Tag::Empty))
            n->tag->setStr(n->tag->tagStr() + n2->tag->tagStr());
        else if(n->tag->type == Tag::Empty && n2->tag->type == Tag::Text)
            n->tag->setStr(n2->tag->tagStr());
        //else n's string is already in n

        if(n->tag->type == Tag::Text || n2->tag->type == Tag::Text)
            n->tag->type = Tag::Text;
        if(!n->tag->cleanStrBuilt() || !n2->tag->cleanStrBuilt())
            n->tag->setCleanStrBuilt(false);
        if(!n->tag->indentationDone() || !n2->tag->indentationDone())
            n->tag->setIndentationDone(false);
        kafkaCommon::extractAndDeleteNode(n2, modifs, false, false, false);

        return true;
    }
    return false;
}

bool kafkaCommon::mergeNodes(Node *n, Node *n2, NodeSelection& cursorHolder, NodeModifsSet *modifs, bool mergeTextOnly)
{
    NodeModif *modif;
    Tag *tag;
    if(!n || !n2)
        return false;

    if(((n->tag->type == Tag::Empty && !mergeTextOnly) || n->tag->type == Tag::Text) &&
         ((n2->tag->type == Tag::Empty && !mergeTextOnly) || n2->tag->type == Tag::Text))
    {
        tag = new Tag(*(n->tag));

        //logging
        if(modifs)
        {
            modif = new NodeModif();
            modif->setType(NodeModif::NodeModified);
            modif->setTag(tag);
            modif->setLocation(getLocation(n));
            modifs->addNodeModif(modif);
        }

        // have in consideration two spaces in a row
        QString nStr(n->tag->tagStr());
        QString n2Str(n2->tag->tagStr());
        if(nStr[nStr.length() - 1] == ' ' && n2Str[0] == ' ')
        {
            nStr = nStr.left(nStr.length() - 1);
            nStr.append("&nbsp;");
            n->tag->setStr(nStr);

            n2Str = n2Str.right(n2Str.length() - 1);
            n2Str.prepend("&nbsp;");
            n2->tag->setStr(n2Str);
        }

        if((n->tag->type == Tag::Text && n2->tag->type == Tag::Text) ||
            (n->tag->type == Tag::Empty && n2->tag->type == Tag::Empty))
        {
            if(cursorHolder.cursorNode() == n2)
                cursorHolder.setCursorOffset(n->tag->tagStr().length() + cursorHolder.cursorOffset() - 1);

            n->tag->setStr(n->tag->tagStr() + n2->tag->tagStr());
        }
        else if(n->tag->type == Tag::Empty && n2->tag->type == Tag::Text)
            n->tag->setStr(n2->tag->tagStr());
        //else n's string is already in n

        if(n->tag->type == Tag::Text || n2->tag->type == Tag::Text)
            n->tag->type = Tag::Text;
        if(!n->tag->cleanStrBuilt() || !n2->tag->cleanStrBuilt())
            n->tag->setCleanStrBuilt(false);
        if(!n->tag->indentationDone() || !n2->tag->indentationDone())
            n->tag->setIndentationDone(false);
        kafkaCommon::extractAndDeleteNode(n2, modifs, false, false, false);

        cursorHolder.setCursorNode(n);

        return true;
    }
    return false;
}

void kafkaCommon::mergeInlineNode(Node *startNode, Node *endNode, Node **cursorNode,
                                  long &cursorOffset, NodeModifsSet *modifs)
{
    Node *startNodeLastInlineParent, *parent, *node, *next;
    bool goUp, success, isCursorNode, isEndNode;
    int nodeLength;

    if(!startNode || !endNode)
        return;

    //first search for the last inline parent of startNode, and then its last prev neighbour
    // which is also inline : the merge will start from this Node.
    startNodeLastInlineParent = startNode;
    parent = startNode->parent;
    while(parent && isInline(parent->tag->name))
    {
        startNodeLastInlineParent = parent;
        parent = parent->parent;
    }
    if(startNodeLastInlineParent->prev)
    {
        if(startNodeLastInlineParent->prev->tag->type == Tag::Text)
            startNodeLastInlineParent = startNodeLastInlineParent->prev;
        else
        {
            node = startNodeLastInlineParent->prev;
            while(node && (node->tag->type == Tag::Empty || node->tag->type == Tag::XmlTagEnd))
                node = node->prev;
            if(node && node->tag->type == Tag::XmlTag && isInline(node->tag->name))
                startNodeLastInlineParent = node;
        }
    }


    //Then navigate though the tree and merge.
    node = startNodeLastInlineParent;
    goUp = false;
    while(node)
    {
        if(node->tag->type == Tag::XmlTag && isInline(node->tag->name))
        {
            next = node->next;
            while(next && (next->tag->type == Tag::XmlTagEnd || next->tag->type == Tag::Empty))
                next = next->next;
            while(next && next != node && compareNodes(node, next))
            {
                while(next->firstChild())
                    moveNode(next->firstChild(), node, 0L, modifs, false);
                if(next == endNode)
                    endNode = node;
                else if((*cursorNode) == node->next)
                {
                    //<TEMPORARY>
                    (*cursorNode) = node;
                    //</TEMPORARY>
                }
                extractNode(next, modifs, false, true);
                next = node->next;
                while(next && (next->tag->type == Tag::XmlTagEnd || next->tag->type == Tag::Empty))
                    next = next->next;
            }
        }
        else if(node->tag->type == Tag::Text)
        {
            while(node->next && (node->next->tag->type == Tag::Text ||
                                 node->next->tag->type == Tag::Empty))
            {
                nodeLength = (signed)node->tag->tagStr().length();
                isCursorNode = ((*cursorNode) == node->next);
                isEndNode = (endNode == node->next);
                success = mergeNodes(node, node->next, modifs);
                if(isCursorNode && success)
                {
                    //<TEMPORARY>
                    (*cursorNode) = node;
                    cursorOffset += nodeLength;
                    //</TEMPORARY>
                }
                else if(isEndNode && success)
                    endNode = node;
            }
        }
        if(node == endNode)
            break;
        node = getNextNode(node, goUp);
    }
}

void kafkaCommon::getEndPosition(const QString &tagString, int bLine, int bCol, int &eLine, int &eCol)
{
    /**int result, oldResult;

    result = tagString.find("\n", 0);
    if(result == -1)
    {
    	eLine = bLine;
    	eCol = bCol + tagString.length() - 1;
    }
    else
    {
    	eLine = bLine;
    	while(result != -1)
    	{
    		eLine++;
    		oldResult = result;
    		result = tagString.find("\n", result + 1);
    	}
    	eCol = tagString.length() - oldResult - 2;
    }*/
    int i;

    eLine = bLine;
    eCol = bCol - 1;
    for(i = 0; i < (signed)tagString.length(); ++i)
    {
        if(tagString[i] == "\n")
        {
            eLine++;
            eCol = -1;
        }
        else
            eCol++;
    }
}

void kafkaCommon::getEndPosition(Node *node, int bLine, int bCol, int &eLine, int &eCol)
{
    if(!node)
    {
        eLine = 0;
        eCol = 0;
        return;
    }

    getEndPosition(node->tag->tagStr(), bLine, bCol, eLine, eCol);
}

void kafkaCommon::setTagString(Node *node, const QString &newTagString, NodeModifsSet* modifs)
{
    int eLine, eCol, bLine, bCol;
    Tag *tag;
    NodeModif* modif;

    if(!node)
        return;

    //logging
    if(modifs)
    {
        tag = new Tag(*(node->tag));
        modif = new NodeModif();
        modif->setType(NodeModif::NodeModified);
        modif->setTag(tag);
        modif->setLocation(getLocation(node));
        modifs->addNodeModif(modif);
    }

    node->tag->beginPos(bLine, bCol);
    node->tag->setStr(newTagString);
    getEndPosition(node, bLine, bCol, eLine, eCol);
    node->tag->setTagPosition(bLine, bCol, eLine, eCol);
}

void kafkaCommon::setTagStringAndFitsNodes(Node *node, const QString &newTagString, NodeModifsSet* modifs)
{
    int eLine, eCol, oldELine, oldECol;
    bool b = false;

    if(!node)
        return;

    node->tag->endPos(oldELine, oldECol);
    setTagString(node, newTagString, modifs);
    node->tag->endPos(eLine, eCol);

    fitsNodesPosition(getNextNode(node, b), eCol - oldECol, eLine - oldELine);
}

void kafkaCommon::editNodeAttribute(Node* node, const QString& name, const QString& value, NodeModifsSet* modifs)
{
    NodeModif *modif = 0;

    if(!node)
        return;

    if(modifs)
    {
        modif = new NodeModif();
        modif->setType(NodeModif::NodeModified);
        modif->setTag(new Tag(*(node->tag)));
        modif->setLocation(getLocation(node));
    }

    if(node->tag->editAttribute(name, value))
    {
        node->tag->setCleanStrBuilt(false);
        if(modifs)
            modifs->addNodeModif(modif);
    }
}

QValueList<int> kafkaCommon::getLocation(Node * node)
{
    QValueList<int> loc;
    int i = 0;

    while(node)
    {
        i = 1;
        while(node->prev)
        {
            ++i;
            node = node->prev;
        }
        loc.prepend(i);
        node = node->parent;
    }
    return loc;
}

QValueList<int> kafkaCommon::getLocation(DOM::Node domNode)
{
    QValueList<int> loc;
    int i = 0;

    while(!domNode.isNull())
    {
        i = 1;
        while(!domNode.previousSibling().isNull())
        {
            ++i;
            domNode = domNode.previousSibling();
        }
        loc.prepend(i);
        domNode = domNode.parentNode();
    }
    return loc;
}

Node* kafkaCommon::getNodeFromLocation(QValueList<int> loc)
{
    QValueList<int>::iterator it;
    Node *node = baseNode;
    Node *m = 0L;
    int i;

    if(!node)
        return 0L;
    for(it = loc.begin(); it != loc.end(); ++it)
    {
        if(!node)
            return 0L;
        for(i = 1; i < (*it); ++i)
        {
            if(!node->next)
                return 0L;
            node = node->next;
        }
        m = node;
        node = node->child;
    }
    return m;
}

Node* kafkaCommon::getNodeFromLocation(QValueList<int> loc, Node* nodeTree)
{
    QValueList<int>::iterator it;
    Node *node = nodeTree;
    if(!node)
        node = baseNode;
    Node *m = 0L;
    int i;

    if(!node)
        return 0L;
    for(it = loc.begin(); it != loc.end(); ++it)
    {
        if(!node)
            return 0L;
        for(i = 1; i < (*it); ++i)
        {
            if(!node->next)
                return 0L;
            node = node->next;
        }
        m = node;
        node = node->child;
    }
    return m;
}

DOM::Node kafkaCommon::getNodeFromLocation(QValueList<int> loc, DOM::Node rootNode)
{
    QValueList<int>::iterator it;
    DOM::Node node = rootNode;
    DOM::Node m = rootNode;
    int i;

    if(rootNode.isNull())
        return DOM::Node();

    for(it = loc.begin(); it != loc.end(); ++it)
    {
        if(node.isNull())
            return DOM::Node();
        for(i = 1; i < (*it); ++i)
        {
            if(node.nextSibling().isNull())
                return DOM::Node();
            node = node.nextSibling();
        }
        m = node;
        node = node.firstChild();
    }
    return m;
}

Node* kafkaCommon::getNodeFromSubLocation(QValueList<int> loc, int locOffset)
{
    QValueList<int>::iterator it = loc.begin();
    QValueList<int> list;
    int i;

    for(i = 0; i < locOffset; ++i)
    {
        list.append((*it));
        ++it;
    }

    return getNodeFromLocation(list);
}

Node* kafkaCommon::getNodeFromSubLocation(QValueList<int> loc, int locOffset, Node* nodeTree)
{
    QValueList<int>::iterator it = loc.begin();
    QValueList<int> list;
    int i;

    for(i = 0; i != locOffset; ++i)
    {
        list.append((*it));
        ++it;
    }

    return getNodeFromLocation(list, nodeTree);
}

int kafkaCommon::compareNodePosition(QValueList<int> pos1, QValueList<int> pos2)
{
    QValueList<int>::iterator it1, it2;

    it1 = pos1.begin();
    it2 = pos2.begin();
    while(it1 != pos1.end() && it2 != pos2.end() && (*it1) == (*it2))
    {
        it1++;
        it2++;
    }

    if(it1 == pos1.end() && it2 == pos2.end())
        return kafkaCommon::isAtTheSamePosition;
    else if(it1 == pos1.end())
        return kafkaCommon::isBefore;
    else if(it2 == pos2.end() || (*it1) > (*it2))
        return kafkaCommon::isAfter;
    else if((*it1) < (*it2))
        return kafkaCommon::isBefore;
    else
        return kafkaCommon::positionError;
}

int kafkaCommon::compareNodePosition(Node *n1, Node *n2)
{
    QValueList<int> pos1, pos2;

    if(!n1 || !n2)
        return kafkaCommon::positionError;

    pos1 = getLocation(n1);
    pos2 = getLocation(n2);

    return compareNodePosition(pos1, pos2);
}

bool kafkaCommon::compareNodes(Node *n1, Node *n2)
{
    int i, j;

    if(!n1 || !n2)
        return false;

    if(n1->tag->type != n2->tag->type)
        return false;

    if(n1->tag->type == Tag::XmlTag)
    {
        if(n1->tag->name.lower() != n2->tag->name.lower())
            return false;

        if(n1->tag->attrCount() != n2->tag->attrCount())
            return false;

        for(i = 0; i < n1->tag->attrCount(); ++i)
        {
            for(j = 0; j < n2->tag->attrCount(); ++j)
            {
                if(n1->tag->getAttribute(i).name.lower() == n2->tag->getAttribute(j).name.lower() &&
                        n1->tag->getAttribute(i).value.lower() == n2->tag->getAttribute(j).value.lower())
                    break;
            }
            if(j == n2->tag->attrCount())
                return false;
        }
    }
    else if(n1->tag->type == Tag::Text)
    {
        //TODO
    }

    return true;
}

int kafkaCommon::nodeDepth(Node *node)
{
    int depth = 0;

    if(!node)
        return -1;

    node = node->parent;
    while(node)
    {
        depth++;
        node = node->parent;
    }

    return depth;
}

Node* kafkaCommon::hasParent(Node *node, const QString &name)
{
    node = node->parent;
    while(node)
    {
        if(node->tag->name.lower() == name.lower())
            return node;
        node = node->parent;
    }

    return 0L;
}

Node* kafkaCommon::hasParent(Node* startNode, Node* endNode, const QString &name)
{
    Q_ASSERT(startNode && endNode);
     //Andras: don't crash
    if (!startNode || !endNode)
      return 0;

    QValueList<int> commonParentStartChildLocation;
    QValueList<int> commonParentEndChildLocation;

    Node* node = DTDGetCommonParent(startNode, endNode, commonParentStartChildLocation, commonParentEndChildLocation, 0);

    while(node)
    {
        if(node->tag->name.lower() == name.lower())
            return node;
        node = node->parent;
    }

    return 0;
}

bool kafkaCommon::insertDomNode(DOM::Node node, DOM::Node parent, DOM::Node nextSibling,
                                DOM::Node rootNode)
{
    if(node.isNull())
        return false;

    if(parent.isNull())
    {
        if(rootNode.isNull())
            return false;
        parent = rootNode;
    }
    //Andras: avoid exceptions
    if (!nextSibling.isNull() && nextSibling.parentNode() != parent)
    {
      kdDebug(25001)<< "kafkaCommon::insertDomNode() - invalid nextSibling!" << endl;
      return false;
    }
    if (node.ownerDocument() != parent.ownerDocument())
    {
      kdDebug(25001)<< "kafkaCommon::insertDomNode() - ownerDocument is different!" << endl;
      return false;
    }

    try
    {
        parent.insertBefore(node, nextSibling);
    }
    catch(DOM::DOMException e)
    {
      kdDebug(25001)<< "kafkaCommon::insertDomNode() - ERROR code :" << e.code << endl;
    }
    return true;
}

bool kafkaCommon::removeDomNode(DOM::Node node)
{
    DOM::Node parent = node.parentNode();

    if(parent.isNull())
        return false;

    parent.removeChild(node);

    return true;
}

DOM::Node kafkaCommon::createDomNode(const QString &nodeName, const DTDStruct* dtd,
                                     DOM::Document rootNode)
{
    // FIXME
    //this will change with the futur multi-DTDs support
    //It does not use exceptions handling, so everything is checked via the DTEP definitions.
    DOM::Node dn;
    QTag *qTag = 0L;

    qTag = QuantaCommon::tagFromDTD(dtd, nodeName);

    if(qTag)
        dn =  rootNode.createElement(nodeName);
#ifdef HEAVY_DEBUG

    else
        kdDebug(25001)<< "kafkaCommon::createDomNode() - ERROR bad nodeName :" <<
        nodeName << endl;
#endif

    return dn;
}

DOM::Node kafkaCommon::createDomNode(Node *node, DOM::Document rootNode)
{
    if(!node)
        return DOM::Node();

    return createDomNode(node->tag->name, node->tag->dtd(), rootNode);
}

DOM::Node kafkaCommon::createTextDomNode(const QString &textString, DOM::Document rootNode)
{
    return rootNode.createTextNode(textString);
}

DOM::Node kafkaCommon::createDomNodeAttribute(const QString &nodeName, const DTDStruct* dtd,
        const QString &attrName, const QString &attrValue, DOM::Document rootNode)
{
    DOM::Node attr;
    QTag *qTag = 0L;

    qTag = QuantaCommon::tagFromDTD(dtd, nodeName);
    if(!qTag)
        return DOM::Node();

    if(qTag->isAttribute(attrName))
    {
        attr = rootNode.createAttribute(attrName);
        attr.setNodeValue(attrValue);
    }
#ifdef HEAVY_DEBUG
    else
        kdDebug(25001)<< "kafkaCommon::createDomNodeAttribute() - ERROR bad attrName " <<
        attrName << endl;
#endif

    return attr;
}

DOM::Node kafkaCommon::createDomNodeAttribute(Node* node, const QString &attrName,
        DOM::Document rootNode)
{
    if(!node)
        return DOM::Node();

    return createDomNodeAttribute(node->tag->name, node->tag->dtd(), attrName, "", rootNode);
}

//DOM::node kafkaCommon::createDomNodeAttribute(DOM::Node node, const QString &attrName,
//	DOM::Document rootNode)
//{
/**if(node.isNull())
	return DOM::node();

return createDomNodeAttribute()*/
//}

bool kafkaCommon::insertDomNodeAttribute(DOM::Node node, DOM::Node attr)
{
    if(node.isNull())
        return false;

    //should we check if the attr is valid???
    node.attributes().setNamedItem(attr);

    return true;
}

bool kafkaCommon::editDomNodeAttribute(DOM::Node node, const QString &nodeName, const DTDStruct* dtd,
                                       const QString &attrName, const QString &attrValue, DOM::Document rootNode)
{
    DOM::Node attr;

    if(node.isNull())
        return false;

    attr = node.attributes().getNamedItem(attrName);
    if(attr.isNull())
    {
        //let's create it
        attr = createDomNodeAttribute(nodeName, dtd, attrName, attrValue, rootNode);
        if(attr.isNull())
            return false;
        insertDomNodeAttribute(node, attr);
    }

    return true;
}

bool kafkaCommon::editDomNodeAttribute(DOM::Node domNode, Node* node,
                                       const QString &attrName, const QString &attrValue, DOM::Document rootNode)
{
    if(!node)
        return false;

    return editDomNodeAttribute(domNode, node->tag->name, node->tag->dtd(),
                                attrName, attrValue, rootNode);
}

DOM::Node kafkaCommon::hasParent(DOM::Node domNode, const QString &name)
{
    while(!domNode.isNull())
    {
        if(domNode.nodeName().string().lower() == name.lower())
            return domNode;
        domNode = domNode.parentNode();
    }

    return DOM::Node();
}

int kafkaCommon::childPosition(DOM::Node domNode)
{
    DOM::Node parentNode, child;
    int position = 1;

    if(domNode.isNull())
        return -1;

    parentNode = domNode.parentNode();
    child = parentNode.firstChild();
    while(!child.isNull() && child != domNode)
    {
        position++;
        child = child.nextSibling();
    }

    if(child == domNode)
        return position;
    else
        return -1;
}

DOM::Node kafkaCommon::getChildNode(DOM::Node parentNode, int position, bool fallback)
{
    DOM::Node child;

    if(parentNode.isNull())
        return DOM::Node();

    child = parentNode.firstChild();
    while(!child.isNull() && position > 1 && ((fallback && !child.nextSibling().isNull()) || !fallback ))
    {
        child = child.nextSibling();
        position--;
    }

    return child;
}

bool kafkaCommon::isInline(DOM::Node domNode)
{
    if(domNode.isNull())
        return false;

    if(domNode.nodeType() == DOM::Node::TEXT_NODE)
        return true;

    return isInline(domNode.nodeName().string());
}

bool kafkaCommon::parentSupports(DOM::Node parent, DOM::Node startNode, DOM::Node endNode,
                                 const DTDStruct* dtd)
{
    QTag *parentQTag;
    DOM::Node child;

    if(!dtd || parent.isNull())
        return false;

    parentQTag = QuantaCommon::tagFromDTD(dtd, parent.nodeName().string());

    if(!parentQTag)
        return false;

    child = startNode;
    while(!child.isNull())
    {
        if(!parentQTag->isChild(child.nodeName().string()))
            return false;
        if(child == endNode)
            return true;
        child = child.nextSibling();
    }

    return true;
}

bool kafkaCommon::isInline(const QString &nodeNam)
{
    QString nodeName = nodeNam.lower();
    if(nodeName == "q" || nodeName == "u" || nodeName == "i" || nodeName == "b" ||
            nodeName == "cite" || nodeName == "em" || nodeName == "var" || nodeName == "em" ||
            nodeName == "tt" || nodeName == "code" || nodeName == "kbd" || nodeName == "samp" ||
            nodeName == "big" || nodeName == "small" || nodeName == "s" || nodeName == "strike" ||
            nodeName == "sub" || nodeName == "sup" || nodeName == "abbr" ||
            nodeName == "acronym" || nodeName == "a" || nodeName == "bdo" ||
            nodeName == "font" || nodeName == "#text" || nodeName == "strong" || nodeName == "dfn" ||
            nodeName == "img" ||  nodeName == "applet" ||  nodeName == "object" ||  nodeName == "basefont" ||
            nodeName == "br" ||  nodeName == "script" ||  nodeName == "map" || nodeName == "span" ||
            nodeName == "iframe" || nodeName == "input" || nodeName == "select" || nodeName == "textarea" ||
            nodeName == "label" || nodeName == "button" )
        return true;
    else
        return false;
}

#ifdef HEAVY_DEBUG
void kafkaCommon::coutDomTree(DOM::Node rootNode, int indent)
#else
void kafkaCommon::coutDomTree(DOM::Node, int)
#endif
{
#ifdef HEAVY_DEBUG
    QString output, dots;
    int j;
    DOM::Node node;
    if(rootNode.isNull())
        kdDebug(25001)<< "kafkaCommon::coutDomTree() - bad node!" << endl;

    node = rootNode;
    while (!node.isNull())
    {
        dots = "";
        dots.fill('_', indent);
        output = dots;
        if (node.nodeType() != DOM::Node::TEXT_NODE)
            output += node.nodeName().string().replace('\n'," ");
        else
        {
            output += "\"";
            output+= node.nodeValue().string().replace('\n'," ");
            output += "\"";
        }
        kdDebug(25001) << output <<" (" << node.nodeType() << ") "<<
        node.handle() <<  endl;
        kdDebug(25001)<< dots << "  +++ prev " << node.previousSibling().handle() << " next " <<
        node.nextSibling().handle() << " parent " <<
        node.parentNode().handle() << " child " << node.firstChild().handle() << endl;
        for(j = 0; j < (int)node.attributes().length(); ++j)
        {
            kdDebug(25001)<< dots << " *** attr" << j << " " <<
            node.attributes().item(j).nodeName().string() <<  " - " <<
            node.attributes().item(j).nodeValue().string() << endl;
        }

        if (node.hasChildNodes())
            coutDomTree(node.firstChild(), indent + 4);
        node = node.nextSibling();
    }
#endif
}

void kafkaCommon::coutTree(Node *node, int indent)
{
    QString output, dots;
    int bLine, bCol, eLine, eCol, j;
    if(!node)
        kdDebug(25001)<< "kafkaCommon::coutTree() - bad node!" << endl;

    while (node)
    {
        dots = "";
        dots.fill('.', indent);
        output = dots;
        node->tag->beginPos(bLine, bCol);
        node->tag->endPos(eLine, eCol);
        if (node->tag->type == Tag::XmlTag || node->tag->type == Tag::XmlTagEnd ||
                node->tag->type == Tag::ScriptTag)
            output += node->tag->name.replace('\n',"<return>");
        else
        {
            output += "\"";
            output+= node->tag->tagStr().replace('\n',"<return>");
            output += "\"";
        }
        kdDebug(25001) << output <<" (" << node->tag->type << ", " << node->tag->cleanStrBuilt() << ", " <<
        node->tag->indentationDone() << ") "<< node << " at pos " << bLine << ":" << bCol << " - " <<
        eLine << ":" << eCol << endl;
        kdDebug(25001)<< dots << "  +++ prev " << node->prev << " next " << node->next << " parent " <<
        node->parent << " child " << node->child << endl;
        for(j = 0; j < node->tag->attrCount(); ++j)
        {
            kdDebug(25001)<< dots << " *** attr" << j << " " <<
            node->tag->getAttribute(j).nameLine << ":" <<
            node->tag->getAttribute(j).nameCol << ":" <<
            node->tag->getAttribute(j).name <<  " - " <<
            node->tag->getAttribute(j).valueLine << ":" <<
            node->tag->getAttribute(j).valueCol << ":" <<
            node->tag->getAttribute(j).value << endl;
        }

        if (node->child)
            coutTree(node->child, indent + 4);
        if(node == node->next || (node->next && node == node->next->next) ||
                (node->next && node->next->next && node == node->next->next->next) ||
                (node->next && node->next->next && node->next->next->next &&
                 node == node->next->next->next->next) || (node->next && node->next->next &&
                         node->next->next->next && node->next->next->next->next && node ==
                         node->next->next->next->next->next))
        {
            //try to detect invalid pointers.
            kdDebug(25001)<< "ERROR - node == node->[..]next" << endl;
            return;
        }
        node = node->next;
    }
}

int kafkaCommon::isInsideTag(Node* start_node, Node* end_node, QString const& tag_name)
{
    Q_ASSERT(start_node && end_node);
    //Andras: don't crash
    if (!start_node || !end_node)
      return -1;

    Node* tag_start = hasParent(start_node, end_node, tag_name);
    if(tag_start)
        return 1; // both start_node and end_node are surrounded by tag_name

    tag_start = hasParent(start_node, tag_name);
    if(tag_start)
        return 0; // only start_node has tag_name as parent

    tag_start = hasParent(end_node, tag_name);
    if(tag_start)
        return 0; // only end_node has tag_name as parent

    return -1; // neither the nodes have tag_name as parent
}

int kafkaCommon::isInsideTag(Node* start_node, Node* end_node, QString const& tag_name,
                             QString const& attribute_name, QString const& attribute_value)
{
    Q_ASSERT(start_node && end_node);
    //Andras: don't crash
    if (!start_node || !end_node)
      return -1;

    Node* tag_start = hasParent(start_node, end_node, tag_name);
    if(tag_start && tag_start->tag->hasAttribute(attribute_name) && tag_start->tag->attributeValue(attribute_name, true) == attribute_value)
        return 1; // both start_node and end_node are surrounded by tag_name

    tag_start = hasParent(start_node, tag_name);
    if(tag_start && tag_start->tag->hasAttribute(attribute_name) && tag_start->tag->attributeValue(attribute_name, true) == attribute_value)
        return 0; // only start_node has tag_name as parent

    tag_start = hasParent(end_node, tag_name);
    if(tag_start && tag_start->tag->hasAttribute(attribute_name) && tag_start->tag->attributeValue(attribute_name, true) == attribute_value)
        return 0; // only end_node has tag_name as parent

    return -1; // neither the nodes have tag_name as parent
}

bool kafkaCommon::isBetweenWords(Node* node, int offset)
{
    Q_ASSERT(node->tag->type == Tag::Text || node->tag->type == Tag::Empty);
    Q_ASSERT(offset >= 0);
    Q_ASSERT(node);
    if (!node)
      return false;  //FIXME: Andras: don't crash

    QString tag_str = node->tag->tagStr();

    return !
            (tag_str[offset].isSpace() || tag_str[offset].isPunct() ||
            tag_str[offset - 1].isSpace() || tag_str[offset - 1].isPunct());/* ||
    tag_str[offset + 1].isSpace() || tag_str[offset + 1].isPunct());*/
}

void kafkaCommon::getStartOfWord(Node*& node, int& offset)
{
  Q_ASSERT(node);
//     Q_ASSERT(isBetweenWords(node, offset)); recursive
  Q_ASSERT(offset >= 0);
  //Andras: don't crash
  if (!node || offset < 0)
    return;

    kdDebug(23100) << "getStartOfWord node length: " << node->tag->tagStr().length() << endl;
    kdDebug(23100) << "getStartOfWord offset BEGIN: " << offset << endl;

    QString tag_str = node->tag->tagStr();
    while(offset >= 0 && !tag_str[offset].isSpace() && !tag_str[offset].isPunct())
        --offset;

    if(offset == -1)
    {
        Node* aux = node->previousSibling();
        while(aux && aux->tag->type != Tag::Text)
        {
            if(!isInline(aux->tag->name))
            {
                ++offset;
                return;
            }

            aux = aux->previousSibling();
        }
        if(aux)
        {
            node = aux;
            offset = aux->tag->tagStr().length() - 1;
            kdDebug(23100) << "getStartOfWord node length: " << node->tag->tagStr().length() << endl;
            kdDebug(23100) << "getStartOfWord offset RECURS: " << offset << endl;
            getStartOfWord(node, offset);
            return;
        }
    }
    ++offset;
    kdDebug(23100) << "getStartOfWord node length: " << node->tag->tagStr().length() << endl;
    kdDebug(23100) << "getStartOfWord offset END: " << offset << endl;
}

void kafkaCommon::getEndOfWord(Node*& node, int& offset)
{
    Q_ASSERT(node);
//     assert(isBetweenWords(node, offset)); recursive
    Q_ASSERT(isBetweenWords(node, offset));
    Q_ASSERT(offset >= 0);

    //Andras: if the following asserts are hit, don't do anything = don't crash
    if (!node || !isBetweenWords(node, offset) || offset < 0)
      return;


    QString tag_str = node->tag->tagStr();
    while((uint)offset != tag_str.length() && !tag_str[offset].isSpace() && !tag_str[offset].isPunct())
        ++offset;

    if((uint)offset == tag_str.length())
    {
        Node* aux = node->nextSibling();
        while(aux && aux->tag->type != Tag::Text)
        {
            if(!isInline(aux->tag->name))
                return;

            aux = aux->nextSibling();
        }
        if(aux)
        {
            node = aux;
            offset = 0;
            getEndOfWord(node, offset);
        }
    }
}

void kafkaCommon::getStartOfParagraph(Node*& node, int& offset)
{
    Q_ASSERT(node);
    //Andras: don't crash
    if (!node)
    {
      offset = 0;
      return;
    }

    Node* previous = node->previousSibling();
    while(previous && (isInline(previous->tag->name) || previous->tag->name.lower() == "br" || previous->tag->type == Tag::Text))
        previous = previous->previousSibling();

    offset = 0;
    if(previous)
    {
        node = previous->nextSibling();
        return;
    }
    Q_ASSERT(node->tag->type == Tag::Text);
}

void kafkaCommon::getEndOfParagraph(Node*& node, int& offset)
{
    Q_ASSERT(node);
    if (!node)
    {
      offset = 0;
      return;
    }

    Node* begin_paragraph = node;
    getStartOfParagraph(begin_paragraph, offset);

    Node* next = begin_paragraph->nextSibling();
    while(nodeDepth(next) > nodeDepth(begin_paragraph))
        next = next->nextSibling();
    while(next && (isInline(next->tag->name) || next->tag->name.lower() == "br" || next->tag->type == Tag::Text))
    {
        next = next->nextSibling();
        while(nodeDepth(next) > nodeDepth(node))
            next = next->nextSibling();
    }
    if(next)
    {
        node = next;
        if(nodeDepth(next) < nodeDepth(begin_paragraph))
            node = node->previousSibling();

        if(node->tag->type == Tag::Text)
            offset = node->tag->tagStr().length() - 1;
        else
            offset = 0;
        return;
    }
}


