/***************************************************************************
    begin                : Mon Jul 11 2005
    copyright            : (C) 2005 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/
#ifndef XMLTAGCOMPLETION_H
#define XMLTAGCOMPLETION_H

#include <completionbase.h>

/**
Handles the tag completion in XML DTEPs.

@author Andras Mantia
*/
class XmlTagCompletion : public CompletionBase
{
public:
  XmlTagCompletion();
  ~XmlTagCompletion();

  /**
   * Returns a list with the completion entries.
   * 
   * @return a list with the entries that should appear in the completion box
   */
  virtual QList<KTextEditor::CompletionItem>* completionEntries();
 
/**
   * Checks if the completion object is valid for the current position and completion mode.
   * @param mode The completion mode
   * @return true if valid, false if not
   */
  virtual bool isValid(CompletionMode mode);

};

#endif
