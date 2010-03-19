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

#ifndef KDEVMLLANGUAGESUPPORT_H
#define KDEVMLLANGUAGESUPPORT_H

#include <interfaces/iplugin.h>
#include <language/interfaces/ilanguagesupport.h>

namespace KDevelop
{
class ParseJob;
}

namespace Ml
{

class LanguageSupport : public KDevelop::IPlugin, public KDevelop::ILanguageSupport
{
    Q_OBJECT
    Q_INTERFACES(KDevelop::ILanguageSupport)

public:
    explicit LanguageSupport(QObject *parent, const QVariantList& args = QVariantList());

    /*Name Of the Language*/
    QString name() const;

    virtual KDevelop::ParseJob *createParseJob(const KUrl &url);

    static LanguageSupport* self();

    virtual KDevelop::SimpleRange specialLanguageObjectRange(const KUrl& url, const KDevelop::SimpleCursor& position);
    virtual QWidget* specialLanguageObjectNavigationWidget(const KUrl& url, const KDevelop::SimpleCursor& position);

private:
    static LanguageSupport* m_self;
};

}

#endif

