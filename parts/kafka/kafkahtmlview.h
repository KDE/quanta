/***************************************************************************
                               kafkahtmlview.h
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

#ifndef KAFKAHTMLVIEW_H
#define KAFKAHTMLVIEW_H

#include <khtmlview.h>

class KafkaWidget;

class KafkaHTMLView : public KHTMLView
{
Q_OBJECT
public:
	KafkaHTMLView(KafkaWidget *part, QWidget *parent, const char *name);
	~KafkaHTMLView();

protected:
	//void leaveEvent(QEvent *);
};

#endif
