/***************************************************************************
                          eventconfigurationdlg.h  -  description
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

#ifndef EVENTCONFIGURATIONDLG_H
#define EVENTCONFIGURATIONDLG_H

#include "eventconfigurationdlgs.h"

class QDomDocument;
class KActionCollection;
struct EventAction;
typedef  QMap<QString, QValueList<EventAction> > EventActions;

class EventConfigurationDlg : public EventConfigurationDlgS
{
  Q_OBJECT

public:
  EventConfigurationDlg(KActionCollection *actionCollection, QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
  ~EventConfigurationDlg();
  void initEvents(EventActions *events);
  void saveEvents(QDomDocument dom);

public slots:
   void slotEditEvent();
   void slotAddEvent();
   void slotDeleteEvent();

protected:
  KActionCollection *m_actionCollection;

protected slots:

};

#endif

