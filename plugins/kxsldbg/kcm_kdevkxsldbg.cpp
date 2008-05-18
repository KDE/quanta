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

#include "kcm_kdevkxsldbg.h"
#include <kgenericfactory.h>
#include <ksettings/dispatcher.h>
#include <libxsldbg/options.h>
#include <QDebug>

K_PLUGIN_FACTORY(KDevKXsldbgConfigFactory, registerPlugin<KCM_KDevKXsldbg>();)
K_EXPORT_PLUGIN(KDevKXsldbgConfigFactory("kcm_kdev_kxsldbg"))

KCM_KDevKXsldbg::KCM_KDevKXsldbg(QWidget *parent, const QVariantList &args)
: KCModule( KDevKXsldbgConfigFactory::componentData(), parent, args )
{
    optionSetAutoConfig(false);
    project = 0;
    Q_FOREACH (KDevelop::IProject* p, KDevelop::Core::self()->projectController()->projects()) {
        if (p->projectFileUrl() == args.at(1).toString()) {
            project = p;
            break;
        }
    }
    Q_ASSERT(project);
    setButtons(Help | Apply);

    qWarning("%s", __PRETTY_FUNCTION__);
    cfg = new XsldbgConfigImpl(parent);
    Q_ASSERT(cfg);
    connect(cfg, SIGNAL(changed()), this, SLOT(changed()));
    model = new XsldbgSettingsModel;
    connect(model, SIGNAL(moelReset()),
            this, SLOT(changed()));
    Q_ASSERT(model);
    cfg->setModel(model);
    KSettings::Dispatcher::registerComponent(KComponentData("KDevKXSLDbg"), model, "revert");
    load();
    cfg->show();
}

KCM_KDevKXsldbg::~KCM_KDevKXsldbg()
{
}

void xsldbgGenericErrorFunc(QString const &text)
{
    qWarning() << text;
}

void KCM_KDevKXsldbg::save()
{
   cfg->update();
   KConfigGroup group = project->projectConfiguration()->group("KXSLDbg");
   if (model->saveSettings(group)){ 
       qWarning("saved OK");
       group.sync();
   }else{
       qWarning("saved failed");
   }

}

void KCM_KDevKXsldbg::load()
{
   KConfigGroup group = project->projectConfiguration()->group("KXSLDbg");
   if (model->loadSettings(group)){
       qWarning("loaded OK");
       cfg->refresh();
   }else{
       qWarning("loaded failed");
   }
}
#include "kcm_kdevkxsldbg.moc"
