/***************************************************************************
                          xsldbgoutputview.h  -  Display raw output from xsldbg
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

#ifndef XSLDBGOUTPUTVIEW_H
#define XSLDBGOUTPUTVIEW_H

/**
  *@author Keith Isdale
  */
#include <qtextedit.h>
#include <qmessagebox.h>

class QDialog;
class XsldbgMsgDialogImpl;

class XsldbgOutputView : public QTextEdit {
  Q_OBJECT
  
public :
    XsldbgOutputView(QWidget *parent = 0);

   void showDialog(QMessageBox::Icon icon, QString title, QString msg);
  
public slots:	
    /** Process a copy of mesage sent to message window */
    void slotProcShowMessage(QString  msg);	
 
    /** Clear message view window of its text */
     void slotClearView();

 private:

     XsldbgMsgDialogImpl *dlg;
     
};

#endif /*  XSLDBGOUTPUTVIEW_H */
