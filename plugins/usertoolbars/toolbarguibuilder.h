/***************************************************************************
    begin                : Wed May 18 2005
    copyright            : (C) 2005 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/
#ifndef TOOLBARGUIBUILDER_H
#define TOOLBARGUIBUILDER_H

#include <kxmlguibuilder.h>

/**
 Builder for the toolbar XML gui client objects. It handles putting and removing the toolbar
 on a tabwidget inside a toolbar. 

@author Andras Mantia <amantia@kde.org>
*/

class ToolbarGUIBuilder:public KXMLGUIBuilder  
{
public:
  /**
   * The builder is a singleton. You can access it via this method.
   * @param widget the argument passes to the KXMLGUIBuilder constructor 
   * @return a pointer to the KXMLGUIBuilder object
   */
  static ToolbarGUIBuilder* const ref(QWidget *widget = 0L)
  {
    static ToolbarGUIBuilder *m_ref;
    if (!m_ref) m_ref = new ToolbarGUIBuilder(widget);
    return m_ref;
  }
  
  /**
   * Called when a new XML gui client is added to the gui factory. 
   * Reimplemented from KXMLGUIBuilder in order to create the toolbar inside 
   * a tabwidget, not as a standalone toolbar.
   */
  virtual QWidget *createContainer(QWidget *parent, int index, const QDomElement &element, int &id);
  /**
   * Called when a XML gui client is removed the gui factory. 
   * Reimplemented from KXMLGUIBuilder in order to remove out customly built toolbar.
   */
  virtual void removeContainer(QWidget *container, QWidget *parent, QDomElement &element, int id);

  /**
   * Sets the toolbar mode.
   * @param separate if true, the toolbars are real separate toolbars, otherwise
   * they are grouped inside a tabwidget
   */
  void setSeparateToolbars(bool separate) { m_separateToolbars = separate; }

private:
  /**
   * The private constructor. 
   * @param widget passed to the KXMLGUIBuilder constructor
   * @return 
   */
  ToolbarGUIBuilder(QWidget *widget):KXMLGUIBuilder(widget){ m_separateToolbars = false; }

  bool m_separateToolbars; ///< True when the toolbars are not on a tab widget
};

#endif
