/***************************************************************************
                          qpevents.h  -  description
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

#ifndef QPEVENTS_H
#define QPEVENTS_H

#include <qobject.h>

/**
@author Andras Mantia
*/

/** Describes an event action. */
struct EventAction {
 /** Possible event types */
  enum Types {
     Internal = 0,
     External = 1
  };
  /** The type of the event. See @ref Types */
  uint type;
  /** the name of the action to be executed. In case of external events
     this is the name of the script, in case of internal events it can be one of the
     following: "email"
  */
  QString action;
  /** The arguments for the event action. It is different for each action.
  */
  QStringList arguments;
};

/** The configured events. The key is the event name, the data is the event description.
For example: events["before_save"] points to the event data that needs to be used
before a file is saved. Possible key names are: before_save, after_save, after_open,
after_project_open, after_project_save, before_upload, after_upload, after_project_add,
after_project_remove, after_commit
*/
typedef  QMap<QString, QValueList<EventAction> > EventActions;

class QPEvents : public QObject
{
Q_OBJECT
public:
    static QPEvents* const ref(QObject *parent = 0L)
    {
      static QPEvents *m_ref;
      if (!m_ref) m_ref = new QPEvents(parent);
      return m_ref;
    }
    ~QPEvents();
    QString fullEventName(const QString &name);
    QString fullActionName(const QString &name);
    QString eventName(const QString &fullName);
    QString actionName(const QString &fullName);
    QStringList eventNames();
    QStringList actionNames();

public slots:
 /** Called when an event has happened */
   void slotEventHappened(const QString& name, const QString& argument1, const QString& argument2);

private:
    QPEvents(QObject *parent = 0, const char *name = 0);
  /** Calls the action associated with an event. Returns true if the call succeeded, false
  otherwise. The call might fail if:
    - the action type is unknown
    - the script cannot be found
    - the user canceled the execution
 */
    bool handleEvent(const EventAction& ev);

    QMap<QString, QString> m_eventNames;
    QMap<QString, QString> m_actionNames;
    QString m_eventName;
};

#endif
