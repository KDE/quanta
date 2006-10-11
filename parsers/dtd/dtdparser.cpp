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
#include <kdialogbase.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kurl.h>
#include <kio/netaccess.h>

//other includes
#ifdef LIBXML_2_5
#include <libxml/hash.h>
#endif

#include <libxml/parser.h>
#include <libxml/valid.h>

//own includes
#include "dtepeditdlg.h"
#include "dtdparser.h"
#include "qtag.h"
#include "dtepcreationdlg.h"
#include "quantacommon.h"
#include "qextfileinfo.h"

#define MAX_CHILD_ELEMENTS 100

namespace DTD
{
  QString dirName;
  xmlDtdPtr dtd_ptr; /* Pointer to the parsed DTD */
  QTextStream entityStream;
}

void saveElement(xmlElementPtr elem, xmlBufferPtr buf);
void saveEntity(xmlEntityPtr entity, xmlBufferPtr buf);

DTDParser::DTDParser(const KURL& dtdURL, const QString &dtepDir)
{
  m_dtdURL = dtdURL;
  m_dtepDir = dtepDir;
}

DTDParser::~DTDParser()
{
}

bool DTDParser::parse(const QString &targetDir, bool entitiesOnly)
{
  bool fineTune = false;
  QString fileName = QString::null;
  if (!KIO::NetAccess::download(m_dtdURL, fileName, 0))
  {
    KMessageBox::error(0, i18n("<qt>Cannot download the DTD from <b>%1</b>.</qt>").arg( m_dtdURL.prettyURL(0, KURL::StripFileProtocol)));
    return false;
  }
  DTD::dtd_ptr = xmlParseDTD(NULL, xmlCharStrndup(fileName.utf8(), fileName.utf8().length()));
  if( DTD::dtd_ptr == NULL )
  {
    QString errorStr = i18n("Unknown");
#ifndef LIBXML_2_5
    xmlErrorPtr errorPtr = xmlGetLastError();
    if (errorPtr != NULL)
    {
      QString s = QString::fromLatin1(errorPtr->message);
      if (!s.isEmpty())
        errorStr = s;
      s = QString::fromLatin1(errorPtr->str1);
      if (!s.isEmpty())
        errorStr += "<br>" + s;
      s = QString::fromLatin1(errorPtr->str2);
      if (!s.isEmpty())
        errorStr += "<br>" + s;
      s = QString::fromLatin1(errorPtr->str2);
      if (!s.isEmpty())
        errorStr += "<br>" + s;
      errorStr += QString("(%1, %2)").arg(errorPtr->line).arg(errorPtr->int2);
      xmlResetError(errorPtr);
    }
#endif
    KMessageBox::error(0, i18n("<qt>Error while parsing the DTD.<br>The error message is:<br><i>%1</i></qt>").arg(errorStr));
    return false;
  }
  if (targetDir.isEmpty())
  {
    KDialogBase dlg(0L, 0L, true, i18n("DTD - > DTEP Conversion"), KDialogBase::Ok | KDialogBase::Cancel);
    DTEPCreationDlg w(&dlg);
    dlg.setMainWidget(&w);
    QString name = QString((const char*)DTD::dtd_ptr->name);
    if (name == "none")
      name = QFileInfo(m_dtdURL.fileName()).baseName();
    w.dtdName->setText(name);
    w.nickName->setText(name);
    w.directory->setText(QFileInfo(m_dtdURL.fileName()).baseName());
    w.doctype->setText(QString((const char*)DTD::dtd_ptr->ExternalID));
    w.dtdURL->setText(QString((const char*)DTD::dtd_ptr->SystemID));
    if (!dlg.exec())
        return false;
    m_name = w.dtdName->text();
    m_nickName = w.nickName->text();
    m_doctype = w.doctype->text();
    m_doctype.replace(QRegExp("<!doctype", false), "");
    m_doctype = m_doctype.left(m_doctype.findRev(">"));
    m_dtdURLLine = w.dtdURL->text();
    m_defaultExtension = w.defaultExtension->text();
    m_caseSensitive = w.caseSensitive->isChecked();
    DTD::dirName = m_dtepDir + "/" + w.directory->text();
    fineTune = w.fineTune->isChecked();
  } else
    DTD::dirName = targetDir;
  KURL u;
  u.setPath(DTD::dirName);
  if (!QExtFileInfo::createDir(u, 0L)) {
    QuantaCommon::dirCreationError(0L, u);
    return false;
  }
  DTD::dirName.append("/");
  if (DTD::dtd_ptr->entities)
  {
    QFile file( DTD::dirName + "entities.tag" );
    if ( file.open( IO_WriteOnly ) )
    {
      DTD::entityStream.setDevice(&file);
      DTD::entityStream.setEncoding(QTextStream::UnicodeUTF8);
      DTD::entityStream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl;
      DTD::entityStream << "<!DOCTYPE TAGS>" << endl
          << "<TAGS>" << endl;
      xmlHashScan((xmlEntitiesTablePtr)DTD::dtd_ptr->entities, (xmlHashScanner)saveEntity, 0);
      DTD::entityStream << "</TAGS>" << endl;
      file.close();
    } else
    {
      KMessageBox::error(0L, i18n("<qt>Cannot create the <br><b>%1</b> file.<br>Check that you have write permission in the parent folder.</qt>")
                             .arg(file.name()));
      return false;
    }
  }
  if (!entitiesOnly)
  {
    if (DTD::dtd_ptr->elements)
    {
      xmlHashScan((xmlElementTablePtr)DTD::dtd_ptr->elements, (xmlHashScanner)saveElement, 0);
    } else
    {
      KMessageBox::error(0, i18n("No elements were found in the DTD."));
      return false;
    }
  }
  xmlFreeDtd(DTD::dtd_ptr);
  if (!entitiesOnly)
  {
    writeDescriptionRC();
    if (fineTune)
    {
      KDialogBase editDlg(0L, "edit_dtep", true, i18n("Configure DTEP"), KDialogBase::Ok | KDialogBase::Cancel);
      DTEPEditDlg dtepDlg(DTD::dirName + "description.rc", &editDlg);
      editDlg.setMainWidget(&dtepDlg);
      if (editDlg.exec())
      {
        dtepDlg.saveResult();
      }
    }
  }
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
//   config.setGroup("Parsing rules");
//   config.writeEntry("SpecialAreas","<!-- -->,<?xml ?>,<!DOCTYPE >");
//   config.writeEntry("SpecialAreaNames","comment,XML PI,DTD");

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
      stream.setEncoding(QTextStream::UnicodeUTF8);
      stream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl;
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
              //if (child_ptr->content->ocur == XML_ELEMENT_CONTENT_PLUS)
              //{
  //              stream << " usage=\"required\"";
    //          }
              QString ocur;
              switch (child_ptr->content->ocur)
              {
                case 1: {ocur = "once"; break;}
                case 2: {ocur = "opt"; break;}
                case 3: {ocur = "mult"; break;}
                case 4: {ocur = "plus"; break;}
              }
              stream << " usage=\"" << ocur << "\"";
              QString name = QString((const char*)child_ptr->content->name);
              if (name == "#PCDATA")
                name == "#text";
              stream << " name2=\"" << name << "\"";
            }
            stream << " />" << endl;
          }

          stream << "</children>" << endl;
          stream << endl;
        }
        /*
        xmlElementContentPtr content_ptr = el_ptr->content;
        if (content_ptr)
        {
          stream << "<children>" << endl;
          while (content_ptr)
          {
            if (!QString((const char*)content_ptr->name).isEmpty())
            {
              stream << "  <child name=\"" << QString((const char*)content_ptr->name) << "\"";
              QString ocur;
              switch (content_ptr->ocur)
              {
                case 1: {ocur = "once"; break;}
                case 2: {ocur = "opt"; break;}
                case 3: {ocur = "mult"; break;}
                case 4: {ocur = "plus"; break;}
              }
              stream << " usage=\"" << ocur << "\"";
              stream << " />" << endl;
            }
            if (content_ptr->c1)
                content_ptr = content_ptr->c1;
            else if (content_ptr->c2)
                content_ptr = content_ptr->c2;
            else
            {
              if (content_ptr == el_ptr->content)
                break;
              if (content_ptr->parent)
              {
                if (content_ptr == content_ptr->parent->c1)
                    content_ptr->c1 = 0L;
                else
                    content_ptr->c2 = 0L;
              }
              content_ptr = content_ptr->parent;
            }
          }
          stream << "</children>" << endl;
        } */
      }
      stream << "</tag>" << endl
             << "</TAGS>" << endl;
      file.close();
    }
 }
}

void saveEntity(xmlEntityPtr entity, xmlBufferPtr buf)
{
  Q_UNUSED(buf);
  if (entity)
  {
    QString name = QString((const char*)entity->name);
    DTD::entityStream << "<tag name=\"" << name << "\" type=\"entity\" />" << endl << endl;
  }
}

QString DTDParser::dirName()
{
  return DTD::dirName;
}

