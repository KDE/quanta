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

#include <qobjectlist.h>
#include <qsplitter.h>
//#include <kdebug.h>

#include "qextfileinfo.h"
#include "project.h"

static int cancelledPixels(int n){
   return (n-1)*6;
}

QMap<QString, QValueList< int > > SIZES;
static int splitterIdNumber = 0;


VisualFrameEditor::VisualFrameEditor(QWidget * parent, const char * name) : QHBox(parent,name){
  m_internalTree = new tree;
  m_internalTree->root()->atts()->setGeometry(QRect(0,0,510,422));
  m_firstInsertedSA = 0L;
  m_markupLanguage = HTML;
}

VisualFrameEditor::~VisualFrameEditor(){
  delete m_internalTree;
  delete m_firstInsertedSA;
}

void VisualFrameEditor::setGeometries(const QString &l){
  int cP = cancelledPixels(m_internalTree->findNode(l)->countChildren());
  QRect newGeometry(m_internalTree->findNode(l)->atts()->geometry());
  QPtrList<treeNode> list=m_internalTree->findNode(l)->childrenList();
  QPtrListIterator<treeNode> it( list );
  treeNode *node;
  if(m_internalTree->findNode(l)->splitType() == VERTICAL){
    int dummyDimension=m_internalTree->findNode(l)->atts()->geometry().width()-cP;
    while ( (node = it.current()) != 0 ) {
      ++it;
      newGeometry.setWidth( int(dummyDimension/m_internalTree->findNode(l)->countChildren()) );
      m_internalTree->findNode(node->label())->atts()->setGeometry(newGeometry);
    }
  }
  else
  if(m_internalTree->findNode(l)->splitType() == HORIZONTAL){
    int dummyDimension=m_internalTree->findNode(l)->atts()->geometry().height()-cP;
    while ( (node = it.current()) != 0 ) {
      ++it;
      newGeometry.setHeight( int(dummyDimension/m_internalTree->findNode(l)->countChildren()) );
      m_internalTree->findNode(node->label())->atts()->setGeometry(newGeometry);
    }
  }
}

void VisualFrameEditor::split(const QString &l, int n, SplitType type) {
  if(l==m_internalTree->root()->label()){
    m_internalTree->root()->setSplitType(type);
    for(int i = 1; i<=n; i++) m_internalTree->addChildNode(l);
    setGeometries(l);
  }
  else {
    QString parentLabel=m_internalTree->findNode(l)->parentLabel();
    SplitType parentSplit=m_internalTree->findNode(parentLabel)->splitType();
    if( parentSplit != type ) {
      m_internalTree->findNode(l)->setSplitType(type);
      for(int i = 1; i<=n; i++) m_internalTree->addChildNode(l);
      setGeometries(l);
    }
    else {
      for(int i = 1; i<=n; i++) m_internalTree->insertChildNode(l);
      m_internalTree->findNode(parentLabel)->removeChildNode(l,true);
      setGeometries(m_internalTree->findNode(parentLabel)->label());
    }
  }
}

void VisualFrameEditor::loadExistingStructure(const QStringList &list){
  if(!list.isEmpty()) {
    m_existingStructure = list;
    m_existingStructure.remove("</frameset>");//closure tag not needed
    buildInternalTree(m_internalTree->root()->label());
  }
}

