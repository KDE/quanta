/****************************************************************************
** Form implementation generated from reading ui file 'copytos.ui'
**
** Created: Sat Oct 21 03:26:44 2000
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "copytos.h"

#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/* 
 *  Constructs a CopyToS which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
CopyToS::CopyToS( QWidget* parent,  const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "CopyToS" );
    resize( 384, 81 ); 
    setCaption( tr( "Form1"  ) );
    grid = new QGridLayout( this ); 
    grid->setSpacing( 6 );
    grid->setMargin( 11 );

    buttonCancel = new QPushButton( this, "buttonCancel" );
    buttonCancel->setText( tr( "Cancel"  ) );

    grid->addMultiCellWidget( buttonCancel, 1, 1, 2, 3 );

    buttonOk = new QPushButton( this, "buttonOk" );
    buttonOk->setText( tr( "Ok"  ) );

    grid->addWidget( buttonOk, 1, 1 );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    grid->addItem( spacer, 1, 0 );

    buttonDir = new QPushButton( this, "buttonDir" );
    buttonDir->setText( tr( "..."  ) );
    buttonDir->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, buttonDir->sizePolicy().hasHeightForWidth() ) );
    buttonDir->setMaximumSize( QSize( 27, 27 ) );

    grid->addWidget( buttonDir, 0, 3 );

    lineDir = new QLineEdit( this, "lineDir" );

    grid->addMultiCellWidget( lineDir, 0, 0, 0, 2 );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
CopyToS::~CopyToS()
{
    // no need to delete child widgets, Qt does it all for us
}

