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
  virtual bool unload();
  virtual bool load();
  virtual bool run();
  virtual bool toggle() {return QuantaPlugin::toggle();}
  virtual bool isLoaded();
  /** Does post process cleanup */
  void cleanupProcess(KProcess *);
  /** Writes (inserts or appends) process' output */
  void writeOutput(KProcess *, char *, int);
};

#endif
