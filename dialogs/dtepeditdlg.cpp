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
#include <qfile.h>
#include <qfileinfo.h>
#include <qtabwidget.h>
#include <qwidgetstack.h>


//kde includes
#include <kcombobox.h>
#include <kconfig.h>
#include <kfiledialog.h>
#include <klineedit.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kstandarddirs.h>

//own includes
#include "dtepeditdlg.h"
#include "dtds.h"
#include "resource.h"

DTEPEditDlg::DTEPEditDlg(const QString& descriptionFile, QWidget *parent, const char *name)
 : DTEPEditDlgS(parent, name)
{
  nameEdit->setFocus();
  m_descriptionFile = descriptionFile;
  
  m_config = new KConfig(m_descriptionFile, false, false);
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

void DTEPEditDlg::saveResult()
{
  QString targetFile = m_descriptionFile;
  QFileInfo f(targetFile);
  if (!f.isWritable())
  {
    if (f.exists())
    {
      if (KMessageBox::questionYesNo(this, i18n("<qt>The file <b>%1</b> is not writable.<br>Do you want to save the configuration to a different file?</qt>").arg(f.filePath()),i18n("Save As")) == KMessageBox::Yes)
      {
        targetFile = KFileDialog::getSaveFileName(locateLocal("data", resourceDir + "dtep/"), i18n("*.rc|DTEP description"), this, i18n("Save Description As"));
      } else
        targetFile = "";
    }
  }
  if (!targetFile.isEmpty())
  {
    QFile f(targetFile);
    if (f.exists())
      f.remove();
    KConfig* newConfig = m_config->copyTo(targetFile);
    newConfig->sync();
    writeGeneral(newConfig);
    writePages(newConfig);
    newConfig->sync();
    delete newConfig;        
  }
}

void DTEPEditDlg::writeGeneral(KConfig *config)
{  
  config->setGroup("General");
  config->writeEntry("Name", nameEdit->text());
  config->writeEntry("NickName", nickNameEdit->text());
  config->writeEntry("Family", typeCombo->currentItem() + 1);
  config->writeEntry("CaseSensitive", caseSensitive->isChecked());
  config->writeEntry("Inherits", inheritsCombo->currentText());
  config->writeEntry("URL", urlEdit->text());
  config->writeEntry("DoctypeString", doctypeEdit->text());
  if (m_family == 1)
    config->writeEntry("TopLevel", topLevel->isChecked());
  
  config->setGroup("Toolbars");
  config->writeEntry("Location", toolbarFolderEdit->text());
  config->writeEntry("Names", toolbarsEdit->text());
}

void DTEPEditDlg::writePages(KConfig *config)
{
  if (m_family == 1)
    config->deleteGroup("Pages");
  else
  {
    int num = 0;
    if (enablePage1->isChecked())
    {
      num++;
      config->setGroup(QString("Page%1").arg(num));
      config->writeEntry("Title", pageTitleEdit1->text());
      config->writeEntry("Group", groupsEdit1->text());
    }
    if (enablePage2->isChecked())
    {
      num++;
      config->setGroup(QString("Page%1").arg(num));
      config->writeEntry("Title", pageTitleEdit2->text());
      config->writeEntry("Group", groupsEdit2->text());
    }
    if (enablePage3->isChecked())
    {
      num++;
      config->setGroup(QString("Page%1").arg(num));
      config->writeEntry("Title", pageTitleEdit3->text());
      config->writeEntry("Group", groupsEdit3->text());
    }
    if (enablePage4->isChecked())
    {
      num++;
      config->setGroup(QString("Page%1").arg(num));
      config->writeEntry("Title", pageTitleEdit4->text());
      config->writeEntry("Group", groupsEdit4->text());
    }
    if (enablePage5->isChecked())
    {
      num++;
      config->setGroup(QString("Page%1").arg(num));
      config->writeEntry("Title", pageTitleEdit5->text());
      config->writeEntry("Group", groupsEdit5->text());
    }
    config->setGroup("General");
    config->writeEntry("NumOfPages", num);
  }
}

#include "dtepeditdlg.moc"
