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

#define QT_CHECK_NULL


propertySetter::propertySetter(QWidget *parent, const char *name ) : QHBox(parent,name) {
  ind = 0;
  cb = 0L;
  le = 0L;
  sb = 0L;
  lE = 0L;
  dlE = 0L;
  dpe = 0L;
  dcbe = 0L;
  pe = 0L;
  ue = 0L;
  fe = 0L;
  te = 0L;
  ae = 0L;
  cr = 0L;
  pb = 0L;
  list.setAutoDelete(true);
  setSpacing( KDialog::spacingHint() );

}
propertySetter::~propertySetter(){
  /*list.clear();
  if(pb) delete pb;
  pb=0;*/
  reset();
}

void propertySetter::reset(){
  if(!list.isEmpty())
    list.clear();
 /* if(cb) {
    delete cb;
    cb=0;
  }
  if(le) {
    delete le;
    le=0;
  }
  if(sb) {
    delete sb;
    sb=0;
  }
  if(lE) {
    delete lE;
    lE=0;
  }
  if(dlE) {
    delete dlE;
    dlE=0;
  }
  if(dpe) {
    delete dpe;
    dpe=0;
  }
  if(dcbe) {
    delete dcbe;
    dcbe=0;
  }
  if(pe) {
    delete pe;
    pe=0;
  }
  if(ue) {
    delete ue;
    ue=0;
  }
  if(fe) {
    delete fe;
    fe=0;
  }
  if(te) {
    delete te;
    te=0;
  }
  if(ae) {
    delete ae;
    ae=0;
  }
  if(cr) {
    delete cr;
    cr=0;
  }
*/
  if(pb) {
    delete pb;
    pb=0;
   }

  ind=0;

}

void propertySetter::setComboBox()
{
  cb = new QComboBox(this);
  Q_CHECK_PTR( cb );
  connect(cb, SIGNAL(activated(const QString&)), this, SIGNAL(valueChanged(const QString&)));
  list.append(cb);
}

void propertySetter::setSpinBox(const QString& min, const QString& max, const QString& s)
{
  sb = new QSpinBox(min.toInt(), max.toInt(), 1, this);
  Q_CHECK_PTR( sb );
  sb->setSuffix(s);
  sb->setValue(0);
  connect(sb, SIGNAL(valueChanged(const QString&)), this ,SIGNAL(valueChanged(const QString&)));
  list.append(sb);
}

void propertySetter::setLineEdit()
{
  le = new QLineEdit(this);
  Q_CHECK_PTR( le );
  connect(le,SIGNAL(textChanged ( const QString & )), this, SIGNAL(valueChanged ( const QString & )));
  list.append(le);
}

void propertySetter::setLengthEditor(){
  lE = new lengthEditor(this);
  Q_CHECK_PTR( lE );
  connect(lE, SIGNAL(valueChanged(const QString&)), this ,SIGNAL(valueChanged(const QString&)));
  list.append(lE);
}

void propertySetter::setDoubleLengthEditor(){
  dlE = new doubleLengthEditor(this);
  Q_CHECK_PTR( dlE );
  connect(dlE, SIGNAL(valueChanged(const QString&)), this ,SIGNAL(valueChanged(const QString&)));
  list.append(dlE);
}

void propertySetter::setFrequencyEditor(){
  fe = new frequencyEditor(this);
  Q_CHECK_PTR( fe );
  connect(fe, SIGNAL(valueChanged(const QString&)), this ,SIGNAL(valueChanged(const QString&)));
  list.append(fe);
}

void propertySetter::setTimeEditor(){
  te = new timeEditor(this);
  Q_CHECK_PTR( te );
  connect(te, SIGNAL(valueChanged(const QString&)), this ,SIGNAL(valueChanged(const QString&)));
  list.append(te);
}

void propertySetter::setAngleEditor(){
  ae = new angleEditor(this);
  Q_CHECK_PTR( ae );
  connect(ae, SIGNAL(valueChanged(const QString&)), this ,SIGNAL(valueChanged(const QString&)));
  list.append(ae);
}

void propertySetter::setURIEditor(){
  ue = new URIEditor(this);
  Q_CHECK_PTR( ue );
  connect(ue, SIGNAL(valueChanged(const QString&)), this ,SIGNAL(valueChanged(const QString&)));
  list.append(ue);
}

