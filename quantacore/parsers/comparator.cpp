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

//own includes
#include "comparator.h"
#include "parserstatus.h"

//kde includes
#include <kdebug.h>

#undef DEBUGMODE

#ifdef DEBUGMODE
#define COMPAREDEBUG( S )  kDebug(24001) << S << endl;
#else
#define COMPAREDEBUG( S )
#endif


Comparator::CompareFunctPtr Comparator::factory(const QString &name)
{
  QString id = name.toLower();
  if (id == "equals") return &equal;
  if (id == "isanychar") return &always;
  if (id == "containedin") return &containedIn;
  if (id == "iswhitespace") return &whitespace;
  if (id == "ischaracter") return &asciiChar;

  kWarning(24001) << "unknown function name '" << id << "' in Comparator::factory" << endl;
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
/*  Q_ASSERT(false);
  return false;*/
  kWarning(24001) << "Unknown comparator function called." << endl;
  return true;
}


bool Comparator::equal(const ParserStatus &parser, const QString &argument)
{
  if (argument.isEmpty())
    return false;

  return parser.m_currChar == argument.at(0);
}

bool Comparator::containedIn(const ParserStatus &parser, const QString &argument)
{
  if (argument.isEmpty())
    return false;

  return argument.contains(parser.m_currChar);
}

bool Comparator::whitespace(const ParserStatus &parser, const QString &argument)
{
  Q_UNUSED(argument);
  return parser.m_currChar.isSpace();
}


bool Comparator::digit(const ParserStatus &parser, const QString &argument)
{
  Q_UNUSED(argument);
  return parser.m_currChar.isDigit();
}


bool Comparator::hex(const ParserStatus &parser, const QString &argument)
{
  Q_UNUSED(argument);
  return parser.m_currChar.isDigit() || (parser.m_currChar >= 'a' && parser.m_currChar <= 'f')
      || (parser.m_currChar >= 'A' && parser.m_currChar <= 'F');
}


bool Comparator::asciiChar(const ParserStatus &parser, const QString &argument)
{
  Q_UNUSED(argument);
  bool result = (parser.m_currChar >= 'a' && parser.m_currChar <= 'z')
      || (parser.m_currChar >= 'A' && parser.m_currChar <= 'Z');

  COMPAREDEBUG( parser.m_currChar << " is a character: " << result )
  return result;
}


bool Comparator::contains(const ParserStatus &parser, const QString &argument)
{
  return argument.contains(parser.m_currChar);
}


// kate: space-indent on; indent-width 2; mixedindent off; indent-mode cstyle;
