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

treeNode::treeNode(QString l,QString pl){
  label = l;
  parentLabel = pl;
  split = "n";
  childrenList.setAutoDelete(true);
  }
treeNode::~treeNode(){
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
  int SIZE = 101;
  nodeList.resize(SIZE);
  db.resize(SIZE);
  areaAttribute *atts = new areaAttribute;
  db.insert(QString::number(nodeId,10),atts);
  db.setAutoDelete(true);
  structure="";
}


tree::~tree(){
  if(root) delete root;
  root =  0;
}

QString tree::RCvalue(treeNode *n)
{

    QString s="";

    int child_number = n->countChildren();



    int i;
     if(n->getSplit()=="v") {
      //we gain "excedeed_pixels" pixels
      int excedeed_pixels = 100 % child_number;
      for(i=1;i<=child_number;++i){
        //we get width in percentage terms of a single frame
        double percent_width=(db.find(n->getChildrenList().at(i-1)->getLabel())->getGeometry().width()*100.0)/500.0;
        //we correct a little bit the previous "WIDTH"
	double d=ceil(percent_width);
        (d-int(d))>0.5 ? d-=0.5 : d+=0.5;
        //we add a pixel to each of the first "excedeed_pixels" frames, to compensate.
        if (excedeed_pixels > 0) d+=1;

        excedeed_pixels--;

        s+=QString::number(int(d),10);

        if(i == child_number) s+="%";
        else s+="%,";
          }
         }
      else
      if(n->getSplit()=="h") {
       //we gain "excedeed_pixels" pixels
       int excedeed_pixels = (100 % (child_number));
       for(i=1;i<=child_number;++i){
        //we get width in percentage terms of a single frame
        double percent_height=((db.find(n->getChildrenList().at(i-1)->getLabel())->getGeometry().height()+0.9)*100.0)/400.0;
        //we correct a little bit the previous "HEIGHT"
	double d=ceil(percent_height);


        excedeed_pixels--;

        s+=QString::number(int(d),10);

        if(i == child_number) s+="%";
        else s+="%,";
          }
         }

        return s;
}

QString tree::createFrameTag(QString l){
  QString src = findAreaAttribute(l)->getSrc(),
          longdesc = findAreaAttribute(l)->getLongdesc(),
          name = findAreaAttribute(l)->getName(),
          scrolling = findAreaAttribute(l)->getScrolling(),
          id = findAreaAttribute(l)->getId(),
          style = findAreaAttribute(l)->getStyle(),
          title = findAreaAttribute(l)->getTitle(),
          Class = findAreaAttribute(l)->getClass(),
          noresize = findAreaAttribute(l)->getNoresize(),
          frameborder = findAreaAttribute(l)->getFrameborder(),
          marginwidth = findAreaAttribute(l)->getMarginwidth(),
          marginheight = findAreaAttribute(l)->getMarginheight();



  QString tagBegin="<frame",
          tagEnd=">\n",
          tagMiddle="";

  if(!src.isEmpty()) tagMiddle+=(" src=\""+src+"\"");
  if(!longdesc.isEmpty()) tagMiddle+=(" longdesc=\""+longdesc+"\"");
  if(!name.isEmpty()) tagMiddle+=(" name=\""+name+"\"");
  if(scrolling!="auto") tagMiddle+=(" scrolling=\""+scrolling+"\"");
  if(!id.isEmpty()) tagMiddle+=(" id=\""+id+"\"");
  if(!style.isEmpty()) tagMiddle+=(" style=\""+style+"\"");
  if(!title.isEmpty()) tagMiddle+=(" title=\""+title+"\"");
  if(!Class.isEmpty()) tagMiddle+=(" class=\""+Class+"\"");
  if(noresize=="noresize") tagMiddle+=(" "+noresize);
  if(frameborder!="0") tagMiddle+=(" frameborder=\""+frameborder+"\"");
  if(!marginwidth.isEmpty()) tagMiddle+=(" marginwidth=\""+marginwidth+"\"");
  if(!marginheight.isEmpty()) tagMiddle+=(" marginheight=\""+marginheight+"\"");
  
 return tagBegin+tagMiddle+tagEnd;

}



