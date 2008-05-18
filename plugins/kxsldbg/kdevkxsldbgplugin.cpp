/**
 *
 *  This file is part of the kdewebdev package
 *  Copyright (c) 2008 Keith Isdale <keith@kdewebdev.org>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License version 2 as published by the Free Software Foundation.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 **/

#include <core.h>
#include <iproject.h>
#include <iprojectcontroller.h>
#include <idocumentcontroller.h>
#include <iuicontroller.h>
#include "kdevkxsldbgplugin.h"
#include "kdevkxsldbgview.h"
#include <klibloader.h>
#include <kparts/mainwindow.h>
#include <kmessagebox.h>
#include <kactionmenu.h>
#include <kpluginfactory.h>
#include <kpluginloader.h>
#include <kapplication.h>
#include <kaction.h>
#include <kactioncollection.h>
#include <kaboutdata.h>
#include <kmainwindow.h>
#include <kxmlguiwindow.h>
#include <ksettings/dispatcher.h>

#include <libxsldbg/files.h>
#include <libxsldbg/utils.h>
#include <libxsldbg/xsldbg.h>
#include <libxsldbg/options.h>

#include <kcomponentdata.h>
#include <kaction.h>
#include <kstandardaction.h>
#include <kfiledialog.h>
#include <kparts/genericfactory.h>
#include <ktexteditor/markinterface.h>
#include <ktexteditor/editor.h>

#include <QFile>
#include <QTextStream>

//#include "../kxsldbg.h"
#include <kcmdlineargs.h>
#include <kiconloader.h>
#include <QMessageBox>
#include <klocale.h>
#include <kdeversion.h>
#include <kinputdialog.h>

// Qxsldbg specific includes
#include "qxsldbgdoc.h"
#include <QVariant>
#include <QStatusBar>
#include <QSplitter>
#include <QDialogButtonBox>

#include <QLayout>
#include <QToolTip>
#include <QAction>
#include <QMenuBar>
#include <QToolBar>
#include <QImage>
#include <QPixmap>
#include <QTextBrowser>
#include <QMessageBox>
#include <QLineEdit>
#include <QPushButton>
#include <QInputDialog>
#include <QObject>
#include <QStackedWidget>
#include "xsldbgoutputview.h"
#include "xsldbgconfigimpl.h"
#include <kdebug.h>
#include <kvbox.h>
#include <kactioncollection.h>
#include <ksettings/dispatcher.h>
// kate specific classes
#include <ktexteditor/document.h>
#include <ktexteditor/markinterface.h>

#include <ioutputview.h>
#include <iplugincontroller.h>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QItemDelegate>
#include "configdialogprivate.h"

K_PLUGIN_FACTORY(KDevKXSLDbgFactory, registerPlugin<KDevKXSLDbgPlugin>(); )
K_EXPORT_PLUGIN(KDevKXSLDbgFactory("kdevkxsldbg"))

class KDevKXSLDbgPlugin;

using namespace KTextEditor;

class KDevKXSLDbgViewFactory : public KDevelop::IToolViewFactory
{
    public:
        KDevKXSLDbgViewFactory(KDevKXSLDbgPlugin *thePart)
            :  myplugin(thePart)
        {

        }
        /**
          Return the main widget for our tool
         */
        virtual QWidget *create(QWidget * parent = 0 );

        /**
          Return a hint as to where to put our tool
         */
        virtual Qt::DockWidgetArea defaultPosition();

        virtual QString id() const;

    private:

        KDevKXSLDbgPlugin * myplugin;
};


QWidget* KDevKXSLDbgViewFactory::create(QWidget * parent)
{
    QWidget *w = new KDevKXSLDbgView(parent, myplugin) ;

    return w;
}

Qt::DockWidgetArea KDevKXSLDbgViewFactory::defaultPosition()
{
    return Qt::BottomDockWidgetArea;
}

QString KDevKXSLDbgViewFactory::id() const
{
    return "org.quanta.KXSLDbg";
}

    KDevKXSLDbgPlugin::KDevKXSLDbgPlugin(QObject *parent, const QVariantList &args)
