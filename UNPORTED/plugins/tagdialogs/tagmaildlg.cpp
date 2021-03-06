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
#include <kiconloader.h>
#include <klocale.h>
#include <kmessagebox.h>
//#include <kabc/stdaddressbook.h>

//qt includes
#include <QLineEdit>
#include <kpushbutton.h>
#include <QStringList>
#include <QRegExp>


TagMailDlg::TagMailDlg(QWidget *parent)
  :KDialog(parent)
{
    setCaption( i18n("TagMail") );
    setButtons( KDialog::Help | KDialog::Ok | KDialog::Cancel );
    setDefaultButton( KDialog::Ok );
  QWidget *w = new QWidget(this);
  setupUi(w);
  setModal(true);
  connect( buttonAddressSelect, SIGNAL(clicked()), SLOT(slotSelectAddress()) );
  lineEmail->setFocus();
  setMainWidget(w);
}

TagMailDlg::~TagMailDlg(){
}
/** Select a mail address, using the KabApi */
void TagMailDlg::slotSelectAddress()
{
  //FIXME: Port to KDE4. There doesn't seem to be a KABC in kdelibs4!
#if 0  
//Based on KMail
  QString addr, email, emailString;
  QStringList result;
  KABC::AddressBook *addressBook = KABC::StdAddressBook::self();
  KABC::AddressBook::Iterator it;
  for( it = addressBook->begin(); it != addressBook->end(); ++it ) {
    QStringList emails = (*it).emails();
    QString n = (*it).prefix() + ' ' +
    (*it).givenName() + ' ' +
    (*it).additionalName() + ' ' +
          (*it).familyName() + ' ' +
    (*it).suffix();
    n = n.simplified();
    for( int i = 0; i < emails.count(); ++i ) {
      if (!emails[i].isEmpty()) {
  if (n.isEmpty() || (emails[i].indexOf( "<" ) != -1))
    addr = "";
  else { /* do we really need quotes around this name ? */
    if (n.indexOf(QRegExp("[^ 0-9A-Za-z\\x0080-\\xFFFF]")) != -1)
      addr = Settings::self()->attributeQuotationChar() + n + Settings::self()->attributeQuotationChar() + ' ';
    else
      addr = n + ' ';
  }
  email = emails[i];
  if (!addr.isEmpty() && (email.indexOf( "<" ) == -1)
      && (email.indexOf( ">" ) == -1)
      && (email.indexOf( "," ) == -1))
    addr += '<' + email + '>';
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
#endif
}

#include "tagmaildlg.moc"
