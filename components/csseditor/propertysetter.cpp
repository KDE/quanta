/***************************************************************************
                          propertysetter.cpp  -  description
                             -------------------
    begin                : gio lug 24 2003
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

#include "propertysetter.h"

#include <qlineedit.h>
#include <qcombobox.h>
#include <qspinbox.h>
//#include <qiconset.h>
//#include <qpixmap.h>
#include <qptrlist.h>
#include <qfiledialog.h>
#include <qlabel.h>
#include <qfontdatabase.h> 
#include <qtooltip.h>
#include <qregexp.h>

#include <kpushbutton.h>
#include <kstandarddirs.h>
#include <kdebug.h>
#include <kurl.h>
#include <kdialog.h>
#include <kiconloader.h>
#include <klocale.h>

#include "qextfileinfo.h"
#include "resource.h"
#include "quanta.h"
#include "fontfamilychooser.h"
#include "colorrequester.h"
#include "specialsb.h"
#include "csseditor_globals.h"



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
  m_pcb = 0L;
  m_ftE = 0L;
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

void propertySetter::setFontEditor()
{
  m_ftE = new fontEditor(this);
  connect(m_ftE, SIGNAL(valueChanged(const QString&)), this, SIGNAL(valueChanged(const QString&)));
  m_list.append(m_ftE);
}

void propertySetter::setComboBox()
{
  m_cb = new QComboBox(this);
  connect(m_cb, SIGNAL(activated(const QString&)), this, SIGNAL(valueChanged(const QString&)));
  m_list.append(m_cb);
}

void propertySetter::setSpinBox(const QString& initialValue, const QString& min, const QString& max, const QString& s)
{
  m_sb = new mySpinBox(min.toInt(), max.toInt(), 1, this);
  m_sb->setSuffix(s);
  m_sb->setValue(initialValue.toInt());
  connect(m_sb, SIGNAL(valueChanged(const QString&)), this ,SIGNAL(valueChanged(const QString&)));
  m_list.append(m_sb);
}

void propertySetter::setLineEdit()
{
  m_le = new QLineEdit(this);
  connect(m_le,SIGNAL(textChanged ( const QString & )), this, SIGNAL(valueChanged ( const QString & )));
  m_list.append(m_le);
}

void propertySetter::setLengthEditor(QString s){
  m_lE = new lengthEditor(this);
  QRegExp pattern("\\d"+m_lE->cbValueList().join("|"));
  
  if(s.contains(pattern)) {
    QString temp1(s.stripWhiteSpace()),
                 temp2(s.stripWhiteSpace());
  
    m_lE->setInitialValue(temp1.remove(QRegExp("\\D")), temp2.remove(QRegExp("\\d")));
  }
  connect(m_lE, SIGNAL(valueChanged(const QString&)), this ,SIGNAL(valueChanged(const QString&)));
  m_list.append(m_lE);
}

void propertySetter::setDoubleLengthEditor(QString s){
  QString temp(s.simplifyWhiteSpace()),
               sx(temp.section(" ",0,0)),
               dx(temp.section(" ",1,1));

  m_dlE = new doubleLengthEditor(this);
  m_dlE->setInitialValue(sx,dx);
  connect(m_dlE, SIGNAL(valueChanged(const QString&)), this ,SIGNAL(valueChanged(const QString&)));
  m_list.append(m_dlE);
}

void propertySetter::setFrequencyEditor(QString s){
  m_fe = new frequencyEditor(this);
  QRegExp pattern("\\d"+m_fe->cbValueList().join("|"));
  
  if(s.contains(pattern)) {
    QString temp1(s.stripWhiteSpace()),
                 temp2(s.stripWhiteSpace());
  
    m_fe->setInitialValue(temp1.remove(QRegExp("\\D")), temp2.remove(QRegExp("\\d")));
  }
  connect(m_fe, SIGNAL(valueChanged(const QString&)), this ,SIGNAL(valueChanged(const QString&)));
  m_list.append(m_fe);
}

void propertySetter::setTimeEditor(QString s){
  m_te = new timeEditor(this);
  QRegExp pattern("\\d"+m_te->cbValueList().join("|"));
  
  if(s.contains(pattern)) {
    QString temp1(s.stripWhiteSpace()),
                 temp2(s.stripWhiteSpace());
  
    m_te->setInitialValue(temp1.remove(QRegExp("\\D")), temp2.remove(QRegExp("\\d")));
  }
  connect(m_te, SIGNAL(valueChanged(const QString&)), this ,SIGNAL(valueChanged(const QString&)));
  m_list.append(m_te);
}

void propertySetter::setAngleEditor(QString s){
  m_ae = new angleEditor(this);
  QRegExp pattern("\\d"+m_ae->cbValueList().join("|"));
  
  if(s.contains(pattern)) {
    QString temp1(s.stripWhiteSpace()),
                 temp2(s.stripWhiteSpace());
  
    m_ae->setInitialValue(temp1.remove(QRegExp("\\D")), temp2.remove(QRegExp("\\d")));
  }
  connect(m_ae, SIGNAL(valueChanged(const QString&)), this ,SIGNAL(valueChanged(const QString&)));
  m_list.append(m_ae);
}

void propertySetter::setUriEditor(){
  m_ue = new URIEditor(this);
  connect(m_ue, SIGNAL(valueChanged(const QString&)), this ,SIGNAL(valueChanged(const QString&)));
  m_list.append(m_ue);
}

void propertySetter::setDoublePercentageEditor(QString s)
{
  QString temp(s.simplifyWhiteSpace()),
               sx(temp.section(" ",0,0)),
               dx(temp.section(" ",1,1));
  m_dpe = new doublePercentageEditor(this);
  m_dpe->setInitialValue(sx,dx);
  connect(m_dpe, SIGNAL(valueChanged(const QString&)), this, SIGNAL(valueChanged(const QString&)));
  m_list.append(m_dpe);
}

void propertySetter::setDoubleComboBoxEditor()
{
  m_dcbe = new doubleComboBoxEditor(this);
  connect(m_dcbe, SIGNAL(valueChanged(const QString&)), this, SIGNAL(valueChanged(const QString&)));
  m_list.append(m_dcbe);
}

void propertySetter::setPercentageEditor(QString s)
{
  m_pe = new percentageEditor(s, this);
  connect(m_pe, SIGNAL(valueChanged(const QString&)), this, SIGNAL(valueChanged(const QString&)));
  m_list.append(m_pe);
}

void propertySetter::setColorRequester()
{
  m_cr = new colorRequester(this);
  connect(m_cr, SIGNAL(textChanged(const QString&)), this, SIGNAL(valueChanged(const QString&)));
  m_list.append(m_cr);
}

void propertySetter::setPredefinedColorListEditor()
{
  m_pcb = new QComboBox(this);
  m_pcb->insertStringList(QStringList::split(",",HTMLColors));
  connect(m_pcb, SIGNAL(activated(const QString&)), this, SIGNAL(valueChanged(const QString&)));
  m_list.append(m_pcb);
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
  QToolTip::add(m_pb, i18n( "More..." ));
  //pb->hide();
  QIconSet iconSet = SmallIconSet(QString::fromLatin1("2rightarrow"));
  QPixmap pixMap = iconSet.pixmap( QIconSet::Small, QIconSet::Normal );
  m_pb->setIconSet(iconSet);
  m_pb->setFixedSize( pixMap.width()+8, pixMap.height()+8 );
  m_pb->hide();
  connect(m_pb, SIGNAL(clicked()), this ,SLOT(Show()));
}


     
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
  QToolTip::add(m_pb, i18n( s ));
}

URIEditor::URIEditor(QWidget *parent, const char* name) : TLPEditor(parent,name)
{
  m_Mode = single;
  setLabelText(" Uri  :");
  setButtonIcon("fileopen");
  setToolTip("Open the URI selector");

  if( m_Mode == single )
    connect(m_le, SIGNAL(textChanged ( const QString & )), this, SLOT(URI(const QString&)));
  connect(m_pb, SIGNAL(clicked()), this, SLOT(openFileDialog()));  
}

void URIEditor::URI(const QString & s)
 {
   KURL u;
   u.setPath(s);
   emit valueChanged("url(\'" + QExtFileInfo::toRelative(u, quantaApp->projectBaseURL()).path() + "\')");
 }

void URIEditor::openFileDialog(){
  QFileDialog* fd = new QFileDialog( this, "file dialog", TRUE );
  switch(m_resourceType) {
    case image :   fd->setFilter( i18n("Image")+" (*.png *.gif *.jpg *.mng)" );break;
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

percentageEditor::percentageEditor(const QString& initialValue, QWidget *parent, const char *name) : QHBox(parent,name)
{
  QString temp(initialValue);
  m_sb = new mySpinBox(0,9999,1,this);
  m_sb->setValue(temp.remove("%").toInt());
  m_sb->setSuffix("%");
  connect(m_sb, SIGNAL(valueChanged ( const QString & )), this, SIGNAL(valueChanged(const QString&)));
}

percentageEditor::~percentageEditor()
{
  delete m_sb;
}

fontEditor::fontEditor(QWidget *parent, const char* name) : TLPEditor(parent,name)
{
  setLabelText(" Font  :");
  setButtonIcon("fonts");
  setToolTip("Open font family chooser");
  
  connect(m_pb, SIGNAL(clicked()), this, SLOT(openFontChooser()));
  connect(m_le, SIGNAL(textChanged ( const QString & )), this, SIGNAL( valueChanged( const QString& ) ) );
}

void fontEditor::openFontChooser(){
  fontFamilyChooser *dlg = new fontFamilyChooser( this );
  
  if( dlg->exec() == QDialog::Accepted ){
    emit valueChanged( dlg->fontList().join(", "));
  }
  delete dlg;
}

#include "propertysetter.moc"
