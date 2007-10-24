/***************************************************************************
                          dtepeditdlg.cpp  -  description
                             -------------------
    begin                : Tue Feb 1 11:07:24 2005
    copyright            : (C) 2005 Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/
//own includes
#include "dtepeditdlg.h"
#include "ui_dtepstructureeditdlgs.h"
#include "dtds.h"
#include "helper.h"


#include <QCheckBox>
#include <QFile>
#include <QFileInfo>
#include <QGroupBox>
#include <QRadioButton>
#include <QTabWidget>
#include <qstackedwidget.h>


//kde includes
#include <kcombobox.h>
#include <kconfig.h>
#include <kfiledialog.h>
#include <kicondialog.h>
#include <k3listbox.h>
#include <klineedit.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kstandarddirs.h>



DTEPEditDlg::DTEPEditDlg(const QString& descriptionFile, QWidget *parent)
 : QWidget(parent)
{
  setupUi(this);
  connect(addStructButton, SIGNAL(clicked()), SLOT(slotAddStructGroup()));
  connect(deleteStructButton, SIGNAL(clicked()), SLOT(slotDeleteStructGroup()));
  connect(editStructButton, SIGNAL(clicked()), SLOT(slotEditStructGroup()));
  connect(typeCombo, SIGNAL(activated(int)), SLOT(slotFamilyChanged(int)));
  nameEdit->setFocus();
  m_descriptionFile = descriptionFile;

  m_config = new KConfig(m_descriptionFile, KConfig::NoGlobals );
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
    tabWidget->insertTab(1, m_pagesWidget, i18n("&Pages"));
  else
    tabWidget->removeTab(tabWidget->indexOf(m_pagesWidget));
}

void DTEPEditDlg::init()
{
  m_pagesWidget = tabWidget->widget(1);
  readGeneral();
  if (m_family == 0)
    readPages();
  else
    tabWidget->removeTab(tabWidget->indexOf(m_pagesWidget));
  readParserRules();
  readStructures();
}

void DTEPEditDlg::readGeneral()
{
  KConfigGroup grp(m_config, "General");
  nameEdit->setText(grp.readEntry("Name"));
  nickNameEdit->setText(grp.readEntry("NickName"));
  m_family = grp.readEntry("Family", 1) - 1;
  extensionEdit->setText(grp.readEntry("DefaultExtension"));
  mimeTypeEdit->setText(grp.readEntry("MimeTypes"));
  typeCombo->setCurrentIndex(m_family);
  rulesWidgetStack->setCurrentIndex (m_family);
  generalWidgetStack->setCurrentIndex (m_family);
  caseSensitive->setChecked(grp.readEntry("CaseSensitive", false));
  QString inherits = grp.readEntry("Inherits");
  QStringList lst(DTDs::ref()->nameList());
  inheritsCombo->addItems(lst);
  int idx = lst.indexOf(inherits);
  if (idx == -1)
    inheritsCombo->insertItem(0, inherits);
  else
    inheritsCombo->setCurrentIndex(idx);

  urlEdit->setText(grp.readEntry("URL"));
  doctypeEdit->setText(grp.readEntry("DoctypeString"));
  topLevel->setChecked(grp.readEntry("TopLevel", false));

  grp.changeGroup("Toolbars");
  toolbarFolderEdit->setText(grp.readPathEntry("Location"));
  toolbarsEdit->setText(grp.readEntry("Names"));

}

void DTEPEditDlg::readPages()
{
  int i = 1;
  while (m_config->hasGroup(QString("Page%1").arg(i)) && i < 6)
  {
    KConfigGroup grp(m_config, QString("Page%1").arg(i));
    QString title = grp.readEntry("Title");
    QString groups = grp.readEntry("Groups");
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

void DTEPEditDlg::readParserRules()
{
  KConfigGroup grp(m_config, "Extra rules");
  enableMinusInWords->setChecked(grp.readEntry("MinusAllowedInWord", false));
  attributeSeparatorEdit->setText(grp.readEntry("AttributeSeparator"));
  tagSeparatorEdit->setText(grp.readEntry("TagSeparator"));
  tagAfterEdit->setText(grp.readEntry("TagAutoCompleteAfter"));

  if (m_family == 0)
  {
    extendedBooleans->setChecked(grp.readEntry("BooleanAttributes", "extended") == "extended");
    trueEdit->setText(grp.readEntry("BooleanTrue","true"));
    falseEdit->setText(grp.readEntry("BooleanFalse","false"));
    xmlStyleTags->setChecked(grp.readEntry("Single Tag Style", "xml").toLower() == "xml");
  } else
  {
    attributesAfterEdit->setText(grp.readEntry("AttributeAutoCompleteAfter"));
    membersAfterEdit->setText(grp.readEntry("MemberAutoCompleteAfter"));
  }

  grp.changeGroup("Parsing rules");
  commentsEdit->setText(grp.readEntry("Comments"));
  mayContainEdit->setText(grp.readEntry("MayContain"));

  if (m_family == 0)
  {
    specialAreasEdit->setText(grp.readEntry("SpecialAreas"));
    specialAreaNamesEdit->setText(grp.readEntry("SpecialAreaNames"));
    specialTagsEdit->setText(grp.readEntry("SpecialTags"));
    useCommonRules->setChecked(grp.readEntry("AppendCommonSpecialAreas", true));
  } else
  {
    areaBordersEdit->setText(grp.readEntry("AreaBorders"));
    definitionTagsEdit->setText(grp.readEntry("Tags"));
    structKeywordsEdit->setText(grp.readEntry("StructKeywords"));
    localStructKeywordsEdit->setText(grp.readEntry("LocalScopeKeywords"));
    structBeginEdit->setText(grp.readEntry("StructBeginStr"));
    structEndEdit->setText(grp.readEntry("StructEndStr"));
    structRxEdit->setText(grp.readEntry("StructRx"));
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
      if (KMessageBox::questionYesNo(this, i18n("<qt>The file <b>%1</b> is not writable.<br />Do you want to save the configuration to a different file?</qt>", f.filePath()),i18n("Save As")) == KMessageBox::Yes)
      {
        targetFile = KFileDialog::getSaveFileName(KStandardDirs::locateLocal("data", Helper::resourceDir() + "dtep/description.rc"), i18n("*.rc|DTEP Description"), this, i18n("Save Description As"));
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
    writeParserRules(newConfig);
    writeStructures(newConfig);
    newConfig->sync();
    delete newConfig;
  }
}

void DTEPEditDlg::writeGeneral(KConfig *config)
{
  KConfigGroup grp(config, "General");
  grp.writeEntry("Name", nameEdit->text());
  grp.writeEntry("NickName", nickNameEdit->text());
  grp.writeEntry("Family", typeCombo->currentIndex() + 1);
  grp.writeEntry("CaseSensitive", caseSensitive->isChecked());
  grp.writeEntry("Inherits", inheritsCombo->currentText());
  grp.writeEntry("URL", urlEdit->text());
  grp.writeEntry("DoctypeString", doctypeEdit->text());
  grp.writeEntry("DefaultExtension", extensionEdit->text());
  grp.writeEntry("MimeTypes", mimeTypeEdit->text());
  if (m_family == 1)
    grp.writeEntry("TopLevel", topLevel->isChecked());

  grp.changeGroup("Toolbars");
  grp.writeEntry("Location", toolbarFolderEdit->text());
  grp.writeEntry("Names", toolbarsEdit->text());
}

void DTEPEditDlg::writePages(KConfig *config)
{
  KConfigGroup grp(config, "Pages");
  config->deleteGroup("Pages");
  if (m_family == 0)
  {
    int num = 0;
    if (enablePage1->isChecked())
    {
      num++;
      grp.changeGroup(QString("Page%1").arg(num));
      grp.writeEntry("Title", pageTitleEdit1->text());
      grp.writeEntry("Groups", groupsEdit1->text());
    }
    if (enablePage2->isChecked())
    {
      num++;
      grp.changeGroup(QString("Page%1").arg(num));
      grp.writeEntry("Title", pageTitleEdit2->text());
      grp.writeEntry("Groups", groupsEdit2->text());
    }
    if (enablePage3->isChecked())
    {
      num++;
      grp.changeGroup(QString("Page%1").arg(num));
      grp.writeEntry("Title", pageTitleEdit3->text());
      grp.writeEntry("Groups", groupsEdit3->text());
    }
    if (enablePage4->isChecked())
    {
      num++;
      grp.changeGroup(QString("Page%1").arg(num));
      grp.writeEntry("Title", pageTitleEdit4->text());
      grp.writeEntry("Groups", groupsEdit4->text());
    }
    if (enablePage5->isChecked())
    {
      num++;
      grp.changeGroup(QString("Page%1").arg(num));
      grp.writeEntry("Title", pageTitleEdit5->text());
      grp.writeEntry("Groups", groupsEdit5->text());
    }
    grp.changeGroup("General");
    grp.writeEntry("NumOfPages", num);
  }
}

void DTEPEditDlg::writeParserRules(KConfig *config)
{
  KConfigGroup grp(config, "Extra rules");
  grp.writeEntry("MinusAllowedInWord", enableMinusInWords->isChecked());
  grp.writeEntry("AttributeSeparator", attributeSeparatorEdit->text());
  grp.writeEntry("TagSeparator", tagSeparatorEdit->text());
  grp.writeEntry("TagAutoCompleteAfter", tagAfterEdit->text());

  if (m_family == 0)
  {
    grp.writeEntry("BooleanAttributes", extendedBooleans->isChecked() ? "extended" : "simple");
    grp.writeEntry("BooleanTrue", trueEdit->text());
    grp.writeEntry("BooleanFalse", falseEdit->text());
    grp.writeEntry("Single Tag Style", xmlStyleTags->isChecked() ? "xml" : "html");
  } else
  {
    grp.writeEntry("AttributeAutoCompleteAfter", attributesAfterEdit->text());
    grp.writeEntry("MemberAutoCompleteAfter", membersAfterEdit->text());
  }

  grp.changeGroup("Parsing rules");
  grp.writeEntry("Comments", commentsEdit->text());
  grp.writeEntry("MayContain", mayContainEdit->text());

  if (m_family == 0)
  {
    grp.writeEntry("SpecialAreas", specialAreasEdit->text());
    grp.writeEntry("SpecialAreaNames", specialAreaNamesEdit->text());
    grp.writeEntry("SpecialTags", specialTagsEdit->text());
    grp.writeEntry("AppendCommonSpecialAreas", useCommonRules->isChecked());
  } else
  {
    grp.writeEntry("AreaBorders", areaBordersEdit->text());
    grp.writeEntry("Tags", definitionTagsEdit->text());
    grp.writeEntry("StructKeywords", structKeywordsEdit->text());
    grp.writeEntry("LocalScopeKeywords", localStructKeywordsEdit->text());
    grp.writeEntry("StructBeginStr", structBeginEdit->text());
    grp.writeEntry("StructEndStr", structEndEdit->text());
    grp.writeEntry("StructRx", structRxEdit->text());
  }
}

void DTEPEditDlg::readStructures()
{
  KConfigGroup grp(m_config, "Extra rules");
  int variableGroupId = grp.readEntry("VariableGroupIndex", -1);
  int functionGroupId = grp.readEntry("FunctionGroupIndex", -1);
  int classGroupId = grp.readEntry("ClassGroupIndex", -1);
  int objectGroupId = grp.readEntry("ObjectGroupIndex", -1);

  int i = 1;
  while (m_config->hasGroup(QString("StructGroup_%1").arg(i)))
  {
    StructGroup group;
    grp.changeGroup(QString("StructGroup_%1").arg(i));
    group.name = grp.readEntry("Name");
    group.noName = grp.readEntry("No_Name");
    group.icon = grp.readEntry("Icon");
    group.tag = grp.readEntry("Tag");
    group.hasFileName = grp.readEntry("HasFileName", false);
    group.fileNameRx = grp.readEntry("FileNameRx");
    group.appendToTags = grp.readEntry("AppendToTags", false);
    group.parentGroup = grp.readEntry("ParentGroup");
    group.definitionRx = grp.readEntry("DefinitionRx");
    group.definitionRxMinimal = grp.readEntry("DefinitionRx_Minimal", false);
    group.usageRx = grp.readEntry("UsageRx");
    group.typeRx = grp.readEntry("TypeRx");
    group.tagType = grp.readEntry("TagType");
    group.parseFile = grp.readEntry("ParseFile", false);
    group.completeAfterRx = grp.readEntry("AutoCompleteAfter");
    group.removeRx = grp.readEntry("RemoveFromAutoCompleteWord");
    group.variableGroup = (variableGroupId == i);
    group.functionGroup = (functionGroupId == i);
    group.classGroup = (classGroupId == i);
    group.objectGroup = (objectGroupId == i);
    group.simpleGroup = (!group.variableGroup && !group.functionGroup && !group.classGroup && !group.objectGroup);

    m_structGroups.append(group);
    i++;
  }
  for (QList<StructGroup>::ConstIterator it = m_structGroups.constBegin(); it != m_structGroups.constEnd(); ++it)
  {
    structuresList->insertItem((*it).name);
  }
}

void DTEPEditDlg::writeStructures(KConfig *config)
{
  KConfigGroup grp(config, "Extra rules");
  grp.writeEntry("StructGroupsCount", m_structGroups.count());

  int i = 1;
  for (QList<StructGroup>::ConstIterator it = m_structGroups.constBegin(); it != m_structGroups.constEnd(); ++it)
  {
    StructGroup group = *it;
    grp.changeGroup(QString("StructGroup_%1").arg(i));
    grp.writeEntry("Name", group.name);
    grp.writeEntry("No_Name", group.noName);
    grp.writeEntry("Icon", group.icon);
    grp.writeEntry("Tag", group.tag);
    grp.writeEntry("HasFileName", group.hasFileName);
    grp.writeEntry("FileNameRx", group.fileNameRx);
    grp.writeEntry("AppendToTags", group.appendToTags);
    if (m_family == 1)
    {
      grp.writeEntry("ParentGroup", group.parentGroup);
      grp.writeEntry("DefinitionRx", group.definitionRx);
      grp.writeEntry("DefinitionRx_Minimal", group.definitionRxMinimal);
      grp.writeEntry("UsageRx", group.usageRx);
      grp.writeEntry("TypeRx", group.typeRx);
      grp.writeEntry("TagType", group.tagType);
      grp.writeEntry("ParseFile", group.parseFile);
      grp.writeEntry("AutoCompleteAfter", group.completeAfterRx);
      grp.writeEntry("RemoveFromAutoCompleteWord", group.removeRx);
      if (group.variableGroup)
      {
        grp.changeGroup("Extra rules");
        grp.writeEntry("VariableGroupIndex", i);
      } else
      if (group.functionGroup)
      {
        grp.changeGroup("Extra rules");
        grp.writeEntry("FunctionGroupIndex", i);
      } else
      if (group.classGroup)
      {
        grp.changeGroup("Extra rules");
        grp.writeEntry("ClassGroupIndex", i);
      } else
      if (group.objectGroup)
      {
        grp.changeGroup("Extra rules");
        grp.writeEntry("ObjectGroupIndex", i);
      }
    }

    i++;
  }
  for (QList<StructGroup>::ConstIterator it = m_structGroups.constBegin(); it != m_structGroups.constEnd(); ++it)
  {
    structuresList->insertItem((*it).name);
  }
}

void DTEPEditDlg::slotEditStructGroup()
{
  int currentItem = structuresList->currentItem();
  if (currentItem != -1)
  {
    KDialog editDlg(this );
    editDlg.setCaption( i18n("Edit Structure Group") );
    editDlg.setButtons( KDialog::Ok | KDialog::Cancel );
    editDlg.setDefaultButton( KDialog::Ok );
    Ui_DTEPStructureEditDlgS structDlg;
    structDlg.setupUi(&editDlg);

    StructGroup group = m_structGroups[currentItem];
    structDlg.nameEdit->setText(group.name);
    structDlg.noNameEdit->setText(group.noName);
    structDlg.iconButton->setIcon(group.icon);
    structDlg.tagEdit->setText(group.tag);
    structDlg.hasFilename->setChecked(group.hasFileName);
    structDlg.fileNameRxEdit->setText(group.fileNameRx);
    structDlg.appendToTags->setChecked(group.appendToTags);
    structDlg.parentGroupEdit->setText(group.parentGroup);
    for (int i = 0; i < structDlg.tagTypeCombo->count(); i++)
    {
      if (structDlg.tagTypeCombo->itemText(i) == group.tagType)
      {
        structDlg.tagTypeCombo->setCurrentIndex(i);
        break;
      }
    }

    structDlg.definitionRxEdit->setText(group.definitionRx);
    structDlg.definitionRxMinimal->setChecked(group.definitionRxMinimal);
    structDlg.usageRxEdit->setText(group.usageRx);
    structDlg.typeRxEdit->setText(group.typeRx);
    structDlg.autoCompleteRxEdit->setText(group.completeAfterRx);
    structDlg.removeRxEdit->setText(group.removeRx);
    structDlg.parseFile->setChecked(group.parseFile);
    structDlg.simpleGroup->setChecked(group.simpleGroup);
    structDlg.variableGroup->setChecked(group.variableGroup);
    structDlg.functionGroup->setChecked(group.functionGroup);
    structDlg.classGroup->setChecked(group.classGroup);
    structDlg.objectGroup->setChecked(group.objectGroup);

    if (m_family == 0)
      structDlg.pseudoGroupBox->setEnabled(false);

    if (editDlg.exec())
    {
      StructGroup group = readFromStructDlg(&structDlg);
      m_structGroups[currentItem] = group;
      structuresList->removeItem(currentItem);
      structuresList->insertItem(group.name, currentItem);
    }
  }
}

void DTEPEditDlg::slotAddStructGroup()
{
  KDialog editDlg(this);
  editDlg.setCaption( i18n("Add Structure Group") );
  editDlg.setButtons(  KDialog::Ok | KDialog::Cancel );
  editDlg.setDefaultButton( KDialog::Ok );
  Ui_DTEPStructureEditDlgS structDlg;
  structDlg.setupUi(&editDlg);
  if (editDlg.exec())
  {
    StructGroup group = readFromStructDlg(&structDlg);
    m_structGroups.append(group);
    structuresList->insertItem(group.name);
  }
}

StructGroup DTEPEditDlg::readFromStructDlg(Ui_DTEPStructureEditDlgS *structDlg)
{
  StructGroup group;

  group.name = structDlg->nameEdit->text();
  group.noName = structDlg->noNameEdit->text();
  group.icon = structDlg->iconButton->icon();
  group.tag = structDlg->tagEdit->text();
  group.hasFileName = structDlg->hasFilename->isChecked();
  group.fileNameRx = structDlg->fileNameRxEdit->text();
  group.appendToTags = structDlg->appendToTags->isChecked();
  group.parentGroup = structDlg->parentGroupEdit->text();
  group.tagType = structDlg->tagTypeCombo->currentText();
  group.definitionRx = structDlg->definitionRxEdit->text();
  group.definitionRxMinimal = structDlg->definitionRxMinimal->isChecked();
  group.usageRx = structDlg->usageRxEdit->text();
  group.typeRx = structDlg->typeRxEdit->text();
  group.completeAfterRx = structDlg->autoCompleteRxEdit->text();
  group.removeRx = structDlg->removeRxEdit->text();
  group.parseFile = structDlg->parseFile->isChecked();
  group.simpleGroup = structDlg->simpleGroup->isChecked();
  group.variableGroup = structDlg->variableGroup->isChecked();
  group.functionGroup = structDlg->functionGroup->isChecked();
  group.classGroup = structDlg->classGroup->isChecked();
  group.objectGroup = structDlg->objectGroup->isChecked();

  return group;
}

void DTEPEditDlg::slotDeleteStructGroup()
{
  int currentItem = structuresList->currentItem();
  if (currentItem != -1)
  {
      if (KMessageBox::warningContinueCancel(this, i18n("<qt>Do you really want to delete the <b>%1</b> group?</qt>", structuresList->currentText()), i18n("Delete Group"),KStandardGuiItem::del()) == KMessageBox::Continue)
    {
      m_structGroups.removeAt(currentItem);
      structuresList->removeItem(currentItem);
    }
  }
}

#include "dtepeditdlg.moc"
