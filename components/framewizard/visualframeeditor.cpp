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

#include <qstringlist.h>
#include <qobjectlist.h>

#include <kdebug.h>
#include <khtmlview.h>

#include "fwglobal.h"
#include "qextfileinfo.h"


static int cancelledPixels(int n){
   return (n-1)*6;
}

QMap<QString, QValueList< int > > SIZES;
static int splitterIdNumber = 0;


VisualFrameEditor::VisualFrameEditor(QWidget * parent, const char * name) : QHBox(parent,name)
{
  t = new tree;
  t->root()->atts()->setGeometry(QRect(0,0,510,422));
  firstInsertedSA = 0L;
}

VisualFrameEditor::~VisualFrameEditor(){
  delete t;
  if(firstInsertedSA) delete firstInsertedSA;
  firstInsertedSA=0L;
}

void VisualFrameEditor::setGeometries(QString l){
   int cP = cancelledPixels(t->findNode(l)->countChildren());
   QRect newGeometry(t->findNode(l)->atts()->geometry());
   QPtrList<treeNode> list=t->findNode(l)->getChildrenList();
   QPtrListIterator<treeNode> it( list );
   treeNode *node;
  if(t->findNode(l)->getSplit() == "v"){
      int dummyDimension=t->findNode(l)->atts()->geometry().width()-cP;
      while ( (node = it.current()) != 0 ) {
         ++it;
         newGeometry.setWidth( int(dummyDimension/t->findNode(l)->countChildren()) );
         t->findNode(node->getLabel())->atts()->setGeometry(newGeometry);
         }
    }
    else
   if(t->findNode(l)->getSplit() == "h"){
      int dummyDimension=t->findNode(l)->atts()->geometry().height()-cP;
      while ( (node = it.current()) != 0 ) {
         ++it;
         newGeometry.setHeight( int(dummyDimension/t->findNode(l)->countChildren()) );
         t->findNode(node->getLabel())->atts()->setGeometry(newGeometry);
      }
    }
}


void VisualFrameEditor::split(QString l, int n, QString type) {
   if(l==t->root()->getLabel()){
    t->root()->setSplit(type);
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
               t->findNode(parentLabel)->removeChildNode(l,true);
               setGeometries(t->findNode(parentLabel)->getLabel());
         }
     }
}


void VisualFrameEditor::loadExistingStructure(QStringList list,QString path){
  if(!list.isEmpty())
   {
    existingStructure = list;
    existingStructure.remove("</frameset>");//closure tag not needed
    build(t->root()->getLabel(),path);
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
        }

        if((*it).contains("%")){
            leftPercentage -= (*it).section("%",0,0).toInt();
        }

        if((*it).contains("*")){
             int weight= (*it).section("*",0,0).toInt();
             if(weight==0) weight=1;
             weightAsteriskCounter += weight;
        }
    }
   if(weightAsteriskCounter!=0)
     leftPercentageDistributedAmongAsterisks= proxInt(double(leftPercentage)/double(weightAsteriskCounter));
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
             leftPercentage-=newPercentage;
               }
        }
    return list;
}

void VisualFrameEditor::build(QString parent, QString path){
  QString line = existingStructure.first();
  if(line.contains("<frameset")) {
    if(line.contains("rows")) {
      split(parent,(line.contains(",")+1),"h");
      
      QRegExp pattern("rows\\s*=\"([\\s\\d%,\\*]*)\"");
      pattern.search(line);

      QRect dummy=t->findNode(parent)->atts()->geometry();
      QStringList percentages = convertAsterisks(pattern.cap(1),dummy.height());

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
        node->atts()->setGeometry(newGeometry);
        percentages.pop_front();
      }
    }
    else
    if(line.contains("cols")) {
      split(parent,(line.contains(",")+1),"v");
      QRegExp pattern("cols\\s*=\"([\\s\\d%,\\*]*)\"");
      pattern.search(line);

      QRect dummy=t->findNode(parent)->atts()->geometry();
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
        node->atts()->setGeometry(newGeometry);
        percentages.pop_front();
      }
    }

    existingStructure.pop_front();
    t->findNode(parent)->firstChild();
    while(t->findNode(parent)->currentChild()) {
      build(t->findNode(parent)->currentChild()->getLabel(),path);
      t->findNode(parent)->nextChild();
    }
  }
  else {
    QMap<QString,QString> attributeMap;
    if( line.contains( QRegExp("\\s+noresize\\s+") ) )
      attributeMap["noresize"] = "noresize";
    else
      attributeMap["noresize"] = QString::null;

    //QRegExp srcPattern("\\s+src\\s*=\\s*\"([\\w\\s\\./_\\+\\d]*)\"");  //search for files
    QRegExp srcPattern("\\s+src\\s*=\\s*\"([%-\\w\\s\\./_\\+\\d]*)\"");  //search for files
    if(srcPattern.search(line) !=-1 ) {      
      KURL pathToConvert, basePath;
      pathToConvert.setPath(srcPattern.cap(1));
      basePath.setPath(path);
      attributeMap["src"] = QExtFileInfo::toAbsolute( pathToConvert, basePath ).url();
      line.remove(srcPattern);//we don't need to operate on this anymore
    }

    QRegExp pattern("\\s+(\\w+\\s*=\\s*\"[\\w\\s\\./_\\+\\d]*\")");

    int pos = 0;
    while ( pos >= 0 ) {
      pos = pattern.search( line, pos );
      attributeMap[ pattern.cap(1).section( QRegExp("=\\s*\"") ,0,0) ] = pattern.cap(1).section(QRegExp("=\\s*\""),1,1).remove("\"");
      if ( pos >= 0 ) 
        pos += pattern.matchedLength();
    }
    t->findNode(parent)->setAtts(attributeMap);
    existingStructure.pop_front();
  }
}

