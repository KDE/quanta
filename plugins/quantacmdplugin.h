/***************************************************************************
                          quantacmdplugin.h  -  description
                             -------------------
    begin                : Wed Sep 18 2002
    copyright            : (C) 2002 by Marc Britton
    email                : consume@optushome.com.au
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QUANTACMDPLUGIN_H
#define QUANTACMDPLUGIN_H

/* QT INCLUDES */
#include <qobject.h>

/* OTHER INCLUDES */
#include "quantaplugin.h"

/**Quanta plugin for command line programs
  *@author Marc Britton
  */

class KProcess;

class QWidget;

class QuantaCmdPlugin : public QuantaPlugin
{
  Q_OBJECT
public: 
  QuantaCmdPlugin();
  ~QuantaCmdPlugin();
protected:
  bool m_firstOutput;
  KProcess *m_process;
public slots:
  /** Stops the command process */
  virtual bool unload();
  /** Loads the plugin */
  /** Prepares the plugin including locating the program, setting command line arguments and windows for output */
  virtual bool load();
  /** Runs the command */
  virtual bool run();
  /** Returns if the plugin should be toggled for actions */
  virtual bool toggle() {return QuantaPlugin::toggle();}
  /** Returns TRUE if the plugin is currently loaded */
  virtual bool isLoaded();
signals:
  void wroteStdout(const QString &);
  void wroteStderr(const QString &);
protected slots:
  /** Writes (inserts or appends) processes stdout */
  void writeStdout(KProcess *, char *, int);
  /** Writes (inserts or appends) processes stderr */
  void writeStderr(KProcess *, char *, int);
  /** Does post process cleanup */
  void cleanupProcess(KProcess *);
protected:
  /** Creates a QString from an array of char */
  QString makeQString(const char *, int);

};

#endif
