/***************************************************************************
                          quantacommon.h  -  description
                             -------------------
    begin                : Sat Jul 27 2002
    copyright            : (C) 2002 by Andras Mantia
    email                : amantia@freemail.hu
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QUANTACOMMON_H
#define QUANTACOMMON_H

#include <qptrlist.h>

class QString;
/**Some common, mostly static functions.
  *@author Andras Mantia
  */

typedef struct Attribute{
        QString name;
        QString type;
        QString defaultValue;
        QString status; // "optional", "required","implied"
      };

typedef QPtrList<Attribute> AttributeList;

class QuantaCommon {
public: 
	QuantaCommon();
	~QuantaCommon();

  /** convert tag to upper or lower case */
  static QString tagCase( const QString tag);
  /** convert tag to upper or lower case */
  static QString attrCase( const QString attr);

};

#endif
