/***************************************************************************
                          parseroptions.cpp  -  description
                             -------------------
    begin                : Fri Aug 23 2002
    copyright            : (C) 2002 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/

#include "parseroptions.h"
#include "parseroptions.moc"
#include <qcombobox.h>
#include <qspinbox.h>

#include <klocale.h>
#include <kconfig.h>

/*
 *  Constructs a ParserOptions which is a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'
 */
ParserOptions::ParserOptions( KConfig *a_config, QWidget* parent,  const char* name )
    : ParserOptionsUI( parent, name )
{
    config = a_config;
    config->setGroup("Parser options");

    QString handleLBM = config->readEntry("LBM", i18n("Find Tag & Open Tree"));
    QString handleMBM = config->readEntry("MBM", i18n("Nothing"));
    QString handleRBM = config->readEntry("RBM", i18n("Popup Menu"));
    QString handleDoubleClick = config->readEntry("Double click", i18n("Select Tag Area"));


    if ( !name )
      setName( "ParserOptions" );

    if ( handleLBM == i18n("Find Tag & Open Tree") )
    {
      comboLBM->setCurrentItem(1);
    }
    else
    {
      comboLBM->setCurrentItem(0);
    }

    if ( handleMBM == i18n("Find Tag & Open Tree") ) comboMBM->setCurrentItem(0);
    if ( handleMBM == i18n("Find Tag") ) comboMBM->setCurrentItem(1);
    if ( handleMBM == i18n("Go to End of Tag") ) comboMBM->setCurrentItem(2);
    if ( handleMBM == i18n("Select Tag Area") ) comboMBM->setCurrentItem(3);
    if ( handleMBM == i18n("Nothing")) comboMBM->setCurrentItem(4);

    if ( handleDoubleClick == i18n("Select Tag Area") )
    {
      comboDoubleClick->setCurrentItem(0);
    }
    else
    {
      comboDoubleClick->setCurrentItem(1);
    }
}

/*
 *  Destroys the object and frees any allocated resources
 */
ParserOptions::~ParserOptions()
{
    // no need to delete child widgets, Qt does it all for us
}


void ParserOptions::updateConfig()
{

    config->setGroup("Parser options");

    config->writeEntry("MBM",comboMBM->currentText());
    config->writeEntry("LBM",comboLBM->currentText());
    config->writeEntry("RBM",comboRBM->currentText());
    config->writeEntry("Double click",comboDoubleClick->currentText());
}
