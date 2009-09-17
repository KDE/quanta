/*  This file is part of KDevelop
    Copyright 2009 Andreas Pakulat <apaku@gmx.de>
    Copyright 2009 Niko Sams <niko.sams@gmail.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "scriptappjob.h"

#include <QFileInfo>

#include <kprocess.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kconfiggroup.h>
#include <kdebug.h>
#include <kparts/mainwindow.h>

#include <interfaces/ilaunchconfiguration.h>
#include <outputview/outputmodel.h>
#include <util/processlinemaker.h>
#include <util/environmentgrouplist.h>

#include <kshell.h>
#include <interfaces/icore.h>
#include <interfaces/iuicontroller.h>
#include <interfaces/iplugincontroller.h>
#include <project/projectmodel.h>

#include "iexecutescriptplugin.h"

ScriptAppJob::ScriptAppJob(QObject* parent, KDevelop::ILaunchConfiguration* cfg)
    : KDevelop::OutputJob( parent ), proc(0)
{
    kDebug() << "creating script app job";
    setCapabilities(Killable);
    
    IExecuteScriptPlugin* iface = KDevelop::ICore::self()->pluginController()->pluginForExtension("org.kdevelop.IExecuteScriptPlugin")->extension<IExecuteScriptPlugin>();
    Q_ASSERT(iface);
        
    KDevelop::EnvironmentGroupList l(KGlobal::config());
    QString envgrp = iface->environmentGroup(cfg);
    
    QString err;
    QString interpreter = iface->interpreter( cfg, err );
    
    if( !err.isEmpty() ) 
    {
        setError( -1 );
        setErrorText( err );
        return;
    }

    KUrl script = iface->script( cfg, err );

    if( !err.isEmpty() )
    {
        setError( -3 );
        setErrorText( err );
        return;
    }

    if( envgrp.isEmpty() )
    {
        kWarning() << "Launch Configuration:" << cfg->name() << i18n("No environment group specified, looks like a broken "
                       "configuration, please check run configuration '%1'. "
                       "Using default environment group.", cfg->name() );
        envgrp = l.defaultGroup();
    }
    
    QStringList arguments = iface->arguments( cfg, err );
    if( !err.isEmpty() ) 
    {
        setError( -2 );
        setErrorText( err );
    }
    
    if( error() != 0 )
    {
        kWarning() << "Launch Configuration:" << cfg->name() << "oops, problem" << errorText();
        return;
    }
    arguments.prepend(script.toLocalFile());
    
    proc = new KProcess( this );
    
    lineMaker = new KDevelop::ProcessLineMaker( proc, this );
    
    setStandardToolView(KDevelop::IOutputView::RunView);
    setBehaviours(KDevelop::IOutputView::AllowUserClose | KDevelop::IOutputView::AutoScroll);
    setModel( new KDevelop::OutputModel(), KDevelop::IOutputView::TakeOwnership );
    
    connect( lineMaker, SIGNAL(receivedStdoutLines(const QStringList&)), model(), SLOT(appendLines(QStringList)) );
    connect( proc, SIGNAL(error(QProcess::ProcessError)), SLOT(processError(QProcess::ProcessError)) );
    connect( proc, SIGNAL(finished(int,QProcess::ExitStatus)), SLOT(processFinished(int,QProcess::ExitStatus)) );

    // Now setup the process parameters
    
    proc->setEnvironment( l.createEnvironment( envgrp, proc->systemEnvironment()) );
    KUrl wc = iface->workingDirectory( cfg );
    if( !wc.isValid() || wc.isEmpty() )
    {
        wc = KUrl( QFileInfo( script.toLocalFile() ).absolutePath() );
    }
    proc->setWorkingDirectory( wc.toLocalFile() );
    proc->setProperty( "executable", interpreter );
    
    kDebug() << "setting app:" << interpreter << arguments;
    
    proc->setOutputChannelMode(KProcess::MergedChannels);
    
    proc->setProgram( interpreter, arguments );
    
    setTitle(cfg->name());
}


void ScriptAppJob::start()
{
    kDebug() << "launching?" << proc;
    if( proc )
    {
        startOutput();
        model()->appendLine( i18n("Starting: %1", proc->program().join(" ") ) );
        proc->start();
    } else
    {
        kWarning() << "No process, something went wrong when creating the job";
        // No process means we've returned early on from the constructor, some bad error happened
        emitResult();
    }
}

bool ScriptAppJob::doKill()
{
    if( proc ) {
        proc->kill();
        model()->appendLine( i18n( "*** Killed Application ***" ) );
    }
    return true;
}


void ScriptAppJob::processFinished( int exitCode , QProcess::ExitStatus status )
{
    if (exitCode == 0 && status == QProcess::NormalExit)
        model()->appendLine( i18n("*** Exited normally ***") );
    else
        if (status == QProcess::NormalExit)
            model()->appendLine( i18n("*** Exited with return code: %1 ***", QString::number(exitCode)) );
        else 
            if (error() == KJob::KilledJobError)
                model()->appendLine( i18n("*** Process aborted ***") );
            else
                model()->appendLine( i18n("*** Crashed with return code: %1 ***", QString::number(exitCode)) );
    kDebug() << "Process done";
    emitResult();
}

void ScriptAppJob::processError( QProcess::ProcessError error )
{
    if( error == QProcess::FailedToStart )
    {
        setError( -1 );
        QString errmsg =  i18n("Could not start program '%1'. Make sure that the "
                           "path is specified correctly.", proc->property("executable").toString() );
        KMessageBox::error( KDevelop::ICore::self()->uiController()->activeMainWindow(), errmsg, i18n("Could not start application") );
        setErrorText( errmsg );
        emitResult();
    }
    kDebug() << "Process error";
}

KDevelop::OutputModel* ScriptAppJob::model()
{
    return dynamic_cast<KDevelop::OutputModel*>( OutputJob::model() );
}


#include "scriptappjob.moc"
