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

#ifndef FORMATTER_H
#define FORMATTER_H

#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtCore/QVariant>
#include <QProcess>

class KProcess;

namespace Php
{

class Formatter : public QObject
{
    Q_OBJECT
public:
    Formatter();
    virtual QString formatSource ( const QString& text, const QString& leftContext = QString(), const QString& rightContext = QString() );

    virtual void setDebugEnabled(bool enabled) {
        m_debugEnabled = enabled;
    }

    static QMap<QString, QVariant> stringToOptionMap(const QString &str);

    virtual void loadStyle ( const QString &content );
    virtual void loadStyle ( const QMap<QString, QVariant> & options ) {m_options = options;}

    virtual const QMap<QString, QVariant> & options() const {
        return m_options;
    }

    virtual QMap<QString, QVariant> & options() {
        return m_options;
    }

    virtual QString saveStyle();

protected:
    QMap<QString, QVariant> m_options;
    bool m_debugEnabled;
    QString m_command;
    KProcess * m_process;
    QString m_stdout;
    QString m_stderr;
    int m_returnStatus;
protected slots:
    void readDataOutput();
    void readDataError();
    void processFinished( int, QProcess::ExitStatus );
};

}

#endif // FORMATTER_H
