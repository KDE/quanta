/***************************************************************************
 *   Copyright (C) 2005 by Jens Herden   *
 *   jens@kdewebdev.org   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef HTMLPREVIEWPART_H
#define HTMLPREVIEWPART_H

#include "htmlpart.h"

#include <QPointer>
#include <QTimer>

//kdevelop includes
#include <interfaces/iplugin.h>

class QMenu;
class KDialog;
namespace KDevelop 
{ 
  class Context; 
  class IProject;
}
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
class HTMLPreviewPart : public KDevelop::IPlugin
{
  Q_OBJECT
public:
  HTMLPreviewPart(QObject *parent, const QVariantList &args);
  virtual ~HTMLPreviewPart();
  // the methods to embed the view 
  virtual QWidget *pluginView() const;
    
  // FIXME 
  virtual Qt::DockWidgetArea dockWidgetAreaHint() const 
    {return Qt::BottomDockWidgetArea;}
  
private slots:
  void init();

  void insertConfigWidget(const KDialog *dlg, QWidget *page, unsigned int pageNo);
  void contextMenu(QMenu *popup, const KDevelop::Context *context);
  void projectOpened(KDevelop::IProject *project);
  void projectClosed(KDevelop::IProject *project);

  void slotPreview();
  void slotActivePartChanged(KParts::Part *part);
  void slotTextChanged(KTextEditor::Document *document);
  void slotIdleTimerExpired();

private:
  void setupActions();
  void loadContent(KTextEditor::Document * editor);

  QAction *action;

  HTMLPart *m_browserPart;
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

