/***************************************************************************
                            kafkahtmlpartplugin.h
                             -------------------

    copyright            : (C) 2001 - The Kafka Team
    email                : kde-kafka@master.kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef KAFKAHTMLPARTPLUGIN_H
#define KAFKAHTMLPARTPLUGIN_H

#include <kparts/part.h>
#include <dom/dom_node.h>

class KafkaHTMLPartPlugin : public KParts::ReadWritePart
{
Q_OBJECT
public: 
	KafkaHTMLPartPlugin(QObject * parent=0, const char * name=0):KParts::ReadWritePart(parent,name){;}
	~KafkaHTMLPartPlugin(){;}
        virtual QWidget *showControl(QWidget *)=0;
        virtual void cleanupControl()=0;
        virtual void setNode(DOM::Node)=0;
	void emitChanged(DOM::Node pNode){ emit sigChanged(pNode);}
    signals:
        void sigChanged(DOM::Node);
};

#endif
