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
#include <qcheckbox.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qregexp.h>
#include <qstring.h>

//kde includes
#include <kconfig.h>
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
#include "dtepcreationdlg.h"
#include "../quantacommon.h"
#include "../qextfileinfo.h"

#define MAX_CHILD_ELEMENTS 100

namespace DTD
{
  QString dirName;
  xmlDtdPtr dtd_ptr; /* Pointer to the parsed DTD */
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
  DTD::dtd_ptr = xmlParseDTD(NULL, xmlCharStrndup(fileName.utf8(), fileName.utf8().length()));
  if( DTD::dtd_ptr == NULL )
  {
     KMessageBox::error(0, i18n("Error while parsing the DTD.\nThe error message is:\n%1").arg("x"));
     return false;
//          xmlGenericError(xmlGenericErrorContext,                          "Could not parse %s\n", argv[1]);
  }
  DTEPCreationDlg dlg;
  QString name = QString((const char*)DTD::dtd_ptr->name);
  if (name == "none")
    name = QFileInfo(m_dtdURL.fileName()).baseName();
  dlg.dtdName->setText(name);
  dlg.nickName->setText(name);
  dlg.directory->setText(QFileInfo(m_dtdURL.fileName()).baseName());
  dlg.doctype->setText(QString((const char*)DTD::dtd_ptr->ExternalID));
  dlg.dtdURL->setText(QString((const char*)DTD::dtd_ptr->SystemID));
  if (!dlg.exec())
      return false;
  m_name = dlg.dtdName->text();
  m_nickName = dlg.nickName->text();
  m_doctype = dlg.doctype->text();
  m_doctype.replace(QRegExp("<!doctype", false), "");
  m_doctype = m_doctype.left(m_doctype.findRev(">") - 1);
  m_dtdURLLine = dlg.dtdURL->text();
  m_defaultExtension = dlg.defaultExtension->text();
  m_caseSensitive = dlg.caseSensitive->isChecked();
  DTD::dirName = m_dtepDir + "/" + dlg.directory->text();
  KURL u;
  u.setPath(DTD::dirName);
  if (!QExtFileInfo::createDir(DTD::dirName)) {
    QuantaCommon::dirCreationError(0, u);
    return false;
  }
  DTD::dirName.append("/");
  if (DTD::dtd_ptr->elements)
  {
    xmlHashScan((xmlElementTablePtr)DTD::dtd_ptr->elements, (xmlHashScanner)saveElement, 0);
  } else
  {
     KMessageBox::error(0, i18n("No elements were found in the DTD."));
     return false;
  }
  xmlFreeDtd(DTD::dtd_ptr);
  writeDescriptionRC();

  return true;
}

void DTDParser::writeDescriptionRC()
{
  KConfig config(DTD::dirName + "description.rc");
  config.setGroup("General");
  config.writeEntry("Name", m_name);
  config.writeEntry("NickName", m_nickName);
  config.writeEntry("DoctypeString", m_doctype);
  config.writeEntry("URL", m_dtdURLLine);
  config.writeEntry("DefaultExtension", m_defaultExtension);
  config.writeEntry("Family", "1");
  config.writeEntry("CaseSensitive", m_caseSensitive);
  config.setGroup("Parsing rules");
  config.writeEntry("SpecialAreas","<!-- -->");
  config.writeEntry("SpecialAreaNames","comment");

  config.sync();
}

void saveElement(xmlElementPtr elem, xmlBufferPtr buf)
{
  Q_UNUSED(buf);
  if (elem)
  {
    QString elemName = QString((const char*)elem->name);
    QFile file( DTD::dirName + elemName + ".tag" );
    if ( file.open( IO_WriteOnly ) )
    {
      QTextStream stream( &file );
      stream << "<!DOCTYPE TAGS>" << endl
             << "<TAGS>" << endl
             << "<tag name=\"" << elemName << "\">" << endl << endl;

      xmlElementPtr  el_ptr; /* Pointer to an element description */
      xmlAttributePtr at_ptr;
      el_ptr = xmlGetDtdElementDesc(DTD::dtd_ptr, elem->name);
      AttributeList attributes;
      attributes.setAutoDelete(true);
      if (el_ptr)
      {
        at_ptr = el_ptr->attributes;
        while (at_ptr) {
          Attribute *attr =  new Attribute;
          attr->name = QString((const char*)at_ptr->name);
          switch (at_ptr->def) {
            case 1: {attr->status = "optional"; break;} //NONE
            case 2: {attr->status = "required"; break;} //REQUIRED
            case 3: {attr->status = "implied"; break;} //IMPLIED
            case 4: {attr->status = "fixed"; break;} //FIXED
          }
          attr->defaultValue = QString((const char*)at_ptr->defaultValue);
          xmlEnumerationPtr enum_ptr;
          enum_ptr = at_ptr->tree;
          while (enum_ptr) {
            attr->values += QString((const char*)enum_ptr->name);
            enum_ptr = enum_ptr->next;
          }
          QString attrtype;
          switch (at_ptr->atype) {
            case 9: {attrtype = "list"; break;}
            default: {attrtype = "input"; break;} //TODO handle the rest of types
          }
          attr->type = attrtype;
          attributes.append(attr);
          at_ptr = at_ptr->nexth;
       }
      }
      if (!attributes.isEmpty())
        stream << QuantaCommon::xmlFromAttributes(&attributes);
      const xmlChar *list_ptr[MAX_CHILD_ELEMENTS];
      int childNum = 0;
      childNum = xmlValidGetPotentialChildren(el_ptr->content, list_ptr,
                        &childNum, MAX_CHILD_ELEMENTS);

      if (childNum > 0)
      {
        stream << "<children>" << endl;
        for( int i = 0; i < childNum; i++ )
        {
          stream << "  <child name=\"" << QString((const char*)list_ptr[i]) << "\"";
          xmlElementPtr child_ptr = xmlGetDtdElementDesc(DTD::dtd_ptr, list_ptr[i]);
          if (child_ptr && child_ptr->content && child_ptr->content->ocur)
          {
            if (child_ptr->content->ocur == XML_ELEMENT_CONTENT_PLUS)
            {
              stream << " usage=\"required\"";
            }
          }
          stream << " />" << endl;
        }
        stream << "</children>" << endl;
      }
      stream << "</tag>" << endl
             << "</TAGS>" << endl;
      file.close();
    }
 }
}

QString DTDParser::dirName()
{
  return DTD::dirName;
}

