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



VisualFrameEditor::VisualFrameEditor(QWidget * parent, const char * name) : QHBox(parent,name)
{
  t = new tree;
  splitterList.setAutoDelete(true);
  SAList.setAutoDelete(true);
  lastModified=t->getRoot()->getLabel();
  t->getRoot()->getAtts()->setGeometry(QRect(0,0,510,422));


}

VisualFrameEditor::~VisualFrameEditor(){
  if(t) delete t;
  t = 0;
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


void VisualFrameEditor::loadExistingStructure(QStringList list,QString path){
  if(!list.isEmpty())
   {
    existingStructure = list;
    existingStructure.remove("</frameset>");//closure tag not needed
    build(t->getRoot()->getLabel(),path);
 }
}

QStringList VisualFrameEditor::convertAsterisks(QString s,int d){
   QStringList list=QStringList::split(",",s);
   int leftPercentage = 100;
   int leftPercentageDistributedAmongAsterisks=0;
   int weightAsteriskCounter=0;
   // This for is used to determine how much percentage must be assign to an asterisk
   // example cols="40%,5*,*"
   // then every asterisk must be assigned a percentage of 10% so the real percentage
   // notation is cols="40%,50%,10%"
   for ( QStringList::Iterator it = list.begin(); it != list.end(); ++it ) {
   
        if(!(*it).contains("%") && !(*it).contains("*")){
            leftPercentage -= ( (*it).toInt()*100 )/d;
	    //qWarning("%s",(*it).latin1());
        }
	 
        if((*it).contains("%")){
            leftPercentage -= (*it).section("%",0,0).toInt();
	    //qWarning("%s",(*it).latin1());
        }

        if((*it).contains("*")){
             int weight= (*it).section("*",0,0).toInt();
             if(weight==0) weight=1;
             weightAsteriskCounter += weight;
	     //qWarning("%s %d",(*it).latin1(),weightAsteriskCounter);
        }
    }
   if(weightAsteriskCounter!=0)
     leftPercentageDistributedAmongAsterisks= proxInt(double(leftPercentage)/double(weightAsteriskCounter));
  // qWarning("%d",leftPercentageDistributedAmongAsterisks);
   // this for changes asterisk notation in percentage notation
   // This part of the comment is for me:
   // NB: I valori delle percentuali generati da if .. else possono non corrispondere
   // a quelli effettivamente generati dal metodo build che opera un'altra normalizzazione.
   // In genere la differenza �dell' 1%
   for ( QStringList::Iterator it = list.begin(); it != list.end(); ++it ) {
        if((*it).contains("*")){
             int weight= (*it).section("*",0,0).toInt();
             if(weight==0) weight=1;
             int newPercentage = weight*leftPercentageDistributedAmongAsterisks;
             (*it)=(QString::number(newPercentage,10)+"%");
	       //qWarning("%s",(*it).latin1());
             leftPercentage-=newPercentage;
               }
        }

    return list;
}

void VisualFrameEditor::build(QString parent, QString path){

   QString line = existingStructure.first();
   if(line.contains("<frameset")){
   
      //QRect dummy=t->findNode(parent)->getAtts()->getGeometry();
      //QPtrList<treeNode> list=t->findNode(parent)->getChildrenList();
      //QPtrListIterator<treeNode> it( list );
      //treeNode *node;
      
      if(line.contains("rows")){
         split(parent,(line.contains(",")+1),"h");

         QRegExp pattern("rows\\s*=\"([\\s\\d%,\\*]*)\"");
         pattern.search(line);
	 
	 //qWarning("%s",pattern.cap(1).latin1());
         QRect dummy=t->findNode(parent)->getAtts()->getGeometry();
         QStringList percentages = convertAsterisks(pattern.cap(1),dummy.height());
	 //for ( QStringList::Iterator it = percentages.begin(); it != percentages.end(); ++it )
	   //qWarning("%s",(*it).latin1());

         

         int dummyDimension=dummy.height()-cancelledPixels(line.contains(",")+1);

         QPtrList<treeNode> list=t->findNode(parent)->getChildrenList();
         QPtrListIterator<treeNode> it( list );
         treeNode *node;
         while ( (node = it.current()) != 0 ) {
            ++it;
            QRect newGeometry(dummy);
	    double   newDimension;
	    if(percentages.first().contains("%"))
              newDimension=(dummyDimension*(percentages.first().remove("%").toInt()))/100.0;
	    else newDimension=(double)percentages.first().toInt();  
            newGeometry.setHeight( proxInt(newDimension) );
            node->getAtts()->setGeometry(newGeometry);
            percentages.pop_front();
         }
      }
      else
      if(line.contains("cols")){
         split(parent,(line.contains(",")+1),"v");
         QRegExp pattern("cols\\s*=\"([\\s\\d%,\\*]*)\"");
         pattern.search(line);
	 
	 QRect dummy=t->findNode(parent)->getAtts()->getGeometry();
         QStringList percentages = convertAsterisks(pattern.cap(1),dummy.width());

         

         int dummyDimension=dummy.width()-cancelledPixels(line.contains(",")+1);

         QPtrList<treeNode> list=t->findNode(parent)->getChildrenList();
         QPtrListIterator<treeNode> it( list );
         treeNode *node;
         while ( (node = it.current()) != 0 ) {
            ++it;
            QRect newGeometry(dummy);
	    double   newDimension;
	    if(percentages.first().contains("%"))
              newDimension=(dummyDimension*(percentages.first().remove("%").toInt()))/100.0;
	    else newDimension=(double)percentages.first().toInt();  
            newGeometry.setWidth( proxInt(newDimension) );
            node->getAtts()->setGeometry(newGeometry);
            percentages.pop_front();
          }
       }
       
      existingStructure.pop_front();
      
      t->findNode(parent)->firstChild();
      while(t->findNode(parent)->getCurrentChild()){
	      build(t->findNode(parent)->getCurrentChild()->getLabel(),path);
	      t->findNode(parent)->nextChild();
	   }
   }
   else
      {
      QMap<QString,QString> attributeMap;
      if( line.contains( QRegExp("\\s+noresize\\s+") ) )  
         attributeMap["noresize"] = "noresize";           
      else                                                 
         attributeMap["noresize"] = QString::null;
	 
      QRegExp srcPattern("\\s+src\\s*=\\s*\"([\\w\\s\\./_\\+\\d]*)\"");	//search for files
      if(srcPattern.search(line) !=-1 ){
         attributeMap["src"] = absolutize(srcPattern.cap(1),path);
         line.remove(srcPattern);//we don't need to operate on this anymore
      }
      
     
      
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
         //qWarning("%s",t->findNode(parentLabel)->firstChild()->getSplit().latin1());
         QPtrList<treeNode> list = t->findNode(parentLabel)->firstChild()->getChildrenList();
	 //qWarning("number of children of the first child %d",list.count());
         treeNode *node;
         for ( node = list.first(); node; node = list.next() ){
              
	    QString newNodeLabel = t->addChildNode(parentLabel);
            t->findNode(newNodeLabel)->setAtts( node->getAttributeMap() );
	    //qWarning("node added");
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
         //((QSplitter *)parent)->setResizeMode(sa->view(),QSplitter::KeepSize );
	 dynamic_cast<QSplitter *>(parent)->setResizeMode(sa->view(),QSplitter::KeepSize );
      SAList.append(sa);
      sa->setIdLabel( n->getLabel() );
      sa->setSource(n->getAtts()->getSrc());
      QObject::connect(sa, SIGNAL(Resized(QRect,QString)), t->findNode(sa->getIdLabel())->getAtts(), SLOT(setGeometry(QRect,QString)));
      QObject::connect(sa, SIGNAL(selected(QString)),this, SIGNAL(areaSelected(QString)));
      
      }
}
