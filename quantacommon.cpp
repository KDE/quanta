/***************************************************************************
                          quantacommon.cpp  -  description
                             -------------------
    begin                : Sat Jul 27 2002
    copyright            : (C) 2002, 2003 by Andras Mantia
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
//qt includes
#include <qstring.h>
#include <qdict.h>
#include <qdir.h>
#include <qwidget.h>

//kde includes
#include <kurl.h>
#include <kdirwatch.h>
#include <kmimetype.h>
#include <kstandarddirs.h>
#include <kmessagebox.h>
#include <klocale.h>
#include <kprotocolinfo.h>

#include "quantacommon.h"

#include "quanta.h"
#include "quantadoc.h"
#include "document.h"
#include "project/project.h"


QConfig qConfig; //holds the main configuration settings
QString tmpDir;

extern QuantaApp *quantaApp;

QDict<DTDStruct> *dtds; //holds all the known tags with attributes for each DTD.
QRegExp scriptBeginRx;
QRegExp scriptEndRx;

Node *baseNode;
Parser *parser;
KDirWatch *fileWatcher;
KProgress *progressBar;

const QString toolbarExtension = ".toolbar.tgz";
const QRegExp newLineRx("\\n");

QuantaCommon::QuantaCommon(){
}

QuantaCommon::~QuantaCommon(){
}

/** convert tag to upper or lower case */
QString QuantaCommon::tagCase( const QString tag)
{
  QString sTag = tag;

  switch (qConfig.tagCase)
  {
    case 1: sTag = tag.lower();
            break;
    case 2: sTag = tag.upper();
  }
  return sTag;
}

/** convert attr of tag to upper or lower case */
QString QuantaCommon::attrCase( const QString attr)
{
  QString sAttr = attr;

  switch (qConfig.attrCase)
  {
    case 1: sAttr = attr.lower();
            break;
    case 2: sAttr = attr.upper();
  }
  return sAttr;
}

/** Set's up the url correctly from urlString. */
void QuantaCommon::setUrl(KURL &url, QString urlString)
{
 KURL oldUrl = url;
 url = urlString;
 if (!KProtocolInfo::isKnownProtocol(url))
 {
   url = oldUrl;
   url.setPath(urlString);
   if (url.protocol().isEmpty())
       url.setProtocol("file");
 }	   		     
}
/** No descriptions */
bool QuantaCommon::isSingleTag(QString dtdName, QString tag)
{
  bool single = false;

  DTDStruct* dtd = dtds->find(dtdName.lower());
  if (dtd)
  {
    QString searchForTag = (dtd->caseSensitive) ? tag : tag.upper();
    QTag* qtag = dtd->tagsList->find(searchForTag);
    if (qtag)
        single = qtag->isSingle();
  }

  return single;
}

/** No descriptions */
bool QuantaCommon::isOptionalTag(QString dtdName, QString tag)
{
  bool optional = false;

  DTDStruct* dtd = dtds->find(dtdName.lower());
  if (dtd)
  {
    QString searchForTag = (dtd->caseSensitive) ? tag : tag.upper();
    QTag* qtag = dtd->tagsList->find(searchForTag);
    if (qtag)
        optional = qtag->isOptional();
  }

  return optional;
}
/** No descriptions */
bool QuantaCommon::isKnownTag(QString dtdName, QString tag)
{
  bool known = false;

  DTDStruct* dtd = dtds->find(dtdName.lower());
  if (dtd)
  {
    QString searchForTag = (dtd->caseSensitive) ? tag : tag.upper();
    if (dtd->tagsList->find(searchForTag))
        known = true;
  }

  return known;
}

AttributeList*  QuantaCommon::tagAttributes(QString dtdName, QString tag)
{
  AttributeList* attrs = 0L;

  DTDStruct* dtd = dtds->find(dtdName.lower());
  if (dtd)
  {
    QString searchForTag = (dtd->caseSensitive) ? tag : tag.upper();
    QTag* qtag = dtd->tagsList->find(searchForTag);
    if (qtag)
        attrs = qtag->attributes();
  }

  return attrs;
}
/** Returns the QTag object for the tag "tag" from the DTD named "dtdname". */
QTag* QuantaCommon::tagFromDTD(QString dtdName, QString tag)
{
  DTDStruct* dtd = dtds->find(dtdName.lower());
  return tagFromDTD(dtd, tag);
}
/** Returns the QTag object for the tag "tag" from the DTD. */
QTag* QuantaCommon::tagFromDTD(DTDStruct *dtd, QString tag)
{
  QTag *qtag = 0;
  if (dtd)
  {
    QString searchForTag = (dtd->caseSensitive) ? tag : tag.upper();
    qtag = dtd->tagsList->find(searchForTag);
  }

 return qtag;
}

