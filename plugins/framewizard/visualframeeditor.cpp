/***************************************************************************
                          visualframeeditor.cpp  -  description
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

#include "visualframeeditor.h"

VisualFrameEditor::VisualFrameEditor(QWidget * parent, const char * name) : QHBox(parent,name){
  t = new tree;
  splitterList = new QPtrList<QSplitter>;
  splitterList->setAutoDelete(true);

  STBList = new QPtrList<SelectableArea>;
  STBList->setAutoDelete(true);
}
VisualFrameEditor::~VisualFrameEditor(){
}

void VisualFrameEditor::setNodeSplitMode(QString n,QString m){
  t->findNode(n)->setSplit(m);
}

void VisualFrameEditor::split(QString l, int n, QString type) {
   if(l==t->getRoot()->getLabel()){
    t->getRoot()->setSplit(type);
    for(int i = 1; i<=n; i++)
      t->addChildNode(l);
    }
   else {
         QString parentLabel=t->findNode(l)->getParentLabel();
         QString parentSplit=t->findNode(parentLabel)->getSplit();
         if(parentSplit!=type) {
                                t->findNode(l)->setSplit(type);
                                for(int i = 1; i<=n; i++)
                                  t->addChildNode(l);
          }
         else {
               if(parentLabel!=t->getRoot()->getLabel()) {
                 t->removeChildNode(parentLabel,l);
                 for(int i = 1; i<=n; i++)
                   t->addChildNode(parentLabel);
                  }
               else {
                  t->getRoot()->removeChildNode(l);
                  for(int i = 1; i<=n; i++)
                    t->addChildNode(parentLabel);
                    }
         }
     }
}

void VisualFrameEditor::draw(){
  repaint();
}

void VisualFrameEditor::paintEvent ( QPaintEvent * ){
  hide();
  splitterList->clear();
  STBList->clear();
  t->draw(t->getRoot(),this,form,splitterList,STBList);
  show();
}

void VisualFrameEditor::removeNode(QString l){
  if(l==t->getRoot()->getLabel()) t->reinitialize();
  else
  {
   QString parentLabel=t->findNode(l)->getParentLabel();
   if(t->findNode(parentLabel)->countChildren()>=2)
    {
     t->removeChildNode(parentLabel,l);
     }
  }
}
