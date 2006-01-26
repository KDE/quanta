/***************************************************************************
                            pathmapperdialog.cpp
                            --------------------
    begin                : 2005-01-08
    copyright            : (C) 2004 Linus McCabe <linus@mccabe.nu>
 ***************************************************************************/

/****************************************************************************
 *                                                                          *
 *   This program is free software; you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation; either version 2 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 ***************************************************************************/


#include "pathmapperdialog.h"
#include <qlistview.h>
#include <qlineedit.h>
#include <qextfileinfo.h>
#include <qcolor.h>
#include <kled.h>

PathMapperDialog::PathMapperDialog(const QString& path, const PathMapperDialog::Direction direction)
    : PathMapperDialogS(0, "PathMapperDialog", false, 0)
{
  m_direction = direction;
  m_path = path;
  linePath->setText(path);

  if(m_direction == LocalToServer)
    ledTranslationExists->hide();
  
  connect(listHistory, SIGNAL(selectionChanged()), this, SLOT(slotSelectionChanged()));
  connect(lineLocalPath, SIGNAL(textChanged(const QString&)), this, SLOT(slotPathsChanged()));
  connect(lineServerPath, SIGNAL(textChanged(const QString&)), this, SLOT(slotPathsChanged()));
}

PathMapperDialog::~PathMapperDialog()
{
}

void PathMapperDialog::addHistory(const QString &serverdir, const QString &localdir)
{
  new QListViewItem(listHistory, localdir, serverdir);
}

void PathMapperDialog::slotSelectionChanged()
{
  lineLocalPath->setText(listHistory->currentItem()->text(0));
  lineServerPath->setText(listHistory->currentItem()->text(1));
}

void PathMapperDialog::slotPathsChanged()
{
  QString translated, from, to;
  if(m_direction == ServerToLocal)
  {
    from = lineServerPath->text();
    to = lineLocalPath->text();
  }
  else
  {
    to = lineServerPath->text();
    from = lineLocalPath->text();
  }

  translated = m_path;
  
  // Check if this dir is matched by the maps
  if(m_path.startsWith(from, false))
  {
    translated.remove(0, from.length());
    translated = to + translated;
  }

  // Indicate wether local file exists
  if(m_direction == ServerToLocal)
  {
    if(QExtFileInfo::exists(translated, true, this))
      ledTranslationExists->setColor(Qt::green);
    else
      ledTranslationExists->setColor(Qt::red);
    ledTranslationExists->on();
  }

  lineTranslated->setText(translated);
}

QString PathMapperDialog::serverPath()
{
  return lineServerPath->text();
}

QString PathMapperDialog::localPath()
{
  return lineLocalPath->text();
}

#include "pathmapperdialog.moc"

