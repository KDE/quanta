/***************************************************************************
                          filecombo.cpp  -  description
                             -------------------
    begin                : Wed Sep 27 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon <pdima@users.sourceforge.net,yshurik@penguinpowered.com,sequitur@easystreet.com>
                           (C) 2002-2003 Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

// QT includes
#include <QLayout>
#include <QComboBox>
#include <QPushButton>

// KDE includes
#include <klocale.h>
#include <kfiledialog.h>
#include <kurl.h>

// app include
#include "filecombo.h"
#include "extfileinfo.h"

FileCombo::FileCombo(const KUrl& a_baseURL, QWidget *parent)
  :QWidget(parent)
{
  baseURL = a_baseURL;
  m_absolutePath = false;

  QHBoxLayout *layout = new QHBoxLayout(this);

  combo   = new QComboBox(this);
  combo->setEditable(true);
  button  = new QPushButton(this);

  button  ->setFixedSize(35,25);
  button  ->setText(i18n("..."));

  layout  ->addWidget( combo );
  layout  ->addWidget( button );

  connect( button, SIGNAL(clicked()), this, SLOT(slotFileSelect()) );
  connect( combo, SIGNAL(activated(const QString&)), SLOT(slotComboActivated(const QString&)));
  connect( combo, SIGNAL(textChanged(const QString&)), SLOT(slotComboActivated(const QString&)));
  setFocusProxy(combo);
}

FileCombo::FileCombo(QWidget *parent)
   :QWidget(parent)
{
  baseURL.setPath(".");

  QHBoxLayout *layout = new QHBoxLayout(this);

  combo   = new QComboBox(this);
  button  = new QPushButton(this);

  button  ->setFixedSize(35,25);
  button  ->setText(i18n("..."));

  layout  ->addWidget( combo );
  layout  ->addWidget( button );

  connect( button, SIGNAL(clicked()), this, SLOT(slotFileSelect()) );
  connect( combo, SIGNAL(activated(const QString&)), SLOT(slotComboActivated(const QString&)));
  connect( combo, SIGNAL(textChanged(const QString&)), SLOT(slotComboActivated(const QString&)));
  setFocusProxy(combo);
}

FileCombo::~FileCombo(){
}

QString FileCombo::text() const
{
  return combo->currentText();
}

void FileCombo::setText( const QString &_txt )
{
  combo ->setEditText( _txt );
}

void FileCombo::slotFileSelect()
{
  KFileDialog *dlg = new KFileDialog(baseURL.url(), i18n("*|All Files"), this);
  dlg->setMode(KFile::File | KFile::Directory | KFile::ExistingOnly);
  dlg->exec();
  KUrl url = dlg->selectedURL();
  delete dlg;
  if ( !url.isEmpty() )
  {
    if (!m_absolutePath)
      url = KUrl::relativeURL(baseURL, url);
    combo->setEditText( url.path() );
  }
}

/** No descriptions */
void FileCombo::setBaseURL(const KUrl& a_baseURL)
{
 baseURL = a_baseURL;
}

/** No descriptions */
void FileCombo::setReturnAbsolutePath(bool absolutePath)
{
 m_absolutePath = absolutePath;
}

void FileCombo::slotComboActivated(const QString&s)
{
  emit activated(s);
}

#include "filecombo.moc"
