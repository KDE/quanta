/*
 * kxsldbg.cpp
 *
 * Copyright (C) 2001  <kurt@granroth.org>
 */
#include "kxsldbg.h"

#include <kkeydialog.h>
#include <kconfig.h>
#include <klocale.h>

#include <kedittoolbar.h>

#include <kaction.h>
#include <kstdaction.h>

#include <klibloader.h>
#include <kmessagebox.h>
#include <kstatusbar.h>

KXsldbg::KXsldbg()
    : KParts::MainWindow( 0L, "kxsldbg" ), DCOPObject("KXsldbg")
{
    // set the shell's ui resource file
    setXMLFile("kxsldbg_shell.rc");

    // then, setup our actions
    setupActions();

    // and a status bar
    statusBar()->show();
    statusBar()->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));

    // this routine will find and load our Part.  it finds the Part by
    // name which is a bad idea usually.. but it's alright in this
    // case since our Part is made for this Shell
    KLibFactory *factory = KLibLoader::self()->factory("libkxsldbgpart");
    if (factory)
    {
        // now that the Part is loaded, we cast it to a Part to get
        // our hands on it
        m_part = static_cast<KParts::ReadOnlyPart *>(factory->create(this,
                                "kxsldbg_part", "KParts::ReadOnlyPart" ));

        if (m_part)
        {
            // tell the KParts::MainWindow that this is indeed the main widget
            setCentralWidget(m_part->widget());

            // and integrate the part's GUI with the shell's
            createGUI(m_part);

	    // connect up signals
	    kapp->dcopClient()->attach();
	    connectDCOPSignal(0, 0, "debuggerPositionChanged(QString,int)", "newDebuggerPosition(QString,int)", false );
	    connectDCOPSignal(0, 0, "editorPositionChanged(QString,int,int)", "newCursorPosition(QString,int,int)", false );
        }

    }
    else
    {
        // if we couldn't find our Part, we exit since the Shell by
        // itself can't do anything useful
        KMessageBox::error(this, i18n("Could not find our part!"));
        kapp->quit();
    }
}

KXsldbg::~KXsldbg()
{
}

void KXsldbg::setupActions()
{
    KStdAction::quit(kapp, SLOT(quit()), actionCollection());

    m_toolbarAction = KStdAction::showToolbar(this, SLOT(optionsShowToolbar()), actionCollection());
    m_statusbarAction = KStdAction::showStatusbar(this, SLOT(optionsShowStatusbar()), actionCollection());

    KStdAction::keyBindings(this, SLOT(optionsConfigureKeys()), actionCollection());
    KStdAction::configureToolbars(this, SLOT(optionsConfigureToolbars()), actionCollection());
}

void KXsldbg::saveProperties(KConfig* /*config*/)
{
    // the 'config' object points to the session managed
    // config file.  anything you write here will be available
    // later when this app is restored
}

void KXsldbg::readProperties(KConfig* /*config*/)
{
    // the 'config' object points to the session managed
    // config file.  this function is automatically called whenever
    // the app is being restored.  read in here whatever you wrote
    // in 'saveProperties'
}


void KXsldbg::optionsShowToolbar()
{
    // this is all very cut and paste code for showing/hiding the
    // toolbar
    if (m_toolbarAction->isChecked())
        toolBar()->show();
    else
        toolBar()->hide();
}

void KXsldbg::optionsShowStatusbar()
{
    // this is all very cut and paste code for showing/hiding the
    // statusbar
    if (m_statusbarAction->isChecked())
        statusBar()->show();
    else
        statusBar()->hide();
}

void KXsldbg::optionsConfigureKeys()
{
    KKeyDialog::configure(actionCollection(), "kxsldbg_shell.rc");
}

void KXsldbg::optionsConfigureToolbars()
{
    saveMainWindowSettings(KGlobal::config(), "MainWindow");

    // use the standard toolbar editor
    KEditToolbar dlg(factory());
    connect(&dlg, SIGNAL(newToolbarConfig()),
            this, SLOT(applyNewToolbarConfig()));
    dlg.exec();
}

void KXsldbg::applyNewToolbarConfig()
{
    applyMainWindowSettings(KGlobal::config(), "MainWindow");
}


void KXsldbg::newCursorPosition(QString file, int lineNumber, int columnNumber)
{
    statusBar()->clear();
    statusBar()->message( i18n("File: %1  Line: %2  Col: %3").arg(file).arg(lineNumber).arg(columnNumber));
}

void KXsldbg::newDebuggerPosition(QString file, int lineNumber)
{
    // maybe do something extra here later
    newCursorPosition(file, lineNumber);
}

#include "kxsldbg.moc"
