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

#include <qlineedit.h>
#include <qcombobox.h>
#include <qpushbutton.h>
#include <qtextstream.h>
#include <qtabwidget.h>
#include <qobjectlist.h>
#include <qfileinfo.h>
#include <qlabel.h>
#include <qregexp.h>
#include <qwhatsthis.h>

#include <kstandarddirs.h>
#include <klocale.h>
#include <kdebug.h>
#include <kmessagebox.h>
#include <kurlrequester.h>
#include <kfiledialog.h>
#include "project.h"
#include "stylesheetparser.h"


CSSSelector::CSSSelector(QWidget *parent, const char* name) : CSSSelectorS (parent,name),m_orderNumber(0),m_stopProcessingStylesheet(false) {

  m_currentItem = 0L;

  Connect();

  kurApplyToFile->fileDialog()->setURL(Project::ref()->projectBaseURL().url());
  kurApplyToFile->fileDialog()->setFilter( "*.html *.htm |" + i18n("HTML Files") +" (*.html *.htm)\n*.xhtml |" + i18n("XHTML Files")+" (*.xhtml)\n*.xml |" + i18n("XML Files")+" (*.xml)\n*.*|" + i18n("All Files")+" (*.*)"  );
  QWhatsThis::add((QLineEdit*)(kurApplyToFile->lineEdit()),"With this line edit you can insert the URL of the file you want to use to preview the style sheet you are editing");

  /*QString configDir = locate("appdata", "csseditor/config.xml");
  configDir = QFileInfo(configDir).dirPath() + "/";*/

  QString configDir = QFileInfo( locate("appdata", "csseditor/config.xml") ).dirPath() + "/";

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

  file.setName( configDir+"dtdTags.xml" );
  if ( !file.open( IO_ReadOnly ) )
    return;
  if ( !doc.setContent( &file ) ) {
    file.close();
    return;
  }
  file.close();

 QStringList dtdNames,
                    dtdNickNames;
 docElem = doc.documentElement();
 n = docElem.firstChild();
  while( !n.isNull() ) {
    QDomElement e = n.toElement();
    if( !e.isNull() ) {
      dtdNames.append(e.attribute("name"));
      dtdNickNames.append(e.attribute("nickName"));
      if(e.attribute("default") == "yes") {
        QStringList tagList = QStringList::split( ',',e.text() );
        tagList.sort();
        cbTag->insertStringList( tagList );
        cbTag->setAutoCompletion(true);
      }
    }
    n = n.nextSibling();
  }
  cbDTD->insertStringList( dtdNickNames );
}

CSSSelector::~CSSSelector(){
}

void CSSSelector::Connect(){

  connect(cbDTD,SIGNAL(activated(const QString&)),this,SLOT(setDTDTags(const QString&)));

  connect(pbAddTag,SIGNAL(clicked()), this ,SLOT(addTag()));
  connect(pbAddClass,SIGNAL(clicked()), this ,SLOT(addClass()));
  connect(pbAddID,SIGNAL(clicked()), this ,SLOT(addID()));
  connect(pbAddPseudo,SIGNAL(clicked()), this ,SLOT(addPseudo()));

  connect(lvTags, SIGNAL(doubleClicked( QListViewItem *  )), this, SLOT(openCSSEditor(QListViewItem *)));
  connect(lvClasses, SIGNAL(doubleClicked( QListViewItem *  )), this, SLOT(openCSSEditor(QListViewItem *)));
  connect(lvIDs, SIGNAL(doubleClicked( QListViewItem *  )), this, SLOT(openCSSEditor(QListViewItem *)));
  connect(lvPseudo, SIGNAL(doubleClicked( QListViewItem *  )), this, SLOT(openCSSEditor(QListViewItem *)));

  connect(lvTags, SIGNAL(selectionChanged( QListViewItem *  )), this, SLOT(setCurrentItem(QListViewItem *)));
  connect(lvClasses, SIGNAL(selectionChanged( QListViewItem *  )), this, SLOT(setCurrentItem(QListViewItem *)));
  connect(lvIDs, SIGNAL(selectionChanged( QListViewItem *  )), this, SLOT(setCurrentItem(QListViewItem *)));
  connect(lvPseudo, SIGNAL( selectionChanged( QListViewItem *  )), this, SLOT(setCurrentItem(QListViewItem *)));

  connect(pbRemoveSelectedTag,SIGNAL(clicked()), this ,SLOT(removeSelected()));
  connect(pbRemoveSelectedClass,SIGNAL(clicked()), this ,SLOT(removeSelected()));
  connect(pbRemoveSelectedID,SIGNAL(clicked()), this ,SLOT(removeSelected()));
  connect(pbRemoveSelectedPseudo,SIGNAL(clicked()), this ,SLOT(removeSelected()));

  connect(twSelectors,SIGNAL(currentChanged ( QWidget * )), this ,SLOT(setCurrentListView( QWidget * )));

  connect(pbRemoveAllTags,SIGNAL(clicked()), this ,SLOT(removeAll()));
  connect(pbRemoveAllClasses,SIGNAL(clicked()), this ,SLOT(removeAll()));
  connect(pbRemoveAllIDs,SIGNAL(clicked()), this ,SLOT(removeAll()));
  connect(pbRemoveAllPseudo,SIGNAL(clicked()), this ,SLOT(removeAll()));
}

