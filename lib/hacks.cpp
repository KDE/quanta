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

#include "hacks.h"

#include <qaction.h>
#include <qfont.h>

#include <kmenu.h>

namespace Hack{

void KMenuAddTitle(KMenu *menu, const QString & title)
{
  QAction *a = menu->addAction(title);
  QFont font = a->font();
  font.setBold(true);
  a->setFont(font);
  a->setEnabled(false);
  menu->addSeparator();

}

}
