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
#include <kurl.h>
#include <kprotocolinfo.h>
#include <kdeversion.h>

//app includes
#include "projectnewgeneral.h"
#include "quantacommon.h"

ProjectNewGeneral::ProjectNewGeneral(QWidget *parent, const char *name )
  : ProjectNewGeneralS(parent,name)
{
  imagelabel->setPixmap( UserIcon("wiznewprjglb") );
  linePrjName->setFocus();

  QStringList protocols = KProtocolInfo::protocols();
  protocols.sort();
  for ( uint i=0; i<protocols.count(); i++ )
  {
    QString p = protocols[i];
    if ( KProtocolInfo::supportsWriting(p) &&
         KProtocolInfo::supportsMakeDir(p) &&
         KProtocolInfo::supportsDeleting(p) &&
         p != "file" )
    {
      comboProtocol->insertItem(p);
    }
  }
  comboProtocol->setCurrentItem(0);

  slotProtocolChanged(i18n("Local"));

  connect(comboProtocol, SIGNAL(activated(const QString&)), SLOT(slotProtocolChanged(const QString &)));
  connect( linePrjFile, SIGNAL(textChanged(const QString &)),
           this,        SLOT(slotLinePrjFile(const QString &)));
  connect( linePrjName, SIGNAL(textChanged(const QString &)),
           this,        SLOT(slotLinePrjFile(const QString &)));
  connect( linePrjDir,  SIGNAL(textChanged(const QString &)),
           this,        SLOT(slotLinePrjFile(const QString &)));
  connect( linePrjDir,  SIGNAL(textChanged(const QString &)),
           this,        SLOT(slotLinePrjTmpl(const QString &)));
  connect( linePrjDir,  SIGNAL(textChanged(const QString &)),
           this,        SLOT(slotLinePrjToolbar(const QString &)));
  connect( buttonDir,    SIGNAL(clicked()),
           this,        SLOT(slotButtonDir()));
  connect( linePrjName, SIGNAL(textChanged(const QString &)),
           this,        SLOT(slotChangeNames(const QString &)));
  connect( linePrjTmpl, SIGNAL(textChanged(const QString &)), SLOT(slotLinePrjFile(const QString &)));
  connect( buttonTmpl,  SIGNAL(clicked()), SLOT(slotButtonTmpl()));
  connect( linePrjToolbar, SIGNAL(textChanged(const QString &)), SLOT(slotLinePrjFile(const QString &)));
  connect( buttonToolbar,  SIGNAL(clicked()), SLOT(slotButtonToolbar()));
}

ProjectNewGeneral::~ProjectNewGeneral(){
}

void ProjectNewGeneral::slotButtonDir()
{
   KURL url = KFileDialog::getExistingURL(linePrjDir->text(), this,
                              i18n("Select Project Folder"));
   if (!url.isEmpty())
   {
     linePrjDir->setText(url.path());
   }
}

void ProjectNewGeneral::slotLinePrjFile( const QString & )
{
  if (   linePrjFile->text().isEmpty() ||
        linePrjName->text().isEmpty() ||
        linePrjDir ->text().isEmpty() ||
        linePrjTmpl->text().isEmpty() ||
        linePrjToolbar->text().isEmpty())
        emit enableNextButton( this, false );
  else
  {
    KURL url;
    url.setHost(lineHost->text());
    url.setUser(lineUser->text());
    url.setPass(linePasswd->text());
    url.setPort(linePort->text().toInt());
    url.setProtocol(comboProtocol->currentText());
    if (url.protocol() == i18n("Local")) url.setProtocol("file");
    QuantaCommon::setUrl(url, linePrjDir->text());
    url.adjustPath(1);
    if (!url.path().startsWith("/")) url.setPath("/"+url.path());

    emit setBaseURL(url);
 }

  emit enableNextButton( this, true  );
}

void ProjectNewGeneral::slotChangeNames( const QString &text )
{
  int i;
  QString fname = text.lower();
  while( (i=fname.find(" ")) >=0 ) fname.remove(i,1);

  linePrjFile->setText( fname+".webprj" );

  QString dir = "";
  QString oldDir = linePrjDir->text();
  QString homeDir = QDir::homeDirPath()+"/";
  if (oldDir. startsWith(homeDir))
  {
    oldDir.remove(0, homeDir.length());
  }

  if (comboProtocol->currentText() == i18n("Local"))
  {
    dir =  homeDir+oldDir;
  }  else
  {
    dir = oldDir;
  }

  linePrjDir ->setText(dir);
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
   KURL url = KFileDialog::getExistingURL(linePrjTmpl->text(), this,
                              i18n("Select Project Template Folder"));
   if (!url.isEmpty())
   {
     linePrjTmpl->setText(url.path());
   }
}

void ProjectNewGeneral::slotButtonToolbar()
{
   KURL url = KFileDialog::getExistingURL(linePrjToolbar->text(), this,
                              i18n("Select Project Toolbar & Actions Folder"));
   if (!url.isEmpty())
   {
     linePrjToolbar->setText(url.path());
   }
}

void ProjectNewGeneral::slotLinePrjToolbar(const QString &Str)
{
  QString str = Str;
  if (!str.endsWith("/"))
      str.append("/");
  linePrjToolbar->setText(str + "toolbars");
}

void ProjectNewGeneral::slotLinePrjTmpl(const QString &Str)
{
  QString str = Str;
  if (!str.endsWith("/"))
      str.append("/");
  linePrjTmpl->setText(str + "templates");
}

/** No descriptions */
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
}

#include "projectnewgeneral.moc"


