/****************************************************************************
** Form implementation generated from reading ui file 'corewidgetdlg.ui'
**
** Created: Thu Nov 2 18:48:48 2000
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "corewidgetdlgs.h"

#include <qframe.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/* 
 *  Constructs a CoreWidgetDlgS which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 */
CoreWidgetDlgS::CoreWidgetDlgS( QWidget* parent,  const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "CoreWidgetDlgS" );
    resize( 318, 316 ); 
    setCaption( tr( "Form1"  ) );
    grid = new QGridLayout( this ); 
    grid->setSpacing( 6 );
    grid->setMargin( 11 );

    TextLabel1 = new QLabel( this, "TextLabel1" );
    TextLabel1->setText( tr( "Id"  ) );

    grid->addWidget( TextLabel1, 0, 0 );

    TextLabel2 = new QLabel( this, "TextLabel2" );
    TextLabel2->setText( tr( "Class            "  ) );

    grid->addWidget( TextLabel2, 1, 0 );

    TextLabel3 = new QLabel( this, "TextLabel3" );
    TextLabel3->setText( tr( "Style"  ) );

    grid->addWidget( TextLabel3, 2, 0 );

    TextLabel4 = new QLabel( this, "TextLabel4" );
    TextLabel4->setText( tr( "Title"  ) );

    grid->addWidget( TextLabel4, 3, 0 );

    Line1 = new QFrame( this, "Line1" );
    Line1->setFrameStyle( QFrame::HLine | QFrame::Sunken );

    grid->addMultiCellWidget( Line1, 4, 4, 0, 2 );

    TextLabel5 = new QLabel( this, "TextLabel5" );
    TextLabel5->setText( tr( "Lang"  ) );

    grid->addWidget( TextLabel5, 5, 0 );

    TextLabel6 = new QLabel( this, "TextLabel6" );
    TextLabel6->setText( tr( "Dir"  ) );

    grid->addWidget( TextLabel6, 6, 0 );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    grid->addItem( spacer, 7, 2 );

    lineId = new QLineEdit( this, "lineId" );

    grid->addWidget( lineId, 0, 2 );

    lineClass = new QLineEdit( this, "lineClass" );

    grid->addWidget( lineClass, 1, 2 );

    lineStyle = new QLineEdit( this, "lineStyle" );

    grid->addWidget( lineStyle, 2, 2 );

    lineTitle = new QLineEdit( this, "lineTitle" );

    grid->addWidget( lineTitle, 3, 2 );

    lineLang = new QLineEdit( this, "lineLang" );

    grid->addWidget( lineLang, 5, 2 );

    lineDir = new QLineEdit( this, "lineDir" );

    grid->addWidget( lineDir, 6, 2 );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
CoreWidgetDlgS::~CoreWidgetDlgS()
{
    // no need to delete child widgets, Qt does it all for us
}

