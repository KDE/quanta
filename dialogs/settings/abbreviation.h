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

#include <qwidget.h>
#include <abbreviations.h>

class QListViewItem;
struct DTDStruct;

class Abbreviation : public Abbreviations
{
  Q_OBJECT
public:
  Abbreviation(QWidget *parent, const char *name = 0);
  ~Abbreviation();

  void saveTemplates();

public slots:
  void slotDTDChanged(const QString& newDTDName);

private slots:
  void slotTemplateSelectionChanged(QListViewItem*);
  void slotAddTemplate();
  void slotRemoveTemplate();
  void slotEditTemplate();

private:
  DTDStruct *m_dtd;
  QListViewItem *oldItem;
};

#endif
