#include "kxsldbg.h"
#include <kapplication.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <klocale.h>
#include <kdebug.h>

static const char *description =
    I18N_NOOP("A KDE KPart Application for xsldbg, a XSLT debugger");

static const char *version = VERSION;

int main(int argc, char **argv)
{
    KAboutData about("kxsldbg", I18N_NOOP("KXsldbg"), version, description, KAboutData::License_GPL, "(C) 2003 Keith Isdale", 0, 0, "k_isdale@tpg.com.au");
    about.addAuthor( "Keith Isdale", 0, "k_isdale@tpg.com.au" );
    KCmdLineArgs::init(argc, argv, &about);
    KApplication app;

    // see if we are starting with session management
    if (app.isRestored())
        RESTORE(KXsldbg)
    else
    {
        KXsldbg *widget = new KXsldbg;
        widget->show();
    }

    return app.exec();
}
