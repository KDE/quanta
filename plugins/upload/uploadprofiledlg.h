/***************************************************************************
*   Copyright 2007 Niko Sams <niko.sams@gmail.com>                        *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#ifndef UPLOADPROFILEDLG_H
#define UPLOADPROFILEDLG_H

#include <QDialog>

class KUrl;
class QListWidgetItem;
namespace Ui {
    class UploadProfileDlg;
}
class UploadProfileItem;

/**
 * Dialog to edit a upload profile
 */
class UploadProfileDlg : public QDialog
{
    Q_OBJECT
public:
    UploadProfileDlg( QWidget *parent = 0 );
    virtual ~UploadProfileDlg();

public Q_SLOTS:
    /**
     * Opens the Dialog to edit an upload profile
     * @return dialog's result code, Accepted or Rejected.
     */
    int editProfile(UploadProfileItem* item);

private:
    Ui::UploadProfileDlg* m_ui;
};


#endif
// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
