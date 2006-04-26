/***************************************************************************
    begin                : Wed Jul 13 2005
    copyright            : (C) 2005 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/
#ifndef XMLATTRIBUTEVALUECOMPLETION_H
#define XMLATTRIBUTEVALUECOMPLETION_H

#include <completionbase.h>

/**
Handle completion of XML tag attribute values.

@author Andras Mantia
*/
class XmlAttributeValueCompletion : public CompletionBase
{
public:
   XmlAttributeValueCompletion();
   ~XmlAttributeValueCompletion();
  /**
   * Returns a list with the completion entries. Must be reimplemented.
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

  void setTagName(const QString& name) {m_tagName = name;}
  void setAttributeName(const QString& name) {m_attributeName= name;}

private:
  /**
   * Get a list with the possible values for an attribute of a tag.
   * @param dtd the DTEP we are working on
   * @param tag the tag name
   * @param attribute the attribute name
   * @return a list with the possible values
   */
  QStringList tagAttributeValues(const DTDStruct* dtd, const QString& tag, const QString &attribute);


  QString m_tagName;
  QString m_attributeName;  

};

#endif
