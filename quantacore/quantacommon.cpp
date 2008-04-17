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

//own includes
#include "quantacommon.h"
#include "extfileinfo.h"
#include "dtds.h"
#include "settings.h"
//#include "resource.h"


//system includes
#include <sys/types.h>
#include <unistd.h>

//qt includes
#include <QStringList>
#include <QDir>
#include <qdom.h>
#include <QFile>
#include <QTextStream>
#include <qwidget.h>

//kde includes
#include <kapplication.h>
#include <kurl.h>
#include <kdirwatch.h>
#include <kmimetype.h>
#include <kstandarddirs.h>
#include <kmessagebox.h>
#include <kio/netaccess.h>
#include <klocale.h>
#include <ktempdir.h>
#include <ktemporaryfile.h>

//remove the below ones when KQPasteAction is removed
#include <kdebug.h>
#include <kmenu.h>
#include <ktoolbar.h>
#include <kstringhandler.h>
#include <QWhatsThis>
#include <QTimer>
#include <QClipboard>
#include <qdatastream.h>

QuantaCommon::QuantaCommon(){
}

QuantaCommon::~QuantaCommon(){
}

/** convert tag to upper or lower case
moved to tag.cpp
*/
// QString QuantaCommon::tagCase( const QString& tag)
// {
//   switch (qConfig.tagCase)
//   {
//     case 1: return tag.toLower();
//     case 2: return tag.toUpper();
//   }
//   return tag;
// }

/** convert attr of tag to upper or lower case

moved to tagattr.cpp
*/
// QString QuantaCommon::attrCase( const QString& attr)
// {
//   switch (qConfig.attrCase)
//   {
//     case 1: return attr.toLower();
//     case 2: return attr.toUpper();
//   }
//   return attr;
// }

/** returns the attribute value in quoted form, by taking care of the
quotation setting

moved to tagattr.cpp
*/
// QString QuantaCommon::quoteAttributeValue(const QString& value)
// {
//   QString quote = Settings::self()->attributeQuotationChar();
//   return quote + value + quote;
// }


/**

moved to dtdstruct

  */
// bool QuantaCommon::isSingleTag(const QString& dtdName, const QString& tag)
// {
//   bool single = false;
//
//   //!doctype is a common tag to all DTDs not listed in the tagsList
//   if(tag.toLower() == "!doctype" || tag.toLower() == "?xml")
//     return true;
//
//   const DTDStruct* dtd = DTDs::ref()->contains(dtdName);
//   if (dtd && !tag.isEmpty())
//   {
//     QString searchForTag = (dtd->caseSensitive) ? tag : tag.upper();
//     QTag* qtag = dtd->tagsList->contains(searchForTag);
//     if (qtag)
//         single = qtag->isSingle();
//   }
//
//   return single;
// }

/** moved to dtdstruct */
// bool QuantaCommon::isOptionalTag(const QString& dtdName, const QString& tag)
// {
//   bool optional = false;
//
//   const DTDStruct* dtd = DTDs::ref()->contains(dtdName);
//   if (dtd && !tag.isEmpty())
//   {
//     QString searchForTag = (dtd->caseSensitive) ? tag : tag.upper();
//     QTag* qtag = dtd->tagsList->contains(searchForTag);
//     if (qtag)
//         optional = qtag->isOptional();
//   }
//
//   return optional;
// }
/** moved to dtdstruct  */
// bool QuantaCommon::isKnownTag(const QString& dtdName, const QString& tag)
// {
//   const DTDStruct* dtd = DTDs::ref()->contains(dtdName);
//   if (dtd && !tag.isEmpty())
//   {
//     QString searchForTag = (dtd->caseSensitive) ? tag : tag.upper();
//     if (dtd->tagsList->contains(searchForTag))
//       return true;
//   }
//   return false;
// }

// AttributeList* QuantaCommon::tagAttributes(const QString& dtdName, const QString& tag)
// {
//   AttributeList* attrs = 0L;
//
//   const DTDStruct* dtd = DTDs::ref()->contains(dtdName);
//   if (dtd && !tag.isEmpty())
//   {
//     QString searchForTag = (dtd->caseSensitive) ? tag : tag.upper();
//     QTag* qtag = dtd->tagsList->contains(searchForTag);
//     if (qtag)
//         attrs = qtag->attributes();
//   }
//
//   return attrs;
// }

/** Returns the QTag object for the tag "tag" from the DTD named "dtdname".

moved to qtag.cpp

*/
// QTag* QuantaCommon::tagFromDTD(const QString& dtdName, const QString& tag)
// {
//   const DTDStruct* dtd = DTDs::ref()->contains(dtdName);
//   return tagFromDTD(dtd, tag);
// }