: KDevelop::IPlugin(KDevKXSLDbgFactory::componentData(),parent)
{
    connect(core()->projectController(), SIGNAL(projectOpened(KDevelop::IProject*)),
                   this, SLOT(projectOpened(KDevelop::IProject*)));
    connect(core()->projectController(), SIGNAL(projectClosed(KDevelop::IProject*)),
                   this, SLOT(projectClosed(KDevelop::IProject*)));
    connect(KDevelop::ICore::self()->documentController(), SIGNAL(documentClosed(KDevelop::IDocument*)),
            this, SLOT(documentClosed(KDevelop::IDocument*)));
    Q_UNUSED(args)
    setXMLFile( "kdevkxsldbg.rc");
    currentColumnNo = 0;
    inspector = 0L;
    debugger = 0L;
    configWidget = 0L;
    dev_outputModel = 0L;
    dev_outputView = 0L;
    dlg = 0L;
    lastProject = 0L;
    toolID = -1;
    optionSetAutoConfig(false);

    outputModel();
    setupActions();
}

KDevKXSLDbgPlugin::~KDevKXSLDbgPlugin()
{
  //  delete dlg;
 //   delete debugger;
 //   delete inspector;
}

void KDevKXSLDbgPlugin::setupActions()
{
    KActionCollection* ac = actionCollection();
    m_projectKXSLDbgActionMenu = new KActionMenu(i18n("Run Project via KXSLDbg"), this);
    m_projectKXSLDbgActionMenu->setIcon(KIcon("xsldbg_source"));
    m_projectKXSLDbgActionMenu->setToolTip(i18n("Run project"));
    m_projectKXSLDbgActionMenu->setVisible(false);

    ac->addAction("project_kxsldbg_run", m_projectKXSLDbgActionMenu);
    connect(m_projectKXSLDbgActionMenu, SIGNAL(triggered(bool)), SLOT(showKXSLDbg()));
    // create our actions
    KStandardAction::open(this, SLOT(fileOpen()), actionCollection());

    configureCmd = ac->addAction( "configureCmd" );
    configureCmd->setText( i18n("Configure...") );
    configureCmd->setIcon( KIcon("configure") );
    connect(configureCmd, SIGNAL(triggered(bool)), SLOT(configureCmd_activated()));
    configureCmd->setShortcut(QKeySequence(Qt::Key_C));
    xsldbgActions.append(configureCmd);

    inspectCmd = ac->addAction( "inspectCmd" );
    inspectCmd->setText( i18n("Inspect...") );
    inspectCmd->setIcon( KIcon("edit-find") );
    connect(inspectCmd, SIGNAL(triggered(bool)), SLOT(inspectorCmd_activated()));
    inspectCmd->setShortcut(QKeySequence(Qt::Key_I));
    xsldbgActions.append(inspectCmd);


    // Motions commands
    runCmd = ac->addAction( "runCmd" );
    runCmd->setText( i18n("Run") );
    runCmd->setIcon( KIcon("system-run") );
    connect(runCmd, SIGNAL(triggered(bool)), SLOT(runCmd_activated()));
    runCmd->setShortcut(QKeySequence(Qt::Key_F5));
    xsldbgActions.append(runCmd);

    continueCmd = ac->addAction( "continueCmd" );
    continueCmd->setText( i18n("Continue") );
    continueCmd->setIcon( KIcon("media-playback-start") );
    connect(continueCmd, SIGNAL(triggered(bool)), SLOT(continueCmd_activated()));
    continueCmd->setShortcut(QKeySequence(Qt::Key_F4));
    xsldbgActions.append(continueCmd);

    stepCmd = ac->addAction( "stepCmd" );
    stepCmd->setText( i18n("Step") );
    stepCmd->setIcon( KIcon("step") );
    connect(stepCmd, SIGNAL(triggered(bool)), SLOT(stepCmd_activated()));
    stepCmd->setShortcut(QKeySequence(Qt::Key_F8));
    xsldbgActions.append(stepCmd);

    nextCmd  = new KAction(KIcon("go-down-search"), i18n("Next"), this);
    ac->addAction("nextCmd", nextCmd );
    connect(nextCmd, SIGNAL(triggered(bool)), SLOT(nextCmd_activated()));
    nextCmd->setShortcut(QKeySequence(Qt::Key_F10));
    xsldbgActions.append(nextCmd);

    stepupCmd  = new KAction(KIcon("xsldbg_stepup"), i18n("Step Up"), this);
    ac->addAction("stepupCmd", stepupCmd );
    connect(stepupCmd, SIGNAL(triggered(bool)), SLOT(stepupCmd_activated()));
    stepupCmd->setShortcut(QKeySequence(Qt::Key_F6));
    xsldbgActions.append(stepupCmd);

    stepdownCmd  = new KAction(KIcon("xsldbg_stepdown"), i18n("Step Down"), this);
    ac->addAction("stepdownCmd", stepdownCmd );
    connect(stepdownCmd, SIGNAL(triggered(bool)), SLOT(stepdownCmd_activated()));
    stepdownCmd->setShortcut(QKeySequence(Qt::Key_F7));
    xsldbgActions.append(stepdownCmd);

    // Breakpoint commands
    breakCmd  = new KAction(KIcon("xsldbg_break"), i18n("Break"), this);
    ac->addAction("breakCmd", breakCmd );
    connect(breakCmd, SIGNAL(triggered(bool)), SLOT(breakCmd_activated()));
    breakCmd->setShortcut(QKeySequence(Qt::Key_F2));
    xsldbgActions.append(breakCmd);

    enableCmd  = new KAction(KIcon("xsldbg_enable"), i18n("Enable/Disable"), this);
    ac->addAction("enableCmd", enableCmd );
    connect(enableCmd, SIGNAL(triggered(bool)), SLOT(enableCmd_activated()));
    enableCmd->setShortcut(QKeySequence(Qt::Key_F3));
    xsldbgActions.append(enableCmd);

    deleteCmd  = new KAction(KIcon("xsldbg_delete"), i18n("Delete"), this);
    ac->addAction("deleteCmd", deleteCmd );
    connect(deleteCmd, SIGNAL(triggered(bool)), SLOT(deleteCmd_activated()));
    deleteCmd->setShortcut(QKeySequence(Qt::Key_Delete));
    xsldbgActions.append(deleteCmd);

    sourceCmd  = new KAction(KIcon("xsldbg_source"), i18n("&Source"), this);
    ac->addAction("sourceCmd", sourceCmd );
    connect(sourceCmd, SIGNAL(triggered(bool)), SLOT(sourceCmd_activated()));
    sourceCmd->setShortcut(QKeySequence(Qt::Key_S));
    xsldbgActions.append(sourceCmd);

    dataCmd  = new KAction(KIcon("xsldbg_data"), i18n("&Data"), this);
    ac->addAction("dataCmd", dataCmd );
    connect(dataCmd, SIGNAL(triggered(bool)), SLOT(dataCmd_activated()));
    dataCmd->setShortcut(QKeySequence(Qt::Key_D));
    xsldbgActions.append(dataCmd);

    outputCmd  = new KAction(KIcon("xsldbg_output"), i18n("&Output"), this);
    ac->addAction("outputCmd", outputCmd );
    connect(outputCmd, SIGNAL(triggered(bool)), SLOT(outputCmd_activated()));
    outputCmd->setShortcut(QKeySequence(Qt::Key_O));
    xsldbgActions.append(outputCmd);

    refreshCmd  = new KAction(KIcon("xsldbg_refresh"), i18n("Reload"), this);
    ac->addAction("refreshCmd", refreshCmd );
    connect(refreshCmd, SIGNAL(triggered(bool)), SLOT(refreshCmd_activated()));
    refreshCmd->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F5));
    xsldbgActions.append(refreshCmd);

    /* tracing and walking */
    walkCmd  = new KAction(i18n("Walk Through Stylesheet..."), this);
    ac->addAction("walkCmd", walkCmd );
    connect(walkCmd, SIGNAL(triggered(bool)), SLOT(walkCmd_activated()));
    walkCmd->setShortcut(QKeySequence(Qt::Key_W));
    xsldbgActions.append(walkCmd);

    walkStopCmd  = new KAction(i18n("Stop Wal&king Through Stylesheet"), this);
    ac->addAction("walkStopCmd", walkStopCmd );
    connect(walkStopCmd, SIGNAL(triggered(bool)), SLOT(walkStopCmd_activated()));
    walkStopCmd->setShortcut(QKeySequence(Qt::Key_K));
    xsldbgActions.append(walkStopCmd);

    traceCmd  = new KAction(i18n("Tr&ace Execution of Stylesheet"), this);
    ac->addAction("traceCmd", traceCmd );
    connect(traceCmd, SIGNAL(triggered(bool)), SLOT(traceCmd_activated()));
    traceCmd->setShortcut(QKeySequence(Qt::Key_A));
    xsldbgActions.append(traceCmd);

    traceStopCmd  = new KAction(i18n("Stop Tracing of Stylesheet"), this);
    ac->addAction("traceStopCmd", traceStopCmd );
    connect(traceStopCmd, SIGNAL(triggered(bool)), SLOT(traceStopCmd_activated()));
    traceStopCmd->setShortcut(QKeySequence(Qt::Key_K));
    xsldbgActions.append(traceStopCmd);

    evaluateCmd  = new KAction(i18n("&Evaluate Expression..."), this);
    ac->addAction("evaluateCmd", evaluateCmd );
    connect(evaluateCmd, SIGNAL(triggered(bool)), SLOT(evaluateCmd_activated()));
    evaluateCmd->setShortcut(QKeySequence(Qt::Key_E));
    xsldbgActions.append(evaluateCmd);

    gotoXPathCmd  = new KAction(i18n("Goto &XPath..."), this);
    ac->addAction("gotoXPathCmd", gotoXPathCmd );
    connect(gotoXPathCmd, SIGNAL(triggered(bool)), SLOT(gotoXPathCmd_activated()));
    gotoXPathCmd->setShortcut(QKeySequence(Qt::Key_X));
    xsldbgActions.append(gotoXPathCmd);

    lookupSystemCmd  = new KAction(i18n("Lookup SystemID..."), this);
    ac->addAction("lookupSystemID", lookupSystemCmd );
    connect(lookupSystemCmd, SIGNAL(triggered(bool)), SLOT(slotLookupSystemID()));
    xsldbgActions.append(lookupSystemCmd);

    lookupPublicIDCmd  = new KAction(i18n("Lookup PublicID..."), this);
    ac->addAction("lookupPublicID", lookupPublicIDCmd );
    connect(lookupPublicIDCmd, SIGNAL(triggered(bool)), SLOT(slotLookupPublicID()));
    xsldbgActions.append(lookupPublicIDCmd);

    foreach (QAction *action, xsldbgActions)
        action->setVisible(false);
}

