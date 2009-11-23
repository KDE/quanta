/*
* Crossfire Debugger Support
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

#ifndef CROSSFIRE_DEBUGJOB_H
#define CROSSFIRE_DEBUGJOB_H

#include <KUrl>

#include <outputview/outputjob.h>

namespace KDevelop {
    class ILaunchConfiguration;
    class OutputModel;
}

namespace Crossfire {

class DebugSession;

class DebugJob : public KDevelop::OutputJob
{
    Q_OBJECT
public:
    DebugJob( DebugSession* session, KDevelop::ILaunchConfiguration*, QObject* parent = 0 );
    virtual void start();

    int browserPid() const { return m_browserPid; }

protected:
    virtual bool doKill();

private slots:
    void sessionFinished();
    void outputLine(QString);

private:
    KDevelop::OutputModel *model();

    KUrl m_url;
    DebugSession* m_session;
    int m_browserPid;
};

}

#endif // CROSSFIRE_DEBUGJOB_H
