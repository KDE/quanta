/***************************************************************************
    begin                : Fri Jul 15 2005
    copyright            : (C) 2005 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/
#ifndef SPECIALAREAGROUPCOMPLETION_H
#define SPECIALAREAGROUPCOMPLETION_H

#include <completionbase.h>

class StructTreeGroup;

/**
Completion object for special area (script) groups.

@author Andras Mantia
*/
class SpecialAreaGroupCompletion : public CompletionBase
{
public:
   SpecialAreaGroupCompletion();
   ~SpecialAreaGroupCompletion();

  /**
   * Returns a list with the completion entries.
   * 
   * @return a list with the entries that should appear in the completion box
   */
  virtual QList<KTextEditor::CompletionItem>* completionEntries();

  /**
   * Called when a completion item was chosen from the list.
   * Resets the m_group variable.
   * @param completion the chosen completion item
   * @return false
   */
  virtual CompletionRequest completionDone(const KTextEditor::CompletionItem& completion) {  Q_UNUSED(completion); m_group = 0L; return RequestNone;}

  /**
   * Sets the group for which the completion entries should be returned. 
   * Must be called before calling completionEntries() !
   * @param group 
   */
  void setGroup(StructTreeGroup const *group) {m_group = group;}

   /**
   * Checks if the completion object is valid for the current position and completion mode.
   * @param mode The completion mode
   * @return true if valid, false if not
   */
  virtual bool isValid(CompletionMode mode);

private:
  StructTreeGroup const *m_group;

};

#endif
