/*
 * Crossfire Debugger Support
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

#include <debugger/variable/variablecollection.h>
#include <debugger/breakpoint/breakpointmodel.h>
#include <interfaces/icore.h>
#include <interfaces/idebugcontroller.h>
#include <debugger/interfaces/iframestackmodel.h>

#include "connection.h"
#include "debugsession.h"
#include "variable.h"
#include "stringhelpers.h"

namespace Crossfire {

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


void VariableController::handleFrame(const QVariantMap& data)
{
    Q_ASSERT(data["command"] == "scope");
    Q_ASSERT(data.contains("body"));
    Q_ASSERT(data["body"].canConvert(QVariant::Map));
    Q_ASSERT(data["body"].toMap().contains("object"));
    Q_ASSERT(data["body"].toMap()["object"].canConvert(QVariant::Map));
    Q_ASSERT(data["body"].toMap()["object"].toMap().contains("value"));
    Q_ASSERT(data["body"].toMap()["object"].toMap()["value"].canConvert(QVariant::Map));
    kDebug() << data;

    QStringList names;
    QMapIterator<QString, QVariant> i(data["body"].toMap()["object"].toMap()["value"].toMap());
    while (i.hasNext()) {
        i.next();
        if (i.key() != "parent") {
            names << i.key();
        }
    }
    kDebug() << names;
    {
        KDevelop::Locals* locals = KDevelop::ICore::self()->debugController()->variableCollection()->locals();

        QList<KDevelop::Variable*> vars = locals->updateLocals(names);

        i.toFront();
        while (i.hasNext()) {
            i.next();
            QString name = i.key();
            if (name == "parent") continue;
            foreach (KDevelop::Variable *v, vars) {
                Q_ASSERT(dynamic_cast<Variable*>(v));
                if (v->expression() == name) {
                    static_cast<Variable*>(v)->setCrossfireValue(i.value().toMap());
                    break;
                }
            }
        }
    }
}


void VariableController::updateLocals()
{
    Callback<VariableController>* cb = new Callback<VariableController>(this, &VariableController::handleFrame);
    QVariantMap args;
    args["scopeNo"] = 0;
    args["frameNumber"] = debugSession()->frameStackModel()->currentFrame();
    debugSession()->sendCommand("scope", args, cb);
}

QString VariableController::expressionUnderCursor(KTextEditor::Document* doc, const KTextEditor::Cursor& cursor)
{
    QString line = doc->line(cursor.line());
    int index = cursor.column();
    QChar c = line[index];
    if (!c.isLetterOrNumber() && c != '_' && c != '$')
        return QString();

    int start = expressionAt(line, index);
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
}

void VariableController::addWatchpoint(KDevelop::Variable* variable)
{
}

KDevelop::Variable* VariableController::
createVariable(KDevelop::TreeModel* model, KDevelop::TreeItem* parent,
               const QString& expression, const QString& display)
{
    return new Variable(model, parent, expression, display);
}

}

#include "variablecontroller.moc"
