/*****************************************************************************
 * Copyright (c) 2009 Ruan Strydom <rm3dom@gmail.com>                        *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
 *****************************************************************************/


#include "xmldocumentview.h"

#include <QtGui/QVBoxLayout>

#include <KDE/KActionCollection>

#include <interfaces/context.h>
#include <interfaces/contextmenuextension.h>
#include <interfaces/icore.h>
#include <interfaces/iplugincontroller.h>

#include <language/interfaces/editorcontext.h>


using namespace KDevelop;

XmlDocumentView::XmlDocumentView(XmlDocument *doc, QWidget* parent): QTabWidget(parent), KXMLGUIClient()
{
    m_xmlDocument = doc;
    QWidget *cont = new QWidget();
    cont->setLayout ( new QVBoxLayout() );
    addTab ( cont, "Source" );
    setTabEnabled ( 0, true );

    m_view = doc->textDocument()->createView ( cont );
    cont->layout()->addWidget ( m_view );
    m_view->setContextMenu( m_view->defaultContextMenu() );
    connect(m_view, SIGNAL(contextMenuAboutToShow(KTextEditor::View*,QMenu*)), this, SLOT(populateContextMenu(KTextEditor::View*,QMenu*)));

    m_tree = new XmlTree ( cont );
    addTab ( m_tree, "Tree" );
    setTabEnabled ( 1, true );
    connect(m_tree, SIGNAL(itemSelected(KTextEditor::Cursor)), this, SLOT(setCursorPosition(KTextEditor::Cursor)));
}

XmlDocumentView::~XmlDocumentView()
{

}

void XmlDocumentView::update ( )
{
  m_tree->update(m_xmlDocument->mimeType(), m_xmlDocument->domDocument());
}

void XmlDocumentView::setCursorPosition(KTextEditor::Cursor c)
{
    m_view->setCursorPosition(c);
}

void XmlDocumentView::populateContextMenu(KTextEditor::View* v, QMenu* m)
{
    m->clear();
    v->defaultContextMenu(m);

    Context* c = new EditorContext( v, v->cursorPosition() );
    QList<ContextMenuExtension> extensions = ICore::self()->pluginController()->queryPluginsForContextMenuExtensions( c );
    m->addSeparator();

    ContextMenuExtension::populateMenu(m, extensions);
}

QAction * XmlDocumentView::action ( const QDomElement &element ) const {return m_view->action ( element );}
KActionCollection* XmlDocumentView::actionCollection() const {return m_view->actionCollection();}
KComponentData XmlDocumentView::componentData() const {return m_view->componentData();}
QDomDocument XmlDocumentView::domDocument() const {return m_view->domDocument();}
QString XmlDocumentView::xmlFile() const {return m_view->xmlFile();}
QString XmlDocumentView::localXMLFile() const {return m_view->localXMLFile();}


#include "xmldocumentview.moc"

