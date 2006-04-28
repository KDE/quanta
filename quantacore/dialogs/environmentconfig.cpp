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
#include "environmentconfig.h"
#include "dtds.h"
#include "settings.h"
#include "tag.h"
#include "tagattr.h"

#include <qstringlist.h>

#include <kcombobox.h>

EnvironmentConfig::EnvironmentConfig(QWidget *parent)
 : QWidget(parent)
{
  setupUi(this);
  QStringList lst = DTDs::ref()->nickNameList(true);
  uint pos = 0;
  for (int i = 0; i < lst.count(); i++)
  {
    defaultDTEP->insertItem(lst[i]);
    if (lst[i] == DTDs::ref()->getDTDNickNameFromName(Settings::self()->defaultDTEP().toLower()))
       pos = i;
  }
  defaultDTEP->setCurrentIndex(pos);
  tagCase->setCurrentIndex(Settings::self()->tagCase());
  attributeCase->setCurrentIndex(Settings::self()->attributeCase());
  pos = 0;
  if (Settings::self()->attributeQuotationChar() == "'")
    pos = 1;
  attributeQuotation->setCurrentIndex(pos);
}


EnvironmentConfig::~EnvironmentConfig()
{
}

void EnvironmentConfig::accept()
{
  Settings::self()->setDefaultDTEP(DTDs::ref()->getDTDNameFromNickName(defaultDTEP->currentText()));
  Settings::self()->setTagCase(tagCase->currentIndex());
  Settings::self()->setAttributeCase(attributeCase->currentIndex());
  QChar attrQuotation = '"';
  if (attributeQuotation->currentIndex() == 1)
    attrQuotation = '\'';
  Settings::self()->setAttributeQuotationChar(attrQuotation);
  Settings::self()->writeConfig();
}

#include "environmentconfig.moc"
