/***************************************************************************
                                   cursor.cpp
                             -------------------

    copyright            : (C) 2004 - Nicolas Deschildre
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
 

#include "wkafkapart.h"
#include "kafkacommon.h"
#include "viewmanager.h"
  
#include "cursors.h"
 
 
 NodeSelectionInd::NodeSelectionInd()
{
  m_cursorOffset = -1;
  m_cursorOffsetEndSel = -1;
  m_cursorAtSelectionStart = true;
}
  
NodeSelectionInd::~NodeSelectionInd()
{
}

bool NodeSelectionInd::operator==(const NodeSelectionInd & nodeSelection)
{
  return (m_cursorNode == nodeSelection.m_cursorNode && m_cursorNodeEndSel == nodeSelection.m_cursorNodeEndSel &&
    m_cursorOffset == nodeSelection.m_cursorOffset && m_cursorOffsetEndSel == nodeSelection.m_cursorOffsetEndSel &&
    m_cursorAtSelectionStart == nodeSelection.m_cursorAtSelectionStart);
}

void NodeSelectionInd::operator=(const NodeSelectionInd & nodeSelection)
{
  m_cursorNode = nodeSelection.m_cursorNode;
  m_cursorNodeEndSel = nodeSelection.m_cursorNodeEndSel;
  m_cursorOffset = nodeSelection.m_cursorOffset;
  m_cursorOffsetEndSel = nodeSelection.m_cursorOffsetEndSel;
  m_cursorAtSelectionStart = nodeSelection.m_cursorAtSelectionStart;
}

void NodeSelectionInd::fillWithVPLCursorSelection()
{
  KafkaDocument *kafkaDoc;
  DOM::Node domNode, domNodeEndSel;
  int domOffset, domOffsetEndSel;
  Node *node, *nodeEndSel;
  int offset, offsetEndSel;

  kafkaDoc = KafkaDocument::ref();
  kafkaDoc->getKafkaWidget()->getCurrentNode(domNode, domOffset);
  kafkaDoc->translateKafkaIntoNodeCursorPosition(domNode, domOffset, &node, offset);
  m_cursorNode = kafkaCommon::getLocation(node);
  m_cursorOffset = offset;
  
  if(kafkaDoc->getKafkaWidget()->hasSelection())
  {
    kafkaDoc->getKafkaWidget()->selection(domNode, (long&)domOffset, domNodeEndSel, (long&)domOffsetEndSel);
    KafkaDocument::ref()->translateKafkaIntoNodeCursorPosition(domNodeEndSel, domOffsetEndSel,
      &nodeEndSel, offsetEndSel);
    m_cursorNodeEndSel = kafkaCommon::getLocation(nodeEndSel);
    m_cursorOffsetEndSel = offsetEndSel;
    
    m_cursorAtSelectionStart = !(m_cursorOffsetEndSel == m_cursorOffset && m_cursorNodeEndSel == m_cursorNode);
    
    if(!m_cursorAtSelectionStart)
    {
      KafkaDocument::ref()->translateKafkaIntoNodeCursorPosition(domNode, domOffset, &node, offset);
      m_cursorNode = kafkaCommon::getLocation(node);
      m_cursorOffset = offset;
    }
  }
}

bool NodeSelectionInd::hasSelection() const
{
    return KafkaDocument::ref()->getKafkaWidget()->hasSelection();
}
