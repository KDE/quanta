#ifndef KXSLDBG_H
#define KXSLDBG_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <kapplication.h>
#include <kparts/mainwindow.h>
#include <dcopobject.h> 
#include <dcopclient.h> 
#include "kxsldbgif.h"

class KToggleAction;

/**
 * This is the application "Shell".  It has a menubar, toolbar, and
 * statusbar but relies on the "Part" to do all the real work.
 *
 * @short Application Shell
 * @author Keith Isdale <k_isdale@tpg.com.au>
 * @version 0.4
 */
class KXsldbg : public KParts::MainWindow, public KXsldbgIf
{
    Q_OBJECT

public:
    /**
     * Default Constructor
     */
    KXsldbg();

    /**
     * Default Destructor
     */
    virtual ~KXsldbg();

    /**
	KxsldbgIf implementation 
    */ 
    void newCursorPosition(QString file, int lineNumber, int columnNumber=0);
    void newDebuggerPosition(QString file, int lineNumber);

protected:
    /**
     * This method is called when it is time for the app to save its
     * properties for session management purposes.
     */
    void saveProperties(KConfig *);

    /**
     * This method is called when this app is restored.  The KConfig
     * object points to the session management config file that was saved
     * with @ref saveProperties
     */
    void readProperties(KConfig *);

private slots:
    void optionsShowToolbar();
    void optionsShowStatusbar();
    void optionsConfigureKeys();
    void optionsConfigureToolbars();

    void applyNewToolbarConfig();


private:
    void setupAccel();
    void setupActions();

private:
    KParts::ReadOnlyPart *m_part;

    KToggleAction *m_toolbarAction;
    KToggleAction *m_statusbarAction;
};

#endif // KXSLDBG_H
