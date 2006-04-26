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
#ifndef COMPLETIONBASE_H
#define COMPLETIONBASE_H

#include <qpoint.h>
#include <qstringlist.h>

class EditorSource;
class Node;
class ParseResult;
class XmlTagCompletion;
class XmlAttributeCompletion;
class XmlAttributeValueCompletion;
class XmlEntityCompletion;
class DTDCompletion;
class SpecialAreaAttributeCompletion;
class SpecialAreaGroupCompletion;
class SpecialAreaTagCompletion;
struct DTDStruct;

namespace KTextEditor
{
  class CompletionItem;
}
class KDevPlugin;

/**
Base class for the auto and manual completion handling classes.

@author Andras Mantia
*/
class CompletionBase{
public:
/** The type of the completion. Depending on the value, the corresponding completion object will be created. */
  enum CompletionType {
    XmlTag = 1, ///<complete XML tags
    XmlAttribute, ///< complete XML tag attributes
    XmlAttributeValue, ///< complete XML tag attribute values
    XmlEntity, ///< complete XML entities
    DTDDefinition, ///< completion for the !DOCTYPE line
    SpecialAreaTag, ///< complete special area tags (ie. CSS selectors, script functions, class members)
    SpecialAreaAttribute, ///< complete special area attributes (ie. CSS selector values, script function arguments)
    SpecialAreaGroups ///< completion for the special area groups
  };

/** The invocation mode of the completion. */
  enum CompletionMode {
    Manual = 1, ///< completion is invoked manually
    Auto, ///< completion appears automatically as the user types
    Forced ///< showing of the completion box is forced
  };

/** The possible actions after the completion is done.*/
  enum CompletionRequest {
    RequestNone = 0, ///< do not request anything
    RequestCompletion, ///< request a new manual completion
    RequestHint ///< request showing of the completion hints
  };

/** Used to get the data for completion hints.*/
  struct HintResult {
    QStringList argList;
    QString wrapper;
    QString delimiter;
  };

  CompletionBase();
  virtual ~CompletionBase();
  /**
    * Returns a completion class based on the context: the cursor position in the document.
    * @param base The parseresult associated with the document
    * @param source the document source
    * @param position the coordinates of the cursor
    * @param currentNode the node corresponding to the position in the document
    * @param type the type of the completion. See @ref CompletionType.
    * @param mode the invokation mode of the completion. See @ref CompletionMode.
    * @param plugin the class is used in this plugin
    * @return pointer to the completion object (of a ScriptCompletion or XmlCompletion class type). Null if the completion object cannot be used at the current position.
    */
  static CompletionBase* getCompletionObject(ParseResult *base, EditorSource *source, const QPoint& position, Node* currentNode, CompletionType type, CompletionMode mode, KDevPlugin *plugin);

  /**
   * Returns a list with the completion entries. Must be reimplemented.
   * 
   * @return a list with the entries that should appear in the completion box
   */
  virtual QList<KTextEditor::CompletionItem>* completionEntries() = 0;


  /**
   * Returns the completion hints. Should be reimplemented if hinting is possible for the completion object.
   * 
   * @return a structure with the list of items in the hint, wrapping and delimiter string
   */
  virtual HintResult completionHint() {return HintResult();}

 /**
   * Called when a completion item was chosen from the list, but before it was inserted into the document.
   * The default implementation removes the first m_starsWithLength chars.
   * @param completion the chosen completion item
   * @param string the current selected string
   */
  virtual void filterCompletion(KTextEditor::CompletionItem *completion , QString *string);

  /**
   * Called when a completion item was chosen from the list.
   * The default implementation does nothing.
   * @param completion the chosen completion item
   * @return true if manual completion should be invoked after the completion is done, false (the default) otherwise
   */
  virtual CompletionRequest completionDone(const KTextEditor::CompletionItem& completion) {Q_UNUSED(completion); return RequestNone;}

protected:
  /**
   * Initializes the completion object.
   * @param base the base of the node tree  
   * @param source the EditorSource of the current document
   * @param position the coordinates
   * @param node the node
   * @param plugin the class is used in this plugin
  */
  void init(ParseResult *base, EditorSource *source, const QPoint& position, Node *currentNode, KDevPlugin *plugin);

  /**
   * Checks if the completion object is valid for the current position and completion mode. Must be reimplemented.
   * @param mode The completion mode
   * @return true if valid, false if not
   */
  virtual bool isValid(CompletionMode mode) = 0;

  QString m_startsWith;
  ParseResult *m_base;
  Node *m_currentNode;
  EditorSource *m_source;
  QPoint m_position;
  QString m_lastInsertedString;
  const DTDStruct *m_dtd; ///<the DTD of the area where the completion happens
  KDevPlugin *m_plugin; ///<the plugin where this completion object is used

private:
  static XmlTagCompletion *m_xmlTagCompletion;
  static XmlAttributeCompletion *m_xmlAttributeCompletion;
  static XmlAttributeValueCompletion *m_xmlAttributeValueCompletion;
  static XmlEntityCompletion *m_xmlEntityCompletion;
  static DTDCompletion *m_dtdCompletion;
  static SpecialAreaAttributeCompletion *m_specialAreaAttributeCompletion;
  static SpecialAreaTagCompletion *m_specialAreaTagCompletion;
  static SpecialAreaGroupCompletion *m_specialAreaGroupCompletion;
};

#endif