void VisualFrameEditor::paintEvent ( QPaintEvent * ){
  hide();
  if(firstInsertedSA) {
    delete firstInsertedSA;
    firstInsertedSA = 0L;
  }
  
  QObjectList* splitterList = queryList("QSplitter");
  for (uint i = 0; i < splitterList->count(); i++) {
    QObject* o = splitterList->at(i);
    removeChild(o); //this will delete all childr of "o"
  }

  delete splitterList;
  splitterIdNumber = 0;
  draw2( t->root(), this);
  show();
}

void VisualFrameEditor::removeNode(QString l){
  if( l == t->root()->getLabel() ) t->reinitialize();//trying to remove root node is equivalent to reinitialize
  else {
    QString parentLabel=t->findNode(l)->getParentLabel();
    if(t->findNode(parentLabel)->countChildren()>=3) 
      t->removeChildNode(parentLabel,l,true);    
    else {
      t->removeChildNode(parentLabel,l,true);
      if( !t->findNode(parentLabel)->firstChild()->hasChildren() ){ //final nodes
        areaAttribute *tmp = new areaAttribute(t->findNode(parentLabel)->firstChild()->atts());
        t->findNode(parentLabel)->removeChildren();
        t->findNode(parentLabel)->setAtts( tmp );
        t->findNode(parentLabel)->setSplit("n");
      }
      else {
        QPtrList<treeNode> list = t->findNode(parentLabel)->firstChild()->getChildrenList();
        if( parentLabel != t->root()->getLabel() ) {
          QString grandParentLabel = t->findNode(parentLabel)->getParentLabel();
          t->removeChildNode( parentLabel,t->findNode(parentLabel)->firstChild()->getLabel(),false );
          t->removeChildNode( grandParentLabel ,parentLabel, true );
          treeNode *node;
          for ( node = list.first(); node; node = list.next() ) {
            node->setParentLabel(grandParentLabel);
            t->findNode(grandParentLabel)->addChildNode(node);
          }
        }
        else {
          t->findNode(parentLabel)->setSplit( t->findNode(parentLabel)->firstChild()->getSplit() );
          t->removeChildNode( parentLabel,t->findNode(parentLabel)->firstChild()->getLabel(),false );
          treeNode *node;
          for ( node = list.first(); node; node = list.next() ) {
            node->setParentLabel(parentLabel);
            t->findNode(parentLabel)->addChildNode(node);
          }
        }
      }
    }
  }
}

void VisualFrameEditor::draw2(treeNode *n, QWidget* parent){
  if(n->hasChildren()) {
    QString splitterName("splitter"+QString::number(++splitterIdNumber,10));
    QSplitter *splitter = new QSplitter(parent,splitterName);
    if(SIZES.contains(splitterName)) splitter->setSizes( SIZES[splitterName] );
    if(n->getSplit() == "v") splitter->setOrientation(QSplitter::Horizontal);
    if(n->getSplit() == "h") splitter->setOrientation(QSplitter::Vertical);
    n->firstChild();
    while(n->currentChild()) {
      draw2(n->currentChild(),splitter);
      n->nextChild();
    }
  }
  else {
    SelectableArea *sa=new SelectableArea(parent,n->getLabel());
    if(parent->isA("QSplitter"))
      dynamic_cast<QSplitter *>(parent)->setResizeMode(sa->view(),QSplitter::KeepSize );
    else
      if(!firstInsertedSA)  
        firstInsertedSA = sa;
    sa->view()->setGeometry(n->atts()->geometry());
    sa->setIdLabel( n->getLabel() );
    sa->setSource( n->atts()->src() );
    QObject::connect(sa, SIGNAL(Resized(QRect)), t->findNode(sa->idLabel())->atts(), SLOT(setGeometry(QRect)));
    QObject::connect(sa, SIGNAL(selected(QString)),this, SIGNAL(areaSelected(QString)));
  }
}

void VisualFrameEditor::reset(){
  t->reinitialize();
}

#include "visualframeeditor.moc"
