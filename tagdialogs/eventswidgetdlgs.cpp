/****************************************************************************
** Form implementation generated from reading ui file 'eventswidgetdlgs.ui'
**
** Created: Fri Nov 3 19:03:49 2000
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "eventswidgetdlgs.h"
#include "eventswidgetdlgs.moc"
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/*
 *  Constructs a EventsWidgetDlgS which is a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'
 */
EventsWidgetDlgS::EventsWidgetDlgS( QWidget* parent,  const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "EventsWidgetDlgS" );
    resize( 445, 387 );
    setCaption( tr( "Form1"  ) );
    grid = new QGridLayout( this );
    grid->setSpacing( 6 );
    grid->setMargin( 11 );

    TextLabel1 = new QLabel( this, "TextLabel1" );
    TextLabel1->setText( tr( "OnClick"  ) );

    grid->addWidget( TextLabel1, 0, 0 );

    TextLabel2 = new QLabel( this, "TextLabel2" );
    TextLabel2->setText( tr( "OnDblClick"  ) );

    grid->addWidget( TextLabel2, 1, 0 );

    TextLabel3 = new QLabel( this, "TextLabel3" );
    TextLabel3->setText( tr( "OnMouseDown"  ) );

    grid->addWidget( TextLabel3, 2, 0 );

    TextLabel4 = new QLabel( this, "TextLabel4" );
    TextLabel4->setText( tr( "OnMouseUp"  ) );

    grid->addWidget( TextLabel4, 3, 0 );

    TextLabel5 = new QLabel( this, "TextLabel5" );
    TextLabel5->setText( tr( "OnMouseOver"  ) );

    grid->addWidget( TextLabel5, 4, 0 );

    TextLabel6 = new QLabel( this, "TextLabel6" );
    TextLabel6->setText( tr( "OnMouseMove"  ) );

    grid->addWidget( TextLabel6, 5, 0 );

    TextLabel7 = new QLabel( this, "TextLabel7" );
    TextLabel7->setText( tr( "OnMouseOut"  ) );

    grid->addWidget( TextLabel7, 6, 0 );

    TextLabel8 = new QLabel( this, "TextLabel8" );
    TextLabel8->setText( tr( "OnKeyPress"  ) );

    grid->addWidget( TextLabel8, 7, 0 );

    TextLabel9 = new QLabel( this, "TextLabel9" );
    TextLabel9->setText( tr( "OnKeyDown"  ) );

    grid->addWidget( TextLabel9, 8, 0 );

    TextLabel10 = new QLabel( this, "TextLabel10" );
    TextLabel10->setText( tr( "OnKeyUp"  ) );

    grid->addWidget( TextLabel10, 9, 0 );

    lineClick = new QLineEdit( this, "lineClick" );

    grid->addWidget( lineClick, 0, 1 );

    lineDblClick = new QLineEdit( this, "lineDblClick" );

    grid->addWidget( lineDblClick, 1, 1 );

    lineMouseDown = new QLineEdit( this, "lineMouseDown" );

    grid->addWidget( lineMouseDown, 2, 1 );

    lineMouseUp = new QLineEdit( this, "lineMouseUp" );

    grid->addWidget( lineMouseUp, 3, 1 );

    lineMouseOver = new QLineEdit( this, "lineMouseOver" );

    grid->addWidget( lineMouseOver, 4, 1 );

    lineMouseMove = new QLineEdit( this, "lineMouseMove" );

    grid->addWidget( lineMouseMove, 5, 1 );

    lineMouseOut = new QLineEdit( this, "lineMouseOut" );

    grid->addWidget( lineMouseOut, 6, 1 );

    lineKeyPress = new QLineEdit( this, "lineKeyPress" );

    grid->addWidget( lineKeyPress, 7, 1 );

    lineKeyDown = new QLineEdit( this, "lineKeyDown" );

    grid->addWidget( lineKeyDown, 8, 1 );

    lineKeyUp = new QLineEdit( this, "lineKeyUp" );

    grid->addWidget( lineKeyUp, 9, 1 );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    grid->addItem( spacer, 10, 1 );
}

/*
 *  Destroys the object and frees any allocated resources
 */
EventsWidgetDlgS::~EventsWidgetDlgS()
{
    // no need to delete child widgets, Qt does it all for us
}

