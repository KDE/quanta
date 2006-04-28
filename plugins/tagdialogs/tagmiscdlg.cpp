/***************************************************************************
                          tagmiscdlg.h  -  description
                             -------------------
    copyright            : (C) 2005 Laurent Montel <montel@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "tagmiscdlg.h"
#include <klocale.h>
#include <qlineedit.h>
#include <qcheckbox.h>

TagMiscDlg::TagMiscDlg( QWidget* parent, bool _addClosingTag, const QString &element )
  :KDialogBase(KDialogBase::Plain, i18n("Misc. Tag"), KDialogBase::Ok | KDialogBase::Cancel, KDialogBase::Ok, parent)
{
  setupUi(this);
  connect( elementName, SIGNAL( textChanged ( const QString & ) ), this, SLOT( slotMiscTagChanged(const QString & ) ) );
  addClosingTagCheckBox->setChecked(_addClosingTag);
  elementName->setText(element);
  slotMiscTagChanged( element );
}

TagMiscDlg::~TagMiscDlg()
{
}


void TagMiscDlg::slotMiscTagChanged( const QString &_tag )
{
    enableButtonOK( !_tag.isEmpty() );
}


QString TagMiscDlg::elementTagName() const
{
    return elementName->text();
}

bool TagMiscDlg::addClosingTag() const
{
    return addClosingTagCheckBox->isChecked();
}

#include "tagmiscdlg.moc"