// toggle "run Project with kxsldbg"
void  KDevKXSLDbgPlugin::showKXSLDbg()
{
    static bool setupWidgets=false;
    static bool runProject=true;

    if (!setupWidgets){
        // add the output view
        Q_ASSERT(outputModel());

        // add the KXSLDbg tool
        m_widgetFactory = new KDevKXSLDbgViewFactory(this);
        core()->uiController()->addToolView("KXSLDbg", m_widgetFactory );

        /* We must have a valid debugger and inspector */
        createInspector();

        if (checkDebugger()){
            dlg = new  ConfigDialogPrivate(0);
            Q_CHECK_PTR(dlg);
            configWidget = dlg->configWidget;
            // create the configuration dialog 
            setupWidgets=true;
        }
        setupWidgets = true;
    }

    if (runProject) {
        // start running this project
        m_projectKXSLDbgActionMenu->setText(i18n("Stop running Project in KXSLDbg"));
        //enable the debugger actions
        foreach (QAction *action, xsldbgActions) 
            action->setVisible(true);
        if (debugger)
            debugger->start();
    }else {
        // stop running this project
        m_projectKXSLDbgActionMenu->setText(i18n("Run Project via KXSLDbg"));
        if (debugger){
            saveProfile();
            debugger->stop();
        }
        //disable the debugger actions
        foreach (QAction *action, xsldbgActions) 
            action->setVisible(false);
        if (dlg)
            dlg->hide();
    }
    runProject = !runProject;

}

