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
#include <qtooltip.h>

//kde includes
#include <kaction.h>
#include <kcombobox.h>
#include <klocale.h>

 //app includes
#include "eventeditordlg.h"
#include "qpevents.h"
#include "project.h"
#include "tagaction.h"

extern QString simpleMemberStr;
extern QString taskLeaderStr;
extern QString teamLeaderStr;
extern QString subprojectLeaderStr;

EventEditorDlg::EventEditorDlg(KActionCollection *actionCollection, QWidget* parent, const char* name, WFlags fl)
: EventEditorDlgS(parent,name,fl)
{
  m_actionCollection = actionCollection;
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
      QString s = argument1Combo->currentText();
      QString actionType = actionCombo->currentText();
      if (actionType == QPEvents::ref()->fullActionName("email"))
      {
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
       } else
       if (actionType == QPEvents::ref()->fullActionName("action"))
       {
          QString s = argument1Combo->currentText();
          for (QMap<QString, QString>::ConstIterator it = m_otherActions.constBegin(); it != m_otherActions.constEnd(); ++it)
          {
              if (it.data() == s)
              {
                  s = it.key();
                  break;
              }
          }
          return s;
       } else

       return s;
   }

   return QString::null;
}

QString EventEditorDlg::argument2()
{
   if (argument2Combo->isEnabled())
   {
     QString actionType = actionCombo->currentText();
     if (actionType == QPEvents::ref()->fullActionName("log"))
     {
        int id = argument2Combo->currentItem();
        if (id == 0)
            return "full";
       else
            return "minimal";
     } else
     if (actionType == QPEvents::ref()->fullActionName("script"))
     {
        int id = argument2Combo->currentItem();
        if (id == 0)
            return "yes";
       else
            return "no";
     }
     return argument2Combo->currentText();
   } else
     return QString::null;
}

QString EventEditorDlg::argument3()
{
   if (argument3Combo->isEnabled())
   {
     QString actionType = actionCombo->currentText();
     if (actionType == QPEvents::ref()->fullActionName("log"))
     {
        int id = argument3Combo->currentItem();
        if (id == 0)
            return "create_new";
       else
            return "append";
     }
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
      s = arguments[1];
      if (s == "yes")
        argument2Combo->setCurrentItem(0);
      else if (s == "no")
        argument2Combo->setCurrentItem(1);
      return; //setup for script arguments completed
   } else
   if (actionType == QPEvents::ref()->fullActionName("action"))
   {
      QString s = arguments[0];
      if (m_otherActions.contains(s))
        s = m_otherActions[s];
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
   if (actionType == QPEvents::ref()->fullActionName("log"))
   {
      argument1Combo->insertItem(arguments[0], 0);
      argument1Combo->setCurrentItem(0);
      QString s = arguments[1];
      if (s == "full")
        argument2Combo->setCurrentItem(0);
      else if (s == "minimal")
        argument2Combo->setCurrentItem(1);
      s = arguments[2];
      if (s == "create_new")
        argument3Combo->setCurrentItem(0);
      else if (s == "append")
        argument3Combo->setCurrentItem(1);
      return; //setup for log arguments completed
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

void EventEditorDlg::resetArgumentWidgets(QLabel *label, KComboBox *combo)
{
   label->setText(i18n("Argument:"));
   label->setEnabled(false);
   combo->setEnabled(false);
   combo->setEditable(true);
   combo->clear();
   QToolTip::remove(combo);
}

void EventEditorDlg::slotActionChanged(const QString &name)
{
   resetArgumentWidgets(argument1Label, argument1Combo);
   resetArgumentWidgets(argument2Label, argument2Combo);
   resetArgumentWidgets(argument3Label, argument3Combo);
   resetArgumentWidgets(argument4Label, argument4Combo);
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
       QToolTip::add(argument1Combo, i18n("A relative file to the project folder or a file outside of the project folder in which case the full path must be specified."));
       argument2Label->setEnabled(true);
       argument2Label->setText(i18n("Detail:"));
       argument2Combo->setEnabled(true);
       argument2Combo->setEditable(false);
       argument2Combo->insertItem(i18n("Full"), 0);
       argument2Combo->insertItem(i18n("Minimal"), 1);
       argument3Label->setEnabled(true);
       argument3Label->setText(i18n("Behavior:"));
       argument3Combo->setEnabled(true);
       argument3Combo->setEditable(false);
       argument3Combo->insertItem(i18n("Create New Log"), 0);
       argument3Combo->insertItem(i18n("Append to Existing Log"), 1);
   } else
   if (name == QPEvents::ref()->fullActionName("script"))
   {
       argument1Label->setEnabled(true);
       argument1Label->setText(i18n("Action name:"));
       TagAction *action = 0L;
       QString s;
       QStringList items;
       QRegExp r("\\&(?!\\&)");
       for (uint i = 0; i < m_actionCollection->count(); i++)
       {
          action = dynamic_cast<TagAction*>(m_actionCollection->action(i));
          if (action && action->type() == "script")
          {
             s = action->text().replace(r, "");
             items.append(s);
             m_scriptActions[action->name()] = s;
          }
       }
       items.sort();
       argument1Combo->insertStringList(items);
       argument1Combo->setEnabled(true);
       argument2Label->setEnabled(true);
       argument2Label->setText(i18n("Blocking:"));
       argument2Combo->setEnabled(true);
       argument2Combo->setEditable(false);
       argument2Combo->insertItem(i18n("Yes"), 0);
       argument2Combo->insertItem(i18n("No"), 1);
   } else
   if (name == QPEvents::ref()->fullActionName("action"))
   {
       argument1Label->setEnabled(true);
       argument1Label->setText(i18n("Action name:"));
       TagAction *action = 0L;
       QString s;
       QRegExp r("\\&(?!\\&)");
       QStringList items;
       for (uint i = 0; i < m_actionCollection->count(); i++)
       {
          KAction *a = m_actionCollection->action(i);
          action = dynamic_cast<TagAction*>(a);
          if (!action || action->type() != "script")
          {
             s = a->text().replace(r, "");
             items.append(s);
             m_otherActions[a->name()] = s;
          }
       }
       items.sort();
       argument1Combo->insertStringList(items);
       argument1Combo->setEnabled(true);
   }
}

#include "eventeditordlg.moc"

