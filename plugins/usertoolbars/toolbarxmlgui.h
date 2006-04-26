/***************************************************************************
                          toolbarxmlgui.h  -  description
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


#ifndef TOOLBARXMLGUI_H
#define TOOLBARXMLGUI_H

#include <kxmlguiclient.h>

/**
* GUI client for user toolbars.
* @author Andras Mantia
*/

class ToolbarXMLGUI : public KXMLGUIClient  {
public:
  
  /**
   * Create a GUI client which uses the file specified as parameter.
   * @param xmlFile the xml file describing this client
   */
  ToolbarXMLGUI(const QString& xmlFile);
  ~ToolbarXMLGUI();
};

#endif
