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
#include <qsplitter.h>
#include <qptrlist.h>
#include <qvaluelist.h>
/**
  *@author gulmini luciano
  */

class VisualFrameEditor : public QHBox  {
   Q_OBJECT
   private:
      tree *t;
      SelectableArea *firstInsertedSA;
      QStringList existingStructure;

      void build(QString,QString);
      void setGeometries(QString);
      void draw2(treeNode *n, QWidget* parent);
      QStringList convertAsterisks(QString,int);

   public:
      VisualFrameEditor( QWidget * parent = 0, const char * name = 0);
      ~VisualFrameEditor();
      void draw() { repaint(); }
      void loadExistingStructure(QStringList,QString);
      QString framesetStructure(QString path){ return (t->framesetStructure(path)); }
      void setNodeSplitMode(QString node,QString mode){ t->findNode(node)->setSplit(mode); }
      void reset();//{ t->reinitialize(); }
      void removeNode(QString l);
      void split(QString,int,QString);
      void setAllAttributes(QString l,QMap<QString,QString> map) { (t->findAreaAttribute(l)->setAllAttributes(map)); }
      QMap<QString,QString> getAttributeMap(QString l) { return t->findAreaAttribute(l)->attributeMap(); }
      QString initSA() const { return t->root()->getLabel(); }

   protected:
      virtual void paintEvent ( QPaintEvent * );
   signals:
      void areaSelected(QString);
      void SAResized();
};

#endif
