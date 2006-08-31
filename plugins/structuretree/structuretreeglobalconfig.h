/***************************************************************************
 *   Copyright (C) 2005 by Jens Herden                                     *
 *   jens@kdewebdev.org                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef STRUCTURETREE_GLOBAL_CONFIG_H
#define STRUCTURETREE_GLOBAL_CONFIG_H

#include "ui_structuretreeglobalconfigbase.h"


class StructureTreePart;
class KConfigDialogManager;

class StructureTreeGlobalConfig : public QWidget, public Ui::StructureTreeGlobalConfigBase
{
  Q_OBJECT
public:
  explicit StructureTreeGlobalConfig(StructureTreePart *part, QWidget *parent = 0);

  ~StructureTreeGlobalConfig();

signals:
  /**
   * emitted when new configuration was accepted
   */
  void accepted();

public slots:
  void accept();

private:
  StructureTreePart *m_part;
  KConfigDialogManager * manager;
};

#endif

// kate: space-indent on; indent-width 2; mixedindent off; indent-mode cstyle;
