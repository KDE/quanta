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
#ifndef PARSERMANAGER_H
#define PARSERMANAGER_H

#include "parser.h"

#include <QObject>

class Node;
class EditorSource;
class ParseResult;
struct DTDStruct;

/**
The manager of the parser(s). Right now it's only an interface to our parser object.
It is a singleton.

@author Andras Mantia
*/
class ParserManager : public QObject
{
Q_OBJECT
public:
  static ParserManager *self(QObject *parent = 0) {
    if (!m_ref) m_ref = new ParserManager(parent);
    return m_ref;
  }
  ~ParserManager();
  /**
   * Request a full parsing.
   * @param source the source to be parsed
   * @param base the base node of the current node tree with additional information. base.baseNode is 0L if no node tree exists for the current source.
   * @param dtd the main DTD of the source
   * @param detailed enable or disable detailed (special area) parsing
   */
  void parse(EditorSource *source, ParseResult *base, const DTDStruct *dtd, bool detailed);

  /**
   * Rebuilds the node tree for the source. Tries to parse as small amount as possible, but sometimes a full parsing cannot be avoided, so the whole node tree will be changed.
   * @param source the source to be parsed
   * @param base the base node of the current node tree with additional information. base.baseNode is 0L if no node tree exists for the current source.
   * @param dtd the main DTD of the source
   * @param detailed enable or disable detailed (special area) parsing
   */
  void rebuild(EditorSource *source, ParseResult *base, const DTDStruct *dtd, bool detailed);

  static ParserManager *m_ref;
signals:

  /**
   * emitted when the parser is about to start parsing
   *
   * @param source the source to parse
   */
  void startParsing(const EditorSource *source);

  /**
   * emitted when the parser has finished parsing
   *
   * @param source the parsed source
   * @param parseResult an object holding the result of the parsing
   */
  void finishedParsing(const EditorSource *source, const ParseResult *parseResult);

  /**
   * emitted when the parser has finished parsing some groups
   *
   * @param source the parsed source
   * @param parseResult an object holding the result of the parsing
   */
  void groupsParsed(const EditorSource *source, const ParseResult *parseResult);

private:
  ParserManager(QObject *parent = 0);
  Parser* m_parser;
};

#endif
