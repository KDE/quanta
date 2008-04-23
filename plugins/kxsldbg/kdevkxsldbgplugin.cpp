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

#include <icore.h>
#include <idocumentcontroller.h>
#include <iuicontroller.h>
#include "kdevkxsldbgplugin.h"
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

#include <libxsldbg/files.h>
#include <libxsldbg/xsldbg.h>

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
// kate specific classes
#include <ktexteditor/document.h>
#include <ktexteditor/markinterface.h>

#include <ioutputview.h>
#include <iplugincontroller.h>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QItemDelegate>

K_PLUGIN_FACTORY(KDevKXSLDbgFactory, registerPlugin<KDevKXSLDbgPlugin>(); )
K_EXPORT_PLUGIN(KDevKXSLDbgFactory("kdevkxsldbg"))

class KDevKXSLDbgPlugin;

using namespace KTextEditor;

class KDevKXSLDbgViewFactory : public KDevelop::IToolViewFactory
{
    public:
        KDevKXSLDbgViewFactory(KDevKXSLDbgPlugin *thePart)
            : KDevelop::IToolViewFactory(), myplugin(thePart)
        {

        }
        /**
          Return the main widget for our tool
         */
        QWidget *create(QWidget * parent = 0 );

        /**
          Return a hint as to where to put our tool
         */
        Qt::DockWidgetArea defaultPosition(const QString &areaName);

        KDevKXSLDbgPlugin * myplugin;
};


QWidget* KDevKXSLDbgViewFactory::create(QWidget * parent)
{
    QWidget *w = 0;

    if (myplugin){
        w = myplugin->topWidget();
        if (w)
            w->setParent(parent);
    }
    return w;
}

Qt::DockWidgetArea KDevKXSLDbgViewFactory::defaultPosition(const QString &areaName)
{
    Q_UNUSED(areaName);
    return Qt::TopDockWidgetArea;
}


    KDevKXSLDbgPlugin::KDevKXSLDbgPlugin(QObject *parent, const QVariantList &args)
: KDevelop::IPlugin(KDevKXSLDbgFactory::componentData(),parent)
{
    Q_UNUSED(args)
    setXMLFile( "kdevkxsldbg.rc");
    m_widgetFactory = new KDevKXSLDbgViewFactory(this);
    core()->uiController()->addToolView("KXSLDbg", m_widgetFactory );
    currentColumnNo = 0;
    inspector = 0L;
    debugger = 0L;
    configWidget = 0L;
    frame = new KVBox(0);
    KHBox *h = new KHBox(frame);
    newXPath = new QLineEdit(h);
    xPathBtn = new QPushButton(i18n("Goto XPath"), h);
    h = new KHBox(frame);
    newEvaluate = new QLineEdit(h);
    evaluateBtn = new QPushButton(i18n("Evaluate"), h);

    setupActions();

}

KDevKXSLDbgPlugin::~KDevKXSLDbgPlugin()
{
}

QWidget *KDevKXSLDbgPlugin::topWidget()
{
    return frame;
}

