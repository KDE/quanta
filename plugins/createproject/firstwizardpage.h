/***************************************************************************
                          projectnewgeneral.h  -  description
                             -------------------
    begin                : Fri Oct 27 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon <pdima@users.sourceforge.net,yshurik@penguinpowered.com,sequitur@easystreet.com>
                           (C) 2001, 2002, 2005 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef FIRSTWIZARDPAGE_H
#define FIRSTWIZARDPAGE_H

#include "firstwizardpagebase.h"

#include <kurl.h>

class QuantaCoreIf;
class KInstance;

class FirstWizardPage : public FirstWizardPageBase  {
   Q_OBJECT
public:
  FirstWizardPage(QuantaCoreIf *qCore, KInstance *instance, QWidget *parent=0, const char *name=0);
  ~FirstWizardPage();

  void setMargin(int);
  KUrl baseURL() const {return m_baseUrl;}
  QString name() const;
  QString fileName() const;
  QString author() const;
  QString email() const;
  QString encoding() const;
  QString dtep() const;

protected slots:
  void slotSelectDirectory();
  /**
   * Called whenever a widget has changed which might affect the base URL of the project
   */
  void slotSetProjectBase();
  /**
   * Called when the project name has changed
   * @param name the new name
   */
  void slotChangeNames(const QString &name);
  void slotProtocolChanged(const QString& protocol);


signals:
  /**
   * Emitted whenever a new base URL was set by the user
   * @param  url the new URL
   */
  void newBaseURL(const KUrl& url);
  void enableNextButton(QWidget *, bool);
  void enableFinishButton(QWidget *, bool);
  void enableWebDownload(bool);
  
private:
  KUrl m_baseUrl;  
};

#endif
