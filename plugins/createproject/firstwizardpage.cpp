/***************************************************************************
                          projectnewgeneral.cpp  -  description
                             -------------------
    begin                : Fri Oct 27 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon <pdima@users.sourceforge.net,yshurik@penguinpowered.com,sequitur@easystreet.com>
                           (C) 2001-2002 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

//app includes
#include "firstwizardpage.h"
#include "settings.h"
#include "quantacoreif.h"

// qt includes
#include <QDir>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>

// kde includes
#include <kcharsets.h>
#include <kglobal.h>
#include <kfiledialog.h>
#include <kiconloader.h>
#include <kinstance.h>
#include <klocale.h>
#include <kurl.h>
#include <kprotocolinfo.h>
#include <kdeversion.h>
#include <kprotocolmanager.h>

FirstWizardPage::FirstWizardPage(QuantaCoreIf *qCore, KInstance *instance, QWidget *parent)
  : QWidget(parent)
{
  setupUi(this);
  connect(comboProtocol, SIGNAL(activated(const QString&)), SLOT(slotProtocolChanged(const QString&)));
  connect(buttonDir, SIGNAL(clicked()), SLOT(slotSelectDirectory()));
  connect(linePrjFile, SIGNAL(textChanged(const QString&)), SLOT(slotSetProjectBase()));
  connect(linePrjDir, SIGNAL(textChanged(const QString&)), SLOT(slotSetProjectBase()));
  connect(linePrjName, SIGNAL(textChanged(const QString&)), SLOT(slotSetProjectBase()));
  connect(linePrjName, SIGNAL(textChanged(const QString&)), SLOT(slotChangeNames(const QString &)));
  
  imagelabel->setPixmap(UserIcon("firstwizardpage", instance));
  linePrjName->setFocus();

  QStringList lst = Settings::self()->loadedDTEPNickNames();
  dtdCombo->addItems(lst);
  QString defaultDTDName = qCore->getDTEPNickName(Settings::self()->defaultDTEP());
  int pos = lst.findIndex(defaultDTDName);
  if (pos >= 0)
    dtdCombo->setCurrentIndex(pos);

  QStringList availableEncodingNames(KGlobal::charsets()->availableEncodingNames());
  encodingCombo->addItems(availableEncodingNames);
  QStringList::ConstIterator iter;
  int iIndex = -1;
  for (iter = availableEncodingNames.begin(); iter != availableEncodingNames.end(); ++iter)
  {
    ++iIndex;
    if ((*iter).toLower() == Settings::self()->defaultEncoding().toLower())
    {
      encodingCombo->setCurrentIndex(iIndex);
      break;
    }
  }

  QStringList protocols = KProtocolInfo::protocols();
  protocols.sort();
  for ( int i=0; i<protocols.count(); i++ )
  {
    KUrl p;
    p.setProtocol(protocols[i]);
    QString protocol = protocols[i];
    if ( KProtocolManager::supportsWriting(p) &&
         KProtocolManager::supportsMakeDir(p) &&
         KProtocolManager::supportsDeleting(p) &&
         (protocol != "file" && protocol != "fonts" && protocol != "floppy" && protocol != "newcd" ))
    {
      comboProtocol->addItem(protocol);
    }
  }
  comboProtocol->setCurrentIndex(0);

  slotProtocolChanged(i18n("Local"));

}

FirstWizardPage::~FirstWizardPage(){
}

void FirstWizardPage::slotSelectDirectory()
{
   slotSetProjectBase(); // make sure baseUrl is correct
   KUrl url = KFileDialog::getExistingUrl(m_baseUrl.url(), this,
                              i18n("Select Project Folder"));
   if (!url.isEmpty())
   {
     linePrjDir->setText(url.path());
   }
}

void FirstWizardPage::slotSetProjectBase()
{

  bool valid = !(linePrjFile->text().isEmpty() ||
                 linePrjName->text().isEmpty() ||
                 linePrjDir ->text().isEmpty() );
  KUrl url;
  QString s = lineHost->text();
  if (!s.isEmpty())
    url.setHost(s);

  s = lineUser->text();
  if (!s.isEmpty())
    url.setUser(s);

  s = linePort->text();
  if (!s.isEmpty())
    url.setPort(linePort->text().toInt());
  
  if (comboProtocol->currentText() == i18n("Local"))
  {
    url.setProtocol("file");
  } else
  {
    url.setProtocol(comboProtocol->currentText());
  }
  url.setPath(linePrjDir->text());
  url.adjustPath(KUrl::AddTrailingSlash);
  if (!url.path().startsWith("/"))
    url.setPath("/" + url.path());

  if (url.isValid())
  {
    emit newBaseURL(url);
    m_baseUrl = url;
  } else
    valid = false;
  emit enableNextButton(this, valid);
  emit enableFinishButton(this, valid);
}

void FirstWizardPage::slotChangeNames( const QString &name )
{
  int i;
  QString fname = name.toLower();
  while((i = fname.indexOf(" ")) >= 0)
    fname.remove(i, 1);

  linePrjFile->setText(fname + ".quanta");
}


void FirstWizardPage::setMargin(int i)
{
  layout()->setMargin(i);
}


void FirstWizardPage::slotProtocolChanged(const QString& protocol)
{
  bool status = true;
  if (protocol == i18n("Local"))
  {
    status = false;
  }
  lineHost->setEnabled(status);
  lineUser->setEnabled(status);
  linePort->setEnabled(status);
  slotChangeNames(linePrjName->text());
  if (!status)
      lineHost->clear();
  emit enableWebDownload(!status);
}

QString FirstWizardPage::fileName() const
{
  return linePrjFile->text();
}

QString FirstWizardPage::name() const
{
  return linePrjName->text();
}

QString FirstWizardPage::author() const
{
  return lineAuthor->text();
}

QString FirstWizardPage::email() const
{
  return lineEmail->text();
}

QString FirstWizardPage::encoding() const
{
  return encodingCombo->currentText();
}

QString FirstWizardPage::dtep() const
{
  return dtdCombo->currentText();
}


#include "firstwizardpage.moc"
