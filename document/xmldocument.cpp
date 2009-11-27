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


#include "xmldocument.h"
#include "xmltree.h"
#include "xmldocumentview.h"

#include <QtGui/QTabWidget>
#include <QtGui/QMenu>
#include <QtGui/QVBoxLayout>

#include <KDE/KMessageBox>
#include <KDE/KTextEditor/Editor>
#include <KDE/KTextEditor/Factory>
#include <KDE/KTextEditor/Document>
#include <KDE/KTextEditor/ModificationInterface>
#include <KDE/KTextEdit>
#include <KDE/KTextEditor/View>
#include <KDE/KParts/MainWindow>
#include <KDE/KParts/Part>

#include <interfaces/icore.h>
#include <interfaces/ipartcontroller.h>
#include <interfaces/iuicontroller.h>
#include <interfaces/contextmenuextension.h>
#include <interfaces/iplugincontroller.h>
#include <interfaces/context.h>
#include <sublime/area.h>
#include <sublime/view.h>
#include <sublime/controller.h>
#include <language/interfaces/editorcontext.h>
#include <language/editor/editorintegrator.h>

using namespace KDevelop;

class XmlDocument::XmlDocumentPrivate {
public:
    XmlDocumentPrivate ( XmlDocument *xmlDocument ) :
            xmlDocument ( xmlDocument ),
            editorFactory ( 0 ),
            editor ( 0 ),
            document ( 0 ) {
        state = IDocument::Clean;
        loaded = false;
    }

    ~XmlDocumentPrivate() {
        if ( document ) {
            delete document;
            document = 0;
        }
        while ( views.size() > 0 ) {
            views.takeFirst()->deleteLater();
        }
    }

    void newDocumentStatus ( KTextEditor::Document *document ) {
        bool dirty = ( state == IDocument::Dirty || state == IDocument::DirtyAndModified );
        setStatus ( document, dirty );
    }

    void textChanged ( KTextEditor::Document *document ) {
        Q_UNUSED ( document );
        xmlDocument->notifyContentChanged();
        //This should happen for all views.
        xmlDocument->parse();
    }

    void populateContextMenu ( KTextEditor::View* v, QMenu* menu ) {
        menu->clear();
        v->defaultContextMenu ( menu );

        Context* c = new EditorContext ( v, v->cursorPosition() );
        QList<ContextMenuExtension> extensions = ICore::self()->pluginController()->queryPluginsForContextMenuExtensions ( c );
        menu->addSeparator();

        ContextMenuExtension::populateMenu ( menu, extensions );
    }

    void modifiedOnDisk ( KTextEditor::Document *document, bool ,
                          KTextEditor::ModificationInterface::ModifiedOnDiskReason reason ) {
        bool dirty = false;
        switch ( reason ) {
            case KTextEditor::ModificationInterface::OnDiskUnmodified:
                break;
            case KTextEditor::ModificationInterface::OnDiskModified:
            case KTextEditor::ModificationInterface::OnDiskCreated:
            case KTextEditor::ModificationInterface::OnDiskDeleted:
                dirty = true;
                break;
        }

        setStatus ( document, dirty );
    }

    void setStatus ( KTextEditor::Document* document, bool dirty ) {
        QIcon statusIcon;

        if ( document->isModified() )
            if ( dirty ) {
                state = IDocument::DirtyAndModified;
                statusIcon = KIcon ( "edit-delete" );
            } else {
                state = IDocument::Modified;
                statusIcon = KIcon ( "document-save" );
            }
        else
            if ( dirty ) {
                state = IDocument::Dirty;
                statusIcon = KIcon ( "document-revert" );
            } else {
                state = IDocument::Clean;
            }

        xmlDocument->notifyStateChanged();
        ICore::self()->uiController()->controller()->setStatusIcon ( xmlDocument, statusIcon );
    }

    void documentUrlChanged ( KTextEditor::Document* document ) {
        if ( xmlDocument->url() != document->url() )
            xmlDocument->setUrl ( document->url() );
    }

    void slotDocumentLoaded() {
        if ( loaded )
            return;
        // Tell the editor integrator first
        loaded = true;
        EditorIntegrator::addDocument ( xmlDocument->textDocument() );
        xmlDocument->notifyLoaded();
        //This should happen for all views.
        xmlDocument->parse();
    }

    void documentSaved ( KTextEditor::Document* document, bool saveAs ) {
        Q_UNUSED ( document );
        Q_UNUSED ( saveAs );
        xmlDocument->notifySaved();
        xmlDocument->notifyStateChanged();
    }

    //The parent container class
    XmlDocument *xmlDocument;

    KTextEditor::Factory *editorFactory;
    KTextEditor::Editor *editor;

    QPointer<KTextEditor::Document> document;
    QDomDocument domDocument;
    
    IDocument::DocumentState state;
    QString encoding;
    bool loaded;
    
    QList<XmlDocumentView *> views;
};

IDocument* XMLDocumentFactory::create ( const KUrl& url, ICore* core ) {
    return new XmlDocument ( url, core );
}

