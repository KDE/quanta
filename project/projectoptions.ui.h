/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <kfiledialog.h>
#include <kdeversion.h>
#include <kmessagebox.h>
#include <klocale.h>

void ProjectOptions::buttonTmpl_clicked()
{
   KURL url = KFileDialog::getExistingURL(linePrjTmpl->text(), this,
                              i18n("Select Project Template Folder"));
   if (!url.isEmpty())
   {
     linePrjTmpl->setText(url.url());
   }
}



void ProjectOptions::buttonToolbar_clicked()
{
   KURL url = KFileDialog::getExistingURL(linePrjToolbar->text(), this,
                              i18n("Select Project Toolbar & Actions Folder"));
   if (!url.isEmpty())
   {
     linePrjToolbar->setText(url.url());
   }
}


void ProjectOptions::keepPasswdToggled( bool )
{
  if (keepPasswd->isChecked())
  {
      KMessageBox::information(this, i18n("<qt>Saving the password is not recommended. Please read the <b>What's This</b> information for this checkbox.</qt>"), i18n("Security Warning"), "SavePasswordWarning");
  }
}
