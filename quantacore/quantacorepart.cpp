/***************************************************************************
    begin                : Thu Jun 16 2005
    copyright            : (C) 2005 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/
#include "quantacorepart.h"
#include "autocompletionconfig.h"
#include "environmentconfig.h"
#include "extfileinfo.h"
#include "ui_donationdialog.h"
#include "dtds.h"
#include "ui_dtdselectdialog.h"
#include "parsermanager.h"
#include "quantacommon.h"
#include "settings.h"
#include "tag.h"
#include "tagattr.h"
#include "tagdialogsif.h"
#include "helper.h"
#include "tagpair.h"

#include <QCheckBox>
#include <QLabel>
#include <QTimer>
#include <QMenu>
#include <qwidget.h>

#include <kaction.h>
#include <kapplication.h>
#include <kcombobox.h>
#include <kdebug.h>
#include <kdialog.h>
#include <kgenericfactory.h>
#include <kiconloader.h>
#include <klocale.h>
#include <kmainwindow.h>
#include <kpushbutton.h>
#include <kparts/part.h>
#include <kio/netaccess.h>
#include <kurl.h>
#include <kstdaction.h>
#include <ktempfile.h>
#include <kstandarddirs.h>
#include <ktoolinvocation.h>

//kdevelop includes
#include <interfaces/kdevmainwindow.h>
#include <interfaces/kdevdocumentcontroller.h>
#include <interfaces/kdevdocument.h>
#include <interfaces/kdevcore.h>

#include <ktexteditor/document.h>

typedef KGenericFactory<QuantaCorePart> QuantaCoreFactory;
K_EXPORT_COMPONENT_FACTORY(libkdevquantacore, QuantaCoreFactory("kdevquantacore"));

#define AUTOCOMPLETE_OPTIONS 1
#define ENVIRONMENT_OPTIONS 2

QuantaCorePart::QuantaCorePart(QObject *parent, const QStringList& )
  : QuantaCoreIf(QuantaCoreFactory::instance(), parent), m_activeQuantaDoc(0)
{
  kDebug(24000) << "Creating Quanta Support Part" << endl;
  setInstance(QuantaCoreFactory::instance());
  setXMLFile("kdevquantacore.rc");

  DTDs::ref(this)->find("dtd"); //load on startup;
  initActions();
  //FIXME: New KCM modules need to be created for each config page
  /*
  m_configProxy = new ConfigWidgetProxy(core());
  m_configProxy->createGlobalConfigPage(i18n("Autocompletion"), AUTOCOMPLETE_OPTIONS, info()->icon());
  m_configProxy->createGlobalConfigPage(i18n("Environment"), ENVIRONMENT_OPTIONS, info()->icon());

  connect(m_configProxy, SIGNAL(insertConfigWidget(const KDialog*, QWidget*, unsigned int )),
      this, SLOT(slotInsertConfigWidget(const KDialog*, QWidget*, unsigned int)));
  */
  connect(KDevApi::self()->documentController(), SIGNAL(documentLoaded(KDevDocument*)), this, SLOT(slotFileLoaded(KDevDocument*)));

  connect(KDevApi::self()->documentController(), SIGNAL(documentActivated(KDevDocument *)), this, SLOT(slotDocumentActivated(KDevDocument *)));

  connect(KDevApi::self()->documentController(), SIGNAL(documentClosed(KDevDocument*)), this, SLOT(slotClosedFile(KDevDocument*)));

  connect(KDevApi::self()->documentController(), SIGNAL(documentUrlChanged(KDevDocument*, const KUrl, const KUrl)), this, SLOT(slotPartURLChanged(KDevDocument*, const KUrl, const KUrl)));

  connect(KDevApi::self()->core(), SIGNAL(contextMenu(QMenu *, const Context *)), this, SLOT(contextMenu(QMenu *, const Context *)));

  QTimer::singleShot(0, this, SLOT(init()));
}


QuantaCorePart::~QuantaCorePart()
{
}

void QuantaCorePart::init()
{
  Settings::self()->readConfig();
  Settings::self()->setLoadedDTEPNickNames(DTDs::ref()->nickNameList(true));
  ParserManager *pm = ParserManager::self(this); //create the parser manager
  connect(pm, SIGNAL(startParsing(const EditorSource *)), SLOT(slotStartParsing(const EditorSource *)));
  connect(pm, SIGNAL(finishedParsing(const EditorSource *, const ParseResult *)), SLOT(slotFinishedParsing(const EditorSource *, const ParseResult *)));
  connect(pm, SIGNAL(groupsParsed(const EditorSource *, const ParseResult *)), SLOT(slotGroupsParsed(const EditorSource *, const ParseResult *)));
}

