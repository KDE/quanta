/***************************************************************************
                          teammembersdlg.h  -  description
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

#ifndef TEAMMEMBERSDLG_H
#define TEAMMEMBERSDLG_H

#include "teammembersdlgs.h"

/**
@author Andras Mantia
*/
class TeamMembersDlg : public TeamMembersDlgS
{
Q_OBJECT
public:
    TeamMembersDlg(QWidget *parent = 0, const char *name = 0);

    ~TeamMembersDlg();
    void setYourself(const QString &name);
    QString yourself() {return m_yourself;}

public slots:
    void slotAddMember();
    void slotEditMember();
    void slotDeleteMember();
    void slotSetToYourself();

private:
   /** Check if the selected role conflicts with an already existing role or not, for
   example a Team Leader is selected, while a team leader already exists. In case
   of  duplicate entries, it shows a dialog and if the user answer yes, the old
   teamleader is set to Simple Member and the new member is set to teamleader
   and the method returns true, otherwise it returns false. */
    bool checkDuplicates(QListViewItem *item, const QString &name, const QString &nickName, const QString &email, const QString &role, const QString &task, const QString &subProject);
    
  QString m_yourself;  
};

#endif
