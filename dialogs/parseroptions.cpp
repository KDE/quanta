/***************************************************************************
                          parseroptions.cpp  -  description
                             -------------------
    begin                : Wed Sep 20 2000
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

#include "parseroptions.h"

#include <qcombobox.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

#include <klocale.h>

/* 
 *  Constructs a ParserOptions which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 */
ParserOptions::ParserOptions( QWidget* parent,  const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
		  setName( "ParserOptions" );
    resize( 400, 330 );
    grid = new QGridLayout( this );
    grid->setSpacing( 13 );
    grid->setMargin( 1 );

    GroupBox2 = new QGroupBox( this, "GroupBox2" );
    GroupBox2->setTitle( i18n( "Clicks on list items:"  ) );
    GroupBox2->setMargin( 10 );
    GroupBox2->setColumnLayout(0, Qt::Vertical );
    GroupBox2->layout()->setSpacing( 0 );
    GroupBox2->layout()->setMargin( 0 );
    grid_2 = new QGridLayout( GroupBox2->layout() );
    grid_2->setAlignment( Qt::AlignTop );
    grid_2->setSpacing( 6 );
    grid_2->setMargin( 11 );
    QSpacerItem* spacer = new QSpacerItem( 109, 20, QSizePolicy::Expanding, QSizePolicy::Fixed );
    grid_2->addItem( spacer, 1, 1 );

    comboMBM = new QComboBox( FALSE, GroupBox2, "comboMBM" );

    comboMBM -> insertItem(i18n("Find tag and open tree"));
    comboMBM -> insertItem(i18n("Find tag"));
    comboMBM -> insertItem(i18n("Select tag area"));
    comboMBM -> insertItem(i18n("nothing"));



    grid_2->addWidget( comboMBM, 1, 2 );

    comboRBM = new QComboBox( FALSE, GroupBox2, "comboRBM" );
    comboRBM -> insertItem(i18n("Popup menu"));

    grid_2->addWidget( comboRBM, 2, 2 );
    QSpacerItem* spacer_2 = new QSpacerItem( 109, 20, QSizePolicy::Expanding, QSizePolicy::Fixed );
    grid_2->addItem( spacer_2, 2, 1 );
    QSpacerItem* spacer_3 = new QSpacerItem( 109, 20, QSizePolicy::Expanding, QSizePolicy::Fixed );
    grid_2->addItem( spacer_3, 3, 1 );

    comboLBM = new QComboBox( FALSE, GroupBox2, "comboLBM" );

    comboLBM -> insertItem(i18n("Find tag"));
    comboLBM -> insertItem(i18n("Find tag and open tree"));



    grid_2->addWidget( comboLBM, 0, 2 );
    QSpacerItem* spacer_4 = new QSpacerItem( 109, 20, QSizePolicy::Expanding, QSizePolicy::Fixed );
    grid_2->addItem( spacer_4, 0, 1 );

    comboDoubleClick = new QComboBox( FALSE, GroupBox2, "comboDoubleClick" );

    comboDoubleClick -> insertItem( i18n("Select tag area") );
    comboDoubleClick -> insertItem( i18n("nothing") );


    grid_2->addWidget( comboDoubleClick, 3, 2 );

    TextLabel2_2 = new QLabel( GroupBox2, "TextLabel2_2" );
    TextLabel2_2->setText( i18n( "Left button"  ) );

    grid_2->addWidget( TextLabel2_2, 0, 0 );

    TextLabel3_2 = new QLabel( GroupBox2, "TextLabel3_2" );
    TextLabel3_2->setText( i18n( "Middle button"  ) );

    grid_2->addWidget( TextLabel3_2, 1, 0 );

    TextLabel4_2 = new QLabel( GroupBox2, "TextLabel4_2" );
    TextLabel4_2->setText( i18n( "Right button"  ) );

    grid_2->addWidget( TextLabel4_2, 2, 0 );

    TextLabel5_2 = new QLabel( GroupBox2, "TextLabel5_2" );
    TextLabel5_2->setText( i18n( "Double click"  ) );

    grid_2->addWidget( TextLabel5_2, 3, 0 );

    grid->addMultiCellWidget( GroupBox2, 0, 0, 0, 1 );
    QSpacerItem* spacer_5 = new QSpacerItem( 20, 79, QSizePolicy::Fixed, QSizePolicy::Expanding );
    grid->addItem( spacer_5, 3, 0 );

    TextLabel7 = new QLabel( this, "TextLabel7" );
    TextLabel7->setText( i18n( "( set 0 to expand whole tree )"  ) );

    grid->addMultiCellWidget( TextLabel7, 2, 2, 0, 1 );

    spinExpand = new QSpinBox( this, "spinExpand" );
    spinExpand->setMaximumSize( QSize( 70, 32767 ) );

    grid->addWidget( spinExpand, 1, 1 );

    TextLabel6 = new QLabel( this, "TextLabel6" );
    TextLabel6->setText( i18n( "Expand tree when reparse to level :"  ) );

    grid->addWidget( TextLabel6, 1, 0 );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
ParserOptions::~ParserOptions()
{
    // no need to delete child widgets, Qt does it all for us
}

