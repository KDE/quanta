/***************************************************************************
                          toolbarconfig.cpp  -  description
                             -------------------
    begin                : Sat Apr 8 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavsky
    email                : pdima@mail.univ.kiev.ua
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

// KDE files
#include <kfm.h>
#include <kapp.h>
#include <kiconloader.h>
#include <kfiledialog.h>

//Qt files
#include <qdir.h>
#include <qheader.h>
#include <qlistview.h>

#include "toolbarconfig.h"
#include "toolbarscriptconfigure.h"

ToolBarConfig::ToolBarConfig(KConfig *config, QWidget *parent, const char *name)
    : QDialog(parent,name,true)
{
	this->config = config;
	setCaption( name );
	initDialog();
	
	buttonNew   ->setPixmap( Icon("new.xpm") );
	buttonUp    ->setPixmap( Icon("up.xpm") );
	buttonDown  ->setPixmap( Icon("down.xpm") );
	buttonDel   ->setPixmap( Icon("delete.xpm") );
	buttonIcon  ->setPixmap( Icon("open.xpm") );
	buttonKIconEdit->setPixmap( Icon("mini/kiconedit.xpm") );
	buttonConfigScript->setPixmap( Icon("configure.xpm") );
	
	groupBox->insert(radioInsertTag);
	groupBox->insert(radioExecuteScript);
	groupBox->setExclusive(true);
	
	list->setFrameStyle( QFrame::Panel | QFrame::Plain );
  list->setLineWidth( 1 );
//  list->addColumn( " " );
  list->addColumn( "Name" );

  list->setColumnWidth(0,200);
  list->setColumnWidthMode(0, QListView::Manual);

  maxpos = 0;


//  list->header()->hide();

  slotRadioToggled();
  slotHaveEndTag();

  connect( buttonOk,    SIGNAL(clicked()), SLOT(accept()) );
	connect( buttonCancel,SIGNAL(clicked()), SLOT(reject()) );
	
	connect( buttonIcon,      SIGNAL(clicked()), this, SLOT(slotOpenIcon()));
	connect( buttonKIconEdit, SIGNAL(clicked()), this, SLOT(slotKIconEdit()));
	connect( buttonConfigScript, SIGNAL(clicked()), this, SLOT(slotConfigScript()));
	
	connect( checkSeparator, SIGNAL(toggled(bool)), this, SLOT(slotSeparator(bool)) );
	connect( radioInsertTag, SIGNAL(clicked()), this, SLOT( slotRadioToggled()));
	connect( radioExecuteScript, SIGNAL(clicked()), this, SLOT( slotRadioToggled()));
	connect( checkEndTag, SIGNAL(clicked()), this, SLOT(slotHaveEndTag()));
	connect( buttonNew, SIGNAL(clicked()), this, SLOT(listNew()));
	connect( buttonUp, SIGNAL(clicked()), this, SLOT(listUp()));
	connect( buttonDown, SIGNAL(clicked()), this, SLOT(listDown()));
	connect( buttonDel, SIGNAL(clicked()), this, SLOT(listDel()));
	
	connect( lineName, SIGNAL(textChanged( const char *)), this, SLOT(slotLineChanged(const char *)));
	connect( lineIcon, SIGNAL(textChanged( const char *)), this, SLOT(slotLineChanged(const char *)));
	connect( lineTag,  SIGNAL(textChanged( const char *)), this, SLOT(slotLineChanged(const char *)));
	connect( lineEndTag, SIGNAL(textChanged( const char *)), this, SLOT(slotLineChanged(const char *)));
	connect( lineScript, SIGNAL(textChanged( const char *)), this, SLOT(slotLineChanged(const char *)));
	
	connect( list, SIGNAL( currentChanged( QListViewItem *)), this, SLOT( listCurrentChanged( QListViewItem *)) );
	
	readConfig();
}

ToolBarConfig::~ToolBarConfig(){
}

/** togle separator | button */
void ToolBarConfig::slotSeparator( bool toggled)
{
  ToolBarItem *item = (ToolBarItem *)list->currentItem();
  if ( !item ) return;

  if ( toggled ) {
    lineName->setEnabled(false);
    lineIcon->setEnabled(false);
    lineTag->setEnabled(false);
    lineEndTag->setEnabled(false);
    lineScript->setEnabled(false);
    radioExecuteScript->setEnabled(false);
    radioInsertTag->setEnabled(false);
    checkEndTag->setEnabled(false);
    buttonIcon->setEnabled(false);
    buttonKIconEdit->setEnabled(false);
    buttonConfigScript->setEnabled(false);

    item->setText(0," ");
    item->setPixmap(0, Icon("tag_hr.xpm") );
  }
  else {
    lineName->setEnabled(true);
    lineIcon->setEnabled(true);
    buttonIcon->setEnabled(true);
    buttonKIconEdit->setEnabled(true);
    radioExecuteScript->setEnabled(true);
    radioInsertTag->setEnabled(true);

    slotRadioToggled();

    item->setText(0, lineName->text() );
    item->setIcon( lineIcon->text() );
  }

  item->data.isSeparator = toggled;
}

