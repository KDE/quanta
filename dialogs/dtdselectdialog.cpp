#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file './dtdselectdialog.ui'
**
** Created: Tue Oct 8 16:33:18 2002
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "dtdselectdialog.h"

#include <qvariant.h>
#include <qcheckbox.h>
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
    resize( 445, 190 ); 
    setCaption( tr2i18n( "DTD Selector" ) );

    QWidget* privateLayoutWidget = new QWidget( this, "Layout2" );
    privateLayoutWidget->setGeometry( QRect( 10, 10, 430, 137 ) ); 
    Layout2 = new QGridLayout( privateLayoutWidget, 1, 1, 0, 6, "Layout2"); 

    messageLabel = new QLabel( privateLayoutWidget, "messageLabel" );
    messageLabel->setText( tr2i18n( "Dialog message:\n"
"Dialog message2:" ) );
    messageLabel->setScaledContents( TRUE );
    messageLabel->setAlignment( int( QLabel::WordBreak | QLabel::AlignVCenter ) );

    Layout2->addWidget( messageLabel, 0, 0 );

    Layout1 = new QGridLayout( 0, 1, 1, 0, 6, "Layout1"); 

    dtdCombo = new QComboBox( FALSE, privateLayoutWidget, "dtdCombo" );

    Layout1->addWidget( dtdCombo, 0, 1 );

    textLabel = new QLabel( privateLayoutWidget, "textLabel" );
    textLabel->setText( tr2i18n( "Current DTD:" ) );

    Layout1->addWidget( textLabel, 1, 0 );

    comboLabel = new QLabel( privateLayoutWidget, "comboLabel" );
    comboLabel->setText( tr2i18n( "Select DTD:" ) );

    Layout1->addWidget( comboLabel, 0, 0 );

    currentDTD = new QLineEdit( privateLayoutWidget, "currentDTD" );
    currentDTD->setReadOnly( TRUE );

    Layout1->addWidget( currentDTD, 1, 1 );

    Layout2->addLayout( Layout1, 1, 0 );

    convertDTD = new QCheckBox( privateLayoutWidget, "convertDTD" );
    convertDTD->setText( tr2i18n( "&Convert the document to the selected DTD" ) );
    convertDTD->setChecked( TRUE );

    Layout2->addWidget( convertDTD, 2, 0 );

    okButton = new QPushButton( this, "okButton" );
    okButton->setGeometry( QRect( 190, 160, 80, 22 ) ); 
    okButton->setText( tr2i18n( "&OK" ) );

    // signals and slots connections

    // tab order
    setTabOrder( dtdCombo, currentDTD );
    setTabOrder( currentDTD, convertDTD );
    setTabOrder( convertDTD, okButton );
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
