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


#ifndef XMLDOCUMENT_H_
#define XMLDOCUMENT_H_

#include <QtXml/QDomDocument>

#include <interfaces/idocument.h>
#include <interfaces/idocumentcontroller.h>

#include <sublime/urldocument.h>

namespace KDevelop
{
  class ICore;
}

class QTabWidget;

class XmlDocument;

class XMLDocumentFactory : public KDevelop::IDocumentFactory
{
public:
    XMLDocumentFactory() : KDevelop::IDocumentFactory() {}
    KDevelop::IDocument* create ( const KUrl& url, KDevelop::ICore* core );
};

class XmlDocument : public Sublime::UrlDocument, public KDevelop::IDocument
{
  Q_OBJECT
public:
    XmlDocument(const KUrl& url, KDevelop::ICore* core);
    virtual ~XmlDocument();
    
    virtual KUrl url() const;

    //virtual Sublime::View* newView ( Document* doc );
    
    virtual QWidget *createViewWidget(QWidget *parent = 0);
    
    virtual KMimeType::Ptr mimeType() const;
    
    virtual KParts::Part* partForView(QWidget *view) const;
    
    virtual bool isTextDocument() const {return true;}
    
    virtual KTextEditor::Document* textDocument() const;
    
    virtual bool save(KDevelop::IDocument::DocumentSaveMode mode = KDevelop::IDocument::Default);
    
    virtual void reload();

    virtual bool close(KDevelop::IDocument::DocumentSaveMode mode = KDevelop::IDocument::Default);
    
    virtual bool isActive() const;
    
    virtual KDevelop::IDocument::DocumentState state() const;
    
    virtual KTextEditor::Cursor cursorPosition() const;
    
    virtual void setCursorPosition(const KTextEditor::Cursor &cursor);
    
    virtual KTextEditor::Range textSelection() const;
    
    virtual void setTextSelection(const KTextEditor::Range &range);
    
    virtual QString textLine() const;
    
    virtual QString textWord() const;
    
    virtual void activate(Sublime::View *activeView, KParts::MainWindow *mainWindow);
    
    virtual bool closeDocument() {
      return close();
    }
    
    virtual const QDomDocument & domDocument();
    
  protected slots:
    virtual void parse();
    virtual void itemSelected(KTextEditor::Cursor cursor);
  private:
    Q_PRIVATE_SLOT(d, void newDocumentStatus(KTextEditor::Document*))
    Q_PRIVATE_SLOT(d, void populateContextMenu(KTextEditor::View*, QMenu*))
    Q_PRIVATE_SLOT(d, void textChanged(KTextEditor::Document*))
    Q_PRIVATE_SLOT(d, void modifiedOnDisk(KTextEditor::Document *, bool, KTextEditor::ModificationInterface::ModifiedOnDiskReason))
    Q_PRIVATE_SLOT(d, void documentUrlChanged(KTextEditor::Document*))
    Q_PRIVATE_SLOT(d, void slotDocumentLoaded())
    Q_PRIVATE_SLOT(d, void documentSaved(KTextEditor::Document*,bool))
    
    class XmlDocumentPrivate;
    XmlDocumentPrivate *d;
};

#endif //XMLDOCUMENT_H_
