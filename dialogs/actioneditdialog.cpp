/***************************************************************************
                          actioneditdialog.cpp  -  description
                             -------------------
    begin                : Wed Oct 11 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
    email                : pdima@users.sourceforge.net,yshurik@penguinpowered.com,sequitur@easystreet.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
/****************************************************************************
** Form implementation generated from reading ui file 'actionedit.ui'
**
** Created: Wed Oct 11 17:49:57 2000
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "actioneditdialog.h"
#include "actioneditdialog.moc"
#include <qbuttongroup.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qmultilineedit.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qtabwidget.h>
#include <qwidget.h>
#include "../tagdialogs/filecombo.h"
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qdir.h>

#include <klocale.h>
#include <kicondialog.h>

const char* standardActionsList[] = {"Quick start","Quick list","Quick table","Insert color","Insert date","CSS",0};

/*
 *  Constructs a ActionEditDialog which is a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
ActionEditDialog::ActionEditDialog( QWidget* parent,  const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "ActionEditDialog" );
    resize( 517, 424 );
    setCaption( i18n( "Action"  ) );
    setSizeGripEnabled( TRUE );
    grid = new QGridLayout( this );
    grid->setSpacing( 6 );
    grid->setMargin( 11 );

    GroupBox7 = new QGroupBox( this, "GroupBox7" );
    GroupBox7->setTitle( i18n( "Action"  ) );
    GroupBox7->setEnabled( TRUE );
    GroupBox7->setColumnLayout(0, Qt::Vertical );
    GroupBox7->layout()->setSpacing( 0 );
    GroupBox7->layout()->setMargin( 0 );
    grid_2 = new QGridLayout( GroupBox7->layout() );
    grid_2->setAlignment( Qt::AlignTop );
    grid_2->setSpacing( 6 );
    grid_2->setMargin( 11 );

    actionIcon = new KIconButton( GroupBox7, "actionIcon" );
    actionIcon->setText( i18n( ""  ) );
    actionIcon->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0 ) );
    actionIcon->setMinimumSize( QSize( 32, 32 ) );
    actionIcon->setMaximumSize( QSize( 32, 32 ) );
    QToolTip::add(  actionIcon, i18n( "Action Icon" ) );
    actionIcon->setIconType(KIcon::User,0,true);

    grid_2->addWidget( actionIcon, 0, 0 );

    checkActionIconText = new QCheckBox( GroupBox7, "checkActionIconText" );
    checkActionIconText->setText( i18n( ""  ) );
    QToolTip::add(  checkActionIconText, i18n( "Use text instead of icon" ) );

    grid_2->addWidget( checkActionIconText, 0, 1 );

    actionButtonText = new QLineEdit( GroupBox7, "actionButtonText" );
    actionButtonText->setMaximumSize( QSize( 70, 32767 ) );
    actionButtonText->setEnabled( FALSE );
    QToolTip::add(  actionButtonText, i18n( "Action button text" ) );

    grid_2->addWidget( actionButtonText, 0, 2 );
    QSpacerItem* spacer = new QSpacerItem( 61, 21, QSizePolicy::Fixed, QSizePolicy::Fixed );
    grid_2->addItem( spacer, 0, 3 );

    actionTab = new QTabWidget( GroupBox7, "actionTab" );

    QWidget* widget = new QWidget( actionTab, "widget" );
    grid_3 = new QGridLayout( widget );
    grid_3->setSpacing( 6 );
    grid_3->setMargin( 11 );

    standardActions = new QComboBox( FALSE, widget, "standardActions" );
    standardActions->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0 ) );

    grid_3->addWidget( standardActions, 0, 0 );
    QSpacerItem* spacer_2 = new QSpacerItem( 20, 92, QSizePolicy::Fixed, QSizePolicy::Expanding );
    grid_3->addItem( spacer_2, 3, 0 );

    checkSeparator = new QCheckBox( widget, "checkSeparator" );
    checkSeparator->setText( i18n( "Separator"  ) );

    grid_3->addWidget( checkSeparator, 2, 0 );
    QSpacerItem* spacer_3 = new QSpacerItem( 20, 92, QSizePolicy::Fixed, QSizePolicy::Expanding );
    grid_3->addItem( spacer_3, 1, 0 );
    actionTab->insertTab( widget, i18n( "Standard" ) );

    QWidget* widget_2 = new QWidget( actionTab, "widget_2" );
    grid_4 = new QGridLayout( widget_2 );
    grid_4->setSpacing( 6 );
    grid_4->setMargin( 11 );

    TextLabel5 = new QLabel( widget_2, "TextLabel5" );
    TextLabel5->setText( i18n( "<tag> :"  ) );

    grid_4->addWidget( TextLabel5, 0, 0 );

    useTagClose = new QCheckBox( widget_2, "useTagClose" );
    useTagClose->setText( i18n( "</tag> :"  ) );

    grid_4->addWidget( useTagClose, 1, 0 );

    tag = new QLineEdit( widget_2, "tag" );

    grid_4->addWidget( tag, 0, 1 );

    tagClose = new QLineEdit( widget_2, "tagClose" );
    tagClose->setEnabled( FALSE );

    grid_4->addWidget( tagClose, 1, 1 );

    insertInLine = new QCheckBox( widget_2, "insertInLine" );
    insertInLine->setText( i18n( "Insert close part in same line"  ) );
    insertInLine->setChecked( true );
    grid_4->addWidget( insertInLine, 2, 0 );

    useActionDialog = new QCheckBox( widget_2, "useActionDialog" );
    useActionDialog->setText( i18n( "Run \"Edit tag\" dialog if avaible"  ) );

    grid_4->addMultiCellWidget( useActionDialog, 3, 3, 0, 1 );
    QSpacerItem* spacer_4 = new QSpacerItem( 257, 20, QSizePolicy::Expanding, QSizePolicy::Fixed );
    grid_4->addMultiCell( spacer_4, 4, 4, 0, 1 );
    QSpacerItem* spacer_5 = new QSpacerItem( 184, 20, QSizePolicy::Expanding, QSizePolicy::Fixed );
    grid_4->addItem( spacer_5, 4, 2 );
    actionTab->insertTab( widget_2, i18n( "Tag" ) );

    QWidget* widget_3 = new QWidget( actionTab, "widget_3" );
    grid_5 = new QGridLayout( widget_3 );
    grid_5->setSpacing( 6 );
    grid_5->setMargin( 11 );

    tabScript = new QTabWidget( widget_3, "tabScript" );

    QWidget* widget_4 = new QWidget( tabScript, "widget_4" );
    grid_6 = new QGridLayout( widget_4 );
    grid_6->setSpacing( 6 );
    grid_6->setMargin( 11 );

    ButtonGroup34 = new QButtonGroup( widget_4, "ButtonGroup34" );
    ButtonGroup34->setTitle( i18n( ""  ) );
    ButtonGroup34->setColumnLayout(0, Qt::Vertical );
    ButtonGroup34->layout()->setSpacing( 0 );
    ButtonGroup34->layout()->setMargin( 0 );
    grid_7 = new QGridLayout( ButtonGroup34->layout() );
    grid_7->setAlignment( Qt::AlignTop );
    grid_7->setSpacing( 6 );
    grid_7->setMargin( 11 );

    inputNone = new QRadioButton( ButtonGroup34, "inputNone" );
    inputNone->setText( i18n( "None"  ) );
    inputNone->setChecked( TRUE );

    grid_7->addWidget( inputNone, 0, 0 );

    inputSelected = new QRadioButton( ButtonGroup34, "inputSelected" );
    inputSelected->setText( i18n( "selected text"  ) );

    grid_7->addWidget( inputSelected, 1, 0 );

    inputCurrent = new QRadioButton( ButtonGroup34, "inputCurrent" );
    inputCurrent->setText( i18n( "current document"  ) );

    grid_7->addWidget( inputCurrent, 2, 0 );

    grid_6->addWidget( ButtonGroup34, 0, 0 );
    tabScript->insertTab( widget_4, i18n( "Input" ) );

    QWidget* widget_5 = new QWidget( tabScript, "widget_5" );
    grid_8 = new QGridLayout( widget_5 );
    grid_8->setSpacing( 6 );
    grid_8->setMargin( 11 );

    ButtonGroup35 = new QButtonGroup( widget_5, "ButtonGroup35" );
    ButtonGroup35->setTitle( i18n( ""  ) );
    ButtonGroup35->setColumnLayout(0, Qt::Vertical );
    ButtonGroup35->layout()->setSpacing( 0 );
    ButtonGroup35->layout()->setMargin( 0 );
    grid_9 = new QGridLayout( ButtonGroup35->layout() );
    grid_9->setAlignment( Qt::AlignTop );
    grid_9->setSpacing( 6 );
    grid_9->setMargin( 11 );

    outputNone = new QRadioButton( ButtonGroup35, "outputNone" );
    outputNone->setText( i18n( "None"  ) );
    outputNone->setChecked( TRUE );

    grid_9->addWidget( outputNone, 0, 0 );

    outputCursor = new QRadioButton( ButtonGroup35, "outputCursor" );
    outputCursor->setText( i18n( "Inser in cursor position"  ) );

    grid_9->addWidget( outputCursor, 1, 0 );

    outputReplace = new QRadioButton( ButtonGroup35, "outputReplace" );
    outputReplace->setText( i18n( "Replace current document"  ) );

    grid_9->addWidget( outputReplace, 2, 0 );

    outputNew = new QRadioButton( ButtonGroup35, "outputNew" );
    outputNew->setText( i18n( "Create new document"  ) );

    grid_9->addWidget( outputNew, 3, 0 );

    outputMessage = new QRadioButton( ButtonGroup35, "outputMessage" );
    outputMessage->setText( i18n( "Message Window"  ) );

    grid_9->addWidget( outputMessage, 4, 0 );

    grid_8->addWidget( ButtonGroup35, 0, 0 );
    tabScript->insertTab( widget_5, i18n( "Output" ) );

    QWidget* widget_6 = new QWidget( tabScript, "widget_6" );
    grid_10 = new QGridLayout( widget_6 );
    grid_10->setSpacing( 6 );
    grid_10->setMargin( 11 );

    ButtonGroup35_2 = new QButtonGroup( widget_6, "ButtonGroup35_2" );
    ButtonGroup35_2->setTitle( i18n( ""  ) );
    ButtonGroup35_2->setColumnLayout(0, Qt::Vertical );
    ButtonGroup35_2->layout()->setSpacing( 0 );
    ButtonGroup35_2->layout()->setMargin( 0 );
    grid_11 = new QGridLayout( ButtonGroup35_2->layout() );
    grid_11->setAlignment( Qt::AlignTop );
    grid_11->setSpacing( 6 );
    grid_11->setMargin( 11 );

    errorNone = new QRadioButton( ButtonGroup35_2, "errorNone" );
    errorNone->setText( i18n( "None"  ) );
    errorNone->setChecked( TRUE );

    grid_11->addWidget( errorNone, 0, 0 );

    errorCursor = new QRadioButton( ButtonGroup35_2, "errorCursor" );
    errorCursor->setText( i18n( "Inser in cursor position"  ) );

    grid_11->addWidget( errorCursor, 1, 0 );

    errorReplace = new QRadioButton( ButtonGroup35_2, "errorReplace" );
    errorReplace->setText( i18n( "Replace current document"  ) );

    grid_11->addWidget( errorReplace, 2, 0 );

    errorNew = new QRadioButton( ButtonGroup35_2, "errorNew" );
    errorNew->setText( i18n( "Create new document"  ) );

    grid_11->addWidget( errorNew, 3, 0 );

    errorMessage = new QRadioButton( ButtonGroup35_2, "errorMessage" );
    errorMessage->setText( i18n( "Message Window"  ) );

    grid_11->addWidget( errorMessage, 4, 0 );

    errorMerge = new QRadioButton( ButtonGroup35_2, "errorMerge" );
    errorMerge->setText( i18n( "Merge with output"  ) );

    grid_11->addWidget( errorMerge, 5, 0 );

    grid_10->addWidget( ButtonGroup35_2, 0, 0 );
    tabScript->insertTab( widget_6, i18n( "Error" ) );

    grid_5->addWidget( tabScript, 1, 0 );

    script = new FileCombo( QDir::homeDirPath() ,widget_3, "script" );
    script->setMinimumSize( QSize( 64, 20 ) );
    script->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)5 ) );

    grid_5->addWidget( script, 0, 0 );
    actionTab->insertTab( widget_3, i18n( "Script" ) );

    QWidget* widget_7 = new QWidget( actionTab, "widget_7" );
    grid_12 = new QGridLayout( widget_7 );
    grid_12->setSpacing( 6 );
    grid_12->setMargin( 11 );

    text = new QMultiLineEdit( widget_7, "text" );

    grid_12->addWidget( text, 0, 0 );
    actionTab->insertTab( widget_7, i18n( "Text" ) );

    grid_2->addMultiCellWidget( actionTab, 1, 1, 0, 4 );

    actionName = new QLineEdit( GroupBox7, "actionName" );

    grid_2->addWidget( actionName, 0, 4 );

    grid->addWidget( GroupBox7, 0, 0 );

    hbox = new QHBoxLayout;
    hbox->setSpacing( 6 );
    hbox->setMargin( 0 );

    buttonHelp = new QPushButton( this, "buttonHelp" );
    buttonHelp->setText( i18n( "&Help"  ) );
    buttonHelp->setAutoDefault( TRUE );
    hbox->addWidget( buttonHelp );
    QSpacerItem* spacer_6 = new QSpacerItem( 235, 20, QSizePolicy::Expanding, QSizePolicy::Fixed );
    hbox->addItem( spacer_6 );

    buttonOk = new QPushButton( this, "buttonOk" );
    buttonOk->setText( i18n( "&OK"  ) );
    buttonOk->setAutoDefault( TRUE );
    buttonOk->setDefault( TRUE );
    hbox->addWidget( buttonOk );

    buttonCancel = new QPushButton( this, "buttonCancel" );
    buttonCancel->setText( i18n( "&Cancel"  ) );
    buttonCancel->setAutoDefault( TRUE );
    hbox->addWidget( buttonCancel );

    grid->addLayout( hbox, 1, 0 );

    // signals and slots connections
    connect( buttonOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( checkActionIconText, SIGNAL( toggled(bool) ), actionButtonText, SLOT( setEnabled(bool) ) );
    connect( checkActionIconText, SIGNAL( toggled(bool) ), actionIcon, SLOT( setDisabled(bool) ) );
    connect( checkSeparator, SIGNAL( toggled(bool) ), standardActions, SLOT( setDisabled(bool) ) );
    connect( useTagClose, SIGNAL( toggled(bool) ), tagClose, SLOT( setEnabled(bool) ) );
    connect( useTagClose, SIGNAL( toggled(bool) ), insertInLine, SLOT( setEnabled(bool) ) );


    standardActions->insertStrList(standardActionsList);
}

/*
 *  Destroys the object and frees any allocated resources
 */
