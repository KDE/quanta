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

#include "xsldbgconfigimpl.h"
#include <QWidget>
#include <QVariant>
#include <kcmodule.h>
#include <core.h>
#include <iproject.h>
#include <iprojectcontroller.h>
#include <idocumentcontroller.h>
#include <iuicontroller.h>

class KCM_KDevKXsldbg :  public KCModule
{
Q_OBJECT

public:
    KCM_KDevKXsldbg(QWidget *parent, const QVariantList &args = QVariantList());
    virtual ~KCM_KDevKXsldbg();
    virtual void save();
    virtual void load();


    XsldbgConfigImpl *cfg;
    KDevelop::IProject* project;
    XsldbgSettingsModel *model;
};

#endif
