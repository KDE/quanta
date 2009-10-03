/*
 * XDebug-specific implementation of thread and frame model.
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

#include <QXmlStreamReader>

#include <KDebug>

#include "framestackmodel.h"
#include "connection.h"

namespace XDebug {

void FrameStackModel::fetchThreads()
{
    //no multithreading in php, create just one
    QList<KDevelop::FrameStackModel::ThreadItem> threadsList;
    KDevelop::FrameStackModel::ThreadItem i;
    i.nr = 0;
    i.name = "main thread";
    threadsList << i;
    setThreads(threadsList);
    setCurrentThread(0);
}

void FrameStackModel::handleStack(QXmlStreamReader* xml)
{
    Q_ASSERT(xml->attributes().value("command") == "stack_get");

    QList<KDevelop::FrameStackModel::FrameItem> frames;
    while (!(xml->isEndElement() && xml->name() == "response")) {
        xml->readNext();
        if (xml->isStartElement() && xml->name() == "stack") {
            kDebug() << "level" << xml->attributes().value("level");
            kDebug() << "type" << xml->attributes().value("type");
            kDebug() << "filename" << xml->attributes().value("filename");
            kDebug() << "lineno" << xml->attributes().value("lineno");
            kDebug() << "where" << xml->attributes().value("where");
            KDevelop::FrameStackModel::FrameItem f;
            f.nr = xml->attributes().value("level").toString().toInt();
            f.name = xml->attributes().value("where").toString();
            f.file = xml->attributes().value("filename").toString();
            f.line = xml->attributes().value("lineno").toString().toInt();
            frames << f;
        }
    }
    setFrames(0, frames);
    setHasMoreFrames(0, false);
}

void FrameStackModel::fetchFrames(int threadNumber, int from, int to)
{
    Q_UNUSED(from); //we fetch always everything
    Q_UNUSED(to);

    if (threadNumber == 0) { //we support only one thread
        Callback<FrameStackModel>* cb = new Callback<FrameStackModel>(this, &FrameStackModel::handleStack);
        session()->connection()->sendCommand("stack_get", QStringList(), QByteArray(), cb);
    }
}

}
