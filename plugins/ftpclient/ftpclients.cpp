/****************************************************************************
** Form implementation generated from reading ui file 'ftp-client-2.ui'
**
** Created: Fri Jan 12 20:05:00 2001
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "ftpclients.h"
#include "ftpclientview.h"

#include <ktoolbar.h>
#include <qcombobox.h>
#include <qgroupbox.h>
#include <qheader.h>
#include <qlabel.h>
#include <qlistview.h>
#include <qprogressbar.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

// kde includes
#include <klocale.h>

/* 
 *  Constructs a FtpClientS which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 */
FtpClientS::FtpClientS( QWidget* parent,  const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "FtpClientS" );
    resize( 578, 480 ); 
    setCaption( i18n( "Form2"  ) );
    FtpClientSLayout = new QGridLayout( this ); 
    FtpClientSLayout->setSpacing( 6 );
    FtpClientSLayout->setMargin( 2 );

    Layout22 = new QGridLayout; 
    Layout22->setSpacing( 6 );
    Layout22->setMargin( 0 );

    Layout21 = new QVBoxLayout; 
    Layout21->setSpacing( 6 );
    Layout21->setMargin( 0 );

    GroupBox1_2 = new QGroupBox( this, "GroupBox1_2" );
    GroupBox1_2->setTitle( i18n( ""  ) );
    GroupBox1_2->setColumnLayout(0, Qt::Vertical );
    GroupBox1_2->layout()->setSpacing( 0 );
    GroupBox1_2->layout()->setMargin( 0 );
    GroupBox1_2Layout = new QGridLayout( GroupBox1_2->layout() );
    GroupBox1_2Layout->setAlignment( Qt::AlignTop );
    GroupBox1_2Layout->setSpacing( 0 );
    GroupBox1_2Layout->setMargin( 6 );

    TextLabel3_2 = new QLabel( GroupBox1_2, "TextLabel3_2" );
    TextLabel3_2->setText( i18n( "Remote Directory"  ) );

    GroupBox1_2Layout->addWidget( TextLabel3_2, 0, 0 );
    Layout21->addWidget( GroupBox1_2 );

    Layout10_2 = new QHBoxLayout; 
    Layout10_2->setSpacing( 6 );
    Layout10_2->setMargin( 0 );

    comboDirRemote = new QComboBox( FALSE, this, "comboDirRemote" );
    comboDirRemote->setEditable( TRUE );
    Layout10_2->addWidget( comboDirRemote );

    buttonRemote = new QPushButton( this, "buttonRemote" );
    buttonRemote->setText( i18n( "..."  ) );
    buttonRemote->setMaximumSize( QSize( 32, 26 ) );
    Layout10_2->addWidget( buttonRemote );
    Layout21->addLayout( Layout10_2 );

    listViewRemote = new FtpClientView( this );
    listViewRemote->addColumn( i18n( "File Name" ) );
    listViewRemote->addColumn( i18n( "Size" ) );
    listViewRemote->addColumn( i18n( "Time" ) );
    listViewRemote->addColumn( i18n( "Date" ) );
    Layout21->addWidget( listViewRemote );

    Layout22->addLayout( Layout21, 1, 1 );

    toolBar = new KToolBar( this, "toolBar" );
#if QT_VERSION < 300
    toolBar->setMinimumSize( QSize( 0, 22 ) );
#else
    toolBar->setMinimumSize( 0, 22 );
#endif

    Layout22->addMultiCellWidget( toolBar, 0, 0, 0, 1 );

    GroupBox3 = new QGroupBox( this, "GroupBox3" );
    GroupBox3->setTitle( i18n( ""  ) );
    GroupBox3->setColumnLayout(0, Qt::Vertical );
    GroupBox3->layout()->setSpacing( 0 );
    GroupBox3->layout()->setMargin( 0 );
    GroupBox3Layout = new QGridLayout( GroupBox3->layout() );
    GroupBox3Layout->setAlignment( Qt::AlignTop );
    GroupBox3Layout->setSpacing( 0 );
    GroupBox3Layout->setMargin( 6 );

    statusText = new QLabel( GroupBox3, "statusText" );
    statusText->setText( i18n( ""  ) );
    statusText->setAlignment( int( QLabel::AlignCenter ) );

    GroupBox3Layout->addWidget( statusText, 0, 0 );

    Layout22->addMultiCellWidget( GroupBox3, 3, 3, 0, 1 );

    Layout20 = new QVBoxLayout; 
    Layout20->setSpacing( 6 );
    Layout20->setMargin( 0 );

    GroupBox1 = new QGroupBox( this, "GroupBox1" );
    GroupBox1->setTitle( i18n( ""  ) );
    GroupBox1->setColumnLayout(0, Qt::Vertical );
    GroupBox1->layout()->setSpacing( 0 );
    GroupBox1->layout()->setMargin( 0 );
    GroupBox1Layout = new QGridLayout( GroupBox1->layout() );
    GroupBox1Layout->setAlignment( Qt::AlignTop );
    GroupBox1Layout->setSpacing( 0 );
    GroupBox1Layout->setMargin( 6 );

    TextLabel3 = new QLabel( GroupBox1, "TextLabel3" );
    TextLabel3->setText( i18n( "Local Directory"  ) );

    GroupBox1Layout->addWidget( TextLabel3, 0, 0 );
    Layout20->addWidget( GroupBox1 );

    Layout10 = new QHBoxLayout; 
    Layout10->setSpacing( 6 );
    Layout10->setMargin( 0 );

    comboDirLocal = new QComboBox( FALSE, this, "comboDirLocal" );
    comboDirLocal->setEditable( TRUE );
    Layout10->addWidget( comboDirLocal );

    buttonLocal = new QPushButton( this, "buttonLocal" );
    buttonLocal->setText( i18n( "..."  ) );
    buttonLocal->setMaximumSize( QSize( 32, 26 ) );
    Layout10->addWidget( buttonLocal );
    Layout20->addLayout( Layout10 );

    listViewLocal = new FtpClientView( this );
    listViewLocal->addColumn( i18n( "File Name" ) );
    listViewLocal->addColumn( i18n( "Size" ) );
    listViewLocal->addColumn( i18n( "Time" ) );
    listViewLocal->addColumn( i18n( "Date" ) );
    Layout20->addWidget( listViewLocal );

    Layout22->addLayout( Layout20, 1, 0 );

    progress = new QProgressBar( this, "progress" );
    progress->setProgress( 0 );

    Layout22->addMultiCellWidget( progress, 2, 2, 0, 1 );

    FtpClientSLayout->addLayout( Layout22, 0, 0 );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
FtpClientS::~FtpClientS()
{
    // no need to delete child widgets, Qt does it all for us
}

