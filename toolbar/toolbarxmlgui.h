/***************************************************************************
                          toolbarxmlgui.h  -  description
                             -------------------
    begin                : Sat Jul 20 2002
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

#ifndef TOOLBARXMLGUI_H
#define TOOLBARXMLGUI_H

#include <kxmlguiclient.h>

/**
  *@author Andras Mantia
  */

class ToolbarXMLGUI : public KXMLGUIClient  {
public:
  ToolbarXMLGUI(const QString& xmlFile);
  ~ToolbarXMLGUI();
};

#endif
