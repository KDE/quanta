/***************************************************************************
                          csseditor.cpp  -  description
                             -------------------
    begin                : mer lug 23 11:20:17 CEST 2003
    copyright            : (C) |YEAR| by si2003    email                : gulmini.luciano@student.unife.it
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
#include "resource.h"
#include "quanta.h"

   
myCheckListItem::myCheckListItem(QListView * parent, const QString & text):QCheckListItem(parent, text, QCheckListItem::CheckBox),m_checkedChildren(0){
  m_sig = new QSignal; 
}

myCheckListItem::myCheckListItem(QCheckListItem * parent, const QString & text):QCheckListItem(parent, text, QCheckListItem::CheckBox),m_checkedChildren(0){
  m_sig = new QSignal;
}
myCheckListItem::~myCheckListItem(){
  delete m_sig;
}

void myCheckListItem::connect( QObject *receiver, const char *member ){
  m_sig->connect( receiver, member );
}

void myCheckListItem::activate(){
  if(isOn()) QCheckListItem::activate();
}

void myCheckListItem::addCheckedChild() {
  m_checkedChildren++;
}

void myCheckListItem::stateChange (bool b){

   if(!b) {
     if(childCount()) {
       QListViewItem * child = firstChild();
       while( child ) {
         m_sig->setValue(QVariant(child->text(0)));
         m_sig->activate();
         static_cast<myCheckListItem*>(child)->setOn(false);
         child = child->nextSibling();
       }
       m_checkedChildren = 0;
     }
     else {
       if(parent()){
         myCheckListItem *p = static_cast<myCheckListItem*>(parent());
         while( p ) {
           if( p->m_checkedChildren != 1) {
             p->m_checkedChildren--;
             m_sig->setValue(QVariant(text(0)));
             m_sig->activate();
             break;
           }
           else {
             m_sig->setValue(QVariant(text(0)));
             m_sig->activate();
             p->setOn(false);
           }
           p = static_cast<myCheckListItem*>(p->parent());
         }
       }
       else {
         m_sig->setValue(QVariant(text(0)));
         m_sig->activate();
       }
     }
   }
    else {
      if(parent()) static_cast<myCheckListItem*>(parent())->addCheckedChild(); 
    }  
} 

void CSSEditor::appendSub(QDomNodeList l, myCheckListItem *cli){
  unsigned int i;
  for(i=0;i<l.length();i++) {
    myCheckListItem *item  = new myCheckListItem(cli,l.item(i).toElement().tagName());
    item->connect(this,SLOT(removeProperty(const QVariant&)));
    if(l.item(i).toElement().attribute("hasSub") == "yes") 
      appendSub(l.item(i).childNodes(),item);
  }
}

void CSSEditor::buildListView(QDomNodeList l, QListView *lv){
  unsigned int i;
  for(i=0;i<l.length();i++) {
    myCheckListItem *item = new myCheckListItem(lv,l.item(i).toElement().tagName());
    item->connect(this,SLOT(removeProperty(const QVariant&)));
    if(l.item(i).toElement().attribute("hasSub") == "yes") {
      QDomNodeList listSub = l.item(i).childNodes();
      appendSub(listSub,item); 
    }
  }  
} 

void CSSEditor::activatePreview() { 
  m_previewer->openURL(KURL( m_testFile->name() )); 
}

void CSSEditor::setCurrentPropOn(const QString& s){
  if( (m_currentProp = static_cast<myCheckListItem*>(lvVisual->findItem( s,0 )) )) 
    m_currentProp->setOn(true);
  else  
  if( (m_currentProp = static_cast<myCheckListItem*>(lvAll->findItem( s,0 )) )) 
    m_currentProp->setOn(true); 
  else  
  if( (m_currentProp = static_cast<myCheckListItem*>(lvAural->findItem( s,0 )) )) 
    m_currentProp->setOn(true);   
  else  
  if( (m_currentProp = static_cast<myCheckListItem*>(lvInteractive->findItem( s,0 )) )) 
    m_currentProp->setOn(true); 
  else  
  if( (m_currentProp = static_cast<myCheckListItem*>(lvPaged->findItem( s,0 )) )) 
    m_currentProp->setOn(true); 
        
  if( m_currentProp && m_currentProp->depth() ) {
    myCheckListItem *p = static_cast<myCheckListItem*>(m_currentProp->parent());
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
  QFileInfo fi(quantaApp->currentURL());
  m_testFileName = ( quantaApp->projectBaseURL().path()+ "quanta_test_file_of_" + fi.baseName() +"."+ fi.extension());
  QString configFile = locate("appdata", "csseditor/config.xml");    

  m_myhi = new QMyHighlighter(display);
  display->setReadOnly(true);
  
  QBoxLayout *fPreviewLayout = new QBoxLayout(fPreview,QBoxLayout::LeftToRight);
  m_previewer=new KHTMLPart(fPreview);
  fPreviewLayout->addWidget(m_previewer->view());
  
  QFile file( configFile );
  if ( !file.open( IO_ReadOnly ) ) {
    return;
  }
  if ( !m_doc.setContent( &file ) ) { 
    file.close();
    return;
  }
  file.close();   

  QDomElement docElem = m_doc.documentElement();

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
  m_testFile = new QFile(m_testFileName);
  m_testFile->open(IO_WriteOnly);   
  
  m_previewer->openURL(KURL( m_testFile->name() ));

  QBoxLayout *fEditingLayout = new QBoxLayout(fEditing,QBoxLayout::LeftToRight);
  
  m_ps = new propertySetter(fEditing);
  fEditingLayout->addWidget(m_ps);
  connect(m_ps, SIGNAL(valueChanged(const QString&)), this, SLOT(checkProperty(const QString&)));
  
  QStringList props;
  QString temp;
  bool normalMode = true;
  QTextStream stream( m_testFile );
  
  if( !m_selectorName.isEmpty() ){ //the cssselector has been called
     m_initialProperties = m_initialProperties.stripWhiteSpace();
     props=QStringList::split(";",m_initialProperties);
     temp=(m_selectorName + " {\n\t");
     stream << ( m_Header + m_selectorName + " { " + m_initialProperties + " } "+ m_Selectors + m_Footer);
   }  
  
  else {
    m_InlineStyleContent = m_InlineStyleContent.stripWhiteSpace();
    normalMode = false;
    props=QStringList::split(";",m_InlineStyleContent);
    temp="\n\t";
    stream << m_initialPreviewText;
  }
  m_testFile->close();
     
  for ( QStringList::Iterator it = props.begin(); it != props.end(); ++it ) {
       const QString propertyName((*it).section(":",0,0).stripWhiteSpace());
       const QString propertyValue((*it).section(":",1,1));
      
      if( ShorthandFormer::SHFormList().contains(propertyName)==0 ) {
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
}

CSSEditor::CSSEditor(QListViewItem *i, QWidget *parent, const char *name) : CSSEditorS(parent, name){
  m_selectorName = i->text(0);
  m_initialProperties = i->text(1);
}

CSSEditor::CSSEditor( QString s, QWidget* parent, const char *name) : CSSEditorS(parent, name){
  m_selectorName = s;
}

CSSEditor::~CSSEditor() { 
    delete m_myhi;
    delete m_ps;
    delete m_previewer;
    if(m_testFile){
      delete m_testFile;
      m_testFile=0L;
    }
    if(QFile::exists(m_testFileName))
      QFile::remove(m_testFileName);
}

void CSSEditor::setMiniEditors(QListViewItem* i)
{
  m_ps->reset();
  
  if(i->childCount()==0) {
   
    m_currentProp = static_cast<myCheckListItem*>(i);

    QDomElement type = m_doc.elementsByTagName(i->text(0)).item(0).firstChild().toElement();

    if(type.tagName() == "selectable") {

      QDomNodeList values = type.childNodes();
      unsigned int i;
      m_ps->setComboBox();
      for(i=0; i<values.length(); i++) {
        m_ps->ComboBox()->insertItem(values.item(i).toElement().attribute("name"));
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
          m_ps->setComboBox();
          for(k=0; k<values.length(); k++) {
            m_ps->ComboBox()->insertItem(values.item(k).toElement().attribute("name"));
          }
        }
        
        if(curr.tagName()=="numeric") {

          QDomNodeList values = curr.childNodes();
          unsigned int k;
          for(k=0; k<values.length(); k++) {

            QString typeName(values.item(k).toElement().attribute("type"));
            
            if( typeName == "spinbox") {
              m_ps->setSpinBox(values.item(k).toElement().attribute("minValue"),
              values.item(k).toElement().attribute("maxValue"),
              values.item(k).toElement().attribute("suffix"));
            }
                
            if( typeName == "edit") {
              m_ps->ComboBox()->setEditable(true);
              m_ps->ComboBox()->lineEdit()->clear();
            }
                
            if( typeName == "length") m_ps->setLengthEditor(); 
            if( typeName == "percentage") m_ps->setPercentageEditor();
            if( typeName == "doubleLength") m_ps->setDoubleLengthEditor();     
            if( typeName == "doublePercentage") m_ps->setDoublePercentageEditor();
            if( typeName == "integer") m_ps->setSpinBox();        
            if( typeName == "number") m_ps->setLineEdit();  
            if( typeName == "frequency") m_ps->setFrequencyEditor();           
            if( typeName == "time") m_ps->setTimeEditor();               
            if( typeName == "angle") m_ps->setAngleEditor();                         
          }
        }  
        else
        if(curr.tagName()=="string") {

          QDomNodeList values = curr.childNodes();
          unsigned int k;
          for(k=0; k<values.length(); k++) {

            QString typeName(values.item(k).toElement().attribute("type"));
            if( typeName == "uri") {
              m_ps->setUriEditor();
              if(values.item(k).toElement().attribute("mode") == "multi")
                m_ps->UriEditor()->setMode(URIEditor::multi);
              if(values.item(k).toElement().attribute("resourceType") == "audio")
                m_ps->UriEditor()->setResourceType(URIEditor::audio);
              else
              if(values.item(k).toElement().attribute("resourceType") == "image")
                m_ps->UriEditor()->setResourceType(URIEditor::image);  
              else
              if(values.item(k).toElement().attribute("resourceType") == "mousePointer")
                m_ps->UriEditor()->setResourceType(URIEditor::mousePointer); 
            }
                
            if( typeName == "edit") {
              m_ps->ComboBox()->setEditable(true);
              m_ps->ComboBox()->lineEdit()->clear();
            }
            
            if( typeName == "color") m_ps->setColorRequester();
            if( typeName == "predefinedColorList") m_ps->setPredefinedColorListEditor();
            if( typeName == "fontDialog" ) m_ps->setFontEditor();                
          }

            }
            else
          if(curr.tagName()=="doubleSelectable")
            {
              m_ps->setDoubleComboBoxEditor();
              unsigned int k;
              QDomNodeList sxValues = curr.firstChild().childNodes();
              for(k=0; k<sxValues.length(); k++){
                m_ps->DoubleComboBoxEditor()->cbSx()->insertItem(sxValues.item(k).toElement().attribute("name"));
              }

              QDomNodeList dxValues = curr.lastChild().childNodes();
              for(k=0; k<dxValues.length(); k++){
                m_ps->DoubleComboBoxEditor()->cbDx()->insertItem(dxValues.item(k).toElement().attribute("name"));
              }            
            }            
        }
       m_ps->addButton();
       
     }// end "mixed" if
     m_ps->Show();
    }
}

void CSSEditor::checkProperty(const QString& v){
  if(!m_currentProp->isOn()) {
    m_currentProp->setOn(true);
  } 
  if( m_currentProp->depth() ){
    myCheckListItem *p = static_cast<myCheckListItem*>(m_currentProp->parent());
    while(p){
      p->setOn(true);
      p=static_cast<myCheckListItem*>(p->parent());
    }
  }
  addProperty(m_currentProp->text(0),v);
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
  m_properties.remove(v.toString());
  updatePreview();
}

QString CSSEditor::generateProperties(){
  QString props;
  QMap<QString,QString>::Iterator it;
  if(!SHckb->isChecked()) {
    for ( it = m_properties.begin(); it != m_properties.end(); ++it ) {
       props+=( it.key() + " : " + it.data() + "; " );
    }
    props.truncate(props.length()-1);//the last white space creates some problem: better remove it
    return props;  
  }  
  else {
    ShorthandFormer sf(m_properties);
    return sf.compress();  
  }  
}

void CSSEditor::updatePreview()
{
    updateTestFile();
    updateDisplay();
    activatePreview();
}

void CSSEditor::updateTestFile()
{
  QMap<QString,QString> dummyProperties = m_properties;

  delete m_testFile;
  m_testFile=0L;
  m_testFile = new QFile(m_testFileName);
  m_testFile->open(IO_WriteOnly);
  
  QString testFileHeader(m_Header);
  QString testFileFooter;
  
  if(!m_selectorName.isEmpty()) {
    testFileHeader += (  m_selectorName + " { \n " );
    testFileFooter = ( "}"  + m_Selectors );
  }
  else {
    testFileHeader += (" style=\"" );
    testFileFooter = "\"" ;
  }
  testFileFooter += m_Footer ;
  
  QTextStream stream( m_testFile );
  stream << testFileHeader;
  QMap<QString,QString>::Iterator it;
  for ( it = dummyProperties.begin(); it != dummyProperties.end(); ++it ) {
    QString s( it.key() + " : " + it.data() + ";");
    stream << s;
   }
   stream << testFileFooter; 
   m_testFile->close();
}

void CSSEditor::updateDisplay()
{
  QString toDisplay;
  bool normalMode = true;
     
  if(!m_selectorName.isEmpty()){// we're working on <style></style> block
      toDisplay= ( m_selectorName +" {\n\t" );
  }    
  else {
      normalMode = false;
      toDisplay = "\n\t";
   }
  QMap<QString,QString>::Iterator it;
  for ( it = m_properties.begin(); it != m_properties.end(); ++it ) {
    QString s( it.key() + " : " + it.data() + ";");
    toDisplay += (s + "\n\t");
  }        
  if(normalMode)//normal mode editing
    toDisplay+="}";
  display->setText(toDisplay);
}

#include "csseditor.moc"