QStringList VisualFrameEditor::convertAsterisks(const QString &s,int d){
  QStringList list=QStringList::split(",",s);
  int leftPercentage = 100;
  int leftPercentageDistributedAmongAsterisks=0;
  int weightAsteriskCounter=0;
   // This for is used to determine how much percentage must be assign to an asterisk
   // example cols="40%,5*,*"
   // then every asterisk must be assigned a percentage of 10% so the real percentage
   // notation is cols="40%,50%,10%"
  for ( QStringList::Iterator it = list.begin(); it != list.end(); ++it ) {
    if(!(*it).contains("%") && !(*it).contains("*")) leftPercentage -= ( (*it).toInt()*100 )/d;
    if((*it).contains("%")) leftPercentage -= (*it).section("%",0,0).toInt();
    if((*it).contains("*")) {
      int weight= (*it).section("*",0,0).toInt();
      if( weight==0 ) weight=1;
      weightAsteriskCounter += weight;
    }
  }

  if(weightAsteriskCounter!=0) leftPercentageDistributedAmongAsterisks = proxInt(double(leftPercentage)/double(weightAsteriskCounter));
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

void VisualFrameEditor::buildInternalTree(const QString &parent){
  QString line = m_existingStructure.first();
  if(line.contains("<frameset")) {
    if(line.contains("rows")) {
      split(parent,(line.contains(",")+1),HORIZONTAL);

      QRegExp pattern("rows\\s*=\"([\\s\\d%,\\*]*)\"");
      pattern.search(line);

      QRect dummy=m_internalTree->findNode(parent)->atts()->geometry();
      QStringList percentages = convertAsterisks(pattern.cap(1),dummy.height());

      int dummyDimension=dummy.height()-cancelledPixels(line.contains(",")+1);

      QPtrList<treeNode> list=m_internalTree->findNode(parent)->childrenList();
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
      split(parent,(line.contains(",")+1),VERTICAL);
      QRegExp pattern("cols\\s*=\"([\\s\\d%,\\*]*)\"");
      pattern.search(line);

      QRect dummy=m_internalTree->findNode(parent)->atts()->geometry();
      QStringList percentages = convertAsterisks(pattern.cap(1),dummy.width());

      int dummyDimension=dummy.width()-cancelledPixels(line.contains(",")+1);

      QPtrList<treeNode> list=m_internalTree->findNode(parent)->childrenList();
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

    m_existingStructure.pop_front();
    m_internalTree->findNode(parent)->firstChild();
    while(m_internalTree->findNode(parent)->currentChild()) {
      buildInternalTree(m_internalTree->findNode(parent)->currentChild()->label());
      m_internalTree->findNode(parent)->nextChild();
    }
  }
  else {
    QMap<QString,QString> attributeMap;
    if( line.contains( QRegExp("\\s+noresize") ) ) attributeMap["noresize"] = "noresize";
    else attributeMap["noresize"] = QString::null;

    QRegExp srcPattern("\\s+src\\s*=\\s*\"([%-\\w\\s\\./_\\+\\d]*)\"");  //search for files
    if(srcPattern.search(line) !=-1 ) {
      KURL pathToConvert, basePath;
      pathToConvert.setPath(srcPattern.cap(1));
      basePath.setPath( Project::ref()->projectBaseURL().path() );
      attributeMap["src"] = QExtFileInfo::toAbsolute( pathToConvert, basePath ).path();
      line.remove(srcPattern);//we don't need to operate on this anymore
    }

    QRegExp pattern("\\s+(\\w+\\s*=\\s*\"[\\w\\s\\./_\\+\\d]*\")");

    int pos = 0;
    while ( pos >= 0 ) {
      pos = pattern.search( line, pos );
      attributeMap[ pattern.cap(1).section( QRegExp("=\\s*\"") ,0,0) ] = pattern.cap(1).section(QRegExp("=\\s*\""),1,1).remove("\"");
      if ( pos >= 0 ) pos += pattern.matchedLength();
    }
    m_internalTree->findNode(parent)->atts()->setAllAttributes(attributeMap);
    m_existingStructure.pop_front();
  }
}

void VisualFrameEditor::paintEvent ( QPaintEvent * ){
  hide();
  delete m_firstInsertedSA;
  m_firstInsertedSA = 0L;

  QObjectList* splitterList = queryList("QSplitter");
  for (uint i = 0; i < splitterList->count(); i++) {
    QObject* o = splitterList->at(i);
    removeChild(o); //this will delete all childr of "o"
  }

  delete splitterList;
  splitterIdNumber = 0;
  drawGUI( m_internalTree->root(), this);
  show();
}

void VisualFrameEditor::removeNode(const QString &l){
  if( l == m_internalTree->root()->label() ) m_internalTree->reset();//trying to remove root node is equivalent to reinitialize
  else {
    QString parentLabel=m_internalTree->findNode(l)->parentLabel();
    if(m_internalTree->findNode(parentLabel)->countChildren()>=3)
      m_internalTree->removeChildNode(parentLabel,l,true);
    else {
      m_internalTree->removeChildNode(parentLabel,l,true);
      if( !m_internalTree->findNode(parentLabel)->firstChild()->hasChildren() ){ //final nodes
        QMap<QString,QString> map = m_internalTree->findNode(parentLabel)->firstChild()->atts()->attributeMap();
        m_internalTree->findNode(parentLabel)->removeChildren();
        m_internalTree->findNode(parentLabel)->atts()->setAllAttributes( map ) ;
        m_internalTree->findNode(parentLabel)->setSplitType(NONE);
      }
      else {
        QPtrList<treeNode> list = m_internalTree->findNode(parentLabel)->firstChild()->childrenList();
        if( parentLabel != m_internalTree->root()->label() ) {
          QString grandParentLabel = m_internalTree->findNode(parentLabel)->parentLabel();
          m_internalTree->removeChildNode( parentLabel,m_internalTree->findNode(parentLabel)->firstChild()->label(),false );
          m_internalTree->removeChildNode( grandParentLabel ,parentLabel, true );
          treeNode *node;
          for ( node = list.first(); node; node = list.next() ) {
            node->setParentLabel(grandParentLabel);
            m_internalTree->findNode(grandParentLabel)->addChildNode(node);
          }
        }
        else {
          m_internalTree->findNode(parentLabel)->setSplitType( m_internalTree->findNode(parentLabel)->firstChild()->splitType() );
          m_internalTree->removeChildNode( parentLabel,m_internalTree->findNode(parentLabel)->firstChild()->label(),false );
          treeNode *node;
          for ( node = list.first(); node; node = list.next() ) {
            node->setParentLabel(parentLabel);
            m_internalTree->findNode(parentLabel)->addChildNode(node);
          }
        }
      }
    }
  }
}

void VisualFrameEditor::drawGUI(treeNode *n, QWidget* parent){
  if(n->hasChildren()) {
    QString splitterName("splitter"+QString::number(++splitterIdNumber,10));
    QSplitter *splitter = new QSplitter(parent,splitterName);
    if(SIZES.contains(splitterName)) splitter->setSizes( SIZES[splitterName] );
    switch( n->splitType() ){
      case VERTICAL : splitter->setOrientation(QSplitter::Horizontal);break;
      case HORIZONTAL : splitter->setOrientation(QSplitter::Vertical);break;
      default:break;
    }
    n->firstChild();
    while(n->currentChild()) {
      drawGUI(n->currentChild(),splitter);
      n->nextChild();
    }
  }
  else {
    SelectableArea *sa=new SelectableArea(parent,n->label());
    if(parent->isA("QSplitter")) dynamic_cast<QSplitter *>(parent)->setResizeMode(sa->view(),QSplitter::KeepSize );
    else
    if(!m_firstInsertedSA) m_firstInsertedSA = sa;
    sa->view()->setGeometry(n->atts()->geometry());
    sa->setIdLabel( n->label() );
    sa->setSource( n->atts()->src() );
    connect(sa, SIGNAL(Resized(QRect)), m_internalTree->findNode(sa->idLabel())->atts(), SLOT(setGeometry(QRect)));
    connect(sa, SIGNAL(selected(const QString &)),this, SIGNAL(areaSelected(const QString &)));
  }
}

QString VisualFrameEditor::createFrameTag(areaAttribute *a){
  QString Src(a->attributeValue("src")),
               Longdesc(a->attributeValue("longdesc")),
               Name(a->attributeValue("name")),
               Scrolling(a->attributeValue("scrolling")),
               Id(a->attributeValue("id")),
               Style(a->attributeValue("style")),
               Title(a->attributeValue("title")),
               Class(a->attributeValue("class")),
               Noresize(a->attributeValue("noresize")),
               Frameborder(a->attributeValue("frameborder")),
               Marginwidth(a->attributeValue("marginwidth")),
               Marginheight(a->attributeValue("marginheight"));

  QString tagBegin="<frame",
          tagEnd,
          tagMiddle;

  if( !Src.isEmpty() ) {
    KURL base;
    base.setPath( Project::ref()->projectBaseURL().path() );
    KURL u;
    u.setPath(Src);
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
  if( Frameborder=="0" )     tagMiddle+=(" frameborder=\""+Frameborder+"\"");
  if( Marginwidth!="0" && !Marginwidth.isEmpty() )     tagMiddle+=(" marginwidth=\""+Marginwidth+"\"");
  if( Marginheight!="0" && !Marginheight.isEmpty())    tagMiddle+=(" marginheight=\""+Marginheight+"\"");

  switch(m_markupLanguage){
    case HTML:  if( Noresize=="noresize" ) tagMiddle+=(" "+Noresize);
                          tagEnd=">\n";break;
    case XHTML:  if( Noresize=="noresize" ) tagMiddle+=(" noresize=\""+Noresize+"\"");
                            tagEnd="/>\n";break;
    default:;
  }

  return tagBegin+tagMiddle+tagEnd;
}

QString VisualFrameEditor::RCvalue(treeNode *n) {
  QString s;
  QMap<int,int> dimMap;
  double percentage = 100.0;
  int remainingPercentage=100;
  int child_number = n->countChildren();
  int lostPixels = (6*(child_number-1)); // 6 pixels are lost every time a splitter is drawn

  switch( n->splitType() ) {
    case VERTICAL: percentage/=n->atts()->geometry().width();
                                 for(int i=1;i<=child_number;++i) dimMap[i]=n->childrenList().at(i-1)->atts()->geometry().width();
                                 break;
    case HORIZONTAL: percentage/=n->atts()->geometry().height();
                                      for(int i=1;i<=child_number;++i) dimMap[i]=n->childrenList().at(i-1)->atts()->geometry().height();
                                      break;
    default:break;
  }

  while( lostPixels > 0) {
    for(int i=1;i<=child_number;++i){
      dimMap[i]+=1;
      lostPixels--;
      if(lostPixels == 0) break;
    }
  }

  for(int i=1;i<=child_number-1;++i) {
    remainingPercentage-=proxInt(dimMap[i]*percentage);
    s+=QString::number(proxInt(dimMap[i]*percentage),10);
    s+="%,";
  }

  return s+=(QString::number(remainingPercentage,10)+"%");
}

static QStringList nonFormattedStructure;

void VisualFrameEditor::createStructure(treeNode* n){
  if(n==m_internalTree->root() && !n->hasChildren()) return;
  if(n->hasChildren()) {
    switch( n->splitType() ){
      case VERTICAL: nonFormattedStructure.append("<frameset cols=\""+RCvalue(n)+"\">\n");break;
      case HORIZONTAL: nonFormattedStructure.append("<frameset rows=\""+RCvalue(n)+"\">\n");break;
      default:break;
    }
    n->firstChild();
    while(n->currentChild()){
      createStructure(n->currentChild());
      n->nextChild();
    }
    nonFormattedStructure.append("</frameset>\n");
  }
  else nonFormattedStructure.append(createFrameTag(n->atts()));
}

QString VisualFrameEditor::formatStructure(){
  QString s;
  int tabNum = 0;
  for ( QStringList::Iterator it = nonFormattedStructure.begin(); it != nonFormattedStructure.end(); ++it ) {
    if((*it).contains("<frameset")) tabNum++;
    else
    if((*it).contains("</frameset>")) {
      tabNum--;
      s.truncate(s.length()-1);
    }
    s+=*it;
    for(int i=1;i<=tabNum;i++) s+='\t';
  }
  nonFormattedStructure.clear();
  return s;
}

QString VisualFrameEditor::framesetStructure() {
  m_internalTree->refreshGeometries(m_internalTree->root());
  createStructure(m_internalTree->root());
  return formatStructure();
}

void VisualFrameEditor::setMarkupLanguage(const QString& s){
  if( s.contains("xhtml",false)!=0 ) m_markupLanguage = XHTML;
  else
  if( s.contains("html",false)!=0 ) m_markupLanguage = HTML;
}

#include "visualframeeditor.moc"