/** Returns the QTag object for the tag "tag" from the DTD.

moved to qtag.cpp

*/
// QTag* QuantaCommon::tagFromDTD(const DTDStruct *dtd, const QString& tag)
// {
//   QTag *qtag = 0;
//   if (dtd && !tag.isEmpty())
//   {
//     QString searchForTag = (dtd->caseSensitive) ? tag : tag.upper();
//     qtag = dtd->tagsList->contains(searchForTag);
//   }
//
//  return qtag;
// }

/** Returns the QTag object for the node "node" from node's DTD.

moved to qtag.cpp

*/
// QTag* QuantaCommon::tagFromDTD(Node *node)
// {
//   if(!node || !node->tag)
//     return 0L;
//
//   return tagFromDTD(node->tag->dtd(), node->tag->name);
// }

/** Returns an XML style string containing the GUI for attributes. */
QString QuantaCommon::xmlFromAttributes(AttributeList* attributes)
{
 QString xmlStr;
 QTextStream stream( &xmlStr, IO_WriteOnly );
 stream.setCodec(QTextCodec::codecForName("UTF-8"));
 if (attributes)
 {
  int row = 0;
  for ( int i = 0; i< attributes->count();i++)
  {
    Attribute *attribute = attributes->at(i);
    QString name = attribute->name.left(1).toUpper() + attribute->name.right(attribute->name. length()-1);
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
      for (int j = 0; j < attribute->values.count(); j++)
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


/** Returns a pointer to a KStandardDirs object usable for plugin searchup. type
is the plugin binary type (exe or lib). The returned pointer must be deleted by
the caller!! */
KStandardDirs* QuantaCommon::pluginDirs(const char *type)
{
 KStandardDirs *dirs = new KStandardDirs();
/* dirs->addKDEDefaults();
 for (uint i = 0; i < qConfig.pluginSearchPaths.count(); i++)
 {
   dirs->addResourceDir(type, qConfig.pluginSearchPaths[i]);
 }*/
 return dirs;
}
/** Return true, if the url belong to the mimetype group. */
bool QuantaCommon::checkMimeGroup(const KUrl& url, const QString& group)
{
 bool status = false;
 KMimeType::Ptr mime = KMimeType::findByUrl(url);
 QString mimetype = mime->name();
 mimetype = mimetype.section('/',-1);
#if 0
 // This is really a slow way to lookup one mimetype - why do it this way? (DF)
 const KMimeType::List list = KMimeType::allMimeTypes(); // slow. We need a allMimeTypeNames for this.
 KMimeType::List::const_iterator it;
 for ( it = list.begin(); it != list.end(); ++it )
 {
    // ## why indexOf and not just == ?
    if ( ((*it)->name().startsWith(group)) && ((*it)->name().indexOf(mimetype) != -1)
)    {
      status = true;
      break;
    }
 }
#endif
 status = mime->name().startsWith(group);

 if (!status && group == "text")
 {
   if (url.isLocalFile())
   {
     status = !KMimeType::isBinaryData(url.path());
   } else
   {
     status = mime->is("text/plain");
   }
 }
 if (!status && group == "text" && mimetype == "x-zerosize")
     status = true;

 return status;
}

/** Return true, if the url has the mimetype type. */
// bool QuantaCommon::checkMimeType(const KUrl& url, const QString& type)
// {
//   QString mimetype = KMimeType::findByURL(url)->name();
//   mimetype = mimetype.section('/',-1);
//   return (mimetype == type);
// }

/** Return true, if the url has exactly the mimetype type. */
// bool QuantaCommon::checkExactMimeType(const KUrl& url, const QString& type)
// {
//   QString mimetype = KMimeType::findByURL(url)->name();
//   return (mimetype == type);
// }


/** Return a string to be used when an url is saved to the project file.
    Returns url.url() if it's an absolute url and
    url.path() if the url is relative */
QString QuantaCommon::qUrl(const KUrl &url)
{
  QString result = url.path();
  if (url.path().startsWith('/')) result = url.url();

  return result;
}

/** No descriptions */
void QuantaCommon::dirCreationError(QWidget *widget, const KUrl& url)
{
  KMessageBox::error(widget, i18n("<qt>Cannot create folder<br /><b>%1</b>.<br />Check that you have write permission in the parent folder or that the connection to<br /><b>%2</b><br />is valid.</qt>",
      url.pathOrUrl(),
      url.protocol()+"://"+url.user()+"@"+url.host()));}

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
      if (!keywords[i].isEmpty() && a_str == i18n(keywords[i].toUtf8()))
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
   pos = dtd->commentsStartRx.indexIn(str, pos);
   if (pos != -1)
   {
     s = dtd->commentsStartRx.cap();
     if (s == "\\\"" || s == "\\'")
     {
       str[pos] = space;
       str[pos+1] = space;
       pos += 2;
     } else
     {
       s = dtd->comments[s];
       l = str.indexOf(s, pos);
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
   pos = strRx.indexIn(str, pos);
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
   pos = dtd->commentsStartRx.indexIn(str, pos);
   if (pos == position)
     return true;
   if (pos != -1)
   {
     s = dtd->commentsStartRx.cap();
     if (s == "\\\"" || s == "\\'")
     {
       str[pos] = space;
       str[pos+1] = space;
       pos += 2;
     } else
     {
       s = dtd->comments[s];
       l = str.indexOf(s, pos);
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
   pos = strRx.indexIn(str, pos);
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

#if 0
DCOPReply QuantaCommon::callDCOPMethod(const QString& interface, const QString& method, const QString& arguments)
{
  QStringList argumentList = arguments.split(",", QString::KeepEmptyParts);
  QString app = "quanta";
  if (!kapp->inherits("KUniqueApplication"))
  {
    pid_t pid = ::getpid();
    app += QString("-%1").arg(pid);
  }
  DCOPRef quantaRef(app.toUtf8(), interface.toUtf8());
  DCOPReply reply;
  int argumentCount = argumentList.count();
  if (argumentCount == 0)
  {
    reply = quantaRef.call(method.toUtf8());
  }
  else if (argumentCount == 1)
  {
    reply = quantaRef.call(method.toUtf8(), argumentList[0]);
  }
  else if (argumentCount == 2)
    reply = quantaRef.call(method.toUtf8(), argumentList[0], argumentList[1]);
  else if (argumentCount == 3)
    reply = quantaRef.call(method.toUtf8(), argumentList[0], argumentList[1], argumentList[2]);
  else if (argumentCount == 4)
    reply = quantaRef.call(method.toUtf8(), argumentList[0], argumentList[1], argumentList[2], argumentList[3]);
  else if (argumentCount == 5)
    reply = quantaRef.call(method.toUtf8(), argumentList[0], argumentList[1], argumentList[2], argumentList[3], argumentList[4]);
  else if (argumentCount == 6)
    reply = quantaRef.call(method.toUtf8(), argumentList[0], argumentList[1], argumentList[2], argumentList[3], argumentList[4], argumentList[5]);
  else if (argumentCount == 7)
    reply = quantaRef.call(method.toUtf8(), argumentList[0], argumentList[1], argumentList[2], argumentList[3], argumentList[4], argumentList[5], argumentList[6]);
  else if (argumentCount == 8)
    reply = quantaRef.call(method.toUtf8(), argumentList[0], argumentList[1], argumentList[2], argumentList[3], argumentList[4], argumentList[5], argumentList[6], argumentList[7]);

  return reply;
}
#endif

void QuantaCommon::normalizeStructure(QString f,QStringList& l)
{
 f.remove("\t");
 f.remove("\n");
 f.remove("\r");

 while(f.contains("<"))
  {
     QString z(f);
     z.truncate(z.indexOf(">")+1);
     z.remove(0,z.indexOf("<"));
     f.remove(0,f.indexOf(">")+1);
     l.append(z);
  }
}


QString QuantaCommon::findWordRev(const QString& textToSearch, bool withHyphen)
{
  QRegExp regx;
  if (withHyphen)
    regx.setPattern(".*([_-\\w]+)\\s*$");
  else
    regx.setPattern(".*([_\\w]+)\\s*$");

  if (regx.indexIn(textToSearch) != -1)
    return regx.cap(1);

  return "";

//   static const QRegExp rx("\\W");
//   QString t = textToSearch;
//   while (t.endsWith(' '))
//     t = t.left(t.length()-1);
//   int startPos = -1;
//   int pos;
//   bool end = false;
//   do{
//     pos = t.lastIndexOf(rx, startPos);
//     if (t[pos] == '_' ||
//         (dtd && dtd->minusAllowedInWord && t[pos] == '-'))
//     {
//       startPos = pos - t.length()-1;
//       end = false;
//     } else
//     {
//       end = true;
//     }
//   } while (!end);
//   return t.remove(0,pos+1);
}
