/****************************************************************************
** Form implementation generated from reading ui file 'filemasks.ui'
**
** Created: Tue Oct 31 21:07:32 2000
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "filemaskss.h"

#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/* 
 *  Constructs a FileMasksS which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 */
FileMasksS::FileMasksS( QWidget* parent,  const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "FileMasksS" );
    resize( 304, 248 ); 
    setCaption( tr( "Form1"  ) );
    grid = new QGridLayout( this ); 
    grid->setSpacing( 6 );
    grid->setMargin( 11 );

    TextLabel4 = new QLabel( this, "TextLabel4" );
    TextLabel4->setText( tr( "Text files:"  ) );

    grid->addWidget( TextLabel4, 3, 0 );

    TextLabel3 = new QLabel( this, "TextLabel3" );
    TextLabel3->setText( tr( "Images:"  ) );

    grid->addWidget( TextLabel3, 2, 0 );

    TextLabel2 = new QLabel( this, "TextLabel2" );
    TextLabel2->setText( tr( "Php files:"  ) );

    grid->addWidget( TextLabel2, 1, 0 );

    TextLabel1 = new QLabel( this, "TextLabel1" );
    TextLabel1->setText( tr( "Html files:"  ) );

    grid->addWidget( TextLabel1, 0, 0 );

    lineHTML = new QLineEdit( this, "lineHTML" );

    grid->addMultiCellWidget( lineHTML, 0, 0, 1, 2 );

    linePHP = new QLineEdit( this, "linePHP" );

    grid->addMultiCellWidget( linePHP, 1, 1, 1, 2 );

    lineImages = new QLineEdit( this, "lineImages" );

    grid->addMultiCellWidget( lineImages, 2, 2, 1, 2 );

    lineText = new QLineEdit( this, "lineText" );

    grid->addMultiCellWidget( lineText, 3, 3, 1, 2 );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    grid->addItem( spacer, 5, 1 );

    buttonDefault = new QPushButton( this, "buttonDefault" );
    buttonDefault->setText( tr( "Default"  ) );

    grid->addWidget( buttonDefault, 4, 2 );
    QSpacerItem* spacer_2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    grid->addItem( spacer_2, 4, 1 );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
FileMasksS::~FileMasksS()
{
    // no need to delete child widgets, Qt does it all for us
}

