/***************************************************************************
    begin                : Wed Apr 19 2006
    copyright            : (C) 2006 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/
#ifndef FILEINFODLG2_H
#define FILEINFODLG2_H

#include "ui_fileinfodlg.h"
#include "qwidget.h"

class FileInfoDlg: public QWidget, public Ui_FileInfoDlg
{
  Q_OBJECT

public:
  FileInfoDlg(QWidget *parent = 0L);

};

#endif
