/***************************************************************************
                          treenode.cpp  -  description
                             -------------------
    begin                : lun mar 17 2003
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

#include "treenode.h"

#include "fwglobal.h"
#include "qextfileinfo.h"

#include <qobject.h>
#include <qfile.h>
#include <qtextstream.h>
#include <math.h>
#include <qmap.h>

#include <kurl.h>


const int SIZE = 101;

treeNode::treeNode(QString l,QString pl) : label(l), parentLabel(pl), split("n"){
  childrenList.setAutoDelete(true);
  m_atts = new areaAttribute;
}



treeNode::~treeNode(){
  delete m_atts;
}


void treeNode::addChildNode(QString l) {
   QString parentLabel = label;
   childrenList.append( new treeNode(l,parentLabel) );
}

 void treeNode::removeChildNode(QString l,bool autoDelete) {
 childrenList.setAutoDelete(autoDelete);
 childrenList.remove(findChild(l));
 }


treeNode* treeNode::findChild(QString l){

  QPtrListIterator<treeNode> it( childrenList );
    treeNode *node;
    while ( (node = it.current()) != 0 ) {
        ++it;
        if(node->getLabel() == l) return node;
    }
 return 0;
}



int tree::nodeId = 0;

tree::tree(){
  m_root = new treeNode(QString::number(nodeId,10));
  nodeList.resize(SIZE);
}

tree::~tree(){
  delete m_root;
}

void tree::refreshGeometries(treeNode *n){
   int dim = -6;// so we won't add exceeding pixels
   if(n->hasChildren()){

      n->firstChild();
     while(n->currentChild()){
        refreshGeometries(n->currentChild());
        n->nextChild();
       }
      QPtrList<treeNode> list=n->getChildrenList();
      QPtrListIterator<treeNode> it( list );
      treeNode *node= it.current();
      QRect newGeometry = n->atts()->geometry();
      if(n->getSplit()=="v"){
            newGeometry.setHeight(node->atts()->geometry().height());
            while ( (node = it.current()) != 0 ) {
               ++it;
               dim += node->atts()->geometry().width();
               dim += 6;
         }

       newGeometry.setWidth(dim);
      }
       else
       if(n->getSplit()=="h"){
         newGeometry.setWidth(node->atts()->geometry().width());
         while ( (node = it.current()) != 0 ) {
            ++it;
            dim += node->atts()->geometry().height();
            dim += 6;
         }

       newGeometry.setHeight(dim);

            }
   n->atts()->setGeometry( newGeometry );
   }


}



QString tree::RCvalue(treeNode *n)
{
  QString s="";
  QMap<int,int> dimMap;
  double percentage = 100.0;
  int remainingPercentage=100;
  int child_number = n->countChildren();
  int lostPixels = (6*(child_number-1)); // 6 pixels are lost every time a splitter is drawn

  if(n->getSplit()=="v") {
    percentage/=n->atts()->geometry().width();
    for(int i=1;i<=child_number;++i){
      dimMap[i]=n->getChildrenList().at(i-1)->atts()->geometry().width();
      }
    }
  else
  if(n->getSplit()=="h") {
    percentage/=n->atts()->geometry().height();
    for(int i=1;i<=child_number;++i){
      dimMap[i]=n->getChildrenList().at(i-1)->atts()->geometry().height();
      }
   }

   while( lostPixels > 0)
        {
          for(int i=1;i<=child_number;++i){
            dimMap[i]+=1;
            lostPixels--;
            if(lostPixels == 0) break;
           }
         }
      for(int i=1;i<=child_number-1;++i){
            remainingPercentage-=proxInt(dimMap[i]*percentage);
            s+=QString::number(proxInt(dimMap[i]*percentage),10);
            s+="%,";
           }

   return s+=(QString::number(remainingPercentage,10)+"%");
}

/*
dummySrc is the src value obtained from the frame editor dialog and has absolute path
including the name of the file used as src.
path is the the absolute path of the file containing the frameset structure
*/



