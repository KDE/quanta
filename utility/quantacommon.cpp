/***************************************************************************
                          quantacommon.cpp  -  description
                             -------------------
    begin                : Sat Jul 27 2002
    copyright            : (C) 2002, 2003 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/

//system includes
#include <sys/types.h>
#include <unistd.h>

//qt includes
#include <qstringlist.h>
#include <qdict.h>
#include <qdir.h>
#include <qdom.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qwidget.h>

//kde includes
#include <kapplication.h>
#include <dcopref.h>
#include <kurl.h>
#include <kdirwatch.h>
#include <kmimetype.h>
#include <kstandarddirs.h>
#include <kmessagebox.h>
#include <kio/netaccess.h>
#include <klocale.h>
#include <kprotocolinfo.h>
#include <kprogress.h>
#include <ktempdir.h>
#include <ktempfile.h>

//remove the below ones when KQPasteAction is removed
#include <dcopclient.h>
#include <kdebug.h>
#include <kpopupmenu.h>
#include <ktoolbar.h>
#include <ktoolbarbutton.h>
#include <kstringhandler.h>
#include <qwhatsthis.h>
#include <qtimer.h>
#include <qclipboard.h>
#include <qdatastream.h>

#include "qextfileinfo.h"
#include "quantacommon.h"
#include "tag.h"
#include "dtds.h"
//#include "resource.h"


QConfig qConfig; //holds the main configuration settings
QString tmpDir;

QRegExp scriptBeginRx;
QRegExp scriptEndRx;

Node *baseNode;
Parser *parser;
KDirWatch *fileWatcher;
KProgress *progressBar;

QString toolbarExtension = ".toolbar.tgz";
QRegExp newLineRx("\\n");
QStringList charList; //hold the list of the &char; chars. See the data/chars file.
QMap<int, QString> replacementMap;
QPtrList<KTempFile> tempFileList;
QPtrList<KTempDir> tempDirList;
bool typingInProgress;

QuantaCommon::QuantaCommon(){
}

QuantaCommon::~QuantaCommon(){
}

/** convert tag to upper or lower case */
QString QuantaCommon::tagCase( const QString& tag)
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
QString QuantaCommon::attrCase( const QString& attr)
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

/** returns the attribute value in quoted form, by taking care of the
quotation setting*/
QString QuantaCommon::quoteAttributeValue(const QString& value)
{
  QString quote = qConfig.attrValueQuotation;
  return quote + value + quote;
}

/** Set's up the url correctly from urlString. */
void QuantaCommon::setUrl(KURL &url, const QString& urlString)
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
bool QuantaCommon::isSingleTag(const QString& dtdName, const QString& tag)
{
  bool single = false;

  //!doctype is a common tag to all DTDs not listed in the tagsList
  if(tag.lower() == "!doctype" || tag.lower() == "?xml")
    return true;

  const DTDStruct* dtd = DTDs::ref()->find(dtdName);
  if (dtd && !tag.isEmpty())
  {
    QString searchForTag = (dtd->caseSensitive) ? tag : tag.upper();
    QTag* qtag = dtd->tagsList->find(searchForTag);
    if (qtag)
        single = qtag->isSingle();
  }

  return single;
}

/** No descriptions */
bool QuantaCommon::isOptionalTag(const QString& dtdName, const QString& tag)
{
  bool optional = false;

  const DTDStruct* dtd = DTDs::ref()->find(dtdName);
  if (dtd && !tag.isEmpty())
  {
    QString searchForTag = (dtd->caseSensitive) ? tag : tag.upper();
    QTag* qtag = dtd->tagsList->find(searchForTag);
    if (qtag)
        optional = qtag->isOptional();
  }

  return optional;
}
/** No descriptions */
bool QuantaCommon::isKnownTag(const QString& dtdName, const QString& tag)
{
  bool known = false;

  const DTDStruct* dtd = DTDs::ref()->find(dtdName);
  if (dtd && !tag.isEmpty())
  {
    QString searchForTag = (dtd->caseSensitive) ? tag : tag.upper();
    if (dtd->tagsList->find(searchForTag))
        known = true;
  }

  return known;
}

AttributeList* QuantaCommon::tagAttributes(const QString& dtdName, const QString& tag)
{
  AttributeList* attrs = 0L;

  const DTDStruct* dtd = DTDs::ref()->find(dtdName);
  if (dtd && !tag.isEmpty())
  {
    QString searchForTag = (dtd->caseSensitive) ? tag : tag.upper();
    QTag* qtag = dtd->tagsList->find(searchForTag);
    if (qtag)
        attrs = qtag->attributes();
  }

  return attrs;
}

