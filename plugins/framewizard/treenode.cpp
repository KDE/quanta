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
#include "selectablearea.h"
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

void treeNode::addChildNode(QString l){
  QString parentLabel = label;
  childrenList.append(new treeNode(l,parentLabel));
  
}

void treeNode::addChildNode(treeNode *n){
  childrenList.append(n);

}

void treeNode::removeChildNode(QString l){
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
  root = new treeNode(QString::number(nodeId,10));
  //nodeList.insert(QString::number(nodeId,10),root);
  //nodeList.setAutoDelete(true);

  nodeList.resize(SIZE);
}


tree::~tree(){
  if(root) delete root;
  root =  0;
}

int proxInt(double d){
  if((d-int(d))>= 0.5 ) return int(d)+1;
  return int(d);
}

/*
QString tree::RCvalue(treeNode *n)
{  
  QString s="";
  QMap<int,int> dimMap;
  double percentage = 0.0;
  int child_number = n->countChildren();
  int lostPixels=2;
  if(child_number > 2) lostPixels = (6*(child_number-1)); // empiric formula

  if(n->getSplit()=="v") {
    percentage=100.0/n->getAtts()->getGeometry().width();
    for(int i=1;i<=child_number;++i){
      dimMap[i]=n->getChildrenList().at(i-1)->getAtts()->getGeometry().width();
      }
    }
  else
  if(n->getSplit()=="h") {
    percentage=100.0/n->getAtts()->getGeometry().height();
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
            s+=QString::number(proxInt(dimMap[i]*percentage),10);
            s+="%,";
           }

   return s+="*";
}*/


QString tree::RCvalue(treeNode *n)
{
  QString s="";
  int v=100/n->countChildren();
  for(int i=1;i<n->countChildren();i++){
    s+=QString::number(v,10);
            s+="%,";
   }
   return s+="*";
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
          Frameborder =  a->getAttributeValue("frameborder");
          //Marginwidth =  a->getAttributeValue("marginwidth"),
          //Marginheight = a->getAttributeValue("marginheight");

  QString tagBegin="<frame",
          tagEnd=">\n",
          tagMiddle="";

  if( !Src.isEmpty() )       tagMiddle+=(" src=\""+a->getAttributeValue("src")+"\"");
  if( !Longdesc.isEmpty() )  tagMiddle+=(" longdesc=\""+a->getAttributeValue("longdesc")+"\"");
  if( !Name.isEmpty() )      tagMiddle+=(" name=\""+a->getAttributeValue("name")+"\"");
  if( Scrolling!="auto" )    tagMiddle+=(" scrolling=\""+a->getAttributeValue("scrolling")+"\"");
  if( !Id.isEmpty() )        tagMiddle+=(" id=\""+a->getAttributeValue("id")+"\"");
  if( !Style.isEmpty() )     tagMiddle+=(" style=\""+a->getAttributeValue("style")+"\"");
  if( !Title.isEmpty() )     tagMiddle+=(" title=\""+a->getAttributeValue("title")+"\"");
  if( !Class.isEmpty() )     tagMiddle+=(" class=\""+a->getAttributeValue("class")+"\"");
  if( Noresize=="noresize" ) tagMiddle+=(" "+a->getAttributeValue("noresize"));
  if( Frameborder=="0" )     tagMiddle+=(" frameborder=\""+a->getAttributeValue("frameborder")+"\"");
  //if(!marginwidth.isEmpty())
  tagMiddle+=(" marginwidth=\""+a->getAttributeValue("marginwidth")+"\"");
  //if(!marginheight.isEmpty())
  tagMiddle+=(" marginheight=\""+a->getAttributeValue("marginheight")+"\"");

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

void tree::addChildNode(QString l){
    treeNode *node;
    if( (node = findNode(l)) != 0) {
      ++nodeId;
      treeNode *newNode = new treeNode(QString::number(nodeId,10),node->getLabel());
      node->addChildNode(newNode);
      nodeList.insert(QString::number(nodeId,10),newNode);
   }
}

void tree::reinitialize(){
  nodeId = 1;
  root->removeChildren();
  nodeList.clear();
}

void tree::removeChildNode(QString l,QString ll){
  findNode(l)->removeChildNode(ll);
}

