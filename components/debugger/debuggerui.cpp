/***************************************************************************
                             debuggerui.cpp
                         ------------------------
    begin                : 2004-04-04
    copyright            : (C) 2004 Thiago Silva

 ***************************************************************************/

/****************************************************************************
 *                                                                          *
 *   This program is free software; you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation; either version 2 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 ***************************************************************************/

#include "debuggerui.h"

#include "variableslistview.h"
#include "debuggervariable.h"

#include "quanta.h"
#include "resource.h"

#include <kiconloader.h>
#include <kdockwidget.h>
#include <klocale.h>
#include <qstring.h>

DebuggerUI::DebuggerUI(QObject *parent, const char *name)
 : QObject(parent, name), m_variablesListView(0)
{
  m_variablesListView = new VariablesListView(quantaApp->getMainDockWidget(), "debuggerVariables");
  m_variablesListView->setIcon(SmallIcon("math_brace"));
  m_variablesListView->setCaption(i18n("Variables"));
  quantaApp->addToolWindow(m_variablesListView, KDockWidget::DockBottom, quantaApp->getMainDockWidget());

}


void DebuggerUI::setVariables(const QPtrList<DebuggerVariable>& vars)
{
  m_variablesListView->clear();
  m_variablesListView->setVariables(vars);
}

void DebuggerUI::addVariable(DebuggerVariable* var)
{
  m_variablesListView->addVariable(var);
}

void DebuggerUI::parsePHPVariables(const QString &varstring)
{
  m_variablesListView->parsePHPVariables(varstring);
}

#include "debuggerui.moc"
