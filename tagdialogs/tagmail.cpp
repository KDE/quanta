#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file './tagmail.ui'
**
** Created: Mon May 20 11:28:22 2002
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "tagmail.h"

#include <qvariant.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/* 
 *  Constructs a TagMail which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
TagMail::TagMail( QWidget* parent,  const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "TagMail" );
    resize( 413, 163 ); 
    setCaption( tr2i18n( "Form1" ) );

    QWidget* privateLayoutWidget = new QWidget( this, "Layout9" );
    privateLayoutWidget->setGeometry( QRect( 10, 130, 403, 28 ) ); 
    Layout9 = new QHBoxLayout( privateLayoutWidget, 0, 6, "Layout9"); 

    buttonHelp = new QPushButton( privateLayoutWidget, "buttonHelp" );
    buttonHelp->setText( tr2i18n( "Help" ) );
    Layout9->addWidget( buttonHelp );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout9->addItem( spacer );

    buttonOk = new QPushButton( privateLayoutWidget, "buttonOk" );
    buttonOk->setText( tr2i18n( "Ok" ) );
    buttonOk->setDefault( TRUE );
    Layout9->addWidget( buttonOk );

    buttonCancel = new QPushButton( privateLayoutWidget, "buttonCancel" );
    buttonCancel->setText( tr2i18n( "Cancel" ) );
    Layout9->addWidget( buttonCancel );

    QWidget* privateLayoutWidget_2 = new QWidget( this, "Layout8" );
    privateLayoutWidget_2->setGeometry( QRect( 15, 15, 105, 100 ) ); 
    Layout8 = new QVBoxLayout( privateLayoutWidget_2, 0, 6, "Layout8"); 

    TextLabel2 = new QLabel( privateLayoutWidget_2, "TextLabel2" );
    TextLabel2->setText( tr2i18n( "E-mail address" ) );
    Layout8->addWidget( TextLabel2 );

    TextLabel2_2 = new QLabel( privateLayoutWidget_2, "TextLabel2_2" );
    TextLabel2_2->setText( tr2i18n( "Subject" ) );
    Layout8->addWidget( TextLabel2_2 );

    TextLabel1 = new QLabel( privateLayoutWidget_2, "TextLabel1" );
    TextLabel1->setText( tr2i18n( "Title" ) );
    Layout8->addWidget( TextLabel1 );

    QWidget* privateLayoutWidget_3 = new QWidget( this, "Layout2" );
    privateLayoutWidget_3->setGeometry( QRect( 125, 15, 250, 100 ) ); 
    Layout2 = new QVBoxLayout( privateLayoutWidget_3, 0, 6, "Layout2"); 

    lineEmail = new QLineEdit( privateLayoutWidget_3, "lineEmail" );
    Layout2->addWidget( lineEmail );

    lineSubject = new QLineEdit( privateLayoutWidget_3, "lineSubject" );
    Layout2->addWidget( lineSubject );

    lineTitle = new QLineEdit( privateLayoutWidget_3, "lineTitle" );
    QToolTip::add( lineTitle, tr2i18n( "Some browsers use this field as the subject" ) );
    Layout2->addWidget( lineTitle );

    buttonAddressSelect = new QPushButton( this, "buttonAddressSelect" );
    buttonAddressSelect->setGeometry( QRect( 380, 20, 32, 26 ) ); 
    buttonAddressSelect->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, buttonAddressSelect->sizePolicy().hasHeightForWidth() ) );
    buttonAddressSelect->setMaximumSize( QSize( 32, 32767 ) );
    buttonAddressSelect->setText( tr2i18n( "..." ) );

    // tab order
    setTabOrder( lineEmail, buttonAddressSelect );
    setTabOrder( buttonAddressSelect, lineSubject );
    setTabOrder( lineSubject, lineTitle );
    setTabOrder( lineTitle, buttonOk );
    setTabOrder( buttonOk, buttonCancel );
    setTabOrder( buttonCancel, buttonHelp );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
TagMail::~TagMail()
{
    // no need to delete child widgets, Qt does it all for us
}

#include "tagmail.moc"
