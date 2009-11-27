/*
Copyright (C) 2009  Ruan Strydom <ruan@jcell.co.za>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef XMLDOCUMENTVIEW_H
#define XMLDOCUMENTVIEW_H

#include "xmltree.h"
#include "xmldocument.h"

#include <QtCore/QPointer>
#include <QtGui/QTabWidget>
#include <QtGui/QMenu>

#include <KDE/KTextEditor/View>
#include <KDE/KTextEditor/Cursor>
#include <KDE/KTextEditor/Document>


class XmlDocumentView : public QTabWidget, public KXMLGUIClient {
    Q_OBJECT
public:
    XmlDocumentView ( XmlDocument *doc, QWidget* parent = 0 );
    virtual ~XmlDocumentView();

public slots:
    virtual void setCursorPosition ( KTextEditor::Cursor );
    virtual void update();
protected slots:
    void populateContextMenu ( KTextEditor::View*, QMenu* );
protected:
    XmlDocument *m_xmlDocument;
    KTextEditor::View *m_view;
    XmlTree *m_tree;

//KXMLGUIClient
public:
    virtual QAction *action ( const QDomElement &element ) const;
    virtual KActionCollection* actionCollection() const;
    virtual KComponentData componentData() const;
    virtual QDomDocument domDocument() const;
    virtual QString xmlFile() const;
    virtual QString localXMLFile() const;

};

#endif // XMLDOCUMENTVIEW_H
