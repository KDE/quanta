/*
 * Crossfire-specific implementation of thread and frame model.
 *
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

#include "framestackmodel.h"

#include <KDebug>

#include "connection.h"

namespace Crossfire {

void FrameStackModel::fetchThreads()
{
    //no multithreading, create just one
    QList<KDevelop::FrameStackModel::ThreadItem> threadsList;
    KDevelop::FrameStackModel::ThreadItem i;
    i.nr = 0;
    i.name = "main thread";
    threadsList << i;
    setThreads(threadsList);
    setCurrentThread(0);
}

void FrameStackModel::handleFrames(const QVariantMap &data)
{
    QList<KDevelop::FrameStackModel::FrameItem> frames;
    QListIterator<QVariant> i(data["body"].toMap()["frames"].toList());
//     i.toBack();
//     while (i.hasPrevious()) {
//         QVariantMap map(i.previous().toMap());
//     i.toBack();
    int nr = data["body"].toMap()["fromFrame"].toInt();
    while (i.hasNext()) {
        QVariantMap map(i.next().toMap());
        //kDebug() << map;
        KDevelop::FrameStackModel::FrameItem f;
        f.nr = nr;
        f.name = map["func"].toString();
        kDebug() << f.nr << f.name;
        f.file = map["script"].toString();
        f.line = map["line"].toInt()-1;
        frames << f;
        ++nr;
    }
    setFrames(0, frames);
    setHasMoreFrames(0, data["body"].toMap()["totalFrames"].toInt() > nr);
}

void FrameStackModel::fetchFrames(int threadNumber, int from, int to)
{
    kDebug() << threadNumber << from << to;
    if (threadNumber == 0) { //we support only one thread
        Callback<FrameStackModel>* cb = new Callback<FrameStackModel>(this, &FrameStackModel::handleFrames);
        QVariantMap args;
        args["fromFrame"] = from;
        args["toFrame"] = to;
        session()->sendCommand("backtrace", args, cb);
    }
}

}