void KDevKXSLDbgPlugin::setupActions()
{
    KActionCollection* ac = actionCollection();
    m_projectKXSLDbgActionMenu = new KActionMenu(i18n("&Run Project via KXSLDbg"), this);
    m_projectKXSLDbgActionMenu->setIcon(KIcon("xsldbg-source"));
    m_projectKXSLDbgActionMenu->setToolTip(i18n("Run project"));
    ac->addAction("project_kxsldbg_run", m_projectKXSLDbgActionMenu);
    connect(m_projectKXSLDbgActionMenu, SIGNAL(triggered(bool)), SLOT(showKXSLDbg()));
    // create our actions
    KStandardAction::open(this, SLOT(fileOpen()), actionCollection());

    configureEditorCmd = ac->addAction( "configureEditorCmd" );
    configureEditorCmd->setText( i18n("Configure Editor...") );
    configureEditorCmd->setIcon( KIcon("configure") );
    connect(configureEditorCmd, SIGNAL(triggered(bool)), SLOT(configureEditorCmd_activated()));
    configureEditorCmd->setVisible(false);
    configureCmd = ac->addAction( "configureCmd" );
    configureCmd->setText( i18n("Configure...") );
    configureCmd->setIcon( KIcon("configure") );
    connect(configureCmd, SIGNAL(triggered(bool)), SLOT(configureCmd_activated()));
    configureCmd->setShortcut(QKeySequence(Qt::Key_C));
    configureEditorCmd->setVisible(false);

    inspectCmd = ac->addAction( "inspectCmd" );
    inspectCmd->setText( i18n("Inspect...") );
    inspectCmd->setIcon( KIcon("edit-find") );
    connect(inspectCmd, SIGNAL(triggered(bool)), SLOT(inspectorCmd_activated()));
    inspectCmd->setShortcut(QKeySequence(Qt::Key_I));
    inspectCmd->setVisible(false);


    // Motions commands
    runCmd = ac->addAction( "runCmd" );
    runCmd->setText( i18n("Run") );
    runCmd->setIcon( KIcon("system-run") );
    connect(runCmd, SIGNAL(triggered(bool)), SLOT(runCmd_activated()));
    runCmd->setShortcut(QKeySequence(Qt::Key_F5));
    runCmd->setVisible(false);

    continueCmd = ac->addAction( "continueCmd" );
    continueCmd->setText( i18n("Continue") );
    continueCmd->setIcon( KIcon("media-playback-start") );
    connect(continueCmd, SIGNAL(triggered(bool)), SLOT(continueCmd_activated()));
    continueCmd->setShortcut(QKeySequence(Qt::Key_F4));
    continueCmd->setVisible(false);

    stepCmd = ac->addAction( "stepCmd" );
    stepCmd->setText( i18n("Step") );
    stepCmd->setIcon( KIcon("step") );
    connect(stepCmd, SIGNAL(triggered(bool)), SLOT(stepCmd_activated()));
    stepCmd->setShortcut(QKeySequence(Qt::Key_F8));
    stepCmd->setVisible(false);

    nextCmd  = new KAction(KIcon("go-down-search"), i18n("Next"), this);
    ac->addAction("nextCmd", nextCmd );
    connect(nextCmd, SIGNAL(triggered(bool)), SLOT(nextCmd_activated()));
    nextCmd->setShortcut(QKeySequence(Qt::Key_F10));
    nextCmd->setVisible(false);

    stepupCmd  = new KAction(KIcon("xsldbg_stepup"), i18n("Step Up"), this);
    ac->addAction("stepupCmd", stepupCmd );
    connect(stepupCmd, SIGNAL(triggered(bool)), SLOT(stepupCmd_activated()));
    stepupCmd->setShortcut(QKeySequence(Qt::Key_F6));
    stepupCmd->setVisible(false);

    stepdownCmd  = new KAction(KIcon("xsldbg_stepdown"), i18n("Step Down"), this);
    ac->addAction("stepdownCmd", stepdownCmd );
    connect(stepdownCmd, SIGNAL(triggered(bool)), SLOT(stepdownCmd_activated()));
    stepdownCmd->setShortcut(QKeySequence(Qt::Key_F7));
    stepdownCmd->setVisible(false);

    // Breakpoint commands
    breakCmd  = new KAction(KIcon("xsldbg_break"), i18n("Break"), this);
    ac->addAction("breakCmd", breakCmd );
    connect(breakCmd, SIGNAL(triggered(bool)), SLOT(breakCmd_activated()));
    breakCmd->setShortcut(QKeySequence(Qt::Key_F2));
    breakCmd->setVisible(false);

    enableCmd  = new KAction(KIcon("xsldbg_enable"), i18n("Enable/Disable"), this);
    ac->addAction("enableCmd", enableCmd );
    connect(enableCmd, SIGNAL(triggered(bool)), SLOT(enableCmd_activated()));
    enableCmd->setShortcut(QKeySequence(Qt::Key_F3));
    enableCmd->setVisible(false);

    deleteCmd  = new KAction(KIcon("xsldbg_delete"), i18n("Delete"), this);
    ac->addAction("deleteCmd", deleteCmd );
    connect(deleteCmd, SIGNAL(triggered(bool)), SLOT(deleteCmd_activated()));
    deleteCmd->setShortcut(QKeySequence(Qt::Key_Delete));
    deleteCmd->setVisible(false);

    sourceCmd  = new KAction(KIcon("xsldbg_source"), i18n("&Source"), this);
    ac->addAction("sourceCmd", sourceCmd );
    connect(sourceCmd, SIGNAL(triggered(bool)), SLOT(sourceCmd_activated()));
    sourceCmd->setShortcut(QKeySequence(Qt::Key_S));
    sourceCmd->setVisible(false);

    dataCmd  = new KAction(KIcon("xsldbg_data"), i18n("&Data"), this);
    ac->addAction("dataCmd", dataCmd );
    connect(dataCmd, SIGNAL(triggered(bool)), SLOT(dataCmd_activated()));
    dataCmd->setShortcut(QKeySequence(Qt::Key_D));
    dataCmd->setVisible(false);

    outputCmd  = new KAction(KIcon("xsldbg_output"), i18n("&Output"), this);
    ac->addAction("outputCmd", outputCmd );
    connect(outputCmd, SIGNAL(triggered(bool)), SLOT(outputCmd_activated()));
    outputCmd->setShortcut(QKeySequence(Qt::Key_O));
    outputCmd->setVisible(false);

    refreshCmd  = new KAction(KIcon("xsldbg_refresh"), i18n("Reload"), this);
    ac->addAction("refreshCmd", refreshCmd );
    connect(refreshCmd, SIGNAL(triggered(bool)), SLOT(refreshCmd_activated()));
    refreshCmd->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F5));
    refreshCmd->setVisible(false);

    /* tracing and walking */
    walkCmd  = new KAction(i18n("Walk Through Stylesheet..."), this);
    ac->addAction("walkCmd", walkCmd );
    connect(walkCmd, SIGNAL(triggered(bool)), SLOT(walkCmd_activated()));
    walkCmd->setShortcut(QKeySequence(Qt::Key_W));
    walkCmd->setVisible(false);

    walkStopCmd  = new KAction(i18n("Stop Wal&king Through Stylesheet"), this);
    ac->addAction("walkStopCmd", walkStopCmd );
    connect(walkStopCmd, SIGNAL(triggered(bool)), SLOT(walkStopCmd_activated()));
    walkStopCmd->setShortcut(QKeySequence(Qt::Key_K));
    walkStopCmd->setVisible(false);

    traceCmd  = new KAction(i18n("Tr&ace Execution of Stylesheet"), this);
    ac->addAction("traceCmd", traceCmd );
    connect(traceCmd, SIGNAL(triggered(bool)), SLOT(traceCmd_activated()));
    traceCmd->setShortcut(QKeySequence(Qt::Key_A));
    traceCmd->setVisible(false);

    traceStopCmd  = new KAction(i18n("Stop Tracing of Stylesheet"), this);
    ac->addAction("traceStopCmd", traceStopCmd );
    connect(traceStopCmd, SIGNAL(triggered(bool)), SLOT(traceStopCmd_activated()));
    traceStopCmd->setShortcut(QKeySequence(Qt::Key_K));
    traceStopCmd->setVisible(false);

    evaluateCmd  = new KAction(i18n("&Evaluate Expression..."), this);
    ac->addAction("evaluateCmd", evaluateCmd );
    connect(evaluateCmd, SIGNAL(triggered(bool)), SLOT(evaluateCmd_activated()));
    evaluateCmd->setShortcut(QKeySequence(Qt::Key_E));
    evaluateCmd->setVisible(false);

    gotoXPathCmd  = new KAction(i18n("Goto &XPath..."), this);
    ac->addAction("gotoXPathCmd", gotoXPathCmd );
    connect(gotoXPathCmd, SIGNAL(triggered(bool)), SLOT(gotoXPathCmd_activated()));
    gotoXPathCmd->setShortcut(QKeySequence(Qt::Key_X));
    gotoXPathCmd->setVisible(false);

    lookupSystemCmd  = new KAction(i18n("Lookup SystemID..."), this);
    ac->addAction("lookupSystemID", lookupSystemCmd );
    connect(lookupSystemCmd, SIGNAL(triggered(bool)), SLOT(slotLookupSystemID()));
    lookupSystemCmd->setVisible(false);

    lookupPublicIDCmd  = new KAction(i18n("Lookup PublicID..."), this);
    ac->addAction("lookupPublicID", lookupPublicIDCmd );
    connect(lookupPublicIDCmd, SIGNAL(triggered(bool)), SLOT(slotLookupPublicID()));
    lookupPublicIDCmd->setVisible(false);

    connect( xPathBtn, SIGNAL( clicked() ),
            this, SLOT( slotGotoXPath() ) );
    connect( evaluateBtn, SIGNAL( clicked() ),
            this, SLOT( slotEvaluate() ) );
}

