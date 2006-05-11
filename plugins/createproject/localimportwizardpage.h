/***************************************************************************
                          projectnewlocal.h  -  description
                             -------------------
    begin                : Fri Oct 27 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon <pdima@users.sourceforge.net,yshurik@penguinpowered.com,sequitur@easystreet.com>
                           (C) 2001-2005 Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef LOCALIMPORTWIZARDPAGE_H
#define LOCALIMPORTWIZARDPAGE_H

#include "ui_localimportwizardpagebase.h"

#include <QPair>
#include <QList>

#include <kurl.h>

class KInstance;

class LocalImportWizardPage : public QWidget, public Ui::LocalImportWizardPageBase
{
   Q_OBJECT
public:
  LocalImportWizardPage(KInstance *instance, QWidget *parent=0);
  ~LocalImportWizardPage();

  /**
   * Get the list of the urls to be added to the project. The format of the 
   * pair is: (source url, destination url)
   * @return the list described above
   */
  QList<QPair<KUrl, KUrl> > files();

protected slots:
  /**
   * Called when insertion of files from the project folder is checked, unchecked or the insertion options were changed.
   */
  void slotInsertFiles();
  void setBaseURL(const KUrl& a_baseURL);
  void slotAddFolder();
  void slotAddFiles();
  void slotClearList();

  virtual void resizeEvent( QResizeEvent * );

private:
  void insertURLs(const QString &sourceBase, const KUrl::List &urls);
  /**
   * Get a list of files under the project directory based on the filters selected by the user.
   * @return the list of the files
   */
  KUrl::List projectFiles();

  KUrl baseURL;
};

#endif
