/***************************************************************************
                          eventeditordlg.h  -  description
                             -------------------
    begin                : Mon Jul 12 2004
    copyright          : (C) 2004 Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef EVENTEDITORDLG_H
#define EVENTEDITORDLG_H

#include "eventeditordlgs.h"

class QLabel;
class KActionCollection;
class KComboBox;

class EventEditorDlg : public EventEditorDlgS
{
  Q_OBJECT

public:
  EventEditorDlg(KActionCollection *actionCollection, QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
  ~EventEditorDlg();
  void setEvent(const QString &name);
  void setAction(const QString &name);
  void setArguments(const QStringList& arguments);
  QString argument1();
  QString argument2();
  QString argument3();
  QString argument4();

public slots:
   void slotActionChanged(const QString &name);

protected:
   void resetArgumentWidgets(QLabel *label, KComboBox *combo);

   QMap<QString, QString> m_scriptActions;
   QMap<QString, QString> m_otherActions;
   KActionCollection *m_actionCollection;

protected slots:

};

#endif

