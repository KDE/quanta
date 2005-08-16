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
#include <qlabel.h>

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

#define NAME_COL 0
#define NICKNAME_COL 1
#define EMAIL_COL 2
#define ROLE_COL 3
#define TASK_COL 4
#define SUBPROJECT_COL 5


extern QString simpleMemberStr;
extern QString taskLeaderStr;
extern QString teamLeaderStr;
extern QString subprojectLeaderStr;

TeamMembersDlg::TeamMembersDlg(QWidget *parent, const char *name)
 : TeamMembersDlgS(parent, name)
{
  m_yourself = "";
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
        QString nickName = memberDlg.nicknameEdit->text();
        if (name.isEmpty())
        {
           KMessageBox::error(this, i18n("The member name cannot be empty."));
           addDlg.show();
        } else
        if (nickName.isEmpty())
        {
           KMessageBox::error(this, i18n("The nickname cannot be empty as it is used as an unique identifier."));
           addDlg.show();
        } else
        if (!checkDuplicates(name, nickName, role, task, subProject))
        {
          addDlg.show();
        } else
        {
          QListViewItem *item = new QListViewItem(membersListView, name, nickName, memberDlg.emailEdit->text(), role, task);
          if (memberDlg.subprojectCombo->isEnabled())
            item->setText(SUBPROJECT_COL, subProject);
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
   memberDlg.selectMember(item->text(NAME_COL));
   memberDlg.nicknameEdit->setText(item->text(NICKNAME_COL));
   memberDlg.emailEdit->setText(item->text(EMAIL_COL));
   QString role = item->text(ROLE_COL);
   for (int i = 0; i < memberDlg.roleCombo->count(); i++)
   {
      if (memberDlg.roleCombo->text(i) == role)
      {
         memberDlg.roleCombo->setCurrentItem(i);
         memberDlg.slotRoleSelected(role);
         break;
      }
   }
   memberDlg.taskEdit->setText(item->text(TASK_COL));
   int idx = 0;
   int subprojectIdx = 0;
   QValueList<SubProject> *subprojects = Project::ref()->subprojects();
   for (QValueList<SubProject>::ConstIterator it = subprojects->constBegin(); it != subprojects->constEnd(); ++it)
   {
      if (item->text(SUBPROJECT_COL) == (*it).name)
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
        QString nickName = memberDlg.nicknameEdit->text();
        QString role = memberDlg.roleCombo->currentText();
        QString task = memberDlg.taskEdit->text();
        QString subProject = memberDlg.subprojectCombo->currentText();
        if (name.isEmpty())
        {
           KMessageBox::error(this, i18n("The member name cannot be empty."));
           editDlg.show();
        } else
         if (nickName.isEmpty())
        {
           KMessageBox::error(this, i18n("The nickname cannot be empty as it is used as an unique identifier."));
           editDlg.show();
        } else
       if (!checkDuplicates(name, nickName, role, task, subProject))
        {
          editDlg.show();
        } else
        {
          item->setText(NAME_COL, name);
          item->setText(NICKNAME_COL, nickName);
          item->setText(EMAIL_COL, memberDlg.emailEdit->text());
          item->setText(ROLE_COL, role);
          item->setText(TASK_COL, task);
          if (memberDlg.subprojectCombo->isEnabled())
            item->setText(SUBPROJECT_COL, subProject);
          result = false;
        }
     }
   } while (result);
}

void TeamMembersDlg::slotDeleteMember()
{
   QListViewItem *item =membersListView->currentItem();
   if (!item) return;
   if (KMessageBox::warningContinueCancel(this, i18n("<qt>Are you sure that you want to remove <b>%1</b> from the project team?</qt>").arg(item->text(NAME_COL)), i18n("Delete Member"), KStdGuiItem::del()) == KMessageBox::Continue)
   {
      delete item;
   }
}

bool TeamMembersDlg::checkDuplicates(const QString &name, const QString &nickName, const QString &role, const QString &task, const QString &subProject)
{
    if (role == i18n(simpleMemberStr.utf8()))
      return true;
    QString s;
    QString nick;
    QListViewItemIterator it(membersListView);
    while ( it.current() )
    {
        s = it.current()->text(ROLE_COL);
        nick = it.current()->text(NICKNAME_COL);
        if  (s == role && it.current()->text(NAME_COL) != name &&
             (role == i18n(teamLeaderStr.utf8()) ||
              (role == i18n(taskLeaderStr.utf8()) && it.current()->text(TASK_COL) == task) ||
              (role == i18n(subprojectLeaderStr.utf8()) && it.current()->text(SUBPROJECT_COL) == subProject)
             ) )
        {
            if (KMessageBox::warningYesNo(this, i18n("<qt>The <b>%1</b> role is already assigned to <b>%2</b>. Do you want to reassign it to the current member?</qt>").arg(role).arg(it.current()->text(NAME_COL)), QString::null, i18n("Reassign"), i18n("Do Not Reassign")) == KMessageBox::Yes)
            {
              it.current()->setText(ROLE_COL, i18n(simpleMemberStr.utf8()));
              return true;
            }
            else
              return false;
        } else
        if (nick == nickName && it.current()->text(NAME_COL) != name)
        {
          KMessageBox::error(this, i18n("<qt>The <b>%1</b> nickname is already assigned to <b>%2</b>.</qt>").arg(nickName).arg(it.current()->text(NAME_COL)));
          return false;
        } 
        ++it;
    }
    return true;
}

void TeamMembersDlg::slotSetToYourself()
{
  QListViewItem *item =membersListView->currentItem();
  if (!item) return; 
  yourselfLabel->setText(item->text(0) + " <" + item->text(1) + ">");
  m_yourself = item->text(0);
}

void TeamMembersDlg::setYourself(const QString &name)
{
  m_yourself = name;
  QListViewItemIterator it(membersListView);
  while ( it.current() )
  {
    if (it.current()->text(0) == name)
    {
      yourselfLabel->setText(name + " <" + it.current()->text(1) + ">");     
      break;
    }
    ++it;
  }
}

#include "teammembersdlg.moc"