void CSSSelector::setDTDTags(const QString& s){

  QString configDir = QFileInfo( locate("appdata", "csseditor/config.xml") ).dirPath() + "/";

  QDomDocument doc;

  QFile file( configDir+"dtdTags.xml" );
  if ( !file.open( IO_ReadOnly ) )
    return;
  if ( !doc.setContent( &file ) ) {
    file.close();
    return;
  }
  file.close();

  QStringList dtdNames;
  QDomElement docElem = doc.documentElement();
  QDomNode n = docElem.firstChild();
  while( !n.isNull() ) {
    if( n.toElement().attribute("nickName") == s )
     break;
  n = n.nextSibling();
  }
  QStringList tagList = QStringList::split( ',',n.toElement().text() );
  tagList.sort();
  cbTag->clear();
  cbTag->insertStringList( tagList );
  cbTag->setAutoCompletion(true);
}

void CSSSelector::addTag(){
  QListViewItem *item = new QListViewItem(lvTags);
  if(!cbTag->currentText().isEmpty()){
    item->setText(0,cbTag->currentText());
    QPair<QString, unsigned int> tmp(QString::null,++m_orderNumber);
    m_currentStylesheetStructure[item->text(0)]=tmp;
  }
}

void CSSSelector::addClass(){
  QListViewItem *item = new QListViewItem(lvClasses);
  if(!leClass->text().isEmpty()){
    item->setText(0,leClass->text());
    QPair<QString, unsigned int> tmp(QString::null,++m_orderNumber);
    m_currentStylesheetStructure[item->text(0)]=tmp;
  }
}

void CSSSelector::addID(){
  QListViewItem *item = new QListViewItem(lvIDs);
  if(!leID->text().isEmpty()){
    item->setText(0,leID->text());
    QPair<QString, unsigned int> tmp(QString::null,++m_orderNumber);
    m_currentStylesheetStructure[item->text(0)]=tmp;
  }
}

void CSSSelector::addPseudo(){
  QListViewItem *item = new QListViewItem(lvPseudo);
  item->setText(0,(lePseudoSelector->text()+":"+cbPseudo->currentText()).stripWhiteSpace());
  QPair<QString, unsigned int> tmp(QString::null,++m_orderNumber);
  m_currentStylesheetStructure[item->text(0)]=tmp;
}

void CSSSelector::openCSSEditor(QListViewItem * i){
  if(!m_stopProcessingStylesheet){
    QListView *lv = i->listView();
    QListViewItem *temp;
    QString s;
    QObjectList *l = queryList( "QListView" );
    QObjectListIt it( *l ); // iterate over the listviews
    QObject *obj;

    while ( (obj = it.current()) != 0 ) {
      QListView *lvTemp = (QListView*)obj;
      if( lv != lvTemp){
        temp = lvTemp->firstChild();
        while(temp){
          s+=(temp->text(0)+" { "+temp->text(1)+" } ");
          temp = temp->nextSibling();
        }
      }
      ++it;
    }
    delete l; // delete the list, not the objects

    temp = lv->firstChild();

    while(temp){
      if(temp != i) s+=(temp->text(0)+" { "+temp->text(1)+" } ");
      temp = temp->nextSibling();
   }

    CSSEditor dlg(i);
    if(m_callingFrom == "XHTML"){
      dlg.setHeader(m_header);
      dlg.setSelectors(s);
      dlg.setFooter(m_footer);
      dlg.setFileToPreview(m_fileToPreview,false);
    }
    else if(m_callingFrom == "CSS"){
              if(kurApplyToFile->url().isEmpty())
                dlg.hidePreviewer();
              else {
                dlg.setFileToPreview(kurApplyToFile->url(),true);

                QString tmp;
                QListViewItem *item = lvTags->firstChild();
                while( item ) {
                  if(i->text(0).stripWhiteSpace() != item->text(0).stripWhiteSpace())
                  tmp += item->text(0) + " {" + item->text(1) + "}";
                  item = item->nextSibling();
                }

                item = lvClasses->firstChild();
                while( item ) {
                  if(i->text(0).stripWhiteSpace() != item->text(0).stripWhiteSpace())
                  tmp += item->text(0) + " {" + item->text(1) + "}";
                  item = item->nextSibling();
                }

                item = lvIDs->firstChild();
                while( item ) {
                  if(i->text(0).stripWhiteSpace() != item->text(0).stripWhiteSpace())
                  tmp += item->text(0) + " {" + item->text(1) + "}";
                  item = item->nextSibling();
                }

                item = lvPseudo->firstChild();
                while( item ) {
                  if(i->text(0).stripWhiteSpace() != item->text(0).stripWhiteSpace())
                  tmp += item->text(0) + " {" + item->text(1) + "}";
                  item = item->nextSibling();
                }

                dlg.setExternalStyleSheetDefinition(tmp);
              }
   }

    dlg.initialize();

    if(dlg.exec())  {
      i->setText(1,dlg.generateProperties());
      QPair<QString, unsigned int> tmp(m_currentStylesheetStructure[i->text(0)]);
      tmp.first = dlg.generateProperties();
      m_currentStylesheetStructure[i->text(0)] = tmp;
    }
  }
}

