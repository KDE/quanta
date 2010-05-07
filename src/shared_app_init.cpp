/***************************************************************************
 *   Copyright (C) 2005 by Andras Mantia <amantia@kde.org>                 *
 *   Copyright (C) 2010 Milian Wolff <mail@milianw.de>                     *
 *   Copyright 2010 Niko Sams <niko.sams@gmail.com>                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/// THIS FILES GETS INCLUDED IN BOTH - main.cpp AND main_internal.cpp

static const char description[] =
    I18N_NOOP("Quanta Plus Web Development Environment");

static const char othertext[] =
    I18N_NOOP("Our goal is to be nothing less than the best possible tool for\n \
    working with tagging and scripting languages.\
    \n\nQuanta Plus is not in any way affiliated with any commercial\n \
    versions of Quanta. \
    \n\nWe hope you enjoy Quanta Plus.\n\n");

KAboutData aboutData("quanta", 0, ki18n("Quanta"),
                      QUANTA_VERSION, ki18n(description), KAboutData::License_GPL,
                      ki18n("(c) 1999-2006, The Quanta+ developers"), ki18n(othertext), "http://www.kdewebdev.org");

aboutData.addAuthor(ki18n("Eric Laffoon"),ki18n("Project Lead - public liaison"), "sequitur@kde.org");
aboutData.addAuthor(ki18n("Andras Mantia"),ki18n("Program Lead - bug squisher"), "amantia@kde.org");
aboutData.addCredit(ki18n("Jens Herden"),
                    ki18n("Invaluable member - Help with code cleanup, porting to KDevelop and KDE4, etc."),
                    "jens@kdewebdev.org");

//we can't use KCmdLineArgs as it doesn't allow arguments for the debugee
//so lookup the --debug switch and eat everything behind by decrementing argc
//debugArgs is filled with args after --debug <debuger>
QStringList debugArgs;
{
    bool debugFound = false;
    int c = argc;
    for (int i=0; i < c; ++i) {
        if (debugFound) {
            debugArgs << argv[i];
        } else if (QString(argv[i]) == "--debug") {
            if (argc <= i+1) {
                argc = i + 1;
            } else {
                i++;
                argc = i + 1;
            }
            debugFound = true;
        } else if (QString(argv[i]).startsWith("--debug=")) {
            argc = i + 1;
            debugFound = true;
        }
    }
}

KCmdLineArgs::init( argc, argv, &aboutData );
KCmdLineOptions options;
options.add("project <project>", ki18n( "Url to project to load" ));
options.add("+files", ki18n( "Files to load" ));
options.add("debug <debugger>", ki18n( "Start debugger, for example gdb. The binary that should be debugged must follow - including arguments." ));