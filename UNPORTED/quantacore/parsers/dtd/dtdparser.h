/***************************************************************************
                          dtdparser.h  -  description
                             -------------------
    begin                : Sun Oct 19 16:47:20 EEST 2003
    copyright            : (C) 2003 Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/
#ifndef DTDPARSER_H
#define DTDPARSER_H

//qt includes
#include <QHash>

#include <kurl.h>


//forward declarations
class QString;
struct Attribute;

/** libxml2 based XML DTD parser and DTEP creation class*/
class DTDParser {
public:
  DTDParser(const KUrl& dtdURL, const QString &dtepDir);
  ~DTDParser();
  QString dirName();
  bool parse();

protected:
  void writeDescriptionRC();

private:
  KUrl m_dtdURL;
  QString m_dtepDir;
  QString m_name;
  QString m_nickName;
  QString m_doctype;
  QString m_dtdURLLine;
  bool m_caseSensitive;
  QString m_defaultExtension;
  QHash<QString, Attribute*> m_tags;
};

#endif
