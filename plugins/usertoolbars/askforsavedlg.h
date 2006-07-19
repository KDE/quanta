/***************************************************************************
    begin                : Thu May 19 2005
    copyright            : (C) 2005 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/
#ifndef ASKFORSAVEDLG_H
#define ASKFORSAVEDLG_H

#include <kdialog.h>

/**
Ask a save question with 4 buttons: Save, Save As, No, Cancel.

@author Andras Mantia
*/
class AskForSaveDlg : public KDialog
{
Q_OBJECT
public:
    /** Constructor for the dialog.
    * @param caption the title of the dialog
    * @param question the question text (should be about saving)
    * @param widget parent widget
    * @param name object name
    */
    AskForSaveDlg(const QString &caption, const QString &question, QWidget *parent = 0);
    ~AskForSaveDlg();
    /**
    * @return The status of the dialog (which button was pressed)
    *         -1: Cancel, 1: Save, 2: Save As, 3:Discard (don't save)
    */
    int status() {return m_status;}

protected slots:
    virtual void slotUser1();
    virtual void slotUser2();
    virtual void slotUser3();

private:
    int m_status;
};

#endif
