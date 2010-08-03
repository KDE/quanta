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
#include <unistd.h>

using namespace Php;

Formatter::Formatter() :  m_debugEnabled(false), m_process(0)
{
    QString phpFile = KStandardDirs::locate("data", "kdevphpformatter/phpStylist.php");
    if (!phpFile.isEmpty())
        m_command = phpFile;
    else
        kDebug() << "Unable to locate kdevphpformatter/phpStylist.php";

    m_options.insert("indent_size", "4");
    m_options.insert("keep_redundant_lines", true);
    m_options.insert("else_along_curly", true);
    m_options.insert("space_after_if", true);
}

QMap< QString, QVariant > Formatter::stringToOptionMap(const QString& str)
{
    QMap<QString, QVariant> map;
    if ( str.isEmpty() )
        return map;
    QStringList pairs = str.split(',', QString::SkipEmptyParts);
    QStringList::const_iterator it;
    for (it = pairs.constBegin(); it != pairs.constEnd(); ++it) {
        QStringList bits = (*it).split('=');
        if(bits.size() == 2)
            map[bits[0].trimmed()] = bits[1];
    }
    return map;
}

void Formatter::loadStyle ( const QString& content ) {
    if ( content.isNull() )
        return;
    kDebug() << content;
    m_options = stringToOptionMap(content);
}

QString Formatter::saveStyle() {
    return KDevelop::ISourceFormatter::optionMapToString ( m_options );
}

QString Formatter::formatSource(const QString& text, const QString& leftContext, const QString& rightContext)
{
    m_stdout.clear ();
    m_stderr.clear ();
    m_returnStatus = 0;
    if(m_process) {
        m_process->deleteLater();
        m_process = 0;
    }

    QString options;
    foreach(const QString &key, m_options.keys()) {
        if(key == "indent_size")
            options += QString(" --%1 %2").arg(key, m_options.value(key).toString());
        else if(m_options.value(key).isValid() && m_options.value(key).toBool() == true)
            options += QString(" --%1").arg(key);
    }

    QByteArray source = text.toUtf8();
    
    m_process = new KProcess ( this );
    if(!m_process) return text;
    
    connect ( m_process, SIGNAL(readyReadStandardOutput()), this, SLOT (readDataOutput()) );
    connect ( m_process, SIGNAL(readyReadStandardError()), this, SLOT(readDataError()) );
    connect ( m_process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(processFinished(int, QProcess::ExitStatus)) );

    QString command = QString("php -f '%1' - %2").arg(m_command, options);
    if(m_debugEnabled)
        kDebug() << command;

    m_process->setShellCommand ( command );
    m_process->setOutputChannelMode ( KProcess::SeparateChannels );
    m_process->start();
    if (m_process->waitForStarted()) {
        if(m_process->write(source) > -1) {
            m_process->closeWriteChannel();
            if(m_process->waitForFinished() 
                && m_stdout.size() > 0 
                && m_returnStatus == 0) {
                m_process->close();
                m_process->deleteLater();
                m_process = 0;
                return m_stdout;
            }
        }
    }
    
    m_process->waitForFinished();
    m_process->close();
    m_process->deleteLater();
    m_process = 0;
    
    return text;
}

void Formatter::processFinished(int ret, QProcess::ExitStatus stat)
{
    if(!m_process) return;
    m_returnStatus = ret;
    if ( stat == QProcess::CrashExit )
        m_returnStatus = 255;
    if(m_debugEnabled)
        kDebug() << "Process finished with exit code: " << m_returnStatus;
}

void Formatter::readDataError()
{
    if(!m_process) return;
    //TODO use a codec
    m_stderr += QString(m_process->readAllStandardError());
    if(m_debugEnabled)
        kDebug() << m_stderr;
}

void Formatter::readDataOutput()
{
    if(!m_process) return;
    //TODO use a codec
    m_stdout += QString(m_process->readAllStandardOutput());
    if(m_debugEnabled)
        kDebug() << m_stdout;
}

#include "formatter.moc"

