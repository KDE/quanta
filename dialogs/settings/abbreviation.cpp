/***************************************************************************
                          abbreviation.cpp  -  description
                             -------------------
    copyright            : (C) 2003 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/

//qt includes
#include <qdom.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qlineedit.h>
#include <qtextedit.h>
#include <qtextstream.h>
#include <qtimer.h>

//kde includes
#include <kcombobox.h>
#include <kdebug.h>
#include <kdialogbase.h>
#include <kglobal.h>
#include <kinputdialog.h>
#include <klistbox.h>
#include <klistview.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kstandarddirs.h>

//app includes
#include "abbreviation.h"
#include "codetemplatedlgs.h"
#include "resource.h"
#include "quantacommon.h"
#include "dtds.h"


AbbreviationDlg::AbbreviationDlg(QWidget *parent, const char *name):
    AbbreviationDlgS(parent, name)
{
  m_dtd = 0L;
  oldItem = 0L;
  currentAbbrev = 0L;

  QMapConstIterator<QString, Abbreviation> it;
  for (it = qConfig.abbreviations.constBegin(); it != qConfig.abbreviations.constEnd(); ++it)
  {
    groupCombo->insertItem(it.key());
  }
  slotGroupChanged(groupCombo->currentText());
}

AbbreviationDlg::~AbbreviationDlg()
{
}

void AbbreviationDlg::slotGroupChanged(const QString& newGroupName)
{
  //save the current template
  QListViewItem *item = templatesList->currentItem();
  if (item && currentAbbrev)
     currentAbbrev->abbreviations.insert(item->text(0) + " " + item->text(1), codeEdit->text());
  dtepList->clear();
  templatesList->clear();
  codeEdit->clear();
  oldItem = 0L;
  currentAbbrev = &qConfig.abbreviations[newGroupName];
  QString templateStr;
  QString templateName;
  QMap<QString, QString>::ConstIterator it;
  for ( it = currentAbbrev->abbreviations.constBegin(); it != currentAbbrev->abbreviations.constEnd(); ++it )
  {
    templateStr = it.key();
    templateName = templateStr.mid(templateStr.find(' ') + 1);
    templateStr = templateStr.left(templateStr.find(' '));
    new KListViewItem(templatesList, templateStr, templateName);
  }
  for ( QStringList::ConstIterator dtepit = currentAbbrev->dteps.constBegin(); dtepit != currentAbbrev->dteps.constEnd(); ++dtepit )
  {
      dtepList->insertItem(DTDs::ref()->getDTDNickNameFromName(*dtepit));
  }
  templatesList->sort();
  templatesList->setCurrentItem(templatesList->firstChild());
  templatesList->setSelected(templatesList->firstChild(), true);
}

void AbbreviationDlg::slotNewGroup()
{
   bool ok;
   QString groupName = KInputDialog::getText(i18n("New Abbreviation Group"), i18n("Group name:"), "", &ok, this);
   if (ok && !groupName.isEmpty())
   {
      if (qConfig.abbreviations.contains(groupName))
      {
          KMessageBox::error(this, i18n("<qt>There is already an abbreviation group called <b>%1</b>. Choose an unique name for the new group.</qt>").arg(groupName), i18n("Group already exists"));
          QTimer::singleShot(0, this, SLOT(slotNewGroup()));
      } else
      {
         groupCombo->insertItem(groupName);
         groupCombo->setCurrentItem(groupCombo->count()-1);
         slotGroupChanged(groupName);
      }
   }
}

void AbbreviationDlg::slotAddDTEP()
{
   QStringList lst = DTDs::ref()->nickNameList(false);
   for (uint i = 0; i < dtepList->count(); i++)
     lst.remove(dtepList->text(i));
  bool ok = false;
  QString res = KInputDialog::getItem(
                  i18n( "Add DTEP" ),
                  i18n( "Select a DTEP:" ), lst, 0, false, &ok, this );
  if (ok)
  {
      dtepList->insertItem(res);
      currentAbbrev->dteps.append(DTDs::ref()->getDTDNameFromNickName(res));
  }
}

void AbbreviationDlg::slotRemoveDTEP()
{
  int currentItem = dtepList->currentItem();
  if (currentItem == -1)
  {
    KMessageBox::error(this, i18n("<qt>Select a DTEP from the list before using <b>Remove</b>.</qt>"), i18n("No DTEP Selected"));
  } else
  {
    currentAbbrev->dteps.remove(DTDs::ref()->getDTDNameFromNickName(dtepList->currentText()));
    dtepList->removeItem(currentItem);
  }
}

void AbbreviationDlg::slotTemplateSelectionChanged(QListViewItem* item)
{
  if (!item)
      return;
  //QListViewItem *oldItem = templatesList->currentItem();
  if (oldItem)
  {
    QString key = oldItem->text(0)+" " +oldItem->text(1);
    currentAbbrev->abbreviations.insert(key, codeEdit->text());
  }

  QString code = currentAbbrev->abbreviations[item->text(0)+" " +item->text(1)];
  codeEdit->setText(code);
  oldItem = item;
}

void AbbreviationDlg::slotAddTemplate()
{
  KDialogBase dlg(this, 0, true, i18n("Add Code Template"), KDialogBase::Ok | KDialogBase::Cancel);
  CodeTemplateDlgS w(&dlg);
  dlg.setMainWidget(&w);
  w.templateEdit->setFocus();
  if (dlg.exec())
  {
    QString templateName = w.templateEdit->text();
    QString description = w.descriptionEdit->text();
    if (!templateName.isEmpty())
    {
      QListViewItem *item = new KListViewItem(templatesList, templateName, description);
      templatesList->setCurrentItem(item);
      templatesList->setSelected(item, true);
      templatesList->ensureItemVisible(item);
      codeEdit->setFocus();
    }
  }
}

void AbbreviationDlg::slotRemoveTemplate()
{
  QListViewItem *item = templatesList->currentItem();
  if (item &&
      KMessageBox::questionYesNo(this, i18n("<qt>Do you really want to remove the <b>%1</b> template?</qt>").arg(item->text(1))) == KMessageBox::Yes)
  {
    currentAbbrev->abbreviations.remove(item->text(0)+" "+item->text(1));
    delete item;
    oldItem = 0L;
    slotTemplateSelectionChanged(templatesList->currentItem());
  }
}

void AbbreviationDlg::slotEditTemplate()
{
  QListViewItem *item = templatesList->currentItem();
  if (!item)
      return;
  KDialogBase dlg(this, 0, true, i18n("Edit Code Template"), KDialogBase::Ok | KDialogBase::Cancel);
  CodeTemplateDlgS w(&dlg);
  dlg.setMainWidget(&w);
  w.templateEdit->setText(item->text(0));
  w.descriptionEdit->setText(item->text(1));
  if (dlg.exec())
  {
    currentAbbrev->abbreviations.remove(item->text(0)+" "+item->text(1));
    item->setText(0, w.templateEdit->text());
    item->setText(1, w.descriptionEdit->text());
    currentAbbrev->abbreviations.insert(item->text(0) + " " + item->text(1), codeEdit->text());
  }
}


void AbbreviationDlg::saveTemplates()
{
  QListViewItem *item = templatesList->currentItem();
  if (item && currentAbbrev)
     currentAbbrev->abbreviations.insert(item->text(0) + " " + item->text(1), codeEdit->text());
  QString abbrevFile = KGlobal::dirs()->saveLocation("data") + resourceDir + "abbreviations.xml";
  QString s = "<!DOCTYPE Templates>\n<Templates>\n</Templates>\n";
  QString s2;
  QDomDocument doc;
  doc.setContent(s);
  QDomNode firstNode = doc.firstChild();
  Abbreviation abbrev;
  QMap<QString, Abbreviation>::ConstIterator abbrevIt;
  for (abbrevIt = qConfig.abbreviations.constBegin(); abbrevIt != qConfig.abbreviations.constEnd(); ++abbrevIt)
  {
    QDomElement groupEl = doc.createElement("Group");
    groupEl.setAttribute("name", abbrevIt.key());
    abbrev = abbrevIt.data();
    for (QStringList::ConstIterator dtepIt = abbrev.dteps.constBegin(); dtepIt != abbrev.dteps.constEnd(); ++dtepIt)
    {
      QDomElement el = doc.createElement("DTEP");
      el.setAttribute("name", *dtepIt);
      groupEl.appendChild(el);
    }
    int pos;
    QMap<QString,QString>::ConstIterator it;
    for (it = abbrev.abbreviations.constBegin(); it != abbrev.abbreviations.constEnd(); ++it)
    {
      QDomElement el = doc.createElement("Template");
      el.setAttribute("code", it.data());
      s = it.key();
      s2 = "";
      pos = s.find(' ');
      if (pos != -1)
      {
        s2 = s.mid(pos+1);
        s = s.left(pos);
      }
      el.setAttribute("name", s);
      el.setAttribute("description", s2);
      groupEl.appendChild(el);
    }
    firstNode.appendChild(groupEl);
  }

  QFile f(abbrevFile);
  if (f.open(IO_WriteOnly | IO_Truncate))
  {
    QTextStream str(&f);
    str.setEncoding(QTextStream::UnicodeUTF8);
    str << doc.toString();
    f.close();
  } else
  {
    KMessageBox::error(this, i18n("<qt>Cannot open the file <b>%1</b> for writing.\nModified abbreviations will be lost when you quit Quanta.</qt>").arg(s));
  }
}

#include "abbreviation.moc"