XmlDocument::XmlDocument ( const KUrl& url, KDevelop::ICore* core ) :
        UrlDocument ( core->uiController()->controller(), url ),
        IDocument ( core ) {
    d = new XmlDocumentPrivate ( this );
}

XmlDocument::~XmlDocument() {
    if ( d )
        delete d;
}

KUrl XmlDocument::url() const {
    return Sublime::UrlDocument::url();
}

/*
Sublime::View* XmlDocument::newView ( Sublime::Document* doc ) {
    emit viewNumberChanged(this);
    return 0;// new View(doc);
}
*/

QWidget* XmlDocument::createViewWidget ( QWidget* parent ) {

    XmlDocumentView *view = 0;
  
    if ( ! d->editorFactory )
        d->editorFactory = qobject_cast<KTextEditor::Factory*> ( IPartController::findPartFactory (
                                 "text/plain",
                                 "KTextEditor/Document",
                                 "KTextEditor::Editor" ) );
    if ( !d->editor )
        d->editor = d->editorFactory->editor();

    if ( d->editor && !d->document && ( d->document = d->editor->createDocument ( this ) ) ) {
        d->document->setAutoDeletePart(false);
        connect ( d->document, SIGNAL ( modifiedChanged ( KTextEditor::Document* ) ),
                  this, SLOT ( newDocumentStatus ( KTextEditor::Document* ) ) );
        connect ( d->document, SIGNAL ( textChanged ( KTextEditor::Document* ) ),
                  this, SLOT ( textChanged ( KTextEditor::Document* ) ) );
        connect ( d->document, SIGNAL ( documentUrlChanged ( KTextEditor::Document* ) ),
                  this, SLOT ( documentUrlChanged ( KTextEditor::Document* ) ) );
        connect ( d->document, SIGNAL ( documentSavedOrUploaded ( KTextEditor::Document*, bool ) ),
                  this, SLOT ( documentSaved ( KTextEditor::Document*, bool ) ) );

        KTextEditor::ModificationInterface *iface = qobject_cast<KTextEditor::ModificationInterface*> ( d->document );
        if ( iface ) {
            iface->setModifiedOnDiskWarning ( true );
            connect ( d->document, SIGNAL ( modifiedOnDisk ( KTextEditor::Document*, bool, KTextEditor::ModificationInterface::ModifiedOnDiskReason ) ),
                      this, SLOT ( modifiedOnDisk ( KTextEditor::Document*, bool, KTextEditor::ModificationInterface::ModifiedOnDiskReason ) ) );
        }

        d->document->openUrl ( url() );
        if ( d->document->highlightingMode().isNull() || d->document->highlightingMode() == "None" )
            d->document->setHighlightingMode ( "XML" );

        EditorIntegrator::addDocument ( textDocument() );
        notifyTextDocumentCreated();
    }

    if ( d->editor && d->document ) {
        view = new XmlDocumentView(this, parent);
        d->views.append ( view );
        parse();
    }
    
    return view;
}

void XmlDocument::itemSelected ( KTextEditor::Cursor cursor ) {
    foreach ( KTextEditor::View * v, d->document->views() ) {
        v->setCursorPosition ( cursor );
    }
}

KMimeType::Ptr XmlDocument::mimeType() const {
    return KMimeType::findByUrl ( url() );
}

KTextEditor::Document* XmlDocument::textDocument() const {
    return d->document;
}

const QDomDocument& XmlDocument::domDocument() {
  return d->domDocument;
}

bool XmlDocument::save ( KDevelop::IDocument::DocumentSaveMode mode ) {
    if ( !d->document )
        return true;

    if ( mode & Discard )
        return true;

    switch ( d->state ) {
        case IDocument::Clean:
            return true;

        case IDocument::Modified:
            break;
            if ( ! ( mode & Silent ) ) {
                int code = KMessageBox::warningYesNoCancel (
                               ICore::self()->uiController()->activeMainWindow(),
                               i18n ( "The document \"%1\" has unsaved changes. Would you like to save them?", d->document->url().toLocalFile() ),
                               i18n ( "Close Document" ) );
                if ( code != KMessageBox::Yes )
                    return false;
            }
            break;

        case IDocument::Dirty:
        case IDocument::DirtyAndModified:
            if ( ! ( mode & Silent ) ) {
                int code = KMessageBox::warningYesNoCancel (
                               ICore::self()->uiController()->activeMainWindow(),
                               i18n ( "The file \"%1\" is modified on disk.\n\nAre "
                                      "you sure you want to overwrite it? (External "
                                      "changes will be lost.)", d->document->url().toLocalFile() ),
                               i18n ( "Document Externally Modified" ) );
                if ( code != KMessageBox::Yes )
                    return false;
            }
            break;
    }

    KUrl urlBeforeSave = d->document->url();
    if ( d->document->save() ) {
        if ( d->document->url() != urlBeforeSave )
            notifyUrlChanged();
        return true;
    }
    return false;
}