/** Returns the QTag object for the tag "tag" from the DTD named "dtdname". */
QTag* QuantaCommon::tagFromDTD(const QString& dtdName, const QString& tag)
{
  const DTDStruct* dtd = DTDs::ref()->find(dtdName);
  return tagFromDTD(dtd, tag);
}

/** Returns the QTag object for the tag "tag" from the DTD. */
QTag* QuantaCommon::tagFromDTD(const DTDStruct *dtd, const QString& tag)
{
  QTag *qtag = 0;
  if (dtd && !tag.isEmpty())
  {
    QString searchForTag = (dtd->caseSensitive) ? tag : tag.upper();
    qtag = dtd->tagsList->find(searchForTag);
  }

 return qtag;
}

/** Returns the QTag object for the node "node" from node's DTD. */
QTag* QuantaCommon::tagFromDTD(Node *node)
{
  if(!node || !node->tag)
    return 0L;

  return tagFromDTD(node->tag->dtd(), node->tag->name);
}

/** Returns an XML style string containing the GUI for attributes. */
QString QuantaCommon::xmlFromAttributes(AttributeList* attributes)
{
 QString xmlStr;
 QTextStream stream( &xmlStr, IO_WriteOnly );
 stream.setEncoding(QTextStream::UnicodeUTF8);
 if (attributes)
 {
  int row = 0;
  for ( uint i = 0; i< attributes->count();i++)
  {
    Attribute *attribute = attributes->at(i);
    QString name = attribute->name.left(1).upper()+attribute->name.right(attribute->name.length()-1);
    stream << "  <attr name=\"" + attribute->name +"\" type=\""+attribute->type+"\"";
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
the caller!! */
KStandardDirs* QuantaCommon::pluginDirs(const char *type)
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
 KMimeType::Ptr mime = KMimeType::findByURL(url);
 QString mimetype = mime->name();
 mimetype = mimetype.section('/',-1);
 for ( it = list.begin(); it != list.end(); ++it )
 {
    if ( ((*it)->name().contains(group)) && ((*it)->name().find(mimetype) != -1)
)    {
      status = true;
      break;
    }
 }

 if (!status && group == "text")
 {
   if (url.isLocalFile())
   {
      KMimeType::Format f = KMimeType::findFormatByFileContent(url.path());
      if (f.text && f.compression == KMimeType::Format::NoCompression)
          status = true;
   } else
   {
     QVariant v = mime->property("X-KDE-text");
     if (v.isValid())
         status = v.toBool();
   }
 }
 if (!status && group == "text" && mimetype == "x-zerosize")
     status = true;

 return status;
}

/** Return true, if the url has the mimetype type. */
bool QuantaCommon::checkMimeType(const KURL& url, const QString& type)
{
 bool status = false;
 QString mimetype = KMimeType::findByURL(url)->name();
 mimetype = mimetype.section('/',-1);
 if (mimetype == type) status = true;

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
  KMessageBox::error(widget, i18n("<qt>Cannot create folder<br><b>%1</b>.<br>Check that you have write permission in the parent folder or that the connection to<br><b>%2</b><br> is valid.</qt>")
                             .arg(url.prettyURL(0, KURL::StripFileProtocol))
                             .arg(url.protocol()+"://"+url.user()+"@"+url.host()));}

/**
Adds the backslash before the special chars (like ?, *, . ) so the returned
string can be used in regular expressions.*/
QString QuantaCommon::makeRxCompatible(const QString& s)
{
  const uint max = 7;
  const QRegExp rxs[max]={QRegExp("\\?"),
                        QRegExp("\\*"),
                        QRegExp("\\."),
                        QRegExp("\\^"),
                        QRegExp("\\$"),
                        QRegExp("\\{"),
                        QRegExp("\\}")
                        };
  const QString strs[max]={QString("\\?"),
                         QString("\\*"),
                         QString("\\."),
                         QString("\\^"),
                         QString("\\$"),
                         QString("\\{"),
                         QString("\\}")
                         };
  QString str = s;
  for (uint i = 0; i < max - 1; i++)
  {
    str.replace(rxs[i], strs[i]);
  }

  return str;
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
      if (!keywords[i].isEmpty() && a_str == i18n(keywords[i].utf8()))
      {
        str = keywords[i];
        break;
      }
    }
  }
  return str;
}

static const QChar space(' ');

