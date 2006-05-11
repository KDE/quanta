/***************************************************************************
    begin                : Fri Jul 1 2005
    copyright            : (C) 2005 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/
#include "autocompletionconfig.h"
#include "settings.h"

#include <QCheckBox>

AutocompletionConfig::AutocompletionConfig(QWidget *parent)
 : QWidget(parent)
{
  setupUi(this);
  closeTags->setChecked(Settings::self()->closeTags());
  closeOptionalTags->setChecked(Settings::self()->closeOptionalTags());
  updateClosingTags->setChecked(Settings::self()->updateClosingTags());
  enableAutocompletion->setChecked(Settings::self()->enableAutocompletion());
  replaceAccented->setChecked(Settings::self()->replaceAccented());
}


AutocompletionConfig::~AutocompletionConfig()
{
  
}


void AutocompletionConfig::accept()
{
  Settings::self()->setCloseTags(closeTags->isChecked());
  Settings::self()->setCloseOptionalTags(closeOptionalTags->isChecked());
  Settings::self()->setUpdateClosingTags(updateClosingTags->isChecked());
  Settings::self()->setEnableAutocompletion(enableAutocompletion->isChecked());
  Settings::self()->setReplaceAccented(replaceAccented->isChecked());
  Settings::self()->writeConfig();
}

#include "autocompletionconfig.moc"
