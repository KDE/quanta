/***************************************************************************
    begin                : Fri Jun 17 2005
    copyright            : (C) 2005 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/
#ifndef EDITORSOURCE_H
#define EDITORSOURCE_H

#include "areastruct.h"
#include "quantaexport.h"

#include <kurl.h>
#include <ktexteditor/document.h>
#include <ktexteditor/view.h>
#include <ktexteditor/cursor.h>

namespace KDevelop {
 class IDocument; 
}

/**
This class can be used to access the editor functionality in an abstracted way.

@author Andras Mantia
*/
class KDEVQUANTA_EXPORT EditorSource : public QObject
{
Q_OBJECT
public:
  explicit EditorSource(KDevelop::IDocument *document, QObject *parent = 0);
  ~EditorSource();

//TODO Needed methods:
  int lineLength(uint line) const {return m_document->lineLength(line);}

  QString text() const {return m_document->text();}

  QString text(int startLine, int startCol, int endLine, int endCol) const;

  QString text(const AreaStruct& area) const {return text(area.bLine(), area.bCol(), area.eLine(), area.eCol());}

  QString selection() const {return m_view->selectionText();}

  KDE_DEPRECATED void cursorPositionReal(uint &line, uint &col)
    {
      KTextEditor::Cursor c = m_view->cursorPosition();
      line = c.line();
      col = c.column();
    }

  KDE_DEPRECATED uint cursorColumnReal() const
  {
    KTextEditor::Cursor c = m_view->cursorPosition();
    return c.column();
  }

  uint numLines() const {return m_document->lines();}

  QString getLine(int line, int endLine, int endCol);

  QString textLine(uint line) const {return m_document->line(line);}

  /** Inserts text at the current cursor position
   *
   * @param text the text to be inserted
   * @param adjustCursor true if the cursor should be moved to end of the inserted text
  */
  void insertText(const QString &text, bool adjustCursor = true);

  /**
   * @return the @KUrl of the source of the text
   */
  KUrl url() const {return m_document ? m_document->url() : KUrl();};

  /**Decrements a column value (gets the position of the previous char in the document).
   *  If the column is 0, it will change to the last column value from the
   * previous line and the line will be also decremented.
   *
   * @param line the current line
   * @param column the current column
   */
  void getPreviousPosition(int &line, int &column) const;

  /**
   * @return pointer to the cut action
   */
  QAction * cutAction() const {return m_cutAction;};

  /**
   * @return pointer to the copy action
   */
  QAction * copyAction() const {return m_copyAction;};

  /**
   * @return pointer to the paste action
   */
  QAction * pasteAction() const {return m_pasteAction;};

  /**set the selection to the area and get the focus
   *
   * @param area
   */
  void selectArea(const AreaStruct &area);

  /** move the cursor to a new position and get the focus
   *
   * @param line
   * @param col
   */
  void setCursorPosition(int line, int col);
  
  

protected:
  KDevelop::IDocument *m_ideDocument;
  KTextEditor::Document *m_document;
  KTextEditor::View *m_view;

  QAction * m_copyAction;
  QAction * m_cutAction;
  QAction * m_pasteAction;
};

#endif

// kate: space-indent on; indent-width 2; mixedindent off; indent-mode cstyle;
