/***************************************************************************
                               kafkaresource.h
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

#ifndef KAFKARESOURCE_H
#define KAFKARESOURCE_H

/**
 * A few globals variables avoiding to use the quantaApp global variables :
 * quantaApp->view()->kafkaInterface()->getKafkaPart()...
 * Also a (little) step torwards kafka independance from the quanta code (to be able to make it a kpart, at last!)
 */

class KafkaDocument;
class KafkaWidget;

extern KafkaWidget *kafkaWidget;
extern KafkaDocument *kafkaDoc;

#endif