void KDevKXSLDbgPlugin::documentClosed( KDevelop::IDocument* document )
{
    Q_ASSERT(document);
    Q_UNUSED(document);
    // TODO handle this case
}

void KDevKXSLDbgPlugin::quit()
{
    qWarning("Custom void KDevKXSLDbgPlugin::quit()");
    closeUrl();
}


bool  KDevKXSLDbgPlugin::openUrl(const KUrl &url)
{
    QString docID = url.url();
    bool result = fetchURL(url);

    if (!result){
        qWarning() << "Open URL failed" << __FILE__ << ":" << __LINE__;
    }else {
        currentFileName = docID;
    }

    return result;
}


/* Don't show the content of URL just loaded it into our data structures */
bool  KDevKXSLDbgPlugin::fetchURL(const KUrl &url)
{
    KDevelop::IDocument* document;
    bool foundDoc = false;

    foreach (document,
            KDevelop::ICore::self()->documentController()->openDocuments()) {
        if (document->url() == url) {
            foundDoc = true;
            break;
        }
    }

    if (!foundDoc)
        document = KDevelop::ICore::self()->documentController()->openDocument(url);

    if (!document)
        qWarning() << "Unable to fetch URL" <<  url;

    return document != 0;
}

bool KDevKXSLDbgPlugin::openFile()
{
    qWarning("bool KDevKXSLDbgPlugin::openFile() called");
    return false;
}

bool KDevKXSLDbgPlugin::closeUrl()
{
    return true;
}

void KDevKXSLDbgPlugin::fileOpen()
{
    // this slot is called whenever the File->Open menu is selected,
    // the Open shortcut is pressed (usually CTRL+O) or the Open toolbar
    // button is clicked
    QString file_name = KFileDialog::getOpenFileName();

    if (file_name.isEmpty() == false)
        openUrl(KUrl( file_name ));
}

