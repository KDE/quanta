/****************************************************************************
** Form implementation generated from reading ui file 'ftp-client.ui'
**
** Created: Fri Dec 1 23:08:45 2000
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "ftpclients.h"
#include "ftpclientview.h"

#include <qbuttongroup.h>
#include <qcombobox.h>
#include <qframe.h>
#include <qheader.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qlistview.h>
#include <qprogressbar.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/* 
 *  Constructs a FtpClientS which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 */
FtpClientS::FtpClientS( QWidget* parent,  const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "FtpClientS" );
    resize( 715, 644 ); 
    setCaption( tr( "Form3"  ) );
    FtpClientSLayout = new QGridLayout( this ); 
    FtpClientSLayout->setSpacing( 6 );
    FtpClientSLayout->setMargin( 11 );

    leftView = new FtpClientView( this );

    FtpClientSLayout->addMultiCellWidget( leftView, 0, 1, 0, 0 );

    progress = new QProgressBar( this, "progress" );
    progress->setProgress( 0 );

    FtpClientSLayout->addMultiCellWidget( progress, 2, 2, 0, 1 );

    rightView = new FtpClientView( this );

    FtpClientSLayout->addWidget( rightView, 1, 1 );

    ButtonGroup1 = new QButtonGroup( this, "ButtonGroup1" );
    ButtonGroup1->setTitle( tr( ""  ) );
    ButtonGroup1->setColumnLayout(0, Qt::Vertical );
    ButtonGroup1->layout()->setSpacing( 0 );
    ButtonGroup1->layout()->setMargin( 0 );
    ButtonGroup1Layout = new QGridLayout( ButtonGroup1->layout() );
    ButtonGroup1Layout->setAlignment( Qt::AlignTop );
    ButtonGroup1Layout->setSpacing( 6 );
    ButtonGroup1Layout->setMargin( 11 );

    TextLabel19 = new QLabel( ButtonGroup1, "TextLabel19" );
    TextLabel19->setText( tr( "User Name"  ) );

    ButtonGroup1Layout->addWidget( TextLabel19, 2, 0 );

    TextLabel16 = new QLabel( ButtonGroup1, "TextLabel16" );
    TextLabel16->setText( tr( "Remote host"  ) );

    ButtonGroup1Layout->addWidget( TextLabel16, 0, 0 );

    TextLabel18 = new QLabel( ButtonGroup1, "TextLabel18" );
    TextLabel18->setText( tr( "Remote Path"  ) );

    ButtonGroup1Layout->addWidget( TextLabel18, 1, 0 );

    TextLabel20 = new QLabel( ButtonGroup1, "TextLabel20" );
    TextLabel20->setText( tr( "Password"  ) );

    ButtonGroup1Layout->addWidget( TextLabel20, 3, 0 );

    remoteHost = new QComboBox( FALSE, ButtonGroup1, "remoteHost" );
    remoteHost->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)0, remoteHost->sizePolicy().hasHeightForWidth() ) );
    remoteHost->setEditable( TRUE );

    ButtonGroup1Layout->addWidget( remoteHost, 0, 1 );

    remotePath = new QComboBox( FALSE, ButtonGroup1, "remotePath" );
    remotePath->setEditable( TRUE );

    ButtonGroup1Layout->addMultiCellWidget( remotePath, 1, 1, 1, 3 );

    userName = new QComboBox( FALSE, ButtonGroup1, "userName" );
    userName->setEditable( TRUE );

    ButtonGroup1Layout->addWidget( userName, 2, 1 );

    password = new QLineEdit( ButtonGroup1, "password" );
    password->setEchoMode( QLineEdit::Password );

    ButtonGroup1Layout->addWidget( password, 3, 1 );

    TextLabel17 = new QLabel( ButtonGroup1, "TextLabel17" );
    TextLabel17->setText( tr( "Port"  ) );

    ButtonGroup1Layout->addWidget( TextLabel17, 0, 2 );

    port = new QLineEdit( ButtonGroup1, "port" );
    port->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, port->sizePolicy().hasHeightForWidth() ) );
    port->setText( tr( "21"  ) );
    port->setMaximumSize( QSize( 50, 32767 ) );
    port->setAlignment( int( QLineEdit::AlignRight ) );

    ButtonGroup1Layout->addWidget( port, 0, 3 );

    connectButton = new QPushButton( ButtonGroup1, "connect" );
    connectButton->setText( tr( "Connect"  ) );

    ButtonGroup1Layout->addMultiCellWidget( connectButton, 3, 3, 2, 3 );

    FtpClientSLayout->addWidget( ButtonGroup1, 0, 1 );

    Frame6 = new QFrame( this, "Frame6" );
    Frame6->setFrameShadow( QFrame::Raised );
    Frame6->setFrameShape( QFrame::StyledPanel );
    Frame6->setLineWidth( 0 );
    Frame6Layout = new QGridLayout( Frame6 ); 
    Frame6Layout->setSpacing( 6 );
    Frame6Layout->setMargin( 1 );

    buttonF5 = new QPushButton( Frame6, "buttonF5" );
    buttonF5->setText( tr( "F5-Copy"  ) );

    Frame6Layout->addWidget( buttonF5, 0, 1 );

    buttonF8 = new QPushButton( Frame6, "buttonF8" );
    buttonF8->setText( tr( "F8-Delete"  ) );

    Frame6Layout->addWidget( buttonF8, 0, 4 );

    buttonF1 = new QPushButton( Frame6, "buttonF1" );
    buttonF1->setText( tr( "F1-Help"  ) );

    Frame6Layout->addWidget( buttonF1, 0, 0 );

    buttonF6 = new QPushButton( Frame6, "buttonF6" );
    buttonF6->setText( tr( "F6-Move"  ) );

    Frame6Layout->addWidget( buttonF6, 0, 2 );

    buttonF7 = new QPushButton( Frame6, "buttonF7" );
    buttonF7->setText( tr( "F7-MkDir"  ) );

    Frame6Layout->addWidget( buttonF7, 0, 3 );

    buttonF10 = new QPushButton( Frame6, "buttonF10" );
    buttonF10->setText( tr( "F10-Exit"  ) );

    Frame6Layout->addWidget( buttonF10, 0, 5 );

    FtpClientSLayout->addMultiCellWidget( Frame6, 3, 3, 0, 1 );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
FtpClientS::~FtpClientS()
{
    // no need to delete child widgets, Qt does it all for us
}

