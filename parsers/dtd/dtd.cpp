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


#include <qfile.h>
#include <qfileinfo.h>
#include <qregexp.h>
#include <qstringlist.h>
#include <qdom.h>

#include <klocale.h>
#include <kurl.h>
#include <kdebug.h>
#include <kio/netaccess.h>
#include <kmessagebox.h>
#include <ktempfile.h>

#include "dtd.h"
#include "../quantacommon.h"
#include "../qextfileinfo.h"


DTD::DTD(const KURL &dtdURL, const QString &dtepDir)
{
  m_dtdURL = dtdURL;
  m_dtepDir = dtepDir + "/"+QFileInfo(dtdURL.fileName()).baseName(); //TODO: get the dir name from the DTD or from the user
}

DTD::~DTD()
{
}

QStringList DTD::getTags()
{
  return tags;
}

AttributeList* DTD::getTagAttributes(QString tag)
{
  return tagAttributes.find(tag);
}


QStringList DTD::getTextCompletion(QString tag)
{
  return QStringList();
}

void DTD::printContents()
{
  for ( QStringList::Iterator tagIt = tags.begin(); tagIt != tags.end(); ++tagIt ) {
    QString tag = *tagIt;
    kdDebug(24000) << tag << endl;
    AttributeList *attributes = getTagAttributes(tag);
    for ( uint i = 0; i < attributes->count(); i++)
    {
      Attribute *attribute = attributes->at(i);
      QString s = "  " + attribute->name + ": ";
      for (uint j = 0; j < attribute->values.count(); j++)
      {
        s += attribute->values[j] + ", ";
      }
      kdDebug(24000) << s << endl;
    }
  }
}

void DTD::writeTagFiles()
{
  QString dirName = m_dtepDir;
  KURL u;
  u.setPath(dirName);
  if (!QExtFileInfo::createDir(dirName)) {
    QuantaCommon::dirCreationError(0, u);
    return;
  }
  dirName.append("/");
  for ( QStringList::Iterator tagIt = tags.begin(); tagIt != tags.end(); ++tagIt ) {
    QString tag = *tagIt;

    QFile file( dirName + tag.lower() + ".tag" );
    if ( file.open( IO_WriteOnly ) ) {
      QTextStream stream( &file );
      stream.setEncoding(QTextStream::UnicodeUTF8);
      stream << "<!DOCTYPE TAGS>" << endl
             << "<TAGS>" << endl
             << "<tag name=\"" << tag << "\">" << endl << endl;

      AttributeList *attributes = getTagAttributes(tag);
      stream << QuantaCommon::xmlFromAttributes(attributes);

      stream << "</tag>" << endl
             << "</TAGS>" << endl;

      file.close();
    } else {
      kdDebug(24000) << "Unable to write tag file: " << file.name() << endl;
    }
  }

  KConfig config(dirName + "description.rc");
  config.setGroup("General");
  config.writeEntry("Name", QFileInfo(m_dtdURL.fileName()).baseName()); //TODO: get from the DTD!
  config.writeEntry("NickName", QFileInfo(m_dtdURL.fileName()).baseName()); //TODO: get from the user!
  config.sync();
}