void QuantaCorePart::initActions()
{
  KActionCollection *ac = actionCollection();
  KAction *newAct = new KAction(i18n("&Quanta Homepage"), ac, "help_homepage");
  connect(newAct, SIGNAL(triggered(bool)), SLOT(slotHelpHomepage()));

  newAct = new KAction(i18n("&User Mailing List"), ac, "help_userlist" );
  connect(newAct, SIGNAL(triggered(bool)), SLOT(slotHelpUserList()));

  newAct = new KAction(i18n("Make &Donation"), ac, "help_donation" );
  connect(newAct, SIGNAL(triggered(bool)), SLOT(slotMakeDonation()));

  newAct = new KAction(i18n("Complete Text"), ac, "show_completion");
  newAct->setShortcut(KShortcut(Qt::CTRL + Qt::Key_Space));
  connect(newAct, SIGNAL(triggered(bool)), SLOT(slotShowCompletion()));

  newAct = new KAction(i18n("Show Completion Hints"), ac, "show_completion_hint");
  newAct->setShortcut(KShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_Space));
  connect(newAct, SIGNAL(triggered(bool)), SLOT(slotShowCompletionHint()));

  m_insertTagAction = new KAction(i18n("Insert &Tag"), ac, "insert_tag");
  connect(m_insertTagAction, SIGNAL(triggered(bool)), SLOT(slotInsertTag()));

  KStdAction::openNew(this, SLOT(slotOpenNew()), ac, "file_new");

  newAct = new KAction(i18n("&Change the DTEP..." ), ac, "change_dtd" );
  connect(newAct, SIGNAL(triggered(bool)), SLOT(slotChangeDTEP()));
}


void QuantaCorePart::insertTag(const TagPair & tagPair, bool inLine, bool showDialog)
{
  if (!m_activeQuantaDoc)
    return;

  QString s = tagPair.opening();
  if (s.startsWith("<"))
      s.remove(0, 1);
  if (s.endsWith(">"))
      s.truncate(s.length()-1);
  s = s.trimmed();
  int i = 0;
  while (!s[i].isSpace() && !s[i].isNull())
    i++;
  QString name = s.left(i);
  QString attributes = s.remove(0, i).trimmed();
  if (showDialog && m_activeQuantaDoc->mainDTEP()->isKnownTag(name))
  {
    TagDialogsIf *tagDialog = extension<TagDialogsIf>("KDevelop/TagDialogs");
    if (tagDialog)
    {
      QString selection = m_activeQuantaDoc->selection();
      TagPair newTagPair = tagDialog->createNewTag(QTag::tagFromDTD(m_activeQuantaDoc->mainDTEP(), name), selection, attributes, KUrl()); //FIXME baseURL instead of KUrl!
      if (!inLine)
      {
        newTagPair.closing().prepend("\n");
      }
      m_activeQuantaDoc->insertTag(newTagPair);
      return;
    }
  }

  QString s1 = Tag::convertCase(name);
  if (tagPair.opening().startsWith("<"))
    s1.prepend("<");
  if (!attributes.isEmpty())
    s1 += " " + TagAttr::convertCase(attributes);
  if (tagPair.opening().endsWith(">"))
  {
    QTag *dtdTag = QTag::tagFromDTD(m_activeQuantaDoc->mainDTEP(), name);
    if (m_activeQuantaDoc->mainDTEP()->singleTagStyle == "xml" && dtdTag &&
        (dtdTag->isSingle() || (!Settings::self()->closeOptionalTags() && dtdTag->isOptional()))
      )
    {
      s1.append(" /");
    }
    s1.append(">");
  }
  QString s2;
  if (!tagPair.closing().isEmpty())
  {
    s2 = Tag::convertCase(tagPair.closing());
    if (!inLine)
    {
      s2.prepend("\n");
    }
  }
  m_activeQuantaDoc->insertTag(TagPair(s1, s2));
}


void QuantaCorePart::slotInsertConfigWidget(const KDialog *dlg, QWidget *page, unsigned int pageNo)
{
    //FIXME: New KCM modules need to be created for each config page
// create configuraton dialogs here
  /*
    switch (pageNo)
    {
        case AUTOCOMPLETE_OPTIONS:
        {
            AutocompletionConfig *w = new AutocompletionConfig(page);
            connect(dlg, SIGNAL(okClicked()), w, SLOT(accept()));
            break;
        }
        case ENVIRONMENT_OPTIONS:
        {
            EnvironmentConfig *w = new EnvironmentConfig(page);
            connect(dlg, SIGNAL(okClicked()), w, SLOT(accept()));
            break;
        }
}*/
}


