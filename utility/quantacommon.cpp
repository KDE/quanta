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
//qt includes
#include <qstringlist.h>
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
#include <kprogress.h>
#include <ktempfile.h>

//remove the below ones when KQPasteAction is removed
#include <dcopclient.h>
#include <dcopref.h>
#include <kdebug.h>
#include <kpopupmenu.h>
#include <ktoolbar.h>
#include <ktoolbarbutton.h>
#include <kapplication.h>
#include <kstringhandler.h>
#include <qwhatsthis.h>
#include <qtimer.h>
#include <qclipboard.h>
#include <qdatastream.h>

#include "quantacommon.h"
#include "tag.h"
//#include "resource.h"


QConfig qConfig; //holds the main configuration settings
QString tmpDir;

QDict<DTDStruct> *dtds; //holds all the known tags with attributes for each DTD.
QRegExp scriptBeginRx;
QRegExp scriptEndRx;

Node *baseNode;
Parser *parser;
KDirWatch *fileWatcher;
KProgress *progressBar;

QString toolbarExtension = ".toolbar.tgz";
QRegExp newLineRx("\\n");
QStringList charList; //hold the list of the &char; chars. See the data/chars file.
QPtrList<KTempFile> tempFileList;
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
bool QuantaCommon::isOptionalTag(const QString& dtdName, const QString& tag)
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
bool QuantaCommon::isKnownTag(const QString& dtdName, const QString& tag)
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

AttributeList*  QuantaCommon::tagAttributes(const QString& dtdName, const QString& tag)
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
QTag* QuantaCommon::tagFromDTD(const QString& dtdName, const QString& tag)
{
  DTDStruct* dtd = dtds->find(dtdName.lower());
  return tagFromDTD(dtd, tag);
}

