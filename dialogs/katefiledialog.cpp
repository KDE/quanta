/***************************************************************************
                          katefiledialog.cpp  -  description
                             -------------------
    begin                : Mon Feb 5 2001
    copyright            : (C) 2001 by Christoph Cullmann 
    email                : cullmann@kde.org
 ***************************************************************************/ 

/***************************************************************************
 *                                                                         * 
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  * 
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
    
#include "katefiledialog.h"

#include <kcombobox.h>
#include <ktoolbar.h>
#include <kglobal.h>
#include <kcharsets.h>
#include <qstringlist.h>

#include <kdebug.h>

KateFileDialog::KateFileDialog (const QString& startDir,
                    const QString& encoding,
			              QWidget *parent,
			              const QString& caption,
										int type) : KFileDialog (startDir, QString::null, parent, "", true)
{
  QString sEncoding (encoding);
  
  setCaption (caption);
  
  QStringList availableEncodingNames(KGlobal::charsets()->availableEncodingNames());

  toolBar()->insertCombo(availableEncodingNames, 33333, false, 0L,
	        0L, 0L, true);

	if (type == KateFileDialog::openDialog)
	  setMode(KFile::Files);
	else {
	  setMode(KFile::File);
	  setOperationMode( Saving );
    }

	this->encoding = toolBar()->getCombo(33333);

        // Set default encoding to the locale one, if a different default wasn't requested
        if (encoding == QString::null)
          sEncoding = QString::fromLatin1(QTextCodec::codecForLocale()->name());

        // This is a bit inefficient, but it's the only way to match
        // KCharsets encodings and QTextCodec encodings (e.g. KCharsets say 'utf8'
        // while QTextCodec says 'UTF-8')
        QStringList::ConstIterator it;
        QTextCodec *codecForEnc;
        int iIndex = -1;
        for (it = availableEncodingNames.begin(); it != availableEncodingNames.end(); ++it) {
          ++iIndex;
          codecForEnc = KGlobal::charsets()->codecForName(*it);
          if (codecForEnc->name() == sEncoding)
             break;
        }

        if (iIndex >= 0)
          this->encoding->setCurrentItem(iIndex);
}

KateFileDialog::~KateFileDialog ()
{

}

KateFileDialogData KateFileDialog::exec()
{
	int n = KDialogBase::exec();

	KateFileDialogData data = KateFileDialogData ();
  
	if (n)
	{
	  data.encoding = this->encoding->currentText();
	  data.url = selectedURL ();
	  data.urls = selectedURLs ();
  }

	return data;
}

void KateFileDialog::slotApply()
{

}

#include "katefiledialog.moc"
