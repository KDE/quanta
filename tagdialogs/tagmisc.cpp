#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file './tagmisc.ui'
**
** Created: Mon May 20 11:28:20 2002
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "tagmisc.h"

#include <qvariant.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/* 
 *  Constructs a TagMisc which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
TagMisc::TagMisc( QWidget* parent,  const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "TagMisc" );
    resize( 331, 116 ); 
    setCaption( tr2i18n( "Misc. tag" ) );

    QWidget* privateLayoutWidget = new QWidget( this, "Layout4" );
    privateLayoutWidget->setGeometry( QRect( 5, 10, 320, 56 ) ); 
    Layout4 = new QGridLayout( privateLayoutWidget, 1, 1, 0, 6, "Layout4"); 

    elementName = new QLineEdit( privateLayoutWidget, "elementName" );

    Layout4->addWidget( elementName, 0, 1 );

    TextLabel1 = new QLabel( privateLayoutWidget, "TextLabel1" );
    TextLabel1->setText( tr2i18n( "Element name:" ) );

    Layout4->addWidget( TextLabel1, 0, 0 );

    addClosingTag = new QCheckBox( privateLayoutWidget, "addClosingTag" );
    addClosingTag->setText( tr2i18n( "&Add closing tag" ) );
    addClosingTag->setChecked( TRUE );

    Layout4->addMultiCellWidget( addClosingTag, 1, 1, 0, 1 );

    QWidget* privateLayoutWidget_2 = new QWidget( this, "Layout5" );
    privateLayoutWidget_2->setGeometry( QRect( 5, 80, 320, 28 ) ); 
    Layout5 = new QGridLayout( privateLayoutWidget_2, 1, 1, 0, 6, "Layout5"); 
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout5->addItem( spacer, 0, 1 );

    buttonCancel = new QPushButton( privateLayoutWidget_2, "buttonCancel" );
    buttonCancel->setText( tr2i18n( "Cancel" ) );

    Layout5->addWidget( buttonCancel, 0, 3 );

    buttonOk = new QPushButton( privateLayoutWidget_2, "buttonOk" );
    buttonOk->setText( tr2i18n( "Ok" ) );
    buttonOk->setDefault( TRUE );

    Layout5->addWidget( buttonOk, 0, 2 );

    buttonHelp = new QPushButton( privateLayoutWidget_2, "buttonHelp" );
    buttonHelp->setText( tr2i18n( "Help" ) );

    Layout5->addWidget( buttonHelp, 0, 0 );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
TagMisc::~TagMisc()
{
    // no need to delete child widgets, Qt does it all for us
}

#include "tagmisc.moc"
