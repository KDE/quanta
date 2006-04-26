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

#include <qstring.h>


class Comparator
{
public:
  typedef bool (*CompareFunct) (const QChar &, const QString &);

  static CompareFunct factory (int id);

  /**
   * id=0 always true
   * @param c unused
   * @param compare unused
   * @return true
   */
  static bool always(const QChar &c, const QString &compare);

  /**
   * id=1 test if character is equal
   * @param c
   * @param compare
   * @return true if c == compare[0]
   */
  static bool equal(const QChar &c, const QString &compare);

  /**
   * id=2 check for whitespace
   * @param c
   * @param compare unused
   * @return true if c is whitespace character
   */
  static bool whitespace(const QChar &c, const QString &compare);

  /**
   * fallback if factory got wrong id
   * @param c unused
   * @param compare unused
   * @return false
   */
  static bool never(const QChar &c, const QString &compare);

};


#endif

// kate: space-indent on; indent-width 2; mixedindent off; indent-mode cstyle;
