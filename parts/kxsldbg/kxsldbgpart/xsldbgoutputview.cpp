/***************************************************************************
                          xsldbgoutputview.cpp -  Display raw output from xsldbg
                             -------------------
    begin                : Sat July 27 2002
    copyright            : (C) 2002 by keith Isdale
    email                : k_isdale@tpg.com.au
 ***************************************************************************/

/***********************************************************************************
 *                                                                         										*
 *   This program is free software; you can redistribute it and/or modify       *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or         *
 *   (at your option) any later version.                                   							*
 *                                                                         										*
 ************************************************************************************/

/**
  *@author Keith Isdale
  */

#include <qlayout.h>
#include <qmessagebox.h>
#include <qdialog.h>
#include <qpushbutton.h>
#include <qlabel.h>

#if QT_VERSION >= 300
#include "xsldbgmsgdialogimpl.h"
#else
#include "xsldbgmsgdialogimpl2.h"
#endif


XsldbgMsgDialogImpl::XsldbgMsgDialogImpl(QWidget *parent,
					 QMessageBox::Icon icon, 
					 QString title, QString msg) 
#if QT_VERSION >= 300
  : XsldbgMsgDialog(parent, "XsldbgMsgDialogImpl" , TRUE )
#else
  : XsldbgMsgDialog2(parent, "XsldbgMsgDialogImpl" , TRUE )
#endif
{
  setCaption(title);

  QMessageBox tmpMsg;
  tmpMsg.setIcon(icon);
  msgTextEdit->setText(msg);    
  iconLbl->setPixmap(*tmpMsg.iconPixmap());    
}

void XsldbgMsgDialogImpl::append(const QString &text)
{
  msgTextEdit->append(text);
}



#if QT_VERSION >= 300

#include "xsldbgoutputview.h"

XsldbgOutputView::XsldbgOutputView(QWidget * parent)
  : QTextEdit(parent, "outputview")
{
  new QBoxLayout(this, QBoxLayout::TopToBottom);
  setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
  setMinimumSize(QSize(500, 80));
  setCaption("xsldbg output");
  setText("\t\txsldbg output capture ready\n\n");
  dlg = 0L;
  show();
  setReadOnly(TRUE);
}

#else

#include "xsldbgoutputview2.h"

XsldbgOutputView::XsldbgOutputView(QWidget * parent)
  : QTextView(parent, "outputview")
{
  new QBoxLayout(this, QBoxLayout::TopToBottom);
  setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
  setMinimumSize(QSize(500,80));
  setCaption("xsldbg output");
  setText("\t\txsldbg output capture ready\n\n");
  dlg = 0L;
  show();
}
#endif // QT_VERSION

void XsldbgOutputView::slotProcShowMessage(QString  msg)
{
  bool processed = FALSE;    
  // Is this a result of an evaluate command
  if ((msg[0] == QChar('=')) && (msg[1] == QChar(' '))){
    int endPosition = msg.find(QChar('\n'));
    if (endPosition >= 0){         
      processed = TRUE;
      showDialog(QMessageBox::Information, "Result of evaluation", 
		 msg.mid(endPosition + 1));
    }
  }else  /* Is there some sort of error message in msg */
    if ((msg.find("Error:") != -1) || 
	(msg.find("Warning:") != -1) ||
	(msg.find("Request to xsldbg failed") != -1) ||
         /* the following errors are libxml or libxslt generated */
	(msg.find("error:") != -1) || 
	(msg.find("xmlXPathEval:") != -1) ||
	(msg.find("runtime error")  != -1)) {
      /* OK we've found an error but ingore any errors about 
	 data or source files */
      if ((msg.find("Error: No source file supplied") == -1) &&
	  (msg.find("Error: No data file supplied") == -1) &&
	  (msg.find("Load of source deferred") == -1) &&
	  (msg.find("Load of data deferred") == -1) )
	showDialog(QMessageBox::Warning, "Request failed ", 
		 msg);      
      processed = TRUE;
      qDebug(QString("Processed error" ) + msg);
  }
  if (processed == FALSE){
    if (isVisible() == FALSE)
      show();
    append(msg);
  }
}
 

void XsldbgOutputView::slotClearView()
{
}

void XsldbgOutputView::showDialog(QMessageBox::Icon icon, QString title, 
				  QString msg)
{
  
  if (dlg != 0L){
    // not pretty, add this text to the open dialog when multiple 
    // calls to showDialog are made 
    dlg->append(msg);
  }else{
      dlg = new XsldbgMsgDialogImpl(this, icon, title, msg);
      if (dlg != 0L){
	dlg->exec();
	delete dlg;
	dlg = 0L;
      }
  }
}



#include "xsldbgoutputview.moc"
