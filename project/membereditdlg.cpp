/***************************************************************************
                          membereditdlg.cpp  -  description
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
#include <qcheckbox.h>

//kde includes
#include <kcombobox.h>
#include <kdialogbase.h>
#include <kinputdialog.h>
#include <klineedit.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kpushbutton.h>
#include <kabc/stdaddressbook.h>

//app includes
#include "listdlg.h"
#include "membereditdlg.h"
#include "subprojecteditdlgs.h"
#include "project.h"

QString simpleMemberStr = I18N_NOOP("Simple Member");
QString taskLeaderStr = I18N_NOOP("Task Leader");
QString teamLeaderStr = I18N_NOOP("Team Leader");
QString subprojectLeaderStr = I18N_NOOP("Subproject Leader");


MemberEditDlg::MemberEditDlg(QWidget *parent, const char *name)
 : MemberEditDlgS(parent, name)
{
   QValueList<SubProject> *subprojects = Project::ref()->subprojects();
   for (QValueList<SubProject>::ConstIterator it = subprojects->constBegin(); it != subprojects->constEnd(); ++it)
     subprojectCombo->insertItem((*it).name);
   subprojectCombo->setEnabled(false);
   editSubprojectsButton->setEnabled(false);

  QMap<QString, TeamMember> members = Project::ref()->allMembers();
  QStringList items;
  for (QMap<QString, TeamMember>::ConstIterator it = members.constBegin(); it != members.constEnd(); ++it)
  {
     nameCombo->insertItem(it.key());
  }
  nameCombo->insertItem(QString::null, 0);

  roleCombo->insertItem(i18n(teamLeaderStr.utf8()));
  roleCombo->insertItem(i18n(subprojectLeaderStr.utf8()));
  roleCombo->insertItem(i18n(taskLeaderStr.utf8()));
  roleCombo->insertItem(i18n(simpleMemberStr.utf8()));
  roleCombo->setCurrentItem(3);
}


MemberEditDlg::~MemberEditDlg()
{
}

void MemberEditDlg::slotRoleSelected(const QString &roleName)
{
   if (roleName != i18n(subprojectLeaderStr.utf8()))
   {
     subprojectCombo->setEnabled(false);
     editSubprojectsButton->setEnabled(false);
   } else
   {
     subprojectCombo->setEnabled(true);
     editSubprojectsButton->setEnabled(true);
   }
}

void MemberEditDlg::slotEditSubprojects()
{
   KDialogBase editDlg(this, "edit_subprojects", true, i18n("Edit Subprojects"), KDialogBase::Ok | KDialogBase::Cancel);
   SubprojectEditDlgS subprojectDlg(&editDlg);
   editDlg.setMainWidget(&subprojectDlg);
   QValueList<SubProject> *subprojects = Project::ref()->subprojects();
   int idx = 0;
   SubProject subProject;
   for (uint i = 0; i < subprojects->count(); i++)
   {
      subProject = (*subprojects)[i];
      if (subprojectCombo->currentText() == subProject.name)
      {
         subprojectDlg.nameEdit->setText(subProject.name);
         subprojectDlg.locationEdit->setText(subProject.location);
         idx = i;
         break;
      }
   }
   if (subprojects->count() == 0)
    subprojectDlg.createNew->setChecked(true);
   if (editDlg.exec())
   {
      QString oldName = subProject.name;
      subProject.name = subprojectDlg.nameEdit->text();
      subProject.location = subprojectDlg.locationEdit->text();
      if (subprojectDlg.createNew->isChecked() && (subProject.name != oldName))
      {
          subprojects->append(subProject);
          subprojectCombo->insertItem(subProject.name);
          subprojectCombo->setCurrentItem(subprojectCombo->count() - 1);
      } else
      {
         (*subprojects)[idx] = subProject;
         subprojectCombo->setCurrentText(subProject.name);
      }
   }
}

void MemberEditDlg::slotMemberSelected()
{
  QString name = nameCombo->currentText();
  QMap<QString, TeamMember> members = Project::ref()->allMembers();
  if (members.contains(name))
  {
     emailEdit->setText(members[name].email);
  }
}

void MemberEditDlg::selectMember(const QString &name)
{
  bool found = false;
  for (int i = 0; i < nameCombo->count(); i++)
  {
     if (nameCombo->text(i) == name)
     {
        nameCombo->setCurrentItem(i);
        found = true;
        break;
     }
  }
  if (!found)
  {
     nameCombo->insertItem(name, 1);
     nameCombo->setCurrentItem(1);
  }
}

void MemberEditDlg::slotSelectFromAddrBook()
{
  QMap<QString, QString> uidMap;
  QString name;
  QStringList result;
  KABC::AddressBook *addressBook = KABC::StdAddressBook::self();
  KABC::AddressBook::ConstIterator it;
  KABC::AddressBook::ConstIterator end = addressBook->end();
  for( it = addressBook->begin(); it != end; ++it )
  {
    result.append((*it).assembledName());
    uidMap.insert((*it).assembledName(), (*it).uid());
  }
 result.sort();

 if ( result.count() > 0 )
 {
   ListDlg listDlg(result);
   listDlg.setCaption(i18n("Select Member"));
   if (!listDlg.exec()) return;
   name = listDlg.getEntry();
   KABC::Addressee entry = addressBook->findByUid(uidMap[name]);
   nameCombo->insertItem(name, 1);
   nameCombo->setCurrentItem(1);
   emailEdit->setText(entry.emails()[0]);
   nicknameEdit->setText(entry.nickName());
 } else
 {
   KMessageBox::error(this,i18n("No entries found in the addressbook."));
 }
}

#include "membereditdlg.moc"