void XmlDocument::reload() {
    if ( !d->document )
        return;

    if ( d->document->isModified() ) {
        if ( KMessageBox::warningYesNo ( ICore::self()->uiController()->activeMainWindow(),
                                         i18n ( "The file \"%1\" is modified "
                                                "in memory. Are you sure you "
                                                "want to reload it? (Local "
                                                "changes will be lost.)", url().toLocalFile() ),
                                         i18n ( "Document is Modified" ) ) == KMessageBox::Yes )
            d->document->setModified ( false );
        else
            return ;
    }

    QList<KTextEditor::Cursor> cursors;
    foreach ( KTextEditor::View *view, d->document->views() )
    cursors << view->cursorPosition();

    d->document->openUrl ( url() );

    notifyStateChanged();

    int i = 0;
    foreach ( KTextEditor::View *view, d->document->views() )
    view->setCursorPosition ( cursors[i++] );
}

bool XmlDocument::close ( KDevelop::IDocument::DocumentSaveMode mode ) {
    if ( !d->document )
        return true;

    this->save ( mode );

    core()->documentController()->notifyDocumentClosed ( this );

    foreach ( Sublime::Area *area,
              core()->uiController()->controller()->allAreas() ) {
        QList<Sublime::View*> areaViews = area->views();
        foreach ( Sublime::View *view, areaViews ) {
            if ( views().contains ( view ) ) {
                area->removeView ( view );
                delete view;
            }
        }
    }
    EditorIntegrator::removeDocument ( textDocument() );
    this->deleteLater();
    return true;
}

bool XmlDocument::isActive() const {
    return true;
}

KDevelop::IDocument::DocumentState XmlDocument::state() const {
    return d->state;
}

KTextEditor::Cursor XmlDocument::cursorPosition() const {
    if ( !d->document )
        KTextEditor::Cursor::invalid();

    KTextEditor::View *view = d->document->activeView();
    if ( view )
        return view->cursorPosition();

    return KTextEditor::Cursor::invalid();
}

void XmlDocument::setCursorPosition ( const KTextEditor::Cursor &cursor ) {
    if ( !cursor.isValid() || !d->document )
        return;

    KTextEditor::View *view = d->document->activeView();

    if ( view )
        view->setCursorPosition ( cursor );
}

KTextEditor::Range XmlDocument::textSelection() const {
    if ( !d->document )
        return KTextEditor::Range::invalid();

    KTextEditor::View *view = d->document->activeView();

    if ( view && view->selection() )
        return view->selectionRange();

    return KTextEditor::Range::invalid();
}

void XmlDocument::setTextSelection ( const KTextEditor::Range &range ) {
    if ( !range.isValid() || !d->document )
        return;

    KTextEditor::View *view = d->document->activeView();

    if ( view ) {
        view->setCursorPosition ( range.start() );
        view->setSelection ( range );
    }
}

QString XmlDocument::textLine() const {
    if ( !d->document )
        return QString();

    KTextEditor::View *view = d->document->activeView();

    if ( view )
        return d->document->line ( view->cursorPosition().line() );

    return QString();
}

QString XmlDocument::textWord() const {
    if ( !d->document )
        return QString();

    KTextEditor::View *view = d->document->activeView();

    if ( view ) {
        KTextEditor::Cursor start = view->cursorPosition();
        kDebug() << "got start position from view:" << start.line() << start.column();
        QString linestr = textLine();
        int startPos = qMax ( qMin ( start.column(), linestr.length() - 1 ), 0 );
        int endPos = startPos;
        startPos --;
        while ( startPos >= 0 && ( linestr[startPos].isLetterOrNumber() || linestr[startPos] == '_' || linestr[startPos] == '~' ) )
            --startPos;
        while ( endPos < linestr.length() && ( linestr[endPos].isLetterOrNumber() || linestr[endPos] == '_' || linestr[endPos] == '~' ) )
            ++endPos;
        if ( startPos != endPos ) {
            kDebug() << "found word" << startPos << endPos << linestr.mid ( startPos + 1, endPos - startPos - 1 );
            return linestr.mid ( startPos + 1, endPos - startPos - 1 );
        }
    }
    return QString();
}

void XmlDocument::activate ( Sublime::View *activeView, KParts::MainWindow *mainWindow ) {
    Q_UNUSED(mainWindow);
    KParts::Part *part = partForView(activeView->widget());
    if(!part)
      return;
    if (ICore::self()->partController()->activePart() != part)
      ICore::self()->partController()->setActivePart(part);
    notifyActivated();
}

KParts::Part* XmlDocument::partForView ( QWidget* view ) const {
    if ( d->views.contains ( (XmlDocumentView *)view ) )
        return d->document;
    return 0;
}

void XmlDocument::parse() {
  if(!d->document)
    return;
  d->domDocument.clear();
  d->domDocument.setContent(d->document->text(), true);
  foreach(XmlDocumentView *v, d->views)
    v->update();
}

#include "xmldocument.moc"
