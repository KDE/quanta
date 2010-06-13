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


void Variable::setCrossfireValue(const QVariantMap& val)
{
    kDebug() << val;
    if (val["type"] == "ref") {
        m_ref = val["handle"].toInt();
        Q_ASSERT(m_ref > 0);
        setHasMore(true);
    } else {
        setValue(val["value"].toString());
    }
}

void Variable::setChildValues(const QVariantMap& data)
{
    deleteChildren();
    QMapIterator<QString, QVariant> i(data);
    while (i.hasNext()) {
        i.next();
        kDebug() << i.key() << i.value();
        KDevelop::Variable* xvar = currentSession()->variableController()->
            createVariable(model(), this,
                            i.key());
        Variable* var = static_cast<Variable*>(xvar);
        var->setTopLevel(false);
        appendChild(var);
        if (i.value().canConvert(QVariant::Map)) {
            var->setCrossfireValue(i.value().toMap());
        } else {
            var->setValue(i.value().toString());
        }
    }
}

void Variable::handleLookup(const QVariantMap& data)
{
    setChildValues(data["body"].toMap()["value"].toMap()["value"].toMap());
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

class EvaluateCallback : public CallbackBase
{
public:
    EvaluateCallback(Variable *variable, QObject *callback, const char *callbackMethod)
    : m_variable(variable), m_callback(callback), m_callbackMethod(callbackMethod)
    {}

    virtual void execute(const QVariantMap &data)
    {
        if (!m_variable) return;

        bool hasValue;
        QVariant result = data["body"].toMap()["result"];
        if (result.canConvert(QVariant::Map)) {
            m_variable->setChildValues(result.toMap());
            hasValue = !result.toMap().isEmpty();
        } else {
            m_variable->setValue(result.toString());
            hasValue = !result.toString().isEmpty();
        }
        if (m_callback && m_callbackMethod) {
            QMetaObject::invokeMethod(m_callback, m_callbackMethod, Q_ARG(bool, hasValue));
        }
    }
private:
    QPointer<Variable> m_variable;
    QObject *m_callback;
    const char *m_callbackMethod;
};

void Variable::attachMaybe(QObject *callback, const char *callbackMethod)
{
    DebugSession* session = currentSession();
    if (session) {
        QVariantMap args;
        args["frame"] = session->frameStackModel()->currentFrame();
        args["expression"] = expression();
        session->sendCommand("evaluate", args, new EvaluateCallback(this, callback, callbackMethod));
    }
}


}
