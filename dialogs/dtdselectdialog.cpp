#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file './dtdselectdialog.ui'
**
** Created: Sat Sep 21 16:59:11 2002
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "dtdselectdialog.h"

#include <qvariant.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "dtdselectdialog.ui.h"

/* 
 *  Constructs a DTDSelectDialog which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
DTDSelectDialog::DTDSelectDialog( QWidget* parent,  const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "DTDSelectDialog" );
    resize( 337, 157 ); 
    setCaption( tr2i18n( "DTD Selector" ) );

    okButton = new QPushButton( this, "okButton" );
    okButton->setGeometry( QRect( 130, 130, 80, 22 ) ); 
    okButton->setText( tr2i18n( "&OK" ) );

    QWidget* privateLayoutWidget = new QWidget( this, "Layout1" );
    privateLayoutWidget->setGeometry( QRect( 10, 60, 315, 60 ) ); 
    Layout1 = new QGridLayout( privateLayoutWidget, 1, 1, 0, 6, "Layout1"); 

    dtdCombo = new QComboBox( FALSE, privateLayoutWidget, "dtdCombo" );

    Layout1->addWidget( dtdCombo, 0, 1 );

    TextLabel1_2 = new QLabel( privateLayoutWidget, "TextLabel1_2" );
    TextLabel1_2->setText( tr2i18n( "Current DTD:" ) );

    Layout1->addWidget( TextLabel1_2, 1, 0 );

    TextLabel1 = new QLabel( privateLayoutWidget, "TextLabel1" );
    TextLabel1->setText( tr2i18n( "Select DTD:" ) );

    Layout1->addWidget( TextLabel1, 0, 0 );

    currentDTD = new QLineEdit( privateLayoutWidget, "currentDTD" );
    currentDTD->setReadOnly( TRUE );

    Layout1->addWidget( currentDTD, 1, 1 );

    messageLabel = new QLabel( this, "messageLabel" );
    messageLabel->setGeometry( QRect( 15, 10, 310, 40 ) ); 
    messageLabel->setText( tr2i18n( "Dialog message:\n"
"Dialog message2:" ) );
    messageLabel->setScaledContents( TRUE );
    messageLabel->setAlignment( int( QLabel::WordBreak | QLabel::AlignVCenter ) );

    // signals and slots connections

    // tab order
    setTabOrder( dtdCombo, currentDTD );
    setTabOrder( currentDTD, okButton );
    init();
}

/*  
 *  Destroys the object and frees any allocated resources
 */
DTDSelectDialog::~DTDSelectDialog()
{
    // no need to delete child widgets, Qt does it all for us
}

#include "dtdselectdialog.moc"
