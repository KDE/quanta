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

//kde includes
#include <kcombobox.h>
#include <kdebug.h>
#include <kdialogbase.h>
#include <kglobal.h>
#include <klistview.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kstandarddirs.h>

//app includes
#include "abbreviation.h"
#include "codetemplatedlgs.h"
#include "resource.h"
#include "quantacommon.h"


Abbreviation::Abbreviation(QWidget *parent, const char *name):
    Abbreviations(parent, name)
{
  m_dtd = 0L;
  oldItem = 0L;
}

Abbreviation::~Abbreviation()
{
}

void Abbreviation::slotDTDChanged(const QString& newDTDName)
{
  saveTemplates();
  templatesList->clear();
  codeEdit->clear();
  oldItem = 0L;
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
  templatesList->setCurrentItem(templatesList->firstChild());
  templatesList->setSelected(templatesList->firstChild(), true);
}

void Abbreviation::slotTemplateSelectionChanged(QListViewItem* item)
{
  if (!item)
      return;
  //QListViewItem *oldItem = templatesList->currentItem();
  if (oldItem)
  {
    QString key = oldItem->text(0)+" " +oldItem->text(1);
    m_dtd->abbreviations.insert(key, codeEdit->text());
  }

  QString code = m_dtd->abbreviations[item->text(0)+" " +item->text(1)];
  codeEdit->setText(code);
  oldItem = item;
}

void Abbreviation::slotAddTemplate()
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

void Abbreviation::slotRemoveTemplate()
{
  QListViewItem *item = templatesList->currentItem();
  if (item &&
      KMessageBox::questionYesNo(this, i18n("<qt>Do you really want to remove the <b>%1</b> template?</qt>").arg(item->text(1))) == KMessageBox::Yes)
  {
    m_dtd->abbreviations.remove(item->text(0)+" "+item->text(1));
    delete item;
    oldItem = 0L;
    slotTemplateSelectionChanged(templatesList->currentItem());
  }
}

void Abbreviation::slotEditTemplate()
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
    m_dtd->abbreviations.remove(item->text(0)+" "+item->text(1));
    item->setText(0, w.templateEdit->text());
    item->setText(1, w.descriptionEdit->text());
    m_dtd->abbreviations.insert(item->text(0) + " " + item->text(1), codeEdit->text());
  }
}


void Abbreviation::saveTemplates()
{
  if (!m_dtd)
      return;
  QListViewItem *item = templatesList->currentItem();
  if (item)
     m_dtd->abbreviations.insert(item->text(0) + " " + item->text(1), codeEdit->text());
  QString s = "<!DOCTYPE Templates>\n<Templates>\n</Templates>\n";
  QString s2;
  QDomDocument doc;
  doc.setContent(s);
  QDomNode node = doc.firstChild();
  int pos;
  QMap<QString,QString>::Iterator it;
  for (it = m_dtd->abbreviations.begin(); it != m_dtd->abbreviations.end(); ++it)
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
    node.appendChild(el);
  }
  if (m_dtd->abbreviations.count() > 0)
  {
    s = QFileInfo(m_dtd->fileName).dirPath();
    QStringList resourceDirs = KGlobal::dirs()->resourceDirs("data");
    bool dirFound = false;
    for (uint i = 0; i < resourceDirs.count(); i++)
    {
      if (s.startsWith(resourceDirs[i]))
      {
        dirFound = true;
        s = s.right(s.length() - resourceDirs[i].length());
        break;
      }
    }
    if (dirFound)
    {
      s = KGlobal::dirs()->saveLocation("data", s);
    }
    s.append("/abbreviations");

    QFile f(s);
    if (f.open(IO_WriteOnly | IO_Truncate))
    {
      QTextStream str(&f);
      str.setEncoding(QTextStream::UnicodeUTF8);
      str << doc.toString();
      f.close();
    } else
    {
      KMessageBox::error(this, i18n("<qt>Cannot open the file <b>%1</b> for writing!\nModified abbreviations will be lost when you quit Quanta.</qt>").arg(s));
    }
  }
}

#include "abbreviation.moc"
