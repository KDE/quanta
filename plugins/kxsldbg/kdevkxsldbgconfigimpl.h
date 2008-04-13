/**
 *
 *  This file is part of the kdewebdev package
 *  Copyright (c) 2008 Keith Isdale <keith@kdewebdev.org>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License version 2 as published by the Free Software Foundation.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 **/

#ifndef KDEVKXSLDBGCONFIGIMPL_H
#define KDEVKXSLDBGCONFIGIMPL_H

#include "ui_xsldbgconfig.h"
#include <QWidget>
#include <QVariant>
#include <kcmodule.h>

class KDevKXsldbgConfigImpl :  public KCModule, Ui::XsldbgConfig
{
Q_OBJECT

public:
    KDevKXsldbgConfigImpl(QWidget *parent, const QVariantList &args);
    ~KDevKXsldbgConfigImpl();
};

#endif
