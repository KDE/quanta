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
#include <qdict.h>

//forward declarations
class KURL;
class QString;
struct Attribute;

/** libxml2 based XML DTD parser and DTEP creation class*/
class DTDParser {
public:
  DTDParser(const KURL& dtdURL, const QString &dtepDir);
  ~DTDParser();
  QString dirName();
  /**
   * Parse the DTD file.
   * @param targetDir the directory of the destination DTEP. If empty, a dialog is shown to configure the destination.
   * @param entitiesOnly if true, only the entities are extracted from the DTD into the entities.tag file 
   * @return true on success, false if some error happened
   */
  bool parse(const QString &targetDir = QString::null, bool entitiesOnly = false);

protected:
  void writeDescriptionRC();

private:
  KURL m_dtdURL;
  QString m_dtepDir;
  QString m_name;
  QString m_nickName;
  QString m_doctype;
  QString m_dtdURLLine;
  bool m_caseSensitive;
  QString m_defaultExtension;
  QDict<Attribute> m_tags;
};

#endif
