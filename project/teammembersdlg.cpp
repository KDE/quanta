/***************************************************************************
                          teammembersdlg.cpp  -  description
                             -------------------
    begin                : Wed Jul 7 2004
    copyright          : (C) 2004 Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
//qt includes

//kde includes
#include <kcombobox.h>
#include <kdialogbase.h>
#include <klineedit.h>
#include <klistview.h>
#include <klocale.h>
#include <kmessagebox.h>

//app includes
#include "teammembersdlg.h"
#include "membereditdlg.h"
#include "project.h"

TeamMembersDlg::TeamMembersDlg(QWidget *parent, const char *name)
 : TeamMembersDlgS(parent, name)
{
}


TeamMembersDlg::~TeamMembersDlg()
{
}

void TeamMembersDlg::slotAddMember()
{
   KDialogBase addDlg(this, "add_member", true, i18n("New Member"), KDialogBase::Ok | KDialogBase::Cancel);
   MemberEditDlg memberDlg(&addDlg);
   addDlg.setMainWidget(&memberDlg);
   if (addDlg.exec())
   {
      QListViewItem *item = new QListViewItem(membersListView, memberDlg.nameEdit->text(), memberDlg.emailEdit->text(), memberDlg.roleCombo->currentText(), memberDlg.taskEdit->text());
      membersListView->insertItem(item);
   }
}

void TeamMembersDlg::slotEditMember()
{
   QListViewItem *item =membersListView->currentItem();
   if (!item) return;
   KDialogBase editDlg(this, "edit_member", true, i18n("Edit Member"), KDialogBase::Ok | KDialogBase::Cancel);
   MemberEditDlg memberDlg(&editDlg);
   memberDlg.nameEdit->setText(item->text(0));
   memberDlg.emailEdit->setText(item->text(1));
   QString role = item->text(2);
   for (int i = 0; i < memberDlg.roleCombo->count(); i++)
   {
      if (memberDlg.roleCombo->text(i) == role)
      {
         memberDlg.roleCombo->setCurrentItem(i);
         memberDlg.slotRoleSelected(role);
         break;
      }
   }
   memberDlg.taskEdit->setText(item->text(3));
   int idx = 0;
   int subprojectIdx = 0;
   QStringList subprojectList;
   QValueList<SubProject> *subprojects = Project::ref()->subprojects();
   for (QValueList<SubProject>::ConstIterator it = subprojects->constBegin(); it != subprojects->constEnd(); ++it)
   {
      subprojectList.append((*it).name);
      if (item->text(4) == (*it).name)
        subprojectIdx = idx;
      idx++;
   }
   memberDlg.subprojectCombo->insertStringList(subprojectList);
   memberDlg.subprojectCombo->setCurrentItem(subprojectIdx);

   editDlg.setMainWidget(&memberDlg);
   if (editDlg.exec())
   {
      item->setText(0, memberDlg.nameEdit->text());
      item->setText(1, memberDlg.emailEdit->text());
      item->setText(2, memberDlg.roleCombo->currentText());
      item->setText(3, memberDlg.taskEdit->text());
      if (memberDlg.subprojectCombo->isEnabled())
        item->setText(4, memberDlg.subprojectCombo->currentText());
   }
}

void TeamMembersDlg::slotDeleteMember()
{
   QListViewItem *item =membersListView->currentItem();
   if (!item) return;
   if (KMessageBox::questionYesNo(this, i18n("<qt>Are you sure that you want to remove <b>%1</b> from the project team?</qt>").arg(item->text(0)), i18n("Delete Member")) == KMessageBox::Yes)
   {
      delete item;
   }
}

#include "teammembersdlg.moc"