void  KDevKXSLDbgPlugin::showKXSLDbg()
{
    QWidget *w = topWidget();
    static bool setupWidgets=false;

    if (setupWidgets){
        if (w)
            w->show();
    }else{
        configureEditorCmd->setVisible(true);
        configureCmd->setVisible(true);
        inspectCmd->setVisible(true);
        runCmd->setVisible(true);
        continueCmd->setVisible(true);
        stepCmd->setVisible(true);
        nextCmd->setVisible(true);
        stepupCmd->setVisible(true);
        stepdownCmd->setVisible(true);
        breakCmd->setVisible(true);
        enableCmd->setVisible(true);
        deleteCmd->setVisible(true);
        sourceCmd->setVisible(true);
        dataCmd->setVisible(true);
        outputCmd->setVisible(true);
        refreshCmd->setVisible(true);
        walkCmd->setVisible(true);
        walkStopCmd->setVisible(true);
        traceCmd->setVisible(true);
        traceStopCmd->setVisible(true);
        evaluateCmd->setVisible(true);
        gotoXPathCmd->setVisible(true);
        lookupSystemCmd->setVisible(true);
        lookupPublicIDCmd->setVisible(true);
        
        KDevelop::IPlugin* plugin = core()->pluginController()->pluginForExtension( "org.kdevelop.IOutputView");
        Q_ASSERT( plugin );

        dev_outputview = plugin->extension<KDevelop::IOutputView>();
        Q_ASSERT( dev_outputview );
        int tvid = dev_outputview->registerToolView(i18n("KXSLDbg"));
        int id = dev_outputview->registerOutputInToolView( tvid, i18n("KXSLDbg Output"),
                KDevelop::IOutputView::AllowUserClose | KDevelop::IOutputView::AutoScroll);
        dev_outputModel = new QStandardItemModel(this);
        Q_ASSERT(dev_outputModel);
        QItemDelegate *itemDelegate = new QItemDelegate();
        Q_ASSERT(itemDelegate);

        dev_outputview->setModel(id, dev_outputModel);
        dev_outputview->setDelegate(id, itemDelegate);

        /* We must have a valid debugger and inspector */
        createInspector();
        if (checkDebugger()){
            configWidget = new XsldbgConfigImpl( debugger, 0L );
            Q_CHECK_PTR( configWidget );
            debugger->start();
        }else{
            openUrl(KUrl());
        }
        setupWidgets=true;
        frame->show();
        // cause the configuration dialog to show upon starting
        configureCmd_activated();
    }
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
    KDevelop::IDocument* document = KDevelop::ICore::self()->documentController()->openDocument(url);
    if (!document)
        qWarning("Unable to fetch URL %s", url.url().toUtf8().constData());

    return document != 0;
}

