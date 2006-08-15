/***************************************************************************
 *   Copyright (C) 2005 by Jens Herden                                     *
 *   jens@kdewebdev.org                                                    *
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

#include "htmlpreviewpart.h"
#include "htmlpart.h"
#include "htmlpreviewglobalconfig.h"
#include "htmlpreviewprojectconfig.h"

#include <QTimer>
#include <QMenu>
#include <QWhatsThis>

#include <klocale.h>
#include <kaction.h>
#include <kdialog.h>
#include <kgenericfactory.h>
#include <kicon.h>
#include <kiconloader.h>
#include <kmessagebox.h>
#include <khtmlview.h>
#include <kparts/partmanager.h>
#include <kparts/part.h>
#include <kparts/browserextension.h>
#include <ktexteditor/document.h>

//kdevelop includes
#include <interfaces/kdevcore.h>
#include <interfaces/kdevmainwindow.h>
#include <interfaces/kdevdocumentcontroller.h>
#include <interfaces/kdevdocument.h>


typedef KGenericFactory<HTMLPreviewPart> HTMLPreviewFactory;
K_EXPORT_COMPONENT_FACTORY( libkdevhtmlpreview, HTMLPreviewFactory("kdevhtmlpreview") );

#define GLOBALDOC_OPTIONS 1
#define PROJECTDOC_OPTIONS 2

HTMLPreviewPart::HTMLPreviewPart(QObject *parent, const QStringList &/*args*/)
  : KDevPlugin(HTMLPreviewFactory::instance(), parent), m_activeEditor(0), m_partmanager(0)
{
  setXMLFile("kdevhtmlpreview.rc");

/*  m_widget = new HTMLPreviewWidget(this);
  m_widget->setWindowTitle("widget caption");
  m_widget->setWindowIcon(SmallIcon(info()->icon()));

  m_widget->setWhatsThis(i18n("WHAT DOES THIS PART DO?"));
*/


  setupActions();
//FIXME: New KCM modules need to be created for each config page
  /*
  m_configProxy = new ConfigWidgetProxy(core());
  m_configProxy->createGlobalConfigPage(i18n("Preview"), GLOBALDOC_OPTIONS, info()->icon());
  m_configProxy->createProjectConfigPage(i18n("Preview"), PROJECTDOC_OPTIONS, info()->icon());
  connect(m_configProxy, SIGNAL(insertConfigWidget(const KDialog*, QWidget*, unsigned int )),
          this, SLOT(insertConfigWidget(const KDialog*, QWidget*, unsigned int)));
*/
  connect(KDevApi::self()->core(), SIGNAL(contextMenu(QMenu *, const Context *)),
          this, SLOT(contextMenu(QMenu *, const Context *)));
  connect(KDevApi::self()->core(), SIGNAL(projectOpened()), this, SLOT(projectOpened()));
  connect(KDevApi::self()->core(), SIGNAL(projectClosed()), this, SLOT(projectClosed()));

  connect(KDevApi::self()->documentController(), SIGNAL(activePartChanged(KParts::Part *)), this, SLOT(slotActivePartChanged(KParts::Part *)));

  QTimer::singleShot(0, this, SLOT(init()));
}

HTMLPreviewPart::~HTMLPreviewPart()
{
// if you embed a widget, you need to tell the mainwindow when you remove it
  if (m_browserPart)
  {
    KDevApi::self()->mainWindow()->removeView(m_browserPart->view());
  }
//   if (m_browserPart)
//     m_partmanager->removePart(m_browserPart);

  delete m_browserPart;
//  delete m_configProxy;
  delete m_idleTimer;
}

void HTMLPreviewPart::init()
{
  // delayed initialization stuff goes here
  m_idleTimer = new QTimer(this);
  connect(m_idleTimer, SIGNAL(timeout()), SLOT(slotIdleTimerExpired()));

//FIXME  m_partmanager = static_cast<KParts::PartManager *>(KDevApi::self()->documentController()); // FIXME find a better solution
}

void HTMLPreviewPart::setupActions()
{
  // create XMLGUI actions here
//FIXME  action = new KAction(KIcon(info()->icon()), i18n("&Preview"), actionCollection(), "plugin_preview" );
  action = new KAction(KIcon(), i18n("&Preview"), actionCollection(), "plugin_preview" );
  connect(action, SIGNAL(triggered(bool)), SLOT(slotPreview()));
  action->setToolTip(i18n("Preview the document in the browser"));
  action->setWhatsThis(i18n("<b>Preview</b><p>The preview shows the current document in an HTML browser."));
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

void HTMLPreviewPart::contextMenu(QMenu */*popup*/, const Context *context)
{
  // put actions into the context menu here
  if (context->hasType(Context::EditorContext))
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
  else if (context->hasType(Context::FileContext))
  {
    // file context menu
//     const FileContext *fcontext = static_cast<const FileContext*>(context);

    //use context and plug actions here
  }
  else if (context->hasType(Context::ProjectItemContext))
  {
    // project tree context menu
//     const ProjectItemContext *pcontext = static_cast<const ProjectItemContext*>(context);

    // use context and plug actions here
  }
  else if (context->hasType(Context::CodeItemContext))
  {
    // class tree context menu
//     const CodeItemContext *mcontext = static_cast<const CodeItemContext*>(context);

    // use context and plug actions here
  }
  else if (context->hasType(Context::DocumentationContext))
  {
    // documentation viewer context menu
//     const DocumentationContext *dcontext = static_cast<const DocumentationContext*>(context);

    // use context and plug actions here
  }
}

void HTMLPreviewPart::projectOpened()
{
  // do something when the project is opened
}

void HTMLPreviewPart::projectClosed()
{
  // do something when the project is closed
}

void HTMLPreviewPart::slotPreview()
{
  if (! m_activeEditor)
    return;

  if (! m_browserPart)
  {
    m_browserPart = new HTMLPart(this);
//FIXME    m_browserPart->widget()->setWindowIcon(SmallIcon(info()->icon()));
    m_browserPart->setOnlyLocalReferences(true);
    m_browserPart->setStatusMessagesEnabled(true);
    m_browserPart->setMetaRefreshEnabled(false);

    //     mainWindow()->embedPartView(m_browserPart->view(), i18n("Preview"));

  // if you want to embed your widget as an outputview, simply uncomment
  // the following line.
    KDevApi::self()->mainWindow()->embedOutputView( m_browserPart->view(), i18n("Preview"), i18n("Preview in a browser") );

  // if you want to embed your widget as a selectview (at the left), simply uncomment
  // the following line.
  // mainWindow()->embedSelectView( m_widget, "name that should appear", "enter a tooltip" );

  // if you want to embed your widget as a selectview (at the right), simply uncomment
  // the following line.
  // mainWindow()->embedSelectViewRight( m_widget, "name that should appear", "enter a tooltip" );


    m_partmanager->addPart(m_browserPart, false);
  }
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
