/***************************************************************************
                             kafkasyncoptions.cpp
                             -------------------

    copyright            : (C) 2003 - Nicolas Deschildre
    email                : nicolasdchd@ifrance.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "kafkasyncoptions.h"
#include "kafkasyncoptions.moc"
#include <qradiobutton.h>
#include <qcheckbox.h>
#include <qspinbox.h>

#include <kconfig.h>

KafkaSyncOptions::KafkaSyncOptions( KConfig *config, QWidget* parent,  const char* name )
    : KafkaSyncOptionsUI( parent, name )
{
	this->config = config;
	config->setGroup("Kafka Synchronization options");

	bool showIcons  = config->readBoolEntry("Show Scripts Icons", true);
	QString sourceRefresh = config->readEntry("Source refresh", "delay");
	uint sourceRefreshDelay = config->readNumEntry("Source refresh delay", 4000);
	QString kafkaRefresh = config->readEntry("Kafka refresh", "focus");
	uint kafkaRefreshDelay = config->readNumEntry("Kafka refresh delay", 4000);

	if ( !name )
		setName( "kafkaSyncOptions" );

	showScriptsIcon->setChecked(showIcons);

	if(sourceRefresh == "focus")
		sourceFocusRefresh->setChecked(true);
	else if(sourceRefresh == "delay")
		sourceDelayRefresh->setChecked(true);
	sourceDelay->setValue(sourceRefreshDelay);

	if(kafkaRefresh == "focus")
		kafkaFocusRefresh->setChecked(true);
	else if(kafkaRefresh == "delay")
		kafkaDelayRefresh->setChecked(true);
	kafkaDelay->setValue(kafkaRefreshDelay);
}

KafkaSyncOptions::~KafkaSyncOptions()
{

}


void KafkaSyncOptions::updateConfig()
{
	config->setGroup("Kafka Synchronization options");

	config->writeEntry("Show Scripts Icons", showScriptsIcon->isChecked());

	if(sourceFocusRefresh->isChecked())
		config->writeEntry("Source refresh", "focus");
	else if(sourceDelayRefresh->isChecked())
		config->writeEntry("Source refresh", "delay");
	config->writeEntry("Source refresh delay", sourceDelay->value());

	if(kafkaFocusRefresh->isChecked())
		config->writeEntry("Kafka refresh", "focus");
	else if(kafkaDelayRefresh->isChecked())
		config->writeEntry("Kafka refresh", "delay");
	config->writeEntry("Kafka refresh delay", kafkaDelay->value());

}

