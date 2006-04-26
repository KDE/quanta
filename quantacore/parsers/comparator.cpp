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

Comparator::CompareFunct Comparator::factory(int id)
{
  switch( id )
  {
    case 0: return &always;
    case 1: return &equal;
    case 2: return &whitespace;
  }
  return &never; // in case id is wrong
}


bool Comparator::always(const QChar &c, const QString &compare)
{
  Q_UNUSED(c);
  Q_UNUSED(compare);
  return true;
}


bool Comparator::never(const QChar &c, const QString &compare)
{
  Q_UNUSED(c);
  Q_UNUSED(compare);
  return false;
}


bool Comparator::equal(const QChar &c, const QString &compare)
{
  if (compare.isEmpty())
    return false;

  return c == compare[0];
}


bool Comparator::whitespace(const QChar &c, const QString &compare)
{
  Q_UNUSED(compare);
  return c.isSpace();
}

// kate: space-indent on; indent-width 2; mixedindent off; indent-mode cstyle;
