//
// C++ Implementation: dtepeditdlg
//
// Description: 
//
//
// Author: Andras Mantia <amantia@kde.org>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

//qt includes
#include <qcheckbox.h>
#include <qtabwidget.h>
#include <qwidgetstack.h>


//kde includes
#include <kcombobox.h>
#include <kconfig.h>
#include <klineedit.h>
#include <klocale.h>

//own includes
#include "dtepeditdlg.h"
#include "dtds.h"

DTEPEditDlg::DTEPEditDlg(const QString& descriptionFile, QWidget *parent, const char *name)
 : DTEPEditDlgS(parent, name)
{
  nameEdit->setFocus();
  m_descriptionFile = descriptionFile;
  
  m_config = new KConfig(m_descriptionFile, true);
  init();
  
}


DTEPEditDlg::~DTEPEditDlg()
{
  delete m_config;
}

void DTEPEditDlg::slotFamilyChanged(int family)
{
  m_family = family;
  if (m_family == 0)
    tabWidget->insertTab(m_pagesWidget, i18n("&Pages"), 1);
  else
   tabWidget->removePage(m_pagesWidget);
}

void DTEPEditDlg::init()
{
  m_pagesWidget = tabWidget->page(1);
  readGeneral();
  if (m_family == 0)
    readPages();
  else
    tabWidget->removePage(m_pagesWidget);
}

void DTEPEditDlg::readGeneral()
{
  m_config->setGroup("General");
  nameEdit->setText(m_config->readEntry("Name"));
  nickNameEdit->setText(m_config->readEntry("NickName"));
  m_family = m_config->readNumEntry("Family", 1) - 1;
  typeCombo->setCurrentItem(m_family);
  rulesWidgetStack->raiseWidget(m_family);
  generalWidgetStack->raiseWidget(m_family);
  caseSensitive->setChecked(m_config->readBoolEntry("CaseSensitive", false));
  QString inherits = m_config->readEntry("Inherits");
  QStringList lst(DTDs::ref()->nameList());
  inheritsCombo->insertStringList(lst);
  int idx = lst.findIndex(inherits);
  if (idx == -1)
    inheritsCombo->insertItem(inherits, 0);
  else
    inheritsCombo->setCurrentItem(idx);
  
  urlEdit->setText(m_config->readEntry("URL"));
  doctypeEdit->setText(m_config->readEntry("DoctypeString"));
  topLevel->setChecked(m_config->readBoolEntry("TopLevel", false));
  
  m_config->setGroup("Toolbars");
  toolbarFolderEdit->setText(m_config->readPathEntry("Location"));
  toolbarsEdit->setText(m_config->readEntry("Names"));
   
}

void DTEPEditDlg::readPages()
{
  int i = 1; 
  while (m_config->hasGroup(QString("Page%1").arg(i)) && i < 6)
  {
    m_config->setGroup(QString("Page%1").arg(i));
    QString title = m_config->readEntry("Title");
    QString groups = m_config->readEntry("Groups");
    if (i == 1)
    {
      enablePage1->setChecked(true);
      pageTitleEdit1->setText(title);
      groupsEdit1->setText(groups);
    } else
    if (i == 2)
    {
      enablePage2->setChecked(true);
      pageTitleEdit2->setText(title);
      groupsEdit2->setText(groups);
    } else
    if (i == 3)
    {
      enablePage3->setChecked(true);
      pageTitleEdit3->setText(title);
      groupsEdit3->setText(groups);
    } else
    if (i == 4)
    {
      enablePage4->setChecked(true);
      pageTitleEdit4->setText(title);
      groupsEdit4->setText(groups);
    } else
    if (i == 5)
    {
      enablePage5->setChecked(true);
      pageTitleEdit5->setText(title);
      groupsEdit5->setText(groups);
    } 
    i++;
  }
}

KConfig *DTEPEditDlg::resultConfig()
{
  return m_config;
}

#include "dtepeditdlg.moc"
