#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file './dtdselectdialog.ui'
**
** Created: Sun Aug 18 15:45:57 2002
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "dtdselectdialog.h"

#include <qvariant.h>
#include <qcombobox.h>
#include <qlabel.h>
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
    resize( 332, 116 ); 
    setCaption( tr2i18n( "Form1" ) );

    ComboBox1 = new QComboBox( FALSE, this, "ComboBox1" );
    ComboBox1->setGeometry( QRect( 85, 20, 230, 20 ) ); 

    TextLabel1 = new QLabel( this, "TextLabel1" );
    TextLabel1->setGeometry( QRect( 20, 25, 59, 12 ) ); 
    TextLabel1->setText( tr2i18n( "Select DTD:" ) );

    cancelButton = new QPushButton( this, "cancelButton" );
    cancelButton->setGeometry( QRect( 185, 80, 80, 22 ) ); 
    cancelButton->setText( tr2i18n( "&Cancel" ) );

    okButton = new QPushButton( this, "okButton" );
    okButton->setGeometry( QRect( 80, 80, 80, 22 ) ); 
    okButton->setText( tr2i18n( "&OK" ) );

    // signals and slots connections
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
