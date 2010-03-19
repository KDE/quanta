/*
Copyright (C) 2010  Ruan Strydom <ruan@jcell.co.za>

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

#include "languagesupport.h"

#include <kpluginfactory.h>
#include <kpluginloader.h>
#include <kaboutdata.h>
#include <KTextEditor/Document>

#include <language/codecompletion/codecompletion.h>
#include <interfaces/idocument.h>
#include <interfaces/icore.h>
#include <interfaces/idocumentcontroller.h>
#include <language/duchain/duchain.h>
#include <language/duchain/duchainlock.h>

#include "parsejob.h"

K_PLUGIN_FACTORY(KDevMlSupportFactory, registerPlugin<Ml::LanguageSupport>();)
K_EXPORT_PLUGIN(KDevMlSupportFactory(KAboutData("kdevmlsupport","kdevml", ki18n("Markup Language Support"), "0.1", 
                                      ki18n("Support for Markup Language"), KAboutData::License_GPL)
                                      .addAuthor(ki18n("Ruan Strydom"), ki18n("Author"), "ruan@jcell.co.za", "")))

namespace Ml
{
LanguageSupport* LanguageSupport::m_self = 0;
    
LanguageSupport::LanguageSupport(QObject* parent, const QVariantList& args) :
        KDevelop::IPlugin(KDevMlSupportFactory::componentData(), parent),
        KDevelop::ILanguageSupport()
{
    m_self = this;
}

QString LanguageSupport::name() const
{
    return "Ml";
}

LanguageSupport* LanguageSupport::self()
{
    return m_self;
}

QWidget* LanguageSupport::specialLanguageObjectNavigationWidget(const KUrl& url, const KDevelop::SimpleCursor& position)
{
    return 0;
}

KDevelop::SimpleRange LanguageSupport::specialLanguageObjectRange(const KUrl& url, const KDevelop::SimpleCursor& position)
{
    return KDevelop::SimpleRange();
}
    
KDevelop::ParseJob* LanguageSupport::createParseJob(const KUrl& url)
{
    return new ParseJob(url);
}       

}

#include "languagesupport.moc"
