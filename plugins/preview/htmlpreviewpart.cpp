/***************************************************************************
 *   Copyright (C) 2005 by Jens Herden                                     *
 *   jens@kdewebdev.org                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "htmlpreviewpart.h"
#include "htmlpart.h"
#include "htmlpreviewglobalconfig.h"
#include "htmlpreviewprojectconfig.h"

#include <QTimer>
#include <QMenu>
#include <QWhatsThis>

#include <klocale.h>
#include <kaction.h>
#include <kactioncollection.h>
#include <kdialog.h>
#include <kicon.h>
#include <kiconloader.h>
#include <kmessagebox.h>
#include <khtmlview.h>
#include <kparts/mainwindow.h>
#include <kparts/partmanager.h>
#include <kparts/part.h>
#include <kparts/browserextension.h>
#include <ktexteditor/document.h>
#include <kpluginfactory.h>
#include <kpluginloader.h>

//kdevelop includes
#include <core.h>
#include <idocumentcontroller.h>
#include <idocument.h>
#include <iprojectcontroller.h>
#include <iuicontroller.h>

K_PLUGIN_FACTORY(HTMLPreviewFactory, registerPlugin<HTMLPreviewPart>();)
K_EXPORT_PLUGIN(HTMLPreviewFactory("kdevhtmlpreview"))


#define GLOBALDOC_OPTIONS 1
#define PROJECTDOC_OPTIONS 2

HTMLPreviewPart::HTMLPreviewPart(QObject *parent, const QVariantList &/*args*/)
  : KDevelop::IPlugin(HTMLPreviewFactory::componentData(), parent), m_activeEditor(0), m_partmanager(0)
{
  kDebug(24000) << "Loading HTMLPreviewPart plugin";
  setXMLFile("kdevhtmlpreview.rc");

  m_browserPart = new HTMLPart(this);
  m_browserPart->view()->setWindowTitle(i18n("HTML Preview"));
  m_browserPart->view()->setObjectName("HtmlPreviewPartWidget");
//FIXME    m_browserPart->widget()->setWindowIcon(SmallIcon(info()->icon()));
  m_browserPart->setOnlyLocalReferences(true);
  m_browserPart->setStatusMessagesEnabled(true);
  m_browserPart->setMetaRefreshEnabled(false);

  setupActions();
  
//FIXME: New KCM modules need to be created for each config page
  /*
  m_configProxy = new ConfigWidgetProxy(core());
  m_configProxy->createGlobalConfigPage(i18n("Preview"), GLOBALDOC_OPTIONS, info()->icon());
  m_configProxy->createProjectConfigPage(i18n("Preview"), PROJECTDOC_OPTIONS, info()->icon());
  connect(m_configProxy, SIGNAL(insertConfigWidget(const KDialog*, QWidget*, unsigned int )),
          this, SLOT(insertConfigWidget(const KDialog*, QWidget*, unsigned int)));
*/
  connect(KDevelop::Core::self()->uiController()->activeMainWindow(), SIGNAL(contextMenu(QMenu *, const Koncrete::Context *)),
          this, SLOT(contextMenu(QMenu *, const Koncrete::Context *)));
  connect(KDevelop::Core::self()->projectController(), SIGNAL(projectOpened( KDevelop::IProject*)), this, SLOT(projectOpened( KDevelop::IProject*)));
  connect(KDevelop::Core::self()->projectController(), SIGNAL(projectClosed( KDevelop::IProject*)), this, SLOT(projectClosed( KDevelop::IProject*)));

  connect(KDevelop::Core::self()->documentController(), SIGNAL(activePartChanged(KParts::Part *)), this, SLOT(slotActivePartChanged(KParts::Part *)));

  QTimer::singleShot(0, this, SLOT(init()));
}

HTMLPreviewPart::~HTMLPreviewPart()
{
  delete m_browserPart;
  delete m_idleTimer;
}

void HTMLPreviewPart::init()
{
  // delayed initialization stuff goes here
  m_idleTimer = new QTimer(this);
  connect(m_idleTimer, SIGNAL(timeout()), SLOT(slotIdleTimerExpired()));

//FIXME  m_partmanager = static_cast<KParts::PartManager *>(KDevApi::self()->documentController()); // FIXME find a better solution
}
  

QWidget *HTMLPreviewPart::pluginView() const
{
  return m_browserPart->view();
}


void HTMLPreviewPart::setupActions()
{
  // create XMLGUI actions here
//FIXME  action = new KAction(KIcon(info()->icon()), i18n("&Preview"), actionCollection(), "plugin_preview" );
  action = actionCollection()->addAction("plugin_preview");
  action->setText(i18n("&Preview"));

  connect(action, SIGNAL(triggered(bool)), SLOT(slotPreview()));
  action->setToolTip(i18n("Preview the document in the browser"));
  action->setWhatsThis(i18n("<b>Preview</b><p>The preview shows the current document in an HTML browser.</p>"));
  action->setEnabled(false);
}

