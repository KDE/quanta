/***************************************************************************
                          cssselector.cpp  -  description
                             -------------------
    begin                : mer ago 6 2003
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

#include "cssselector.h"
#include "csseditor.h"
#include <qlistview.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include <qpushbutton.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qdom.h>
#include <qtabwidget.h>
#include <qobjectlist.h>
#include <qregexp.h>
#include <qfileinfo.h>
#include <qfiledialog.h> 
#include <kstandarddirs.h>
#include <klocale.h>
#include "csseditor_globals.h"
#include "../framewizard/fwglobal.h"
#include "../../resource.h"
#include "../../quanta.h"

CSSSelector::CSSSelector(QString dtd, QWidget *parent, const char* name) : CSSSelectorS (parent,name), currentDocumentDTD(dtd) {
  
  lvTags->setAllColumnsShowFocus(true);
  lvClasses->setAllColumnsShowFocus(true);
  lvIDs->setAllColumnsShowFocus(true);
  lvPseudo->setAllColumnsShowFocus(true);
  lvAtRules->setAllColumnsShowFocus(true);
  
  Connect();
  QString configDir = locate("appdata", "csseditor/config.xml");
  configDir = QFileInfo(configDir).dirPath() + "/";

  QDomDocument doc;
  QFile file( configDir+"pseudo.xml" );
  if ( !file.open( IO_ReadOnly ) )
    return;
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
      cbPseudo->insertItem(e.attribute("name"));
    }
    n = n.nextSibling();
  }

 file.setName( configDir+"atrules.xml" );
  if ( !file.open( IO_ReadOnly ) )
    return;
  if ( !doc.setContent( &file ) ) {
    file.close();
    return;
  }
  file.close();
 docElem = doc.documentElement();

  n = docElem.firstChild();
  while( !n.isNull() ) {
    QDomElement e = n.toElement();
    if( !e.isNull() ) {
      cbAtRules->insertItem(e.attribute("name"));
    }
    n = n.nextSibling();
  }
}

CSSSelector::~CSSSelector(){
}

void CSSSelector::Connect(){
    
  connect(pbAddTag,SIGNAL(clicked()), this ,SLOT(addTag()));
  connect(pbAddClass,SIGNAL(clicked()), this ,SLOT(addClass()));
  connect(pbAddID,SIGNAL(clicked()), this ,SLOT(addID()));
  connect(pbAddPseudo,SIGNAL(clicked()), this ,SLOT(addPseudo()));
  connect(pbAddAtRules,SIGNAL(clicked()), this ,SLOT(addAtRules()));

  connect(lvTags, SIGNAL(doubleClicked( QListViewItem *  )), this, SLOT(openCSSEditor(QListViewItem *)));
  connect(lvClasses, SIGNAL(doubleClicked( QListViewItem *  )), this, SLOT(openCSSEditor(QListViewItem *)));
  connect(lvIDs, SIGNAL(doubleClicked( QListViewItem *  )), this, SLOT(openCSSEditor(QListViewItem *)));
  connect(lvPseudo, SIGNAL(doubleClicked( QListViewItem *  )), this, SLOT(openCSSEditor(QListViewItem *)));
  connect(lvAtRules, SIGNAL(doubleClicked( QListViewItem *  )), this, SLOT(openAtRulesEditor(QListViewItem *)));
   
  connect(lvTags, SIGNAL(selectionChanged( QListViewItem *  )), this, SLOT(setCurrentItem(QListViewItem *)));
  connect(lvClasses, SIGNAL(selectionChanged( QListViewItem *  )), this, SLOT(setCurrentItem(QListViewItem *)));
  connect(lvIDs, SIGNAL(selectionChanged( QListViewItem *  )), this, SLOT(setCurrentItem(QListViewItem *)));
  connect(lvPseudo, SIGNAL( selectionChanged( QListViewItem *  )), this, SLOT(setCurrentItem(QListViewItem *)));
  connect(lvAtRules, SIGNAL( selectionChanged( QListViewItem *  )), this, SLOT(setCurrentItem(QListViewItem *)));
  
  connect(pbRemoveSelectedTag,SIGNAL(clicked()), this ,SLOT(removeSelected()));
  connect(pbRemoveSelectedClass,SIGNAL(clicked()), this ,SLOT(removeSelected()));
  connect(pbRemoveSelectedID,SIGNAL(clicked()), this ,SLOT(removeSelected()));
  connect(pbRemoveSelectedPseudo,SIGNAL(clicked()), this ,SLOT(removeSelected()));
  connect(pbRemoveSelectedAtRules,SIGNAL(clicked()), this ,SLOT(removeSelected()));

  connect(twSelectors,SIGNAL(currentChanged ( QWidget * )), this ,SLOT(setCurrentListView( QWidget * )));

  connect(pbRemoveAllTags,SIGNAL(clicked()), this ,SLOT(removeAll()));
  connect(pbRemoveAllClasses,SIGNAL(clicked()), this ,SLOT(removeAll()));
  connect(pbRemoveAllIDs,SIGNAL(clicked()), this ,SLOT(removeAll()));
  connect(pbRemoveAllPseudo,SIGNAL(clicked()), this ,SLOT(removeAll()));
  connect(pbRemoveAllAtRules,SIGNAL(clicked()), this ,SLOT(removeAll()));
}

void CSSSelector::addTag(){
  QListViewItem *item = new QListViewItem(lvTags);
  if(!cbTag->currentText().isEmpty())
    item->setText(0,cbTag->currentText());
}

void CSSSelector::addClass(){
  QListViewItem *item = new QListViewItem(lvClasses);
  if(!leClass->text().isEmpty())
    item->setText(0,leClass->text());
}

void CSSSelector::addID(){
  QListViewItem *item = new QListViewItem(lvIDs);
  if(!leID->text().isEmpty())
    item->setText(0,leID->text());
}

void CSSSelector::addPseudo(){
  QListViewItem *item = new QListViewItem(lvPseudo);
  item->setText(0,(lePseudoSelector->text()+":"+cbPseudo->currentText()).stripWhiteSpace());
}

void CSSSelector::addAtRules(){
  QListViewItem *item = new QListViewItem(lvAtRules);
  item->setText(0,"@" +cbAtRules->currentText());
}

void CSSSelector::openCSSEditor(QListViewItem * i){
  CSSEditor *dlg = new CSSEditor(i);

  //dlg->setSourceFileName(sourceFileName);

  QListView *lv = i->listView();
  QListViewItem *temp;
  QString s;

  if(lv != lvTags){
    temp = lvTags->firstChild();
    while(temp){
      if(temp)
        s+=(temp->text(0)+" { "+temp->text(1)+" } ");
      temp = temp->nextSibling();
    }

  }

  if(lv != lvIDs){
    temp = lvIDs->firstChild();
    while(temp){
      if(temp)
        s+=(temp->text(0)+" { "+temp->text(1)+" } ");
      temp = temp->nextSibling();
    }
  }

  if(lv != lvClasses){
    temp = lvClasses->firstChild();
    while(temp){
      if(temp)
        s+=(temp->text(0)+" { "+temp->text(1)+" } ");
      temp = temp->nextSibling();
    }
  }

  if(lv != lvPseudo){
    temp = lvPseudo->firstChild();
    while(temp){
      if(temp)
        s+=(temp->text(0)+" { "+temp->text(1)+" } ");
      temp = temp->nextSibling();
    }
  }

  temp = lv->firstChild();


  while(temp){
    if(temp != i) {
      s+=(temp->text(0)+" { "+temp->text(1)+" } ");
    }
    temp = temp->nextSibling();
  }

  dlg->setForInitialPreview(m_initialPreviewText);
  
  dlg->setHeader(m_header);
  dlg->setSelectors(s);
  dlg->setFooter(m_footer);
  dlg->initialize();
  if(dlg->exec()){
     i->setText(1,dlg->generateProperties());
  }
  delete dlg;
}

void CSSSelector::setCurrentListView(QWidget* w){
  QObjectList *l = w->queryList( "QListView" );
  m_currentListView = static_cast<QListView*>(l->first());
}

void CSSSelector::removeAll(){
   m_currentListView->clear();
}

void CSSSelector::removeSelected(){
  delete m_currentItem;
}

void CSSSelector::loadExistingStyleSection(QString s){
  s.remove("\n");
  s.remove("\t");
  uint lf,rt;
  while(true){
   lf=s.find("/*",0);
   rt=s.find("*/",0);
   if(s.contains("/*") == 0 || s.contains("*/") == 0 ) break;
   s.remove(lf,rt+2-lf);
  }
  
  int  atPos=s.find("@");

  QString tempStr,
               atRuleType;

  while(atPos >=0 ){
    int openParePos=s.find("{",atPos);
    if(openParePos>=0){
      if(s.mid(atPos,openParePos-atPos).contains(";")){
        int SCPos=s.find(";",atPos);
        tempStr=s.mid(atPos,SCPos-atPos).simplifyWhiteSpace();
        tempStr.remove("\n");
        tempStr.remove("\t");
        QListViewItem *item;
        item = new QListViewItem(lvAtRules);
        item->setText(0,tempStr.left( tempStr.find(" ") ) );
        tempStr.remove(0,tempStr.find(" ") );
        item->setText(1,tempStr);
        s.remove(atPos,SCPos-atPos+1);
      }
      else{
        int closedParePos = s.find("}",atPos);
        if(closedParePos>=0){
          int closedPareNum = 1;
          int openPareNum = s.mid(atPos,closedParePos-atPos).contains("{");
          while( closedPareNum != openPareNum ){
            int oldClosedParePos = closedParePos;
            closedParePos = s.find("}",closedParePos+1);
            if(s.mid(oldClosedParePos,closedParePos-oldClosedParePos).contains("{"))
              openPareNum++;
            closedPareNum++;
          }
        }

        tempStr=s.left(closedParePos+1).simplifyWhiteSpace();
        tempStr.remove("\n");
        tempStr.remove("\t");
        QListViewItem *item;
        item = new QListViewItem(lvAtRules);
        item->setText(0,tempStr.left( tempStr.find("{") ) );
        tempStr.remove(0,tempStr.find("{") );

        item->setText(1,tempStr);
        s.remove(atPos,closedParePos-atPos+1);

      }

    }
    else{
      int SCPos=s.find(";",atPos);
      if(SCPos){
        tempStr=s.mid(atPos,SCPos-atPos);
        s.remove(tempStr+";");
        tempStr.remove("\n");
        tempStr.remove("\t");
        QListViewItem *item;
        item = new QListViewItem(lvAtRules);
        item->setText(0,tempStr.left( tempStr.find(" ") ) );
        tempStr.remove(0,tempStr.find(" ") );
        item->setText(1,tempStr);
      }
      else{
        break;
      }
    }
    atPos=s.find("@");
  }

  QMap<QString,QString> styleTagEntities;
    QStringList temp = QStringList::split("}",s.stripWhiteSpace());  
    for ( QStringList::Iterator it = temp.begin(); it != temp.end(); ++it ) {
      (*it).remove("\n");
      (*it).remove("\t");
      styleTagEntities[(*it).section("{",0,0).simplifyWhiteSpace()]=(*it).section("{",1,1).simplifyWhiteSpace();
    }

  QMap<QString,QString>::Iterator it;
  for ( it = styleTagEntities.begin(); it != styleTagEntities.end(); ++it ) {
    QListViewItem *item;
    if(it.key().contains(":")){
      item = new QListViewItem(lvPseudo);
    }
    else
    if(it.key().contains("#")){
      item = new QListViewItem(lvIDs);
    }
    else
    if(it.key().contains(".")){
      item = new QListViewItem(lvClasses); 
    }
    else {
      item = new QListViewItem(lvTags);
    }

    item->setText(0,it.key());
    item->setText(1,it.data());
  }
}