ActionEditDialog::~ActionEditDialog()
{
    // no need to delete child widgets, Qt does it all for us
}


void replaceDomItem( QDomElement &el, QString name, QString value )
  {
    QDomDocument document = el.ownerDocument();
    QDomElement item = el.namedItem(name).toElement();
    if ( !item.isNull() )
      el.removeChild(item);
    item = document.createElement(name);
    el.appendChild(item);
    item.appendChild( document.createTextNode( value ) );
  }



void ActionEditDialog::operator << ( QDomElement &el)
{
  if ( el.hasAttribute("icon") )
    actionIcon->setIcon( el.attribute("icon") );
  actionName->setText( el.attribute("name") );



  QDomElement elxtag = el.namedItem("xtag").toElement();
  QDomElement eltag = el.namedItem("tag").toElement();

  tagClose->setText( elxtag.text() );
  tag->setText( eltag.text() );

  useTagClose->setChecked( elxtag.attribute("use","false") == "true" );
  insertInLine->setChecked( elxtag.attribute("inLine","true") == "true" );
  useActionDialog->setChecked( eltag.attribute("useDialog","false") == "true" );

  QDomElement elScript = el.namedItem("script").toElement();
  script->setText( elScript.text() );

  QString sInput  = elScript.attribute("input","none");
  QString sOutput = elScript.attribute("output","none");
  QString sError  = elScript.attribute("error","none");

  inputCurrent->setChecked( sInput == "current" );
  inputSelected->setChecked( sInput == "selected" );

  outputCursor ->setChecked( sOutput == "cursor" );
  outputMessage->setChecked( sOutput == "message" );
  outputNew    ->setChecked( sOutput == "new" );
  outputReplace->setChecked( sOutput == "replace" );

  errorCursor ->setChecked( sError == "cursor" );
  errorMessage->setChecked( sError == "message" );
  errorNew    ->setChecked( sError == "new" );
  errorReplace->setChecked( sError == "replace" );
  errorMerge  ->setChecked( sError == "merge" );

  text->setText( el.namedItem("text").toElement().text() );

  QString type = el.attribute("type","separator");

  if ( type == "tag" )
    actionTab->setCurrentPage(1);
  if ( type == "script" )
    actionTab->setCurrentPage(2);
  if ( type == "text" )
    actionTab->setCurrentPage(3);

  checkSeparator->setChecked( type == "separator" );

  QString standardAction = el.namedItem("standard").toElement().text();
  for ( int i=0; standardActionsList[i]; i++ )
    if ( standardAction == standardActionsList[i] )
      standardActions->setCurrentItem(i);

  checkActionIconText->setChecked( el.attribute("useTextButton","false") == "true" );
  actionButtonText->setText( el.attribute("textButton","") );

}


