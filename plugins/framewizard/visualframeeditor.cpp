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
#include <qdom.h>
#include <qfile.h>
#include <qstring.h>
#include <qobjectlist.h>// forse non serve
#include <kdebug.h>
#include <khtmlview.h>

VisualFrameEditor::VisualFrameEditor(QWidget * parent, const char * name) : QHBox(parent,name){
  t = new tree;
  splitterList.setAutoDelete(true);
  SAList.setAutoDelete(true);
}
VisualFrameEditor::~VisualFrameEditor(){
  if(t) delete t;
  t = 0;
}

void VisualFrameEditor::setNodeSplitMode(QString node,QString mode){
  t->findNode(node)->setSplit(mode);
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
               t->getRoot()->removeChildNode(l);
               for(int i = 1; i<=n; i++)
                 t->addChildNode(parentLabel);

         }
     }
}

void VisualFrameEditor::draw(){
  repaint();
}

void VisualFrameEditor::paintEvent ( QPaintEvent * ){
  hide();
  SAList.clear();
  splitterList.clear();
  draw2(t->getRoot(),this);
  show();
}

void VisualFrameEditor::removeNode(QString l){
  if(l==t->getRoot()->getLabel()) t->reinitialize();
  else
  {
   QString parentLabel=t->findNode(l)->getParentLabel();
   if(t->findNode(parentLabel)->countChildren()>=3)
    {
     t->removeChildNode(parentLabel,l);
     }
   else
     {
      t->removeChildNode(parentLabel,l);
      areaAttribute *tmp = new areaAttribute(t->findNode(parentLabel)->firstChild()->getAtts());
      t->findNode(parentLabel)->removeChildren();
      t->findNode(parentLabel)->setAtts(tmp);
     }
  }
}


void VisualFrameEditor::draw2(treeNode *n, QWidget* parent){

    if(n->hasChildren()) {
        QSplitter *splitter = new QSplitter(parent);
	splitterList.append(splitter);
	if(n->getSplit() == "v") splitter->setOrientation(QSplitter::Horizontal);
	if(n->getSplit() == "h") splitter->setOrientation(QSplitter::Vertical);
	n->firstChild();
	while(n->getCurrentChild()){
	    draw2(n->getCurrentChild(),splitter);
	    n->nextChild();
	}
    }
    else{
	SelectableArea *te=new SelectableArea(parent);
	SAList.append(te);
        te->setIdLabel( n->getLabel() );
        //te->view()->setMinimumSize(QSize(20,25));
        te->setSource(n->getAtts()->getSrc());
        //QObject::connect(te, SIGNAL(Resized(QRect)), n->getAtts(), SLOT(setGeometry(QRect)));
	QObject::connect(te, SIGNAL(selected(QString)),form, SLOT(catchSelectedArea(QString)));
    }
}
