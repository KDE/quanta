/***************************************************************************
                          quantapropertiespagedlg.h  -  description
                             -------------------
    begin                : Wed Jun 26 2002
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

#ifndef QUANTAPROPERTIESPAGEDLG_H
#define QUANTAPROPERTIESPAGEDLG_H

#include <qwidget.h>
#include <quantapropertiespage.h>

/**
  *@author Andras Mantia
  */

class QuantaPropertiesPageDlg : public QuantaPropertiesPage  {
   Q_OBJECT
public: 
	QuantaPropertiesPageDlg(QWidget *parent=0, const char *name=0);
	~QuantaPropertiesPageDlg();
};

#endif
