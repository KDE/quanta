/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/
#include "fmrceditor.h"
#include "fmfpeditor.h"
#include <qmessagebox.h>




void FrameWizard::init()
{
    splitted=false;
    vfe->setupForm(this);
    vfe->draw();
    currSTB=vfe->initSTB();
    hasSelected = false;
    connect(this, SIGNAL(launchDraw()), this, SLOT(draw()));
}

void FrameWizard::catchSelectedArea( QString id )
{
    currSTB = id; //is the current SelectableTextBrowser selected
    hasSelected = true;
}

void FrameWizard::split()
{
    if(hasSelected) {
	int split = 0;
	QString currNodeLabel = currSTB;
	QString senderName=sender()->name();
	if(senderName=="pbHorizontal"){
	    split = showRCeditorDlg("Enter the desidered number of rows");
	    if(split>=2)
		vfe->split(currNodeLabel,split,"h");
	    }
	else if(senderName=="pbVertical"){
	    split = showRCeditorDlg("Enter the desidered number of columns");
	    if(split>=2)
		vfe->split(currNodeLabel,split,"v");
	}
	emit launchDraw();
    }
     else QMessageBox::warning( this, "Warning","You must select an area!" , "Ok");
     hasSelected=false;
     splitted=true;
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
    if(hasSelected) {
	fmFPeditor *dlg = new fmFPeditor;
	dlg->setup(vfe->getAttributeMap(currSTB));
	if(dlg->exec()) {
	    vfe->setAllAttribute(currSTB,dlg->getAttributeMap());
	    vfe->draw();
	}
	delete dlg;
    }
    else QMessageBox::warning( this, "Warning","You must select an area!" , "Ok");
    hasSelected=false;
}

void FrameWizard::reset(){
    vfe->reset();
    splitted=false;
    draw();
}

void FrameWizard::generate(){
    if(splitted) {
	vfe->framesetStructure();}
    else{
	QMessageBox::warning( this, "Warning","No structure will be created!" , "Ok");
    }
}

void FrameWizard::remove()
{
    if(hasSelected) {
	vfe->removeNode(currSTB);
	draw();
    }
    else QMessageBox::warning( this, "Warning","You must select an area!" , "Ok");
     hasSelected=false;
}
