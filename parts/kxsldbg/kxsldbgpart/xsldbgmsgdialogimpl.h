/***********************************************************************************
 *                                                                         										*
 *   This program is free software; you can redistribute it and/or modify       *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or         *
 *   (at your option) any later version.                                   							*
 *                                                                         										*
 ************************************************************************************/

#ifndef XSLDBMSGDIALOGIMPL_H
#define XSLDBMSGDIALOGIMPL_H

/**
  *@author Keith Isdale
  */

#include "xsldbgmsgdialog.h"
#include <qtextedit.h>

class XsldbgMsgDialogImpl : public XsldbgMsgDialog {

public:

  XsldbgMsgDialogImpl(QWidget *parent,
		      QMessageBox::Icon icon, 
		      QString title, QString msg);

  void append(const QString &text);
};
#endif
