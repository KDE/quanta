/***************************************************************************
                          csseditor.h  -  description
                             -------------------
    begin                : sab ago 24 2002
    copyright            : (C) 2002 by Andras Mantia, Andrea Bergia
    email                : amantia@freemail.hu, andreabergia@yahoo.it
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef CSSEDITOR_H
#define CSSEDITOR_H

#include <qwidget.h>
#include "csseditors.h"

// Forward classes declarations
class QStringList;

/** @author Andrea Bergia */
class CSSEditor : public CSSEditorS  {
   Q_OBJECT
public:
  CSSEditor(QString code, QWidget *parent=0, const char *name=0);
  ~CSSEditor();

  QString code();

protected slots:
  /** Show the window to add a selector */
  void slotAddSelector();

  /** Show the window to edit an existing selector */
  void slotEditSelector();

  /** Ask to remove an existing selector */
  void slotRemoveSelector();

  /** Copy a selector into anothers */
  void slotCopySelector();

  /** Place all the selector contained in the string list into the list box */
  void slotShowSelectors ();
  
protected:
  /** All the parsed selectors, in the form "Selector { css code } */
  QStringList selectors;
};

#endif
