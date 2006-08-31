/***************************************************************************
 *   Copyright (C) 2005-2006 by Jens Herden                                *
 *   jens@kdewebdev.org                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QUANTAFILESTREE_GLOBAL_CONFIG_H
#define QUANTAFILESTREE_GLOBAL_CONFIG_H

#include "ui_quantafilestreeglobalconfigbase.h"

class QuantaFilesTreePart;
class KConfigDialogManager;

class QuantaFilesTreeGlobalConfig: public QWidget, public Ui::QuantaFilesTreeGlobalConfigBase
{
    Q_OBJECT
public:
    explicit QuantaFilesTreeGlobalConfig(QuantaFilesTreePart *part, QWidget *parent = 0);

    ~QuantaFilesTreeGlobalConfig();

signals:
  /**
   * emitted when new configuration was accepted
   */
  void accepted();

public slots:
    void accept();

private:
    QuantaFilesTreePart *m_part;

    KConfigDialogManager *manager;
};

#endif

// kate: space-indent on; indent-width 2; mixedindent off; indent-mode cstyle;
