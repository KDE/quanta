/***************************************************************************
                          tagmaildlg.cpp  -  description
                             -------------------
    begin                : Sat Nov 24 2001
    copyright            : (C) 2001-2002 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *   (at your option) any later version.                                   *
 ***************************************************************************/


#include "tagmaildlg.h"
#include "listdlg.h"
#include "settings.h"

//kde includes
#include <ktoolinvocation.h>
#include <kdialog.h>
#include <kiconloader.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kabc/stdaddressbook.h>

//qt includes
#include <qlineedit.h>
#include <kpushbutton.h>
#include <qstringlist.h>
#include <qregexp.h>


TagMailDlg::TagMailDlg(QWidget *parent)
  :KDialogBase(KDialogBase::Plain, i18n("TagMail"), KDialogBase::Help | KDialogBase::Ok | KDialogBase::Cancel, KDialogBase::Ok, parent)
{
  setupUi(this);
  setModal(true);

/*  connect( buttonOk,     SIGNAL(clicked()), SLOT(accept()) );
  connect( buttonCancel, SIGNAL(clicked()), SLOT(reject()) );
  connect( buttonHelp,SIGNAL(clicked()),this,SLOT(slotShowHelp()));*/
  connect( buttonAddressSelect, SIGNAL(clicked()), SLOT(slotSelectAddress()) );
 /* buttonOk->setIcon(SmallIconSet("button_ok"));
  buttonCancel->setIcon(SmallIconSet("button_cancel"));
  buttonHelp->setIcon(SmallIconSet("help"));*/
  lineEmail->setFocus();
}

TagMailDlg::~TagMailDlg(){
}
/** Select a mail address, using the KabApi */
void TagMailDlg::slotSelectAddress()
{
//Based on KMail
  QString addr, email, emailString;
  QStringList result;
  KABC::AddressBook *addressBook = KABC::StdAddressBook::self();
  KABC::AddressBook::Iterator it;
  for( it = addressBook->begin(); it != addressBook->end(); ++it ) {
    QStringList emails = (*it).emails();
    QString n = (*it).prefix() + " " +
    (*it).givenName() + " " +
    (*it).additionalName() + " " +
          (*it).familyName() + " " +
    (*it).suffix();
    n = n.simplified();
    for( int i = 0; i < emails.count(); ++i ) {
      if (!emails[i].isEmpty()) {
  if (n.isEmpty() || (emails[i].indexOf( "<" ) != -1))
    addr = "";
  else { /* do we really need quotes around this name ? */
    if (n.indexOf(QRegExp("[^ 0-9A-Za-z\\x0080-\\xFFFF]")) != -1)
      addr = Settings::self()->attributeQuotationChar() + n + Settings::self()->attributeQuotationChar() + " ";
    else
      addr = n + " ";
  }
  email = emails[i];
  if (!addr.isEmpty() && (email.indexOf( "<" ) == -1)
      && (email.indexOf( ">" ) == -1)
      && (email.indexOf( "," ) == -1))
    addr += "<" + email + ">";
  else
    addr += email;
  addr.trimmed();
  result.append( addr );
      }
    }
  }
 result.sort();

 if ( result.count() > 0 )
 {
   ListDlg listDlg(result);
   if (!listDlg.exec()) return;
   emailString = listDlg.getEntry();
   lineEmail->setText(emailString);
 } else
 {
//Actually this may mean also other erros, but the result is the same for us: cannot get the
//e-mail address
   KMessageBox::error(this,i18n("No addresses found."));
  }
}

void TagMailDlg::slotShowHelp()
{
//FIXME: "tag-mail" should be replaced with the real help section tag
  KToolInvocation::invokeHelp("tag-mail","quanta");
}
#include "tagmaildlg.moc"