bool KDevKXSLDbgPlugin::openFile()
{
    qWarning("bool KDevKXSLDbgPlugin::openFile() called");
    return false;
}

bool KDevKXSLDbgPlugin::closeUrl()
{
    // TODO
    //docDictionary.clear();
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

bool KDevKXSLDbgPlugin::checkDebugger()
{
    bool result = debugger != 0L;
    if (!result){
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
                frame);
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
                QString(), &ok, frame );
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
    if (!checkDebugger())
        return;

    if (configWidget != 0L){
        configWidget->refresh();
        configWidget->show();
    }
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
            QMessageBox::information(frame, i18n("Result of evaluation"),
                    message.mid(endPosition + 1));
        }
    }else  /* Is there some sort of error message in message */
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
                QMessageBox::warning(frame, i18n("Request Failed "), message);
            processed = true;
        }
    if (dev_outputModel && !processed)
        dev_outputModel->appendRow(new QStandardItem(message));

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
    Q_UNUSED(file)
        Q_UNUSED(line)
        Q_UNUSED(row)
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
    if ( ( inspector != 0L ) &&  checkDebugger() && ( configWidget != 0L ) ){
        debugger->setOutputFileActive(true);
        lineNoChanged( configWidget->getOutputFile(), 1, false );
        refreshCmd_activated();
    }
}

