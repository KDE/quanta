/****************************************************************************
** Form implementation generated from reading ui file 'project-local.ui'
**
** Created: Sat Oct 21 03:14:46 2000
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "projectnewlocals.h"

#include <qcheckbox.h>
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
 *  Constructs a ProjectNewLocalS which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 */
ProjectNewLocalS::ProjectNewLocalS( QWidget* parent,  const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "ProjectNewLocalS" );
    resize( 435, 461 ); 
    setCaption( tr( "Form3"  ) );
    grid = new QGridLayout( this ); 
    grid->setSpacing( 6 );
    grid->setMargin( 11 );

    checkInsert = new QCheckBox( this, "checkInsert" );
    checkInsert->setText( tr( "Insert files from"  ) );

    grid->addWidget( checkInsert, 0, 0 );

    checkInsertWithMask = new QCheckBox( this, "checkInsertWithMask" );
    checkInsertWithMask->setText( tr( "Insert files with the specific file mask"  ) );

    grid->addWidget( checkInsertWithMask, 3, 0 );

    mask = new QLineEdit( this, "mask" );
    mask->setEnabled( FALSE );

    grid->addWidget( mask, 4, 0 );

    webmask = new QLineEdit( this, "webmask" );
    webmask->setEnabled( FALSE );

    grid->addWidget( webmask, 2, 0 );

    TextLabel8 = new QLabel( this, "TextLabel8" );
    TextLabel8->setText( tr( "Included files:"  ) );

    grid->addWidget( TextLabel8, 6, 0 );

    checkInsertWeb = new QCheckBox( this, "checkInsertWeb" );
    checkInsertWeb->setText( tr( "Insert only web files and images"  ) );

    grid->addWidget( checkInsertWeb, 1, 0 );

    listFiles = new QListView( this, "listFiles" );
    listFiles->addColumn( tr( "File name" ) );
    listFiles->addColumn( tr( "Size" ) );

    grid->addWidget( listFiles, 7, 0 );
    QSpacerItem* spacer = new QSpacerItem( 20, 30, QSizePolicy::Minimum, QSizePolicy::Fixed );
    grid->addItem( spacer, 5, 0 );

    // signals and slots connections
    connect( checkInsertWithMask, SIGNAL( toggled(bool) ), mask, SLOT( setEnabled(bool) ) );
    connect( checkInsertWeb, SIGNAL( toggled(bool) ), webmask, SLOT( setEnabled(bool) ) );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
ProjectNewLocalS::~ProjectNewLocalS()
{
    // no need to delete child widgets, Qt does it all for us
}

