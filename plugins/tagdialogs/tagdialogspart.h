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

#ifndef TAGDIALOGSPART_H
#define TAGDIALOGSPART_H

#include "tagdialogsif.h"

#include <iplugin.h>

class QuantaCoreIf;

namespace KDevelop 
{ 
  class Context; 
}
class QMenu;
class KAction;

/**
Please read the README.dox file for more info about this part
*/
class TagDialogsPart: public KDevelop::IPlugin, TagDialogsIf
{
  Q_OBJECT
  Q_INTERFACES(TagDialogsIf)
public:
  TagDialogsPart(QObject *parent, const QVariantList &args);
  ~TagDialogsPart();

  TagPair createNewTag(QTag *dtdTag, const QString& selection, const QString& attrs = QString(), const KUrl& baseURL = KUrl());

private slots:
  void init();
  void slotContextMenu(QMenu *popup, const KDevelop::Context *context);
  void slotEditCurrentTag();
  
private:
  void setupActions();
  
  QuantaCoreIf * m_qcore;
  KAction *m_editCurrentTagAction;
};

#endif
