/*
 * XDebug-specific Variable
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

#include <interfaces/icore.h>
#include <debugger/interfaces/ivariablecontroller.h>
#include "connection.h"
#include <QXmlStreamReader>

namespace XDebug {

static bool hasStartedSession()
{
    KDevelop::IDebugSession *session = KDevelop::ICore::self()->debugController()->currentSession();
    if (!session)
        return false;

    KDevelop::IDebugSession::DebuggerState s = session->state();
    return s != KDevelop::IDebugSession::NotStartedState
        && s != KDevelop::IDebugSession::EndedState;
}

Variable::Variable(KDevelop::TreeModel* model, KDevelop::TreeItem* parent,
            const QString& expression, const QString& display)
: KDevelop::Variable(model, parent, expression, display)
{
}

Variable::~Variable()
{
}

class PropertyGetCallback : public CallbackBase
{
public:
    PropertyGetCallback(Variable *variable, QObject *callback, const char *callbackMethod)
    : m_variable(variable), m_callback(callback), m_callbackMethod(callbackMethod)
    {}

    virtual void execute(QXmlStreamReader* xml)
    {
        Q_ASSERT(xml->attributes().value("command") == "property_get");

        if (!m_variable) return;
        bool hasValue = false;

        while (!xml->atEnd() && !(xml->isEndElement() && xml->name() == "response")) {
            xml->readNext();
            if (xml->isStartElement() && xml->name() == "property") {
                m_variable->handleProperty(xml);
            }
        }

        hasValue = true; //TODO: implement correctly

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
    if (hasStartedSession()) {
        // FIXME: Eventually, should be a property of variable.
        KDevelop::IDebugSession* is = KDevelop::ICore::self()->debugController()->currentSession();
        DebugSession* s = static_cast<DebugSession*>(is);
        QStringList args;
        args << "-n " + expression();
        s->connection()->sendCommand("property_get", args, QByteArray(),
                                     new PropertyGetCallback(this, callback, callbackMethod));
    }
}

void Variable::fetchMoreChildren()
{
    int c = childItems.size();
    // FIXME: should not even try this if app is not started.
    // Probably need to disable open, or something
    if (hasStartedSession()) {
        // FIXME: Eventually, should be a property of variable.
        KDevelop::IDebugSession* is = KDevelop::ICore::self()->debugController()->currentSession();
        DebugSession* s = static_cast<DebugSession*>(is);
        //TODO: implement
    }
}


void Variable::handleProperty(QXmlStreamReader *xml)
{
    Q_ASSERT(xml->isStartElement() && xml->name() == "property");

    setInScope(true);

    QStringRef encoding;
    if (xml->attributes().hasAttribute("encoding")) {
        encoding = xml->attributes().value("encoding");
    }
    m_fullName = xml->attributes().value("fullname").toString();

    QMap<QString, Variable*> existing;
    for (int i = 0; i < childCount(); i++) {
        Q_ASSERT(dynamic_cast<Variable*>(child(i)));
        Variable* v = static_cast<Variable*>(child(i));
        existing[v->expression()] = v;
    }

    QSet<QString> current;
    xml->readNext();
    while (!xml->atEnd() && !(xml->isEndElement() && xml->name() == "property")) {
        if (xml->isStartElement() && xml->name() == "property") {
            QString name = xml->attributes().value("name").toString();
            kDebug() << name;
            current << name;
            Variable* v = 0;
            if( !existing.contains(name) ) {
                v = new Variable(model(), this, name);
                appendChild( v, false );
            } else {
                v = existing[name];
            }

            /* TODO: implement correctly
            kDebug() << "children" << xml->attributes().value("children");
            bool hasMore = xml->attributes().value("children") == "true";
            m_variable->setHasMore(hasMore);
            if (isExpanded() && hasMore) {
                fetchMoreChildren();
            }
            */

            v->handleProperty(xml);
        }
        if (xml->isCDATA() || xml->isCharacters()) {
            QString v;
            if (encoding == "base64") {
                //TODO: use Connection::m_codec->toUnicode
                v = QString::fromUtf8(QByteArray::fromBase64(xml->text().toString().toAscii()));
            } else {
                v = xml->text().toString();
            }
            setValue(v);
        }
        xml->readNext();
    }


    for (int i = 0; i < childCount(); ++i) {
        Q_ASSERT(dynamic_cast<KDevelop::Variable*>(child(i)));
        KDevelop::Variable* v = static_cast<KDevelop::Variable*>(child(i));
        if (!current.contains(v->expression())) {
            removeChild(i);
            --i;
            delete v;
        }
    }
}


QString Variable::fullName() const
{
    return m_fullName;
}


}
