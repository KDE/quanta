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

#include <kpushbutton.h>
#include <kurl.h>
#include <kdialog.h>
#include <kiconloader.h>
#include <klocale.h>
#include <kfiledialog.h>
#include <kimagefilepreview.h>

#include "qextfileinfo.h"
#include "quanta.h"
#include "resource.h"
#include "tlpeditors.h"
#include "fontfamilychooser.h"
#include "project.h"
    
TLPEditor::TLPEditor(QWidget *parent, const char* name) : QHBox(parent,name){
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

URIEditor::URIEditor(QWidget *parent, const char* name) : TLPEditor(parent,name){
  m_Mode = Single;
  setLabelText(" Uri  :");
  setButtonIcon("fileopen");
  setToolTip(i18n("Open the URI selector"));

  if( m_Mode == Single )
    connect(m_le, SIGNAL(textChanged ( const QString & )), this, SLOT(URI(const QString&)));
  connect(m_pb, SIGNAL(clicked()), this, SLOT(openFileDialog()));
}

void URIEditor::URI(const QString & s){
   KURL u;
   u.setPath(s);
   emit valueChanged("url(\'" + QExtFileInfo::toRelative(u, quantaApp->projectBaseURL()).path() + "\')");
 }
 
void URIEditor::openFileDialog(){
  
  KFileDialog* fd = new KFileDialog( Project::ref()->projectBaseURL().url(), "*.*", this, "file dialog", TRUE );
  switch(m_resourceType) {
    case image :   {
                             fd->setFilter( "*.png *.gif *.jpg *.mng|" + i18n("Image files (*.png *.gif *.jpg *.mng)") +"\n*.*|" + i18n("All files (*.*)") );
                             KImageFilePreview *ip = new KImageFilePreview( fd );
                             fd->setPreviewWidget( ip );
                            } 
                             break;
    case audio :  fd->setFilter( "*.au *.aiff *.wav|" + i18n("Audio files (*.au *.aiff *.wav)")+"\n*.*|" + i18n("All files (*.*)") );break;
    //case mousePointer :   fd->setFilter( "*.|" + i18n("Mouse Pointers (*.)")+"\n*.*|" + i18n("All files (*.*)") );break;
    case mousePointer :   fd->setFilter( "*.*|" + i18n("All files (*.*)") );break;
    
    default:;
  }
  
  bool multi=false;
  
  if( m_Mode == Single) fd->setMode(1); 
  else {
    fd->setMode(4);
    multi=true;
  }
  
  if( fd->exec() ){
    if( !multi)
      URI( fd->selectedFile() );
    else {
      QStringList selectedFiles = fd->selectedFiles();
      KURL u;
      for ( QStringList::Iterator it = selectedFiles.begin(); it != selectedFiles.end(); ++it )
      {      
        u.setPath(*it);   
        m_sFiles.append( "url(\'" + QExtFileInfo::toRelative(u, quantaApp->projectBaseURL()).path() + "\')");
      }
      emit valueChanged(m_sFiles.join(","));
    }
  }
  delete fd;
}

fontEditor::fontEditor(QWidget *parent, const char* name) : TLPEditor(parent,name), m_initialValue(QString::null){
  setLabelText(" Font  family :");
  setButtonIcon("fonts");
  setToolTip(i18n("Open font family chooser"));
  connect(m_pb, SIGNAL(clicked()), this, SLOT(openFontChooser()));
  connect(m_le, SIGNAL(textChanged ( const QString & )), this, SIGNAL( valueChanged( const QString& ) ) );
}

void fontEditor::openFontChooser(){
  fontFamilyChooser *dlg = new fontFamilyChooser( this );
  dlg->setInitialValue(m_initialValue);
  if( dlg->exec() == QDialog::Accepted ){
    emit valueChanged( dlg->fontList().join(", "));
  }
  delete dlg;
}

#include "tlpeditors.moc"
