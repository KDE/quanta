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

#include "cvsservicedcopif.h"

class QPopupMenu;
class KActionCollection;
class CvsJob_stub;
class CvsService_stub;
class Repository_stub;

class CVSCommitDlgS;

/** @short This class manages the CVS repositories from withing Quanta with the help of "cvsservice"
 *
 */
class CVSService : public QObject, public CVSServiceDCOPIf
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

  void setAppId(const QCString& id);
  /** Returns true if the cvsservice was found */
  bool exists() {return !m_appId.isEmpty();}
  void setRepository(const QString &repository);
  void setCurrentFile(const QString &file) {m_defaultFile = file;}
  QPopupMenu *menu() {return m_menu;}

public slots:
  void slotUpdate();
  void slotUpdate(const QStringList &files);
  void slotCommit();
  void slotCommit(const QStringList &files);

  virtual void slotJobExited(bool normalExit, int exitStatus);
  virtual void slotReceivedStdout(QString output);

signals:
  void clearMessages();
  void showMessage(const QString &msg, bool append);

private:
  CVSService(KActionCollection *ac);
  void notInRepository();

  QCString m_appId;
  QPopupMenu *m_menu;
  Repository_stub *m_repository;
  CvsJob_stub *m_cvsJob;
  CvsService_stub *m_cvsService;
  QString m_defaultFile;
  QString m_repositoryPath;
  CVSCommitDlgS *m_commitDlg;
};

#endif
