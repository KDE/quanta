#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file './projectuploads.ui'
**
** Created: Tue Feb 6 13:25:36 2001
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "projectuploads.h"

#include <qheader.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qlistview.h>
#include <qprogressbar.h>
#include <qpushbutton.h>
#include <qspinbox.h>
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
    resize( 439, 458 ); 
    setCaption( i18n( "Upload project files"  ) );
    setSizeGripEnabled( TRUE );
    ProjectUploadSLayout = new QGridLayout( this ); 
    ProjectUploadSLayout->setSpacing( 6 );
    ProjectUploadSLayout->setMargin( 11 );

    buttonAll = new QPushButton( this, "buttonAll" );
    buttonAll->setText( i18n( "All"  ) );

    ProjectUploadSLayout->addWidget( buttonAll, 3, 5 );

    buttonModified = new QPushButton( this, "buttonModified" );
    buttonModified->setText( i18n( "Modified"  ) );

    ProjectUploadSLayout->addWidget( buttonModified, 4, 5 );

    buttonClear = new QPushButton( this, "buttonClear" );
    buttonClear->setText( i18n( "Clear"  ) );

    ProjectUploadSLayout->addWidget( buttonClear, 5, 5 );

    list = new QListView( this, "list" );
    list->addColumn( i18n( "File name" ) );
    list->addColumn( i18n( "Date" ) );
    list->addColumn( i18n( "Size" ) );

    ProjectUploadSLayout->addMultiCellWidget( list, 3, 8, 0, 4 );

    buttonCancel = new QPushButton( this, "buttonCancel" );
    buttonCancel->setText( i18n( "&Cancel"  ) );
    buttonCancel->setAutoDefault( TRUE );

    ProjectUploadSLayout->addWidget( buttonCancel, 8, 5 );

    buttonUpload = new QPushButton( this, "buttonUpload" );
    buttonUpload->setText( i18n( "&Upload"  ) );
    buttonUpload->setAutoDefault( TRUE );
    buttonUpload->setDefault( TRUE );

    ProjectUploadSLayout->addWidget( buttonUpload, 7, 5 );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    ProjectUploadSLayout->addItem( spacer, 6, 5 );

    ProgressBar1 = new QProgressBar( this, "ProgressBar1" );

    ProjectUploadSLayout->addMultiCellWidget( ProgressBar1, 11, 11, 0, 5 );
    QSpacerItem* spacer_2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    ProjectUploadSLayout->addMultiCell( spacer_2, 9, 9, 0, 4 );

    TextLabel2_2 = new QLabel( this, "TextLabel2_2" );
    TextLabel2_2->setText( i18n( "Path"  ) );

    ProjectUploadSLayout->addWidget( TextLabel2_2, 1, 0 );

    TextLabel4 = new QLabel( this, "TextLabel4" );
    TextLabel4->setText( i18n( "Host:"  ) );

    ProjectUploadSLayout->addWidget( TextLabel4, 0, 0 );

    TextLabel2 = new QLabel( this, "TextLabel2" );
    TextLabel2->setText( i18n( "User"  ) );

    ProjectUploadSLayout->addWidget( TextLabel2, 2, 0 );

    labelCurFile = new QLabel( this, "labelCurFile" );
    labelCurFile->setText( i18n( ""  ) );

    ProjectUploadSLayout->addMultiCellWidget( labelCurFile, 10, 10, 0, 5 );

    lineUser = new QLineEdit( this, "lineUser" );

    ProjectUploadSLayout->addWidget( lineUser, 2, 1 );

    TextLabel3 = new QLabel( this, "TextLabel3" );
    TextLabel3->setText( i18n( "Password"  ) );

    ProjectUploadSLayout->addWidget( TextLabel3, 2, 2 );

    linePasswd = new QLineEdit( this, "linePasswd" );
    linePasswd->setEchoMode( QLineEdit::Password );

    ProjectUploadSLayout->addMultiCellWidget( linePasswd, 2, 2, 3, 4 );

    linePath = new QLineEdit( this, "linePath" );

    ProjectUploadSLayout->addMultiCellWidget( linePath, 1, 1, 1, 4 );

    spinPort = new QSpinBox( this, "spinPort" );
    spinPort->setValue( 21 );

    ProjectUploadSLayout->addWidget( spinPort, 0, 4 );

    TextLabel1_2 = new QLabel( this, "TextLabel1_2" );
    TextLabel1_2->setText( i18n( "Port:"  ) );

    ProjectUploadSLayout->addWidget( TextLabel1_2, 0, 3 );

    lineHost = new QLineEdit( this, "lineHost" );

    ProjectUploadSLayout->addMultiCellWidget( lineHost, 0, 0, 1, 2 );

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
