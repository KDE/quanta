/***************************************************************************
                          dtdparser.cpp  -  description
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

//qt includes
#include <qfile.h>
#include <qstring.h>

//kde includes
#include <kdebug.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kurl.h>
#include <kio/netaccess.h>

//other includes
#include <libxml/hash.h>
#include <libxml/parser.h>
#include <libxml/valid.h>

//own includes
#include "dtdparser.h"
#include "qtag.h"
#include "../quantacommon.h"
#include "../qextfileinfo.h"

namespace DTD
{
  QString dirName;
}

void saveElement(xmlElementPtr elem, xmlBufferPtr buf);

DTDParser::DTDParser(const KURL& dtdURL, const QString &dtepDir)
{
  m_dtdURL = dtdURL;
  m_dtepDir = dtepDir;
}

DTDParser::~DTDParser()
{
}

bool DTDParser::parse()
{
  QString fileName = QString::null;
  if (!KIO::NetAccess::download(m_dtdURL, fileName))
  {
    KMessageBox::error(0, i18n("<qt>Cannot download the DTD from <b>%1</b>.</qt>").arg( m_dtdURL.prettyURL(0, KURL::StripFileProtocol)));
    return false;
  }
  xmlDtdPtr      dtd_ptr; /* Pointer to a parsed DTD */
  xmlElementPtr  el_ptr; /* Pointer to an element description */
  xmlAttributePtr at_ptr;
  xmlEnumerationPtr enum_ptr;
  dtd_ptr = xmlParseDTD(NULL, xmlCharStrndup(fileName.utf8(), fileName.utf8().length()));
  if( dtd_ptr == NULL )
  {
     KMessageBox::error(0, i18n("Error while parsing the DTD.\nThe error message is:\n%1").arg("x"));
     return false;
//          xmlGenericError(xmlGenericErrorContext,                          "Could not parse %s\n", argv[1]);
  }
  DTD::dirName = m_dtepDir + "/" + QString((const char*)dtd_ptr->name);
  KURL u;
  u.setPath(DTD::dirName);
  if (!QExtFileInfo::createDir(DTD::dirName)) {
    QuantaCommon::dirCreationError(0, u);
    return false;
  }
  DTD::dirName.append("/");
  if (dtd_ptr->elements)
  {
    xmlHashScan((xmlElementTablePtr)dtd_ptr->elements, (xmlHashScanner)saveElement, 0);
  } else
  {
     KMessageBox::error(0, i18n("No elements were found in the DTD."));
     return false;
  }
  xmlFreeDtd(dtd_ptr);
  return true;
}

void saveElement(xmlElementPtr elem, xmlBufferPtr buf)
{
  if (elem)
  {
    QString elemName = QString((const char*)elem->name);
    QFile file( DTD::dirName + elemName + ".tag" );
    if ( file.open( IO_WriteOnly ) ) {
      kdDebug(24000) << "Element: " << elemName << endl;
      file.close();
    }
 }
}