bool KDevKXSLDbgPlugin::checkDebugger(bool showWarningDialog)
{
    bool result = debugger != 0L;
    if (!result && showWarningDialog){
        QMessageBox::information(0L, i18n("Debugger Not Ready"),
                i18n("Configure and start the debugger first."),
                QMessageBox::Ok);
    }

    return result;
}


void KDevKXSLDbgPlugin::lookupSystemID( QString systemID)
{
    bool ok = false;
    if (!checkDebugger())
        return;

    if (systemID.isEmpty()){
        systemID = KInputDialog::getText(
                i18n( "Lookup SystemID" ),
                i18n( "Please enter SystemID to find:" ),
                QString(), &ok,
                0);
    }else{
        ok = true;
    }
    if ( ok && !systemID.isEmpty() ){
        // user entered something and pressed ok
        QString msg(QString("system %1").arg(systemID));  // noTr
        debugger->fakeInput(msg, true);
    }

}


void KDevKXSLDbgPlugin::lookupPublicID(QString publicID)
{
    bool ok = false;
    if (!checkDebugger())
        return;

    if (publicID.isEmpty()){
        publicID = KInputDialog::getText(
                i18n( "Lookup PublicID" ),
                i18n( "Please enter PublicID to find:" ),
                QString(), &ok, 0);
    }else{
        ok = true;
    }
    if ( ok && !publicID.isEmpty()){
        // user entered something and pressed ok
        QString msg(QString("public %1").arg(publicID));  // noTr
        debugger->fakeInput(msg, true);
    }
}


void KDevKXSLDbgPlugin::slotLookupSystemID()
{
    lookupSystemID("");
}

void KDevKXSLDbgPlugin::slotLookupPublicID()
{
    lookupPublicID("");
}

void KDevKXSLDbgPlugin::configureCmd_activated()
{
    if (!checkDebugger() || !dlg || !configWidget)
        return;

    configWidget->setModel(optionDataModel());

    dlg->show();
}

    void
KDevKXSLDbgPlugin::runCmd_activated()
{
    if ( checkDebugger() )
        debugger->slotRunCmd();
}

void KDevKXSLDbgPlugin::inspectorCmd_activated()
{
    if (inspector == 0L)
        createInspector();

    if (checkDebugger() && (inspector != 0L)){
        inspector->show();
    }
}


void KDevKXSLDbgPlugin::showMessage(QString message)
{
    bool processed = false;
    // Is this a result of an evaluate command
    if ((message[0] == QChar('=')) && (message[1] == QChar(' '))){
        int endPosition = message.indexOf(QChar('\n'));
        if (endPosition >= 0){
            processed = true;
            QMessageBox::information(0, i18n("Result of evaluation"),
                    message.mid(endPosition + 1));
        }
    }else {
        /* Is there some sort of error message in message */
        if ((message.indexOf("Error:") != -1) ||
                (message.indexOf("Warning:") != -1) ||
                (message.indexOf("Request to xsldbg failed") != -1) ||
                /* the following errors are libxml or libxslt generated */
                (message.indexOf("error:") != -1) ||
                (message.indexOf("xmlXPathEval:") != -1) ||
                (message.indexOf("runtime error")  != -1)) {
            /* OK we've found an error but ingore any errors about
               data or source files */
            if ((message.indexOf("Error: No XSL source file supplied") == -1) &&
                    (message.indexOf("Error: No XML data file supplied") == -1) &&
                    (message.indexOf("Load of source deferred") == -1) &&
                    (message.indexOf("Load of data deferred") == -1) )
                QMessageBox::warning(0, i18n("Request Failed "), message);
            processed = true;
        }
    }
    if (outputModel() && !processed){
        QStandardItem *item = new QStandardItem(message);
        if (item)
            outputModel()->appendRow(item);
    }

}

