/*
 * XDebug Debugger Support
 *
 * Copyright 2007 Hamish Rodda <rodda@kde.org>
 * Copyright 2008 Vladimir Prus <ghost@cs.msu.su>
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

#include "variablecontroller.h"

#include <QXmlStreamReader>

#include <debugger/variable/variablecollection.h>
#include <debugger/breakpoint/breakpointmodel.h>
#include <interfaces/icore.h>
#include <interfaces/idebugcontroller.h>

#include "connection.h"
#include "debugsession.h"
#include "variable.h"
#include "stringhelpers.h"

namespace XDebug {

VariableController::VariableController(DebugSession* parent)
    : KDevelop::IVariableController(parent)
{
    Q_ASSERT(parent);
}

DebugSession *VariableController::debugSession() const
{
    return static_cast<DebugSession*>(const_cast<QObject*>(QObject::parent()));
}

void VariableController::update()
{
    //if (autoUpdate() & UpdateWatches) {
        variableCollection()->watches()->reinstall();
    //}
//    if (autoUpdate() & UpdateLocals) {
        updateLocals();
//    }
}

void VariableController::handleLocals(const QDomDocument &xml)
{
    Q_ASSERT(xml.documentElement().attribute("command") == "context_get");

    QStringList names;
    {
        QDomElement el = xml.documentElement().firstChildElement("property");
        while (!el.isNull()) {
            names << el.attribute("fullname");
            el = el.nextSiblingElement("property");
        }
    }

    {
        KDevelop::Locals* locals = KDevelop::ICore::self()->debugController()->variableCollection()->locals();
        QList<KDevelop::Variable*> vars = locals->updateLocals(names);

        QDomElement el = xml.documentElement().firstChildElement("property");
        while (!el.isNull()) {
            QString name = el.attribute("fullname");
            foreach (KDevelop::Variable *v, vars) {
                Q_ASSERT(dynamic_cast<Variable*>(v));
                if (v->expression() == name) {
                    static_cast<Variable*>(v)->handleProperty(el);
                    break;
                }
            }
            el = el.nextSiblingElement("property");
        }
    }
}

void VariableController::updateLocals()
{
    Callback<VariableController>* cb = new Callback<VariableController>(this, &VariableController::handleLocals);
    debugSession()->connection()->sendCommand("context_get", QStringList(), QByteArray(), cb);
}

QString VariableController::expressionUnderCursor(KTextEditor::Document* doc, const KTextEditor::Cursor& cursor)
{
    QString line = doc->line(cursor.line());
    int index = cursor.column();
    QChar c = line[index];
    if (!c.isLetterOrNumber() && c != '_' && c != '$')
        return QString();

    int start = Utils::expressionAt(line, index);
    int end = index;
    for (; end < line.size(); ++end) {
        QChar c = line[end];
        if (!(c.isLetterOrNumber() || c == '_' || c == '$'))
            break;
    }
    if (!(start < end))
        return QString();

    QString expression(line.mid(start, end-start));
    expression = expression.trimmed();
    return expression;
}


void VariableController::addWatch(KDevelop::Variable* variable)
{
    if (Variable *v = dynamic_cast<Variable*>(variable)) {
        variableCollection()->watches()->add(v->fullName());
    }
}

void VariableController::addWatchpoint(KDevelop::Variable* variable)
{
    if (Variable *v = dynamic_cast<Variable*>(variable)) {
        KDevelop::ICore::self()->debugController()->breakpointModel()->addWatchpoint(v->fullName());
    }
}

KDevelop::Variable* VariableController::
createVariable(KDevelop::TreeModel* model, KDevelop::TreeItem* parent,
               const QString& expression, const QString& display)
{
    return new Variable(model, parent, expression, display);
}

void VariableController::handleEvent(KDevelop::IDebugSession::event_t event)
{
    IVariableController::handleEvent(event);
}

}

#include "variablecontroller.moc"
