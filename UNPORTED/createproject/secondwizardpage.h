/***************************************************************************
                          projectnewgeneral.h  -  description
                             -------------------
    begin                : Fri Oct 27 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon <pdima@users.sourceforge.net,yshurik@penguinpowered.com,sequitur@easystreet.com>
                           (C) 2001-2002 Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef SECONDWIZARDPAGE_H
#define SECONDWIZARDPAGE_H

#include "ui_secondwizardpagebase.h"

class QStackedWidget;

class KIconLoader;
class SecondWizardPage : public QWidget, Ui::SecondWizardPageBase  {
   Q_OBJECT
public:
  SecondWizardPage(QStackedWidget *importStack, KIconLoader *iconLoader, QWidget *parent=0);
  ~SecondWizardPage();

  void setMargin(int);

  enum InsertMode{
    Local = 1,
    WGet
  };
  InsertMode insertMode() const;
  bool usePreviewPrefix() const;
  QString previewPrefix() const;
  QString templates() const;
  QString toolbars() const;
  bool insertGlobalTemplates() const;
  bool insertLocalTemplates() const;

public slots:
  void slotEnableWebDownload(bool enable);
private slots:
  void slotLocalInsertToggled(bool toogled);

private:
  QStackedWidget *m_importStack;
};

#endif
