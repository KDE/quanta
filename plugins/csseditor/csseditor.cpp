/***************************************************************************
                          csseditor.cpp  -  description
                             -------------------
    begin                : mer lug 23 11:20:17 CEST 2003
    copyright            : (C) |YEAR| by si2003    email                : simone@localhost
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "csseditor.h"
#include <qlistview.h>
#include <qlayout.h>

#include <qtabwidget.h>
#include <qfile.h>
#include <qpushbutton.h>
#include <qcombobox.h>
#include <qcheckbox.h>
#include <qlineedit.h>
#include <qtextedit.h>
#include <qhbox.h>


#include <qregexp.h>
#include <qtextstream.h>
#include <qmap.h>

#include <klocale.h>
#include <khtml_part.h>
#include <khtmlview.h>
#include <kstandarddirs.h>
#include <ktempfile.h>

#include "propertysetter.h"
#include "qmyhighlighter.h"
#include "resource.h"
#include "cssshpropertyparser.h"
#include "shorthandformer.h"
   
myCheckListItem::myCheckListItem(QListView * parent, const QString & text):QCheckListItem(parent, text, QCheckListItem::CheckBox),checkedChildren(0){
  sig = new QSignal; 
}

myCheckListItem::myCheckListItem(QCheckListItem * parent, const QString & text):QCheckListItem(parent, text, QCheckListItem::CheckBox),checkedChildren(0){
  sig = new QSignal;
}
myCheckListItem::~myCheckListItem(){
  delete sig;
}

void myCheckListItem::connect( QObject *receiver, const char *member ){
  sig->connect( receiver, member );
}

void myCheckListItem::activate(){
  if(isOn()) QCheckListItem::activate();
}

void myCheckListItem::addCheckedChild() {
    checkedChildren++;
}

void myCheckListItem::stateChange (bool b){

   if(!b) {
     if(childCount()) {
       QListViewItem * child = firstChild();
       while( child ) {
         sig->setValue(QVariant(child->text(0)));
         sig->activate();
         static_cast<myCheckListItem*>(child)->setOn(false);
         child = child->nextSibling();
       }
       checkedChildren = 0;
     }
     else {
       if(parent()){
         myCheckListItem *p = static_cast<myCheckListItem*>(parent());
         while( p ) {
           if( p->checkedChildren != 1) {
             p->checkedChildren--;
             sig->setValue(QVariant(text(0)));
             sig->activate();
             break;
           }
           else {
             sig->setValue(QVariant(text(0)));
             sig->activate();
             p->setOn(false);
           }
           p = static_cast<myCheckListItem*>(p->parent());
         }
       }
       else {
         sig->setValue(QVariant(text(0)));
         sig->activate();
       }
     }
   }
    else {
      if(parent())
        static_cast<myCheckListItem*>(parent())->addCheckedChild(); 
    }  
} 

void CSSEditor::appendSub(QDomNodeList l, myCheckListItem *cli){
  unsigned int i;
  for(i=0;i<l.length();i++) {
    myCheckListItem *item;
    if(l.item(i).toElement().attribute("hasSub") == "no") {
      item = new myCheckListItem(cli,l.item(i).toElement().tagName());
      item->connect(this,SLOT(removeProperty(const QVariant&)));
    }
    else
    if(l.item(i).toElement().attribute("hasSub") == "yes") {
      item = new myCheckListItem(cli,l.item(i).toElement().tagName());
      item->connect(this,SLOT(removeProperty(const QVariant&)));
      appendSub(l.item(i).childNodes(),item);
    }
  }
}

void CSSEditor::buildListView(QDomNodeList l, QListView *lv){
  unsigned int i;
  for(i=0;i<l.length();i++) {
    myCheckListItem *item;
    if(l.item(i).toElement().attribute("hasSub") == "no"){
      item = new myCheckListItem(lv,l.item(i).toElement().tagName());
      item->connect(this,SLOT(removeProperty(const QVariant&)));
    }
    else
    if(l.item(i).toElement().attribute("hasSub") == "yes") {
      item = new myCheckListItem(lv,l.item(i).toElement().tagName());
      item->connect(this,SLOT(removeProperty(const QVariant&)));
      QDomNodeList listSub = l.item(i).childNodes();
      appendSub(listSub,item);
    }
  }
} 


void CSSEditor::activatePreview() { 
previewer->openURL(testFile->name()); 
}

void CSSEditor::setCurrentPropOn(const QString& s){

if( (currentProp = static_cast<myCheckListItem*>(lvVisual->findItem( s,0 )) )) 
        currentProp->setOn(true);
      else  
      if( (currentProp = static_cast<myCheckListItem*>(lvAll->findItem( s,0 )) )) 
        currentProp->setOn(true); 
      else  
      if( (currentProp = static_cast<myCheckListItem*>(lvAural->findItem( s,0 )) )) 
        currentProp->setOn(true);   
      else  
      if( (currentProp = static_cast<myCheckListItem*>(lvInteractive->findItem( s,0 )) )) 
        currentProp->setOn(true); 
      else  
      if( (currentProp = static_cast<myCheckListItem*>(lvPaged->findItem( s,0 )) )) 
        currentProp->setOn(true); 
        
      if(currentProp && currentProp->depth() ) {
          myCheckListItem *p = static_cast<myCheckListItem*>(currentProp->parent());
          while(p) {
            p->setOn(true);
            p=static_cast<myCheckListItem*>(p->parent());
          }
        }  

}

 void CSSEditor::addAndSetPropertyOn(const QString& property, const QString& value){
   addProperty(property,value);
   setCurrentPropOn(property);
 }


void CSSEditor::initialize()
{   

  QString configFile = locate("appdata", "csseditor/config.xml");    

  myhi = new QMyHighlighter(display);

  display->setReadOnly(true);
  
  QBoxLayout *fPreviewLayout = new QBoxLayout(fPreview,QBoxLayout::LeftToRight);
  previewer=new KHTMLPart(fPreview);
  fPreviewLayout->addWidget(previewer->view());
  
  QFile file( configFile );
  if ( !file.open( IO_ReadOnly ) ) {
    return;
  }
  if ( !doc.setContent( &file ) ) { 
    file.close();
    return;
  }
  file.close();   

  QDomElement docElem = doc.documentElement();

  QDomNode n = docElem.firstChild();
  while( !n.isNull() ) {
    QDomElement e = n.toElement(); 
    if( !e.isNull() ) {
      if( e.attribute("type") == "visual") {
        lvVisual->setRootIsDecorated(true);          
        buildListView(e.childNodes(), lvVisual);            
      }
      if( e.attribute("type") == "paged") {
        lvPaged->setRootIsDecorated(true);
        buildListView(e.childNodes(), lvPaged);
      }
      if( e.attribute("type") == "interactive") {
        lvInteractive->setRootIsDecorated(true);
        buildListView(e.childNodes(), lvInteractive);
      }
      if( e.attribute("type") == "aural") {
        lvAural->setRootIsDecorated(true);
        buildListView(e.childNodes(), lvAural);
      }
      if( e.attribute("type") == "all"){
        lvAll->setRootIsDecorated(true);
        buildListView(e.childNodes(), lvAll);
      }
    }
    n = n.nextSibling();
  } // end while  

  Connect();
  
  testFile = new KTempFile(tmpDir,".html");
  testFile->setAutoDelete(true);
  testFile->textStream()->setEncoding(QTextStream::UnicodeUTF8);
  
  previewer->openURL(testFile->name());

  QBoxLayout *fEditingLayout = new QBoxLayout(fEditing,QBoxLayout::LeftToRight);
  
  ps = new propertySetter(fEditing);
  fEditingLayout->addWidget(ps);
  connect(ps, SIGNAL(valueChanged(const QString&)), this, SLOT(checkProperty(const QString&)));
  
  QStringList props;
  QString temp;
  bool normalMode = true;
  
  if( !selectorName.isEmpty() ){ //the cssselector has been called
     initialProperties = initialProperties.stripWhiteSpace();
     props=QStringList::split(";",initialProperties);
     temp=(selectorName + " {\n\t");
     *(testFile->textStream()) << ( Header + selectorName + " { " + initialProperties + " } "+ Selectors + Footer);
  }  
  
  else {
    InlineStyleContent = InlineStyleContent.stripWhiteSpace();
    normalMode = false;
    props=QStringList::split(";",InlineStyleContent);
    temp="\n\t";
    *(testFile->textStream()) << initialPreviewText;
  }
  
  SHFormList.append("cue"); 
  SHFormList.append("pause");
  SHFormList.append("font");
  SHFormList.append("background");
  SHFormList.append("border");
  SHFormList.append("border-top");
  SHFormList.append("border-bottom");
  SHFormList.append("border-left");
  SHFormList.append("border-right");
  SHFormList.append("border-color");
  SHFormList.append("border-style");
  SHFormList.append("border-width");
  SHFormList.append("outline");
  SHFormList.append("list-style");
   
  for ( QStringList::Iterator it = props.begin(); it != props.end(); ++it ) {
       const QString propertyName((*it).section(":",0,0).stripWhiteSpace());
       const QString propertyValue((*it).section(":",1,1));
      
      if(SHFormList.contains(propertyName)==0) {
         temp+=( propertyName + " : " + propertyValue +";\n\t");
         addAndSetPropertyOn(propertyName,propertyValue);
      }       
    else{
      ShorthandFormer decompressor;
      QMap<QString,QString> expandedProperties = decompressor.expand(propertyName, propertyValue );
      QMap<QString,QString>::Iterator it;
      for ( it = expandedProperties.begin(); it != expandedProperties.end(); ++it ) {
        addAndSetPropertyOn( it.key(), it.data() );
        temp += ( it.key() + " : " + it.data() + ";\n\t");      
      }
    }    
    }  
    temp.truncate(temp.length()-1);
    if(normalMode)//normal mode editing
      temp+="}";
    display->setText(temp);
    testFile->close();   
}

CSSEditor::CSSEditor(QWidget *parent, const char *name) : CSSEditorS(parent, name){
 SHckb->hide();
 SHckb->setChecked(false);
}

CSSEditor::CSSEditor(QListViewItem *i, QWidget *parent, const char *name) : CSSEditorS(parent, name){
  selectorName = i->text(0);
  initialProperties = i->text(1);
  SHckb->hide();
  SHckb->setChecked(false);
}

CSSEditor::CSSEditor( QString s, QWidget* parent, const char *name) : CSSEditorS(parent, name){
  selectorName = s;
  SHckb->hide();
  SHckb->setChecked(false);
}

CSSEditor::~CSSEditor() { 
    delete myhi;
    delete ps;
    delete previewer;
    if(testFile){
      delete testFile;
      testFile=0;
    }
    
}

void CSSEditor::setMiniEditors(QListViewItem* i)
{
  ps->reset();
  
  if(i->childCount()==0) {
   
    currentProp = static_cast<myCheckListItem*>(i);

    QDomElement type = doc.elementsByTagName(i->text(0)).item(0).firstChild().toElement();

    if(type.tagName() == "selectable") {

      QDomNodeList values = type.childNodes();
      unsigned int i;
      ps->setComboBox();
      for(i=0; i<values.length(); i++) {
        ps->getComboBox()->insertItem(values.item(i).toElement().attribute("name"));
      }
    }
    else
    if(type.tagName() == "mixed") {

      QDomNodeList subTypes = type.childNodes();
      unsigned int i;
      for(i=0; i<subTypes.length(); i++) {
        QDomElement curr =subTypes.item(i).toElement();
        if(curr.tagName()=="selectable") {
          QDomNodeList values = curr.childNodes();
          unsigned int k;
          ps->setComboBox();
          for(k=0; k<values.length(); k++) {
            ps->getComboBox()->insertItem(values.item(k).toElement().attribute("name"));
          }
        }
        
        if(curr.tagName()=="numeric") {

          QDomNodeList values = curr.childNodes();
          unsigned int k;
          for(k=0; k<values.length(); k++) {

            QString typeName(values.item(k).toElement().attribute("type"));
            
            if( typeName == "spinbox") {
              ps->setSpinBox(values.item(k).toElement().attribute("minValue"),
              values.item(k).toElement().attribute("maxValue"),
              values.item(k).toElement().attribute("suffix"));
            }
                
            if( typeName == "edit") {
              ps->getComboBox()->setEditable(true);
              ps->getComboBox()->lineEdit()->clear();
            }
                
            if( typeName == "length") {
              ps->setLengthEditor();
            }
                
            if( typeName == "percentage") {
              ps->setPercentageEditor();
            }
                
            if( typeName == "doubleLength") {
              ps->setDoubleLengthEditor();
            }
                
            if( typeName == "doublePercentage") {
              ps->setDoublePercentageEditor();
            }
                
            if( typeName == "integer") {
              ps->setSpinBox();
            }
                
            if( typeName == "number") {
              ps->setLineEdit();
            }
                
            if( typeName == "frequency") {
              ps->setFrequencyEditor();
            }
                
            if( typeName == "time") {
              ps->setTimeEditor();
            }
                
            if( typeName == "angle") {
              ps->setAngleEditor();
            }
                
          }
        }  
        else
        if(curr.tagName()=="string") {

          QDomNodeList values = curr.childNodes();
          unsigned int k;
          for(k=0; k<values.length(); k++) {

            QString typeName(values.item(k).toElement().attribute("type"));
            if( typeName == "uri") {
              ps->setURIEditor();
              if(values.item(k).toElement().attribute("mode") == "multi")
                ps->getURIEditor()->setMode(Multi);
            }
                
            if( typeName == "edit") {
              ps->getComboBox()->setEditable(true);
              ps->getComboBox()->lineEdit()->clear();
            }
            
            if( typeName == "color") {
              ps->setColorRequester();
            }
              
          }




          if(curr.firstChild().toElement().attribute("type") == "edit"){
            ps->getComboBox()->setEditable(true);
            ps->getComboBox()->lineEdit()->clear();
              }
            }
            else
          if(curr.tagName()=="doubleSelectable")
            {
              ps->setDoubleComboBoxEditor();
              unsigned int k;
              QDomNodeList sxValues = curr.firstChild().childNodes();
              for(k=0; k<sxValues.length(); k++){
                ps->getDoubleComboBoxEditor()->getCbSx()->insertItem(sxValues.item(k).toElement().attribute("name"));
              }

              QDomNodeList dxValues = curr.lastChild().childNodes();
              for(k=0; k<dxValues.length(); k++){
                ps->getDoubleComboBoxEditor()->getCbDx()->insertItem(dxValues.item(k).toElement().attribute("name"));
              } 
              
            }
        }
       ps->addButton();
       
     }// end "mixed" if
     ps->Show();

    }
}

void CSSEditor::checkProperty(const QString& v){
 if(!currentProp->isOn()) {
     currentProp->setOn(true);
   } 
 if( currentProp->depth() ){
   myCheckListItem *p = static_cast<myCheckListItem*>(currentProp->parent());
   while(p){
     p->setOn(true);
     p=static_cast<myCheckListItem*>(p->parent());
   }
 }
   addProperty(currentProp->text(0),v);
   emit signalUpdatePreview(); 
}

void CSSEditor::Connect(){
  connect(this, SIGNAL(signalUpdatePreview()), this, SLOT(updatePreview()));
  connect(lvVisual,SIGNAL(selectionChanged ( QListViewItem *  )),this,SLOT(setMiniEditors ( QListViewItem *  )));
  connect(lvAll,SIGNAL( selectionChanged( QListViewItem *  )),this,SLOT(setMiniEditors ( QListViewItem *  )));
  connect(lvAural,SIGNAL( selectionChanged( QListViewItem *  )),this,SLOT(setMiniEditors ( QListViewItem *  )));
  connect(lvInteractive,SIGNAL( selectionChanged( QListViewItem *  )),this,SLOT(setMiniEditors ( QListViewItem *  )));
  connect(lvPaged,SIGNAL( selectionChanged( QListViewItem *  )),this,SLOT(setMiniEditors ( QListViewItem *  )));

}

void CSSEditor::removeProperty(const QVariant& v)
{
  properties.remove(v.toString());
  updatePreview();
}

QString CSSEditor::generateProperties(){
  QString props;
  QMap<QString,QString>::Iterator it;
  if(!SHckb->isChecked()) {
    for ( it = properties.begin(); it != properties.end(); ++it ) {
       props+=( it.key() + " : " + it.data() + "; " );
    }
    props.truncate(props.length()-1);//the last white space creates some problem: better remove it
    return props;  
  }  
  else {
    ShorthandFormer sf(properties);
    return sf.compress();  
  }  
}

void CSSEditor::updatePreview()
{
  QString toDisplay;
  
  delete testFile;
  testFile=0;
  //TODO : substitue the extension with the proper one
  testFile = new KTempFile(tmpDir,".html");
  testFile->setAutoDelete(true);
  testFile->textStream()->setEncoding(QTextStream::UnicodeUTF8);
     
  if(!selectorName.isEmpty()){// we're working on <style></style> block
      toDisplay= ( selectorName +" {\n\t" );
      *(testFile->textStream()) << Header << selectorName << " { \n ";
      
      QMap<QString,QString>::Iterator it;
      for ( it = properties.begin(); it != properties.end(); ++it ) {
        QString s( it.key() + " : " + it.data() + ";");
        *(testFile->textStream()) << s;
        toDisplay += (s + "\n\t");
      }
      *(testFile->textStream()) << "}" << Selectors<<Footer;
    }
  
    else {
      toDisplay = "\n\t";
      *(testFile->textStream()) << Header <<" style=\"";
      QMap<QString,QString>::Iterator it;
      for ( it = properties.begin(); it != properties.end(); ++it ) {
        QString s( it.key() + " : " + it.data() + ";");
        *(testFile->textStream()) << s;
        toDisplay += (s + "\n\t");
      }
      *(testFile->textStream()) << "\"" << Footer;
    }
    display->setText(toDisplay);
    testFile->close();
    activatePreview();
}

void CSSEditor::setInlineStyleContent( const QString& s) {  
  InlineStyleContent = s; 
}

  

#include "csseditor.moc"
