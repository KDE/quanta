/****************************************************************************
** Form implementation generated from reading ui file 'project1.ui'
**
** Created: Fri Oct 27 23:20:35 2000
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "projectnewgenerals.h"

#include <qbuttongroup.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/* 
 *  Constructs a ProjectNewGeneralS which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 */
ProjectNewGeneralS::ProjectNewGeneralS( QWidget* parent,  const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "ProjectNewGeneralS" );
    resize( 399, 388 ); 
    setCaption( tr( "Form2"  ) );
    grid = new QGridLayout( this ); 
    grid->setSpacing( 6 );
    grid->setMargin( 11 );

    TextLabel2 = new QLabel( this, "TextLabel2" );
    TextLabel2->setText( tr( "Project destination"  ) );

    grid->addWidget( TextLabel2, 1, 0 );

    TextLabel3 = new QLabel( this, "TextLabel3" );
    TextLabel3->setText( tr( "Project source file"  ) );

    grid->addWidget( TextLabel3, 2, 0 );

    lineAuthor = new QLineEdit( this, "lineAuthor" );

    grid->addMultiCellWidget( lineAuthor, 4, 4, 1, 2 );

    linePrjFile = new QLineEdit( this, "linePrjFile" );

    grid->addMultiCellWidget( linePrjFile, 2, 2, 1, 2 );

    lineEmail = new QLineEdit( this, "lineEmail" );

    grid->addMultiCellWidget( lineEmail, 5, 5, 1, 2 );

    TextLabel4 = new QLabel( this, "TextLabel4" );
    TextLabel4->setText( tr( "Author"  ) );

    grid->addWidget( TextLabel4, 4, 0 );

    TextLabel5 = new QLabel( this, "TextLabel5" );
    TextLabel5->setText( tr( "E-mail"  ) );

    grid->addWidget( TextLabel5, 5, 0 );

    TextLabel1 = new QLabel( this, "TextLabel1" );
    TextLabel1->setText( tr( "Project name"  ) );

    grid->addWidget( TextLabel1, 0, 0 );

    linePrjName = new QLineEdit( this, "linePrjName" );

    grid->addMultiCellWidget( linePrjName, 0, 0, 1, 2 );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    grid->addItem( spacer, 6, 1 );

    ButtonGroup1_2 = new QButtonGroup( this, "ButtonGroup1_2" );
    ButtonGroup1_2->setTitle( tr( "Project sources"  ) );
    ButtonGroup1_2->setColumnLayout(0, Qt::Vertical );
    ButtonGroup1_2->layout()->setSpacing( 0 );
    ButtonGroup1_2->layout()->setMargin( 0 );
    grid_2 = new QGridLayout( ButtonGroup1_2->layout() );
    grid_2->setAlignment( Qt::AlignTop );
    grid_2->setSpacing( 6 );
    grid_2->setMargin( 11 );

    radioLocal = new QRadioButton( ButtonGroup1_2, "radioLocal" );
    radioLocal->setText( tr( "Local files"  ) );
    radioLocal->setChecked( TRUE );

    grid_2->addWidget( radioLocal, 0, 0 );

    radioWeb = new QRadioButton( ButtonGroup1_2, "radioWeb" );
    radioWeb->setText( tr( "Web files"  ) );

    grid_2->addWidget( radioWeb, 1, 0 );

    radioCvs = new QRadioButton( ButtonGroup1_2, "radioCvs" );
    radioCvs->setText( tr( "CVS project"  ) );

    grid_2->addWidget( radioCvs, 2, 0 );

    grid->addMultiCellWidget( ButtonGroup1_2, 7, 7, 0, 2 );
    QSpacerItem* spacer_2 = new QSpacerItem( 20, 30, QSizePolicy::Minimum, QSizePolicy::Fixed );
    grid->addItem( spacer_2, 3, 1 );

    buttonDir = new QPushButton( this, "buttonDir" );
    buttonDir->setText( tr( "..."  ) );
    buttonDir->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, buttonDir->sizePolicy().hasHeightForWidth() ) );
    buttonDir->setMaximumSize( QSize( 32, 32767 ) );

    grid->addWidget( buttonDir, 1, 2 );

    linePrjDir = new QLineEdit( this, "linePrjDir" );

    grid->addWidget( linePrjDir, 1, 1 );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
ProjectNewGeneralS::~ProjectNewGeneralS()
{
    // no need to delete child widgets, Qt does it all for us
}

