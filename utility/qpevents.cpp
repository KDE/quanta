/***************************************************************************
                          qpevents.cpp  -  description
                             -------------------
    begin                : Sun Jul 11 2004
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

//qt includes

//kde includes
#include <kapplication.h>
#include <klocale.h>
#include <kmessagebox.h>

//app includes
#include "qpevents.h"
#include "document.h"
#include "project.h"
#include "qextfileinfo.h"
#include "viewmanager.h"

QPEvents::QPEvents(QObject *parent, const char *name)
 : QObject(parent, name)
{
}


QPEvents::~QPEvents()
{
}

void QPEvents::slotEventHappened(const QString& name)
{
  EventActions *events = Project::ref()->events();
  if (events && events->contains(name))
  {
    if (name == "after_save")
    {
        Document *w = ViewManager::ref()->activeDocument();
        if (w && Project::ref()->contains(w->url()))
        {
          EventAction ev = (*events)[name];
          ev.arguments << i18n("The file %1 was saved").arg(QExtFileInfo::toRelative(w->url(), Project::ref()->projectBaseURL()).path());
          handleEvent(ev);
        }
     }
  }
}

bool QPEvents::handleEvent(const EventAction& ev)
{
  return true;
  if (ev.type == EventAction::Internal)
  {
    if (ev.action == "email")
    {
       QString receiver = ev.arguments[0];
       TeamMember member;
       if (receiver == "teamleader")
         member = Project::ref()->teamLeader();
       else if (receiver.startsWith("subprojectleader-"))
         member = Project::ref()->subprojectLeader(receiver.remove("subprojectleader-"));
       else if (receiver.startsWith("taskleader-"))
         member = Project::ref()->taskLeader(receiver.remove("taskleader-"));

       kapp->invokeMailer(member.name + "<" + member.email + ">", ev.arguments[1], "");
       return true;
    } else
      KMessageBox::sorry(0L, i18n("Unsupported internal event."));
  } else
  if (ev.type == EventAction::External)
  {
      KMessageBox::sorry(0L, i18n("External event actions are not yet supported."));
  } else
      KMessageBox::sorry(0L, i18n("Unknown event type."));
  return false;
}


#include "qpevents.moc"
