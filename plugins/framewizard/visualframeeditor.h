/***************************************************************************
                          visualframeeditor.h  -  description
                             -------------------
    begin                : mar mar 25 2003
    copyright            : (C) 2003 by gulmini luciano
    email                : gulmini.luciano@student.unife.it
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef VISUALFRAMEEDITOR_H
#define VISUALFRAMEEDITOR_H

#include "treenode.h"
#include "selectablearea.h"
#include <qmap.h>
#include <qhbox.h>
/**
  *@author gulmini luciano
  */

class VisualFrameEditor : public QHBox  {
private:
  tree *t;
  QWidget *form;
  void draw2(treeNode *n, QWidget* parent);
public:
	VisualFrameEditor( QWidget * parent = 0, const char * name = 0);
	~VisualFrameEditor();
        void setupForm(QWidget *f){ form = f; }
        void draw();
        QString framesetStructure(){ return t->framesetStructure(); }
        void setNodeSplitMode(QString,QString);//node,mode
        void reset(){ t->reinitialize(); }
        void removeNode(QString l);
        void split(QString,int,QString);
        void setAllAttribute(QString l,QMap<QString,QString> map) { (t->findAreaAttribute(l)->setAllAttribute(map)); }
        QMap<QString,QString> getAttributeMap(QString l) { return t->findAreaAttribute(l)->getAttributeMap(); }
        QString initSA() const { return t->getRoot()->getLabel(); }
protected:
        virtual void paintEvent ( QPaintEvent * );
};

#endif
