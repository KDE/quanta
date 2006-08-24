/***************************************************************************
 *   Copyright (C) 2005 by Andras Mantia                                   *
 *   amantia@kde.org                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "tagdialogspart.h"
#include "tagdialog.h"
#include "tagpair.h"
#include "quantacoreif.h"

#include <QMenu>
#include <QTimer>

#include <kaction.h>
#include <kdebug.h>
#include <kgenericfactory.h>
#include <klocale.h>

//kdevelop includes
#include <kdevcore.h>
#include <kdevcontext.h>
#include <kdevmainwindow.h>
#include <kdevplugincontroller.h>

typedef KGenericFactory<TagDialogsPart> TagDialogsFactory;
K_EXPORT_COMPONENT_FACTORY( libkdevtagdialogs, TagDialogsFactory("kdevtagdialogs") )

#define GLOBALDOC_OPTIONS 1
#define PROJECTDOC_OPTIONS 2

TagDialogsPart::TagDialogsPart(QObject *parent, const QStringList &/*args*/)
  : TagDialogsIf(TagDialogsFactory::instance(), parent)
{
    setXMLFile("kdevtagdialogs.rc");
    kDebug(24000) << "TagDialogs plugin loaded" << endl;

    setupActions();

    connect(KDevCore::mainWindow(), SIGNAL(contextMenu(QMenu *, const Context *)),
        this, SLOT(slotContextMenu(QMenu *, const Context *)));

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
  m_qcore = KDevCore::pluginController()->extension<QuantaCoreIf>("KDevelop/Quanta");

}

void TagDialogsPart::slotContextMenu(QMenu *popup, const Context *context)
{
// put actions into the context menu here
    if (context->hasType(Context::EditorContext))
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
    }
}


void TagDialogsPart::setupActions()
{
// create XMLGUI actions here
  KActionCollection *ac = actionCollection();
  m_editCurrentTagAction = new KAction(i18n("&Edit Current Tag..."), ac, "edit_current_tag");
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
  kDebug(24000) << "slotEditCurrentTag " << endl;
}

#include "tagdialogspart.moc"
