#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file './copytos.ui'
**
** Created: Mon Sep 23 15:59:34 2002
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "copytos.h"

#include <qvariant.h>
#include <kurlrequester.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/* 
 *  Constructs a CopyToS which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
CopyToS::CopyToS( QWidget* parent,  const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "CopyToS" );
    resize( 376, 84 ); 
    setCaption( tr2i18n( "Files: Copy to Project" ) );
    CopyToSLayout = new QGridLayout( this, 1, 1, 11, 6, "CopyToSLayout"); 

    buttonCancel = new QPushButton( this, "buttonCancel" );
    buttonCancel->setText( tr2i18n( "&Cancel" ) );

    CopyToSLayout->addWidget( buttonCancel, 1, 2 );

    buttonOk = new QPushButton( this, "buttonOk" );
    buttonOk->setText( tr2i18n( "&OK" ) );
    buttonOk->setDefault( TRUE );

    CopyToSLayout->addWidget( buttonOk, 1, 1 );
    QSpacerItem* spacer = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
    CopyToSLayout->addItem( spacer, 1, 0 );

    urlRequester = new KURLRequester( this, "urlRequester" );

    CopyToSLayout->addMultiCellWidget( urlRequester, 0, 0, 0, 2 );

    // tab order
    setTabOrder( buttonOk, buttonCancel );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
CopyToS::~CopyToS()
{
    // no need to delete child widgets, Qt does it all for us
}

#include "copytos.moc"
