/***************************************************************************
                          projectupload.h  -  description
                             -------------------
    begin                : Wed Nov 15 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon <pdima@users.sourceforge.net,yshurik@penguinpowered.com,sequitur@easystreet.com>
                           (C) 2002, 2004 Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef PROJECTUPLOAD_H
#define PROJECTUPLOAD_H

#include "projectuploads.h"
#include "project.h"

#include <qdom.h>
#include <qptrlist.h>
#include <qstringlist.h>

#include <kio/job.h>

/**
  *@author Dmitry Poplavsky & Alexander Yakovlev & Andras Mantia
  */

class KURL;
class Project;
class UploadProfileDlgS;

class ProjectUpload : public ProjectUploadS
{
  Q_OBJECT
public:
  /** Initializes the dialog. If showOnlyProfiles is true, the dialog
  displays only the list of profiles, no real upload can be performed.
  If quickUpload is true, the upload starts immediately without checking
  for modifications or confirmation from the user. The url will be
  uploaded to the default profile */
  ProjectUpload(const KURL& url, const QString& profileName = QString::null, bool showOnlyProfiles = false, bool quickUpload = false, bool markOnly = false, const char * name = 0);
  ~ProjectUpload();
  QString defaultProfile();

public slots: // Public slots
  /** No descriptions */
  void slotBuildTree();

protected slots:
  void startUpload();
  void upload();
  void uploadFinished( KIO::Job *job );
  void uploadProgress ( KIO::Job *job, unsigned long percent );
  void uploadMessage ( KIO::Job *, const QString & msg );
  void slotUploadNext();

  void clearSelection();
  void selectAll();
  void selectModified();
  void invertSelection();
  void expandAll();
  void collapseAll();
  void clearProjectModified();

  void slotNewProfile();
  void slotEditProfile();
  void slotRemoveProfile();
  void slotNewProfileSelected(const QString &profileName);

  virtual void resizeEvent( QResizeEvent * );
  virtual void reject();


signals: // Signals
  /** No descriptions */
  void uploadNext();
  void eventHappened(const QString&, const QString&, const QString&);

private:
  void buildSelectedItemList();
  void fillProfileDlg(UploadProfileDlgS *profileDlg);
  void readProfileDlg(UploadProfileDlgS *profileDlg);
  void setProfileTooltip();
  void loadRemoteUploadInfo();
  void saveRemoteUploadInfo();
  void initProjectInfo(const QString& defaultProfile);
  void initBaseUrl(); /// Reads the current profile setting and initialize the baseUrl from it

  KURL::List modified; // modified files
  QValueList<QListViewItem*> needsConfirmation;
  QPtrList<QListViewItem> toUpload; // list of files , still didn't uploaded
  QListViewItem *currentItem;
  KURL currentURL;
  KURL::List madeDirs;
  KURL *baseUrl;
  KURL startUrl;
  bool stopUpload;
  bool uploadInProgress;
  bool suspendUpload;
  QString m_lastPassword;
  Project *m_project;
  QString m_defaultProfile;
  QDomElement m_currentProfileElement;
  QDomElement m_lastEditedProfileElement;
  QDomNode m_profilesNode;
  QDomDocument m_uploadStatusDom;
  QMap<QString, int> m_uploadTimeList;
  bool m_profilesOnly;
  bool m_quickUpload;
};

#endif
