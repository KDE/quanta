/*****************************************************************************
 * Copyright (c) 2007 Piyush verma <piyush.verma@gmail.com>                  *
 * Copyright (c) 2008,2010 Niko Sams <niko.sams@gmail.com>                   *
 * Copyright (c) 2010 Milian Wolff <mail@milianw.de>                         *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 2 of the License, or         *
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

#include "parsejob.h"

#include <QFile>
#include <QReadWriteLock>
#include <QtCore/QReadLocker>
#include <QtCore/QThread>

#include <kdebug.h>
#include <KMimeType>

#include <language/duchain/duchainlock.h>
#include <language/duchain/duchain.h>
#include <language/duchain/topducontext.h>
#include <interfaces/ilanguage.h>
#include <language/duchain/parsingenvironment.h>
#include <language/duchain/declaration.h>
#include <language/backgroundparser/urlparselock.h>

#include "parser/editorintegrator.h"
#include "parser/parsesession.h"
#include "csslanguagesupport.h"
#include "duchain/builders/declarationbuilder.h"
#include "parser/htmlparser.h"


namespace Css
{
extern int debugArea();
#define debug() kDebug(debugArea())

ParseJob::ParseJob(const KUrl &url)
        : KDevelop::ParseJob(url)
{
    kDebug();
}

ParseJob::~ParseJob()
{
    kDebug();
}

LanguageSupport *ParseJob::css() const
{
    return LanguageSupport::self();
}


void ParseJob::run()
{
    KDevelop::UrlParseLock urlLock(document());

    if ( !(minimumFeatures() & KDevelop::TopDUContext::ForceUpdate) ) {
        KDevelop::DUChainReadLocker lock(KDevelop::DUChain::lock());
        bool needsUpdate = true;
        foreach(const KDevelop::ParsingEnvironmentFilePointer &file, KDevelop::DUChain::self()->allEnvironmentFiles(document())) {
            if (file->needsUpdate()) {
                needsUpdate = true;
                break;
            } else {
                needsUpdate = false;
            }
        }
        if (!needsUpdate) {
            debug() << "Already up to date" << document().str();
            return;
        }
    }

    debug() << "parsing" << document().str();

    bool readFromDisk = !contentsAvailableFromEditor();

    QString contents;

    if (readFromDisk) {
        QFile file(document().str());
        //TODO: Read the first lines to determine encoding using Css encoding and use that for the text stream

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            /*
            KDevelop::ProblemPointer p(new KDevelop::Problem());
            p->setSource(KDevelop::ProblemData::Disk);
            p->setDescription(i18n("Could not open file '%1'", document().str()));
            switch (file.error()) {
            case QFile::ReadError:
                p->setExplanation(i18n("File could not be read from."));
                break;
            case QFile::OpenError:
                p->setExplanation(i18n("File could not be opened."));
                break;
            case QFile::PermissionsError:
                p->setExplanation(i18n("File permissions prevent opening for read."));
                break;
            default:
                break;
            }
            p->setFinalLocation(KDevelop::DocumentRange(document().str(), KTextEditor::Cursor(0, 0), KTextEditor::Cursor(0, 0)));
            // TODO addProblem(p);
            */
            kWarning() << "Could not open file" << document().str();
            return abortJob();
        }

        QTextStream s(&file);
        contents = s.readAll();
        file.close();
    } else {
        contents = contentsFromEditor();
    }

    QList<HtmlParser::Part> parts;

    if (KMimeType::findByUrl(document().toUrl())->name() == "text/css") {
        HtmlParser::Part part;
        part.kind = HtmlParser::Part::Standalone;
        part.contents = contents;
        part.range.start = KDevelop::SimpleCursor(0, 0);
        //part.range.end = TODO (needed?)
        parts << part;
    } else {
        HtmlParser p;
        p.setContents(contents);
        parts = p.parse();
        if (parts.isEmpty()) {
            parts << HtmlParser::Part(); //empty part
        }
    }
    Q_ASSERT(!parts.isEmpty());

    HtmlAst *fileAst = new HtmlAst;
    fileAst->kind = HtmlAst::KIND;
    foreach (const HtmlParser::Part &part, parts) {

        ParseSession *session = new ParseSession;
        session->setCurrentDocument(document().str());
        session->setOffset(part.range.start);
        session->setContents(part.contents);
        if (part.kind != HtmlParser::Part::InlineStyle) {
            StyleElementAst *el = new StyleElementAst;
            el->kind = StyleElementAst::KIND;
            StartAst* ast = 0;
            session->parse(&ast);
            el->start = ast;
            el->session = session;
            fileAst->elements << el;
        } else {
            InlineStyleAst *el = new InlineStyleAst;
            el->kind = InlineStyleAst::KIND;
            DeclarationListAst* ast = 0;
            session->parse(&ast);
            el->declarationList = ast;
            el->session = session;
            fileAst->elements << el;
        }

        if (abortRequested())
        {
            return abortJob();
        }
    }

    KDevelop::ReferencedTopDUContext top;
    {
        KDevelop::DUChainReadLocker lock(KDevelop::DUChain::lock());
        top = KDevelop::DUChain::self()->chainForDocument(document());
    }
    if (top) {
        debug() << "re-compiling" << document().str();
        KDevelop::DUChainWriteLocker lock(KDevelop::DUChain::lock());
        top->clearImportedParentContexts();
        top->parsingEnvironmentFile()->clearModificationRevisions();
        top->clearProblems();
    } else {
        debug() << "compiling" << document().str();
    }

    QReadLocker parseLock(css()->language()->parseLock());

    EditorIntegrator editor;
    DeclarationBuilder builder;
    builder.setEditor(&editor);
    top = builder.build(document(), fileAst, top);
    kDebug() << top;
    Q_ASSERT(top);

    foreach (AstNode *el, fileAst->elements) {

        ParseSession *session = 0;
        if (el->kind == StyleElementAst::KIND) {
            session = static_cast<StyleElementAst*>(el)->session;
            static_cast<StyleElementAst*>(el)->session = 0; //deleted below
        } else if (el->kind == InlineStyleAst::KIND) {
            session = static_cast<InlineStyleAst*>(el)->session;
            static_cast<InlineStyleAst*>(el)->session = 0; //deleted below
        }
        Q_ASSERT(session);
        foreach(const KDevelop::ProblemPointer &p, session->problems()) {
            KDevelop::DUChainWriteLocker lock(KDevelop::DUChain::lock());
            top->addProblem(p);
        }
        delete session;

        if (abortRequested()) {
            return abortJob();
        }
        delete el;
    }
    delete fileAst;
    setDuChain(top);

    cleanupSmartRevision();


    KDevelop::DUChainWriteLocker lock(KDevelop::DUChain::lock());

    top->setFeatures(minimumFeatures());
    KDevelop::ParsingEnvironmentFilePointer file = top->parsingEnvironmentFile();

    QFileInfo fileInfo(document().str());
    QDateTime lastModified = fileInfo.lastModified();
    if (readFromDisk) {
        file->setModificationRevision(KDevelop::ModificationRevision(lastModified));
    } else {
        file->setModificationRevision(KDevelop::ModificationRevision(lastModified, revisionToken()));
    }
    KDevelop::DUChain::self()->updateContextEnvironment( top->topContext(), file.data() );

    cleanupSmartRevision();
}

}

#include "parsejob.moc"
// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on; auto-insert-doxygen on
