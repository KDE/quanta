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
#include "resource.h"
#include "quanta.h"
#include "tagaction.h"

QPEvents::QPEvents(QObject *parent, const char *name)
 : QObject(parent, name)
{
   m_eventNames["before_save"] = i18n("Before document save");
   m_eventNames["after_save"] = i18n("After document save");
   m_eventNames["after_open"] = i18n("After document open");
   m_eventNames["before_close"] = i18n("Before document close");
   m_eventNames["after_close"] = i18n("After document close");
   m_eventNames["after_project_open"] = i18n("After project open");
   m_eventNames["after_project_close"] = i18n("After project close");
   m_eventNames["before_upload"] = i18n("Before upload");
   m_eventNames["after_upload"] = i18n("After upload");
   m_eventNames["after_project_add"] = i18n("After addition to project");
   m_eventNames["after_project_remove"] = i18n("After removal from project");
   m_eventNames["after_commit"] = i18n("After committing to CVS");
   m_eventNames["after_file_move"] = i18n("After moving a file inside the project");
//   m_eventNames["after_multiple_save"] = i18n("After saving more files at once (like Save All)");

   m_actionNames["email"] = i18n("Send email");
   m_actionNames["log"] = i18n("Log event");
   m_actionNames["script"] = i18n("Script action");
}


QPEvents::~QPEvents()
{
}

void QPEvents::slotEventHappened(const QString& name, const QString& argument1, const QString& argument2)
{
  EventActions *events = Project::ref()->events();
  if (events && events->contains(name))
  {
    EventAction ev = (*events)[name];
    if (ev.type == EventAction::Internal)
    {
       if (KMessageBox::warningYesNo(0L, i18n("<qt>An internal action (<i>%1</i>) associated with an event (<i>%2</i>) will be executed.  Do you want to allow the execution of this action?</qt>").arg(ev.action).arg(name), i18n("Event Triggered"), KStdGuiItem::yes(), KStdGuiItem::no(), "Warn about internal actions") == KMessageBox::No)
         return;
    } else
    {
       if (KMessageBox::warningYesNo(0L, i18n("<qt>An external action (<i>%1</i>) associated with an event (<i>%2</i>) will be executed.  Do you want to allow the execution of this action?</qt>").arg(ev.action).arg(name), i18n("Event Triggered"), KStdGuiItem::yes(), KStdGuiItem::no(), "Warn about external actions") == KMessageBox::No)
         return;
    }
    KURL url = KURL::fromPathOrURL(argument1);
    KURL url2 = KURL::fromPathOrURL(argument2);
    if (url.isValid())
    {
        if (url2.isValid())
        {
            if (name == "before_upload")
            {
              ev.arguments << i18n("About to upload a document");
              ev.arguments << url.path();
              ev.arguments << url2.path();
              handleEvent(ev);
            } else
            if (name == "after_upload")
            {
              ev.arguments << i18n("Document uploaded");
              ev.arguments << url.path();
              ev.arguments << url2.path();
              handleEvent(ev);
            } else
            if (name == "after_file_move")
            {
              ev.arguments << i18n("Document moved");
              ev.arguments << url.path();
              ev.arguments << url2.path();
              handleEvent(ev);
            }
        } else
        {
            QString relativePath = QExtFileInfo::toRelative(url, Project::ref()->projectBaseURL()).path();
            if (name == "after_save")
            {
              ev.arguments << i18n("Document saved");
              ev.arguments << relativePath;
              handleEvent(ev);
            } else
            if (name == "before_save")
            {
              ev.arguments << i18n("About to save a document");
              ev.arguments << relativePath;
              handleEvent(ev);
            } else
            if (name == "after_open")
            {
              ev.arguments << i18n("Document opened");
              ev.arguments << relativePath;
              handleEvent(ev);
            } else
            if (name == "after_close")
            {
              ev.arguments << i18n("Document closed");
              ev.arguments << relativePath;
              handleEvent(ev);
            } else
            if (name == "before_close")
            {
              ev.arguments << i18n("About to close a document");
              ev.arguments << relativePath;
              handleEvent(ev);
            } else
            if (name == "after_project_open")
            {
                ev.arguments << i18n("Project opened");
                ev.arguments <<  url.path();
                handleEvent(ev);
            } else
            if (name == "after_project_close")
            {
                ev.arguments << i18n("Project closed");
                ev.arguments <<  url.path();
                handleEvent(ev);
            }
            if (name == "after_project_add")
            {
                ev.arguments << i18n("Document added to project");
                ev.arguments <<  url.path();
                handleEvent(ev);
            } else
            if (name == "after_project_remove")
            {
                ev.arguments << i18n("Document removed from project");
                ev.arguments <<  url.path();
                handleEvent(ev);
            }
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
      KMessageBox::sorry(0L, i18n("Unsupported internal event action."));
  } else
  if (ev.type == EventAction::External)
  {
      //KMessageBox::sorry(0L, i18n("External event actions are not yet supported."));
      if (ev.action == "script")
      {
          QString name = ev.arguments[0];
          TagAction *action = dynamic_cast<TagAction*>(quantaApp->actionCollection()->action(name));
          if (action)
          {
            action->addArguments(ev.arguments);
            action->execute();
          }
          else
            KMessageBox::sorry(0L, i18n("<qt>The <b>%1</b> script action was not found on your system.</qt>").arg(name), i18n("Action Execution Error"));
      }  else
      KMessageBox::sorry(0L, i18n("Unsupported external event action."));
  } else
      KMessageBox::sorry(0L, i18n("Unknown event type."));
  return false;
}

QString QPEvents::fullEventName(const QString &name)
{
   if (m_eventNames.contains(name))
     return m_eventNames[name];
   else
     return name;
}

QString QPEvents::fullActionName(const QString& name)
{
   if (m_actionNames.contains(name))
     return m_actionNames[name];
   else
     return name;
}

QString QPEvents::eventName(const QString &fullName)
{
   for (QMap<QString, QString>::ConstIterator it = m_eventNames.constBegin(); it != m_eventNames.constEnd(); ++it)
  {
    if (fullName  == it.data())
      return it.key();
  }
  return fullName;
}

QString QPEvents::actionName(const QString &fullName)
{
   for (QMap<QString, QString>::ConstIterator it = m_actionNames.constBegin(); it != m_actionNames.constEnd(); ++it)
  {
    if (fullName  == it.data())
      return it.key();
  }
  return fullName;
}

QStringList QPEvents::eventNames()
{
   QStringList names;
   for (QMap<QString, QString>::ConstIterator it = m_eventNames.constBegin(); it != m_eventNames.constEnd(); ++it)
  {
    names << it.data();
  }
  return names;
}

QStringList QPEvents::actionNames()
{
   QStringList names;
   for (QMap<QString, QString>::ConstIterator it = m_actionNames.constBegin(); it != m_actionNames.constEnd(); ++it)
  {
    names << it.data();
  }
  return names;
}

#include "qpevents.moc"
