/***************************************************************************
                          csseditor.cpp  -  description
                             -------------------
    begin                : mer lug 23 11:20:17 CEST 2003
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

#include "csseditor.h"
#include <qlayout.h>
#include <qtabwidget.h>
#include <qpushbutton.h>
#include <qcheckbox.h>
#include <qtextedit.h>
#include <qtextstream.h>
#include <qfileinfo.h>

#include <kapplication.h>
#include <klocale.h>
#include <khtml_part.h>
#include <khtmlview.h>
#include <kstandarddirs.h>
#include <klineedit.h>
#include <kdebug.h>
#include <kconfig.h>
#include <kparts/browserextension.h>

#include "propertysetter.h"
#include "qmyhighlighter.h"
#include "cssshpropertyparser.h"
#include "shorthandformer.h"
#include "percentageeditor.h"
#include "colorslider.h"
#include "tlpeditors.h"
#include "doubleeditors.h"
#include "specialsb.h"
#include "colorrequester.h"



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

 void CSSEditor::setSidesOfPropertyBorderOn(const QString& s){
   static_cast<myCheckListItem*>(lvVisual->findItem( "border-top",0 ))->setOn(true);
   static_cast<myCheckListItem*>(lvVisual->findItem( "border-right",0 ))->setOn(true);
   static_cast<myCheckListItem*>(lvVisual->findItem( "border-bottom",0 ))->setOn(true);
   static_cast<myCheckListItem*>(lvVisual->findItem( "border-left",0 ))->setOn(true);
   static_cast<myCheckListItem*>(lvVisual->findItem( "border-top-"+s,0 ))->setOn(true);
   static_cast<myCheckListItem*>(lvVisual->findItem( "border-right-"+s,0 ))->setOn(true);
   static_cast<myCheckListItem*>(lvVisual->findItem( "border-bottom-"+s,0 ))->setOn(true);
   static_cast<myCheckListItem*>(lvVisual->findItem( "border-left-"+s,0 ))->setOn(true);
 }

void CSSEditor::hidePreviewer(){
  fPreview->hide();
 }

void CSSEditor::initialize(){

  m_config = kapp->config();     
  connect(pbOk, SIGNAL(clicked()), this, SLOT(toggleShortendForm()));
  m_config->setGroup("CSSEditor Options");
  SHckb->setChecked(m_config->readBoolEntry("Shorthand form enabled",false));

  QString configFile = locate("appdata", "csseditor/config.xml");

  m_myhi = new QMyHighlighter(display);

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

  QBoxLayout *fEditingLayout = new QBoxLayout(fEditing,QBoxLayout::LeftToRight);

  m_ps = new propertySetter(fEditing);
  fEditingLayout->addWidget(m_ps);
  connect(m_ps, SIGNAL(valueChanged(const QString&)), this, SLOT(checkProperty(const QString&)));

  QStringList props;
  QString temp;
  bool normalMode = true;

  if( !m_selectorName.isEmpty() ){ //the cssselector has been called
     m_initialProperties = m_initialProperties.stripWhiteSpace();
     props=QStringList::split(";",m_initialProperties);
     temp= m_selectorName + " {\n\t" ;
   }

  else {
    m_InlineStyleContent = m_InlineStyleContent.stripWhiteSpace();
    normalMode = false;
    props=QStringList::split(";",m_InlineStyleContent);
    temp="\n\t";
  }

  for ( QStringList::Iterator it = props.begin(); it != props.end(); ++it ) {
       const QString propertyName((*it).section(":",0,0).stripWhiteSpace());
       const QString propertyValue((*it).section(":",1));

      if( ShorthandFormer::SHFormList().contains(propertyName)==0 ) {
         temp+= propertyName + " : " + propertyValue +";\n\t";
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
    //temp.chop(1);
    if(normalMode)//normal mode editing
      temp+="}";
    display->setText(temp);
    activatePreview();
}

void CSSEditor::toggleShortendForm()
{
 m_config->setGroup("CSSEditor Options");
 m_config->writeEntry("Shorthand form enabled", SHckb->isChecked());
 m_config->sync();
}

CSSEditor::CSSEditor(QListViewItem *i, QWidget *parent, const char *name) : CSSEditorS(parent, name){
  m_selectorName = i->text(0);
  m_initialProperties = i->text(1);
}

CSSEditor::~CSSEditor() {
    delete m_myhi;
    delete m_ps;
    delete m_previewer;
}

void CSSEditor::setMiniEditors(QListViewItem* i){

  m_ps->reset();

  if(i->childCount()==0) {
    m_currentProp = static_cast<myCheckListItem*>(i);
    QDomNodeList valueTypes = m_doc.elementsByTagName(i->text(0)).item(0).childNodes();
    unsigned int i;
    for(i=0; i<valueTypes.length(); i++) {
      QDomElement curr =valueTypes.item(i).toElement();
      QString valueTypeName(curr.tagName());

      if(valueTypeName =="list") {
        m_ps->setComboBox();
        QStringList values = QStringList::split(",",curr.attribute("value"));
        m_ps->ComboBox()->insertStringList(values);
        if(m_properties.contains(m_currentProp->text(0)) !=0 )
          if( values.contains(m_currentProp->text(0)))
            m_ps->ComboBox()->setCurrentText(m_properties[m_currentProp->text(0)]);
        if(curr.attribute("editable") == "yes"){
          m_ps->ComboBox()->setEditable(true);
          /*if(m_properties.contains(m_currentProp->text(0)) !=0 )
            m_ps->ComboBox()->setEditText(m_properties[m_currentProp->text(0)]); */
        }
      }
      else

      /*if( typeName == "spinbox") {
            m_ps->setSpinBox("0", values.item(k).toElement().attribute("minValue"),
            values.item(k).toElement().attribute("maxValue"),
            values.item(k).toElement().attribute("suffix"));
          }

      else*/
      if( valueTypeName == "number") m_ps->setLineEdit();
      else
      if( valueTypeName == "integer") {
        if(m_properties.contains(m_currentProp->text(0)) !=0 ) {
          if(!curr.attribute("minValue").isNull())
            m_ps->setSpinBox(m_properties[m_currentProp->text(0)],curr.attribute("minValue"));
          else
            m_ps->setSpinBox(m_properties[m_currentProp->text(0)]);
        }
        else
          if(!curr.attribute("minValue").isNull())
            m_ps->setSpinBox("0",curr.attribute("minValue"));
          else
            m_ps->setSpinBox();
      }
      else
      if( valueTypeName == "length") {
        lengthEditor *editor = new lengthEditor(m_ps);
        if(m_properties.contains(m_currentProp->text(0)) !=0 )
          editor->setInitialValue(m_properties[m_currentProp->text(0)]);
        else
          editor->setInitialValue(QString::null);
        m_ps->installMiniEditor(editor);
      }
      else
      if( valueTypeName == "percentage") {
        if(m_properties.contains(m_currentProp->text(0)) !=0 ){
          percentageEditor *editor = new percentageEditor(m_properties[m_currentProp->text(0)],m_ps);
          m_ps->installMiniEditor(editor);
        }
        else {
          percentageEditor *editor = new percentageEditor("0",m_ps);
          m_ps->installMiniEditor(editor);
        }
      }
      else
      if( valueTypeName == "doubleLength") {
        doubleLengthEditor *editor = new doubleLengthEditor(m_ps);

        if(m_properties.contains(m_currentProp->text(0)) !=0 ){
          QString temp(m_properties[m_currentProp->text(0)].simplifyWhiteSpace()),
                       sx(temp.section(" ",0,0)),
                       dx(temp.section(" ",1,1));

          editor->setInitialValue(sx,dx);
        }
        else editor->setInitialValue(QString::null,QString::null);
        m_ps->installMiniEditor(editor);
      }
      else
      if( valueTypeName == "doublePercentage") {
        doublePercentageEditor *editor = new doublePercentageEditor(m_ps);

        if(m_properties.contains(m_currentProp->text(0)) !=0 ){
          QString temp(m_properties[m_currentProp->text(0)].simplifyWhiteSpace()),
                       sx(temp.section(" ",0,0)),
                       dx(temp.section(" ",1,1));

          editor->setInitialValue(sx,dx);
        }
        else editor->setInitialValue(QString::null,QString::null);
        m_ps->installMiniEditor(editor);
      }
      else
      if( valueTypeName == "frequency") {
        frequencyEditor *editor = new frequencyEditor(m_ps);
        if(m_properties.contains(m_currentProp->text(0)) !=0 )
          editor->setInitialValue(m_properties[m_currentProp->text(0)]);
        else
          editor->setInitialValue(QString::null);
        m_ps->installMiniEditor(editor);
      }
      else
      if( valueTypeName == "time") {
        timeEditor *editor = new timeEditor(m_ps);
        if(m_properties.contains(m_currentProp->text(0)) !=0 )
          editor->setInitialValue(m_properties[m_currentProp->text(0)]);
        else
          editor->setInitialValue(QString::null);
        m_ps->installMiniEditor(editor);
      }
      else
      if( valueTypeName == "angle") {
        angleEditor *editor = new angleEditor(m_ps);
        if(m_properties.contains(m_currentProp->text(0)) !=0 )
         editor->setInitialValue(m_properties[m_currentProp->text(0)]);
        else
          editor->setInitialValue(QString::null);
        m_ps->installMiniEditor(editor);
      }
      else
      if( valueTypeName == "freeedit") {
        m_ps->setLineEdit();
      }
      else
      if( valueTypeName == "uri") {
        URIEditor *editor = new URIEditor(m_ps);

        if(curr.attribute("mode") == "multi") editor->setMode(URIEditor::Multi);
        else editor->setMode(URIEditor::Single);

        if( curr.attribute("resourceType") == "audio") editor->setResourceType(URIEditor::audio);
        else
          if( curr.attribute("resourceType") == "image") editor->setResourceType(URIEditor::image);
          else
            if( curr.attribute("resourceType") == "mousePointer") editor->setResourceType(URIEditor::mousePointer);

        m_ps->installMiniEditor(editor);
      }
      else
      if( valueTypeName == "colors") {
        RGBcolorSlider *RGBeditor = new RGBcolorSlider(m_ps);
        colorRequester *CReditor = new colorRequester(m_ps);
        if(m_properties.contains(m_currentProp->text(0)) !=0 ){
          CReditor->lineEdit()->setText(m_properties[m_currentProp->text(0)]);
          CReditor->setInitialValue();

        }

        m_ps->installMiniEditor(CReditor);
        m_ps->setPredefinedColorListEditor();
        m_ps->installMiniEditor(RGBeditor);
      }
      else
      if( valueTypeName =="doubleList") {
        doubleComboBoxEditor *editor = new doubleComboBoxEditor(m_ps);
        editor->cbSx()->insertStringList(QStringList::split(",",curr.firstChild().toElement().attribute("value")));
        editor->cbDx()->insertStringList(QStringList::split(",",curr.lastChild().toElement().attribute("value")));
        m_ps->installMiniEditor(editor);
      }
      else
        if( valueTypeName == "fontDialog" ){
          fontEditor *editor = new fontEditor(m_ps);
          if(m_properties.contains(m_currentProp->text(0)) !=0 ){
            editor->setInitialValue(m_properties[m_currentProp->text(0)]);
          }
          else editor->setInitialValue(QString::null);
          m_ps->installMiniEditor(editor);
        }
    }
    m_ps->addButton();
    m_ps->Show();
  }
}

