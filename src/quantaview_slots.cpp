/**************************************************************************
                          quantaview_slots.cpp  -  description
                             -------------------
    begin                : Thu Mar 9 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavsky <pdima@mail.univ.kiev.ua>
                           (C) 2002, 2003 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

// standard header
#include <time.h>

// include files for QT
#include <qdir.h>
#include <qregexp.h>
#include <qcstring.h>
#include <qlineedit.h>
#include <qclipboard.h>
#include <qcheckbox.h>
#include <qtabwidget.h>
#include <qtextedit.h>
#include <qtextcodec.h>
#include <qspinbox.h>
#include <qradiobutton.h>

// include files for KDE
#include <kaction.h>
#include <kconfig.h>
#include <kdebug.h>
#include <klocale.h>
#include <kprocess.h>
#include <kmessagebox.h>
#include <kdeversion.h>
#include <kstatusbar.h>

#include <ktexteditor/configinterface.h>
#include <ktexteditor/clipboardinterface.h>
#include <ktexteditor/selectioninterface.h>
#include <ktexteditor/markinterface.h>
#include <ktexteditor/dynwordwrapinterface.h>

// application specific includes
#include "quantacommon.h"
#include "document.h"
#include "quanta.h"
#include "quantadoc.h"
#include "quantaview.h"
#include "resource.h"
#include "qextfileinfo.h"

#include "tag.h"
#include "project.h"
#include "messageoutput.h"

// dialogs headers
#include "tagdialog.h"

#include "tagmaildlg.h"


#include "qdom.h"