void KDevKXSLDbgPlugin::createInspector()
{
    if ( inspector == 0L ) {
        debugger = new XsldbgDebugger();
        Q_CHECK_PTR( debugger );
        if ( debugger != 0L ) {
            connect(debugger, SIGNAL( debuggerReady()),
                    this, SLOT(debuggerStarted()));
            connect(debugger,
                    SIGNAL( showMessage(QString /* msg*/)),
                    this,
                    SLOT(showMessage(QString /* msg*/)));
            inspector = new XsldbgInspector( debugger );
            Q_CHECK_PTR( inspector );
            debugger->setInspector( inspector );
            if (inspector != 0L){
                /*process line number and/or file name changed */
                connect(debugger,
                        SIGNAL(lineNoChanged
                            (QString /* fileName */ ,
                             int /* lineNumber */ ,
                             bool /* breakpoint */ ) ),
                        this,
                        SLOT(lineNoChanged
                            ( QString /* fileName */ ,
                              int /* lineNumber */ ,
                              bool /* breakpoint */ ) ) );
                connect(debugger,
                        SIGNAL(breakpointItem(QString /* fileName*/,
                                int /* lineNumber */,
                                QString /*templateName*/,
                                QString /* modeName */,
                                bool /* enabled */,
                                int /* id */)),
                        this,
                        SLOT( breakpointItem(QString /* fileName*/,
                                int /* lineNumber */,
                                QString /*templateName*/,
                                QString /* modeName */,
                                bool /* enabled */,
                                int /* id */)));
                connect(debugger, SIGNAL(resolveItem(QString /*URI*/)),
                        this, SLOT(slotProcResolveItem(QString /*URI*/)));
            }
        }
    }
}

const KUrl KDevKXSLDbgPlugin::currentFile()
{
    KUrl url;
    Q_ASSERT(KDevelop::ICore::self());
    Q_ASSERT(KDevelop::ICore::self()->documentController());
    Q_ASSERT(KDevelop::ICore::self()->documentController()->activeDocument());
    KDevelop::IDocument* document = KDevelop::ICore::self()->documentController()->activeDocument();
    if (document){
        url = document->url();
        currentFileName = url.url();
    }
    return url;
}
int KDevKXSLDbgPlugin::currentLineNo()
{
    int lineNo = -1;
    KDevelop::IDocument* document = KDevelop::ICore::self()->documentController()->activeDocument();
    if (document && document->textDocument() && document->textDocument()->activeView()){
        KTextEditor::Cursor cursor = document->textDocument()->activeView()->cursorPosition();
        lineNo = cursor.line() + 1;
    }
    return lineNo;
}

void KDevKXSLDbgPlugin::emitOpenFile(QString file, int line, int row)
{
    Q_UNUSED(file);
    Q_UNUSED(line);
    Q_UNUSED(row);
    qWarning("%s %s", __PRETTY_FUNCTION__, "called but not supported");
}

void KDevKXSLDbgPlugin::continueCmd_activated()
{
    if ( checkDebugger() )
        debugger->slotContinueCmd();

}

void KDevKXSLDbgPlugin::stepCmd_activated()
{
    if ( checkDebugger() )
        debugger->slotStepCmd();
}

void KDevKXSLDbgPlugin::nextCmd_activated()
{
    if ( checkDebugger() )
        debugger->fakeInput("next", true);  // noTr
}


void KDevKXSLDbgPlugin::stepupCmd_activated()
{
    if ( checkDebugger() )
        debugger->fakeInput("stepup", true);  // noTr
}


void KDevKXSLDbgPlugin::stepdownCmd_activated()
{
    if ( checkDebugger() )
        debugger->fakeInput("stepdown", true);  // noTr
}


void KDevKXSLDbgPlugin::dataCmd_activated()
{
    if ( checkDebugger() )
        debugger->slotDataCmd();
}

    void
KDevKXSLDbgPlugin::sourceCmd_activated()
{
    if ( checkDebugger() )
        debugger->slotSourceCmd();
}

    void
KDevKXSLDbgPlugin::outputCmd_activated()
{
    XsldbgSettingData item;
    if ( ( inspector != 0L ) &&  checkDebugger() && ( configWidget != 0L ) && optionDataModel()
            && optionDataModel()->findSetting(OPTIONS_OUTPUT_FILE_NAME, item)){
        debugger->setOutputFileActive(true);
        lineNoChanged( item.m_value.toString(), 1, false );
        refreshCmd_activated();
    }
}

void KDevKXSLDbgPlugin::refreshCmd_activated()
{
    KDevelop::IDocument* document;
    MarkInterface *iface;

    if ( !currentFileName.isEmpty() ){
        foreach (document,
                KDevelop::ICore::self()->documentController()->openDocuments()) {
            iface = dynamic_cast<MarkInterface*>(document->textDocument());
            if (iface) {
                iface->clearMarks();
            }
        }
        if ( checkDebugger() ){
            debugger->fakeInput("showbreak", true);  // noTr
        }
    }
}

void KDevKXSLDbgPlugin::enableCmd_activated()
{
    if ( checkDebugger() )
        debugger->slotEnableCmd( currentFile().url(), currentLineNo());
}

void KDevKXSLDbgPlugin::deleteCmd_activated()
{
    if ( checkDebugger() ){
        debugger->slotDeleteCmd( currentFile().url(), currentLineNo());
    }
}

