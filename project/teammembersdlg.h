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

public slots:
    void slotAddMember();
    void slotEditMember();
    void slotDeleteMember();
};

#endif