void QuantaCommon::removeCommentsAndQuotes(QString &str, const DTDStruct *dtd)
{
 //Replace all the commented strings and the escaped quotation marks (\", \')
 // with spaces so they will not mess up our parsing
 int pos = 0;
 int l;
 QString s;
 while (pos != -1)
 {
   pos = dtd->commentsStartRx.search(str, pos);
   if (pos != -1)
   {
     s = dtd->commentsStartRx.cap();
     if (s == "\\\"" || s == "\\'")
     {
        int i = pos;
        int slahNum = 0;
        while (i > 0 && str[i] == '\\')
        {
          slahNum++;
          i--;
        }
        if (slahNum % 2 == 0)
        {
          pos++;
        } else
        {
          str[pos] = space;
          str[pos+1] = space;
          pos += 2;
        }
     } else
     {
       s = dtd->comments[s];
       l = str.find(s, pos);
       l = (l == -1) ? str.length() : l;
       for (int i = pos; i < l ; i++)
       {
         str[i] = space;
       }
       pos = l + s.length();
     }
   }
 }

 //Now replace the quoted strings with spaces
 QRegExp strRx("(\"[^\"]*\"|'[^']*')");
 pos = 0;
 while (pos != -1)
 {
   pos = strRx.search(str, pos);
   if (pos != -1)
   {
    l = strRx.matchedLength();
    for (int i = pos; i < pos + l ; i++)
    {
      str[i] = space;
    }
    pos += l;
   }
 }

}

bool QuantaCommon::insideCommentsOrQuotes(int position, const QString &string, const DTDStruct *dtd)
{
 //Return true if position is inside a commented or quoted string
 QString str = string;
 int pos = 0;
 int l;
 QString s;
 while (pos != -1)
 {
   pos = dtd->commentsStartRx.search(str, pos);
   if (pos == position)
     return true;
   if (pos != -1)
   {
     s = dtd->commentsStartRx.cap();
     if (s == "\\\"" || s == "\\'")
     {
        int i = pos;
        int slahNum = 0;
        while (i > 0 && str[i] == '\\')
        {
          slahNum++;
          i--;
        }
        if (slahNum % 2 == 0)
        {
          pos++;
        } else
        {
          str[pos] = space;
          str[pos+1] = space;
          pos += 2;
        }
     } else
     {
       s = dtd->comments[s];
       l = str.find(s, pos);
       l = (l == -1) ? str.length() : l;
       for (int i = pos; i < l ; i++)
       {
         str[i] = space;
         if (i == position)
           return true;
       }
       pos = l + s.length();
     }
   }
 }

 //Now replace the quoted strings with spaces
 const QRegExp strRx("(\"[^\"]*\"|'[^']*')");
 pos = 0;
 while (pos != -1)
 {
   pos = strRx.search(str, pos);
   if (pos != -1)
   {
    l = strRx.matchedLength();
    for (int i = pos; i < pos + l ; i++)
    {
      str[i] = space;
      if (i == position)
        return true;
    }
    pos += l;
   }
 }

  return false;
}

DCOPReply QuantaCommon::callDCOPMethod(const QString& interface, const QString& method, const QString& arguments)
{
  QStringList argumentList = QStringList::split(",", arguments, true);
  QString app = "quanta";
  if (!kapp->inherits("KUniqueApplication"))
  {
    pid_t pid = ::getpid();
    app += QString("-%1").arg(pid);
  }
  DCOPRef quantaRef(app.utf8(), interface.utf8());
  DCOPReply reply;
  int argumentCount = argumentList.count();
  if (argumentCount == 0)
  {
    reply = quantaRef.call(method.utf8());
  }
  else if (argumentCount == 1)
  {
    reply = quantaRef.call(method.utf8(), argumentList[0]);
  }
  else if (argumentCount == 2)
    reply = quantaRef.call(method.utf8(), argumentList[0], argumentList[1]);
  else if (argumentCount == 3)
    reply = quantaRef.call(method.utf8(), argumentList[0], argumentList[1], argumentList[2]);
  else if (argumentCount == 4)
    reply = quantaRef.call(method.utf8(), argumentList[0], argumentList[1], argumentList[2], argumentList[3]);
  else if (argumentCount == 5)
    reply = quantaRef.call(method.utf8(), argumentList[0], argumentList[1], argumentList[2], argumentList[3], argumentList[4]);
  else if (argumentCount == 6)
    reply = quantaRef.call(method.utf8(), argumentList[0], argumentList[1], argumentList[2], argumentList[3], argumentList[4], argumentList[5]);
  else if (argumentCount == 7)
    reply = quantaRef.call(method.utf8(), argumentList[0], argumentList[1], argumentList[2], argumentList[3], argumentList[4], argumentList[5], argumentList[6]);
  else if (argumentCount == 8)
    reply = quantaRef.call(method.utf8(), argumentList[0], argumentList[1], argumentList[2], argumentList[3], argumentList[4], argumentList[5], argumentList[6], argumentList[7]);

  return reply;
}

