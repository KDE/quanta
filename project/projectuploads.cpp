/****************************************************************************
** Form implementation generated from reading ui file 'project-upload.ui'
**
** Created: Mon Nov 6 03:51:50 2000
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "projectuploads.h"

#include <qheader.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qlistview.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/* 
 *  Constructs a ProjectUploadS which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
ProjectUploadS::ProjectUploadS( QWidget* parent,  const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "ProjectUploadS" );
    resize( 426, 453 ); 
    setCaption( tr( "MyDialog"  ) );
    setSizeGripEnabled( TRUE );
    grid = new QGridLayout( this ); 
    grid->setSpacing( 6 );
    grid->setMargin( 11 );

    vbox = new QVBoxLayout; 
    vbox->setSpacing( 6 );
    vbox->setMargin( 0 );

    buttonOk = new QPushButton( this, "buttonOk" );
    buttonOk->setText( tr( "&OK"  ) );
    buttonOk->setAutoDefault( TRUE );
    buttonOk->setDefault( TRUE );
    vbox->addWidget( buttonOk );

    buttonCancel = new QPushButton( this, "buttonCancel" );
    buttonCancel->setText( tr( "&Cancel"  ) );
    buttonCancel->setAutoDefault( TRUE );
    vbox->addWidget( buttonCancel );

    buttonHelp = new QPushButton( this, "buttonHelp" );
    buttonHelp->setText( tr( "&Help"  ) );
    buttonHelp->setAutoDefault( TRUE );
    vbox->addWidget( buttonHelp );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    vbox->addItem( spacer );

    grid->addMultiCellLayout( vbox, 0, 3, 4, 4 );

    TextLabel4 = new QLabel( this, "TextLabel4" );
    TextLabel4->setText( tr( "Server URL"  ) );

    grid->addMultiCellWidget( TextLabel4, 0, 0, 0, 1 );

    TextLabel2 = new QLabel( this, "TextLabel2" );
    TextLabel2->setText( tr( "User"  ) );

    grid->addMultiCellWidget( TextLabel2, 1, 1, 0, 1 );

    TextLabel3 = new QLabel( this, "TextLabel3" );
    TextLabel3->setText( tr( "Password"  ) );

    grid->addMultiCellWidget( TextLabel3, 2, 2, 0, 1 );

    list = new QListView( this, "list" );
    list->addColumn( tr( "File name" ) );
    list->addColumn( tr( "Date" ) );
    list->addColumn( tr( "Size" ) );

    grid->addMultiCellWidget( list, 3, 3, 0, 3 );

    linePasswd = new QLineEdit( this, "linePasswd" );
    linePasswd->setEchoMode( QLineEdit::Password );

    grid->addMultiCellWidget( linePasswd, 2, 2, 2, 3 );

    lineUser = new QLineEdit( this, "lineUser" );

    grid->addMultiCellWidget( lineUser, 1, 1, 2, 3 );

    lineUrl = new QLineEdit( this, "lineUrl" );

    grid->addMultiCellWidget( lineUrl, 0, 0, 2, 3 );

    buttonAll = new QPushButton( this, "buttonAll" );
    buttonAll->setText( tr( "All"  ) );

    grid->addWidget( buttonAll, 4, 0 );

    buttonModified = new QPushButton( this, "buttonModified" );
    buttonModified->setText( tr( "Modified"  ) );

    grid->addMultiCellWidget( buttonModified, 4, 4, 1, 2 );

    buttonClear = new QPushButton( this, "buttonClear" );
    buttonClear->setText( tr( "Clear"  ) );

    grid->addWidget( buttonClear, 4, 3 );

    // signals and slots connections
    connect( buttonOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
ProjectUploadS::~ProjectUploadS()
{
    // no need to delete child widgets, Qt does it all for us
}

