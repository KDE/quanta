#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file './projectuploads.ui'
**
** Created: Sun Feb 11 21:18:01 2001
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "projectuploads.h"

#include <qcombobox.h>
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
 *  Constructs a ProjectUploadS which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
ProjectUploadS::ProjectUploadS( QWidget* parent,  const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "ProjectUploadS" );
    resize( 435, 458 ); 
    setCaption( i18n( "Upload project files" ) );
    setSizeGripEnabled( TRUE );
    ProjectUploadSLayout = new QGridLayout( this ); 
    ProjectUploadSLayout->setSpacing( 6 );
    ProjectUploadSLayout->setMargin( 11 );

    list = new QListView( this, "list" );
    list->addColumn( i18n( "File name" ) );
    list->addColumn( i18n( "Date" ) );
    list->addColumn( i18n( "Size" ) );

    ProjectUploadSLayout->addWidget( list, 1, 0 );

    ProgressBar1 = new QProgressBar( this, "ProgressBar1" );

    ProjectUploadSLayout->addMultiCellWidget( ProgressBar1, 3, 3, 0, 1 );

    labelCurFile = new QLabel( this, "labelCurFile" );
    labelCurFile->setText( QString::null );

    ProjectUploadSLayout->addMultiCellWidget( labelCurFile, 2, 2, 0, 1 );

    Layout1 = new QVBoxLayout; 
    Layout1->setSpacing( 6 );
    Layout1->setMargin( 0 );

    buttonAll = new QPushButton( this, "buttonAll" );
    buttonAll->setText( i18n( "All" ) );
    Layout1->addWidget( buttonAll );

    buttonModified = new QPushButton( this, "buttonModified" );
    buttonModified->setText( i18n( "Modified" ) );
    Layout1->addWidget( buttonModified );

    buttonClear = new QPushButton( this, "buttonClear" );
    buttonClear->setText( i18n( "Clear" ) );
    Layout1->addWidget( buttonClear );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Layout1->addItem( spacer );

    buttonUpload = new QPushButton( this, "buttonUpload" );
    buttonUpload->setText( i18n( "&Upload" ) );
    buttonUpload->setAutoDefault( TRUE );
    buttonUpload->setDefault( TRUE );
    Layout1->addWidget( buttonUpload );

    buttonCancel = new QPushButton( this, "buttonCancel" );
    buttonCancel->setText( i18n( "&Cancel" ) );
    buttonCancel->setAutoDefault( TRUE );
    Layout1->addWidget( buttonCancel );

    ProjectUploadSLayout->addLayout( Layout1, 1, 1 );

    Layout3 = new QGridLayout; 
    Layout3->setSpacing( 6 );
    Layout3->setMargin( 0 );

    TextLabel2_2 = new QLabel( this, "TextLabel2_2" );
    TextLabel2_2->setText( i18n( "Path" ) );

    Layout3->addWidget( TextLabel2_2, 1, 0 );

    TextLabel2 = new QLabel( this, "TextLabel2" );
    TextLabel2->setText( i18n( "User" ) );

    Layout3->addWidget( TextLabel2, 2, 0 );

    linePasswd = new QLineEdit( this, "linePasswd" );
    linePasswd->setEchoMode( QLineEdit::Password );

    Layout3->addMultiCellWidget( linePasswd, 2, 2, 3, 4 );

    lineHost = new QLineEdit( this, "lineHost" );

    Layout3->addMultiCellWidget( lineHost, 0, 0, 1, 3 );

    comboProtocol = new QComboBox( FALSE, this, "comboProtocol" );

    Layout3->addMultiCellWidget( comboProtocol, 0, 0, 5, 6 );

    TextLabel1 = new QLabel( this, "TextLabel1" );
    TextLabel1->setText( i18n( "Protocol : " ) );
    TextLabel1->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );

    Layout3->addWidget( TextLabel1, 0, 4 );

    lineUser = new QLineEdit( this, "lineUser" );

    Layout3->addWidget( lineUser, 2, 1 );

    TextLabel4 = new QLabel( this, "TextLabel4" );
    TextLabel4->setText( i18n( "Host:" ) );

    Layout3->addWidget( TextLabel4, 0, 0 );

    TextLabel1_2 = new QLabel( this, "TextLabel1_2" );
    TextLabel1_2->setText( i18n( "Port:" ) );

    Layout3->addWidget( TextLabel1_2, 1, 5 );

    port = new QLineEdit( this, "port" );

    Layout3->addWidget( port, 1, 6 );

    linePath = new QLineEdit( this, "linePath" );

    Layout3->addMultiCellWidget( linePath, 1, 1, 1, 4 );

    TextLabel3 = new QLabel( this, "TextLabel3" );
    TextLabel3->setText( i18n( "Password" ) );

    Layout3->addWidget( TextLabel3, 2, 2 );

    ProjectUploadSLayout->addMultiCellLayout( Layout3, 0, 0, 0, 1 );

    // signals and slots connections
    connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( buttonAll, SIGNAL( clicked() ), this, SLOT( selectAll() ) );
    connect( buttonModified, SIGNAL( clicked() ), this, SLOT( selectModified() ) );
    connect( buttonClear, SIGNAL( clicked() ), this, SLOT( clearSelection() ) );
    connect( buttonUpload, SIGNAL( clicked() ), this, SLOT( startUpload() ) );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
ProjectUploadS::~ProjectUploadS()
{
    // no need to delete child widgets, Qt does it all for us
}

void ProjectUploadS::clearSelection()
{
    qWarning( "ProjectUploadS::clearSelection(): Not implemented yet!" );
}

void ProjectUploadS::startUpload()
{
    qWarning( "ProjectUploadS::startUpload(): Not implemented yet!" );
}

void ProjectUploadS::selectAll()
{
    qWarning( "ProjectUploadS::selectAll(): Not implemented yet!" );
}

void ProjectUploadS::selectModified()
{
    qWarning( "ProjectUploadS::selectModified(): Not implemented yet!" );
}

#include "projectuploads.moc"