bool DTD::parseDTD(const KURL &url)
{
 QString fileName = QString::null;
 if (!KIO::NetAccess::download(url, fileName))
 {
   KMessageBox::error(0, i18n("<qt>Cannot download the DTD from <b>%1</b>.</qt>").arg(url.prettyURL(0, KURL::StripFileProtocol)));
   return false;
 }
  QFile file(fileName);
  if (file.open(IO_ReadOnly))
  {
    QTextStream fileStream(&file);
    fileStream.setEncoding(QTextStream::UnicodeUTF8);
    QString entireDTD = fileStream.read();
    file.close();
    removeComments(entireDTD);

    QString line;
    QStringList lines = QStringList::split("\n",entireDTD);
    QStringList::Iterator it = lines.begin();
    while (it != lines.end()) {
      line = *it;

      if (line.startsWith("<")) {
        while (!line.endsWith(">") && it != lines.end()) {
          ++it;
          line += " \\end" + *it;
        }
      } else if (line.startsWith("%")) {
        while (!line.endsWith(";") && it != lines.end()) {
          ++it;
          line += *it;
        }
      }

      line = line.stripWhiteSpace();
      line = line.simplifyWhiteSpace();

      //kdDebug(24000) << "Parsed line is: " << line << endl;

      if ( line.startsWith("<!ENTITY") && line.endsWith(">"))
      {
        parseDTDEntity(line);
      }
      else
      if (line.startsWith("<!ELEMENT") && line.endsWith(">"))
      {
        parseDTDElement(line);
      }
      else
      if (line.startsWith("<!ATTLIST") && line.endsWith(">"))
      {
        parseDTDAttlist(line);
      }
      else
      if (line.startsWith("%") && line.endsWith(";"))
      {
        line.remove(0,1);
        line.truncate(line.length()-1);
        KURL entityURL = url;
        entityURL.setPath(url.directory()+ "/" + line + ".ent");
        parseDTD(entityURL);
      } else
      {
        kdDebug(24000) << QString("Unknown tag: [%1]").arg(line) << endl;
      }

      if (it != lines.end()) ++it;
    }
  }
}

void DTD::parseDTDEntity(QString line) {
  QString name;
  QString *value;

  line.replace("\\end", " ");
  name = line.mid(11);
  int firstSpace = name.find(' ');
  name = name.remove(firstSpace, name.length()-firstSpace);

  value = new QString(line.mid(11+firstSpace));
  value->remove(0, value->find("\"")+1);
  value->remove(value->findRev("\""), value->length());

  parseDTDReplace(value);
  stripSpaces(value);

  entities.insert(name, value);

  //kdDebug() << "Entity --- Name: " << name << " --- Value: " << *value << endl;
}

void DTD::parseDTDElement(const QString &l) {
  QString name;
  QString *value;

  QString line = l;
  line.replace("\\end", " ");
  name = line.mid(10);
  int firstSpace = name.find(' ');
  name.remove(firstSpace, name.length()-firstSpace);

  value = new QString(line.mid(10+firstSpace));
  //value->remove(0, value->find("\"")+1);
  value->remove(value->find(">"), 10000);

  parseDTDReplace(&name);
  parseDTDReplace(value);

  if ( name.startsWith("(") && name.endsWith(")") ) {
    name.remove(0,1);
    name.remove(name.length()-1,1);
    QStringList multipleTags = QStringList::split("|", name);
    QStringList::Iterator it = multipleTags.begin();
    while(it != multipleTags.end()) {
      name = *it;
      name = name.stripWhiteSpace();
      elements.insert(name, value);
      tags.append(name);
      //kdDebug() << "Element --- Name: " << name << " --- Value: " << *value << endl;
      ++it;
    }
  } else {
    elements.insert(name, value);
    tags.append(name);
    //kdDebug() << "Element --- Name: " << name << " --- Value: " << *value << endl;
  }
}

void DTD::parseDTDAttlist(const QString &l) {
  QString name;
  QString *value;

  QString line = l;
  line.replace("\\end", " ");
  name = line.mid(10);
  int firstSpace = name.find(' ');
  name.remove(firstSpace, name.length()-firstSpace);

  value = new QString(line.mid(10+firstSpace));
  //value->remove(0, value->find("\"")+1);
  value->remove(value->find(">"), 10000);

  parseDTDReplace(&name);
  parseDTDReplace(value);

  if ( name.startsWith("(") && name.endsWith(")") ) {
    name.remove(0,1);
    name.remove(name.length()-1,1);
    QStringList multipleTags = QStringList::split("|", name);
    QStringList::Iterator it = multipleTags.begin();
    while(it != multipleTags.end()) {
      name = *it;
      name = name.stripWhiteSpace();
      //elements.insert(name, value);
      parseTagAttributeValues(name, value);
      //kdDebug() << "Attlist --- Name: " << name << " --- Value: " << *value << endl;
      ++it;
    }
  } else {
    //elements.insert(name, value);
    parseTagAttributeValues(name, value);
    //kdDebug() << "Attlist --- Name: " << name << " --- Value: " << *value << endl;
  }

}

