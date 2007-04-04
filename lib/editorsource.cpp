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
#include "editorsource.h"

#include <ktexteditor/range.h>

#include <kdebug.h>

#include <idocument.h>

EditorSource::EditorSource(KDevelop::IDocument *document, QObject *parent)
  : QObject(parent)
{
  m_ideDocument = document;
  m_document = m_ideDocument->textDocument();
  m_view = m_document->activeView();


  m_copyAction = m_document->action("edit_copy");
  m_cutAction = m_document->action("edit_cut");
  m_pasteAction = m_document->action("edit_paste");
}


EditorSource::~EditorSource()
{
}

/**
 * We cannot use directly m_editIf->text() because it fails in some cases, like
 * when the end column is -1. In this case it will return the whole end line, instead of
 * returning everything until the end line (excluding it).
 */
QString EditorSource::text(int bLine, int bCol, int eLine, int eCol) const
{
  if (bLine > eLine)
  {
    qSwap(bLine, eLine);
    qSwap(bCol, eCol);
  }
  if (bCol < 0)
  {
    bLine--;
    if (bLine < 0)
        bLine = 0;
    bCol = m_document->lineLength(bLine);
  }
  if (eCol < 0)
  {
    eLine--;
    if (eLine < 0)
        eLine = 0;
    eCol = m_document->lineLength(eLine);
  }
  //use eCol + 1, so the char at eCol is returned as well
  return m_document->text(KTextEditor::Range(bLine, bCol, eLine, eCol + 1));
}


QString EditorSource::getLine(int line, int endLine, int endCol)
{
  QString textLine = m_document->line(line);
  if (line == endLine)
  {
    if (endCol >0)
      textLine.truncate(endCol + 1);
    else
      textLine = "";
  }
  return textLine;
}


void EditorSource::insertText(const QString &text, bool adjustCursor)
{
  if (text.isEmpty())
    return;

  uint line, col;

  KTextEditor::Cursor c = m_view->cursorPosition();
  line = c.line();
  col = c.column();
  m_document->insertText(c, text);

  // calculate new cursor position
  // counts the words and whitespace of the text so we can place the
  // cursor correctly and quickly with the viewCursorInterace, avoiding
  // the KTexEditor::insertText method
  if (adjustCursor)
  {
    unsigned textLength = text.length();
    unsigned int wordWrapAt = 80;
    bool noWordWrap = false;

    //FIXME: What to use here???
/*    KTextEditor::WordWrapInterface *wordWrapIf = dynamic_cast<KTextEditor::WordWrapInterface *>(m_document);
    if (wordWrapIf)
    {
        wordWrapAt = wordWrapIf->wordWrapAt();
        noWordWrap = !(wordWrapIf->wordWrap());
     }*/
    wordWrapAt = 80;
    noWordWrap = false;


    uint i=0, j=0;
    int wordLength;
    const char *ascii = text.toLatin1(); // use ascii for maximum speed
    bool lineLock = false;

    while (i < textLength)
    {
      if (ascii[i] == '\n') // add a line, first column
      {
        ++line;
        col=0;
        ++i;
        lineLock = false;
      }
      else if (ascii[i] == '\r')
      {
        col = 0; ++i;
      }
      else if (!noWordWrap && !(QChar(ascii[i]).isSpace())) // new word, see if it wraps
      {
      // TOO SLOW  int wordLength = (text.mid(i).section(QRegExp("[ \t\r\n]"), 0, 0).length());
        wordLength = -1;
        for (j = i+1; ascii[j]; ++j) // get word size, ascii is MUCH faster
        {
          if (QChar(ascii[j]).isSpace())
           {
              wordLength = j-i;
              break;
           }
        }
        if (wordLength == -1)
          wordLength = (textLength)-i;

        if ((wordLength+col) > wordWrapAt)
        {
          if (col && !lineLock) // wraps onto new line unless locked by kate
          {
            col=0;
            ++line;
          }
        }
        col += wordLength;
        i += wordLength;
        if (wordLength > (int) wordWrapAt)
          lineLock = true; // words > wordWrapAt lock the rest of the line
      }
      else // whitespace
      {
        ++col; ++i;
        if (!noWordWrap)
          if (col > wordWrapAt && !lineLock)  // wrap like words
          {
            col -= wordWrapAt;
            ++line;
          }
      }
    }
  }
  m_view->setCursorPosition(KTextEditor::Cursor(line, col));
}


void EditorSource::selectArea(const AreaStruct &area)
{
  m_view->setSelection(KTextEditor::Range(area.bLine(), area.bCol(), area.eLine(), area.eCol() + 1));
  m_view->setFocus();
}


void EditorSource::setCursorPosition(int line, int col)
{
  m_view->setCursorPosition(KTextEditor::Cursor(line, col));
  m_view->setFocus();
}


void EditorSource::getPreviousPosition(int &line, int &column) const
{
  column--;
  if (column < 0)
  {
    line--;
    if (line < 0)
    {
      line = 0;
      column = 0;
    } else
      column = m_document->lineLength(line);
  }
}


#include "editorsource.moc"

// kate: space-indent on; indent-width 2; mixedindent off; indent-mode cstyle;