void KDevKXSLDbgPlugin::breakCmd_activated()
{
    if ( checkDebugger() ){
        debugger->slotBreakCmd( currentFile().url(), currentLineNo());
    }
}

void KDevKXSLDbgPlugin::evaluateCmd_activated()
{
    QString expression = KInputDialog::getText(i18n("Evaluate Expression"), i18n("XPath:"));
    if (checkDebugger()  && (expression.length() > 0)){
        debugger->slotCatCmd( expression);
    }
}

void KDevKXSLDbgPlugin::gotoXPathCmd_activated()
{
    QString xpath = KInputDialog::getText(i18n("Goto XPath"), i18n("XPath:"));
    if (checkDebugger() && xpath.length() > 0){
        debugger->slotCdCmd( xpath );
    }
}

    void
KDevKXSLDbgPlugin::lineNoChanged(QString fileName, int lineNumber, bool breakpoint)
{
    if ( fileName.isEmpty() ) {
        return;
    }

    KDevelop::IDocument* document;
    MarkInterface *iface;

    foreach (document,
            KDevelop::ICore::self()->documentController()->openDocuments()) {
        iface = dynamic_cast<MarkInterface*>(document->textDocument());
        if (iface){
            document->textDocument()->blockSignals(true);
            QHashIterator<int, KTextEditor::Mark*> it = iface->marks();
            while (it.hasNext()) {
                Mark* mark = it.next().value();
                iface->removeMark( mark->line, MarkInterface::Execution );
                iface->removeMark( mark->line, MarkInterface::BreakpointReached );

            }
            document->textDocument()->blockSignals(false);
        }
    }

    document = KDevelop::ICore::self()->documentController()->openDocument(KUrl(debugger->workDirName() +  fileName ),
            KTextEditor::Cursor(lineNumber - 1, 0));
    if (document){
        currentFileName = fileName;
        iface = dynamic_cast<MarkInterface*>(document->textDocument());
        if ( iface ){
            document->textDocument()->blockSignals(true);
            iface->addMark( lineNumber - 1, MarkInterface::Execution );

            /* Did we stop at a breakpoint set the break marker */
            if (breakpoint)
                iface->addMark( lineNumber - 1, MarkInterface::BreakpointReached );
            document->textDocument()->blockSignals(false);
        }
    }

}


void  KDevKXSLDbgPlugin::docChanged()
{
}

void  KDevKXSLDbgPlugin::debuggerStarted()
{
    if (configWidget != 0L){
        loadProfile();
        configWidget->refresh();
        configureCmd_activated();
    }
}

void  KDevKXSLDbgPlugin::addBreakPoint(int lineNumber)
{
    if ( checkDebugger() ){
        debugger->slotBreakCmd( currentFileName, lineNumber);
    }
}

void  KDevKXSLDbgPlugin::enableBreakPoint(int lineNumber)
{
    if ( checkDebugger() ){
        debugger->slotEnableCmd( currentFileName, lineNumber);
    }
}


void  KDevKXSLDbgPlugin::deleteBreakPoint(int lineNumber)
{
    if ( checkDebugger() ){
        debugger->slotDeleteCmd( currentFileName, lineNumber);
    }
}



void KDevKXSLDbgPlugin::slotSearch(const QString &text)
{
    if ( checkDebugger() ) {
        QString msg(QString("search \"%1\"").arg(text));  // noTr
        debugger->fakeInput(msg, false);
    }
}


void KDevKXSLDbgPlugin::slotEvaluate(const QString &text)
{
    if ( checkDebugger() ){
        debugger->slotCatCmd( text );
    }
}

void KDevKXSLDbgPlugin::slotGotoXPath( const QString &text)
{
    if ( checkDebugger() ){
        debugger->slotCdCmd( text );
    }
}



void KDevKXSLDbgPlugin::slotProcResolveItem(QString URI)
{
    if (!URI.isEmpty()){
        QMessageBox::information(0, i18n("SystemID or PublicID Resolution Result"),
                i18n("SystemID or PublicID has been resolved to\n.%1", URI),
                QMessageBox::Ok);
    }
}

