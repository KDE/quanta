/***************************************************************************
    begin                : Tue Jul 12 2005
    copyright            : (C) 2005 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/
#ifndef XMLATTRIBUTECOMPLETION_H
#define XMLATTRIBUTECOMPLETION_H

#include <completionbase.h>

/**
Handles code completion for XML tag attributes.

@author Andras Mantia
*/
class XmlAttributeCompletion : public CompletionBase
{
public:
  XmlAttributeCompletion();
  ~XmlAttributeCompletion();

  /**
   * Returns a list with the completion entries.
   * 
   * @return a list with the entries that should appear in the completion box
   */
  virtual QList<KTextEditor::CompletionItem>* completionEntries();
  /**
   * Called when a completion item was chosen from the list.
   * It inserts =" after the attribute.
   * @param completion the chosen completion item
   * @return true as manual completion for attribute values should be invoked after the completion is done
  */
  virtual CompletionRequest completionDone(const KTextEditor::CompletionItem& completion);

  /**
   * Checks if the completion object is valid for the current position and completion mode.
   * @param mode The completion mode
   * @return true if valid, false if not
   */
  virtual bool isValid(CompletionMode mode);
};

#endif
