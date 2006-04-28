//
// C++ Implementation: hacks
//
// Description: 
//
//
// Author: Andras Mantia <amantia@kde.org>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef HACKS_H
#define HACKS_H

class KMenu;
class QString;

/** A collection of hacks that should be removed as soon as the Qt/KDE4 methods
 * appear.
 */
namespace Hack {

  /** Somewhat simulates the old KPopupMenu::insertTitle method. Inserts a non-
   * selectable item and a separator in the menu.
   * @param menu the menu to insert the title in
   * @param title the title
   *
   * This method should be removed when proper headers/titles are supported in
   * QMenu/KMenu.
   */
  void KMenuAddTitle(KMenu *menu, const QString & title);
};


#endif

