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
#include "fwglobal.h"
#include <qstringlist.h>
#include <qdom.h>
#include <qstring.h>
#include <qmap.h>
#include <kdebug.h>
#include <khtmlview.h>
#include <qobjectlist.h>


QMap<QString, QValueList< int > > SIZES;
static QString lastModified;
static int splitterIdNumber = 0;

VisualFrameEditor::VisualFrameEditor(QWidget * parent, const char * name) : QHBox(parent,name){
  t = new tree;
  splitterList.setAutoDelete(true);
  SAList.setAutoDelete(true);
  lastModified=t->getRoot()->getLabel();
  t->getRoot()->getAtts()->setGeometry(QRect(0,0,510,422));
  //t->getRoot()->getAtts()->setGeometry(geometry());
}

VisualFrameEditor::~VisualFrameEditor(){
  if(t) delete t;
  t = 0;
}

void VisualFrameEditor::setNodeSplitMode(QString node,QString mode){
  t->findNode(node)->setSplit(mode);
}


void VisualFrameEditor::setGeometries(QString l){

   int cP=cancelledPixels(t->findNode(l)->countChildren());
   QRect newGeometry(t->findNode(l)->getAtts()->getGeometry());
   QPtrList<treeNode> list=t->findNode(l)->getChildrenList();
   QPtrListIterator<treeNode> it( list );
   treeNode *node;
	if(t->findNode(l)->getSplit() == "v"){
      int dummyDimension=t->findNode(l)->getAtts()->getGeometry().width()-cP;
      while ( (node = it.current()) != 0 ) {
         ++it;
         newGeometry.setWidth( int(dummyDimension/t->findNode(l)->countChildren()) );
         t->findNode(node->getLabel())->getAtts()->setGeometry(newGeometry);
         }
    }
    else
	 if(t->findNode(l)->getSplit() == "h"){
      int dummyDimension=t->findNode(l)->getAtts()->getGeometry().height()-cP;
      while ( (node = it.current()) != 0 ) {
         ++it;
         newGeometry.setHeight( int(dummyDimension/t->findNode(l)->countChildren()) );
         t->findNode(node->getLabel())->getAtts()->setGeometry(newGeometry);
      }
    }
}


void VisualFrameEditor::split(QString l, int n, QString type) {
   if(l==t->getRoot()->getLabel()){
    t->getRoot()->setSplit(type);
    for(int i = 1; i<=n; i++)
      t->addChildNode(l);
    setGeometries(l);
    }
   else {
         QString parentLabel=t->findNode(l)->getParentLabel();
         QString parentSplit=t->findNode(parentLabel)->getSplit();
         if(parentSplit!=type) {
                                t->findNode(l)->setSplit(type);
                                for(int i = 1; i<=n; i++)
                                  t->addChildNode(l);
                                setGeometries(l);
         }
         else {
               for(int i = 1; i<=n; i++)
                 t->insertChildNode(l);
               t->findNode(parentLabel)->removeChildNode(l);
               setGeometries(t->findNode(parentLabel)->getLabel());
         }
     }


}


void VisualFrameEditor::loadExistingStructure(const QStringList& list){
  if(!list.isEmpty())
   {
    existingStructure = list;
    existingStructure.remove("</frameset>");//closure tag not needed
    build(t->getRoot()->getLabel());
 }
}


void VisualFrameEditor::build(QString parent){
   QString line = existingStructure.first();
   if(line.contains("<frameset")){
      if(line.contains("rows")){
         split(parent,(line.contains(",")+1),"h");

         QRegExp pattern("rows\\s*=\"([\\d%,]*)\"");
         pattern.search(line);
         QStringList percentages=QStringList::split(",",pattern.cap(1));

         QRect dummy=t->findNode(parent)->getAtts()->getGeometry();

         int dummyDimension=dummy.height()-cancelledPixels(line.contains(",")+1);

         QPtrList<treeNode> list=t->findNode(parent)->getChildrenList();
         QPtrListIterator<treeNode> it( list );
         treeNode *node;
         while ( (node = it.current()) != 0 ) {
            ++it;
            QRect newGeometry(dummy);
            double newDimension=(dummyDimension*(percentages.first().remove("%").toInt()))/100.0;
            newGeometry.setHeight( proxInt(newDimension) );
            node->getAtts()->setGeometry(newGeometry);
            percentages.pop_front();
         }
      }
      else
      if(line.contains("cols")){
         split(parent,(line.contains(",")+1),"v");
         QRegExp pattern("cols\\s*=\"([\\d%,]*)\"");
         pattern.search(line);
         QStringList percentages=QStringList::split(",",pattern.cap(1));

         QRect dummy=t->findNode(parent)->getAtts()->getGeometry();

         int dummyDimension=dummy.width()-cancelledPixels(line.contains(",")+1);

         QPtrList<treeNode> list=t->findNode(parent)->getChildrenList();
         QPtrListIterator<treeNode> it( list );
         treeNode *node;
         while ( (node = it.current()) != 0 ) {
            ++it;
            QRect newGeometry(dummy);
            double newDimension=(dummyDimension*(percentages.first().remove("%").toInt()))/100.0;
            newGeometry.setWidth( proxInt(newDimension) );
            node->getAtts()->setGeometry(newGeometry);
            percentages.pop_front();
          }
       }

      existingStructure.pop_front();
      t->findNode(parent)->firstChild();
      while(t->findNode(parent)->getCurrentChild()){
	      build(t->findNode(parent)->getCurrentChild()->getLabel());
	      t->findNode(parent)->nextChild();
	   }
   }
   else
      {
      QMap<QString,QString> attributeMap;
      if( line.contains( QRegExp("\\s+noresize\\s+") ) )
         attributeMap["noresize"] = "noresize";
      else
         attributeMap["noresize"] = "";

      QRegExp pattern("\\s+(\\w+\\s*=\\s*\"[\\w\\s\\./_\\+\\d]*\")");

      int pos = 0;

      while ( pos >= 0 ) {
         pos = pattern.search( line, pos );
         attributeMap[ pattern.cap(1).section( QRegExp("=\\s*\"") ,0,0) ] = pattern.cap(1).section(QRegExp("=\\s*\""),1,1).remove("\"");

         if ( pos >= 0 ) {
               pos += pattern.matchedLength();
         }
      }
   t->findNode(parent)->setAtts(attributeMap);
   existingStructure.pop_front();
   }
}


