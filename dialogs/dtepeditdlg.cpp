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

#include <qcheckbox.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qgroupbox.h>
#include <qradiobutton.h>
#include <qtabwidget.h>
#include <qwidgetstack.h>


//kde includes
#include <kcombobox.h>
#include <kconfig.h>
#include <kfiledialog.h>
#include <kicondialog.h>
#include <klistbox.h>
#include <klineedit.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kstandarddirs.h>

//own includes
#include "dtepeditdlg.h"
#include "dtepstructureeditdlgs.h"
#include "dtds.h"
#include "resource.h"
#include "quantacommon.h"

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
  if (family == 0 && m_family != 0)
    tabWidget->insertTab(m_pagesWidget, i18n("&Pages"), 1);
  else
  if (family == 1)
   tabWidget->removePage(m_pagesWidget);
  m_family = family;
}

void DTEPEditDlg::init()
{
  m_pagesWidget = tabWidget->page(1);
  readGeneral();
  if (m_family == 0)
    readPages();
  else
    tabWidget->removePage(m_pagesWidget);
  readParserRules();
  readStructures();
}

void DTEPEditDlg::readGeneral()
{
  m_config->setGroup("General");
  nameEdit->setText(m_config->readEntry("Name"));
  nickNameEdit->setText(m_config->readEntry("NickName"));
  m_family = m_config->readNumEntry("Family", 1) - 1;
  extensionEdit->setText(m_config->readEntry("DefaultExtension"));
  mimeTypeEdit->setText(m_config->readEntry("MimeTypes"));
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
  toolbarFolderEdit->setText(QuantaCommon::readPathEntry(m_config, "Location"));
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

void DTEPEditDlg::readParserRules()
{
  m_config->setGroup("Extra rules");
  enableMinusInWords->setChecked(m_config->readBoolEntry("MinusAllowedInWord", false));
  attributeSeparatorEdit->setText(m_config->readEntry("AttributeSeparator"));
  tagSeparatorEdit->setText(m_config->readEntry("TagSeparator"));
  tagAfterEdit->setText(m_config->readEntry("TagAutoCompleteAfter"));

  if (m_family == 0)
  {
    extendedBooleans->setChecked(m_config->readEntry("BooleanAttributes", "extended") == "extended");
    trueEdit->setText(m_config->readEntry("BooleanTrue","true"));
    falseEdit->setText(m_config->readEntry("BooleanFalse","false"));
    xmlStyleTags->setChecked(m_config->readEntry("Single Tag Style", "xml").lower() == "xml");
  } else
  {
    attributesAfterEdit->setText(m_config->readEntry("AttributeAutoCompleteAfter"));
    membersAfterEdit->setText(m_config->readEntry("MemberAutoCompleteAfter"));
  }

  m_config->setGroup("Parsing rules");
  commentsEdit->setText(m_config->readEntry("Comments"));
  mayContainEdit->setText(m_config->readEntry("MayContain"));

  if (m_family == 0)
  {
    specialAreasEdit->setText(m_config->readEntry("SpecialAreas"));
    specialAreaNamesEdit->setText(m_config->readEntry("SpecialAreaNames"));
    specialTagsEdit->setText(m_config->readEntry("SpecialTags"));
    useCommonRules->setChecked(m_config->readBoolEntry("AppendCommonSpecialAreas", true));
  } else
  {
    areaBordersEdit->setText(m_config->readEntry("AreaBorders"));
    definitionTagsEdit->setText(m_config->readEntry("Tags"));
    structKeywordsEdit->setText(m_config->readEntry("StructKeywords"));
    localStructKeywordsEdit->setText(m_config->readEntry("LocalScopeKeywords"));
    structBeginEdit->setText(m_config->readEntry("StructBeginStr"));
    structEndEdit->setText(m_config->readEntry("StructEndStr"));
    structRxEdit->setText(m_config->readEntry("StructRx"));
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
      if (KMessageBox::questionYesNo(this, i18n("<qt>The file <b>%1</b> is not writable.<br>Do you want to save the configuration to a different file?</qt>").arg(f.filePath()),i18n("Save As"),i18n("Save to Different File"), i18n("Do Not Save")) == KMessageBox::Yes)
      {
        targetFile = KFileDialog::getSaveFileName(locateLocal("data", resourceDir + "dtep/description.rc"), i18n("*.rc|DTEP Description"), this, i18n("Save Description As"));
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
  config->setGroup("General");
  writeEntry(config, "Name", nameEdit->text());
  writeEntry(config, "NickName", nickNameEdit->text());
  config->writeEntry("Family", typeCombo->currentItem() + 1);
  config->writeEntry("CaseSensitive", caseSensitive->isChecked());
  writeEntry(config, "Inherits", inheritsCombo->currentText());
  writeEntry(config, "URL", urlEdit->text());
  writeEntry(config, "DoctypeString", doctypeEdit->text());
  writeEntry(config, "DefaultExtension", extensionEdit->text());
  writeEntry(config, "MimeTypes", mimeTypeEdit->text());
  if (m_family == 1)
    config->writeEntry("TopLevel", topLevel->isChecked());

  config->setGroup("Toolbars");
  writeEntry(config, "Location", toolbarFolderEdit->text());
  writeEntry(config, "Names", toolbarsEdit->text());
}

void DTEPEditDlg::writePages(KConfig *config)
{
  config->deleteGroup("Pages");
  if (m_family == 0)
  {
    int num = 0;
    if (enablePage1->isChecked())
    {
      num++;
      config->setGroup(QString("Page%1").arg(num));
      writeEntry(config, "Title", pageTitleEdit1->text());
      writeEntry(config, "Groups", groupsEdit1->text());
    }
    if (enablePage2->isChecked())
    {
      num++;
      config->setGroup(QString("Page%1").arg(num));
      writeEntry(config, "Title", pageTitleEdit2->text());
      writeEntry(config, "Groups", groupsEdit2->text());
    }
    if (enablePage3->isChecked())
    {
      num++;
      config->setGroup(QString("Page%1").arg(num));
      writeEntry(config, "Title", pageTitleEdit3->text());
      writeEntry(config, "Groups", groupsEdit3->text());
    }
    if (enablePage4->isChecked())
    {
      num++;
      config->setGroup(QString("Page%1").arg(num));
      writeEntry(config, "Title", pageTitleEdit4->text());
      writeEntry(config, "Groups", groupsEdit4->text());
    }
    if (enablePage5->isChecked())
    {
      num++;
      config->setGroup(QString("Page%1").arg(num));
      writeEntry(config, "Title", pageTitleEdit5->text());
      writeEntry(config, "Groups", groupsEdit5->text());
    }
    config->setGroup("General");
    config->writeEntry("NumOfPages", num);
  }
}

void DTEPEditDlg::writeParserRules(KConfig *config)
{
  config->setGroup("Extra rules");
  config->writeEntry("MinusAllowedInWord", enableMinusInWords->isChecked());
  writeEntry(config, "AttributeSeparator", attributeSeparatorEdit->text());
  writeEntry(config, "TagSeparator", tagSeparatorEdit->text());
  writeEntry(config, "TagAutoCompleteAfter", tagAfterEdit->text());

  if (m_family == 0)
  {
    writeEntry(config, "BooleanAttributes", extendedBooleans->isChecked() ? "extended" : "simple");
    writeEntry(config, "BooleanTrue", trueEdit->text());
    writeEntry(config, "BooleanFalse", falseEdit->text());
    writeEntry(config, "Single Tag Style", xmlStyleTags->isChecked() ? "xml" : "html");
  } else
  {
    writeEntry(config, "AttributeAutoCompleteAfter", attributesAfterEdit->text());
    writeEntry(config, "MemberAutoCompleteAfter", membersAfterEdit->text());
  }

  config->setGroup("Parsing rules");
  writeEntry(config, "Comments", commentsEdit->text());
  writeEntry(config, "MayContain", mayContainEdit->text());

  if (m_family == 0)
  {
    writeEntry(config, "SpecialAreas", specialAreasEdit->text());
    writeEntry(config, "SpecialAreaNames", specialAreaNamesEdit->text());
    writeEntry(config, "SpecialTags", specialTagsEdit->text());
    config->writeEntry("AppendCommonSpecialAreas", useCommonRules->isChecked());
  } else
  {
    writeEntry(config, "AreaBorders", areaBordersEdit->text());
    writeEntry(config, "Tags", definitionTagsEdit->text());
    writeEntry(config, "StructKeywords", structKeywordsEdit->text());
    writeEntry(config, "LocalScopeKeywords", localStructKeywordsEdit->text());
    writeEntry(config, "StructBeginStr", structBeginEdit->text());
    writeEntry(config, "StructEndStr", structEndEdit->text());
    writeEntry(config, "StructRx", structRxEdit->text());
  }
}

void DTEPEditDlg::readStructures()
{
  m_config->setGroup("Extra rules");
  int variableGroupId = m_config->readNumEntry("VariableGroupIndex", -1);
  int functionGroupId = m_config->readNumEntry("FunctionGroupIndex", -1);
  int classGroupId = m_config->readNumEntry("ClassGroupIndex", -1);
  int objectGroupId = m_config->readNumEntry("ObjectGroupIndex", -1);

  int i = 1;
  while (m_config->hasGroup(QString("StructGroup_%1").arg(i)))
  {
    StructGroup group;
    m_config->setGroup(QString("StructGroup_%1").arg(i));
    group.name = m_config->readEntry("Name");
    group.noName = m_config->readEntry("No_Name");
    group.icon = m_config->readEntry("Icon");
    group.tag = m_config->readEntry("Tag");
    group.hasFileName = m_config->readBoolEntry("HasFileName", false);
    group.fileNameRx = m_config->readEntry("FileNameRx");
    group.appendToTags = m_config->readBoolEntry("AppendToTags", false);
    group.parentGroup = m_config->readEntry("ParentGroup");
    group.definitionRx = m_config->readEntry("DefinitionRx");
    group.definitionRxMinimal = m_config->readBoolEntry("DefinitionRx_Minimal", false);
    group.usageRx = m_config->readEntry("UsageRx");
    group.typeRx = m_config->readEntry("TypeRx");
    group.tagType = m_config->readEntry("TagType");
    group.parseFile = m_config->readBoolEntry("ParseFile", false);
    group.completeAfterRx = m_config->readEntry("AutoCompleteAfter");
    group.removeRx = m_config->readEntry("RemoveFromAutoCompleteWord");
    group.variableGroup = (variableGroupId == i);
    group.functionGroup = (functionGroupId == i);
    group.classGroup = (classGroupId == i);
    group.objectGroup = (objectGroupId == i);
    group.simpleGroup = (!group.variableGroup && !group.functionGroup && !group.classGroup && !group.objectGroup);

    m_structGroups.append(group);
    i++;
  }
  for (QValueList<StructGroup>::ConstIterator it = m_structGroups.constBegin(); it != m_structGroups.constEnd(); ++it)
  {
    structuresList->insertItem((*it).name);
  }
}

void DTEPEditDlg::writeStructures(KConfig *config)
{
  config->setGroup("Extra rules");
  config->writeEntry("StructGroupsCount", m_structGroups.count());

  int i = 1;
  for (QValueList<StructGroup>::ConstIterator it = m_structGroups.constBegin(); it != m_structGroups.constEnd(); ++it)
  {
    StructGroup group = *it;
    config->setGroup(QString("StructGroup_%1").arg(i));
    writeEntry(config, "Name", group.name);
    writeEntry(config, "No_Name", group.noName);
    writeEntry(config, "Icon", group.icon);
    writeEntry(config, "Tag", group.tag);
    config->writeEntry("HasFileName", group.hasFileName);
    writeEntry(config, "FileNameRx", group.fileNameRx);
    config->writeEntry("AppendToTags", group.appendToTags);
    if (m_family == 1)
    {
      writeEntry(config, "ParentGroup", group.parentGroup);
      writeEntry(config, "DefinitionRx", group.definitionRx);
      config->writeEntry("DefinitionRx_Minimal", group.definitionRxMinimal);
      writeEntry(config, "UsageRx", group.usageRx);
      writeEntry(config, "TypeRx", group.typeRx);
      writeEntry(config, "TagType", group.tagType);
      config->writeEntry("ParseFile", group.parseFile);
      writeEntry(config, "AutoCompleteAfter", group.completeAfterRx);
      writeEntry(config, "RemoveFromAutoCompleteWord", group.removeRx);
      if (group.variableGroup)
      {
        config->setGroup("Extra rules");
        config->writeEntry("VariableGroupIndex", i);
      } else
      if (group.functionGroup)
      {
        config->setGroup("Extra rules");
        config->writeEntry("FunctionGroupIndex", i);
      } else
      if (group.classGroup)
      {
        config->setGroup("Extra rules");
        config->writeEntry("ClassGroupIndex", i);
      } else
      if (group.objectGroup)
      {
        config->setGroup("Extra rules");
        config->writeEntry("ObjectGroupIndex", i);
      }
    }

    i++;
  }
  for (QValueList<StructGroup>::ConstIterator it = m_structGroups.constBegin(); it != m_structGroups.constEnd(); ++it)
  {
    structuresList->insertItem((*it).name);
  }
}

void DTEPEditDlg::slotEditStructGroup()
{
  int currentItem = structuresList->currentItem();
  if (currentItem != -1)
  {
    KDialogBase editDlg(this, "edit_group", true, i18n("Edit Structure Group"), KDialogBase::Ok | KDialogBase::Cancel);
    DTEPStructureEditDlgS structDlg(&editDlg);
    editDlg.setMainWidget(&structDlg);

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
      if (structDlg.tagTypeCombo->text(i) == group.tagType)
      {
        structDlg.tagTypeCombo->setCurrentItem(i);
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
  KDialogBase editDlg(this, "edit_group", true, i18n("Add Structure Group"), KDialogBase::Ok | KDialogBase::Cancel);
  DTEPStructureEditDlgS structDlg(&editDlg);
  editDlg.setMainWidget(&structDlg);
  if (editDlg.exec())
  {
    StructGroup group = readFromStructDlg(&structDlg);
    m_structGroups.append(group);
    structuresList->insertItem(group.name);
  }
}

StructGroup DTEPEditDlg::readFromStructDlg(DTEPStructureEditDlgS *structDlg)
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
      if (KMessageBox::warningContinueCancel(this, i18n("<qt>Do you really want to delete the <b>%1</b> group?</qt>").arg(structuresList->currentText()), i18n("Delete Group"),KStdGuiItem::del()) == KMessageBox::Continue)
    {
      m_structGroups.remove(m_structGroups.at(currentItem));
      structuresList->removeItem(currentItem);
    }
  }
}

void DTEPEditDlg::writeEntry(KConfig *config, const QString &key, const QString &value)
{
  if (value.isEmpty())
    config->deleteEntry(key);
  else
    config->writeEntry(key, value);
}


#include "dtepeditdlg.moc"
