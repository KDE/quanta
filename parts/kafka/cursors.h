/***************************************************************************
                                   cursor.h
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
 
 #ifndef CURSORS_H
 #define CURSORS_H
 
 /**
  * This represents a Node selection : StartNode and StartOffset, endNode and endOffset.
  * IT can also only hold the cursor. If so, cursorNodeEndSel() will be empty.
  * TODO: selection support
  */
 class NodeSelection
 {
 public:
   NodeSelection(Node* node = 0L, int offset = 0){m_cursorNode = node; m_cursorOffset = offset;}
   ~NodeSelection(){}
   
   /**
    * @return Return the Node when the selection start (and where the cursor is if 
    * m_cursorAtSelectionStart).
    */
   Node* cursorNode(){return m_cursorNode;}
   void setCursorNode(Node* node){m_cursorNode = node;}
   
   /**
    * @return Returns the offset of the cursor where the selection begin (and where the cursor is if 
    * m_cursorAtSelectionStart)
    */
   int cursorOffset(){return m_cursorOffset;}
   void setCursorOffset(int offset){m_cursorOffset = offset;}
   
 private:
  Node* m_cursorNode, *m_cursorNodeEndSel;
  int m_cursorOffset, m_cursorOffsetEndSel;
  bool m_cursorAtSelectionStart;
 };
 
 /**
 * This represents a Node selection : startNode and startOffset, endNode and endOffset.
 * The difference with NodeSelection is that it don't store the Node address of startNode
 * and endNode, but it store the Node position of the Node tree e.g. it is the first child
 * of the second child of the rootNode...
 * It can also only hold the cursor. If so, cursorNodeEndSel() will be empty.
 */
class NodeSelectionInd
{
public:
  NodeSelectionInd();
  NodeSelectionInd(Node* cursor_node, int cursor_offset);
  NodeSelectionInd(Node* start_node, int start_offset, Node* end_node, int end_offset);
  ~NodeSelectionInd();
  
  /**
   * Compare nodeselection to the current instance.
   */
  bool operator==(const NodeSelectionInd & nodeSelection);
  
  void operator=(const NodeSelectionInd & nodeSelection);
  
  /**
   * Take the current VPL cursor selection coordinates, if not speficied otherwise, translate
   * them into Node cursor coordinates and store them.
   */
  void fillWithVPLCursorSelection();
  
  bool hasSelection() const;
  
  /**---------------------------------- GET/SET --------------------------------------------*/

  /**
  * @return Returns the location of the Node where the selection begin (and where the cursor is if 
  * m_cursorAtSelectionStart).
  */
  QValueList<int>& cursorNode() {return m_cursorNode;}
  
  /**
  * @param cursorNode Set the location of the Node where the selection begin (and where the cursor is if 
  * m_cursorAtSelectionStart)
  */
  void setCursorNode(QValueList<int> cursorNode) {m_cursorNode = cursorNode;}
  
  /**
  * @return Returns the offset of the cursor where the selection begin (and where the cursor is if 
  * m_cursorAtSelectionStart)
  */
  int cursorOffset() {return m_cursorOffset;}
  
  /**
  * @param cursorOffset Set the offset of the cursor where the selection begin (and where the cursor is if 
  * m_cursorAtSelectionStart)
  */
  void setCursorOffset(int cursorOffset) {m_cursorOffset = cursorOffset;}
  
  /**
   * @return Returns true if the cursor is at the beginning of the selection. Otherwise, it is
   * at the end of the selection.
   */
  bool cursorAtSelectionStart() {return m_cursorAtSelectionStart;}
  
  /**
   * @param cursorAtSelectionStart Set if the cursor is at the beginning of the selection.
   */
  void setCursorAtSelectionStart(bool cursorAtSelectionStart) {m_cursorAtSelectionStart = cursorAtSelectionStart;}
  
  /**
  * @return Returns the location of the end selection Node where the
  * selection ends.
  * Empty if this instance only carry the cursor coordinates.
  */
  QValueList<int>& cursorNodeEndSel() {return m_cursorNodeEndSel;}
  
  /**
  * @param cursorNodeEndSel Set the location of the end selection Node where the
  * selection ends.
  */
  void setCursorNodeEndSel(QValueList<int> cursorNodeEndSel) 
    {m_cursorNodeEndSel = cursorNodeEndSel;}
    
  /**
  * @return Returns the offset of the cursor in the Node of the end of the selection.
  */
  int cursorOffsetEndSel() {return m_cursorOffsetEndSel;}
  
  /**
  * @param cursorOffsetEndSel Set the offset of the cursor in the Node of the end of the selection
  */
  void setCursorOffsetEndSel(int cursorOffsetEndSel)
    {m_cursorOffsetEndSel = cursorOffsetEndSel;}
  
private:
  QValueList<int> m_cursorNode, m_cursorNodeEndSel;
  int m_cursorOffset, m_cursorOffsetEndSel;
  bool m_cursorAtSelectionStart;
};

#endif
