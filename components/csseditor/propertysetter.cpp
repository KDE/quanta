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
#include <qlabel.h>
#include <qtooltip.h>
#include <qregexp.h>
#include <qvbox.h>

#include <kpushbutton.h>
#include <kdebug.h>
#include <kdialog.h>
#include <kiconloader.h>
#include <klocale.h>
#include <klineedit.h>


#include "csseditor_globals.h"
#include "minieditor.h"

propertySetter::propertySetter(QWidget *parent, const char *name ) : QHBox(parent,name) {
  m_ind = 0;
  m_cb = 0L;
  m_list.setAutoDelete(true);
  m_pb = 0L;
  setSpacing( KDialog::spacingHint() );
}

propertySetter::~propertySetter(){
  reset();
}

void propertySetter::reset(){
  if(!m_list.isEmpty()) m_list.clear();
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
  connect(m_cb, SIGNAL(textChanged(const QString&)), this, SIGNAL(valueChanged(const QString&)));
  m_list.append(m_cb);
}

void propertySetter::setSpinBox(const QString& initialValue, const QString& min, const QString& max, const QString& s)
{
  mySpinBox *editor = new mySpinBox(min.toInt(), max.toInt(), 1, this);
  editor->setSuffix(s);
  editor->setValue(initialValue.toInt());
  connect(editor, SIGNAL(valueChanged(const QString&)), this ,SIGNAL(valueChanged(const QString&)));
  m_list.append(editor);
}

void propertySetter::setLineEdit()
{
  QLineEdit *editor = new QLineEdit(this);
  connect(editor,SIGNAL(textChanged ( const QString & )), this, SIGNAL(valueChanged ( const QString & )));
  m_list.append(editor);
}

void propertySetter::setPredefinedColorListEditor()
{  
  QComboBox *editor = new QComboBox(this);
  editor->insertStringList(CSSEditorGlobals::HTMLColors);
  connect(editor, SIGNAL(activated(const QString&)), this, SIGNAL(valueChanged(const QString&)));
  m_list.append(editor);
}

void propertySetter::Show(){
  QWidget *w;
  for ( w = m_list.first(); w; w = m_list.next() )  
    w->hide();
    
  m_list.at(m_ind)->show();
  
  if(m_list.count() == 1) {
    if(m_pb) 
      m_pb->hide();
  }
  else  
    if(m_ind<(m_list.count()-1)) { 
      m_ind++;
      m_pb->show();
    }
    else 
      m_ind=0;
}

void propertySetter::addButton(){

  m_pb = new KPushButton(this);
  QToolTip::add(m_pb, i18n( "More..." ));
  QIconSet iconSet = SmallIconSet(QString::fromLatin1("2rightarrow"));
  QPixmap pixMap = iconSet.pixmap( QIconSet::Small, QIconSet::Normal );
  m_pb->setIconSet(iconSet);
  m_pb->setFixedSize( pixMap.width()+8, pixMap.height()+8 );
  m_pb->hide();
  connect(m_pb, SIGNAL(clicked()), this ,SLOT(Show()));
}
    
void propertySetter::installMiniEditor(miniEditor *m){
  m->connectToPropertySetter(this);
  m_list.append(m);
}

#include "propertysetter.moc"