void  KDevKXSLDbgPlugin::breakpointItem(QString fileName, int lineNumber ,
        QString /*templateName*/, QString /* modeName */,
        bool  enabled , int /* id */)
{

    KDevelop::IDocument* document;
    MarkInterface *iface;

    if (fileName == 0L){
        foreach (document,
                KDevelop::ICore::self()->documentController()->openDocuments()) {
            iface = dynamic_cast<MarkInterface*>(document->textDocument());
            if (iface) {
                document->textDocument()->blockSignals(true);
                iface->clearMarks();
                document->textDocument()->blockSignals(false);
            }
        }
    }else{
        KUrl url(debugger->workDirName() + fileName);
        fetchURL(url);
        document = KDevelop::ICore::self()->documentController()->documentForUrl(url);
        if (document){
            iface = dynamic_cast<MarkInterface*>(document->textDocument());
            if ( iface ){
                document->textDocument()->blockSignals(true);
                if (enabled)
                    iface->addMark( lineNumber - 1, MarkInterface::BreakpointActive );
                else
                    iface->addMark( lineNumber - 1, MarkInterface::BreakpointDisabled );
                document->textDocument()->blockSignals(false);
            }
        }
    }
}


void KDevKXSLDbgPlugin::walkCmd_activated()
{
    if (checkDebugger()){
        debugger->slotWalkCmd();
    }
}

void KDevKXSLDbgPlugin::walkStopCmd_activated()
{
    if (checkDebugger()){
        debugger->slotWalkStopCmd();
    }
}

void KDevKXSLDbgPlugin::traceCmd_activated()
{
    if (checkDebugger()){
        debugger->slotTraceCmd();
    }
}

void KDevKXSLDbgPlugin::traceStopCmd_activated()
{
    walkStopCmd_activated();
}


void KDevKXSLDbgPlugin::projectOpened(KDevelop::IProject* project)
{
    lastProject = project;

    //TODO support more than one open project
    m_projectKXSLDbgActionMenu->setVisible(true);

    if (!checkDebugger(false))
        return;

    KConfigGroup group = project->projectConfiguration()->group("KXSLDbg");
    if (debugger && !optionDataModel()->loadSettings(group))
        qWarning("Load of KXSLDbg settings failed");
}

void KDevKXSLDbgPlugin::projectClosed(KDevelop::IProject* project)
{
    //TODO support more than one open project
    m_projectKXSLDbgActionMenu->setVisible(false);
    qWarning(" %s %d", __PRETTY_FUNCTION__, __LINE__);

    if (!checkDebugger(false))
        return;
    qWarning(" %s %d", __PRETTY_FUNCTION__, __LINE__);

    KConfigGroup group = project->projectConfiguration()->group("KXSLDbg");
   if (debugger && optionDataModel()->saveSettings(group))
       group.sync();
   else
       qWarning("Save of KXSLDbg settings failed");
    lastProject = 0;
}


void  KDevKXSLDbgPlugin::loadProfile()
{
    if (!lastProject || !checkDebugger())
        return;

    projectOpened(lastProject);
}

void  KDevKXSLDbgPlugin::saveProfile()
{
    if (!lastProject || !checkDebugger())
        return;

   KConfigGroup group = lastProject->projectConfiguration()->group("KXSLDbg");
   if (debugger && optionDataModel()->saveSettings(group))
       group.sync();
   else
       qWarning("Save of KXSLDbg settings failed");
}

void  KDevKXSLDbgPlugin::unload()
{
    qWarning("%s", __PRETTY_FUNCTION__);
    saveProfile();
}

QStandardItemModel*  KDevKXSLDbgPlugin::outputModel()
{
    if (dev_outputModel)
       return dev_outputModel;

    KDevelop::IPlugin* plugin = core()->pluginController()->pluginForExtension( "org.kdevelop.IOutputView");
    Q_ASSERT( plugin );

    if ( plugin ){
        dev_outputView = plugin->extension<KDevelop::IOutputView>();
        if (dev_outputView){
            int tvid = dev_outputView->registerToolView(i18n("KXSLDbg Output"));
            toolID = dev_outputView->registerOutputInToolView(tvid, i18n("KXSLDbg Output"),
                    KDevelop::IOutputView::AllowUserClose | KDevelop::IOutputView::AutoScroll );
            dev_outputModel = new QStandardItemModel(this);
            if (dev_outputModel){
                connect(plugin, SIGNAL(viewRemoved(int)), this, SLOT(outputViewRemoved(int)));
                dev_outputView->setModel(toolID, dev_outputModel);
                dev_outputView->setDelegate(toolID, new QItemDelegate(dev_outputModel));
            }
        }
    }
    if (!dev_outputModel)
        qWarning("Creating output model failed");
    return dev_outputModel;
}

void KDevKXSLDbgPlugin::outputViewRemoved(int id)
{
    qWarning("%s", __PRETTY_FUNCTION__);
    if (id == toolID) {
        dev_outputView = 0;
        dev_outputModel = 0;
    }
}
#include "kdevkxsldbgplugin.moc"