/** Returns an XML style string containing the GUI for attributes. */
QString QuantaCommon::xmlFromAttributes(AttributeList* attributes)
{
 QString xmlStr;
 QTextStream stream( &xmlStr, IO_WriteOnly );
 if (attributes)
 {
  int row = 0;
  for ( uint i = 0; i< attributes->count();i++)
  {
    Attribute *attribute = attributes->at(i);
    QString name = attribute->name.left(1).upper()+attribute->name.right(attribute->name.length()-1);
    stream << "  <attr name=\"" + attribute->name +"\"type=\""+attribute->type+"\"";
    if (!attribute->defaultValue.isEmpty())
        stream  << " defaultValue=\"" + attribute->defaultValue + "\"";
    if (!attribute->status.isEmpty())
        stream  << " status=\"" + attribute->status + "\"";
        stream << ">" << endl;
        stream << "    <text>" << name << "</text>" << endl;
        if (attribute->type != "check")
        {
          stream  << "    <textlocation col=\"0\" row=\"" << row << "\" />" << endl;
        }
        stream  << "    <location col=\"1\" row=\"" << row << "\" />" << endl;

    if (attribute->type == "list")
    {
      stream << "    <items>" << endl;
      for (uint j = 0; j < attribute->values.count(); j++)
      {
        stream << "      <item>" << attribute->values[j] << "</item>" << endl;
      }
      stream << "    </items>" << endl;
    }
    stream  << "  </attr>" << endl << endl ;
    row++;
  } //for
 } //if

 return xmlStr;
}

/** Returns the DTD name (identifier) corresponding to the DTD's nickname */
QString QuantaCommon::getDTDNameFromNickName(QString nickName)
{
  QString name = nickName;
  QDictIterator<DTDStruct> it(*dtds);
  for( ; it.current(); ++it )
  {
    if (it.current()->nickName.lower() == nickName.lower())
    {
     name = it.current()->name;
     break;
    }
  }

 return name;
}

/** Returns the DTD iddentifier from the given nickname */
QString QuantaCommon::getDTDNickNameFromName(QString name)
{
  QString nickName = name;
  QDictIterator<DTDStruct> it(*dtds);
  for( ; it.current(); ++it )
  {
    if (it.current()->name.lower() == name.lower())
    {
      nickName = it.current()->nickName;
      break;
    }
  }

  return nickName;
}

  /** Returns 0 if the (line,col) is inside the area specified by the other
arguments,      -1 if it is before the area and 1 if it is after. */
int QuantaCommon::isBetween(int line, int col, int bLine, int bCol, int eLine,
int eCol){
  int pos = 0;
  if (line < bLine || (line == bLine && (col < bCol) )) pos = -1; //it is before
  if (line > eLine || (line == eLine && (col > eCol) )) pos = 1;  //it is after

 return pos;
}

/** Returns a pointer to a KStandardDirs object usable for plugin searchup. type
is the plugin binary type (exe or lib). The returned pointer must be deleted by
the caller!! */KStandardDirs* QuantaCommon::pluginDirs(const char *type)
{
 KStandardDirs *dirs = new KStandardDirs();
 dirs->addKDEDefaults();
 for (uint i = 0; i < qConfig.pluginSearchPaths.count(); i++)
 {
   dirs->addResourceDir(type, qConfig.pluginSearchPaths[i]);
 }
 return dirs;
}
/** Return true, if the url belong to the mimetype group. */
bool QuantaCommon::checkMimeGroup(const KURL& url, const QString& group)
{
 KMimeType::List list = KMimeType::allMimeTypes();
 KMimeType::List::iterator it;
 bool status = false;
 QString mimetype = KMimeType::findByURL(url)->name();
 mimetype = mimetype.section('/',-1);
 for ( it = list.begin(); it != list.end(); ++it )
 {
    if ( ((*it)->name().contains(group)) && ((*it)->name().find(mimetype) != -1) )
    {
      status = true;
      break;
    }
 }
 if (!status && group == "text" && url.isLocalFile())
 {
   Format f = findFormatByFileContent(url.path());
   if (f.text && f.compression == Format::NoCompression)
       status = true;
 }

 if (!status && mimetype == "x-zerosize")
    status = true;


 return status;
}