QString tree::createFrameTag(areaAttribute *a){
  QString Src =          a->attributeValue("src"),
          Longdesc =     a->attributeValue("longdesc"),
          Name =         a->attributeValue("name"),
          Scrolling =    a->attributeValue("scrolling"),
          Id =           a->attributeValue("id"),
          Style =        a->attributeValue("style"),
          Title =        a->attributeValue("title"),
          Class =        a->attributeValue("class"),
          Noresize =     a->attributeValue("noresize"),
          Frameborder =  a->attributeValue("frameborder"),
          Marginwidth =  a->attributeValue("marginwidth"),
          Marginheight = a->attributeValue("marginheight");

  QString tagBegin="<frame",
          tagEnd=">\n",
          tagMiddle=QString::null;




  if( !Src.isEmpty() )
  {
    KURL u;
    u.setPath(reldPath);
    KURL base;
    base.setPath(Src);
    tagMiddle+= (" src=\"" + QExtFileInfo::toRelative( u, base).path() + "\"");
  }       
      
  if( !Longdesc.isEmpty() )
      tagMiddle+= (" longdesc=\""+Longdesc+"\"");
  //if( !Name.isEmpty() )
  tagMiddle+=(" name=\""+Name+"\"");
  if( Scrolling!="auto" && !Scrolling.isEmpty() )    tagMiddle+=(" scrolling=\""+Scrolling+"\"");
  if( !Id.isEmpty() )        tagMiddle+=(" id=\""+Id+"\"");
  if( !Style.isEmpty() )     tagMiddle+=(" style=\""+Style+"\"");
  if( !Title.isEmpty() )     tagMiddle+=(" title=\""+Title+"\"");
  if( !Class.isEmpty() )     tagMiddle+=(" class=\""+Class+"\"");
  if( Noresize=="noresize" ) tagMiddle+=(" "+Noresize);
  if( Frameborder=="0" )     tagMiddle+=(" frameborder=\""+Frameborder+"\"");
  if( Marginwidth!="0" && !Marginwidth.isEmpty() )     tagMiddle+=(" marginwidth=\""+Marginwidth+"\"");
  if( Marginheight!="0" && !Marginheight.isEmpty())    tagMiddle+=(" marginheight=\""+Marginheight+"\"");


 return tagBegin+tagMiddle+tagEnd;
}



int tree::tabNum = 0; // number of tabulation character used to indent the frame structure



void tree::createStructure(treeNode* n){
if(n==m_root && !n->hasChildren()) return;
if(n->hasChildren()) {

        if(n->getSplit()=="v")
          nonFormattedStructure.append("<frameset cols=\""+RCvalue(n)+"\">\n");
        else
        if(n->getSplit()=="h")
          nonFormattedStructure.append("<frameset rows=\""+RCvalue(n)+"\">\n");
        n->firstChild();
  while(n->currentChild()){
      createStructure(n->currentChild());
      n->nextChild();
  }
          nonFormattedStructure.append("</frameset>\n");
    }
    else
      nonFormattedStructure.append(createFrameTag(n->atts()));
}

QString tree::formatStructure(){
  QString s(QString::null);
  refreshGeometries(m_root);
  createStructure(m_root);
  for ( QStringList::Iterator it = nonFormattedStructure.begin(); it != nonFormattedStructure.end(); ++it ) {
        if((*it).contains("<frameset")) tabNum++;
        else if((*it).contains("</frameset>")) {tabNum--;s.truncate(s.length()-1);}
        s+=*it;
        for(int i=1;i<=tabNum;i++)
          s+='\t';
    }
  nonFormattedStructure.clear();
  return s;
}


treeNode* tree::findNode(QString l){
  if(l==m_root->getLabel()) return m_root;
 return nodeList.find(l);
}

QString tree::addChildNode(QString l){
    treeNode *node;
    if( (node = findNode(l)) != 0) {
      ++nodeId;
      treeNode *newNode = new treeNode(QString::number(nodeId,10),node->getLabel());
      newNode->setAtts( "src",node->atts()->src() );
      node->addChildNode(newNode);
      nodeList.insert(QString::number(nodeId,10),newNode);
   }
   return QString::number(nodeId,10);
}

bool tree::insertChildNode(QString l){
   QString parent = findNode(l)->getParentLabel();
   int pos=findNode( parent )->childPosition( findNode(l) );
   ++nodeId;
   treeNode *newNode = new treeNode(QString::number(nodeId,10),parent);
   newNode->setAtts( "src",findNode(l)->atts()->src() );
   nodeList.insert(QString::number(nodeId,10),newNode);
   return findNode( parent )->insertChild(pos,newNode);
}

void tree::reinitialize(){
  nodeId = 1;
  m_root->removeChildren();
  m_root->atts()->resetAttributes();
  nodeList.clear();
}

void tree::removeChildNode(QString l,QString ll,bool autoDelete){
  findNode(l)->removeChildNode(ll,autoDelete);
}





