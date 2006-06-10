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

// qt includes
#include <qdir.h>
#include <qevent.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qcombobox.h>

// kde includes
#include <kfiledialog.h>
#include <kiconloader.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kurl.h>
#include <kprotocolinfo.h>
#include <kdeversion.h>

//app includes
#include "projectnewgeneral.h"
#include "quantacommon.h"
#include "qextfileinfo.h"

ProjectNewGeneral::ProjectNewGeneral(QWidget *parent, const char *name )
  : ProjectNewGeneralS(parent,name)
{
  imagelabel->setPixmap( UserIcon("wiznewprjglb") );
  linePrjName->setFocus();

  QStringList protocols = KProtocolInfo::protocols();
  protocols.sort();
  for ( uint i=0; i<protocols.count(); i++ )
  {
    KURL p;
    p.setProtocol(protocols[i]);
    QString protocol = protocols[i];
    if ( KProtocolInfo::supportsWriting(p) &&
         KProtocolInfo::supportsMakeDir(p) &&
         KProtocolInfo::supportsDeleting(p) &&
         (protocol != "file" && protocol != "fonts" && protocol != "floppy" && protocol != "newcd" ))
    {
      comboProtocol->insertItem(protocol);
    }
  }
  comboProtocol->setCurrentItem(0);

  slotProtocolChanged(i18n("Local"));

  linePrjTmpl->setText("templates");
  linePrjToolbar->setText("toolbars");

  connect(comboProtocol, SIGNAL(activated(const QString&)), SLOT(slotProtocolChanged(const QString &)));
  connect( linePrjFile, SIGNAL(textChanged(const QString &)),
           this,        SLOT(slotLinePrjFile(const QString &)));
  connect( linePrjName, SIGNAL(textChanged(const QString &)),
           this,        SLOT(slotLinePrjFile(const QString &)));
  connect( linePrjDir,  SIGNAL(textChanged(const QString &)),
           this,        SLOT(slotLinePrjFile(const QString &)));
  connect( buttonDir,    SIGNAL(clicked()),
           this,        SLOT(slotButtonDir()));
  connect( linePrjName, SIGNAL(textChanged(const QString &)),
           this,        SLOT(slotChangeNames(const QString &)));
  connect( linePrjTmpl, SIGNAL(textChanged(const QString &)), SLOT(slotLinePrjFile(const QString &)));
  connect( buttonTmpl,  SIGNAL(clicked()), SLOT(slotButtonTmpl()));
  connect( linePrjToolbar, SIGNAL(textChanged(const QString &)), SLOT(slotLinePrjFile(const QString &)));
  connect( buttonToolbar,  SIGNAL(clicked()), SLOT(slotButtonToolbar()));

  linePrjTmpl->installEventFilter(this);
  linePrjToolbar->installEventFilter(this);
}

ProjectNewGeneral::~ProjectNewGeneral(){
}

void ProjectNewGeneral::slotButtonDir()
{
   slotLinePrjFile(""); // make sure baseUrl is correct
   KURL url = KFileDialog::getExistingURL(baseUrl.url(), this,
                              i18n("Select Project Folder"));
   if (!url.isEmpty())
   {
     linePrjDir->setText(url.path());
   }
}

void ProjectNewGeneral::slotLinePrjFile( const QString & )
{

  bool valid = !(linePrjFile->text().isEmpty() ||
                 linePrjName->text().isEmpty() ||
                 linePrjDir ->text().isEmpty() ||
                 linePrjTmpl->text().isEmpty() ||
                 linePrjToolbar->text().isEmpty());
  KURL url;
  QString s = lineHost->text();
  if (! s.isEmpty())
    url.setHost(s);

  s = lineUser->text();
  if (! s.isEmpty())
    url.setUser(s);
  s = linePasswd->text();
  if (! s.isEmpty())
    url.setPass(s);

  s = linePort->text();
  if (! s.isEmpty())
    url.setPort(linePort->text().toInt());

  url.setProtocol(comboProtocol->currentText());
  if (url.protocol() == i18n("Local")) url.setProtocol("file");
  url.setPath(linePrjDir->text());
  url.adjustPath(1);
  if (!url.path().startsWith("/")) url.setPath("/"+url.path());

  if (url.isValid())
  {
    emit setBaseURL(url);
    baseUrl = url;
    if (baseUrl.isLocalFile())
    {
      s = QExtFileInfo::canonicalPath(baseUrl.path());
      if (!s.isEmpty())
        baseUrl.setPath(s);
    }
  }
  emit enableNextButton( this, valid );
}

