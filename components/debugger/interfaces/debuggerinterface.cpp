/***************************************************************************
                            debuggerinterface.cpp
                            ---------------------
    begin                : 2004-03-12
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

#include "debuggerinterface.h"

DebuggerInterface::DebuggerInterface (QObject *parent, const char* name)
    : QObject(parent, name)
{
}


#include "debuggerinterface.moc"
