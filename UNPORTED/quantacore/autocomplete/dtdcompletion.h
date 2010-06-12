/***************************************************************************
    begin                : Thu Jul 14 2005
    copyright            : (C) 2005 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/
#ifndef DTDCOMPLETION_H
#define DTDCOMPLETION_H

#include <completionbase.h>

/**
Completion class for the !DOCTYPE tags.

@author Andras Mantia
*/
class DTDCompletion : public CompletionBase
{
public:
  DTDCompletion();
  ~DTDCompletion();
  /**
   * Returns a list with the completion entries.
   * 
   * @return a list with the entries that should appear in the completion box
   */
  virtual QList<KTextEditor::CompletionItem>* completionEntries();
 /**
   * Called when a completion item was chosen from the list, but before it was inserted into the document.
   * Replaces the selected DTEP name with the full doctype line
   * @param completion the chosen completion item
   * @param string the current selected string
   */
  virtual void filterCompletion(KTextEditor::CompletionItem *completion , QString *string);
  
   /**
   * Checks if the completion object is valid for the current position and completion mode. 
   * @param mode The completion mode
   * @return true if valid, false if not
   */
  virtual bool isValid(CompletionMode mode);
};

#endif
