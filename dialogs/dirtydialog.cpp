#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file './dirtydialog.ui'
**
** Created: Fri Sep 13 16:30:56 2002
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "dirtydialog.h"

#include <qvariant.h>
#include <qbuttongroup.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/* 
 *  Constructs a DirtyDialog which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
DirtyDialog::DirtyDialog( QWidget* parent,  const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "DirtyDialog" );
    resize( 365, 207 ); 
    setCaption( tr2i18n( "File changed" ) );

    okButton = new QPushButton( this, "okButton" );
    okButton->setGeometry( QRect( 80, 175, 80, 22 ) ); 
    okButton->setText( tr2i18n( "&OK" ) );

    cancelButton = new QPushButton( this, "cancelButton" );
    cancelButton->setGeometry( QRect( 230, 175, 80, 22 ) ); 
    cancelButton->setText( tr2i18n( "&Cancel" ) );

    TextLabel1 = new QLabel( this, "TextLabel1" );
    TextLabel1->setGeometry( QRect( 5, 5, 290, 16 ) ); 
    TextLabel1->setText( tr2i18n( "The file was changed outside of the Quanta editor." ) );

    ButtonGroup2 = new QButtonGroup( this, "ButtonGroup2" );
    ButtonGroup2->setGeometry( QRect( 5, 30, 355, 125 ) ); 
    ButtonGroup2->setTitle( tr2i18n( " How do you want to proceed?" ) );

    buttonLoad = new QRadioButton( ButtonGroup2, "buttonLoad" );
    buttonLoad->setGeometry( QRect( 6, 44, 338, 17 ) ); 
    buttonLoad->setText( tr2i18n( "&Use the version from disk (will lose the current content)" ) );

    TextLabel2 = new QLabel( ButtonGroup2, "TextLabel2" );
    TextLabel2->setGeometry( QRect( 30, 85, 310, 25 ) ); 
    TextLabel2->setText( tr2i18n( "(If you save later the document, you will lose what was on the disk.)" ) );
    TextLabel2->setAlignment( int( QLabel::WordBreak | QLabel::AlignVCenter ) );

    buttonIgnore = new QRadioButton( ButtonGroup2, "buttonIgnore" );
    buttonIgnore->setGeometry( QRect( 6, 67, 338, 17 ) ); 
    buttonIgnore->setText( tr2i18n( "&Do not load the modified version from disk." ) );

    buttonCompare = new QRadioButton( ButtonGroup2, "buttonCompare" );
    buttonCompare->setGeometry( QRect( 5, 20, 338, 17 ) ); 
    buttonCompare->setText( tr2i18n( "&Compare the two versions and load the result" ) );
    buttonCompare->setChecked( TRUE );
    QToolTip::add( buttonCompare, tr2i18n( "Use Kompare if available. Otherwise this checkbox is disabled." ) );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
DirtyDialog::~DirtyDialog()
{
    // no need to delete child widgets, Qt does it all for us
}

#include "dirtydialog.moc"
