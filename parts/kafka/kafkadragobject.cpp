/***************************************************************************
                               kafkadragobject.h
                             -------------------
 
    copyright            : (C) 2004 - Paulo Moura Guedes
    email                : moura@kdewebdev.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <kdebug.h>

#include <qdom.h> 
#include <qtextstream.h> 
 
#include "kafkadragobject.h"
#include "node.h"
#include "kafkacommon.h" // for debug only

char const* KafkaDragObject::m_mimetype = "application/kafka-nodeselection";

KafkaDragObject::KafkaDragObject(Node const* item, QWidget* dragSource, const char* name)
    : QStoredDrag(m_mimetype, dragSource, name), m_item(item)
{
    QDomDocument doc("KafkaSelection");
    QString docstart = "<KafkaSelection/>";
    doc.setContent(docstart);
    QDomElement itemsElement = doc.createElement("Items");
    doc.documentElement().appendChild(itemsElement);
    //kdDebug(25001) << doc.toString() << endl;
    item->save(itemsElement);
    //QDataStream s(m_array, IO_WriteOnly);
    QTextStream s(m_array, IO_WriteOnly);
    s << doc.toString();
    //s << "biltre";
    /*
    kdDebug(25001) << m_array.size() << endl;    
    kdDebug(25001) << doc.toString() << endl;
    kdDebug(25001) << QString(m_array) << endl;
    */
    //setEncodedData(m_array);
}

KafkaDragObject::~KafkaDragObject()
{}

QByteArray KafkaDragObject::encodedData(const char* mimetype) const
{
    if(m_mimetype == mimetype)
        return m_array;
    
    return QByteArray();
}

Node const* KafkaDragObject::getItem() const
{
    return m_item;
}

bool KafkaDragObject::canDecode(QMimeSource *e)
{
    return e->provides(m_mimetype);
}

bool KafkaDragObject::decode(QMimeSource* e, Node* node)
{
    if(!node || !e->provides(m_mimetype))
        return false;
    
    QByteArray data = e->encodedData(m_mimetype);    
    //kdDebug(25001) << QString(data) << endl;
    QDomDocument doc;
    doc.setContent(data, false);
    //kdDebug(25001) << doc.toString() << endl;
    node->load(doc.documentElement().firstChild().toElement());
    
    kdDebug(25001) << "Load: " << endl;
    kafkaCommon::coutTree(node, 3);
    
    return node;
}

#include "kafkadragobject.moc"
