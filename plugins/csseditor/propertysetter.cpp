/***************************************************************************
                          propertysetter.cpp  -  description
                             -------------------
    begin                : gio lug 24 2003
    copyright            : (C) 2003 by simone
    email                : simone@localhost
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "propertysetter.h"
#include <qlineedit.h>
#include <qcombobox.h>
#include <qspinbox.h>
#include <kpushbutton.h>
#include <qiconset.h>
#include <qpixmap.h>
#include <qptrlist.h>
#include <kdialog.h>
#include <kiconloader.h>
#include <qfiledialog.h>
#include <qlabel.h>
#include <kstandarddirs.h>
#include "colorrequester.h"
#include "specialsb.h"
#include "csseditor_globals.h"
#include <klocale.h>

#include "../framewizard/fwglobal.h"
#include "../../resource.h"
#include "../../quanta.h"


propertySetter::propertySetter(QWidget *parent, const char *name ) : QHBox(parent,name) {
  m_ind = 0;
  m_cb = 0L;
  m_le = 0L;
  m_sb = 0L;
  m_lE = 0L;
  m_dlE = 0L;
  m_dpe = 0L;
  m_dcbe = 0L;
  m_pe = 0L;
  m_ue = 0L;
  m_fe = 0L;
  m_te = 0L;
  m_ae = 0L;
  m_cr = 0L;
  m_pb = 0L;
  m_list.setAutoDelete(true);
  setSpacing( KDialog::spacingHint() );
}

propertySetter::~propertySetter(){
  reset();
}

void propertySetter::reset(){
  if(!m_list.isEmpty())
    m_list.clear();
  if(m_pb) {
    delete m_pb;
    m_pb=0L;
   }
  m_ind=0;
}

void propertySetter::setComboBox()
{
  m_cb = new QComboBox(this);
  connect(m_cb, SIGNAL(activated(const QString&)), this, SIGNAL(valueChanged(const QString&)));
  m_list.append(m_cb);
}

void propertySetter::setSpinBox(const QString& min, const QString& max, const QString& s)
{
  m_sb = new QSpinBox(min.toInt(), max.toInt(), 1, this);
  m_sb->setSuffix(s);
  m_sb->setValue(0);
  connect(m_sb, SIGNAL(valueChanged(const QString&)), this ,SIGNAL(valueChanged(const QString&)));
  m_list.append(m_sb);
}

void propertySetter::setLineEdit()
{
  m_le = new QLineEdit(this);
  connect(m_le,SIGNAL(textChanged ( const QString & )), this, SIGNAL(valueChanged ( const QString & )));
  m_list.append(m_le);
}

void propertySetter::setLengthEditor(){
  m_lE = new lengthEditor(this);
  connect(m_lE, SIGNAL(valueChanged(const QString&)), this ,SIGNAL(valueChanged(const QString&)));
  m_list.append(m_lE);
}

void propertySetter::setDoubleLengthEditor(){
  m_dlE = new doubleLengthEditor(this);
  connect(m_dlE, SIGNAL(valueChanged(const QString&)), this ,SIGNAL(valueChanged(const QString&)));
  m_list.append(m_dlE);
}

void propertySetter::setFrequencyEditor(){
  m_fe = new frequencyEditor(this);
  connect(m_fe, SIGNAL(valueChanged(const QString&)), this ,SIGNAL(valueChanged(const QString&)));
  m_list.append(m_fe);
}

void propertySetter::setTimeEditor(){
  m_te = new timeEditor(this);
  connect(m_te, SIGNAL(valueChanged(const QString&)), this ,SIGNAL(valueChanged(const QString&)));
  m_list.append(m_te);
}

void propertySetter::setAngleEditor(){
  m_ae = new angleEditor(this);
  connect(m_ae, SIGNAL(valueChanged(const QString&)), this ,SIGNAL(valueChanged(const QString&)));
  m_list.append(m_ae);
}

void propertySetter::setUriEditor(){
  m_ue = new URIEditor(this);
  connect(m_ue, SIGNAL(valueChanged(const QString&)), this ,SIGNAL(valueChanged(const QString&)));
  m_list.append(m_ue);
}

void propertySetter::setDoublePercentageEditor()
{
  m_dpe = new doublePercentageEditor(this);
  connect(m_dpe, SIGNAL(valueChanged(const QString&)), this, SIGNAL(valueChanged(const QString&)));
  m_list.append(m_dpe);
}

void propertySetter::setDoubleComboBoxEditor()
{
  m_dcbe = new doubleComboBoxEditor(this);
  connect(m_dcbe, SIGNAL(valueChanged(const QString&)), this, SIGNAL(valueChanged(const QString&)));
  m_list.append(m_dcbe);
}

void propertySetter::setPercentageEditor()
{
  m_pe = new percentageEditor(this);
  connect(m_pe, SIGNAL(valueChanged(const QString&)), this, SIGNAL(valueChanged(const QString&)));
  m_list.append(m_pe);
}

void propertySetter::setColorRequester()
{
  m_cr = new colorRequester(this);
  connect(m_cr, SIGNAL(textChanged(const QString&)), this, SIGNAL(valueChanged(const QString&)));
  m_list.append(m_cr);
}

void propertySetter::Show(){

  QWidget *w;
  for ( w = m_list.first(); w; w = m_list.next() )  w->hide();
  m_list.at(m_ind)->show();
  if(m_list.count() == 1)
    {
      if(m_pb)
        m_pb->hide();
    }
  else  if(m_ind<m_list.count()-1)
          { m_ind++;
            m_pb->show();
            }
  else m_ind=0;
}

void propertySetter::addButton(){

  m_pb = new KPushButton(this);
  //pb->hide();
  QIconSet iconSet = SmallIconSet(QString::fromLatin1("2rightarrow"));
  QPixmap pixMap = iconSet.pixmap( QIconSet::Small, QIconSet::Normal );
  m_pb->setIconSet(iconSet);
  m_pb->setFixedSize( pixMap.width()+8, pixMap.height()+8 );
  m_pb->hide();
  connect(m_pb, SIGNAL(clicked()), this ,SLOT(Show()));
}

URIEditor::URIEditor(QWidget *parent, const char* name) : QHBox(parent,name)
{
  m_Mode = single;
  m_label = new QLabel(this);
  m_le = new QLineEdit(this);
  m_pb = new KPushButton(this);

  m_label->setText(" Uri :");

  QIconSet iconSet =  SmallIconSet(QString::fromLatin1("fileopen"));
  QPixmap pixMap = iconSet.pixmap( QIconSet::Small, QIconSet::Normal );

  m_pb->setIconSet(iconSet);
  m_pb->setFixedSize( pixMap.width()+8, pixMap.height()+8 );
  setSpacing( KDialog::spacingHint() );

  if( m_Mode == single )
    connect(m_le, SIGNAL(textChanged ( const QString & )), this, SLOT(URI(const QString&)));
  connect(m_pb, SIGNAL(clicked()), this, SLOT(openFileDialog()));
}

URIEditor::~URIEditor(){
  delete m_label;
  delete m_le;
  delete m_pb;
}

void URIEditor::URI(const QString & s)
 {
   QString h=relativize( s , quantaApp->projectBaseURL().path() );
   emit valueChanged("url(\'" + h + "\')");
 }

void URIEditor::openFileDialog(){
  QFileDialog* fd = new QFileDialog( this, "file dialog", TRUE );
  switch(m_resourceType) {
    case image :   fd->setFilter( i18n("Image")+" (*.png *.gif *.jpg)" );break;
    case audio :  fd->setFilter( i18n("Audio")+" (*.au *.aiff *.wav)" );break;
    case mousePointer :   fd->setFilter( i18n("Mouse Pointer")+" (*.cur)" );break;
    default:;
  }

  bool multi=false;
  
  if( m_Mode == single){
    fd->setMode(QFileDialog::ExistingFile);
  }
  else {
    fd->setMode(QFileDialog::ExistingFiles);
    multi=true;
  }
  
  if( fd->exec() == QDialog::Accepted ){
    if( !multi)
      URI( fd->selectedFile() );
    else {
      QStringList selectedFiles = fd->selectedFiles();
      for ( QStringList::Iterator it = selectedFiles.begin(); it != selectedFiles.end(); ++it ) 
        m_sFiles.append( "url(\'" + relativize( (*it), quantaApp->projectBaseURL().path() ) + "\')");
      emit valueChanged(m_sFiles.join(","));
    }
  }
  delete fd;
}

percentageEditor::percentageEditor(QWidget *parent, const char *name) : QHBox(parent,name)
{
  sb = new QSpinBox(0,9999,1,this);
  sb->setSuffix("%");
  connect(sb, SIGNAL(valueChanged ( const QString & )), this, SIGNAL(valueChanged(const QString&)));

}

percentageEditor::~percentageEditor()
{
  delete sb;
}

multipleSpinBox::multipleSpinBox(QWidget *parent, const char* name, int n) : QHBox(parent,name){
  SBList.setAutoDelete(true);
  int i;
  for(i=0;i<n;i++){
    QSpinBox *sb=new QSpinBox(this);
    SBList.append(sb);
    connect(sb, SIGNAL(valueChanged(const QString&)), this, SLOT(valueChangedSlot(const QString&)));
  }
}

multipleSpinBox::~multipleSpinBox(){
  SBList.clear();
}

void multipleSpinBox::setSuffix(const QString& s){
  QSpinBox *sb;
  for ( sb = SBList.first(); sb; sb = SBList.next() )
    sb->setSuffix(s);
}

void multipleSpinBox::valueChangedSlot(const QString& s) {
  QSpinBox *sb;
  for ( sb = SBList.first(); sb; sb = SBList.next() )
    emit valueChanged(s);
}

#include "propertysetter.moc"
