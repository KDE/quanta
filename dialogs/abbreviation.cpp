//
//
// C++ Implementation: cpp
//
// Description:
//
//
// Author: Andras Mantia <amantia@freemail.hu>, (C) 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//

//qt includes
#include <qtextedit.h>

//kde includes
#include <kcombobox.h>
#include <klistview.h>

//app includes
#include "abbreviation.h"
#include "../resource.h"


Abbreviation::Abbreviation(QWidget *parent, const char *name):
    Abbreviations(parent, name)
{
}

Abbreviation::~Abbreviation()
{
}

void Abbreviation::slotDTDChanged(const QString& newDTDName)
{
  templatesList->clear();
  QString dtdName = QuantaCommon::getDTDNameFromNickName(newDTDName);
  m_dtd = dtds->find(dtdName.lower());
  QString templateStr;
  QString templateName;
  QMap<QString, QString>::Iterator it;
  for ( it = m_dtd->abbreviations.begin(); it != m_dtd->abbreviations.end(); ++it )
  {
    templateStr = it.key();
    templateName = templateStr.mid(templateStr.find(' ') + 1);
    templateStr = templateStr.left(templateStr.find(' '));
    new KListViewItem(templatesList, templateStr, templateName);
  }
  templatesList->sort();
}

void Abbreviation::slotTemplateSelectionChanged(QListViewItem* item)
{
  QString code = m_dtd->abbreviations[item->text(0)+" " +item->text(1)];
  codeEdit->setText(code);
}

#include "abbreviation.moc"
