/****************************************************************************
** Form implementation generated from reading ui file 'project-final.ui'
**
** Created: Sat Nov 4 01:22:42 2000
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "projectnewfinals.h"

#include <qcheckbox.h>
#include <qframe.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/* 
 *  Constructs a ProjectNewFinalS which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 */
ProjectNewFinalS::ProjectNewFinalS( QWidget* parent,  const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "ProjectNewFinalS" );
    resize( 329, 342 ); 
    setCaption( tr( "Form1"  ) );
    grid = new QGridLayout( this ); 
    grid->setSpacing( 6 );
    grid->setMargin( 11 );

    lineUrl = new QLineEdit( this, "lineUrl" );

    grid->addMultiCellWidget( lineUrl, 1, 1, 1, 2 );

    TextLabel3 = new QLabel( this, "TextLabel3" );
    TextLabel3->setText( tr( "Server url"  ) );

    grid->addWidget( TextLabel3, 1, 0 );

    TextLabel4 = new QLabel( this, "TextLabel4" );
    TextLabel4->setText( tr( "User name"  ) );

    grid->addWidget( TextLabel4, 2, 0 );

    TextLabel5 = new QLabel( this, "TextLabel5" );
    TextLabel5->setText( tr( "Password"  ) );

    grid->addWidget( TextLabel5, 3, 0 );

    lineUser = new QLineEdit( this, "lineUser" );

    grid->addMultiCellWidget( lineUser, 2, 2, 1, 2 );

    linePasswd = new QLineEdit( this, "linePasswd" );

    grid->addMultiCellWidget( linePasswd, 3, 3, 1, 2 );

    Line1 = new QFrame( this, "Line1" );
    Line1->setFrameStyle( QFrame::HLine | QFrame::Sunken );

    grid->addMultiCellWidget( Line1, 4, 4, 0, 2 );

    TextLabel6 = new QLabel( this, "TextLabel6" );
    TextLabel6->setText( tr( "Preview prefix"  ) );

    grid->addWidget( TextLabel6, 6, 0 );

    linePrefix = new QLineEdit( this, "linePrefix" );
    linePrefix->setEnabled( FALSE );

    grid->addMultiCellWidget( linePrefix, 6, 6, 1, 2 );

    checkPublishing = new QCheckBox( this, "checkPublishing" );
    checkPublishing->setText( tr( "Project publishing"  ) );
    checkPublishing->setChecked( TRUE );

    grid->addWidget( checkPublishing, 0, 0 );

    checkPrefix = new QCheckBox( this, "checkPrefix" );
    checkPrefix->setText( tr( "Use preview prefix ( for use PHP etc )"  ) );

    grid->addMultiCellWidget( checkPrefix, 5, 5, 0, 2 );

    checkFilter = new QCheckBox( this, "checkFilter" );
    checkFilter->setText( tr( "Use filter for preview"  ) );

    grid->addMultiCellWidget( checkFilter, 7, 7, 0, 2 );

    TextLabel7 = new QLabel( this, "TextLabel7" );
    TextLabel7->setText( tr( "Filter app"  ) );

    grid->addWidget( TextLabel7, 8, 0 );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    grid->addItem( spacer, 9, 1 );

    lineFilter = new QLineEdit( this, "lineFilter" );
    lineFilter->setEnabled( FALSE );

    grid->addWidget( lineFilter, 8, 1 );

    buttonFilter = new QPushButton( this, "buttonFilter" );
    buttonFilter->setText( tr( "..."  ) );
    buttonFilter->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)1, buttonFilter->sizePolicy().hasHeightForWidth() ) );
    buttonFilter->setMaximumSize( QSize( 32, 32767 ) );
    buttonFilter->setEnabled( FALSE );

    grid->addWidget( buttonFilter, 8, 2 );

    // signals and slots connections
    connect( checkPublishing, SIGNAL( toggled(bool) ), lineUrl, SLOT( setEnabled(bool) ) );
    connect( checkPrefix, SIGNAL( toggled(bool) ), linePrefix, SLOT( setEnabled(bool) ) );
    connect( checkFilter, SIGNAL( toggled(bool) ), lineFilter, SLOT( setEnabled(bool) ) );
    connect( checkPublishing, SIGNAL( toggled(bool) ), lineUser, SLOT( setEnabled(bool) ) );
    connect( checkPublishing, SIGNAL( toggled(bool) ), linePasswd, SLOT( setEnabled(bool) ) );
    connect( checkFilter, SIGNAL( toggled(bool) ), buttonFilter, SLOT( setEnabled(bool) ) );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
ProjectNewFinalS::~ProjectNewFinalS()
{
    // no need to delete child widgets, Qt does it all for us
}

