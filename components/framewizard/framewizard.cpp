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
#include <kdebug.h>
#include <klocale.h>
#include <qstringlist.h>

static const QString info1=i18n("You must select an area!"),
		     info2=i18n("Before editing a frame you must save the file!");
		     
FrameWizard::FrameWizard(QWidget *parent, const char *name) : FrameWizardS(parent, name),
saved(false),framesetFileCurrentPath(QString::null) 
{
  hasSelected = false;
  currSA=vfe->initSA();
  connect(this, SIGNAL(launchDraw()), this, SLOT(draw()));
  connect(vfe, SIGNAL(areaSelected(QString)), this, SLOT(catchSelectedArea(QString)));
}

FrameWizard::~FrameWizard()
{
}

void FrameWizard::catchSelectedArea( QString id )
{
    currSA = id; //is the current SelectableArea selected
    hasSelected = true;// a SelectableArea has been selected
}

void FrameWizard::split()
{
    if(hasSelected) {
	int split = 0;
	QString currNodeLabel = currSA;
	QString senderName=sender()->name();
	if(senderName=="pbHorizontal"){
	    split = showRCeditorDlg(i18n("Enter the desired number of rows"));
	    if(split>=2)
		   vfe->split(currNodeLabel,split,"h");
	}
	else if(senderName=="pbVertical"){
	    split = showRCeditorDlg(i18n("Enter the desired number of columns"));
	    if(split>=2)
		vfe->split(currNodeLabel,split,"v");
	}
	emit launchDraw();
    }
    else KMessageBox::information( this, info1, i18n("Warning") );
    hasSelected=false;
}

void FrameWizard::draw()
{
    vfe->draw();
}


int FrameWizard::showRCeditorDlg( QString s)
{
    int res = 0;
    fmRCeditor *dlg = new fmRCeditor;
    dlg->setLabelText(s);
    if(dlg->exec()) {
	res = dlg->getSpinBoxValue();
    }
    delete dlg;
    return res;
}


void FrameWizard::showFrameEditorDlg()
{
  if(saved){
    if(hasSelected) {
	fmFPeditor *dlg = new fmFPeditor();
	dlg->setup(vfe->getAttributeMap(currSA));
	if(dlg->exec()) {
	    vfe->setAllAttributes(currSA,dlg->getAttributeMap());
	    vfe->draw();
	}
	delete dlg;
    }
    else KMessageBox::information( this, info1, i18n("Warning") );
    hasSelected=false;
    }
  else  KMessageBox::information( this, info2, i18n("Warning") ); 
}

void FrameWizard::reset(){
    vfe->reset();
    draw();
}
    
void FrameWizard::remove()
{
    if(hasSelected) {
	   vfe->removeNode(currSA);
	   draw();
      }
    else KMessageBox::information( this, info1, i18n("Warning") );
    hasSelected=false;
}


#include "framewizard.moc"