/** Return true, if the url has the mimetype type. */
bool QuantaCommon::checkMimeType(const KURL& url, const QString& type)
{
 bool status = false;
 QString mimetype = KMimeType::findByURL(url)->name();
 mimetype = mimetype.section('/',-1);
 if (mimetype == type)
 {
   status = true;
 }
 return status;
}

/** Return true, if the url has exactly the mimetype type. */
bool QuantaCommon::checkExactMimeType(const KURL& url, const QString& type)
{
 bool status = false;
 QString mimetype = KMimeType::findByURL(url)->name();
 if (mimetype == type) status = true;

 return status;
}

/** Returns the url without the filename. */
KURL QuantaCommon::convertToPath(const KURL& url)
{
 KURL result = url;
 result.setFileName("");
 result.adjustPath(1);
 return result;
}

/** Return a string to be used when an url is saved to the project file.
    Returns url.url() if it's an absolute url and
    url.path() if the url is relative */
QString QuantaCommon::qUrl(const KURL &url)
{
  QString result = url.path();
  if (url.path().startsWith("/")) result = url.url();

  return result;
}
/** No descriptions */
void QuantaCommon::dirCreationError(QWidget *widget, const KURL& url)
{
  KMessageBox::error(widget, i18n("Can't create directory \n\"%1\".\nCheck that you have write permission in the parent directory or that the connection to\n\"%2\"\n is valid!")
                             .arg(url.prettyURL())
                             .arg(url.protocol()+"://"+url.user()+"@"+url.host()));
}

/** Returns the translated a_str in English. A "back-translation" useful e.g in case of CSS elements selected from a listbox. */
QString QuantaCommon::i18n2normal(const QString& a_str)
{
//TODO: a QMap lookup would be faster, but we need a pre-built QMap<QString,QString>
  const int keywordNum = 15 *5;
  const QString keywords[keywordNum] =
                            {"normal",  "italic",  "oblique",   "serif",      "sans-serif",
                             "cursive", "fantasy", "monospace", "small-caps", "lighter",
                             "bold",    "bolder",  "xx-small",  "x-small",    "small",
                             "medium",  "large",   "x-large",   "xx-large",   "smaller",
                             "larger",  "repeat",  "repeat-x",  "repeat-y",   "no-repeat",
                             "scroll",  "fixed",   "top",       "center",     "bottom",
                             "left",    "right",   "none",      "underline",  "overline"
                             "line-through", "blibk", "justify","baseline",   "sub",
                             "super",   "text-top","text-bottom","capitalize","uppercase",
                             "lowercase","thin",   "thick",  "[length value]","dotted",
                             "dashed",  "solid",   "double",    "groove",     "ridge",
                             "inset",   "outset",  "block",     "inline",     "list-item",
                             "none",    "pre",     "nowrap",    "disc",       "circle",
                             "square",  "decimal", "lower-roman","upper-roman","lower-alpha",
                             "upper-alpha","inside","outside",  "auto",        "both" };
  QString str = a_str;
  if (!a_str.isEmpty())
  {
    for (int i = 0; i < keywordNum; i++)
    {
      if (a_str == i18n(keywords[i]))
      {
        str = keywords[i];
        break;
      }
    }
  }
  return str;
}

#define GZIP_MAGIC1     0x1f
#define GZIP_MAGIC2     0x8b

Format QuantaCommon::findFormatByFileContent( const QString &fileName )
{
  Format result;
  result.compression = Format::NoCompression;
  KMimeType::Ptr mime = KMimeType::findByFileContent(fileName);

  result.text = mime->name().startsWith("text/");
  QVariant v = mime->property("X-KDE-text");
  if (v.isValid())
     result.text = v.toBool();

  if (mime->name().startsWith("inode/"))
     return result;

  QFile f(fileName);
  if (f.open(IO_ReadOnly))
  {
     unsigned char buf[10+1];
     int l = f.readBlock((char *)buf, 10);
     if ((l > 2) && (buf[0] == GZIP_MAGIC1) && (buf[1] == GZIP_MAGIC2))
        result.compression = Format::GZipCompression;
  }
  return result;
}
