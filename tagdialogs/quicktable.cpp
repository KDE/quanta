#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file './quicktable.ui'
**
** Created: Mon May 20 11:28:17 2002
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "quicktable.h"

#include <qvariant.h>
#include <qcheckbox.h>
#include <qframe.h>
#include <qheader.h>
#include <qlabel.h>
#include <qlistview.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>


/* 
 *  Constructs a Quick_Table which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
Quick_Table::Quick_Table( QWidget* parent,  const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "Quick_Table" );
    resize( 440, 320 ); 
    setCaption( tr2i18n( "Quick Table" ) );
    setSizeGripEnabled( TRUE );
    setProperty( "name>", "Quick_Table" );

    QWidget* privateLayoutWidget = new QWidget( this, "Layout10" );
    privateLayoutWidget->setGeometry( QRect( 5, 5, 107, 265 ) ); 
    Layout10 = new QVBoxLayout( privateLayoutWidget, 0, 6, "Layout10"); 

    Layout6 = new QGridLayout( 0, 1, 1, 0, 6, "Layout6"); 

    SpinBoxCol = new QSpinBox( privateLayoutWidget, "SpinBoxCol" );
    SpinBoxCol->setMinValue( 1 );
    QToolTip::add( SpinBoxCol, tr2i18n( "Number of columns" ) );

    Layout6->addWidget( SpinBoxCol, 0, 1 );

    TextLabel3 = new QLabel( privateLayoutWidget, "TextLabel3" );
    TextLabel3->setText( tr2i18n( "Cols" ) );

    Layout6->addWidget( TextLabel3, 0, 0 );

    TextLabel3_2 = new QLabel( privateLayoutWidget, "TextLabel3_2" );
    TextLabel3_2->setText( tr2i18n( "Rows" ) );

    Layout6->addWidget( TextLabel3_2, 1, 0 );

    SpinBoxRow = new QSpinBox( privateLayoutWidget, "SpinBoxRow" );
    SpinBoxRow->setMinValue( 1 );
    QToolTip::add( SpinBoxRow, tr2i18n( "Number of columns" ) );

    Layout6->addWidget( SpinBoxRow, 1, 1 );
    Layout10->addLayout( Layout6 );

    CheckBox4 = new QCheckBox( privateLayoutWidget, "CheckBox4" );
    CheckBox4->setEnabled( FALSE );
    CheckBox4->setText( tr2i18n( "Use COLS" ) );
    Layout10->addWidget( CheckBox4 );

    useTHead = new QCheckBox( privateLayoutWidget, "useTHead" );
    useTHead->setText( tr2i18n( "Insert &header" ) );
    Layout10->addWidget( useTHead );

    useTFoot = new QCheckBox( privateLayoutWidget, "useTFoot" );
    useTFoot->setText( tr2i18n( "Insert &footer" ) );
    Layout10->addWidget( useTFoot );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Layout10->addItem( spacer );

    QWidget* privateLayoutWidget_2 = new QWidget( this, "Layout1" );
    privateLayoutWidget_2->setGeometry( QRect( 5, 285, 428, 28 ) ); 
    Layout1 = new QHBoxLayout( privateLayoutWidget_2, 0, 6, "Layout1"); 

    buttonHelp = new QPushButton( privateLayoutWidget_2, "buttonHelp" );
    buttonHelp->setText( tr2i18n( "&Help" ) );
    buttonHelp->setAutoDefault( TRUE );
    Layout1->addWidget( buttonHelp );
    QSpacerItem* spacer_2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout1->addItem( spacer_2 );

    buttonApply = new QPushButton( privateLayoutWidget_2, "buttonApply" );
    buttonApply->setEnabled( FALSE );
    buttonApply->setText( tr2i18n( "&Apply" ) );
    buttonApply->setAutoDefault( TRUE );
    Layout1->addWidget( buttonApply );

    buttonOk = new QPushButton( privateLayoutWidget_2, "buttonOk" );
    buttonOk->setText( tr2i18n( "&OK" ) );
    buttonOk->setAutoDefault( TRUE );
    buttonOk->setDefault( TRUE );
    Layout1->addWidget( buttonOk );

    buttonCancel = new QPushButton( privateLayoutWidget_2, "buttonCancel" );
    buttonCancel->setText( tr2i18n( "&Cancel" ) );
    buttonCancel->setAutoDefault( TRUE );
    Layout1->addWidget( buttonCancel );

    Line1 = new QFrame( this, "Line1" );
    Line1->setGeometry( QRect( 5, 270, 425, 16 ) ); 
    Line1->setFrameStyle( QFrame::HLine | QFrame::Sunken );

    ListView1 = new QListView( this, "ListView1" );
    ListView1->addColumn( tr2i18n( "Columns" ) );
    ListView1->addColumn( tr2i18n( "Width" ) );
    ListView1->addColumn( tr2i18n( "Align" ) );
    ListView1->addColumn( tr2i18n( "VAlign" ) );
    ListView1->addColumn( tr2i18n( "CSS class" ) );
    QListViewItem * item = new QListViewItem( ListView1, 0 );
    item->setText( 0, tr2i18n( "Column 1" ) );

    ListView1->setEnabled( FALSE );
    ListView1->setGeometry( QRect( 120, 5, 313, 240 ) ); 

    newColumn = new QPushButton( this, "newColumn" );
    newColumn->setEnabled( FALSE );
    newColumn->setGeometry( QRect( 205, 250, 114, 26 ) ); 
    newColumn->setText( tr2i18n( "&New item" ) );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
Quick_Table::~Quick_Table()
{
    // no need to delete child widgets, Qt does it all for us
}

#include "quicktable.moc"
