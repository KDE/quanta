/***************************************************************************
                          eventconfigurationdlg.cpp  -  description
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
#include <qdom.h>

//kde includes
#include <kcombobox.h>
#include <kdialogbase.h>
#include <klistview.h>
#include <klocale.h>
#include <kmessagebox.h>

//app includes
#include "eventconfigurationdlg.h"
#include "eventeditordlg.h"
#include "qpevents.h"

EventConfigurationDlg::EventConfigurationDlg(KActionCollection *actionCollection, QWidget* parent, const char* name, WFlags fl)
: EventConfigurationDlgS(parent,name,fl)
{
  m_actionCollection = actionCollection;
}

EventConfigurationDlg::~EventConfigurationDlg()
{
}

void EventConfigurationDlg::initEvents(EventActions *events)
{
  QListViewItem *item;
  for (EventActions::ConstIterator it = events->constBegin(); it != events->constEnd(); ++it)
  {
    QValueList<EventAction> evList = it.data();
    for (QValueList<EventAction>::ConstIterator it2 = evList.constBegin(); it2 != evList.constEnd(); ++it2)
    {
      EventAction ev = *it2;
      item = new QListViewItem(eventsListView, QPEvents::ref()->fullEventName(it.key()), QPEvents::ref()->fullActionName(ev.action));
      int argcount = ev.arguments.count();
      if (argcount > 0)
          item->setText(2, ev.arguments[0]);
      if (argcount > 1)
          item->setText(3, ev.arguments[1]);
      if (argcount > 2)
          item->setText(4, ev.arguments[2]);
      if (argcount > 3)
          item->setText(5, ev.arguments[3]);
    }
  }
}

void EventConfigurationDlg::saveEvents(QDomDocument dom)
{
    QDomNode projectNode = dom.firstChild().firstChild();
    QDomNode eventsNode = projectNode.namedItem("events");
    projectNode.removeChild(eventsNode);
    eventsNode = dom.createElement("events");
    projectNode.appendChild(eventsNode);
    QDomElement node;
    QListViewItemIterator it(eventsListView);
    QListViewItem *item;
    while (it.current())
    {
       item = it.current();
       node = dom.createElement("event");
       eventsNode.appendChild(node);
       QDomElement el = node.toElement();
       el.setAttribute("name", QPEvents::ref()->eventName(item->text(0)));
       el.setAttribute("action", QPEvents::ref()->actionName(item->text(1)));
       if (el.attribute("action") == "script" || el.attribute("action") == "action")
         el.setAttribute("type", "external");
       else
         el.setAttribute("type", "internal");
       for (uint  i = 2; i < 6; i++)
       {
          QString s = item->text(i);
          if (s.isEmpty())
            s = "--not set--";
          QDomNode argNode = dom.createElement("argument");
          node.appendChild(argNode);
          argNode.appendChild(dom.createTextNode(s));
        }
        ++it;
    }
}

void EventConfigurationDlg::slotAddEvent()
{
   KDialogBase editDlg(this, "add_event", true, i18n("New Event"), KDialogBase::Ok | KDialogBase::Cancel);
   EventEditorDlg eventDlg(m_actionCollection, &editDlg);
   editDlg.setMainWidget(&eventDlg);
   if (editDlg.exec())
   {
      QListViewItem *item = new QListViewItem(eventsListView);
      item->setText(0, eventDlg.eventCombo->currentText());
      item->setText(1, eventDlg.actionCombo->currentText());
      item->setText(2, eventDlg.argument1());
      item->setText(3, eventDlg.argument2());
      item->setText(4, eventDlg.argument3());
      item->setText(5, eventDlg.argument4());
   }
}

void EventConfigurationDlg::slotEditEvent()
{
   QListViewItem *item = eventsListView->currentItem();
   if (!item) return;
   KDialogBase editDlg(this, "edit_event", true, i18n("Edit Event"), KDialogBase::Ok | KDialogBase::Cancel);
   EventEditorDlg eventDlg(m_actionCollection, &editDlg);
   editDlg.setMainWidget(&eventDlg);
   eventDlg.setEvent(item->text(0));
   eventDlg.setAction(item->text(1));
   QStringList arguments;
   arguments << item->text(2) << item->text(3) << item->text(4) << item->text(5);
   eventDlg.setArguments(arguments);
   if (editDlg.exec())
   {
      item->setText(0, eventDlg.eventCombo->currentText());
      item->setText(1, eventDlg.actionCombo->currentText());
      item->setText(2, eventDlg.argument1());
      item->setText(3, eventDlg.argument2());
      item->setText(4, eventDlg.argument3());
      item->setText(5, eventDlg.argument4());
   }
}

void EventConfigurationDlg::slotDeleteEvent()
{
   QListViewItem *item = eventsListView->currentItem();
   if (!item) return;
   if (KMessageBox::warningContinueCancel(this, i18n("<qt>Are you sure that you want to remove the configuration of the <b>%1</b> event?</qt>").arg(item->text(0)), i18n("Delete Event Configuration"),KStdGuiItem::del()) == KMessageBox::Continue)
   {
      delete item;
   }
}

#include "eventconfigurationdlg.moc"

