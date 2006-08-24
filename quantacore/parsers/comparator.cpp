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

#include "comparator.h"
#include "parserstatus.h"


Comparator::CompareFunctPtr Comparator::factory(const QString &name)
{
  QString id = name.toLower();
  if (id == "isanychar") return &always;
  if (id == "equals") return &equal;
  if (id == "iswhitespace") return &whitespace;
  if (id == "ischaracter") return &asciiChar;

  return &never; // in case name is wrong
}


bool Comparator::always(const ParserStatus &parser, const QString &argument)
{
  Q_UNUSED(parser);
  Q_UNUSED(argument);
  return true;
}


bool Comparator::never(const ParserStatus &parser, const QString &argument)
{
  Q_UNUSED(parser);
  Q_UNUSED(argument);
  Q_ASSERT(false);
  return false;
}


bool Comparator::equal(const ParserStatus &parser, const QString &argument)
{
  if (argument.isEmpty())
    return false;

  return parser.currChar == argument[0];
}


bool Comparator::whitespace(const ParserStatus &parser, const QString &argument)
{
  Q_UNUSED(argument);
  return parser.currChar.isSpace();
}


bool Comparator::digit(const ParserStatus &parser, const QString &argument)
{
  Q_UNUSED(argument);
  return parser.currChar.isDigit();
}


bool Comparator::hex(const ParserStatus &parser, const QString &argument)
{
  Q_UNUSED(argument);
  return parser.currChar.isDigit() || (parser.currChar >= 'a' && parser.currChar <= 'f')
      || (parser.currChar >= 'A' && parser.currChar <= 'F');
}


bool Comparator::asciiChar(const ParserStatus &parser, const QString &argument)
{
  Q_UNUSED(argument);
  return (parser.currChar >= 'a' && parser.currChar <= 'z')
      || (parser.currChar >= 'A' && parser.currChar <= 'Z');
}


bool Comparator::contains(const ParserStatus &parser, const QString &argument)
{
  return argument.contains(parser.currChar);
}


// kate: space-indent on; indent-width 2; mixedindent off; indent-mode cstyle;