void DTD::parseTagAttributeValues(const QString &name, QString *value) {
  AttributeList *attributes = new AttributeList();

  QStringList attrLines = QStringList::split("\\end",*value);
  QStringList::Iterator lineIt = attrLines.begin();
  while (lineIt != attrLines.end()) //iterate through the attribute lines
  {
    //split the attribute line
    QStringList all = QStringList::split(" ", *lineIt);
    QStringList::Iterator it = all.begin();
    while(it != all.end())
    {
      Attribute *attr = new Attribute();
      attr->name = *it;
      //kdDebug() << "Inserting for tag " << name << ": " << *it << endl;
      ++it;

      QString values = *it;
      //list of possible values
      if ( values.startsWith("(") && values.endsWith(")") )
      {
        values.remove(0,1);
        values.remove(values.length()-1,1);
        attr->values = QStringList::split("|", values);
        QString s = (attr->values[0]+attr->values[1]).lower();
        stripSpaces(&s);
        if ((s == "truefalse") || (s == "falsetrue"))
        {
          attr->type = "check";
        } else
        {
          attr->type = "list";
        }
      } else
      {
        attr->values = values;
        attr->type = "input";
      }

      //kdDebug() << " --- values: " << *it << endl;
      if (it != all.end())
      {
        ++it;
        QString s=*it;
        if (s.startsWith("\"") && s.endsWith("\"") && it!=all.end())
        {
          s.remove(0,1);
          s.remove(s.length()-1,1);
          attr->defaultValue = s;
        }
        if (s.startsWith("#") && it != all.end())
        {
          s.remove(0,1);
          attr->status = s.lower();
        }
        if (*it == "#FIXED" && it != all.end())
        {
          ++it;
          attr->values.append(*it);
        }
      }

      if (it != all.end())
      {
        ++it;
      }
      attributes->append(attr);
    }
    ++lineIt;
  }
  tagAttributes.insert(name, attributes);
}

void DTD::parseDTDReplace(QString *value) {
  int begin, end;
  begin = value->find("%");
  end = value->find(";");
  while (begin != -1 && end != -1) {
    QString replaceText = value->mid(begin+1, end-begin-1);
    QString *replaceValue = entities.find(replaceText);

    if (replaceValue != 0L) {
      value->replace(begin, end-begin+1, *replaceValue);
    } else {
      kdDebug(24000) << "Can not find entity: " << replaceText << endl;
      return;
    }

    begin = value->find("%");
    end = value->find(";");
  }
}

void DTD::stripSpaces(QString *value) {
  int index=-1;
  while ( (index=value->find(' ',++index)) != -1 ) {
    if ( value->findRev('(',index) != -1 && value->find(')',index) != -1)
      value->remove(index,1);
  }
}

void DTD::removeComments(QString &value) {
  int begin, end;
  begin = value.find("<!--");
  end = value.find("-->",begin+2);
  while (begin != -1 && end != -1) {
    value.remove(begin, end-begin+3);
    begin = value.find("<!--");
    end = value.find("-->",begin+2);
  }

  begin = value.find("--");
  end = value.find("--",begin+2);
  while (begin != -1 && end != -1) {
    value.remove(begin, end-begin+2);
    begin = value.find("--");
    end = value.find("--",begin+2);
  }

  value.replace(QRegExp("<!>"), "");
}

bool DTD::parseDTD()
{
  return parseDTD(m_dtdURL);
}
