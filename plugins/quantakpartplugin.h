/***************************************************************************
                          quantakpartplugin.h  -  description
                             -------------------
    begin                : Tue Sep 17 2002
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

#ifndef QUANTAKPARTPLUGIN_H
#define QUANTAKPARTPLUGIN_H

/* KDE INCLUDES */
#include <kparts/part.h>

/* OTHER INCLUDES */
#include "quantaplugin.h"

/**
  *@author Marc Britton
  */

class QWidget;
class QuantaApp;

class QuantaKPartPlugin : public QuantaPlugin
{
  Q_OBJECT
public: 
  QuantaKPartPlugin(QuantaApp *);
  ~QuantaKPartPlugin();
  bool isLoaded() const;
public slots:
  bool load();
  bool run();
  bool unload();
protected:
  QuantaApp *m_app;
  KParts::ReadOnlyPart *m_part;
};

#endif
