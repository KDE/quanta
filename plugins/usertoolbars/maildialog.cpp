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


#include "maildialog.h"

//kde includes
#include <kdialog.h>
#include <kiconloader.h>
#include <kinputdialog.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <ktoolinvocation.h>
#include <kabc/stdaddressbook.h>

//qt includes
#include <qlineedit.h>
#include <kpushbutton.h>
#include <qstringlist.h>
#include <qregexp.h>


MailDialog::MailDialog(QWidget *parent, const QString& caption)
  :MailDialogBase(parent)
{
  setWindowTitle(caption);
  setModal(true);

  connect( buttonOk,     SIGNAL(clicked()), SLOT(accept()) );
  connect( buttonCancel, SIGNAL(clicked()), SLOT(reject()) );
  connect( buttonHelp,SIGNAL(clicked()),this,SLOT(slotShowHelp()));
  connect( buttonAddressSelect, SIGNAL(clicked()), SLOT(slotSelectAddress()) );
  buttonOk->setIconSet(SmallIconSet("button_ok"));
  buttonCancel->setIconSet(SmallIconSet("button_cancel"));
  buttonHelp->setIconSet(SmallIconSet("help"));
  lineEmail->setFocus();
}

MailDialog::~MailDialog(){
}
/** Select a mail address, using the KabApi */
void MailDialog::slotSelectAddress()
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
    for( unsigned int i = 0; i < emails.count(); ++i ) {
      if (!emails[i].isEmpty()) {
  if (n.isEmpty() || (emails[i].indexOf( "<" ) != -1))
    addr = "";
  else { /* do we really need quotes around this name ? */
    if (n.indexOf(QRegExp("[^ 0-9A-Za-z\\x0080-\\xFFFF]")) != -1)
      addr = "\"" + n + "\"" + " ";
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

 bool ok = FALSE;
 QString res = KInputDialog::getItem(
                 i18n( "Select recipient" ),
                 i18n( "Please select an e-mail address:" ), result, 1, FALSE, &ok, this);

 if (ok)
 {
   lineEmail->setText(res);
 } else
 {
//Actually this may mean also other erros, but the result is the same for us: cannot get the
//e-mail address
   KMessageBox::error(this, i18n("No addresses found."));
  }
}

void MailDialog::slotShowHelp()
{
//FIXME: "tag-mail" should be replaced with the real help section tag
  KToolInvocation::invokeHelp("tag-mail","quanta");
}
#include "maildialog.moc"
