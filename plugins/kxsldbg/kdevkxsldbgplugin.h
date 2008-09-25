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

#ifndef KDEVKXSLDBGPLUGIN_H
#define KDEVKXSLDBGPLUGIN_H
#include <iplugin.h>
#include <QVariant>
#include <QMainWindow>
#include <QActionGroup>
#include <QHash>
#include <QMenu>
#include <xsldbgdebugger.h>
#include <xsldbginspector.h>
#include <ktexteditor/document.h>
#include <ktexteditor/view.h>
#include <QDBusConnection>
class KxsldbgAdaptor;

class QGridLayout;
class QAction;
class QLineEdit;
class QDockWindow;
class QStackedWidget;

class XsldbgBrkStatusView;
class QXsldbgDoc;
class XsldbgOutputView;
class XsldbgConfigImpl;

class QWidget;
class QStandardItemModel;
class QPainter;
class KUrl;
class KVBox;
class KFrame;
class KAboutData;
class ConfigDialogPrivate;

namespace KDevelop
{
    class IOutputView;
    class IDocument;
    class IProject;
}

typedef QHash<QString, QXsldbgDoc*> XsldbgDocDict;
class KActionMenu;

class KDevKXSLDbgViewFactory;

class KDevKXSLDbgPlugin: public KDevelop::IPlugin
{
    Q_OBJECT
    public:
        KDevKXSLDbgPlugin(QObject *parent, const QVariantList & = QVariantList());
        virtual ~KDevKXSLDbgPlugin();
        virtual void unload();
        QStandardItemModel* outputModel();


        void setupActions();


        /** Checks the debugger, if it is not ready then
         *      display a error in a message box
         *
         * @returns TRUE if debugger is ready, otherwise FALSE
         */
        bool checkDebugger(bool showWarningDialog=true);
        void lookupSystemID(QString systemID);
        void lookupPublicID(QString publicID);
        void createInspector();
        int currentLineNo();
        const KUrl currentFile();

Q_SIGNALS:
        Q_SCRIPTABLE void newCursorPosition(const QString &file, int lineNumber, int columnNumber);
        Q_SCRIPTABLE void newDebuggerPosition(const QString &file, int lineNumber);

        public slots:
        void showKXSLDbg();
        virtual bool openUrl(const KUrl &url);
        virtual bool closeUrl();
        void quit();
        void emitOpenFile(QString file, int line, int row);
        void showMessage(QString msg);
        void slotLookupSystemID();
        void slotLookupPublicID();
        void walkCmd_activated();
        void walkStopCmd_activated();
        void traceCmd_activated();
        void traceStopCmd_activated();

        //Tool bar commands
        void configureCmd_activated();
        void inspectorCmd_activated();
        void runCmd_activated();
        void stepCmd_activated();
        void nextCmd_activated();
        void continueCmd_activated();
        void stepupCmd_activated();
        void stepdownCmd_activated();
        void sourceCmd_activated();
        void dataCmd_activated();
        void outputCmd_activated();
        void refreshCmd_activated();
        void enableCmd_activated();
        void breakCmd_activated();
        void deleteCmd_activated();

        /** Evaluate expression entered */	
        void evaluateCmd_activated();

        /** Goto/display file XPath entered */
        void gotoXPathCmd_activated();

        virtual void lineNoChanged( QString fileName, int lineNumber,
                bool breakpoint );

        void addBreakPoint(int lineNumber);
        void enableBreakPoint(int lineNumber);
        void deleteBreakPoint(int lineNumber);

        /** Evaluate expression entered in expressionEdit */
        void slotEvaluate(const QString &text);

        /** Goto/display file that matches XPath specified in xPathEdit */
        void slotGotoXPath(const QString &text);

        /** Run search on data base , see search.dtd for DTD of search database */
        void slotSearch(const QString &text);

        /* Process the URI for SystemID or PublicID requested */
        void slotProcResolveItem(QString URI);

        /* used to get breakpoint list notfication */
        void breakpointItem(QString fileName, int  lineNumber ,
                QString /*templateName*/, QString /* modeName */,
                bool  enabled , int /* id */);

        void docChanged();

        void outputViewRemoved(int);


        /**
         * Called when project was opened, adds a upload-action to the project-menu.
         */
        void projectOpened(KDevelop::IProject* project);

        /**
         * Called when project was closed, removes the upload-action from the project-menu.
         */
        void projectClosed(KDevelop::IProject* project);
        void documentClosed(KDevelop::IDocument* document);

        void debuggerStarted();

        void loadProfile();
        void saveProfile();

    protected:
        /**
         * This must be implemented by each part
         */
        virtual bool openFile();
        bool fetchURL(const KUrl &url);

    protected slots:
        void fileOpen();

    private:
        KDevKXSLDbgViewFactory *m_widgetFactory;
        KActionMenu* m_projectKXSLDbgActionMenu; ///< KXSLDbg ActionMenu, displayed in the Project-Menu

        uint currentColumnNo;
        XsldbgInspector *inspector;
        XsldbgDebugger *debugger;
        XsldbgConfigImpl *configWidget;
        QString currentFileName;
        ConfigDialogPrivate *dlg;
        QDBusInterface *dbusIface;
        KDevelop::IProject *lastProject;

        KDevelop::IOutputView *dev_outputView;
        QStandardItemModel *dev_outputModel;
        int toolID;
        QAction *configureCmd;
        QAction *inspectCmd;
        QAction *runCmd;
        QAction *continueCmd;
        QAction *stepCmd;
        QAction *nextCmd;
        QAction *stepupCmd;
        QAction *stepdownCmd;
        QAction *breakCmd;
        QAction *enableCmd;
        QAction *deleteCmd;
        QAction *sourceCmd;
        QAction *dataCmd;
        QAction *outputCmd;
        QAction *refreshCmd;
        QAction *walkCmd;
        QAction *walkStopCmd;
        QAction *traceCmd;
        QAction *traceStopCmd;
        QAction *evaluateCmd;
        QAction *gotoXPathCmd;
        QAction *lookupSystemCmd;
        QAction *lookupPublicIDCmd;
        QList<QAction *> xsldbgActions;
};

#endif
