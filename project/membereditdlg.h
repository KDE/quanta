/***************************************************************************
                          membereditdlg.h  -  description
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

#ifndef MEMBEREDITDLG_H
#define MEMBEREDITDLG_H

#include <membereditdlgs.h>

/**
@author Andras Mantia
*/
class MemberEditDlg : public MemberEditDlgS
{
Q_OBJECT
public:
    MemberEditDlg(QWidget *parent = 0, const char *name = 0);
    ~MemberEditDlg();
    void selectMember(const QString &name);

public slots:
    void slotRoleSelected(const QString& roleName);
    void slotEditSubprojects();
    void slotMemberSelected();
    void slotSelectFromAddrBook();
};

#endif