void QuantaCorePart::slotMakeDonation()
{
  KDialog dlg(KDevApi::self()->mainWindow()->main() );
  dlg.setCaption( i18n("Support Quanta with Financial Donation") );
  dlg.setButtons( KDialog::Close );
  dlg.setDefaultButton( KDialog::Close );
  QWidget *w = new QWidget(&dlg);
  Ui::DonationDialog form;
  form.setupUi(w);
  dlg.setMainWidget(w);
  dlg.exec();
}

void QuantaCorePart::slotHelpHomepage()
{
  KToolInvocation::invokeBrowser("http://kdewebdev.org");
}

void QuantaCorePart::slotHelpUserList()
{
  KToolInvocation::invokeBrowser("http://mail.kde.org/mailman/listinfo/quanta");
}

void QuantaCorePart::slotFileLoaded(KDevDocument* document)
{
//   kDebug(24000) << "slotFileLoaded: " << url << endl;
  KTextEditor::Document *doc = document->textDocument();
  if (doc)
  {
    m_activeQuantaDoc = new QuantaDoc(doc, this);
    m_documents.insert(document->url().url(), m_activeQuantaDoc);
  }
}


void QuantaCorePart::slotStartParsing(const EditorSource *source)
{
if (source == m_activeQuantaDoc)
   emit startParsing(); // signal in QuantaCoreIf
}


void QuantaCorePart::slotFinishedParsing(const EditorSource *source, const ParseResult *parseResult)
{
  if (source == m_activeQuantaDoc)
    emit finishedParsing(parseResult); // signal in QuantaCoreIf
}


void QuantaCorePart::slotGroupsParsed(const EditorSource *source, const ParseResult *parseResult)
{
  kDebug(24000) << "Group parsing done" << endl;
  if (source == m_activeQuantaDoc)
    emit groupsParsed(parseResult); // signal in QuantaCoreIf
}

void QuantaCorePart::slotDocumentActivated(KDevDocument *document)
{
  m_activeQuantaDoc = 0;
  KParts::ReadOnlyPart * part = dynamic_cast<KParts::ReadOnlyPart *>(document->part());
  if (! part)
  {
    emit finishedParsing(0); // clear the trees
    return;
  }
  QuantaDoc * doc = m_documents.value(part->url().url());
  if (doc && doc->isDocument(part))
  {
    m_activeQuantaDoc = doc;
    emit finishedParsing(doc->parseResult()); // signal in QuantaCoreIf
    emit groupsParsed(doc->parseResult()); // signal in QuantaCoreIf
  } else
  {
    emit finishedParsing(0); // clear the trees
  }
}

void QuantaCorePart::slotClosedFile(KDevDocument* document)
{
//   kDebug(24000) << "-----------slotClosedFile " << url.url() << endl;
  QuantaDoc * doc = m_documents.value(document->url().url());
  if (doc)
  {
    delete doc;
    m_documents.remove(document->url().url());
    if (doc == m_activeQuantaDoc)
      emit finishedParsing(0); // notify that current doc was closed
  }
}


void QuantaCorePart::slotPartURLChanged(KDevDocument* document, const KUrl &oldUrl, const KUrl &newUrl)
{
  QuantaDoc * doc = m_documents.value(oldUrl.url());
  if (doc)
  {
    m_documents.remove(oldUrl.url());
    m_documents.insert(newUrl.url(), doc);
  }
}


void QuantaCorePart::slotInsertTag(const KUrl& url, Helper::DirInfo * dirInfo)
{
  if (m_activeQuantaDoc)
  {
    KUrl baseURL = m_activeQuantaDoc->parseResult()->baseURL;
//FIXME     if  (w->isUntitled() )
//     {
//       baseURL = Project::ref()->projectBaseURL();
//     } else
    KUrl relURL = KUrl::relativeUrl(baseURL, url);
    QString urlStr = relURL.url();
    if (relURL.protocol() == baseURL.protocol())
        urlStr = relURL.path();

    if (dirInfo && (!dirInfo->preText.isEmpty() || !dirInfo->postText.isEmpty()))
    {
      m_activeQuantaDoc->insertText(dirInfo->preText + urlStr + dirInfo->postText);
    } else
    {
      bool isImage = false;
      if (KMimeType::findByURL(url)->name().startsWith("image/"))
      {
        QString imgFileName;
        KIO::NetAccess::download(url, imgFileName, KDevApi::self()->mainWindow()->main());
        QImage img(imgFileName);
        if (!img.isNull())
        {
          QString width,height;
          width.setNum(img.width());
          height.setNum(img.height());
          QString imgTag = Tag::convertCase("<img ");
          imgTag += TagAttr::convertCase("src=");
          imgTag += TagAttr::quoteAttributeValue(urlStr);
          imgTag += TagAttr::convertCase(" width=");
          imgTag += TagAttr::quoteAttributeValue(width);
          imgTag += TagAttr::convertCase(" height=");
          imgTag += TagAttr::quoteAttributeValue(height);
          imgTag += TagAttr::convertCase(" border=");
          imgTag += TagAttr::quoteAttributeValue(QString("%1").arg(0));
          imgTag += ">";
          m_activeQuantaDoc->insertText(imgTag);
          isImage = true;
        }
        KIO::NetAccess::removeTempFile(imgFileName);
      }
      if (!isImage)
      {
        m_activeQuantaDoc->insertTag(TagPair::createLinkTag(urlStr));
      }
    }
  }
}