void propertySetter::setDoublePercentageEditor()
{
  dpe = new doublePercentageEditor(this);
  Q_CHECK_PTR( dpe );
  connect(dpe, SIGNAL(valueChanged(const QString&)), this, SIGNAL(valueChanged(const QString&)));
  list.append(dpe);
}

void propertySetter::setDoubleComboBoxEditor()
{
  dcbe = new doubleComboBoxEditor(this);
  Q_CHECK_PTR( dcbe );
  connect(dcbe, SIGNAL(valueChanged(const QString&)), this, SIGNAL(valueChanged(const QString&)));
  list.append(dcbe);
}

void propertySetter::setPercentageEditor()
{
  pe = new percentageEditor(this);
  Q_CHECK_PTR( pe );
  connect(pe, SIGNAL(valueChanged(const QString&)), this, SIGNAL(valueChanged(const QString&)));
  list.append(pe);
}

void propertySetter::setColorRequester()
{
  cr = new colorRequester(this);
  Q_CHECK_PTR( cr );
  connect(cr, SIGNAL(textChanged(const QString&)), this, SIGNAL(valueChanged(const QString&)));
  list.append(cr);
}

void propertySetter::Show(){

  QWidget *w;
  for ( w = list.first(); w; w = list.next() )  w->hide();
  list.at(ind)->show();
  if(list.count() == 1)
    {
      if(pb)
        pb->hide();
    }
  else  if(ind<list.count()-1)
          { ind++;
            pb->show();
            }
  else ind=0;

}

void propertySetter::addButton(){

  pb = new KPushButton(this);
  Q_CHECK_PTR( pb );
  pb->hide();
  QString arrow = locate("appdata","csseditor/data/ar.png");
//  QIconSet iconSet(QPixmap(arrow));
//  QPixmap pixMap = iconSet.pixmap( QIconSet::Small, QIconSet::Normal );

//  pb->setIconSet(iconSet);
  QPixmap pixMap(arrow);
  pb->setPixmap(pixMap);
  pb->setFixedSize( pixMap.width()+8, pixMap.height()+8 );
  connect(pb, SIGNAL(clicked()), this ,SLOT(Show()));
}

void propertySetter::analyze(const QString& s){


}


URIEditor::URIEditor(QWidget *parent, const char* name) : QHBox(parent,name)
{
  Mode = single;
  label = new QLabel(this);
  le = new QLineEdit(this);
  pb = new KPushButton(this);

  label->setText(" Uri :");

  QIconSet iconSet =  SmallIconSet(QString::fromLatin1("fileopen"));
  QPixmap pixMap = iconSet.pixmap( QIconSet::Small, QIconSet::Normal );

  pb->setIconSet(iconSet);
  pb->setFixedSize( pixMap.width()+8, pixMap.height()+8 );
  setSpacing( KDialog::spacingHint() );

  if( Mode == single )
    connect(le, SIGNAL(textChanged ( const QString & )), this, SLOT(URI(const QString&)));
  connect(pb, SIGNAL(clicked()), this, SLOT(openFileDialog()));
}

URIEditor::~URIEditor(){
  if(le) delete le;
  le = 0;
  if(pb) delete pb;
  pb=0;

}

void URIEditor::URI(const QString & s)
 {
   emit valueChanged("url(\"" + s + "\")");
 }

void URIEditor::openFileDialog(){
  QFileDialog* fd = new QFileDialog( this, "file dialog", TRUE );
  bool multi=false;

  if( Mode == single){
        fd->setMode(QFileDialog::ExistingFile);
       }
  else {
    fd->setMode(QFileDialog::ExistingFiles);
    multi=true;
    }

  if( fd->exec() == QDialog::Accepted )
    {
      if( !multi){
        emit valueChanged("url(\"" + fd->selectedFile() + "\")");
      }
      else {
        QStringList selectedFiles = fd->selectedFiles();

        for ( QStringList::Iterator it = selectedFiles.begin(); it != selectedFiles.end(); ++it ) {
          sFiles.append("url(\"" + *it + "\")") ;
        }
        emit valueChanged(sFiles.join(","));

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
  if(sb) delete sb;
  sb=0;
}

/*comboBoxEditor::comboBoxEditor(QWidget *parent, const char* name) : QHbox(parent,name){
  cb = new QComboBox(this);
}
comboBoxEditor::~comboBoxEditor();  */

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