void VisualFrameEditor::paintEvent ( QPaintEvent * ){
  hide();
/*
 //NOTE: Don't use this, as when there are more items in the lists and
 //on item is a child of the other, when the parent is deleted, the
 //child object (eg. QSplitter) gets automatically deleted, and
 //the result will be double deletion and crash with an Alarm Clock.
 //Maybe the best is to remove the two lists completely. Andras
  SAList.clear();
  splitterList.clear();
*/
  //Delete the Selectable areas and the splitters
  QObjectList* ch = queryList("SelectableArea");
  for (uint i = 0; i < ch->count(); i++)
  {
    QObject* o = ch->at(i);
    removeChild(o); //this will delete all childr of "o"
    SAList.remove(static_cast<SelectableArea*>(o));
  }
  SAList.clear();

  ch = queryList("QSplitter");
  for (uint i = 0; i < ch->count(); i++)
  {
    QObject* o = ch->at(i);
    removeChild(o); //this will delete all children of "o"
    splitterList.remove(static_cast<QSplitter*>(o));
  }
  splitterList.clear();

  splitterIdNumber=0;
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
      if( !t->findNode(parentLabel)->firstChild()->hasChildren() ){
         areaAttribute *tmp = new areaAttribute(t->findNode(parentLabel)->firstChild()->getAtts());
         t->findNode(parentLabel)->removeChildren();
         t->findNode(parentLabel)->setAtts( tmp );
         t->findNode(parentLabel)->setSplit("n");
         }
      else{
         t->findNode(parentLabel)->setSplit( t->findNode(parentLabel)->firstChild()->getSplit() );

         QPtrList<treeNode> list = t->findNode(parentLabel)->firstChild()->getChildrenList();
         treeNode *node;
         for ( node = t->findNode(parentLabel)->firstChild()->getChildrenList().first(); node; node = t->findNode(parentLabel)->firstChild()->getChildrenList().next() ){
            QString newNodeLabel = t->addChildNode(parentLabel);
            t->findNode(newNodeLabel)->setAtts( node->getAttributeMap() );
         }
         t->removeChildNode( parentLabel,t->findNode(parentLabel)->firstChild()->getLabel() );
      }
     }
  }
  lastModified=t->findNode(l)->getParentLabel();
}


void VisualFrameEditor::draw2(treeNode *n, QWidget* parent){
   if(n->hasChildren()) {
      QString splitterName("splitter"+QString::number(++splitterIdNumber,10));
      QSplitter *splitter = new QSplitter(parent,splitterName);
      if(SIZES.contains(splitterName)) splitter->setSizes( SIZES[splitterName] );
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
      SelectableArea *sa=new SelectableArea(parent,n->getLabel());
      sa->view()->setGeometry(n->getAtts()->getGeometry());
      if(parent->isA("QSplitter"))
         ((QSplitter *)parent)->setResizeMode(sa->view(),QSplitter::KeepSize );
      SAList.append(sa);
      sa->setIdLabel( n->getLabel() );
      sa->setSource(n->getAtts()->getSrc());
      QObject::connect(sa, SIGNAL(Resized(QRect,QString)), t->findNode(sa->getIdLabel())->getAtts(), SLOT(setGeometry(QRect,QString)));
	   QObject::connect(sa, SIGNAL(selected(QString)),form, SLOT(catchSelectedArea(QString)));
      QObject::connect(sa, SIGNAL(Resized()),form, SLOT(SAResized()));

      }
}
