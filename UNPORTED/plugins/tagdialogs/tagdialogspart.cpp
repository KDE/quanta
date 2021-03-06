/***************************************************************************
 *   Copyright (C) 2005 by Andras Mantia                                   *
 *   amantia@kde.org                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "tagdialogspart.h"
#include "tagdialog.h"
#include "tagpair.h"
#include "quantacoreif.h"

#include <QMenu>
#include <QTimer>

#include <kaction.h>
#include <kactioncollection.h>
#include <kdebug.h>
#include <klocale.h>
#include <kparts/mainwindow.h>
#include <kpluginfactory.h>
#include <kpluginloader.h>

//kdevelop includes
#include <core.h>
// #include <kdevcontext.h>
#include <iplugincontroller.h>
#include <iuicontroller.h>

K_PLUGIN_FACTORY(TagDialogsFactory, registerPlugin<TagDialogsPart>();)
K_EXPORT_PLUGIN(TagDialogsFactory("kdevtagdialogs"))


#define GLOBALDOC_OPTIONS 1
#define PROJECTDOC_OPTIONS 2

TagDialogsPart::TagDialogsPart(QObject *parent, const QVariantList &/*args*/)
  : KDevelop::IPlugin(TagDialogsFactory::componentData(), parent), TagDialogsIf()
{
  KDEV_USE_EXTENSION_INTERFACE( TagDialogsIf )
    setXMLFile("kdevtagdialogs.rc");
    kDebug(24000) << "TagDialogs plugin loaded";

    setupActions();

    connect(KDevelop::Core::self()->uiController()->activeMainWindow(), SIGNAL(contextMenu(QMenu *, const Koncrete::Context *)),
            this, SLOT(slotContextMenu(QMenu *, const Koncrete::Context *)));

/*    m_configProxy = new ConfigWidgetProxy(core());
    m_configProxy->createGlobalConfigPage(i18n("Document Structure"), GLOBALDOC_OPTIONS, info()->icon());
    m_configProxy->createProjectConfigPage(i18n("Document Structure"), PROJECTDOC_OPTIONS, info()->icon());
    connect(m_configProxy, SIGNAL(insertConfigWidget(const KDialog*, QWidget*, unsigned int )),
        this, SLOT(insertConfigWidget(const KDialog*, QWidget*, unsigned int)));*/


    QTimer::singleShot(0, this, SLOT(init()));
}

TagDialogsPart::~TagDialogsPart()
{
/*    delete m_widget;
    delete m_configProxy;*/
}

void TagDialogsPart::init()
{
// delayed initialization stuff goes here
  KDevelop::IPlugin *corePlugin = KDevelop::Core::self()->pluginController()->pluginForExtension("org.kdevelop.QuantaCoreIf");
  
  m_qcore = corePlugin->extension<QuantaCoreIf>();
}

void TagDialogsPart::slotContextMenu(QMenu *popup, const KDevelop::Context *context)
{
  //FIXME: context menu handling was changed, adapt it!
  /*
// put actions into the context menu here
  if (context->hasType(Koncrete::Context::EditorContext))
    {
        // editor context menu
//         const EditorContext *econtext = static_cast<const EditorContext*>(context);
        if (m_editCurrentTagAction)
           popup->addAction(m_editCurrentTagAction);

        // or create menu items on the fly
        // int id = -1;
        // id = popup->insertItem(i18n("Do Something Here"),
        //     this, SLOT(doSomething()) );
        // popup->setWhatsThis(id, i18n("<b>Do something here</b><p>Describe here what does this action do."
    }*/
}


void TagDialogsPart::setupActions()
{
// create XMLGUI actions here
  KActionCollection *ac = actionCollection();
    m_editCurrentTagAction  = new KAction(i18n("&Edit Current Tag..."), this);
    actionCollection()->addAction("edit_current_tag", m_editCurrentTagAction );
  connect(m_editCurrentTagAction, SIGNAL(triggered(bool)), SLOT(slotEditCurrentTag()));
}


TagPair TagDialogsPart::createNewTag(QTag *dtdTag, const QString& selection, const QString &attrs, const KUrl &baseURL)
{
  TagDialog dlg(dtdTag, selection, attrs, baseURL);
  if (dlg.exec())
  {
    return dlg.getNewlyCreatedTag();
  }
  return TagPair();
}

void TagDialogsPart::slotEditCurrentTag()
{
  kDebug(24000) << "slotEditCurrentTag ";
}

#include "tagdialogspart.moc"