/**  */
void ToolBarConfig::slotRadioToggled()
{
  ToolBarItem *item = (ToolBarItem *)list->currentItem();
  if ( !item ) return;

  if ( radioInsertTag->isChecked() ) {
    lineTag->setEnabled(true);
    checkEndTag->setEnabled(true);
    slotHaveEndTag();

    lineScript->setEnabled(false);
    buttonConfigScript->setEnabled(false);

    item->data.type = ToolBarData::Tag;
  }
  else {
    lineTag->setEnabled(false);
    lineEndTag->setEnabled(false);
    checkEndTag->setEnabled(false);

    lineScript->setEnabled(true);
    buttonConfigScript->setEnabled(true);

    item->data.type = ToolBarData::Script;
  }
}

/**  */
void ToolBarConfig::slotHaveEndTag()
{
  ToolBarItem *item = (ToolBarItem *)list->currentItem();
  if ( !item ) return;

  lineEndTag->setEnabled ( checkEndTag->isChecked() );

  item->data.haveEndTag = checkEndTag->isChecked();
}

/** select icon file name */
void ToolBarConfig::slotOpenIcon()
{
  QString path = QDir::homeDirPath()+"/.kde/share/apps/quanta/";
  QDir dir( path );
  if ( !dir.exists() ) {
    dir.mkdir( path );

    QString src  = KApplication::kde_datadir() + "/quanta/pics/ball.xpm";
    QString dest = path + "ball.xpm";

    KFM kfm;
    kfm.copy( src, dest );
  }

  QString fileName = KFileDialog::getOpenFileName( path, "*.xpm|Xpm files\n*|All files");

	if (fileName.isEmpty()) return;
	lineIcon->setText( fileName.data() );
	
	ToolBarItem *item = (ToolBarItem *)list->currentItem();
  if (item)
    item->setIcon( fileName );
	
}

/** open icon in kiconedit */
void ToolBarConfig::slotKIconEdit()
{
  KFM kfm;
  QString fileName = lineIcon->text();

  if ( !fileName.isEmpty() )
  {
    QFile fi( fileName );
    if ( !fi.exists() ) {
      QString src  = KApplication::kde_datadir() + "/quanta/pics/ball.xpm";
      kfm.copy( src, fileName );
      sleep(2);
    }
    kfm.openURL( fileName );
  }
}

/**  */
void ToolBarConfig::listNew()
{
  ToolBarItem *item = new ToolBarItem(list,"new item", maxpos++ );
  list->setCurrentItem(item);
  list->setSelected(item,true);
}

void ToolBarConfig::listUp()
{
  ToolBarItem *item = (ToolBarItem *)list->currentItem();
  if (!item) return;
  ToolBarItem *above = (ToolBarItem *)item->itemAbove();
  if (!above) return;
  // swap pos of item and above :
  int oldpos = item->pos;
  item->pos = above->pos;
  above->pos = oldpos;
  list->setSorting(0,true);

  // I didn't another way to resort list ...
  ToolBarItem *tmp = new ToolBarItem(list,"");
  delete(tmp);

  list->setCurrentItem(item);
  list->setSelected(item,true);

}

void ToolBarConfig::listDown()
{
  ToolBarItem *item = (ToolBarItem *)list->currentItem();
  if (!item) return;
  ToolBarItem *below = (ToolBarItem *)item->itemBelow();
  if (!below) return;
  // swap pos of item and above :
  int oldpos = item->pos;
  item->pos = below->pos;
  below->pos = oldpos;
  list->setSorting(0,true);

  // I didn't another way to resort list ...
  ToolBarItem *tmp = new ToolBarItem(list,"");
  delete(tmp);

  list->setCurrentItem(item);
  list->setSelected(item,true);
}

void ToolBarConfig::listDel()
{

  delete( list->currentItem() );
  QListViewItem  *item = list->firstChild();
  if ( !item ) return;

  list->setCurrentItem(item);
  list->setSelected(item,true);
}

/**  */
void ToolBarConfig::slotLineChanged( const char *text)
{
  ToolBarItem *item = (ToolBarItem *)list->currentItem();
  if ( !item ) return;
  ToolBarData *data  = &item->data;

  item->setText( lineName->text() );

  data->iconFile = lineIcon->text();
  data->tag      = lineTag->text();
  data->endTag   = lineEndTag->text();
  data->script   = lineScript->text();
}

