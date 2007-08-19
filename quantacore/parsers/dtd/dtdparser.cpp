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

//own includes
#include "dtdparser.h"
#include "dtepeditdlg.h"
#include "qtag.h"
#include "ui_dtepcreationdlg.h"
#include "quantacommon.h"
#include "extfileinfo.h"

//qt includes
#include <QCheckBox>
#include <QFile>
#include <QFileInfo>
#include <QLabel>
#include <QLineEdit>
#include <QRegExp>
#include <QString>
#include <QTextStream>

//kde includes
#include <kconfig.h>
#include <kdebug.h>
#include <kdialog.h>
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

#define MAX_CHILD_ELEMENTS 100

namespace DTD
{
  QString dirName;
  xmlDtdPtr dtd_ptr; /* Pointer to the parsed DTD */
  QTextStream entityStream;
}

void saveElement(xmlElementPtr elem, xmlBufferPtr buf);
void saveEntity(xmlEntityPtr entity, xmlBufferPtr buf);

DTDParser::DTDParser(const KUrl& dtdURL, const QString &dtepDir)
{
  m_dtdURL = dtdURL;
  m_dtepDir = dtepDir;
}

DTDParser::~DTDParser()
{
}

bool DTDParser::parse()
{
  QString fileName;
  fileName.clear();
  if (!KIO::NetAccess::download(m_dtdURL, fileName, 0))
  {
    KMessageBox::error(0, i18n("<qt>Cannot download the DTD from <b>%1</b>.</qt>", m_dtdURL.pathOrUrl()));
    return false;
  }
  DTD::dtd_ptr = xmlParseDTD(NULL, xmlCharStrndup(fileName.toUtf8(), fileName.toUtf8().length()));
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
      xmlResetError(errorPtr);
    }
#endif
    KMessageBox::error(0, i18n("<qt>Error while parsing the DTD.<br />The error message is:<br /><i>%1</i></qt>", errorStr));
    return false;
  }
  KDialog dlg(0L );
  dlg.setCaption( i18n("DTD - > DTEP Conversion") );
  dlg.setButtons( KDialog::Ok | KDialog::Cancel );
  dlg.setDefaultButton( KDialog::Ok);
  QWidget *w = new QWidget(&dlg);
  Ui::DTEPCreationDlg form;
  form.setupUi(w);
  dlg.setMainWidget(w);
  QString name = QString((const char*)DTD::dtd_ptr->name);
  if (name == "none")
    name = QFileInfo(m_dtdURL.fileName()).baseName();
  form.dtdName->setText(name);
  form.nickName->setText(name);
  form.directory->setText(QFileInfo(m_dtdURL.fileName()).baseName());
  form.doctype->setText(QString((const char*)DTD::dtd_ptr->ExternalID));
  form.dtdURL->setText(QString((const char*)DTD::dtd_ptr->SystemID));
  if (!dlg.exec())
      return false;
  m_name = form.dtdName->text();
  m_nickName = form.nickName->text();
  m_doctype = form.doctype->text();
  m_doctype.replace(QRegExp("<!doctype", Qt::CaseInsensitive), "");
  m_doctype = m_doctype.left(m_doctype.lastIndexOf(">"));
  m_dtdURLLine = form.dtdURL->text();
  m_defaultExtension = form.defaultExtension->text();
  m_caseSensitive = form.caseSensitive->isChecked();
  DTD::dirName = m_dtepDir + '/' + form.directory->text();
  KUrl u;
  u.setPath(DTD::dirName);
  if (!ExtFileInfo::createDir(u)) {
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
  if (DTD::dtd_ptr->entities)
  {
    QFile file( DTD::dirName + "entities.tag" );
    if ( file.open( IO_WriteOnly ) )
    {
      DTD::entityStream.setDevice(&file);
      DTD::entityStream.setCodec(QTextCodec::codecForName("UTF-8"));
      DTD::entityStream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl;
      DTD::entityStream << "<!DOCTYPE TAGS>" << endl
          << "<TAGS>" << endl;
      xmlHashScan((xmlEntitiesTablePtr)DTD::dtd_ptr->entities, (xmlHashScanner)saveEntity, 0);
      DTD::entityStream << "</TAGS>" << endl;
      file.close();
    }
  }
  xmlFreeDtd(DTD::dtd_ptr);
  writeDescriptionRC();
  if (form.fineTune->isChecked())
  {
    KDialog editDlg(0L );
    dlg.setCaption( i18n("Configure DTEP") );
    dlg.setButtons( KDialog::Ok | KDialog::Cancel );
    dlg.setDefaultButton( KDialog::Ok);
    DTEPEditDlg dtepDlg(DTD::dirName + "description.rc", &editDlg);
    editDlg.setMainWidget(&dtepDlg);
    if (editDlg.exec())
    {
      dtepDlg.saveResult();
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
  config.setGroup("Parsing rules");
  config.writeEntry("SpecialAreas","<!-- -->,<?xml ?>,<!DOCTYPE >");
  config.writeEntry("SpecialAreaNames","comment,XML PI,DTD");

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

