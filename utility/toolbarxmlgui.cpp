/***************************************************************************
                          toolbarxmlgui.cpp  -  description
                             -------------------
    begin                : Sat Jul 20 2002
    copyright            : (C) 2002 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/

#include "toolbarxmlgui.h"

ToolbarXMLGUI::ToolbarXMLGUI(const QString& xmlFile)
 : KXMLGUIClient()
{
  setLocalXMLFile(xmlFile);
  setXMLFile(xmlFile, false);
}


ToolbarXMLGUI::~ToolbarXMLGUI(){
}