void ProjectNewGeneral::slotChangeNames( const QString &text )
{
  int i;
  QString fname = text.lower();
  while( (i=fname.find(" ")) >=0 ) fname.remove(i,1);

  linePrjFile->setText( fname+".webprj" );
}

QString ProjectNewGeneral::type()
{
//  if ( radioCvs  ->isChecked() ) return "CVS";
  if ( radioLocal->isChecked() ) return "Local";
  if ( radioWeb  ->isChecked() ) return "Web";
  return "Local";
}

void ProjectNewGeneral::setMargin(int i)
{
  layout()->setMargin(i);
}

void ProjectNewGeneral::slotButtonTmpl()
{
   slotLinePrjFile(""); // make sure baseUrl is correct
   KURL url = KFileDialog::getExistingURL(baseUrl.url(), this,
                              i18n("Select Project Template Folder"));
   if (!url.isEmpty() && baseUrl.isParentOf(url))
   {
     linePrjTmpl->setText(KURL::relativeURL(baseUrl, url));
   } else
   {
     KMessageBox::sorry(this, i18n("<qt>The project templates must be stored under the main project folder: <br><br><b>%1</b></qt>").arg(baseUrl.prettyURL(0, KURL::StripFileProtocol)));
    }
}

void ProjectNewGeneral::slotButtonToolbar()
{
   slotLinePrjFile(""); // make sure baseUrl is correct
   KURL url = KFileDialog::getExistingURL(baseUrl.url(), this,
                              i18n("Select Project Toolbar & Actions Folder"));
   if (!url.isEmpty() && baseUrl.isParentOf(url))
   {
     linePrjToolbar->setText(KURL::relativeURL(baseUrl, url));
   }
   {
      KMessageBox::sorry(0, i18n("<qt>The project toolbars must be stored under the main project folder: <br><br><b>%1</b></qt>")
                                  .arg(baseUrl.prettyURL(0, KURL::StripFileProtocol)));
   }
}


void ProjectNewGeneral::slotProtocolChanged(const QString& protocol)
{
 bool status = true;
 if (protocol == i18n("Local"))
 {
   status = false;
 }
 lineHost->setEnabled(status);
 lineUser->setEnabled(status);
 linePasswd->setEnabled(status);
 linePort->setEnabled(status);
 radioWeb->setEnabled(!status);
 slotChangeNames(linePrjName->text());
 if ( !status )
     lineHost->clear();
}

bool ProjectNewGeneral::eventFilter ( QObject * watched, QEvent * e )
{
  if (e->type() == QEvent::FocusOut)
  {
    if (watched == linePrjTmpl)
    {
      KURL url = baseUrl;
      QuantaCommon::setUrl(url, linePrjTmpl->text());
      url = QExtFileInfo::toAbsolute(url, baseUrl);
      if (!baseUrl.isParentOf(url))
      {
        KMessageBox::sorry(this,i18n("<qt>The project templates must be stored under the main project folder: <br><br><b>%1</b></qt>").arg(baseUrl.prettyURL(0, KURL::StripFileProtocol)));
        linePrjTmpl->setFocus();
        emit enableNextButton(this, false);
      } else
        emit enableNextButton(this, true);
    } else
    if (watched == linePrjToolbar)
    {
      KURL url = baseUrl;
      QuantaCommon::setUrl(url, linePrjToolbar->text());
      url = QExtFileInfo::toAbsolute(url, baseUrl);
      if (!baseUrl.isParentOf(url))
      {
        KMessageBox::sorry(0,i18n("<qt>The project toolbars must be stored under the main project folder: <br><br><b>%1</b></qt>")
                                    .arg(baseUrl.prettyURL(0, KURL::StripFileProtocol)));
        linePrjToolbar->setFocus();
        emit enableNextButton(this, false);
      } else
        emit enableNextButton(this, true);
    }
  }
  return false;  
}

#include "projectnewgeneral.moc"


