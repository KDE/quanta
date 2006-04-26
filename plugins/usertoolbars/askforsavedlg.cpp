/***************************************************************************
    begin                : Thu May 19 2005
    copyright            : (C) 2005 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/
#include "askforsavedlg.h"

//qt includes
#include <qlabel.h>
#include <qlayout.h>

//kde includes
#include <kiconloader.h>

AskForSaveDlg::AskForSaveDlg(const QString &caption, const QString &question, QWidget *parent, const char *name)
 : KDialogBase(KDialogBase::Plain, caption, KDialogBase::User2 | KDialogBase::User1 | KDialogBase::User3 | KDialogBase::Cancel, KDialogBase::User3, parent, name, true, false, KStdGuiItem::discard(), KStdGuiItem::saveAs(), KStdGuiItem::save())
{

  QGridLayout *layout = new QGridLayout(plainPage(), 1, 1, 11, 6, "AksForSaveDlgLayout");
  QLabel *pixmapLabel = new QLabel(plainPage(), "pixmapLabel");
  pixmapLabel->setMaximumSize(QSize(32, 32));
  pixmapLabel->setPixmap(BarIcon("messagebox_warning", K3Icon::SizeMedium));
  layout->addWidget(pixmapLabel, 0, 0);
  QLabel *label = new QLabel(plainPage());
  label->setText(question);
  layout->addWidget(label, 0, 1 );
  resize( QSize(425, 150).expandedTo(minimumSizeHint()) );
  m_status = -1; //cancel
}


AskForSaveDlg::~AskForSaveDlg()
{
}

void AskForSaveDlg::slotUser1()
{
  m_status = 3; //do not save
  accept();
}

void AskForSaveDlg::slotUser2()
{
  m_status = 2; //save as
  accept();
}

void AskForSaveDlg::slotUser3()
{
  m_status = 1; //save
  accept(); 
}


#include "askforsavedlg.moc"
