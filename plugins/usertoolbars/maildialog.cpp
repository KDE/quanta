/***************************************************************************
                          tagmaildlg.cpp  -  description
                             -------------------
    begin                : Sat Nov 24 2001
    copyright            : (C) 2001-2006 by Andras Mantia <amantia@kde.org>
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
#include <kiconloader.h>
#include <kinputdialog.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <ktoolinvocation.h>
//#include <kabc/stdaddressbook.h>

//qt includes
#include <QLineEdit>
#include <kpushbutton.h>
#include <QStringList>
#include <QRegExp>


MailDialog::MailDialog(QWidget *parent, const QString& caption)
  :KDialog(parent)
{
    setCaption( caption );
    setButtons( KDialog::Ok | KDialog::Cancel | KDialog::Help);
    setDefaultButton( KDialog::Ok );
  QWidget *w = new QWidget(this);
  setupUi(w);
  setMainWidget(w);
  setModal(true);

  connect( buttonAddressSelect, SIGNAL(clicked()), SLOT(slotSelectAddress()) );
  lineEmail->setFocus();
}

MailDialog::~MailDialog(){
}
/** Select a mail address, using the KabApi */
void MailDialog::slotSelectAddress()
{
  //FIXME: What is instead of KABC??
#if 0
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
#endif
}
#include "maildialog.moc"
