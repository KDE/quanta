/***************************************************************************
                          eventeditordlg.cpp  -  description
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

//qt includes
#include <qlabel.h>
#include <qregexp.h>

//kde includes
#include <kaction.h>
#include <kcombobox.h>
#include <klocale.h>

 //app includes
#include "eventeditordlg.h"
#include "qpevents.h"
#include "project.h"
#include "resource.h"
#include "quanta.h"
#include "tagaction.h"

extern QString simpleMemberStr;
extern QString taskLeaderStr;
extern QString teamLeaderStr;
extern QString subprojectLeaderStr;

EventEditorDlg::EventEditorDlg(QWidget* parent, const char* name, WFlags fl)
: EventEditorDlgS(parent,name,fl)
{
   eventCombo->insertStringList(QPEvents::ref()->eventNames());
   actionCombo->insertStringList(QPEvents::ref()->actionNames());
   slotActionChanged(actionCombo->currentText());
}

EventEditorDlg::~EventEditorDlg()
{
}

void EventEditorDlg::setEvent(const QString &name)
{
   for (int i = 0; i < eventCombo->count(); i++)
   {
       if (eventCombo->text(i) == name)
       {
           eventCombo->setCurrentItem(i);
           break;
       }
   }
}

void EventEditorDlg::setAction(const QString &name)
{
   for (int i = 0; i < actionCombo->count(); i++)
   {
       if (actionCombo->text(i) == name)
       {
           actionCombo->setCurrentItem(i);
           slotActionChanged(name);
           break;
       }
   }
}

QString EventEditorDlg::argument1()
{
   if (argument1Combo->isEnabled())
   {
      QString actionType = actionCombo->currentText();
      if (actionType == QPEvents::ref()->fullActionName("email"))
      {
          QString s = argument1Combo->currentText();
          if (s ==  i18n(teamLeaderStr.utf8()))
              s = "teamleader";
          else
          if (s.startsWith(i18n(taskLeaderStr.utf8()) +" - "))
          {
              s.remove(i18n(taskLeaderStr.utf8()) +" - ");
              s.prepend("taskleader-");
          } else
          if (s.startsWith(i18n(subprojectLeaderStr.utf8()) +" - "))
          {
              s.remove(i18n(subprojectLeaderStr.utf8()) +" - ");
              s.prepend("subprojectleader-");
          }
          return s;
       } else
       if (actionType == QPEvents::ref()->fullActionName("script"))
       {
          QString s = argument1Combo->currentText();
          for (QMap<QString, QString>::ConstIterator it = m_scriptActions.constBegin(); it != m_scriptActions.constEnd(); ++it)
          {
              if (it.data() == s)
              {
                  s = it.key();
                  break;
              }
          }
          return s;
       }
   }

   return QString::null;
}

QString EventEditorDlg::argument2()
{
   if (argument2Combo->isEnabled())
   {
     return argument2Combo->currentText();
   } else
     return QString::null;
}

QString EventEditorDlg::argument3()
{
   if (argument3Combo->isEnabled())
   {
     return argument3Combo->currentText();
   } else
     return QString::null;
}

QString EventEditorDlg::argument4()
{
   if (argument4Combo->isEnabled())
   {
     return argument4Combo->currentText();
   } else
     return QString::null;
}

void EventEditorDlg::setArguments(const QStringList& arguments)
{
   QString actionType = actionCombo->currentText();
   if (actionType == QPEvents::ref()->fullActionName("email"))
   {
      QString s = arguments[0];
      if (s == "teamleader")
          s = i18n(teamLeaderStr.utf8());
      else
      if (s.startsWith("taskleader-"))
      {
         s.remove("taskleader-");
         s.prepend(i18n(taskLeaderStr.utf8()) +" - ");
      } else
      if (s.startsWith("subprojectleader-"))
      {
         s.remove("subprojectleader-");
         s.prepend(i18n(subprojectLeaderStr.utf8()) +" - ");
      }
      if (argument1Combo->contains(s))
      {
          for (int i = 0; i < argument1Combo->count(); i++)
          {
              if (argument1Combo->text(i) == s)
              {
                  argument1Combo->setCurrentItem(i);
                  break;
              }
          }
      }
      else
      {
         argument1Combo->insertItem(s, 0);
         argument1Combo->setCurrentItem(0);
      }
   } else
   if (actionType == QPEvents::ref()->fullActionName("script"))
   {
      QString s = arguments[0];
      if (m_scriptActions.contains(s))
        s = m_scriptActions[s];
      if (argument1Combo->contains(s))
      {
          for (int i = 0; i < argument1Combo->count(); i++)
          {
              if (argument1Combo->text(i) == s)
              {
                  argument1Combo->setCurrentItem(i);
                  break;
              }
          }
      }
      else
      {
         argument1Combo->insertItem(s, 0);
         argument1Combo->setCurrentItem(0);
      }
   } else
   {
     argument1Combo->insertItem(arguments[0], 0);
     argument1Combo->setCurrentItem(0);
   }
   argument2Combo->insertItem(arguments[1], 0);
   argument2Combo->setCurrentItem(0);
   argument3Combo->insertItem(arguments[2], 0);
   argument3Combo->setCurrentItem(0);
   argument4Combo->insertItem(arguments[3], 0);
   argument4Combo->setCurrentItem(0);
}

void EventEditorDlg::slotActionChanged(const QString &name)
{
   argument1Label->setEnabled(false);
   argument2Label->setEnabled(false);
   argument3Label->setEnabled(false);
   argument4Label->setEnabled(false);
   argument1Combo->setEnabled(false);
   argument2Combo->setEnabled(false);
   argument3Combo->setEnabled(false);
   argument4Combo->setEnabled(false);
   argument1Combo->clear();
   argument2Combo->clear();
   argument3Combo->clear();
   argument4Combo->clear();
   if (name == QPEvents::ref()->fullActionName("email"))
   {
       argument1Label->setEnabled(true);
       argument1Label->setText(i18n("Receiver:"));
       argument1Combo->setEnabled(true);
       argument1Combo->insertItem(i18n(teamLeaderStr.utf8()));
       QValueList<SubProject> *subprojects = Project::ref()->subprojects();
       for (uint i = 0; i < subprojects->count(); i++)
       {
          QString s = (*subprojects)[i].name;
          if (!Project::ref()->subprojectLeader(s).name.isEmpty())
            argument1Combo->insertItem(i18n(subprojectLeaderStr.utf8()) + " - " + s);
       }
       QStringList tasks = Project::ref()->tasks();
       for (uint i = 0; i < tasks.count(); i++)
       {
          argument1Combo->insertItem(i18n(taskLeaderStr.utf8()) + " - " + tasks[i]);
       }
   } else
   if (name == QPEvents::ref()->fullActionName("log"))
   {
       argument1Label->setEnabled(true);
       argument1Label->setText(i18n("Log file:"));
       argument1Combo->setEnabled(true);
   } else
   if (name == QPEvents::ref()->fullActionName("script"))
   {
       argument1Label->setEnabled(true);
       argument1Label->setText(i18n("Action name:"));
       TagAction *action = 0L;
       QString s;
       QRegExp r("\\&(?!\\&)");
       for (uint i = 0; i < quantaApp->actionCollection()->count(); i++)
       {
          action = dynamic_cast<TagAction*>(quantaApp->actionCollection()->action(i));
          if (action && action->type() == "script")
          {
             s = action->text().replace(r, "");
             argument1Combo->insertItem(s);
             m_scriptActions[action->name()] = s;
          }
       }
       argument1Combo->setEnabled(true);
   }
}

#include "eventeditordlg.moc"
