/*
 * Crossfire-specific Variable
 *
 * Copyright 2009 Vladimir Prus <ghost@cs.msu.su>
 * Copyright 2009 Niko Sams <niko.sams@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "variable.h"
#include "debugsession.h"

#include <QXmlStreamReader>

#include <interfaces/icore.h>
#include <debugger/interfaces/ivariablecontroller.h>
#include <debugger/interfaces/iframestackmodel.h>

#include "connection.h"

namespace Crossfire {

bool hasStartedSession()
{
    KDevelop::IDebugSession *session = KDevelop::ICore::self()->debugController()->currentSession();
    if (!session)
        return false;
    if (!dynamic_cast<DebugSession*>(session)) return false;

    KDevelop::IDebugSession::DebuggerState s = session->state();
    return s != KDevelop::IDebugSession::NotStartedState
        && s != KDevelop::IDebugSession::EndedState;
}
DebugSession *currentSession()
{
    if (!hasStartedSession()) return 0;
    return static_cast<DebugSession*>(KDevelop::ICore::self()->debugController()->currentSession());
}


Variable::Variable(KDevelop::TreeModel* model, KDevelop::TreeItem* parent,
            const QString& expression, const QString& display)
: KDevelop::Variable(model, parent, expression, display), m_ref(-1)
{
}

Variable::~Variable()
{
}


void Variable::setValue(const QVariantMap& val)
{
    kDebug() << val;
    if (val["type"] == "ref") {
        m_ref = val["handle"].toInt();
        Q_ASSERT(m_ref > 0);
        setHasMore(true);
    } else {
        KDevelop::Variable::setValue(val["value"].toString());
    }
}

void Variable::handleLookup(const QVariantMap& data)
{
    QMapIterator<QString, QVariant> i(data["body"].toMap()["value"].toMap()["value"].toMap());
    while (i.hasNext()) {
        i.next();
        kDebug() << i.key() << i.value();
        KDevelop::Variable* xvar = currentSession()->variableController()->
            createVariable(model(), this,
                            i.key());
        Variable* var = static_cast<Variable*>(xvar);
        var->setTopLevel(false);
        appendChild(var);
        var->setValue(i.value().toMap());
    }
}

void Variable::fetchMoreChildren()
{
    Q_ASSERT(m_ref);
    DebugSession* session = currentSession();
    if (session) {
        QVariantMap args;
        args["handle"] = m_ref;
        Callback<Variable>* cb = new Callback<Variable>(this, &Variable::handleLookup);
        session->sendCommand("lookup", args, cb);
    }
}

void Variable::attachMaybe(QObject *callback, const char *callbackMethod)
{
}


}
