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

#ifndef KAFKADRAGOBJECT_H
#define KAFKADRAGOBJECT_H

#include <qcstring.h>
#include <qdragobject.h>

class Node;

/**
@author Paulo Moura Guedes
*/
class KafkaDragObject: public QStoredDrag
{
    Q_OBJECT

public:
    KafkaDragObject(Node const* item, QWidget* dragSource = 0, const char* name = 0);
    ~KafkaDragObject();

    QByteArray encodedData(const char* c) const;
    Node const* getItem() const;
    
    static bool canDecode(QMimeSource*);
    static bool decode(QMimeSource*, Node* node);

private:
    QByteArray m_array;
    Node const* m_item;    
    
    static char const* m_mimetype;
};

#endif
