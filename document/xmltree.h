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

#ifndef XMLTREE_H
#define XMLTREE_H

#include <QtGui/QTreeView>
#include <QtGui/QWidget>
#include <QtXml/QDomDocument>

#include <KDE/KMimeType>
#include <KDE/KTextEditor/Cursor>

namespace KDevelop {
  class IDocument;
}

class AbstractXmlModel;

/**
 * The xml tree view.
 */
class XmlTree : public QTreeView
{
  Q_OBJECT
  public:
    XmlTree(QWidget *parent = 0);
    virtual ~XmlTree();
    //virtual bool parse (const KDevelop::IDocument *document, QString * errorMsg = 0, int * errorLine = 0, int * errorColumn = 0);
    virtual void update(const KMimeType::Ptr mime, const QDomDocument & doc);
  signals:
    void itemSelected(KTextEditor::Cursor cursor);
  protected slots:
    void itemSelected(const QModelIndex& index);
  protected:
    AbstractXmlModel *m_treeModel;
};

#endif // XMLTREE_H
