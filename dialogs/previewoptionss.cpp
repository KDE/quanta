/****************************************************************************
** Form implementation generated from reading ui file 'previewoptions.ui'
**
** Created: Tue Oct 31 22:17:20 2000
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "previewoptionss.h"

#include <qbuttongroup.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include <kfontdialog.h>

/*
 *  Constructs a PreviewOptionsS which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 */
PreviewOptionsS::PreviewOptionsS( QWidget* parent,  const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
			setName( "PreviewOptionsS" );
    resize( 450, 350 ); 
    setCaption( tr( "Form1"  ) );
    grid = new QGridLayout( this ); 
    grid->setSpacing( 6 );
    grid->setMargin( 11 );


    pixmap = new QLabel( this, "PixmapLabel2" );
    pixmap->setScaledContents( TRUE );
    pixmap->setMinimumSize( QSize( 180, 130 ) );
    pixmap->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, pixmap->sizePolicy().hasHeightForWidth() ) );

    grid->addWidget( pixmap, 0, 1 );

    ButtonGroup1 = new QButtonGroup( this, "ButtonGroup1" );
    ButtonGroup1->setTitle( tr( "Preview"  ) );
    ButtonGroup1->setColumnLayout(0, Qt::Vertical );
    ButtonGroup1->layout()->setSpacing( 0 );
    ButtonGroup1->layout()->setMargin( 0 );
    grid_2 = new QGridLayout( ButtonGroup1->layout() );
    grid_2->setAlignment( Qt::AlignTop );
    grid_2->setSpacing( 6 );
    grid_2->setMargin( 11 );

    radioRight = new QRadioButton( ButtonGroup1, "radioRight" );
    radioRight->setText( tr( "Right"  ) );

    grid_2->addWidget( radioRight, 0, 0 );

    radioBottom = new QRadioButton( ButtonGroup1, "radioBottom" );
    radioBottom->setText( tr( "Bottom"  ) );

    grid_2->addWidget( radioBottom, 1, 0 );

    radioFullworkspace = new QRadioButton( ButtonGroup1, "radioFullworkspace" );
    radioFullworkspace->setText( tr( "Full workspace"  ) );

    grid_2->addWidget( radioFullworkspace, 2, 0 );

    radioDisabled = new QRadioButton( ButtonGroup1, "radioDisabled" );
    radioDisabled->setText( tr( "Disabled"  ) );

    grid_2->addWidget( radioDisabled, 3, 0 );

    grid->addWidget( ButtonGroup1, 0, 0 );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    grid->addItem( spacer, 1, 1 );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
PreviewOptionsS::~PreviewOptionsS()
{
    // no need to delete child widgets, Qt does it all for us
}

