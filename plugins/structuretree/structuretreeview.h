/***************************************************************************
    begin                : Thu Apr 17 2008
    copyright            : (C) 2008 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/


//kate: indent-mode cstyle; space-indent on; indent-width 2; replace-tabs on; mixedindent off; encoding utf-8
#ifndef STRUCTURETREEVIEW_H
#define STRUCTURETREEVIEW_H

#include <QTreeView>

class ParseResult;

/**
	@author Andras Mantia <amantia@kde.org>
*/
class StructureTreeView : public QTreeView
{
Q_OBJECT
public:
    StructureTreeView(QWidget *parent = 0);

    ~StructureTreeView();
    
public Q_SLOTS:    
  
  void newDataArrived(const ParseResult *data);

  private:
    const ParseResult *m_parseResult;
};

#endif
