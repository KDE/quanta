/***************************************************************************
    begin                : Thu Jul 21 2005
    copyright            : (C) 2005 by Jens Herden <jens@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/
#ifndef FILTERABLEITEMIF_H
#define FILTERABLEITEMIF_H

#include <QString>

/**
@short a small interface to filter items
 
 this is designed to use the same searchline class with different treeviews
 
@author Jens Herden
*/

class FilterableItemIf
{
public:
  FilterableItemIf() {}

  virtual ~FilterableItemIf() {}
  
  /**
   * @param s the string to compare with
   * @return true if this item should be visible
   */
  virtual bool filter(const QString &s) const = 0;

};

#endif

// kate: space-indent on; indent-width 2; mixedindent off; indent-mode cstyle;
