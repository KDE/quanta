/***************************************************************************
                          colorrequester.cpp  -  description
                             -------------------
    copyright            : (C) 2004 by gulmini luciano
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

#include "colorrequester.h"
#include <klineedit.h>
#include <kcombobox.h>
#include <kcolordialog.h>
#include <qtooltip.h>
#include <qiconset.h>
#include <kiconloader.h>
#include <klocale.h>
#include <kaccel.h>

#include "propertysetter.h"

class colorRequester::colorRequesterPrivate{
  public:
    KLineEdit *edit;
    colorRequesterPrivate() { edit = 0L; }
    void setText( const QString& text ) { edit->setText( text ); }
    void connectSignals( QObject *receiver ) { connect( edit, SIGNAL( textChanged( const QString& )),receiver, SIGNAL( textChanged( const QString& ))); }
};

colorRequester::colorRequester(QWidget *parent, const char* name) : miniEditor(parent,name){
  d = new colorRequesterPrivate;
  init();
}

colorRequester::~colorRequester(){
  delete myColorDialog;
  delete d;
}

void colorRequester::connectToPropertySetter(propertySetter* p){
  connect( this, SIGNAL(textChanged(const QString&)), p, SIGNAL(valueChanged(const QString&)));
}

void colorRequester::init()
{
     myColorDialog    = 0L;

     if ( !d->edit )
     d->edit = new KLineEdit( this, "line edit" );

     myButton = new KPushButton( this, "kfile button");
     QIconSet iconSet = SmallIconSet(QString::fromLatin1("colorize"));
     QPixmap pixMap = iconSet.pixmap( QIconSet::Small, QIconSet::Normal );
     myButton->setIconSet( iconSet );
     myButton->setFixedSize( pixMap.width()+8, pixMap.height()+8 );
     QToolTip::add(myButton, i18n("Open color dialog"));

     setSpacing( KDialog::spacingHint() );

     QWidget *widget = (QWidget*) d->edit;
     setFocusProxy( widget );

     d->connectSignals( this );
     connect( myButton, SIGNAL( clicked() ), this, SLOT( openColorDialog() ));
     connect( d->edit, SIGNAL( textChanged ( const QString & ) ), this, SLOT( setInitialValue(/*const QString&*/ ) ));

     KAccel *accel = new KAccel( this );
     accel->insert( KStdAccel::Open, this, SLOT( openColorDialog() ));
     accel->readSettings();
 }

void colorRequester::openColorDialog(){
 KColorDialog dlg(this,"dlg",true);
 dlg.setColor(QColor(m_initialValue));
 if(dlg.exec()){
   QColor myColor(dlg.color());
   d->edit->setText(myColor.name());
   emit textChanged(myColor.name());
 }
}

KLineEdit * colorRequester::lineEdit() const{
  return d->edit;
}
#include <kdebug.h>
void colorRequester::setInitialValue(/*const QString& s*/){
  QString temp = d->edit->text();
  temp.remove(" ");
 if( temp.contains("#") != 0){
   temp.remove("#");
   if(temp.length() == 3) {
     QString temp2;
     temp2.append(temp[0]);
     temp2.append(temp[0]);
     temp2.append(temp[1]);
     temp2.append(temp[1]);
     temp2.append(temp[2]);
     temp2.append(temp[2]);
     temp = temp2;
   }
   bool ok;
   int r = temp.left(2).toInt( &ok, 16 );
   int g = temp.mid(2,2).toInt( &ok, 16 );
   int b = temp.right(2).toInt( &ok, 16 );
   m_initialValue.setRgb(r,g,b);
 }
 else

   if( temp.contains("rgb(") != 0){
     temp.remove("rgb(").remove(")");
     QStringList rgbValues = QStringList::split(",",temp);
//      bool ok;
     int r = rgbValues[0].toInt();
     int g = rgbValues[1].toInt();
     int b = rgbValues[2].toInt();
     m_initialValue.setRgb(r,g,b);
   }
   else
      m_initialValue.setNamedColor(d->edit->text());
}

#include "colorrequester.moc"
