/***************************************************************************
 *   Copyright (C) 2005 by Jens Herden   *
 *   jens@kdewebdev.org   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef KDEVTAGDIALOGS_H
#define KDEVTAGDIALOGS_H

#include "tagdialogsif.h"

class QuantaCoreIf;

class Context;
class QMenu;
class KAction;

/**
Please read the README.dox file for more info about this part
*/
class TagDialogsPart: public TagDialogsIf
{
  Q_OBJECT
public:
  TagDialogsPart(QObject *parent, const QStringList &args);
  ~TagDialogsPart();

  TagPair createNewTag(QTag *dtdTag, const QString& selection, const QString& attrs = QString::null, const KUrl& baseURL = KUrl());

private slots:
  void init();
  void slotContextMenu(QMenu *popup, const Context *context);
  void slotEditCurrentTag();
  
private:
  void setupActions();
  
  QuantaCoreIf * m_qcore;
  KAction *m_editCurrentTagAction;
};

#endif