/**  */
void ToolBarConfig::listCurrentChanged( QListViewItem *newItem )
{

  ToolBarItem *item = (ToolBarItem *)newItem;
  ToolBarData data  = item->data;

  lineName->setText( data.name );
  lineTag->setText( data.tag );
  lineEndTag->setText( data.endTag );
  lineScript->setText( data.script );
  lineIcon->setText( data.iconFile);
  checkEndTag->setChecked( data.haveEndTag );
  checkSeparator->setChecked( data.isSeparator );

  if ( data.type == ToolBarData::Tag ) groupBox->setButton(0);
  else groupBox->setButton(1);

  slotHaveEndTag();
  slotRadioToggled();
  slotSeparator( data.isSeparator );

}

/** save toolbar configuration */
void ToolBarConfig::saveConfig()
{
  config->setGroup("User Toolbars");

  config->writeEntry("Count", list->childCount() );

  ToolBarItem *item;
  item = (ToolBarItem *)list->firstChild();

  int i = 0;
  while ( item )
  {
    QString head = QString().sprintf("Toolbar %d",i);
    config->setGroup( head.data() );

    ToolBarData data = item->data;

    config->writeEntry("Name", data.name);
    config->writeEntry("Icon", data.iconFile);
    config->writeEntry("Is Separator", data.isSeparator);
    config->writeEntry("Type", data.type);
    config->writeEntry("Have End Tag", data.haveEndTag);
    config->writeEntry("Tag", data.tag);
    config->writeEntry("EndTag", data.endTag);
    config->writeEntry("Script", data.script);
    config->writeEntry("Script input", data.input);
    config->writeEntry("Script output", data.output);

    item = (ToolBarItem *)item->nextSibling();
    i++;
  }
}
/** read configuration */
void ToolBarConfig::readConfig()
{
  config->setGroup("User Toolbars");

  int count = config->readNumEntry("Count",0);

  for( int i = 0;i<count;i++)
  {
    QString head = QString().sprintf("Toolbar %d",i);
    config->setGroup( head.data() );

    listNew();
    ToolBarItem *item = (ToolBarItem *)list->currentItem();
    ToolBarData *data  = &item->data;

    item->setText( config->readEntry("Name"));
    item->setIcon( config->readEntry("Icon"));
    data->isSeparator = config->readBoolEntry("Is Separator");
    data->type        = config->readNumEntry("Type");
    data->haveEndTag  = config->readBoolEntry("Have End Tag");
    data->tag    = config->readEntry("Tag");
    data->endTag = config->readEntry("EndTag");
    data->script = config->readEntry("Script");
    data->input  = config->readNumEntry("Script input");
    data->output = config->readNumEntry("Script output");

    listCurrentChanged( list->currentItem() );

  }
}

/**  */
void ToolBarConfig::slotConfigScript()
{
  ToolbarScriptConfigure *dlg = new ToolbarScriptConfigure(0L,"Configure script...");

  ToolBarItem *item = (ToolBarItem *)list->currentItem();
  if ( !item ) return;
  ToolBarData *data  = &item->data;

  switch ( data->input ) {
    case ToolBarData::None:
      dlg->radioInputNone   ->setChecked(true);
    break;
    case ToolBarData::SelectedText:
      dlg->radioSelectedText->setChecked(true);
    break;
    case ToolBarData::CurrentDoc:
      dlg->radioCurrentDoc  ->setChecked(true);
    break;
  }

  switch ( data->output ) {
    case ToolBarData::None:
      dlg->radioOutputNone   ->setChecked(true);
    break;
    case ToolBarData::InsInCurPos:
      dlg->radioInsInCurPos  ->setChecked(true);
    break;
    case ToolBarData::ReplaceCurDoc:
      dlg->radioReplaceCurDoc->setChecked(true);
    break;
    case ToolBarData::CreateNewDoc:
      dlg->radioCreateNewDoc ->setChecked(true);
    break;
    case ToolBarData::InMesWindow:
      dlg->radioInMesWindow  ->setChecked(true);
    break;
  }

  if ( dlg->exec() )
  {
    if ( dlg->radioInputNone   ->isChecked() ) data->input = ToolBarData::None;
    if ( dlg->radioSelectedText->isChecked() ) data->input = ToolBarData::SelectedText;
    if ( dlg->radioCurrentDoc  ->isChecked() ) data->input = ToolBarData::CurrentDoc;

    if ( dlg->radioOutputNone   ->isChecked() ) data->output = ToolBarData::None;
    if ( dlg->radioInsInCurPos  ->isChecked() ) data->output = ToolBarData::InsInCurPos;
    if ( dlg->radioReplaceCurDoc->isChecked() ) data->output = ToolBarData::ReplaceCurDoc;
    if ( dlg->radioCreateNewDoc ->isChecked() ) data->output = ToolBarData::CreateNewDoc;
    if ( dlg->radioInMesWindow  ->isChecked() ) data->output = ToolBarData::InMesWindow;
  }

  delete (dlg);
}