QString CSSSelector::generateStyleSection(){

  QListViewItem *temp;
  QString styleSection;

  temp = lvTags->firstChild();
  while(temp){
    styleSection+=(temp->text(0)+" { "+temp->text(1)+" } \n\t");
    temp = temp->nextSibling();
    }

  temp = lvIDs->firstChild();
  while(temp){
    styleSection+=(temp->text(0)+" { "+temp->text(1)+" } \n\t");
    temp = temp->nextSibling();
    }

  temp = lvClasses->firstChild();
  while(temp){
    styleSection+=(temp->text(0)+" { "+temp->text(1)+" } \n\t");
    temp = temp->nextSibling();
    }

  temp = lvPseudo->firstChild();
  while(temp){
    styleSection+=(temp->text(0)+" "+temp->text(1)+"\n\t");
    temp = temp->nextSibling();
    }
   
  temp = lvAtRules->firstChild();
  while(temp){
    if( temp->text(0) == "@charset" )
      styleSection+=(temp->text(0)+" \"" + temp->text(1) + "\";\n\t");
    else
    if( temp->text(0) == "@import" )
      styleSection+=(temp->text(0)+ " url('" + temp->text(1) + "');\n\t");
    temp = temp->nextSibling();
    }
      
  styleSection.truncate(styleSection.length()-1); //we elminate the last \t

  return QString("\n\t")+styleSection;
}




#include "cssselector.moc"
