/***************************************************************************
                          dtdparser.cpp  -  description
                             -------------------
    begin                : Tue Jul 30 15:26:20 EEST 2002
    copyright            : (C) 2002 by Jason P. Hanley <jphanley@buffalo.edu>
                           (C) 2002, 2003 Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef DTD_H
#define DTD_H

//qt includes
#include <qdict.h>

//app includes
#include "qtag.h"

class KURL;

class DTD
{

public:
  DTD(const KURL &dtdURL, const QString& dtepDir);
  ~DTD();

public:
  QStringList getTags();
  AttributeList* getTagAttributes(QString tag);
  QStringList getTextCompletion(QString tag);
  void printContents();
  void writeTagFiles();
  /** No descriptions */
  bool parseDTD();

private:
  bool parseDTD(const KURL& url);
  void parseDTDEntity(const QString &line);
  void parseDTDElement(const QString &line);
  void parseDTDAttlist(const QString &line);
  void parseTagAttributeValues(const QString &name, QString *value);
  void parseDTDReplace(QString *value);
  void stripSpaces(QString *value);
  void removeComments(QString &value);

  QDict<QString> entities;
  QDict<QString> elements;
  QStringList tags;
  QDict<AttributeList> tagAttributes;
  /** From where to load the DTD file. */
  KURL m_dtdURL;
  QString m_dtepDir;
};

#endif
