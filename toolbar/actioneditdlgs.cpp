#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file './actioneditdlgs.ui'
**
** Created: Thu Feb 15 22:39:55 2001
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "actioneditdlgs.h"

#include <filecombo.h>
#include <kicondialog.h>
#include <qbuttongroup.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qlistbox.h>
#include <qmultilineedit.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qtabwidget.h>
#include <qwidget.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/* 
 *  Constructs a ActionEditDlgS which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
ActionEditDlgS::ActionEditDlgS( QWidget* parent,  const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "ActionEditDlgS" );
    resize( 509, 494 ); 
    setCaption( i18n( "MyDialog" ) );
    setSizeGripEnabled( TRUE );
    ActionEditDlgSLayout = new QGridLayout( this ); 
    ActionEditDlgSLayout->setSpacing( 6 );
    ActionEditDlgSLayout->setMargin( 11 );

    lineStatusText = new QLineEdit( this, "lineStatusText" );

    ActionEditDlgSLayout->addWidget( lineStatusText, 2, 4 );

    actionTab = new QTabWidget( this, "actionTab" );

    Widget3 = new QWidget( actionTab, "Widget3" );
    Widget3Layout = new QGridLayout( Widget3 ); 
    Widget3Layout->setSpacing( 6 );
    Widget3Layout->setMargin( 11 );

    TextLabel5_2 = new QLabel( Widget3, "TextLabel5_2" );
    TextLabel5_2->setText( i18n( "<tag> :" ) );

    Widget3Layout->addWidget( TextLabel5_2, 0, 0 );

    useTagClose = new QCheckBox( Widget3, "useTagClose" );
    useTagClose->setText( i18n( "</tag> :" ) );
    useTagClose->setChecked( TRUE );

    Widget3Layout->addWidget( useTagClose, 1, 0 );

    tag = new QLineEdit( Widget3, "tag" );

    Widget3Layout->addWidget( tag, 0, 1 );

    tagClose = new QLineEdit( Widget3, "tagClose" );

    Widget3Layout->addWidget( tagClose, 1, 1 );

    useActionDialog = new QCheckBox( Widget3, "useActionDialog" );
    useActionDialog->setText( i18n( "Run \"Edit tag\" dialog if avaible" ) );
    useActionDialog->setChecked( TRUE );

    Widget3Layout->addMultiCellWidget( useActionDialog, 2, 2, 0, 1 );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Widget3Layout->addMultiCell( spacer, 3, 3, 0, 1 );
    QSpacerItem* spacer_2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Widget3Layout->addItem( spacer_2, 3, 2 );
    actionTab->insertTab( Widget3, i18n( "Tag" ) );

    Widget4 = new QWidget( actionTab, "Widget4" );
    Widget4Layout = new QGridLayout( Widget4 ); 
    Widget4Layout->setSpacing( 6 );
    Widget4Layout->setMargin( 11 );

    scriptPath = new FileCombo( Widget4, "scriptPath" );
    scriptPath->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)5, scriptPath->sizePolicy().hasHeightForWidth() ) );
    scriptPath->setMinimumSize( QSize( 64, 20 ) );

    Widget4Layout->addWidget( scriptPath, 0, 0 );

    tabScript = new QTabWidget( Widget4, "tabScript" );

    Widget5 = new QWidget( tabScript, "Widget5" );
    Widget5Layout = new QGridLayout( Widget5 ); 
    Widget5Layout->setSpacing( 6 );
    Widget5Layout->setMargin( 11 );

    ButtonGroup34_2 = new QButtonGroup( Widget5, "ButtonGroup34_2" );
    ButtonGroup34_2->setTitle( QString::null );
    ButtonGroup34_2->setColumnLayout(0, Qt::Vertical );
    ButtonGroup34_2->layout()->setSpacing( 0 );
    ButtonGroup34_2->layout()->setMargin( 0 );
    ButtonGroup34_2Layout = new QGridLayout( ButtonGroup34_2->layout() );
    ButtonGroup34_2Layout->setAlignment( Qt::AlignTop );
    ButtonGroup34_2Layout->setSpacing( 6 );
    ButtonGroup34_2Layout->setMargin( 11 );

    inputNone = new QRadioButton( ButtonGroup34_2, "inputNone" );
    inputNone->setText( i18n( "None" ) );
    inputNone->setChecked( TRUE );

    ButtonGroup34_2Layout->addWidget( inputNone, 0, 0 );

    inputSelected = new QRadioButton( ButtonGroup34_2, "inputSelected" );
    inputSelected->setText( i18n( "selected text" ) );

    ButtonGroup34_2Layout->addWidget( inputSelected, 1, 0 );

    inputCurrent = new QRadioButton( ButtonGroup34_2, "inputCurrent" );
    inputCurrent->setText( i18n( "current document" ) );

    ButtonGroup34_2Layout->addWidget( inputCurrent, 2, 0 );

    Widget5Layout->addWidget( ButtonGroup34_2, 0, 0 );
    tabScript->insertTab( Widget5, i18n( "Input" ) );

    Widget6 = new QWidget( tabScript, "Widget6" );
    Widget6Layout = new QGridLayout( Widget6 ); 
    Widget6Layout->setSpacing( 6 );
    Widget6Layout->setMargin( 11 );

    ButtonGroup35_3 = new QButtonGroup( Widget6, "ButtonGroup35_3" );
    ButtonGroup35_3->setTitle( QString::null );
    ButtonGroup35_3->setColumnLayout(0, Qt::Vertical );
    ButtonGroup35_3->layout()->setSpacing( 0 );
    ButtonGroup35_3->layout()->setMargin( 0 );
    ButtonGroup35_3Layout = new QGridLayout( ButtonGroup35_3->layout() );
    ButtonGroup35_3Layout->setAlignment( Qt::AlignTop );
    ButtonGroup35_3Layout->setSpacing( 6 );
    ButtonGroup35_3Layout->setMargin( 11 );

    outputNone = new QRadioButton( ButtonGroup35_3, "outputNone" );
    outputNone->setText( i18n( "None" ) );
    outputNone->setChecked( FALSE );

    ButtonGroup35_3Layout->addWidget( outputNone, 0, 0 );

    outputCursor_2 = new QRadioButton( ButtonGroup35_3, "outputCursor_2" );
    outputCursor_2->setText( i18n( "Inser in cursor position" ) );

    ButtonGroup35_3Layout->addWidget( outputCursor_2, 1, 0 );

    outputReplace = new QRadioButton( ButtonGroup35_3, "outputReplace" );
    outputReplace->setText( i18n( "Replace current document" ) );

    ButtonGroup35_3Layout->addWidget( outputReplace, 2, 0 );

    outputNew = new QRadioButton( ButtonGroup35_3, "outputNew" );
    outputNew->setText( i18n( "Create new document" ) );

    ButtonGroup35_3Layout->addWidget( outputNew, 3, 0 );

    outputMessage = new QRadioButton( ButtonGroup35_3, "outputMessage" );
    outputMessage->setText( i18n( "Message Window" ) );
    outputMessage->setChecked( TRUE );

    ButtonGroup35_3Layout->addWidget( outputMessage, 4, 0 );

    Widget6Layout->addWidget( ButtonGroup35_3, 0, 0 );
    tabScript->insertTab( Widget6, i18n( "Output" ) );

    Widget7 = new QWidget( tabScript, "Widget7" );
    Widget7Layout = new QGridLayout( Widget7 ); 
    Widget7Layout->setSpacing( 6 );
    Widget7Layout->setMargin( 11 );

    ButtonGroup35_2_2 = new QButtonGroup( Widget7, "ButtonGroup35_2_2" );
    ButtonGroup35_2_2->setTitle( QString::null );
    ButtonGroup35_2_2->setColumnLayout(0, Qt::Vertical );
    ButtonGroup35_2_2->layout()->setSpacing( 0 );
    ButtonGroup35_2_2->layout()->setMargin( 0 );
    ButtonGroup35_2_2Layout = new QVBoxLayout( ButtonGroup35_2_2->layout() );
    ButtonGroup35_2_2Layout->setAlignment( Qt::AlignTop );
    ButtonGroup35_2_2Layout->setSpacing( 6 );
    ButtonGroup35_2_2Layout->setMargin( 11 );

    errorNone = new QRadioButton( ButtonGroup35_2_2, "errorNone" );
    errorNone->setText( i18n( "None" ) );
    errorNone->setChecked( FALSE );
    ButtonGroup35_2_2Layout->addWidget( errorNone );

    errorCursor = new QRadioButton( ButtonGroup35_2_2, "errorCursor" );
    errorCursor->setText( i18n( "Inser in cursor position" ) );
    ButtonGroup35_2_2Layout->addWidget( errorCursor );

    errorReplace = new QRadioButton( ButtonGroup35_2_2, "errorReplace" );
    errorReplace->setText( i18n( "Replace current document" ) );
    ButtonGroup35_2_2Layout->addWidget( errorReplace );

    errorNew = new QRadioButton( ButtonGroup35_2_2, "errorNew" );
    errorNew->setText( i18n( "Create new document" ) );
    ButtonGroup35_2_2Layout->addWidget( errorNew );

    errorMessage = new QRadioButton( ButtonGroup35_2_2, "errorMessage" );
    errorMessage->setText( i18n( "Message Window" ) );
    errorMessage->setChecked( TRUE );
    ButtonGroup35_2_2Layout->addWidget( errorMessage );

    Widget7Layout->addWidget( ButtonGroup35_2_2, 0, 0 );
    tabScript->insertTab( Widget7, i18n( "Error" ) );

    Widget4Layout->addWidget( tabScript, 1, 0 );
    actionTab->insertTab( Widget4, i18n( "Script" ) );

    Widget8 = new QWidget( actionTab, "Widget8" );
    Widget8Layout = new QGridLayout( Widget8 ); 
    Widget8Layout->setSpacing( 6 );
    Widget8Layout->setMargin( 11 );

    text = new QMultiLineEdit( Widget8, "text" );

    Widget8Layout->addWidget( text, 0, 0 );
    actionTab->insertTab( Widget8, i18n( "Text" ) );

    ActionEditDlgSLayout->addMultiCellWidget( actionTab, 3, 3, 1, 4 );

    Layout5 = new QHBoxLayout; 
    Layout5->setSpacing( 6 );
    Layout5->setMargin( 0 );

    buttonNew = new QPushButton( this, "buttonNew" );
    buttonNew->setText( i18n( "&New" ) );
    buttonNew->setAutoDefault( TRUE );
    Layout5->addWidget( buttonNew );

    buttonDelete = new QPushButton( this, "buttonDelete" );
    buttonDelete->setText( i18n( "Delete" ) );
    Layout5->addWidget( buttonDelete );
    QSpacerItem* spacer_3 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout5->addItem( spacer_3 );

    buttonOk_2 = new QPushButton( this, "buttonOk_2" );
    buttonOk_2->setText( i18n( "&OK" ) );
    buttonOk_2->setAutoDefault( TRUE );
    buttonOk_2->setDefault( TRUE );
    Layout5->addWidget( buttonOk_2 );

    buttonCancel_2 = new QPushButton( this, "buttonCancel_2" );
    buttonCancel_2->setText( i18n( "&Cancel" ) );
    buttonCancel_2->setAutoDefault( TRUE );
    Layout5->addWidget( buttonCancel_2 );

    ActionEditDlgSLayout->addMultiCellLayout( Layout5, 4, 4, 0, 4 );
    QSpacerItem* spacer_4 = new QSpacerItem( 20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum );
    ActionEditDlgSLayout->addItem( spacer_4, 0, 2 );

    TextLabel3 = new QLabel( this, "TextLabel3" );
    TextLabel3->setText( i18n( "Status text:" ) );

    ActionEditDlgSLayout->addMultiCellWidget( TextLabel3, 2, 2, 1, 3 );

    lineToolTip = new QLineEdit( this, "lineToolTip" );

    ActionEditDlgSLayout->addWidget( lineToolTip, 1, 4 );

    actionsList = new QListBox( this, "actionsList" );
    actionsList->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, actionsList->sizePolicy().hasHeightForWidth() ) );

    ActionEditDlgSLayout->addMultiCellWidget( actionsList, 0, 3, 0, 0 );

    TextLabel2 = new QLabel( this, "TextLabel2" );
    TextLabel2->setText( i18n( "ToolTip:" ) );

    ActionEditDlgSLayout->addMultiCellWidget( TextLabel2, 1, 1, 1, 3 );

    TextLabel1_2 = new QLabel( this, "TextLabel1_2" );
    TextLabel1_2->setText( i18n( "Text:" ) );

    ActionEditDlgSLayout->addWidget( TextLabel1_2, 0, 3 );

    lineText = new QLineEdit( this, "lineText" );

    ActionEditDlgSLayout->addWidget( lineText, 0, 4 );

    actionIcon = new KIconButton( this, "actionIcon" );
    actionIcon->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, actionIcon->sizePolicy().hasHeightForWidth() ) );
    actionIcon->setMinimumSize( QSize( 32, 32 ) );
    actionIcon->setMaximumSize( QSize( 32, 32 ) );

    ActionEditDlgSLayout->addWidget( actionIcon, 0, 1 );

    // signals and slots connections
    connect( buttonOk_2, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( buttonCancel_2, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( useTagClose, SIGNAL( toggled(bool) ), tagClose, SLOT( setEnabled(bool) ) );
    connect( actionsList, SIGNAL( highlighted(int) ), this, SLOT( actionSelected() ) );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
ActionEditDlgS::~ActionEditDlgS()
{
    // no need to delete child widgets, Qt does it all for us
}

void ActionEditDlgS::deleteAction()
{
    qWarning( "ActionEditDlgS::deleteAction(): Not implemented yet!" );
}

void ActionEditDlgS::newAction()
{
    qWarning( "ActionEditDlgS::newAction(): Not implemented yet!" );
}

void ActionEditDlgS::actionSelected()
{
    qWarning( "ActionEditDlgS::actionSelected(): Not implemented yet!" );
}

#include "actioneditdlgs.moc"