/** Returns the QTag object for the tag "tag" from the DTD. */
QTag* QuantaCommon::tagFromDTD(DTDStruct *dtd, const QString& tag)
{
  QTag *qtag = 0;
  if (dtd)
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

  return tagFromDTD(node->tag->dtd, node->tag->name);
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

/** Returns the DTD name (identifier) corresponding to the DTD's nickname */
QString QuantaCommon::getDTDNameFromNickName(const QString& nickName)
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
QString QuantaCommon::getDTDNickNameFromName(const QString& name)
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
#if KDE_VERSION < KDE_MAKE_VERSION(3,1,90)
      Format f = findFormatByFileContent(url.path());
      if (f.text && f.compression == Format::NoCompression)
          status = true;
#else
      KMimeType::Format f = KMimeType::findFormatByFileContent(url.path());
      if (f.text && f.compression == KMimeType::Format::NoCompression)
          status = true;
#endif
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
  KMessageBox::error(widget, i18n("<qt>Can't create directory<br><b>%1</b>.<br>Check that you have write permission in the parent directory or that the connection to<br><b>%2</b><br> is valid!</qt>")
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

#if KDE_VERSION < KDE_MAKE_VERSION(3,1,90)

#define GZIP_MAGIC1     0x1f
#define GZIP_MAGIC2     0x8b

Format QuantaCommon::findFormatByFileContent( const QString &fileName )
{
  Format result;
  result.compression = Format::NoCompression;
  KMimeType::Ptr mime = KMimeType::findByPath(fileName);
  if (mime->name() == "application/octet-stream")
     mime =  KMimeType::findByFileContent(fileName);

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

QString QuantaCommon::obscure( const QString &str )
{
  QString result;
  for ( uint i = 0; i < str.length(); ++i )
    result += ( str[ i ].unicode() < 0x20 ) ? str[ i ] :
        QChar( 0x1001F - str[ i ].unicode() );

  return result;
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
    QString tag1Name = tag1->dtd->caseSensitive ? tag1->name : tag1->name.upper();
    QString tag2Name = tag2->dtd->caseSensitive ? tag2->name : tag2->name.upper();
    if ("/" + tag1Name != tag2Name)
      return false; //not the closing tag
  } else
  {
    if (tag2->nameSpace.isEmpty())
      return false; //namespace missmatch
    QString tag1Name = tag1->dtd->caseSensitive ? (tag1->nameSpace + tag1->name) : (tag1->nameSpace.upper() + tag1->name.upper());
    QString tag2Name = tag2->dtd->caseSensitive ? (tag2->nameSpace + tag2->name) : (tag2->nameSpace.upper() + tag2->name.upper());
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

int QuantaCommon::denyBinaryInsert()
{
  int result = KMessageBox::warningYesNo(0L, i18n("The file type is not recognized. \
  Opening binary files may confuse Quanta.\n Are you sure you want to open this file?"),
  i18n("Unknown Type"), KStdGuiItem::yes(), KStdGuiItem::no(), "Open Everything");
 return result;
}


KQPasteAction::KQPasteAction( const QString& text,
                            const QString& icon,
                            const KShortcut& cut,
                            const QObject* receiver,
                            const char* slot, QObject* parent,
                            const char* name)
  : KAction( text, icon, cut, receiver, slot, parent, name )
{
  m_popup = new KPopupMenu;
  connect(m_popup, SIGNAL(aboutToShow()), this, SLOT(menuAboutToShow()));
  connect(m_popup, SIGNAL(activated(int)), this, SLOT(menuItemActivated(int)));
  m_popup->setCheckable(true);
}

KQPasteAction::~KQPasteAction()
{
  delete m_popup;
}

int KQPasteAction::plug( QWidget *widget, int index )
{
  if (kapp && !kapp->authorizeKAction(name()))
    return -1;
  // This is very related to KActionMenu::plug.
  // In fact this class could be an interesting base class for KActionMenu
  if ( widget->inherits( "KToolBar" ) )
  {
    KToolBar *bar = (KToolBar *)widget;

    int id_ = KAction::getToolButtonID();

    KInstance * instance;
    if ( m_parentCollection )
        instance = m_parentCollection->instance();
    else
        instance = KGlobal::instance();

    bar->insertButton( icon(), id_, SIGNAL( clicked() ), this,
                       SLOT( slotActivated() ), isEnabled(), plainText(),
                       index, instance );

    addContainer( bar, id_ );

    connect( bar, SIGNAL( destroyed() ), this, SLOT( slotDestroyed() ) );

    bar->setDelayedPopup( id_, m_popup, true );

    if ( !whatsThis().isEmpty() )
        QWhatsThis::add( bar->getButton( id_ ), whatsThisWithIcon() );

    return containerCount() - 1;
  }

  return KAction::plug( widget, index );
}

void KQPasteAction::menuAboutToShow()
{
    m_popup->clear();
    QStringList list;
    DCOPClient *client = kapp->dcopClient();
    if (client->isAttached() &&
        client->isApplicationRegistered("klipper"))
    {
      DCOPRef klipper("klipper","klipper");
      DCOPReply reply = klipper.call("getClipboardHistoryMenu");
      if (reply.isValid())
      {
        list = reply;
      }
    }
    QString clipboardText = qApp->clipboard()->text(QClipboard::Clipboard);
    clipboardText.replace("&", "&&");
    clipboardText = KStringHandler::csqueeze(clipboardText, 45);
    if (list.isEmpty())
        list << clipboardText;
    bool found = false;
    for ( QStringList::Iterator it = list.begin(); it != list.end(); ++it )
    {
      int id = m_popup->insertItem(*it);
      if (!found && *it == clipboardText)
      {
        m_popup->setItemChecked(id, true);
        found = true;
      }
    }
}

void KQPasteAction::menuItemActivated( int id)
{
    DCOPClient *client = kapp->dcopClient();
    if (client->isAttached() && client->isApplicationRegistered("klipper"))
    {
      DCOPRef klipper("klipper","klipper");
      DCOPReply reply = klipper.call("getClipboardHistoryItem(int)", m_popup->indexOf(id));
      if (!reply.isValid())
        return;
      QString clipboardText = reply;
      reply = klipper.call("setClipboardContents(QString)", clipboardText);
      if (reply.isValid())
        kdDebug(24000) << "Clipboard: " << qApp->clipboard()->text(QClipboard::Clipboard) << endl;
    }
    QTimer::singleShot(20, this, SLOT(slotActivated()));
}

class KQRecentFilesAction::KQRecentFilesActionPrivate
{
public:
  KQRecentFilesActionPrivate()
  {
    m_maxItems = 0;
    m_popup = 0;
  }
  uint m_maxItems;
  KPopupMenu *m_popup;
};

KQRecentFilesAction::KQRecentFilesAction( const QString& text,
                                        const QString& pix,
                                        const KShortcut& cut,
                                        const QObject* receiver,
                                        const char* slot,
                                        QObject* parent, const char* name,
                                        uint maxItems )
  : KListAction( text, pix, cut, parent, name)
{
  d = new KQRecentFilesActionPrivate;
  d->m_maxItems = maxItems;

  init();

  if ( receiver )
    connect( this,     SIGNAL(urlSelected(const KURL&)),
             receiver, slot );
}

void KQRecentFilesAction::init()
{
  connect( this, SIGNAL( activated( const QString& ) ),
           this, SLOT( itemSelected( const QString& ) ) );

  setMenuAccelsEnabled( false );
}

KQRecentFilesAction::~KQRecentFilesAction()
{
  delete d->m_popup;
  delete d; d = 0;
}

KPopupMenu *KQRecentFilesAction::popupMenu() const
{
  if ( !d->m_popup ) {
    KQRecentFilesAction *that = const_cast<KQRecentFilesAction*>(this);
    that->d->m_popup = new KPopupMenu;
    connect(d->m_popup, SIGNAL(aboutToShow()), this, SLOT(menuAboutToShow()));
    connect(d->m_popup, SIGNAL(activated(int)), this, SLOT(menuItemActivated(int)));
  }
  return d->m_popup;
}

uint KQRecentFilesAction::maxItems() const
{
    return d->m_maxItems;
}

void KQRecentFilesAction::setMaxItems( uint maxItems )
{
    QStringList lst = items();
    uint oldCount   = lst.count();

    // set new maxItems
    d->m_maxItems = maxItems;

    // remove all items that are too much
    while( lst.count() > maxItems )
    {
        // remove last item
        lst.remove( lst.last() );
    }

    // set new list if changed
    if( lst.count() != oldCount )
        setItems( lst );
}

void KQRecentFilesAction::addURL( const KURL& url )
{
    QString     file = url.prettyURL();
    if ( url.isLocalFile() && !KGlobal::dirs()->relativeLocation("tmp", url.path()).startsWith("/"))
       return;
    QStringList lst = items();

    // remove file if already in list
    lst.remove( file );

    // remove las item if already maxitems in list
    if( lst.count() == d->m_maxItems )
    {
        // remove last item
        lst.remove( lst.last() );
    }

    // add file to list
    lst.prepend( file );
    setItems( lst );
}

void KQRecentFilesAction::removeURL( const KURL& url )
{
    QStringList lst = items();
    QString     file = url.prettyURL();

    // remove url
    if( lst.count() > 0 )
    {
        lst.remove( file );
        setItems( lst );
    }
}

void KQRecentFilesAction::clearURLList()
{
    clear();
}

void KQRecentFilesAction::loadEntries( KConfig* config, QString groupname)
{
    QString     key;
    QString     value;
    QString     oldGroup;
    QStringList lst;

    oldGroup = config->group();

    if (groupname.isEmpty())
      groupname = "RecentFiles";
    config->setGroup( groupname );

    // read file list
    for( unsigned int i = 1 ; i <= d->m_maxItems ; i++ )
    {
        key = QString( "File%1" ).arg( i );
        value = config->readPathEntry( key );

        if (!value.isNull())
            lst.append( value );
    }

    // set file
    setItems( lst );

    config->setGroup( oldGroup );
}

void KQRecentFilesAction::saveEntries( KConfig* config, QString groupname )
{
    QString     key;
    QString     value;
    QString     oldGroup;
    QStringList lst = items();

    oldGroup = config->group();

    if (groupname.isEmpty())
      groupname = "RecentFiles";
    config->deleteGroup( groupname, true );
    config->setGroup( groupname );

    // write file list
    for( unsigned int i = 1 ; i <= lst.count() ; i++ )
    {
        key = QString( "File%1" ).arg( i );
        value = lst[ i - 1 ];
        config->writePathEntry( key, value );
    }

    config->setGroup( oldGroup );
}

void KQRecentFilesAction::itemSelected( const QString& text )
{
    emit urlSelected( KURL( text ) );
}

void KQRecentFilesAction::menuItemActivated( int id )
{
    emit urlSelected( KURL(popupMenu()->text(id)) );
}

void KQRecentFilesAction::menuAboutToShow()
{
    KPopupMenu *menu = popupMenu();
    menu->clear();
    QStringList list = items();
    for ( QStringList::Iterator it = list.begin(); it != list.end(); ++it )
        menu->insertItem(*it);
}

int KQRecentFilesAction::plug( QWidget *widget, int index )
{
  if (kapp && !kapp->authorizeKAction(name()))
    return -1;
  // This is very related to KActionMenu::plug.
  // In fact this class could be an interesting base class for KActionMenu
  if ( widget->inherits( "KToolBar" ) )
  {
    KToolBar *bar = (KToolBar *)widget;

    int id_ = KAction::getToolButtonID();

    KInstance * instance;
    if ( m_parentCollection )
        instance = m_parentCollection->instance();
    else
        instance = KGlobal::instance();

    bar->insertButton( icon(), id_, SIGNAL( clicked() ), this,
                       SLOT( slotClicked() ), isEnabled(), plainText(),
                       index, instance );

    addContainer( bar, id_ );

    connect( bar, SIGNAL( destroyed() ), this, SLOT( slotDestroyed() ) );

    bar->setDelayedPopup( id_, popupMenu(), true);

    if ( !whatsThis().isEmpty() )
        QWhatsThis::add( bar->getButton( id_ ), whatsThisWithIcon() );

    return containerCount() - 1;
  }

  return KListAction::plug( widget, index );
}

void KQRecentFilesAction::slotClicked()
{
  KAction::slotActivated();
}

void KQRecentFilesAction::slotActivated()
{
  emit activated( currentItem() );
  emit activated( currentText() );
}


#include "quantacommon.moc"
