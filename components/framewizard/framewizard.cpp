/***************************************************************************
                          framewizard.cpp  -  description
                             -------------------
    begin                : mer giu  4 14:14:07 CEST 2003
    copyright            : (C) |YEAR| by Gu2003Luciano
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

#include "framewizard.h"
#include "fmrceditor.h"
#include "fmfpeditor.h"
#include <kmessagebox.h>
//#include <kdebug.h>
#include <klocale.h>
#include <qpushbutton.h>

#include "fwglobal.h"

static const QString info1=i18n("You must select an area."),
                     info2=i18n("Before editing a frame you must save the file.");

FrameWizard::FrameWizard(QWidget *parent, const char *name) : FrameWizardS(parent, name),
m_hasSelected(false),m_saved(false)
{
  m_hasSelected = false;
  m_currSA=vfe->internalTree()->root()->label();
  connect(this, SIGNAL(launchDraw()), this, SLOT(draw()));
  connect(vfe, SIGNAL(areaSelected(const QString &)), this, SLOT(catchSelectedArea(const QString &)));

  connect(pbHorizontal, SIGNAL(clicked()), this, SLOT(split()));
  connect(pbVertical, SIGNAL(clicked()), this, SLOT(split()));
  connect(pbEditFrame, SIGNAL(clicked()), this, SLOT(showFrameEditorDlg()));
  connect(pbReset, SIGNAL(clicked()), this, SLOT(reset()));
  connect(pbDelete, SIGNAL(clicked()), this, SLOT(remove()));
}

FrameWizard::~FrameWizard(){
#define QT_CHECK_NULL
Q_CHECK_PTR( vfe );
}

void FrameWizard::catchSelectedArea(const QString &id ){
  m_currSA = id; //is the current SelectableArea selected
  m_hasSelected = true;// a SelectableArea has been selected
}

void FrameWizard::split(){
  if(m_hasSelected) {
    int split = 0;
    QString currNodeLabel = m_currSA;
    QString senderName=sender()->name();
    if(senderName=="pbHorizontal"){
      split = showRCeditorDlg(i18n("Enter the desired number of rows:"));
      if(split>=2) vfe->split(currNodeLabel,split,HORIZONTAL);
    }
    else
    if(senderName=="pbVertical"){
      split = showRCeditorDlg(i18n("Enter the desired number of columns:"));
      if(split>=2) vfe->split(currNodeLabel,split,VERTICAL);
    }
    emit launchDraw();
  }
  else KMessageBox::information( this, info1, i18n("Warning") );
  m_hasSelected=false;
}

void FrameWizard::draw(){
  vfe->draw();
}

int FrameWizard::showRCeditorDlg(const QString &s){
  int res = 0;
  fmRCeditor *dlg = new fmRCeditor;
  dlg->setLabelText(s);
  if(dlg->exec()) res = dlg->spinBoxValue();
  delete dlg;
  return res;
}

void FrameWizard::showFrameEditorDlg(){
  if(m_saved){
    if(m_hasSelected) {
      fmFPeditor *dlg = new fmFPeditor();
      dlg->setup(vfe->internalTree()->findAreaAttribute(m_currSA)->attributeMap());
      if(dlg->exec()) {
        vfe->internalTree()->findAreaAttribute(m_currSA)->setAllAttributes(dlg->attributeMap());
        vfe->draw();
      }
    delete dlg;
    }
    else KMessageBox::information( this, info1, i18n("Warning") );
    m_hasSelected=false;
  }
  else  KMessageBox::information( this, info2, i18n("Warning") );
}

void FrameWizard::reset(){
  vfe->internalTree()->reset();
  draw();
}

void FrameWizard::remove(){
  if(m_hasSelected) {
    vfe->removeNode(m_currSA);
    draw();
  }
  else KMessageBox::information( this, info1, i18n("Warning") );
  m_hasSelected=false;
}

#include "framewizard.moc"
