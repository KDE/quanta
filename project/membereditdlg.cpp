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
#include <kpushbutton.h>

//app includes
#include "membereditdlg.h"
#include "subprojecteditdlgs.h"
#include "project.h"

MemberEditDlg::MemberEditDlg(QWidget *parent, const char *name)
 : MemberEditDlgS(parent, name)
{
}


MemberEditDlg::~MemberEditDlg()
{
}

void MemberEditDlg::slotRoleSelected(const QString &roleName)
{
   if (roleName != i18n("Subproject Leader"))
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

void MemberEditDlg::slotSelectMember()
{
  QMap<QString, TeamMember> members = Project::ref()->allMembers();
  QStringList items;
  for (QMap<QString, TeamMember>::ConstIterator it = members.constBegin(); it != members.constEnd(); ++it)
  {
     items.append(it.key());
  }
  bool ok;
  QString name = KInputDialog::getItem(i18n("Select Member"), i18n("Name:"), items, 0, false, &ok);
  if (ok)
  {
     nameEdit->setText(name);
     emailEdit->setText(members[name].email);
  }
}

#include "membereditdlg.moc"
