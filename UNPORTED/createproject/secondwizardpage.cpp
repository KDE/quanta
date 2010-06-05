/***************************************************************************
                          projectnewgeneral.cpp  -  description
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

// app includes
#include "secondwizardpage.h"
#include "dtdstruct.h"

// qt includes
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QRadioButton>
#include <QStringList>
#include <qstackedwidget.h>

// kde includes
#include <kiconloader.h>

SecondWizardPage::SecondWizardPage(QStackedWidget *importStack, KIconLoader *iconLoader, QWidget *parent)
  : QWidget(parent)
{
  setupUi(this);
  connect(radioLocal, SIGNAL(toggled(bool)), SLOT(slotLocalInsertToggled(bool)));
  imagelabel->setPixmap(iconLoader->loadIcon("secondwizardpage", KIconLoader::User));
  m_importStack = importStack;
 
  linePrjTmpl->setText("resources/templates");
  linePrjToolbar->setText("resources/toolbars");
}

SecondWizardPage::~SecondWizardPage(){
}

void SecondWizardPage::setMargin(int i)
{
  layout()->setMargin(i);
}

bool SecondWizardPage::usePreviewPrefix() const
{
  return checkPrefix->isChecked();
}

QString SecondWizardPage::previewPrefix() const
{
  return linePrefix->text();
}

SecondWizardPage::InsertMode SecondWizardPage::insertMode() const
{
  if (radioWeb->isChecked()) 
    return WGet;
  return Local;
}

QString SecondWizardPage::templates() const
{
  return linePrjTmpl->text();
}

QString SecondWizardPage::toolbars() const
{
  return linePrjToolbar->text();
}
void SecondWizardPage::slotEnableWebDownload(bool enable)
{
  radioWeb->setEnabled(enable);
}

bool SecondWizardPage::insertGlobalTemplates() const
{
  return checkInsertGlobalTemplates->isChecked();
}

bool SecondWizardPage::insertLocalTemplates() const
{
  return checkInsertLocalTemplates->isChecked();
}

void SecondWizardPage::slotLocalInsertToggled(bool toggled)
{
  //FIXME: verify this
  m_importStack->setCurrentIndex(toggled ? 0 : 1);
}

#include "secondwizardpage.moc"