int tree::tabNum = 0; // number of tabulation character used to indent the frame structure

void tree::createStructure(treeNode* n, QString last){
 if(n->hasChildren()) {
 QString lastChildLabel=n->getLastChild()->getLabel();
        if(n->getSplit()=="v")
          structure+=("<frameset cols=\""+RCvalue(n)+"\">\n");
        else if(n->getSplit()=="h")
               structure+=("<frameset rows=\""+RCvalue(n)+"\">\n");
        tabNum++;
        for(int i=1;i<=tabNum;i++)
          structure+=("\t");
        n->firstChild();
	while(n->getCurrentChild()){
	    createStructure(n->getCurrentChild(),lastChildLabel);
	    n->nextChild();
            last = n->getLastChild()->getLabel();
	}
     

           --tabNum;
           for(int i=1;i<=tabNum;i++)
                 structure+=("\t");
           structure+="</frameset>\n";

           for(int i=1;i<=tabNum;i++)
                 structure+=("\t");


    }
    else{
         /*areaAttributeDBIterator it( db );
         areaAttribute *a;
         while ( (a = it.current()) != 0 ) {
           if( a->getLabel() == n->getLabel()) break;
         ++it;

         }*/
         if(n->getLabel() == last){
           structure+=createFrameTag(n->getLabel());
         //  --tabNum;
          /* for(int i=1;i<=tabNum;i++)
                 structure+=("\t");
           structure+="</frameset>\n";*/

          /* for(int i=1;i<=tabNum;i++)
                 structure+=("\t");*/
         }
         else {
               structure+=createFrameTag(n->getLabel());
               for(int i=1;i<=tabNum;i++)
                 structure+=("\t");
               }

            }
}



QString tree::framesetStructure() {
  createStructure(root,root->getLastChild()->getLabel());
  //the code between comments it's only for demonstration purpose ******************************
  QString header="<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Frameset//EN\" \"http://www.w3.org/TR/html4/frameset.dtd\"><html><title>Test page</title><head></head>\n",
          footer="</html>";
  QFile file( "./testpage.html" );
    if ( file.open( IO_WriteOnly ) ) {
        QTextStream stream( &file );

            stream << (header+structure+footer);
        file.close();
    }
  //********************************************************************************************
  return structure;
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
      areaAttribute *atts = new areaAttribute;
      db.insert(QString::number(nodeId,10),atts);
   }
}

void tree::reinitialize(){
  nodeId = 1;
  root->removeChildren();
  nodeList.clear();
  db.clear();
  areaAttribute *atts = new areaAttribute;
  db.insert(root->getLabel(),atts);

}

static int splitterIdNumber=0;



void tree::draw(treeNode *n, QWidget* parent, QWidget *form, QPtrList<QSplitter>* ls,QPtrList<SelectableArea>* stb){
   
    if(n->hasChildren()) {
        QString splitterId="splitter"+QString::number(splitterIdNumber,10);
        splitterIdNumber++;
	QSplitter *splitter = new QSplitter(parent,splitterId);
        ls->append(splitter);
	if(n->getSplit() == "v") splitter->setOrientation(QSplitter::Horizontal);
	if(n->getSplit() == "h") splitter->setOrientation(QSplitter::Vertical);
	n->firstChild();
	while(n->getCurrentChild()){
	    draw(n->getCurrentChild(),splitter,form,ls,stb);
	    n->nextChild();
	}
    }
    else{
	SelectableArea *te=new SelectableArea(parent);
        stb->append(te);
        te->setIdLabel( n->getLabel() );
        te->setMinimumSize(QSize(20,25));
        te->setSource(db.find(n->getLabel())->getSrc());
        QObject::connect(te, SIGNAL(Resized(QRect)), db.find(n->getLabel()), SLOT(setGeometry(QRect)));
	QObject::connect(te, SIGNAL(selected(QString)),form, SLOT(catchSelectedArea(QString)));
    }
}

void tree::removeChildNode(QString l,QString ll){
  findNode(l)->removeChildNode(ll);
  db.remove(l);
}

