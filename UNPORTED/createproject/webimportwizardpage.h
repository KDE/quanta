/***************************************************************************
                          projectnewweb.h  -  description
                             -------------------
    begin                : Fri Oct 27 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon <pdima@users.sourceforge.net,yshurik@penguinpowered.com,sequitur@easystreet.com>
                           (C) 2002, 2005 Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef WEBIMPORTWIZARDPAGE_H
#define WEBIMPORTWIZARDPAGE_H

//app includes
#include "ui_webimportwizardpagebase.h"

//qt includes
#include <qwidget.h>
#include <QStringList>

//kde includes
#include <kurl.h>

class KIconLoader;
class K3Process;
class KUrl;

class WebImportWizardPage : public QWidget, public Ui::WebImportWizardPageBase  {
   Q_OBJECT
public:
  explicit WebImportWizardPage(KIconLoader *iconLoader, QWidget *parent=0);
  ~WebImportWizardPage();

public slots:
  void enableStart(const QString&);
  void setCommandL(const QString&);
  void setBaseURL(const KUrl& a_baseURL);

  void slotStart();

  KUrl::List files();

  void slotGetWgetExited(K3Process *);
  void slotGetWgetOutput(K3Process *proc, char *buffer, int buflen);

signals:

  void enableMessagesWidget();

  void messages(const QString& );
  /** No descriptions */
  void enableNextButton(QWidget *,bool);

private:
  KUrl baseURL;

  bool start;

  K3Process *proc;

private slots: // Private slots
  /** No descriptions */
  void setProtocol(const QString& protocol);
};

#endif
