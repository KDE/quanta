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
    EventAction ev = (*events)[name];
    if (ev.type == EventAction::Internal)
    {
       if (KMessageBox::warningYesNo(0L, i18n("<qt>An internal action (<i>%1</i>) associated with an event (<i>%2</i>) will be executed.  Do you want to allow the execution of this action?</qt>").arg(ev.action).arg(name), i18n("Event Triggered"), KStdGuiItem::yes(), KStdGuiItem::no(), "warn_about_internal_actions") == KMessageBox::No)
         return;
    } else
    {
       if (KMessageBox::warningYesNo(0L, i18n("<qt>An external action (<i>%1</i>) associated with an event (<i>%2</i>) will be executed.  Do you want to allow the execution of this action?</qt>").arg(ev.action).arg(name), i18n("Event Triggered"), KStdGuiItem::yes(), KStdGuiItem::no(), "warn_about_external_actions") == KMessageBox::No)
         return;
    }
    if (name == "after_save")
    {
        Document *w = ViewManager::ref()->activeDocument();
        if (w && Project::ref()->contains(w->url()))
        {
          ev.arguments << i18n("Document saved");
          ev.arguments << QExtFileInfo::toRelative(w->url(), Project::ref()->projectBaseURL()).path();
          handleEvent(ev);
        }
     } else
     if (name == "before_save")
     {
        Document *w = ViewManager::ref()->activeDocument();
        if (w && Project::ref()->contains(w->url()))
        {
          ev.arguments << i18n("About to save");
          ev.arguments << QExtFileInfo::toRelative(w->url(), Project::ref()->projectBaseURL()).path();
          handleEvent(ev);
        }
     }
  }
}

bool QPEvents::handleEvent(const EventAction& ev)
{
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

       QString body;
       for (uint i = 2; i < ev.arguments.count(); i++)
          body += ev.arguments[i] + "\n";
       kapp->invokeMailer(member.name + "<" + member.email + ">", "", "", ev.arguments[1], body, "", QStringList(), "");

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
