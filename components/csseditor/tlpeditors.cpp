/***************************************************************************
                          tlpeditors.cpp  -  description
                             -------------------
    begin                : gio apr 1 2004
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

#include <qlineedit.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

#include <kpushbutton.h>
#include <kurl.h>
#include <kdialog.h>
#include <kiconloader.h>
#include <klocale.h>
#include <kfiledialog.h>
#include <kimagefilepreview.h>

#include "qextfileinfo.h"
#include "tlpeditors.h"
#include "fontfamilychooser.h"
#include "project.h"
    
TLPEditor::TLPEditor(QWidget *parent, const char* name) : miniEditor(parent,name){
  m_label = new QLabel(this);
  m_le = new QLineEdit(this);
  m_pb = new KPushButton(this);
  setSpacing( KDialog::spacingHint() );
}

TLPEditor::~TLPEditor(){
  delete m_label;
  delete m_le;
  delete m_pb;
}

void TLPEditor::setButtonIcon(QString s){
  QIconSet iconSet =  SmallIconSet(QString::fromLatin1(s));
  QPixmap pixMap = iconSet.pixmap( QIconSet::Small, QIconSet::Normal );
  m_pb->setIconSet(iconSet);
  m_pb->setFixedSize( pixMap.width()+8, pixMap.height()+8 );
}

void TLPEditor::setLabelText(QString s){
  m_label->setText(s);
}

void TLPEditor::setToolTip(QString s){
  QToolTip::add(m_pb, s);
}

void TLPEditor::setWhatsThis(QString s){
  QWhatsThis::add(m_le,s);
}

URIEditor::URIEditor(QWidget *parent, const char* name) : TLPEditor(parent,name){
  QString whatsthis =i18n("With this line edit you can insert the URI of the resource you want to reach");  
  setWhatsThis(whatsthis);
  setLabelText(" Uri  :");
  setButtonIcon("fileopen");
  setToolTip(i18n("Open the URI selector"));
    
  connect(m_pb, SIGNAL(clicked()), this, SLOT(openFileDialog()));
}

void URIEditor::connectToPropertySetter(propertySetter* p){
  connect(this,SIGNAL(valueChanged(const QString&)), p ,SIGNAL(valueChanged(const QString&))); 
}

void URIEditor::setMode(const mode& m) { 
  m_Mode = m ; 
  if( m_Mode == Single )
    connect(m_le, SIGNAL(textChanged ( const QString & )), this, SLOT(selectedURI(const QString&)));
  else{
    connect(m_le, SIGNAL(textChanged ( const QString & )), this, SLOT(selectedURIs(const QStringList&))); 
  }  
}

void URIEditor::selectedURI(const QString & s){
   KURL u;
   u.setPath(s);
   emit valueChanged("url(\'" + QExtFileInfo::toRelative(u, Project::ref()->projectBaseURL()).path() + "\')");
}

void URIEditor::selectedURIs(const QStringList& s){
  KURL u;
  QStringList selectedFiles = s,
                     selectedFilesWithURLFormat;
  for ( QStringList::Iterator it = selectedFiles.begin(); it != selectedFiles.end(); ++it ){      
    u.setPath(*it);   
    selectedFilesWithURLFormat.append( "url(\'" + QExtFileInfo::toRelative(u, Project::ref()->projectBaseURL()).path() + "\')");
  }
  emit valueChanged(selectedFilesWithURLFormat.join(","));
}
 
void URIEditor::openFileDialog(){
  
  KFileDialog fd( Project::ref()->projectBaseURL().url(), "*.*", this, "file dialog", true );
  switch(m_resourceType) {
    case image :   {
                             fd.setFilter( "*.png *.gif *.jpg *.mng|" + i18n("Image Files") +" (*.png *.gif *.jpg *.mng)\n*.*|" + i18n("All Files")+(" (*.*)") );
                             KImageFilePreview *ip = new KImageFilePreview( &fd );
                             fd.setPreviewWidget( ip );
                            } 
                             break;
    case audio : {
                           fd.setFilter( "*.au *.aiff *.wav|" + i18n("Audio Files")+" (*.au *.aiff *.wav)\n*.*|" + i18n("All Files")+(" (*.*)") );

    
                         }
                         break; 
  //case mousePointer :   fd.setFilter( "*.|" + i18n("Mouse Pointers (*.)")+"\n*.*|" + i18n("All Files (*.*)") );break;
    case mousePointer :   fd.setFilter( "*.*|" + i18n("All Files")+(" (*.*)") );break;
    
    default:;
  }
    
  if( m_Mode == Single) 
    fd.setMode(KFile::File); 
  else 
    fd.setMode(KFile::Files);
 
  
  if( fd.exec() ){
    if( fd.mode() == KFile::File )
      selectedURI( fd.selectedFile() );
    else {
      selectedURIs( fd.selectedFiles() );
      /*QStringList selectedFiles = fd.selectedFiles();
      KURL u;
      for ( QStringList::Iterator it = selectedFiles.begin(); it != selectedFiles.end(); ++it ){      
        u.setPath(*it);   
      m_sFiles.append( "url(\'" + QExtFileInfo::toRelative(u, Project::ref()->projectBaseURL()).path() + "\')");
      }
      emit valueChanged(m_sFiles.join(","));*/
    }
  }
}

fontEditor::fontEditor(QWidget *parent, const char* name) : TLPEditor(parent,name), m_initialValue(QString::null){
  QString whatsthis =i18n("With this line edit you can insert the name of the font you want to use");  
  setWhatsThis(whatsthis);
  setLabelText(i18n("Font family:"));
  setButtonIcon("fonts");
  setToolTip(i18n("Open font family chooser"));
  connect(m_pb, SIGNAL(clicked()), this, SLOT(openFontChooser()));
  connect(m_le, SIGNAL(textChanged ( const QString & )), this, SIGNAL( valueChanged( const QString& ) ) );
}

void fontEditor::connectToPropertySetter(propertySetter* p){
  connect(this, SIGNAL(valueChanged(const QString&)), p, SIGNAL(valueChanged(const QString&))); 
}

void fontEditor::openFontChooser(){
  fontFamilyChooser dlg( this );
  dlg.setInitialValue(m_initialValue);
  if( dlg.exec() )
    emit valueChanged( dlg.fontList().join(", "));
}

#include "tlpeditors.moc"
