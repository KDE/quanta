/***************************************************************************
                     specialchardialog.cpp
                             -------------------

    copyright            : (C) 2004 - Michal Rudolf
    email                : mrudolf@kdewebdev.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <qfile.h>
#include <qtextstream.h>
#include <qregexp.h> 
#include <klocale.h>
#include <kstandarddirs.h>
#include <klistbox.h>
#include <klineedit.h>

#include "specialchardialog.h"

SpecialCharDialog::SpecialCharDialog( QWidget* parent, const char* name, bool modal, WFlags fl)
    :SpecialCharDialogS( parent, name, modal, fl )
{
  readChars();  
  connect ( FilterLineEdit, SIGNAL(textChanged(const QString&)), 
      SLOT(filterChars(const QString&)) );
  connect ( CharsListBox, SIGNAL(doubleClicked(QListBoxItem*)), 
      SLOT(select(QListBoxItem*)) );
  filterChars("");
}

SpecialCharDialog::~SpecialCharDialog()
{
}

void SpecialCharDialog::readChars()
{
   if (m_charList.count()) 
     return;
   QFile file( locate("appdata","chars") );
   if ( file.open(IO_ReadOnly) ) {    // file opened successfully
     QTextStream t( &file );        // use a text stream
     t.setEncoding(QTextStream::UnicodeUTF8);
     QString s;
     while ( !t.eof() ) {           // until end of file...
       m_charList << i18n(t.readLine().utf8()); // line excluding '\n'
     }
     file.close();
   }
}

void SpecialCharDialog::filterChars(const QString& filter)
{
  CharsListBox->clear();
  if (filter.isEmpty())
    CharsListBox->insertStringList(m_charList);
  else {
    QRegExp p_reg( QString("\\b%1\\b").arg(filter) );
    QString p_begin = QString("%1 ").arg(filter);
    for (QStringList::ConstIterator it = m_charList.begin(); it != m_charList.end(); ++it)
      if ( (*it).contains(p_reg) || (*it).startsWith(p_begin) )
        CharsListBox->insertItem(*it);
  }
  if (CharsListBox->currentItem() == -1 && CharsListBox->count())
    CharsListBox->setCurrentItem(0);
}

QString SpecialCharDialog::selection()
{
  QString selected = CharsListBox->text(CharsListBox->currentItem());
  int begin = selected.find("(&")+1;
  int length = selected.find(";)") - begin + 1;
  return selected.mid(begin, length);
}

void SpecialCharDialog::select(QListBoxItem*)
{
  done(QDialog::Accepted);
}

