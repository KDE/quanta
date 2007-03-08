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
#include <QLabel>
#include <QLayout>
#include <QPixmap>

//kde includes
#include <kiconloader.h>
#include <kstandardguiitem.h>

AskForSaveDlg::AskForSaveDlg(const QString &caption, const QString &question, QWidget *parent)
 : KDialog(parent)
{
    setCaption( caption );
    setButtons( KDialog::User2 | KDialog::User1 | KDialog::User3 | KDialog::Cancel );
    setDefaultButton( KDialog::User3 );
    setButtonGuiItem( KDialog::User1,KStandardGuiItem::discard() );
    setButtonGuiItem( KDialog::User2,KStandardGuiItem::saveAs());
    setButtonGuiItem( KDialog::User3,KStandardGuiItem::save());
    QWidget *widget = new QWidget( this );
    setMainWidget( widget );
  QGridLayout *layout = new QGridLayout(widget);
  layout->setObjectName("AksForSaveDlgLayout");
  layout->setMargin(11); //TODO can we remove this, it comes from the style
  layout->setSpacing(6); //TODO can we remove this, it comes from the style
  QLabel *pixmapLabel = new QLabel(widget);
  pixmapLabel->setObjectName("pixmapLabel");
  pixmapLabel->setMaximumSize(QSize(32, 32));
  pixmapLabel->setPixmap(BarIcon("dialog-warning", K3Icon::SizeMedium));
  layout->addWidget(pixmapLabel, 0, 0);
  QLabel *label = new QLabel(widget);
  label->setText(question);
  layout->addWidget(label, 0, 1 );
  resize( QSize(425, 150).expandedTo(minimumSizeHint()) );
  m_status = -1; //cancel
  connect(this,SIGNAL(user1Clicked()),this,SLOT(slotUser1()));
  connect(this,SIGNAL(user2Clicked()),this,SLOT(slotUser2()));
  connect(this,SIGNAL(user3Clicked()),this,SLOT(slotUser3()));

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