void KDevKXSLDbgPlugin::refreshCmd_activated()
{
    qWarning("%s", __PRETTY_FUNCTION__);
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
        debugger->slotEnableCmd( currentFileName, currentLineNo());
}

void KDevKXSLDbgPlugin::deleteCmd_activated()
{
    if ( checkDebugger() ){
        debugger->slotDeleteCmd( currentFileName, currentLineNo());
    }
}

void KDevKXSLDbgPlugin::breakCmd_activated()
{
    if ( checkDebugger() ){
        debugger->slotBreakCmd( currentFileName, currentLineNo());
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

    document = KDevelop::ICore::self()->documentController()->openDocument(KUrl( fileName ),
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
        KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
        if (args){
            int i=0, result=1, noFilesFound = 0;
            QString expandedName;      /* contains file name with path expansion if any */

            for (i = 0; i < args->count(); i++) {
                if (!result)
                    break;

                if (args->arg(i)[0] != '-') {
                    expandedName = QString::fromUtf8((const char*)filesExpandName((const xmlChar*)args->arg(i).toUtf8().data()));
                    if (expandedName.isEmpty()) {
                        result = 0;
                        break;
                    }
                    switch (noFilesFound) {
                        case 0:
                            configWidget->slotSourceFile(expandedName);
                            noFilesFound++;
                            break;
                        case 1:
                            configWidget->slotDataFile(expandedName);
                            noFilesFound++;
                            break;
                        case 2:
                            configWidget->slotOutputFile(expandedName);
                            noFilesFound++;
                            break;

                        default:
                            xsldbgGenericErrorFunc(i18n("Error: Too many file names supplied via command line.\n"));
                            result = 0;
                    }
                    continue;
                }
            }
            configWidget->refresh();
        }
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



void KDevKXSLDbgPlugin::slotSearch()
{
    if ((newSearch != 0L)  && checkDebugger() ) {
        QString msg(QString("search \"%1\"").arg(newSearch->text()));  // noTr
        debugger->fakeInput(msg, false);
    }
}


void KDevKXSLDbgPlugin::slotEvaluate()
{
    if ((newEvaluate != 0L) && checkDebugger() ){
        debugger->slotCatCmd( newEvaluate->text() );
    }
}

void KDevKXSLDbgPlugin::slotGotoXPath()
{
    if ((newXPath != 0L) && checkDebugger() ){
        debugger->slotCdCmd( newXPath->text() );
    }
}



void KDevKXSLDbgPlugin::slotProcResolveItem(QString URI)
{
    if (!URI.isEmpty()){
        QMessageBox::information(frame, i18n("SystemID or PublicID Resolution Result"),
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
        KUrl url(fileName);
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

