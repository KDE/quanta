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


#include "xmltree.h"
#include "xmldocumentmodel.h"
#include "xsddocumentmodel.h"

#include <KDE/KMessageBox>

XmlTree::XmlTree ( QWidget* parent ) : QTreeView ( parent ), m_treeModel ( 0 ) {
    //viewport()->setAutoFillBackground(false);
    //setBackgroundRole(QPalette::NoRole);
    //setFrameStyle(QFrame::NoFrame);
    //viewport()->setBackgroundRole(QPalette::NoRole);
    //viewport()->setAutoFillBackground(false);
    //setAnimated(false);
    setSortingEnabled ( false );
    setRootIsDecorated ( true );
    connect ( this, SIGNAL ( clicked ( QModelIndex ) ), this, SLOT ( itemSelected ( QModelIndex ) ) );
}

XmlTree::~XmlTree() {
    setModel ( 0 );
    if ( m_treeModel )
        delete m_treeModel;
    m_treeModel = 0;
}

 /* TODO This looses the current expanded tree's.
  * Needs to update the model rahter than replace it.
  */
void XmlTree::update ( const KMimeType::Ptr mime,  const QDomDocument& doc ) {
    setModel ( 0 );
    if ( m_treeModel )
        delete m_treeModel;
    if ( mime->is ( "application/xsd" ) )
        m_treeModel = new XsdDocumentModel();
    else
        m_treeModel = new XmlDocumentModel();
    m_treeModel->update ( doc );
    setModel ( m_treeModel );
    expandToDepth ( 0 );
    resizeColumnToContents ( 0 );
    resizeColumnToContents ( 1 );
}

void XmlTree::itemSelected ( const QModelIndex& index ) {
    if ( !index.isValid() )
        return;
    if ( !m_treeModel )
        return;
    AbstractXmlModelNode *n = m_treeModel->getNode ( index );
    if ( n )
        emit itemSelected ( KTextEditor::Cursor ( n->lineNumber(), n->columnNumber() ) );
}

#include "xmltree.moc"

