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
#include "tagmisc.h"
#include <klocale.h>
#include <qlineedit.h>
#include <qcheckbox.h>

TagMiscDlg::TagMiscDlg( QWidget* parent, const char* name, bool addClosingTag, const QString &element )
    :KDialogBase( parent, name, true, i18n("Misc. Tag"), KDialogBase::Ok | KDialogBase::Cancel )
{
    miscWidget = new TagMisc(this);
    connect( miscWidget->elementName, SIGNAL( textChanged ( const QString & ) ), this, SLOT( slotMiscTagChanged(const QString & ) ) );
     setMainWidget(miscWidget);
     miscWidget->addClosingTag->setChecked(addClosingTag);
     miscWidget->elementName->setText(element);
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
    return miscWidget->elementName->text();
}

bool TagMiscDlg::addClosingTag() const
{
    return miscWidget->addClosingTag->isChecked();
}

#include "tagmiscdlg.moc"