void HTMLPreviewPart::insertConfigWidget(const KDialog *dlg, QWidget *page, unsigned int pageNo)
{
  // create configuraton dialogs here
  switch (pageNo)
  {
    case GLOBALDOC_OPTIONS:
    {
      HTMLPreviewGlobalConfig *w = new HTMLPreviewGlobalConfig(this, page);
      connect(dlg, SIGNAL(okClicked()), w, SLOT(accept()));
      break;
    }
    case PROJECTDOC_OPTIONS:
    {
      HTMLPreviewProjectConfig *w = new HTMLPreviewProjectConfig(this, page);
      connect(dlg, SIGNAL(okClicked()), w, SLOT(accept()));
      break;
    }
  }
}

void HTMLPreviewPart::contextMenu(QMenu */*popup*/, const KDevelop::Context *context)
{
  //FIXME: context menu handling was changed
  // put actions into the context menu here
  /*
  if (context->hasType(Koncrete::Context::EditorContext))
  {
    // editor context menu
//     const EditorContext *econtext = static_cast<const EditorContext*>(context);

    // use context and plug actions here
//     action->plug(popup);

    // or create menu items on the fly
    // int id = -1;
    // id = popup->insertItem(i18n("Do Something Here"),
    //     this, SLOT(doSomething()) );
    // popup->setWhatsThis(id, i18n("<b>Do something here</b><p>Describe here what does this action do."
  }
  else if (context->hasType(Koncrete::Context::FileContext))
  {
    // file context menu
//     const FileContext *fcontext = static_cast<const FileContext*>(context);

    //use context and plug actions here
  }
  else if (context->hasType(Koncrete::Context::ProjectItemContext))
  {
    // project tree context menu
//     const ProjectItemContext *pcontext = static_cast<const ProjectItemContext*>(context);

    // use context and plug actions here
  }
  else if (context->hasType(Koncrete::Context::CodeItemContext))
  {
    // class tree context menu
//     const CodeItemContext *mcontext = static_cast<const CodeItemContext*>(context);

    // use context and plug actions here
  }
/*  else if (context->hasType(Context::DocumentationContext))
  {
    // documentation viewer context menu
//     const DocumentationContext *dcontext = static_cast<const DocumentationContext*>(context);

    // use context and plug actions here
}*/
}

void HTMLPreviewPart::projectOpened(KDevelop::IProject *project)
{
  // do something when the project is opened
}

void HTMLPreviewPart::projectClosed(KDevelop::IProject *project)
{
  // do something when the project is closed
}

void HTMLPreviewPart::slotPreview()
{
  if (! m_activeEditor)
    return;

  loadContent(m_activeEditor);
  //FIXME: how to adapt to KDevelop4?
  //KDevApi::self()->documentController()->documentForPart(m_browserPart)->activate();
}


void HTMLPreviewPart::loadContent(KTextEditor::Document * editor)
{
  if (! editor)
    return;


  int x = 0, y = 0;
  KParts::BrowserExtension* be = m_browserPart->browserExtension();
  if (be && editor->url() == m_browserPart->url())
  {
    x = be->xOffset();
    y = be->yOffset();
  }
  m_browserPart->begin(editor->url(), x, y);
  m_browserPart->write(editor->text());
  m_browserPart->end();

  //FIXME: do we need this? The Document always has an editor interface
  /*
  else
  {
  // Save all files
//     if (partController()->saveAllFiles() == false)
//       return; //user cancelled

    // Force KHTMLPart to reload the page and not use the cache
    KParts::BrowserExtension* be = m_browserPart->browserExtension();
    if (be)
    {
      KParts::URLArgs urlArgs(be->urlArgs());
      urlArgs.reload = true;
      be->setURLArgs(urlArgs);
    }
    m_browserPart->openURL(editor->url());
}*/
}


void HTMLPreviewPart::slotActivePartChanged(KParts::Part *part)
{
  m_idleTimer->stop();

  KTextEditor::Document * newEditor = dynamic_cast<KTextEditor::Document *>(part);
  if (newEditor)
    action->setEnabled(true);

  if (m_browserPart && m_browserPart->view()->isVisible() &&  newEditor && newEditor != m_activeEditor)
    loadContent(newEditor);

  m_activeEditor = newEditor;

  if (newEditor)
  { // connect to the editor
    disconnect(part, 0, this, 0 ); // to make sure that it is not connected twice
    /*if(configData->getRealtimeParsing())*/
    {
      connect(part, SIGNAL(textChanged(KTextEditor::Document *)), this, SLOT(slotTextChanged(KTextEditor::Document *)));
    }
  }
}


void HTMLPreviewPart::slotTextChanged(KTextEditor::Document *document)
{
  Q_UNUSED(document)
  m_idleTimer->setSingleShot(true);
  m_idleTimer->start(500);
}


void HTMLPreviewPart::slotIdleTimerExpired()
{
  if (m_browserPart && m_activeEditor)
    loadContent(m_activeEditor);
}


#include "htmlpreviewpart.moc"

// kate: space-indent on; indent-width 2; mixedindent off; indent-mode cstyle;
