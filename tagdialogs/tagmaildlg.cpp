/***************************************************************************
                          tagmaildlg.cpp  -  description
                             -------------------
    begin                : Sat Nov 24 2001
    copyright            : (C) 2001-2002 by Andras Mantia
    email                : amantia@freemail.hu
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "tagmaildlg.h"
#include "listdlg.h"

//kde includes
#include <kapplication.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kabc/stdaddressbook.h>

//qt includes
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qstringlist.h>
#include <qregexp.h>


TagMailDlg::TagMailDlg(QWidget *parent, const char *name)
	:TagMail(parent,name,true)
{
  setCaption(name);
	
  connect( buttonOk,     SIGNAL(clicked()), SLOT(accept()) );
  connect( buttonCancel, SIGNAL(clicked()), SLOT(reject()) );
  connect( buttonHelp,SIGNAL(clicked()),this,SLOT(slotShowHelp()));
  connect( buttonAddressSelect, SIGNAL(clicked()), SLOT(slotSelectAddress()) );

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
    n = n.simplifyWhiteSpace();
    for( unsigned int i = 0; i < emails.count(); ++i ) {
      if (!emails[i].isEmpty()) {
	if (n.isEmpty() || (emails[i].find( "<" ) != -1))
	  addr = "";
	else { /* do we really need quotes around this name ? */
	  if (n.find(QRegExp("[^ 0-9A-Za-z\\x0080-\\xFFFF]")) != -1)
	    addr = "\"" + n + "\" ";
	  else
	    addr = n + " ";
	}
	email = emails[i];
	if (!addr.isEmpty() && (email.find( "<" ) == -1)
	    && (email.find( ">" ) == -1)
	    && (email.find( "," ) == -1))
	  addr += "<" + email + ">";
	else
	  addr += email;
	addr.stripWhiteSpace();
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
   KMessageBox::error(this,i18n("No addresses found!"));
  }
}

void TagMailDlg::slotShowHelp()
{
//FIXME: "tag-mail" should be replaced with the real help section tag
   kapp->invokeHelp("tag-mail","quanta");
}
#include "tagmaildlg.moc"
