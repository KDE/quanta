/*
 * Crossfire Debugger Support
 *
 * Copyright 2009 Niko Sams <niko.sams@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef CROSSFIRE_CROSSFIREPLUGIN_H
#define CROSSFIRE_CROSSFIREPLUGIN_H

#include <interfaces/iplugin.h>
#include <QVariant>


namespace Crossfire {

class DebugSession;

class CrossfirePlugin : public KDevelop::IPlugin
{
Q_OBJECT

public:
    CrossfirePlugin( QObject *parent, const QVariantList & = QVariantList() );
    ~CrossfirePlugin();
    DebugSession* createSession() const;
};

}

#endif // CROSSFIRE_CROSSFIREPLUGIN_H
