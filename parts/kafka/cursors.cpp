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

}
  
NodeSelectionInd::~NodeSelectionInd()
{

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
  KafkaDocument::ref()->translateKafkaIntoNodeCursorPosition(domNode, domOffset, &node, offset);
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
