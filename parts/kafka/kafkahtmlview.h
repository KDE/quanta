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

#include "kafkahtmlpart.h"


class KafkaHTMLView : public KHTMLView
{
Q_OBJECT
public: 
	KafkaHTMLView(KafkaHTMLPart *part, QWidget *parent, const char *name);
	~KafkaHTMLView();

protected:
	void leaveEvent(QEvent *);
};

#endif