void CSSEditor::checkProperty(const QString& v){
  if(!m_currentProp->isOn())
    m_currentProp->setOn(true);

  if( m_currentProp->depth() ){
    myCheckListItem *p = static_cast<myCheckListItem*>(m_currentProp->parent());
    while(p){
      p->setOn(true);
      p=static_cast<myCheckListItem*>(p->parent());
    }
  }

  if(m_currentProp->text(0) =="border-style" ){
    setSidesOfPropertyBorderOn("style");
    QStringList values = QStringList::split(" ",v.stripWhiteSpace());
    addProperty("border-top-style",values[0]);
    switch(values.count()) {
      case 1:
                   addProperty("border-right-style",values[0]);
                   addProperty("border-bottom-style",values[0]);
                   addProperty("border-left-style",values[0]);
                   break;

      case 2:
                   addProperty("border-right-style",values[1]);
                   addProperty("border-bottom-style",values[0]);
                   addProperty("border-left-style",values[1]);
                   break;

      case 3:
                   addProperty("border-right-style",values[1]);
                   addProperty("border-bottom-style",values[2]);
                   addProperty("border-left-style",values[1]);
                   break;

      case 4:
                   addProperty("border-right-style",values[1]);
                   addProperty("border-bottom-style",values[2]);
                   addProperty("border-left-style",values[3]);break;
    }
  }
  else
  if(m_currentProp->text(0) =="border-width" ){
    setSidesOfPropertyBorderOn("width");
    QStringList values = QStringList::split(" ",v.stripWhiteSpace());
    addProperty("border-top-width",values[0]);
    switch(values.count()) {
      case 1:
                   addProperty("border-right-width",values[0]);
                   addProperty("border-bottom-width",values[0]);
                   addProperty("border-left-width",values[0]);
                   break;

      case 2:
                   addProperty("border-right-width",values[1]);
                   addProperty("border-bottom-width",values[0]);
                   addProperty("border-left-width",values[1]);
                   break;

      case 3:
                   addProperty("border-right-width",values[1]);
                   addProperty("border-bottom-width",values[2]);
                   addProperty("border-left-width",values[1]);
                   break;

      case 4:
                   addProperty("border-right-width",values[1]);
                   addProperty("border-bottom-width",values[2]);
                   addProperty("border-left-width",values[3]);break;
    }
  }
  else
  if(m_currentProp->text(0) =="border-color" ){
    setSidesOfPropertyBorderOn("color");
    QStringList values = QStringList::split(" ",v.stripWhiteSpace());
    addProperty("border-top-color",values[0]);
    switch(values.count()) {
      case 1:
                   addProperty("border-right-color",values[0]);
                   addProperty("border-bottom-color",values[0]);
                   addProperty("border-left-color",values[0]);
                   break;

      case 2:
                   addProperty("border-right-color",values[1]);
                   addProperty("border-bottom-color",values[0]);
                   addProperty("border-left-color",values[1]);
                   break;

      case 3:
                   addProperty("border-right-color",values[1]);
                   addProperty("border-bottom-color",values[2]);
                   addProperty("border-left-color",values[1]);
                   break;

      case 4:
                   addProperty("border-right-style",values[1]);
                   addProperty("border-bottom-style",values[2]);
                   addProperty("border-left-style",values[3]);break;
    }
  }
  else addProperty(m_currentProp->text(0),v);
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

void CSSEditor::removeProperty(const QVariant& v){
  m_properties.remove(v.toString());
  updatePreview();
}

QString CSSEditor::generateProperties(){
  QString props;
  QMap<QString,QString>::Iterator it;
  if(!SHckb->isChecked()) {
    for ( it = m_properties.begin(); it != m_properties.end(); ++it )
      props+= it.key() + " : " + it.data().stripWhiteSpace() + "; " ;
    props.truncate(props.length()-1);//the last white space creates some problems: better remove it
    //props.chop(1);
    return props;
  }
  else {
    ShorthandFormer sf(m_properties);
    return sf.compress();
  }
}

void CSSEditor::updatePreview(){
  updateDisplay();
  activatePreview();
}

void CSSEditor::activatePreview() {
  if(!m_isFileToPreviewExternal){
   QString testHeader,
                 testFooter,
                 testBody;

    if(!m_selectorName.isEmpty()) {
      testHeader +=  m_selectorName + " { \n ";
      testFooter =  "\n}"  + m_Selectors;
    }
    else {
      testHeader += " style=\"" ;
      testFooter = "\"" ;
    }

    QMap<QString,QString>::Iterator it;
    for ( it = m_properties.begin(); it != m_properties.end(); ++it )
      testBody+= it.key() + " : " + it.data() + ";";

    m_previewer->begin( KURL(m_fileToPreview) );
    m_previewer->write( m_Header +  testHeader + testBody+ testFooter+ m_Footer);
    m_previewer->end();


  }
  else {
    QString tmp("{");
    QFile file(m_fileToPreview);
    if ( file.open( IO_ReadOnly ) ) {
        QMap<QString,QString>::Iterator it;
        for ( it = m_properties.begin(); it != m_properties.end(); ++it )
          tmp+= it.key() + " : " + it.data() + ";";


        QFileInfo fi(m_fileToPreview);
        KParts::URLArgs a;
        if(fi.extension().lower() == "xml" || fi.extension().lower() == "xhtml")
          a.serviceType="text/xml";
        if(fi.extension().lower() == "html" || fi.extension().lower() == "html")
          a.serviceType="text/xml";
        m_previewer->browserExtension()->setURLArgs(a);
        QTextStream stream( &file );

        m_previewer->begin(KURL(m_fileToPreview));
        m_previewer->write(stream.read());
        m_previewer->end();
        m_previewer->setUserStyleSheet(m_externalStyleSheetDefinition + " " + m_selectorName+" "+ tmp +"}");
        file.close();
    }
   //else KMessageBox::sorry(this,i18n("The css file you want to edit can't be opened"));
  }
}

void CSSEditor::updateDisplay(){
  QString toDisplay;
  QMap<QString,QString>::Iterator it;
  for ( it = m_properties.begin(); it != m_properties.end(); ++it )
    toDisplay += it.key() + " : " + it.data() + ";\n\t";

  if(!m_selectorName.isEmpty()){// we're working on <style></style> block
    toDisplay.prepend(m_selectorName +" {\n\t");
    toDisplay+="}";
  }
  else  toDisplay.prepend("\n\t");

  display->setText(toDisplay);
}

#include "csseditor.moc"
