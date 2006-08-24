/***************************************************************************
    begin                : Sun Jan 22 2006
    copyright            : (C) 2006 by Jens Herden <jens@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/

#ifndef COMPARATOR_H
#define COMPARATOR_H

#include <QString>


class ParserStatus;

/**
 * \short This class holds the static functions that are use for comparision in a parse state.
 * 
 * Use the factory to get a pointer for a functions name.
 * 
 * \author Jens Herden   \<jens@kdewebdev.org\>
 **/

class Comparator
{
public:
  typedef bool (*CompareFunctPtr) (const ParserStatus &parser, const QString &);

  static CompareFunctPtr factory (const QString &name);

  /**
   * @param parser unused
   * @param argument unused
   * @return true
   */
  static bool always(const ParserStatus &parser, const QString &argument);

  /**
   * @param parser the ParserStatus to use
   * @param argument 
   * @return true if currChar == argument[0]
   */
  static bool equal(const ParserStatus &parser, const QString &argument);

  /**
   * @param parser the ParserStatus to use
   * @param argument unused
   * @return \e true if parser.currChar is whitespace
   */
  static bool whitespace(const ParserStatus &parser, const QString &argument);

  /**
   * fallback if factory got wrong name
   * @param parser unused
   * @param argument unused
   * @return \e false
   */
  static bool never(const ParserStatus &parser, const QString &argument);

  /**
   * @param parser the ParserStatus to use
   * @param argument unused
   * @return \e true if parser.currChar is a digit
   */
  static bool digit(const ParserStatus &parser, const QString &argument);

  /**
   * @param parser the ParserStatus to use
   * @param argument unused
   * @return \e true if parser.currChar is a hex number
   */
  static bool hex(const ParserStatus &parser, const QString &argument);
  
  /**
   * @param parser the ParserStatus to use
   * @param argument unused
   * @return \e true if parser.currChar is in [a-z] or [A-Z]
   */
  static bool asciiChar(const ParserStatus &parser, const QString &argument);
  
  /**
   * @param parser the ParserStatus to use
   * @param argument string to lookup in
   * @return \e true argument contains parser.currChar
   */
  static bool contains(const ParserStatus &parser, const QString &argument);

};


#endif

// kate: space-indent on; indent-width 2; mixedindent off; indent-mode cstyle;
