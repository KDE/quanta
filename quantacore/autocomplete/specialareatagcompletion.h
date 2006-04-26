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
#ifndef SPECIALAREATAGCOMPLETION_H
#define SPECIALAREATAGCOMPLETION_H

#include <completionbase.h>

/**
Handles code completion for special area tags (like PHP functions or CSS selectors)

@author Andras Mantia
*/
class SpecialAreaTagCompletion : public CompletionBase
{
public:
  SpecialAreaTagCompletion();
  ~SpecialAreaTagCompletion();

  /**
   * Returns a list with the completion entries.
   * 
   * @return a list with the entries that should appear in the completion box
   */
  virtual QList<KTextEditor::CompletionItem>* completionEntries();

 /**
   * Called when a completion item was chosen from the list, but before it was inserted into the document.
   * @param completion the chosen completion item
   * @param string the current selected string
   */
  virtual void filterCompletion(KTextEditor::CompletionItem *completion , QString *string);
 
 /**
   * Called when a completion item was chosen from the list.
   * @param completion the chosen completion item
   * @return true if manual completion should be invoked after the completion is done, false (the default) otherwise
   */
  virtual CompletionRequest completionDone(const KTextEditor::CompletionItem& completion);

  /**
   * Checks if the completion object is valid for the current position and completion mode. Must be reimplemented.
   * @param mode The completion mode
   * @return true if valid, false if not
   */
  virtual bool isValid(CompletionMode mode);

private:
  /**
   * Checks if a class is a derivate for another class.
   * @param className the class to check
   * @param baseClass the baseclass to check against
   * @return true if className extends baseClass
   */
  bool isDerivatedFrom(const QString& className, const QString &baseClass);

};

#endif
