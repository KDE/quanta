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
#include <kmessagebox.h>

const static QString info1="You must select an area!",
                     info2="No structure will be created!";
void FrameWizard::init()
{   
    splitted=false;
    vfe->setupForm(this);
    vfe->draw();
    currSA=vfe->initSA();
    hasSelected = false;   
    connect(this, SIGNAL(launchDraw()), this, SLOT(draw()));
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
     else KMessageBox::information( this, info1, "Warning" );
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
	dlg->setup(vfe->getAttributeMap(currSA));
	if(dlg->exec()) {
	    vfe->setAllAttribute(currSA,dlg->getAttributeMap());
	    vfe->draw();
	}
	delete dlg;
    }
    else KMessageBox::information( this, info1, "Warning" );
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
    else KMessageBox::information( this, info2, "Warning" );
}

void FrameWizard::remove()
{
    if(hasSelected) {
	vfe->removeNode(currSA);
	draw();
    }
    else KMessageBox::information( this, info1, "Warning" );
    hasSelected=false;
}



