/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename slots use Qt Designer which will
** update this file, preserving your code. Create an init() slot in place of
** a constructor, and a destroy() slot in place of a destructor.
*****************************************************************************/
#include <kurlrequester.h>
QString fmFPeditor::getIdValue()
{
    return leId->text();
}


QString fmFPeditor::getSrcValue()
{
    return fc->url();
}


QString fmFPeditor::getClassValue()
{
    return leClass->text();
}

QString fmFPeditor::getLongdescValue()
{
    return leLongdesc->text();
}

QString fmFPeditor::getTitleValue()
{
    return leTitle->text();
}

QString fmFPeditor::getStyleValue()
{
    return leStyle->text();
}

QString fmFPeditor::getNameValue() {
    return leName->text();
}

QString fmFPeditor::getNoresizeValue() {
    if(bgNoresize->selected()->text().lower()=="yes")
     return "";
   return "noresize"; 
}
QString fmFPeditor::getScrollingValue() {
    return bgScrolling->selected()->text().lower();
}
QString fmFPeditor::getFrameborderValue() {
    if( bgBorder->selected()->text().lower()=="yes" ) return "1";
    return "0";
}
QString fmFPeditor::getMarginwidthValue() {
    return QString::number( sbLeft->value(),10 );
}
QString fmFPeditor::getMarginheightValue() {
    return QString::number( sbTop->value(),10 );
}


void fmFPeditor::setup(QMap<QString,QString> m){
    leId->setText(m["id"]);
    fc->setURL(m["src"]);
    leClass->setText(m["class"]);
    leLongdesc->setText(m["longdesc"]);
    leTitle->setText(m["title"]);
    leStyle->setText(m["style"]);
    leName->setText(m["name"]);
    
    if(m["noresize"]!="noresize") bgNoresize->setButton(0);
    else bgNoresize->setButton(1);
    
    if(m["scrolling"]=="yes") bgScrolling->setButton(0);
      else if(m["scrolling"]=="no") bgScrolling->setButton(2);
	   
     if(m["frameborder"]=="0") bgBorder->setButton(1);
    	   
    sbLeft->setValue(m["marginwidth"].toInt());
    sbTop->setValue(m["marginheight"].toInt());
     
}

QMap<QString,QString> fmFPeditor::getAttributeMap(){
  QMap<QString,QString> map;

  map["name"] = getNameValue();
  map["longdesc"] = getLongdescValue();
  map["src"] = getSrcValue();
  map["scrolling"] = getScrollingValue();
  map["id"] = getIdValue();
  map["style"] = getStyleValue();
  map["title"] = getTitleValue();
  map["class"] = getClassValue();
  map["noresize"] = getNoresizeValue();
  map["frameborder"] = getFrameborderValue();
  map["marginwidth"] = getMarginwidthValue();
  map["marginheight"] = getMarginheightValue();

  return map;
}

void fmFPeditor::init()
{
   sbLeft->setWrapping(true);
   sbTop->setWrapping(true);
}


