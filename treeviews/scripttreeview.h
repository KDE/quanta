/***************************************************************************
                          scripttreeview.h  -  description
                             -------------------
    begin                : Thu Sep 16 2003
    copyright            : (C) 2003 by Andras Mantia
    email                : amantia@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef SCRIPTTREEVIEW_H
#define SCRIPTTREEVIEW_H

#include "filestreeview.h"

class KURL;

class ScriptTreeView : public FilesTreeView  {
   Q_OBJECT
public:
   ScriptTreeView(const KURL& projectBaseURL, QWidget *parent=0, const char *name=0);
  ~ScriptTreeView();

private:
   KURL m_baseURL;
};

#endif