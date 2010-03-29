/*****************************************************************************
 * Copyright (c) 2010 Ruan Strydom <ruan@jcell.co.za>                        *
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
#include "languagesupport.h"

#include "parser/parsesession.h"
#include "parser/sgmlparser.h"

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


namespace Xml
{

ParseJob::ParseJob(const KUrl& url) : KDevelop::ParseJob(url)
{
    kDebug();
}

ParseJob::~ParseJob()
{
    kDebug();
}

void ParseJob::run()
{
    /* NOTE
     * Just a quicky to test files.
     */
    
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
            kDebug() << "Already up to date" << document().str();
            return;
        }
    }

    kDebug() << "parsing" << document().str();

    ParseSession session;
    bool readFromDisk = !contentsAvailableFromEditor();
    if (readFromDisk) {
        session.readFile(document().str());
    } else {
        session.setContents(contentsFromEditor());
    }
    StartAst *start=0;
    session.parse(&start);
}

LanguageSupport* ParseJob::xml() const
{
    return LanguageSupport::self();
}

}

#include "parsejob.moc"
// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on; auto-insert-doxygen on
