/****************************************************************************
** Form implementation generated from reading ui file 'project-web.ui'
**
** Created: Fri Oct 27 22:12:00 2000
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "projectnewwebs.h"

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
 *  Constructs a ProjectNewWebS which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 */
ProjectNewWebS::ProjectNewWebS( QWidget* parent,  const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "ProjectNewWebS" );
    resize( 390, 384 ); 
    setCaption( tr( "Form4"  ) );
    grid = new QGridLayout( this ); 
    grid->setSpacing( 6 );
    grid->setMargin( 11 );

    siteUrl = new QLineEdit( this, "siteUrl" );

    grid->addWidget( siteUrl, 0, 1 );

    TextLabel9 = new QLabel( this, "TextLabel9" );
    TextLabel9->setText( tr( "Site source"  ) );

    grid->addWidget( TextLabel9, 0, 0 );

    TextLabel10 = new QLabel( this, "TextLabel10" );
    TextLabel10->setText( tr( "wget's command line"  ) );

    grid->addWidget( TextLabel10, 1, 0 );

    commandLine = new QLineEdit( this, "commandLine" );

    grid->addWidget( commandLine, 1, 1 );

    listFiles = new QListView( this, "listFiles" );
    listFiles->addColumn( tr( "File name" ) );
    listFiles->addColumn( tr( "Size" ) );

    grid->addMultiCellWidget( listFiles, 4, 4, 0, 1 );

    textFromTo = new QLabel( this, "textFromTo" );
    textFromTo->setText( tr( "Download files from:"  ) );

    grid->addMultiCellWidget( textFromTo, 3, 3, 0, 1 );

    hbox = new QHBoxLayout; 
    hbox->setSpacing( 6 );
    hbox->setMargin( 0 );

    progress = new QProgressBar( this, "progress" );
    progress->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)1, progress->sizePolicy().hasHeightForWidth() ) );
    hbox->addWidget( progress );

    button = new QPushButton( this, "button" );
    button->setText( tr( "Start"  ) );
    button->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, button->sizePolicy().hasHeightForWidth() ) );
    hbox->addWidget( button );

    grid->addMultiCellLayout( hbox, 5, 5, 0, 1 );
    QSpacerItem* spacer = new QSpacerItem( 20, 30, QSizePolicy::Minimum, QSizePolicy::Fixed );
    grid->addItem( spacer, 2, 1 );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
ProjectNewWebS::~ProjectNewWebS()
{
    // no need to delete child widgets, Qt does it all for us
}