void QuantaCorePart::slotInsertTag()
{
  KUrl::List::ConstIterator it;
  for (it = m_fileContextList.constBegin(); it != m_fileContextList.constEnd(); ++it)
  {
    slotInsertTag(*it, 0);
  }
  m_fileContextList.clear();
}

void QuantaCorePart::contextMenu(QMenu *popup, const Context *context)
{
  if (m_activeQuantaDoc && context->hasType(Context::FileContext))
  {
    m_fileContextList = static_cast<const FileContext*>(context)->urls();
    popup->addSeparator();
    popup->addAction(m_insertTagAction);
  }
}


void QuantaCorePart::slotOpenNew()
{
  KTempFile * file = new KTempFile(KStandardDirs::locateLocal("tmp", ""), i18n(".unsaved"));
//   file->setAutoDelete(true);
  file->close();
  KUrl url = KUrl(file->name());
  KDevDocument *doc = KDevApi::self()->documentController()->editDocument(url);
  if (doc)
  {
    KParts::ReadOnlyPart *part = dynamic_cast<KParts::ReadOnlyPart *>(doc->part());
    if (part)
      part->closeURL();
  }
  file->unlink();
}

void QuantaCorePart::slotChangeDTEP()
{
  if (m_activeQuantaDoc)
  {
    KDialog dlg(KDevApi::self()->mainWindow()->main() );
    dlg.setCaption( i18n("DTEP Selector") );
    dlg.setButtons( KDialog::Ok | KDialog::Cancel );
    dlg.setDefaultButton( KDialog::Ok );
    QWidget *w = new QWidget(&dlg);
    Ui::DTDSelectDialog form;
    form.setupUi(w);
    w->setMinimumHeight(130);
    dlg.setMainWidget(w);
    int pos = -1;
    int defaultIndex = 0;

    QString oldDtdName = m_activeQuantaDoc->mainDTEP()->name;
//FIXME    QString defaultDocType = Project::ref()->defaultDTD();
    QString defaultDocType = Settings::self()->defaultDTEP();
    QStringList lst = DTDs::ref()->nickNameList(true);

    QString oldDtdNickName = DTDs::ref()->getDTDNickNameFromName(oldDtdName);
    QString defaultDtdNickName = DTDs::ref()->getDTDNickNameFromName(defaultDocType);
    for(int i = 0; i < lst.count(); ++i)
    {
      form.dtdCombo->addItem(lst[i]);
      if (lst[i] == oldDtdNickName)
        pos = i;
      if (lst[i] == defaultDtdNickName)
        defaultIndex = i;
    }

    if (pos == -1)
      pos = defaultIndex;
    form.dtdCombo->setCurrentIndex(pos);
    form.messageLabel->setText(i18n("Change the current DTD."));
    form.currentDTD->setText(m_activeQuantaDoc->mainDTEP()->nickName);
    //dlg->useClosestMatching->setShown(false);
    delete form.useClosestMatching;
    form.useClosestMatching = 0L;
    w->adjustSize();
    if (dlg.exec())
    {
      const DTDStruct *dtd = DTDs::ref()->find(DTDs::ref()->getDTDNameFromNickName(form.dtdCombo->currentText()));
      m_activeQuantaDoc->setMainDTEP(dtd);
      if (form.convertDTD->isChecked() && dtd->family == DTDStruct::Xml)
      {
/*        if (tag)
        {
          int bLine, bCol, eLine, eCol;
          tag->beginPos(bLine,bCol);
          tag->endPos(eLine,eCol);
          w->editIf->removeText(bLine, bCol, eLine, eCol+1);
          w->viewCursorIf->setCursorPositionReal((uint)bLine, (uint)bCol);
          w->insertText("<!DOCTYPE" + dtd->doctypeStr +">");
          delete tag;
        } else*/
        {
          m_activeQuantaDoc->setCursorPosition(0,0);
          m_activeQuantaDoc->insertText("<!DOCTYPE" + dtd->doctypeStr + ">\n");
        }
      }
    }

    //FIXME slotLoadToolbarForDTD(w->getDTDIdentifier());
    m_activeQuantaDoc->parse();
  }
}


QString QuantaCorePart::getDTEPNickName(const QString &name) const
{
  return DTDs::ref()->getDTDNickNameFromName(name.toLower());
}

#include "quantacorepart.moc"
