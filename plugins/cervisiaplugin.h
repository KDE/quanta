/***************************************************************************
                          cervisiaplugin.h  -  description
                             -------------------
    begin                : Tue Sep 24 2002
    copyright            : (C) 2002 by Andras Mantia
    email                : amantia@freemail.hu
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef CERVISIAPLUGIN_H
#define CERVISIAPLUGIN_H

#include "quantakpartplugin.h"

/**A Cervisia specific KPart plugin.
  *@author Andras Mantia
  */

class QuantaApp;

class CervisiaPlugin : public QuantaKPartPlugin  {
public: 
	CervisiaPlugin(QuantaApp *);
	~CervisiaPlugin();
public slots:
  virtual bool run();
  virtual bool unload();
};

#endif
