/***************************************************************************
    begin                : Sun Jul 17 2005
    copyright            : (C) 2005 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/
#ifndef SPECIALAREAATTRIBUTECOMPLETION_H
#define SPECIALAREAATTRIBUTECOMPLETION_H

#include <completionbase.h>

/**
@author Andras Mantia
*/
class SpecialAreaAttributeCompletion : public CompletionBase
{
public:
  SpecialAreaAttributeCompletion();

  ~SpecialAreaAttributeCompletion();

  /**
   * Returns a list with the completion entries.
   * 
   * @return a list with the entries that should appear in the completion box
   */
  virtual QList<KTextEditor::CompletionItem>* completionEntries();

  /**
   * Returns the completion hints. Should be reimplemented if hinting is possible for the completion object.
   * 
   * @return a structure with the list of items in the hint, wrapping and delimiter string
   */
  virtual HintResult completionHint();

 /**
   * Called when a completion item was chosen from the list, but before it was inserted into the document.
   * @param completion the chosen completion item
   * @param string the current selected string
   */
  virtual void filterCompletion(KTextEditor::CompletionItem *completion , QString *string);

  /**
   * Checks if the completion object is valid for the current position and completion mode. Must be reimplemented.
   * @param mode The completion mode
   * @return true if valid, false if not
   */
  virtual bool isValid(CompletionMode mode);

private:
  int m_attrAutoCompleteAfterCharPosition;
  QString m_tagStr;
};

#endif
