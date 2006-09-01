/***************************************************************************
                          quantaInit.h  -  description
                             -------------------
    begin                : ?? ???  9 13:29:57 EEST 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon <pdima@users.sourceforge.net,yshurik@linuxfan.com,sequitur@easystreet.com>
                           (C) 2001-2003 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QUANTAINIT_H
#define QUANTAINIT_H


// include files for Qt
#include <qobject.h>

// include files for KDE
#include <kparts/dockmainwindow.h>

//app includes

class KMdiToolViewAccessor;

class QuantaApp;
class ViewManager;
class ProjectTreeView;
class TemplatesTreeView;
class ScriptTreeView;
class KConfig;

/**
  * The base class for Quanta application windows.
  */
class QuantaInit : public QObject
{
  Q_OBJECT

public:
  QuantaInit(QuantaApp * quantaApp);
  ~QuantaInit();

  /** Loads the initial project */
  void loadInitialProject(const QString& url);

  /**Executes *nix ps command */
  void execCommandPS(const QString& cmd);
  /** if there are backup files, asks user whether wants to restore them or to mantain the originals instead*/
  void recoverCrashed(QStringList& recoveredFileNameList);
  /** Delayed initialization. */
  void initQuanta();
  void openLastFiles();
  void loadVPLConfig();

signals: // Signals
  /** signal used to hide the splash screen */
  void hideSplash();

private:
  QuantaApp * m_quanta;
  void initToolBars();
  void readOptions();

  void initActions();
  void initStatusBar();
  void initDocument();
  void initView();
  void initProject();
  void checkRuntimeDependencies();
  void readAbbreviations();

  KMdiToolViewAccessor* addToolTreeView(QWidget *widget, const QString &name, const QPixmap &icon, KDockWidget::DockPosition position);

  /** Initialize the plugin architecture. */
  void initPlugins();

  /** find where was url backed up in the list of autosaved urls*/
  QString searchPathListEntry(const QString& url, const QStringList& autosavedUrlsList, bool &notFound);
  /** Retrieves hashed path from the name of a backup file */
  QString retrieveHashedPath(const QString& filename);
  /** Obtains PID from file extension */
  QString retrievePID(const QString& filename);
  ProjectTreeView *pTab;
  TemplatesTreeView *tTab;
  // config
  KConfig *m_config;
  QStringList m_PIDlist;
  /**  @return number of Quanta instances registered by dcop */
  int runningQuantas();
};

#endif // QUANTA_H
