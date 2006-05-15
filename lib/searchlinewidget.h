/***************************************************************************
    begin                : Tue Jul 19 2005
    copyright            : (C) 2005-2006 by Jens Herden <jens@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/
#ifndef SEARCHLINEWIDGET_H
#define SEARCHLINEWIDGET_H

#include <ktreewidgetsearchline.h>

class KTreeWidgetSearchLine;
class QWidget;
class QTreeWidget;


/**
@author Jens Herden
*/
class SearchLineWidget : public KTreeWidgetSearchLineWidget
{
  Q_OBJECT
public:
  SearchLineWidget(QWidget *parent = 0, QTreeWidget * treeWidget = 0);

  ~SearchLineWidget();

protected:
  virtual KTreeWidgetSearchLine * createSearchLine(QTreeWidget * treeWidget) const;

};

#endif

// kate: space-indent on; indent-width 2; mixedindent off; indent-mode cstyle;
