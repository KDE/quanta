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
#include <qobject.h>
#include <qfile.h>
#include <qtextstream.h>
#include <math.h>
#include <qmap.h>

const int SIZE = 101;

treeNode::treeNode(QString l,QString pl) : label(l), parentLabel(pl), split("n"){
  childrenList.setAutoDelete(true);
  atts = new areaAttribute;
}



treeNode::~treeNode(){
   if(atts) delete atts;
   atts = 0;
}


void treeNode::addChildNode(QString l) {
   QString parentLabel = label;
   childrenList.append( new treeNode(l,parentLabel) );
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
  root = new treeNode(QString::number(nodeId,10));
  //nodeList.insert(QString::number(nodeId,10),root);
  //nodeList.setAutoDelete(true);

  nodeList.resize(SIZE);
}


tree::~tree(){
  if(root) delete root;
  root =  0;
}





void tree::refreshGeometries(treeNode *n){
   int dim = -6;// so we won't add exceeding pixels
   if(n->hasChildren()){

      n->firstChild();
	   while(n->getCurrentChild()){
	      refreshGeometries(n->getCurrentChild());
	      n->nextChild();
       }
      QPtrList<treeNode> list=n->getChildrenList();
      QPtrListIterator<treeNode> it( list );
      treeNode *node= it.current();
      QRect newGeometry = n->getAtts()->getGeometry();
      if(n->getSplit()=="v"){
            newGeometry.setHeight(node->getAtts()->getGeometry().height());
            while ( (node = it.current()) != 0 ) {
               ++it;
               dim += node->getAtts()->getGeometry().width();
               dim += 6;
         }

       newGeometry.setWidth(dim);
      }
       else
       if(n->getSplit()=="h"){
         newGeometry.setWidth(node->getAtts()->getGeometry().width());
         while ( (node = it.current()) != 0 ) {
            ++it;
            dim += node->getAtts()->getGeometry().height();
            dim += 6;
         }

       newGeometry.setHeight(dim);

            }
   n->getAtts()->setGeometry( newGeometry );
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
    percentage/=n->getAtts()->getGeometry().width();
    for(int i=1;i<=child_number;++i){
      dimMap[i]=n->getChildrenList().at(i-1)->getAtts()->getGeometry().width();
      }
    }
  else
  if(n->getSplit()=="h") {
    percentage/=n->getAtts()->getGeometry().height();
    for(int i=1;i<=child_number;++i){
      dimMap[i]=n->getChildrenList().at(i-1)->getAtts()->getGeometry().height();
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

QString tree::createFrameTag(areaAttribute *a){

  QString Src =          a->getAttributeValue("src"),
          Longdesc =     a->getAttributeValue("longdesc"),
          Name =         a->getAttributeValue("name"),
          Scrolling =    a->getAttributeValue("scrolling"),
          Id =           a->getAttributeValue("id"),
          Style =        a->getAttributeValue("style"),
          Title =        a->getAttributeValue("title"),
          Class =        a->getAttributeValue("class"),
          Noresize =     a->getAttributeValue("noresize"),
          Frameborder =  a->getAttributeValue("frameborder"),
          Marginwidth =  a->getAttributeValue("marginwidth"),
          Marginheight = a->getAttributeValue("marginheight");

  QString tagBegin="<frame",
          tagEnd=">\n",
          tagMiddle="";

  if( !Src.isEmpty() )       tagMiddle+=(" src=\""+a->getAttributeValue("src")+"\"");
  if( !Longdesc.isEmpty() )  tagMiddle+=(" longdesc=\""+a->getAttributeValue("longdesc")+"\"");
  //if( !Name.isEmpty() )
  tagMiddle+=(" name=\""+a->getAttributeValue("name")+"\"");
  if( Scrolling!="auto" && !Scrolling.isEmpty() )    tagMiddle+=(" scrolling=\""+a->getAttributeValue("scrolling")+"\"");
  if( !Id.isEmpty() )        tagMiddle+=(" id=\""+a->getAttributeValue("id")+"\"");
  if( !Style.isEmpty() )     tagMiddle+=(" style=\""+a->getAttributeValue("style")+"\"");
  if( !Title.isEmpty() )     tagMiddle+=(" title=\""+a->getAttributeValue("title")+"\"");
  if( !Class.isEmpty() )     tagMiddle+=(" class=\""+a->getAttributeValue("class")+"\"");
  if( Noresize=="noresize" ) tagMiddle+=(" "+a->getAttributeValue("noresize"));
  if( Frameborder=="0" )     tagMiddle+=(" frameborder=\""+a->getAttributeValue("frameborder")+"\"");
  if( Marginwidth!="0" && !Marginwidth.isEmpty() )     tagMiddle+=(" marginwidth=\""+a->getAttributeValue("marginwidth")+"\"");
  if( Marginheight!="0" && !Marginheight.isEmpty())    tagMiddle+=(" marginheight=\""+a->getAttributeValue("marginheight")+"\"");

 return tagBegin+tagMiddle+tagEnd;
}



int tree::tabNum = 0; // number of tabulation character used to indent the frame structure



void tree::createStructure(treeNode* n){

if(n->hasChildren()) {

        if(n->getSplit()=="v")
          nonFormattedStructure.append("<frameset cols=\""+RCvalue(n)+"\">\n");
        else 
        if(n->getSplit()=="h")
          nonFormattedStructure.append("<frameset rows=\""+RCvalue(n)+"\">\n");
        n->firstChild();
	while(n->getCurrentChild()){
	    createStructure(n->getCurrentChild());
	    n->nextChild();
	}
          nonFormattedStructure.append("</frameset>\n");
    }
    else
      nonFormattedStructure.append(createFrameTag(n->getAtts()));
}

QString tree::formatStructure(){
  QString s="";
  refreshGeometries(root);
  createStructure(root);
  for ( QStringList::Iterator it = nonFormattedStructure.begin(); it != nonFormattedStructure.end(); ++it ) {
        if((*it).contains("<frameset")) tabNum++;
        else if((*it).contains("</frameset>")) {tabNum--;s.truncate(s.length()-1);}
        s+=*it;
        for(int i=1;i<=tabNum;i++)
          s+='\t';
    }
  return s;
}

QString tree::framesetStructure() {

  //the code between comments it's only for demonstration purpose ******************************
  QString header="<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Frameset//EN\" \"http://www.w3.org/TR/html4/frameset.dtd\"><html><title>Test page</title><head></head>\n",
          footer="</html>";
  QFile file( "./testpage.html" );
    if ( file.open( IO_WriteOnly ) ) {
        QTextStream stream( &file );

            stream << (header+formatStructure()+footer);
        file.close();
    }
  //********************************************************************************************
  return formatStructure();
}

treeNode* tree::findNode(QString l){
  if(l==root->getLabel()) return root;
 return nodeList.find(l);
}

QString tree::addChildNode(QString l){
    treeNode *node;
    if( (node = findNode(l)) != 0) {
      ++nodeId;
      treeNode *newNode = new treeNode(QString::number(nodeId,10),node->getLabel());
      newNode->setAtts( "src",node->getAtts()->getSrc() );
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
   newNode->setAtts( "src",findNode(l)->getAtts()->getSrc() );
   nodeList.insert(QString::number(nodeId,10),newNode);
   return findNode( parent )->insertChild(pos,newNode);
}

void tree::reinitialize(){
  nodeId = 1;
  root->removeChildren();
  root->getAtts()->resetAttributes();
  nodeList.clear();
}

void tree::removeChildNode(QString l,QString ll){
  findNode(l)->removeChildNode(ll);
}





