/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/

#include <kfiledialog.h>
#include <kdeversion.h>

void ProjectOptions::buttonTmpl_clicked()
{
   KURL url = KFileDialog::getExistingURL(linePrjTmpl->text(), this,
                              i18n("Select Project Template Directory"));
   if (!url.isEmpty())
   {
     linePrjTmpl->setText(url.url());
   }
}



void ProjectOptions::buttonToolbar_clicked()
{
   KURL url = KFileDialog::getExistingURL(linePrjToolbar->text(), this,
                              i18n("Select Project Toolbar & Actions Directory"));
   if (!url.isEmpty())
   {
     linePrjToolbar->setText(url.url());
   }
}
