/***************************************************************************
    copyright            : (C) 2003 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/

#ifndef __ACTIONCONFIGDIALOG_INTERFACE_H
#define __ACTIONCONFIGDIALOG_INTERFACE_H

class ActionConfigDialogInterface
{
public:
    ActionConfigDialogInterface() {}
    virtual ~ActionConfigDialogInterface() {}

    virtual void accept() = 0;
    virtual void reject() = 0;

private:
    ActionConfigDialogInterface( const ActionConfigDialogInterface& source );
    void operator = ( const ActionConfigDialogInterface& source );
};


#endif // __ACTIONCONFIGDIALOG_INTERFACE_H