void CSSSelector::setCurrentListView(QWidget* w){
  QObjectList *l = w->queryList( "QListView" );
  m_currentListView = static_cast<QListView*>(l->first());
}

void CSSSelector::removeAll(){
  QListViewItemIterator it( m_currentListView );
        while ( it.current() ) {
            QListViewItem *item = it.current();
            m_currentStylesheetStructure.remove(item->text(0));
            ++it;
        }
   m_currentListView->clear();
}

void CSSSelector::removeSelected(){
  if( m_currentItem ) {
    m_currentStylesheetStructure.remove(m_currentItem->text(0));
    delete m_currentItem;
    m_currentItem = 0L;
  }
}

void CSSSelector::loadCSSContent(const QString& s){
  stylesheetParser p(s);
  connect(&p,SIGNAL(errorOccurred(const QString&)), this, SLOT(setStylesheetProcessing(const QString&)));
  p.parse();
  m_orderNumber = p.orderNumber();

  QMap<QString, QPair<QString,unsigned int> >::Iterator it;
  m_currentStylesheetStructure = p.stylesheetStructure();
  for ( it = m_currentStylesheetStructure.begin(); it != m_currentStylesheetStructure.end(); ++it ) {
    if(!it.key().startsWith("@rule") && !it.key().startsWith("/*")){
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
      item->setText(1,it.data().first);

    }
  }
}

QString CSSSelector::generateFormattedStyleSection(){
  QMap< QString,QPair<QString,unsigned int> >::Iterator it;
  QString styleSection,tmpStr;
  unsigned int indentWidth,
                      indentDisplacement = 2;
  for ( unsigned int i=0;i<=m_orderNumber;i++ ) {
    for ( it = m_currentStylesheetStructure.begin(); it != m_currentStylesheetStructure.end(); ++it ) {
      QString key = it.key();
      if(it.data().second == i){
       if(key.startsWith("@rule"))
         styleSection += it.data().first;
       else if(key.startsWith("/*"))
                 styleSection += it.data().first;
               else {
                  key.remove(QRegExp("-v[\\d]+$"));
                  styleSection += "\n" + key + " {\n";
                  indentWidth = indentDisplacement + 2;
                  QStringList props = QStringList::split(";",it.data().first.simplifyWhiteSpace());
                  QString indentStr;
                  indentStr.fill(' ',indentWidth);
                  for ( QStringList::Iterator it = props.begin(); it != props.end(); ++it ) {
                    if((*it).startsWith(" "))
                      tmpStr +=  indentStr + (*it).remove(0,1) + ";\n";
                    else
                      tmpStr += indentStr + (*it) + ";\n";
                  }
                  indentStr.fill(' ', indentDisplacement);
                  styleSection += tmpStr + indentStr + "}\n\n";
                  tmpStr = QString::null;
              }
      }
    }
  }
  return "\n"+styleSection;
}

void CSSSelector::enableApplyToFile(){
  tlApplyToFile->setEnabled(true);
  kurApplyToFile->setEnabled(true);
}

void CSSSelector::setStylesheetProcessing(const QString& msg) {
  m_stopProcessingStylesheet=true;
  KMessageBox::error (0L, msg );
}

#include "cssselector.moc"
