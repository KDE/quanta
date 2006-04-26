/***************************************************************************
    begin                : Mon Jun 27 2005
    copyright            : (C) 2005 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/
#ifndef QUANTADOC_H
#define QUANTADOC_H

#include "parseresult.h"
#include "editorsource.h"

#include <qobject.h>

struct AreaStruct;
struct DTDStruct;
class CompletionBase;
class EditorSource;
class Node;
class QuantaCorePart;
class TagPair;

class QTextCodec;
class QTimer;
class KParts::Part;

namespace KTextEditor
{
  class CodeCompletionInterface;
  class CompletionItem;
  class Document;
  class View;
}

/**
This class handles the events belonging to an opened document and stores information about the document itself. It provides a way to interact with the edited document.

@author Andras Mantia
*/
class QuantaDoc : public EditorSource
{
Q_OBJECT
public:
  QuantaDoc(KTextEditor::Document *document, QuantaCorePart *qcore);

  ~QuantaDoc();
  /** Code completion was requested by the user. */
  void codeCompletionRequested();
  /** Bring up the code completion tooltip. */
  void codeCompletionHintRequested();
  const DTDStruct * mainDTEP() {return m_dtd;}
  void setMainDTEP(const DTDStruct* dtd) {m_dtd = dtd;}
  /**
   * Inserts a tag/closing tag pair, living the cursors between them. If there is a 
   * selected area in the document, the tag/closing tag will surround it.
   * @param tagPair pair of full opening and closing strings
   */
  void insertTag(const TagPair & tagPair);

  void parse();
  
  ParseResult * parseResult() {return &m_parseResult;};
  
  /**
   * compares a KPart with the internal Document
   * @param part 
   * @return true if the part is the internal Document
   */
  bool isDocument(KParts::Part *part);
  
private slots:
  /** Called whenever the text in the document is changed. */
  void slotTextChanged(KTextEditor::Document* document);
  /** Handle the text changed events. Usually called from slotTextChanged,
  but it's possible to force the handling by calling manually and setting
  forced to true. 
   * 
   * @param forced true if the event must be handled in any case
   */
  void slotDelayedTextChanged(bool forced = false);
  /**
   * Called when the cursor moves inside the document.
   */
  void slotCursorPositionChanged(KTextEditor::View *view, const KTextEditor::Cursor &newPosition);
  /**
   * Request showing the completion box in a delayed way.
   */
  void slotDelayedShowCodeCompletion();
  /**
   * Request the manual completion in a delayed way.
   */
  void slotDelayedManualCodeCompletion();
  /**
   * Request the completion hint in a delayed way.
   */
  void slotDelayedCodeCompletionHint();
  /**
   * A slot to used a delayed replacement of accented characters.
   */
  void slotDelayedReplaceChar();
  /** Called when a user selects a completion, we then can modify it */
  void slotFilterCompletion(KTextEditor::CompletionItem*,QString *);
  /** Called after a completion is inserted */
  void slotCompletionDone( KTextEditor::CompletionItem completion );
  /** Called when the code completion is aborted.*/
  void slotCompletionAborted();
  /**
   * Called when a character was interactively inserted in the document (eg. typed in).
   * @param line the line where the character was inserted
   * @param column the column where the character was inserted
   * @param string the inserted text
   */
  void slotCharactersInserted(KTextEditor::View *view, const KTextEditor::Cursor &cursor, const QString &string);

  /**
   * calls emitNewCursorPosition in @see QuantaCorePart with the current
   * cursor position
   */
  void slotIdleTimerExpired();

private:
  /** Try to identify the DTD of the source the parser works on.
    @return The area of the tag defining the DTD, empty if not found

   */
  AreaStruct parseForDTD();
  /**
   * Gets the active DTEP for a position 
   * @param line the line to check
   * @param col the column to check
   * @return the DTEP if found, the main DTEP if not found
   */
  const DTDStruct *dtdAt(int line, int col) const;
  /**
   * (Re)starts the idle timer.
   */
  void startIdleTimer();
  
  /** Recursively insert the mandatory childs of tag. 
   * 
   * @param tag insert the children for this tag
   * @param lastTag do not insert more tags if the current tag is this one. Used internally to avoid infinite recursion.
   */
  void insertChildTags(QTag *tag, QTag* lastTag = 0L);

  /** Brings up list of code completions 
   * 
   * @param completions the list of the completion entries
   */
  void showCodeCompletions( QList<KTextEditor::CompletionItem> *completions );

  /**
   * Replace a char with it's corresponding unicode entity, if it cannot be encoded
   * in the encoding of the document.
   * @param position the position of the character
   * @param ch the character itself
   */
  void replaceAccentedChars(const QPoint& position, const QChar& ch);
  
  /**
   * search backwards and forewards to find the boundaries of a tag
   * @param position current cursor position
   * @return the area of the found tag
   */
  AreaStruct findTagArea(const QPoint& position) {return AreaStruct(findTagBegin(position), findTagEnd(position));};

  /**
   * @param position the current cursor position
   * @return the begin of the tag or the begin of the document, if no begin was found
   */
  QPoint findTagBegin(const QPoint& position);
  
  /**
   * @param position the current cursor position
   * @return the end of the tag or the end of the document, if no end was found
   */
  QPoint findTagEnd(const QPoint& position);

  /**
   * Appends the closing pair for the tag that is in node if needed.
   * @param node the node
   */
  void closeTag(Node *node);

  const DTDStruct *m_dtd; ///<The main DTEP object of the document
  ParseResult m_parseResult; ///< a structure holding the base node of the node tree associated with this document as well as some additional information
  
  /** the timer used to keep track of idle periods
   *  every keystroke restarts this timer
   *  @code m_idleTimer->isActive() @endcode
   *  returns true if there is typing in progress
   */
  QTimer *m_idleTimer;
  bool m_parsingNeeded; ///< true if the document and the node tree is not yet in synch and parsing is needed before reading something from the node tree
  bool m_parserEnabled; ///< true if parsing is enabled

  KTextEditor::CodeCompletionInterface *m_codeCompletionIf;

  QString m_encoding; ///<the last known encoding of the document
  QTextCodec *m_codec; ///<the last known codec for the document


  const DTDStruct *m_completionDTD; ///<The DTEP used for code completion
  bool m_completionInProgress; ///< true if there is a completion box visible. TODO: Maybe we can get rid of it when Kate doesn't emit completionAborted before completionDone

  QuantaCorePart *m_qcore;
  CompletionBase *m_completion;

//TODO: Get rid of this members used to simulate the delayed calling of slots with method. Qt4 supports delayed emitting of signals.
  QPoint m_replaceCharsPosition;
  QString m_replaceCharsString;
 
  //stores the data after an autocompletion. Used when bringing up the
  //autocompletion box delayed with the singleshot timer (workaround for
  //a bug: the box is not showing up if it is called from slotCompletionDone)
  int m_lastLine, m_lastCol;

  QList<KTextEditor::CompletionItem> *m_completionEntries;
};

#endif
