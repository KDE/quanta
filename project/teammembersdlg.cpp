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

extern QString simpleMemberStr;
extern QString taskLeaderStr;
extern QString teamLeaderStr;
extern QString subprojectLeaderStr;

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
   bool result;
   do {
     result = addDlg.exec();
     if (result)
     {
        QString name = memberDlg.nameCombo->currentText();
        QString role = memberDlg.roleCombo->currentText();
        QString task = memberDlg.taskEdit->text();
        QString subProject = memberDlg.subprojectCombo->currentText();
        if (name.isEmpty())
        {
           KMessageBox::error(this, i18n("The member name cannot be empty."));
           addDlg.show();
        } else
        if (!checkDuplicates(role, task, subProject))
        {
          addDlg.show();
        } else
        {
          QListViewItem *item = new QListViewItem(membersListView, name, memberDlg.emailEdit->text(), role, task);
          if (memberDlg.subprojectCombo->isEnabled())
            item->setText(4, subProject);
          result = false;
        }
     }
   } while (result);
}

void TeamMembersDlg::slotEditMember()
{
   QListViewItem *item =membersListView->currentItem();
   if (!item) return;
   KDialogBase editDlg(this, "edit_member", true, i18n("Edit Member"), KDialogBase::Ok | KDialogBase::Cancel);
   MemberEditDlg memberDlg(&editDlg);
   memberDlg.selectMember(item->text(0));
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
   QValueList<SubProject> *subprojects = Project::ref()->subprojects();
   for (QValueList<SubProject>::ConstIterator it = subprojects->constBegin(); it != subprojects->constEnd(); ++it)
   {
      if (item->text(4) == (*it).name)
      {
        subprojectIdx = idx;
        break;
      }
      idx++;
   }
   memberDlg.subprojectCombo->setCurrentItem(subprojectIdx);

   editDlg.setMainWidget(&memberDlg);
   bool result;
   do {
     result = editDlg.exec();
     if (result)
     {
        QString name = memberDlg.nameCombo->currentText();
        QString role = memberDlg.roleCombo->currentText();
        QString task = memberDlg.taskEdit->text();
        QString subProject = memberDlg.subprojectCombo->currentText();
        if (name.isEmpty())
        {
           KMessageBox::error(this, i18n("The member name cannot be empty."));
           editDlg.show();
        } else
        if (!checkDuplicates(role, task, subProject))
        {
          editDlg.show();
        } else
        {
          item->setText(0, name);
          item->setText(1, memberDlg.emailEdit->text());
          item->setText(2, role);
          item->setText(3, task);
          if (memberDlg.subprojectCombo->isEnabled())
            item->setText(4, subProject);
          result = false;
        }
     }
   } while (result);
}

void TeamMembersDlg::slotDeleteMember()
{
   QListViewItem *item =membersListView->currentItem();
   if (!item) return;
   if (KMessageBox::warningContinueCancel(this, i18n("<qt>Are you sure that you want to remove <b>%1</b> from the project team?</qt>").arg(item->text(0)), i18n("Delete Member"),KStdGuiItem::del()) == KMessageBox::Continue)
   {
      delete item;
   }
}

bool TeamMembersDlg::checkDuplicates(const QString &role, const QString &task, const QString &subProject)
{
    if (role == i18n(simpleMemberStr.utf8()))
      return true;
    QString s;
    QListViewItemIterator it(membersListView);
    while ( it.current() )
    {
        s = it.current()->text(2);
        if  (s == role &&
             (role == i18n(teamLeaderStr.utf8()) ||
              (role == i18n(taskLeaderStr.utf8()) && it.current()->text(3) == task) ||
              (role == i18n(subprojectLeaderStr.utf8()) && it.current()->text(4) == subProject)
             ) )
        {
            if (KMessageBox::warningYesNo(this, i18n("<qt>The <b>%1</b> role is already assigned to <b>%2</b>. Do you want to reassign it to the current member?</qt>").arg(role).arg(it.current()->text(0)), QString::null, i18n("Reassign"), i18n("Do Not Reassign")) == KMessageBox::Yes)
            {
              it.current()->setText(2, i18n(simpleMemberStr.utf8()));
              return true;
            }
            else
              return false;
        }
        ++it;
    }
    return true;
}

#include "teammembersdlg.moc"