void ActionEditDialog::operator >> ( QDomElement &el )
{

  el.setAttribute("icon", actionIcon->icon() );
  el.setAttribute("name", actionName->text() );
  QString stab = actionTab->tabLabel( actionTab->currentPage() );

  QDomDocument document = el.ownerDocument();

  if ( stab == i18n("Standard") ) {

    if ( checkSeparator->isChecked() )
      el.setAttribute("type","separator");
    else {
      el.setAttribute("type","standard");
      replaceDomItem(el,"standard", standardActions->currentText() );
    }
  }

  if ( stab == i18n("Tag") ) {
    el.setAttribute("type","tag");
    replaceDomItem(el,"tag",tag->text() );
    replaceDomItem(el,"xtag",tagClose->text() );
    el.namedItem("xtag").toElement().setAttribute("use", useTagClose->isChecked() ? "true" : "false" );
    el.namedItem("xtag").toElement().setAttribute("inLine", insertInLine->isChecked() ? "true" : "false" );
    el.namedItem("tag").toElement().setAttribute("useDialog", useActionDialog->isChecked() ? "true" : "false" );
  }

  if ( stab == i18n("Text") ) {
    el.setAttribute("type","text");
    replaceDomItem(el,"text",text->text() );
  }

  if ( stab == i18n("Script") ) {
    el.setAttribute("type","script");
    replaceDomItem(el,"script", script->text() );

    QDomElement script = el.namedItem("script").toElement();

    if ( inputNone       ->isChecked() ) script.setAttribute("input","none");
    if ( inputCurrent    ->isChecked() ) script.setAttribute("input","current");
    if ( inputSelected   ->isChecked() ) script.setAttribute("input","selected");

    if ( outputNone      ->isChecked() ) script.setAttribute("output","none");
    if ( outputCursor    ->isChecked() ) script.setAttribute("output","cursor");
    if ( outputMessage   ->isChecked() ) script.setAttribute("output","message");
    if ( outputNew       ->isChecked() ) script.setAttribute("output","new");
    if ( outputReplace   ->isChecked() ) script.setAttribute("output","replace");

    if ( errorNone      ->isChecked() ) script.setAttribute("error","none");
    if ( errorCursor    ->isChecked() ) script.setAttribute("error","cursor");
    if ( errorMessage   ->isChecked() ) script.setAttribute("error","message");
    if ( errorNew       ->isChecked() ) script.setAttribute("error","new");
    if ( errorReplace   ->isChecked() ) script.setAttribute("error","replace");
    if ( errorMerge     ->isChecked() ) script.setAttribute("error","merge");
  }

  el.setAttribute( "useTextButton" , checkActionIconText->isChecked() );
  el.setAttribute(  "textButton", actionButtonText->text() );

}