void QuantaCommon::normalizeStructure(QString f,QStringList& l)
{
 f.remove("\t");
 f.remove("\n");
 f.remove("\r");

 while(f.contains("<"))
  {
     QString z(f);
     z.truncate(z.find(">")+1);
     z.remove(0,z.find("<"));
     f.remove(0,f.find(">")+1);
     l.append(z);
  }
}

bool QuantaCommon::closesTag(Tag *tag1, Tag *tag2)
{
  if (tag1->nameSpace.isEmpty())
  {
    if (!tag2->nameSpace.isEmpty())
      return false; //namespace missmatch
    QString tag1Name = tag1->dtd()->caseSensitive ? tag1->name : tag1->name.upper();
    QString tag2Name = tag2->dtd()->caseSensitive ? tag2->name : tag2->name.upper();
    if ("/" + tag1Name != tag2Name)
      return false; //not the closing tag
  } else
  {
    if (tag2->nameSpace.isEmpty())
      return false; //namespace missmatch
    QString tag1Name = tag1->dtd()->caseSensitive ? (tag1->nameSpace + tag1->name) : (tag1->nameSpace.upper() + tag1->name.upper());
    QString tag2Name = tag2->dtd()->caseSensitive ? (tag2->nameSpace + tag2->name) : (tag2->nameSpace.upper() + tag2->name.upper());
    if ("/" + tag1Name != tag2Name)
      return false; //namespace missmatch or not the closing tag
  }
  return true;
}

bool QuantaCommon::closesTag(QString namespaceName, QString tagName, bool caseSensitive,
        QString namespaceName2, QString tagName2, bool caseSensitive2)
{
        QString tag1Name, tag2Name;
        if(namespaceName.isEmpty())
        {
                if(!namespaceName.isEmpty())
                        return false;//namespace missmatch
                tag1Name = caseSensitive ? tagName : tagName.upper();
                tag2Name = caseSensitive2 ? tagName2 : tagName2.upper();
                if("/" + tag1Name != tag2Name)
                        return false;//not the closing tag
        }
        else
        {
                if(namespaceName2.isEmpty())
                        return false;//namespace missmatch
                tag1Name = caseSensitive ? (namespaceName + tagName) : (namespaceName.upper() +
                        tagName.upper());
                tag2Name = caseSensitive2 ? (namespaceName2 + tagName2) : (namespaceName2.upper() +
                        tagName2.upper());
                if("/" + tag1Name != tag2Name)
                        return false; //namespace missmatch or not the closing tag
        }
        return true;
}

int QuantaCommon::denyBinaryInsert(QWidget *window)
{
  int result = KMessageBox::questionYesNo(window, i18n("The file type is not recognized. \
  Opening binary files may confuse Quanta.\n Are you sure you want to open this file?"),
  i18n("Unknown Type"), KStdGuiItem::open(), i18n("Do Not Open"), "Open Everything");
 return result;
}

bool QuantaCommon::checkOverwrite(const KURL& url, QWidget *window)
{
  bool result = true;

  if (QExtFileInfo::exists(url, false, window))
  {
    if (KMessageBox::warningContinueCancel(window,
            i18n( "<qt>The file <b>%1</b> already exists.<br>Do you want to overwrite it?</qt>" ).arg(url.prettyURL(0, KURL::StripFileProtocol)), QString::null, i18n("Overwrite")) == KMessageBox::Cancel)
            result = false;
  }

  return result;
}

QStringList QuantaCommon::readPathListEntry(KConfig *config, const QString &pKey)
{
  QStringList list = config->readPathListEntry(pKey);
  QStringList::Iterator end = list.end();
  for (QStringList::Iterator it = list.begin(); it != end; ++it)
  {
    KURL u = KURL::fromPathOrURL(*it);
    if (u.isValid() && u.isLocalFile())
    {
      u.setPath(QExtFileInfo::canonicalPath(u.path()));
      *it = u.url();
    }
  }
  return list;
}

QString QuantaCommon::readPathEntry(KConfig *config, const QString &pKey)
{
  QString path = config->readPathEntry(pKey);
  KURL u = KURL::fromPathOrURL(path);
  if (u.isValid() && u.isLocalFile())
  {
    u.setPath(QExtFileInfo::canonicalPath(u.path()));
    path = u.url();
  }
  return path;
}

QString QuantaCommon::encodedChar(uint code)
{
   
  if (replacementMap.contains(code))
    return QString("%1;").arg(replacementMap[code]);
  else
    return QString("&#%1;").arg(code);
}

