/***************************************************************************
                          abbreviation.cpp  -  description
                             -------------------
    copyright            : (C) 2003 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/

#ifndef ABBREVIATION_H
#define ABBREVIATION_H

#include <qmap.h>
#include <qstringlist.h>

#include <abbreviations.h>

class QListViewItem;
class Abbreviation;
struct DTDStruct;

class AbbreviationDlg : public AbbreviationDlgS
{
  Q_OBJECT
public:
  AbbreviationDlg(QWidget *parent, const char *name = 0);
  ~AbbreviationDlg();

  void saveTemplates();

public slots:
  void slotGroupChanged(const QString& newGroupName);

private slots:
  void slotNewGroup();
  void slotAddDTEP();
  void slotRemoveDTEP();
  void slotTemplateSelectionChanged(QListViewItem*);
  void slotAddTemplate();
  void slotRemoveTemplate();
  void slotEditTemplate();

private:
  QListViewItem *m_oldItem;
  Abbreviation *m_currentAbbrev;
};

#endif
