/***************************************************************************
                            conditionalbreakpointdialog.cpp
                            --------------------
    begin                : 2005-01-08
    copyright            : (C) 2004 Linus McCabe <linus@mccabe.nu>
 ***************************************************************************/

/****************************************************************************
 *                                                                          *
 *   This program is free software; you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation; either version 2 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 ***************************************************************************/


#include "conditionalbreakpointdialog.h"
#include <qlistview.h>
#include <qlineedit.h>
#include <qextfileinfo.h>
#include <qcolor.h>
#include <kcombobox.h>
#include <kled.h>
#include <kiconloader.h>
#include <qpushbutton.h>
#include <qradiobutton.h>

#include "debuggerbreakpoint.h"

ConditionalBreakpointDialog::ConditionalBreakpointDialog(const QString& expression, const QString& inFile, const QString& inClass, const QString& inFunction)
    : ConditionalBreakpointDialogS(0, "ConditionalBreakpointDialog", false, 0)
{
  comboExpression->setCurrentText(expression);
  lineFile->setText(inFile);
  lineClass->setText(inClass);
  lineFunction->setText(inFunction);

  buttonClearFile->setPixmap(SmallIcon("clear_left"));
  buttonClearClass->setPixmap(SmallIcon("clear_left"));
  buttonClearFunction->setPixmap(SmallIcon("clear_left"));
  
  connect(comboExpression, SIGNAL(textChanged(const QString&)), this, SLOT(slotExpressionChanged()));

  connect(buttonClearFile, SIGNAL(pressed()), this, SLOT(slotClearFile()));
  connect(buttonClearClass, SIGNAL(pressed()), this, SLOT(slotClearClass()));
  connect(buttonClearFunction, SIGNAL(pressed()), this, SLOT(slotClearFunction()));

  slotExpressionChanged();
}

ConditionalBreakpointDialog::~ConditionalBreakpointDialog()
{
}

void ConditionalBreakpointDialog::slotExpressionChanged()
{

  if(comboExpression->currentText().find( QRegExp("[^=!]=[^=]"), 0 ) >= 0)
    ledWarning->on();
  else
    ledWarning->off();
    
}

void ConditionalBreakpointDialog::slotClearFile()
{
  lineFile->setText("");
}

void ConditionalBreakpointDialog::slotClearClass()
{
  lineClass->setText("");
}

void ConditionalBreakpointDialog::slotClearFunction()
{
  lineFunction->setText("");
}

/*DebuggerBreakpoint::Types type()*/

DebuggerBreakpoint *ConditionalBreakpointDialog::breakpoint()
{
  if(comboExpression->currentText().isEmpty())
    return NULL;
    
  DebuggerBreakpoint *bp = new DebuggerBreakpoint(
    (radioOnChange->isChecked() ? DebuggerBreakpoint::ConditionalChange : DebuggerBreakpoint::ConditionalTrue),
    comboExpression->currentText(),
    lineFile->text(),
    lineClass->text(),
    lineFunction->text());

  return bp;
}


#include "conditionalbreakpointdialog.moc"

