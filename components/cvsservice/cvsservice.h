/***************************************************************************
                                              cvsservice.h  -  description
                                             ------------------------------
    begin                : Sun May 16 17:50:25 2004
    copyright          : (C) 2004 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2
 *
 ***************************************************************************/

#ifndef CVSSERVICE_H
#define CVSSERVICE_H

#include <qobject.h>
#include <dcopobject.h>

class QPopupMenu;
class KActionCollection;
class CvsJob_stub;
class Repository_stub;

/** @short This class manages the CVS repositories from withing Quanta with the help of "cvsservice"
 *
 */
class CVSService : public QObject, public DCOPObject
{
  Q_OBJECT

public:

  /**
   *  since this class is a singleton you must use this function to access it
   *  @return the class pointer
   */
  static CVSService* ref(KActionCollection *ac = 0L)
  {
    static CVSService *m_ref;
    if (!m_ref) m_ref = new CVSService(ac);
    return m_ref;
  }

  ~CVSService();

  void setAppId(const QCString& id) {m_appId = id;}
  /** Returns true if the cvsservice was found */
  bool exists() {return !m_appId.isEmpty();}
  void setRepository(const QString &repository);
  QPopupMenu *menu() {return m_menu;}

public slots:
  void slotUpdate(const QStringList &files);
  void slotJobExited(bool normalExit, int exitStatus);
  void slotReceivedStdout(QString output);

private:
  CVSService(KActionCollection *ac);

  QCString m_appId;
  QPopupMenu *m_menu;
  Repository_stub *m_repository;
  CvsJob_stub *m_cvsJob;
};

#endif
