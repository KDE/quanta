/***************************************************************************
                              kafkahtmlview.cpp
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

#include "kafkahtmlview.moc"
#include "kafkahtmlpart.h"

KafkaHTMLView::KafkaHTMLView(KafkaHTMLPart *part, QWidget *parent, const char *name)
	: KHTMLView(part, parent, name)
{
}

KafkaHTMLView::~KafkaHTMLView()
{
}

void KafkaHTMLView::leaveEvent(QEvent *)
{
    setCursor(Qt::arrowCursor);
}

