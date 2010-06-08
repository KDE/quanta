/*****************************************************************************
 * Copyright (c) 2010 Ruan Strydom <rm3dom@gmail.com>                        *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
 *****************************************************************************/

#include "formatter.h"
#include <interfaces/isourceformatter.h>
#include <KStandardDirs>
#include <KProcess>
#include <KDebug>

using namespace Php;

Formatter::Formatter()
{
    m_debugEnabled = false;
    QString phpFile = KStandardDirs::locate("data", "kdevphpformatter/phpStylist.php");
    if (!phpFile.isEmpty())
        m_command = QString("php -f '%1' -").arg(phpFile);
    else
        kDebug() << "Unable to locate kdevphpformatter/phpStylist.php";
    m_process = new KProcess ( this );
    connect ( m_process, SIGNAL(readyReadStandardOutput()), this, SLOT (readDataOutput()) );
    connect ( m_process, SIGNAL(readyReadStandardError()), this, SLOT(readDataError()) );
    connect ( m_process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(processFinished(int, QProcess::ExitStatus)) );
    
    m_options.insert("indent_size", "4");
}


void Formatter::loadStyle ( const QString& content ) {
    if ( content.isNull() )
        return;
    m_options = KDevelop::ISourceFormatter::stringToOptionMap ( content );
}

QString Formatter::saveStyle() {
    return KDevelop::ISourceFormatter::optionMapToString ( m_options );
}

QString Formatter::formatSource(const QString& text, const QString& leftContext, const QString& rightContext)
{
    m_stdout.clear ();
    m_stderr.clear ();
    m_process->clearProgram ();
    
    QString options;
    foreach(const QString &key, m_options.keys()) {
        if(key == "indent_size")
            options += QString(" --%1 %2").arg(key, m_options.value(key).toString());
        else if(m_options.value(key).toBool() == true)
            options += QString(" --%1").arg(key);
    }
    
    QString source = text;
    source = source.replace('\\',"\\134");
    source = source.replace('"',"\\042");
    source = source.replace('%',"\\045");
    source = source.replace('\n',"\\012");
    source = source.replace('\r',"\\015");
    source = source.replace('\'',"\\047");
    m_process->setShellCommand ( QString("printf '%1' | %2 - %3").arg(source, m_command, options));
    m_process->setOutputChannelMode ( KProcess::SeparateChannels );
    int exitCode = m_process->execute();
    return m_stdout;
}

void Formatter::processFinished(int ret, QProcess::ExitStatus stat)
{
    if ( stat == QProcess::CrashExit )
        kDebug() << "The process crashed";
    else
        kDebug() << QString("Exit status: %1").arg(ret);
}

void Formatter::readDataError()
{
    //TODO use a codec
    m_stderr += QString(m_process->readAllStandardError());
    kDebug() << m_stderr;
}

void Formatter::readDataOutput()
{
    //TODO use a codec
    m_stdout += QString(m_process->readAllStandardOutput());
    kDebug() << m_stdout;
}

#include "formatter.moc"

