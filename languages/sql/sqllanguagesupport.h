/*
   Copyright (C) 2010 Niko Sams <niko.sams@gmail.com>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef KDEVSQLLANGUAGESUPPORT_H
#define KDEVSQLLANGUAGESUPPORT_H

#include <interfaces/iplugin.h>
#include <language/interfaces/ilanguagesupport.h>

namespace Sql
{
int debugArea();

class ResultTableWidget;
template<typename T> class ToolFactory;

class LanguageSupport : public KDevelop::IPlugin, public KDevelop::ILanguageSupport
{
    Q_OBJECT
    Q_INTERFACES(KDevelop::ILanguageSupport)

public:
    explicit LanguageSupport(QObject *parent, const QVariantList& args = QVariantList());

    /*Name Of the Language*/
    QString name() const;

    virtual KDevelop::ParseJob* createParseJob(const KUrl&) { return 0; }

    static LanguageSupport* self();

private slots:
    void runSql();

private:
    static LanguageSupport* m_self;
    ToolFactory<ResultTableWidget>* m_resultTableFactory;
};

}

#endif

