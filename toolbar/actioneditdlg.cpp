/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

 /***************************************************************************
    copyright            : (C) 2001 by Dmitry Poplavsky 
    email                : dima@kde.org
 ***************************************************************************/


#include "actioneditdlg.h"
#include <kaction.h>
#include <kiconloader.h>
#include <kicondialog.h>

ActionEditDlg::ActionEditDlg( KActionCollection *col, QWidget* parent, const char* name, bool modal, WFlags fl )
    :ActionEditDlgS( parent, name, modal, fl )
{
   actionIcon->setIconType(KIcon::User,0,true);
   
   QValueList<KAction*> actions = col->actions();
   
   QValueList<KAction*>::Iterator it;
   for( it = actions.begin(); it != actions.end(); ++it )
        actionsList->insertItem( UserIcon((*it)->icon()), (*it)->plainText() );

}

ActionEditDlg::~ActionEditDlg()
{
}



void ActionEditDlg::deleteAction()
{
    
}

void ActionEditDlg::newAction()
{
    
}

void ActionEditDlg::actionSelected()
{
    
}


