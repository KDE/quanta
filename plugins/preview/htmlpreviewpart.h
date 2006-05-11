/***************************************************************************
 *   Copyright (C) 2005 by Jens Herden   *
 *   jens@kdewebdev.org   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef KDEVHTMLPREVIEW_H
#define KDEVHTMLPREVIEW_H

#include "htmlpart.h"

#include <QPointer>
#include <QTimer>

//kdevelop includes
#include <interfaces/kdevplugin.h>

class QMenu;
class KAction;
class KDialogBase;
class Context;
class ConfigWidgetProxy;

namespace KTextEditor {
  class Document;
}
namespace KParts {
  class PartManager;
}


/**
Please read the README.dox file for more info about this part
*/
class HTMLPreviewPart : public KDevPlugin
{
  Q_OBJECT
public:
  HTMLPreviewPart(QObject *parent, const QStringList &args);
  ~HTMLPreviewPart();

private slots:
  void init();

  void insertConfigWidget(const KDialogBase *dlg, QWidget *page, unsigned int pageNo);
  void contextMenu(QMenu *popup, const Context *context);
  void projectOpened();
  void projectClosed();

  void slotPreview();
  void slotActivePartChanged(KParts::Part *part);
  void slotTextChanged(KTextEditor::Document *document);
  void slotIdleTimerExpired();

private:
  void setupActions();
  void loadContent(KTextEditor::Document * editor);

  KAction *action;

  QPointer<HTMLPart> m_browserPart;
  ConfigWidgetProxy * m_configProxy;
  KTextEditor::Document * m_activeEditor;
  
  /** the timer used to keep track of idle periods
   *  every keystroke restarts this timer
   *  @code m_idleTimer->isActive() @endcode
   *  returns true if there is typing in progress
   */
  QTimer *m_idleTimer;
  KParts::PartManager * m_partmanager;
};


#endif

// kate: space-indent on; indent-width 2; mixedindent off; indent-mode cstyle;

