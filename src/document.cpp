/***************************************************************************
                          document.cpp  -  description
                             -------------------
    begin                : Tue Jun 6 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon <pdima@users.sourceforge.net,yshurik@penguinpowered.com,sequitur@easystreet.com>
                           (C) 2001-2004 Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include <list>
#include <cctype>
#include <cstdlib>
#include <stdlib.h>

//QT includes
#include <qcheckbox.h>
#include <qdir.h>
#include <qeventloop.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qlineedit.h>
#include <qtextcodec.h>
#include <qtextstream.h>
#include <qregexp.h>
#include <qradiobutton.h>

// KDE includes
#include <kapplication.h>
#include <kwin.h>
#include <klocale.h>
#include <kaction.h>
#include <kactionclasses.h>
#include <kdialogbase.h>
#include <kiconloader.h>
#include <kmdcodec.h>
#include <kmessagebox.h>
#include <ktempfile.h>
#include <kdirwatch.h>
#include <kdebug.h>
#include <kprogress.h>
#include <kio/netaccess.h>
#include <kstandarddirs.h>

#include <ktexteditor/document.h>
#include <ktexteditor/view.h>
#include <ktexteditor/cursorinterface.h>
#include <ktexteditor/clipboardinterface.h>
#include <ktexteditor/codecompletioninterface.h>
#include <ktexteditor/configinterface.h>
#include <ktexteditor/editinterface.h>
#include <ktexteditor/editinterfaceext.h>
#include <ktexteditor/encodinginterface.h>
#include <ktexteditor/selectioninterface.h>
#include <ktexteditor/selectioninterfaceext.h>
#include <ktexteditor/viewcursorinterface.h>
#include <ktexteditor/wordwrapinterface.h>
#include <ktexteditor/markinterfaceextension.h>

#include <kate/view.h>

#include "tag.h"
#include "quantacommon.h"
#include "document.h"
#include "resource.h"
#include "dirtydlg.h"
#include "dirtydialog.h"
#include "casewidget.h"
#include "project.h"
#include "dtdselectdialog.h"

#include "quanta.h"
#include "quantaview.h"
#include "structtreeview.h"
#include "qextfileinfo.h"
#include "viewmanager.h"
#include "messageoutput.h"

#include "undoredo.h"

#include "dtds.h"

#include "sagroupparser.h"

#define STEP 1

extern GroupElementMapList globalGroupMap;

Document::Document(KTextEditor::Document *doc,
                   QWidget *parent, const char *name, WFlags f )
  : QWidget(parent, name, f)
{
  m_dirty   = false;
  busy    = false;
  changed = false;
  m_md5sum = "";
  m_doc = doc;
  m_view = 0L; //needed, because createView() calls processEvents() and the "this" may be deleted before m_view gets a value => crash on delete m_view;
  m_view = m_doc->createView(parent, 0L);
  completionInProgress = false;
  argHintVisible = false;
  completionRequested = false;
  userTagList.setAutoDelete(true);

  // remove the unwanted actions
  KAction *a = m_view->actionCollection()->action( "file_export" );
  if (a)
      m_view->actionCollection()->take(a);
  a =  m_view->actionCollection()->action( "file_save" );
  if (a)
    m_view->actionCollection()->take(a);
  a = m_view->actionCollection()->action( "file_save_as" );
  if (a)
    m_view->actionCollection()->take(a);
  a = m_view->actionCollection()->action( "file_reload" );
  if (a)
    m_view->actionCollection()->take(a);
  a = m_view->actionCollection()->action( "edit_undo" );
  if (a)
    m_view->actionCollection()->take(a);
  a = m_view->actionCollection()->action( "edit_redo" );
  if (a)
    m_view->actionCollection()->take(a);
  //because they are not implemented in VPL

  a = m_view->actionCollection()->action( "edit_copy" );
  if (a)
  m_view->actionCollection()->take(a);
  a = m_view->actionCollection()->action( "edit_cut" );
  if (a)
    m_view->actionCollection()->take(a);
  a = m_view->actionCollection()->action( "edit_paste" );
  if (a)
    m_view->actionCollection()->take(a);

  //conflicting shortcuts, so change them
  a = m_view->actionCollection()->action("view_border");
  if (a)
    a->setShortcut(Qt::SHIFT + Qt::Key_F9);

  a = m_view->actionCollection()->action("view_folding_markers");
  if (a)
    a->setShortcut(Qt::SHIFT + Qt::Key_F11);

  KActionMenu *bookmarkAction = dynamic_cast<KActionMenu*>(m_view->actionCollection()->action( "bookmarks" ));
  if (bookmarkAction)
  {
    m_view->actionCollection()->take(bookmarkAction);
    //kdDebug(24000) << "Bookmarks found!" << endl;
    //bookmarkAction->insert(quantaApp->actionCollection()->action( "file_quit" ));
  }

  editIf = dynamic_cast<KTextEditor::EditInterface *>(m_doc);
  editIfExt = dynamic_cast<KTextEditor::EditInterfaceExt *>(m_doc);
  encodingIf = dynamic_cast<KTextEditor::EncodingInterface*>(m_doc);
  m_encoding = quantaApp->defaultEncoding();
  if (encodingIf)
        m_encoding = encodingIf->encoding();
  if (m_encoding.isEmpty())
      m_encoding = "utf8";  //final fallback
  m_codec = QTextCodec::codecForName(m_encoding);

  selectionIf = dynamic_cast<KTextEditor::SelectionInterface *>(m_doc);
  selectionIfExt = dynamic_cast<KTextEditor::SelectionInterfaceExt *>(m_doc);
  configIf = dynamic_cast<KTextEditor::ConfigInterface*>(m_doc);
  if (configIf)
      configIf->readConfig();
  viewCursorIf = dynamic_cast<KTextEditor::ViewCursorInterface *>(m_view);
  codeCompletionIf = dynamic_cast<KTextEditor::CodeCompletionInterface *>(m_view);
  markIf = dynamic_cast<KTextEditor::MarkInterface *>(m_doc);
  KTextEditor::MarkInterfaceExtension* iface = dynamic_cast<KTextEditor::MarkInterfaceExtension*>(m_doc);
  if (iface)
  {
    iface->setPixmap(KTextEditor::MarkInterface::markType07, SmallIcon("stop"));
    iface->setPixmap(KTextEditor::MarkInterface::markType02, SmallIcon("debug_breakpoint"));
    iface->setDescription(KTextEditor::MarkInterface::markType02, i18n("Breakpoint"));
    iface->setPixmap(KTextEditor::MarkInterface::markType05, SmallIcon("debug_currentline"));
    iface->setDescription(KTextEditor::MarkInterface::markType08, i18n("Annotation"));
    iface->setPixmap(KTextEditor::MarkInterface::markType08, SmallIcon("stamp"));

    // This is allows user to set breakpoints and bookmarks by clicking or rightclicking on the icon border.
    iface->setMarksUserChangable(KTextEditor::MarkInterface::markType01 + KTextEditor::MarkInterface::markType02);

  }

  tempFile = 0;
  m_tempFileName = QString::null;
  dtdName = Project::ref()->defaultDTD();
  reparseEnabled = true;
  repaintEnabled = true;
  delayedTextChangedEnabled = true;
  docUndoRedo = new undoRedo(this);

  //path of the backup copy file of the document
  m_backupPathValue = QString::null;

  connect( m_doc,  SIGNAL(charactersInteractivelyInserted (int ,int ,const QString&)),
           this,  SLOT(slotCharactersInserted(int ,int ,const QString&)) );

  connect( m_view, SIGNAL(completionAborted()),
           this,  SLOT(  slotCompletionAborted()) );

  connect( m_view, SIGNAL(completionDone(KTextEditor::CompletionEntry)),
           this,  SLOT(  slotCompletionDone(KTextEditor::CompletionEntry)) );

  connect( m_view, SIGNAL(filterInsertString(KTextEditor::CompletionEntry*,QString *)),
           this,  SLOT(  slotFilterCompletion(KTextEditor::CompletionEntry*,QString *)) );
  connect( m_doc, SIGNAL(textChanged()), SLOT(slotTextChanged()));

  connect(m_view, SIGNAL(gotFocus(Kate::View*)), SIGNAL(editorGotFocus()));

  connect(fileWatcher, SIGNAL(dirty(const QString&)), SLOT(slotFileDirty(const QString&)));

//   connect(m_doc, SIGNAL(marksChanged()), this, SLOT(slotMarksChanged()));
  connect(m_doc, SIGNAL(markChanged(KTextEditor::Mark, KTextEditor::MarkInterfaceExtension::MarkChangeAction)), this, SLOT(slotMarkChanged(KTextEditor::Mark, KTextEditor::MarkInterfaceExtension::MarkChangeAction)));

}

Document::~Document()
{
  if (configIf)
      configIf->writeConfig();
  parser->clearGroups();
  //  kdDebug(24000) << "Document::~ Document: " << this << endl;
  m_doc->closeURL(false); //TODO: Workaround for a Kate bug. Remove when KDE < 3.2.0 support is dropped.
  delete m_doc;
}

void Document::setUntitledUrl(const QString &url)
{
  untitledUrl = url;
  openURL(KURL());
}

bool Document::isUntitled()
{
  return (m_doc->url().url().isEmpty()) ? true : false;
}

KURL Document::url()
{
  return ( isUntitled() ) ? KURL("file:"+untitledUrl) : m_doc->url();
}

// kwrite addons

void Document::insertTag(const QString &s1, const QString &s2)
{
  QString selection;

  if (selectionIf && selectionIf->hasSelection())
  {
    reparseEnabled = false;
    selection = selectionIf->selection();
    selectionIf->removeSelectedText();
    reparseEnabled = true;
  }
  insertText(s1 + selection);
  insertText(s2, false); // don't adjust cursor, thereby leaving it in the middle of tag
}


/** Change the current tag's attributes with those from dict */
void Document::changeTag(Tag *tag, QDict<QString> *dict )
{
  tag->modifyAttributes(dict);
  QString tagStr = tag->toString();

  reparseEnabled = false;
  int bLine, bCol, eLine, eCol;
  tag->beginPos(bLine,bCol);
  tag->endPos(eLine,eCol);
  editIf->removeText(bLine, bCol, eLine, eCol+1);
  viewCursorIf->setCursorPositionReal((uint)bLine, (uint)bCol);
  insertText(tagStr);
}

/**Change the namespace in a tag. Add if it's not present, or remove if the
namespace argument is empty*/
void Document::changeTagNamespace(Tag *tag, const QString& nameSpace)
{
  int bl, bc;
  int nl, nc;
  if (!tag->nameSpace.isEmpty())
  {
    tag->beginPos(bl, bc);
    if (tag->type == Tag::XmlTagEnd)
      bc++;
    tag->namePos(nl, nc);
    reparseEnabled = false;
    editIf->removeText(bl, bc + 1, nl, nc);
    reparseEnabled = true;
  } else
  {
    tag->beginPos(bl, bc);
    if (tag->type == Tag::XmlTagEnd)
      bc++;
  }
  if (!nameSpace.isEmpty())
  {
    viewCursorIf->setCursorPositionReal((uint)bl, (uint)(bc + 1));
    insertText(nameSpace + ":", true, false);
  }
  slotDelayedTextChanged(true);
  quantaApp->slotNewLineColumn();
}

/**Change the attr value of the called attrName to attrValue*/
void Document::changeTagAttribute(Tag *tag, const QString& attrName, const QString& attrValue)
{
  QString value;
  int line, col;
  int index = tag->attributeIndex(attrName);
  if (index != -1)
  {
    int endCol;
    value = tag->attributeValue(index);
    if (value == attrValue)
        return;
    int aLine, aCol;
    tag->attributeNamePos(index, aLine, aCol);
    tag->attributeValuePos(index, line, col);
    if (line == aLine && col == aCol)
    {
      col += tag->attribute(index).length();
      value = QString("=") + qConfig.attrValueQuotation + attrValue + qConfig.attrValueQuotation;
    } else
    {
      endCol = col + value.length();
      if (attrValue.isEmpty())
      {
        tag->attributeNamePos(index, line, col);
        endCol++;
      }
      reparseEnabled = false;
      QString textLine = editIf->textLine(line);
      while (col > 1 && textLine[col-1].isSpace())
        col--;

      editIf->removeText(line, col, line, endCol);
      reparseEnabled = true;
      value = attrValue;
    }
  } else
  {
    index = tag->attrCount() - 1;
    if (tag->attribute(index) == "/")
    {
      tag->attributeNamePos(index, line, col);
      col--;
    } else
    {
      tag->endPos(line, col);
    }
    if (attrValue.isEmpty())
    {
      value = "";
    } else
    {
      value = " " + QuantaCommon::attrCase(attrName) + "=" + qConfig.attrValueQuotation + attrValue + qConfig.attrValueQuotation;
    }
  }
  if (!value.isEmpty())
  {
    viewCursorIf->setCursorPositionReal((uint)line, (uint)col);
    insertText(value);
  }
  quantaApp->slotNewLineColumn();

  //else
//  slotDelayedTextChanged();
}


void Document::selectText(int x1, int y1, int x2, int y2 )
{
  if (selectionIf)
    selectionIf->setSelection(x1, y1, x2, y2);
}


void Document::replaceSelected(const QString &s)
{
  if (selectionIf)
  {
    unsigned int line, col;

    viewCursorIf->cursorPositionReal(&line, &col);
    reparseEnabled = false;
    selectionIf->removeSelectedText();
    reparseEnabled = true;
    editIf->insertText(line, col, s);
  }
}

void Document::insertFile(const KURL& url)
{
  QString fileName;
  if (url.isLocalFile())
  {
    fileName = url.path();
  } else
  {
    if (!KIO::NetAccess::download(url, fileName, this))
    {
      KMessageBox::error(this, i18n("<qt>Cannot download <b>%1</b>.</qt>").arg( url.prettyURL(0, KURL::StripFileProtocol)));
      return;
    }
  }
  QFile file(fileName);
  if (file.open(IO_ReadOnly))
  {
    QTextStream stream( &file );
    stream.setEncoding(QTextStream::UnicodeUTF8);
    insertText(stream.read());
    file.close();
  } else
    KMessageBox::error(this, i18n("<qt>Cannot open <b>%1</b> for reading.</qt>").arg(url.prettyURL(0, KURL::StripFileProtocol)));
}

/** Inserts text at the current cursor position */
void Document::insertText(const QString &a_text, bool adjustCursor, bool reparse)
{
  QString text = a_text;
  if(text.isEmpty())
    return;

  reparseEnabled = false;
  unsigned int line, col;

  viewCursorIf->cursorPositionReal(&line, &col);
  Node *n = parser->nodeAt(line, col, true);
  if (n && n->tag->dtd()->family != Xml)
  {
    int bLine, bCol;
    n->tag->beginPos(bLine, bCol);
    QString s = this->text(bLine, bCol, line, col);
    bool insideQuotes = false;
    for (int i = 0 ; i < (int)s.length() - 1; i++)
    {
      if (s[i] == '"' && (i == 0 || s[i-1] != '\\'))
        insideQuotes = !insideQuotes;
    }
    int eLine, eCol;
    n->tag->endPos(eLine, eCol);
    s = this->text(line + 1, col, eLine, eCol);
    bool closeQuotationFound = false;
    for (int i = 0 ; i < (int)s.length() - 1; i++)
    {
      if (s[i] == '"' && (i == 0 || s[i-1] != '\\'))
      {
        closeQuotationFound = true;
        break;
      }
    }
    if (insideQuotes && closeQuotationFound)
    {
      text.replace("\\\"", "\"");
      text.replace("\"", "\\\"");
    }
  }

  editIf->insertText(line, col, text);

  // calculate new cursor position
  // counts the words and whitespace of the text so we can place the
  // cursor correctly and quickly with the viewCursorInterace, avoiding
  // the KTexEditor::insertText method
  if(adjustCursor)
  {
    unsigned textLength = text.length();
    unsigned int wordWrapAt = 80;
    bool noWordWrap = false;
    KTextEditor::WordWrapInterface *wordWrapIf = dynamic_cast<KTextEditor::WordWrapInterface *>(m_doc);
    if (wordWrapIf)
    {
        wordWrapAt = wordWrapIf->wordWrapAt();
        noWordWrap = !(wordWrapIf->wordWrap());
     }
    uint i=0, j=0;
    int wordLength;
    const char *ascii = text.latin1(); // use ascii for maximum speed
    bool lineLock =false;

    while(i < textLength)
    {
      if(ascii[i] == '\n') // add a line, first column
      {
         ++line; col=0; ++i; lineLock = false;
      }
      else if(ascii[i] == '\r')
      {
        col = 0; ++i;
      }
      else if(!noWordWrap && !(isspace(ascii[i]))) // new word, see if it wraps
      {
      // TOO SLOW  int wordLength = (text.mid(i).section(QRegExp("[ \t\r\n]"), 0, 0).length());
        wordLength = -1;
        for(j = i+1;ascii[j];++j) // get word size, ascii is MUCH faster
        {
            if(isspace(ascii[j]))
            {
              wordLength = j-i;
              break;
           }
        }
        if(wordLength == -1)
          wordLength = (textLength)-i;

        if((wordLength+col) > wordWrapAt)
        {
          if(col && !lineLock) // wraps onto new line unless locked by kate
          {
            col=0;
            ++line;
          }
        }
        col += wordLength;
        i += wordLength;
        if(wordLength > (int) wordWrapAt)
          lineLock = true; // words > wordWrapAt lock the rest of the line
      }
      else // whitespace
      {
        ++col; ++i;
        if(!noWordWrap)
          if(col > wordWrapAt && !lineLock)  // wrap like words
          {
            col -= wordWrapAt;
            ++line;
          }
      }
    }
  }
  viewCursorIf->setCursorPositionReal(line, col);
  reparseEnabled = true;
  if (reparse)
  {
    baseNode = parser->rebuild(this);
    if (qConfig.instantUpdate && quantaApp->structTreeVisible())
    {
      typingInProgress = false;
      StructTreeView::ref()->slotReparse(this, baseNode , qConfig.expandLevel);
    }
    quantaApp->updateTreeViews();
  }
}

bool Document::insertChildTags(QTag *tag, QTag *lastTag)
{
  bool childInserted = false;
  if (!tag || tag == lastTag) //avoid infinite recursion
  {
    return false;
  }
  QMap<QString, bool>::Iterator it;
  for (it = tag->childTags.begin(); it != tag->childTags.end(); ++it)
  {
    if (it.data())
    {
      childInserted = true;
      QTag *childTag = QuantaCommon::tagFromDTD(tag->parentDTD, it.key());
      QString tagStr =QuantaCommon::tagCase(it.key());
      if ( tag->parentDTD->singleTagStyle == "xml" &&
           ( childTag->isSingle() ||
            (childTag->isOptional() && !qConfig.closeOptionalTags)) )
      {
        insertText("<" +tagStr + "/>", true, false);
      } else
      {
        insertText("<" +tagStr + ">", true, false);
      }
      QString closingStr;
      if (insertChildTags(childTag, tag))
      {
        closingStr = "";
      }
      if ( (!childTag->isSingle() && !childTag->isOptional() && qConfig.closeTags) ||
           (childTag->isOptional() && qConfig.closeOptionalTags) )
      {
        insertText(closingStr + "</" + tagStr + ">", true, false);
      }
    }
  }
  return childInserted;
}

/** Get the view of the document */
KTextEditor::View* Document::view()
{
  return m_view;
}

/** Get the KTextEditor::Document of the document */
KTextEditor::Document* Document::doc()
{
  return m_doc;
}

/** Returns true if the document was modified. */
bool Document::isModified()
{
  bool modified = false;
  if (m_doc)
   modified = m_doc->isModified();

  return modified;
}

void Document::setModified(bool flag)
{
  if (m_doc)
    m_doc->setModified(flag);
}

void Document::createTempFile()
{
  closeTempFile();
  tempFile = new KTempFile(tmpDir);
  tempFile->setAutoDelete(true);
  m_tempFileName = QFileInfo(*(tempFile->file())).filePath();
  QString encoding = quantaApp->defaultEncoding();
  if (encodingIf)
        encoding = encodingIf->encoding();
  if (encoding.isEmpty())
      encoding = "utf8";  //final fallback
  tempFile->textStream()->setCodec(QTextCodec::codecForName(encoding));
  * (tempFile->textStream()) << editIf->text();

  m_tempFileName = QFileInfo(*(tempFile->file())).filePath();
  tempFile->close();
// kdDebug(24000) << "Creating tempfile " << m_tempFileName << " for " << url() << endl;
}

void Document::closeTempFile()
{
 if (tempFile != 0)
 {
   delete tempFile;
   tempFile = 0L;
 }
 if (QFileInfo(m_tempFileName).exists())
     QFile::remove(m_tempFileName);

 m_tempFileName = QString::null;
}

QString Document::tempFileName()
{
 return m_tempFileName;
}


/** This will return the current tag name at the given position.
    It will work even if the tag has not been completed yet. An
    empty string will be returned if no tag is found.
*/
QString Document::getTagNameAt(int line, int col )
{
 QString name = "";
 QString textLine = editIf->textLine(line);
 textLine = textLine.left(col);
 while (line >= 0)
 {
    QuantaCommon::removeCommentsAndQuotes(textLine, completionDTD);
    int pos = textLine.findRev("<");
    int pos2 = textLine.findRev(">");
    if (pos != -1 && pos2 < pos)
    {
      textLine.remove(0, pos + 1);
      pos = 0;
      while (pos < (int)textLine.length() &&
            !textLine[pos].isSpace() &&
            textLine[pos] != '>')
            pos++;
      name = textLine.left(pos).stripWhiteSpace();
      pos = name.find(":");
      if (pos != -1)
        name = name.mid(pos + 1);
      break;
    } else
    {
      if (pos2 == -1)
      {
        line--;
        if (line >= 0)
          textLine = editIf->textLine(line);
      } else
      {
        name = "";
        break;
      }
    }
 }

 return name;
}

/** Show the code completions passed in as an argument */
void Document::showCodeCompletions( QValueList<KTextEditor::CompletionEntry> *completions ) {
  bool reparse = reparseEnabled;
  reparseEnabled = false;
  codeCompletionIf->showCompletionBox( *completions, false );
  reparseEnabled = reparse;
  argHintVisible = false;
  delete completions;
}

/** Once the completed text has been inserted into the document we
    want to update the cursor position.
*/
void Document::slotCompletionDone( KTextEditor::CompletionEntry completion )
{
  unsigned int line,col;
  completionInProgress = false;
  argHintVisible = false;
  viewCursorIf->cursorPositionReal(&line,&col);
  const DTDStruct* dtd = currentDTD();
/*  if (completion.type == "charCompletion")
  {
    m_lastCompletionList = getCharacterCompletions(completion.userdata);
    QTimer::singleShot(0, this, SLOT(slotDelayedShowCodeCompletion()));
  } else*/
  if (completion.type == "attribute")
  {
    viewCursorIf->setCursorPositionReal(line,col-1);
    if (dtd)
    {
      QTag *tag = QuantaCommon::tagFromDTD(dtd,completion.userdata);
      if (tag)
      {
        m_lastCompletionList = getAttributeValueCompletions(tag->name(), completion.text);
        QTimer::singleShot(0, this, SLOT(slotDelayedShowCodeCompletion()));
      }
    }
  } else
  if (completion.type == "attributeValue")
  {
    viewCursorIf->setCursorPositionReal(line, col);
  } else
  if (completion.type == "doctypeList")
  {
    viewCursorIf->setCursorPositionReal(line,col+1);
  } else
  if (completion.type == "script")
  {
    viewCursorIf->setCursorPositionReal(line,col);
    if (dtd)
    {
      m_lastLine = line;
      m_lastCol = col - 1;
      QTimer::singleShot(0, this, SLOT(slotDelayedScriptAutoCompletion()));
    }
  }
}

void Document::slotDelayedScriptAutoCompletion()
{
  scriptAutoCompletion(m_lastLine, m_lastCol, "");
}

void Document::slotDelayedShowCodeCompletion()
{
  showCodeCompletions(m_lastCompletionList);
}

/** This is called when the user selects a completion. We
    can filter this completion to allow more intelligent
    code compeltions
*/
void Document::slotFilterCompletion( KTextEditor::CompletionEntry *completion ,QString *string )
{
  kdDebug(24000) << *string << endl;
  kdDebug(24000) << completion->userdata << endl;
  int pos = completion->userdata.find("|");
  QString s = completion->userdata.left(pos);
  completion->userdata.remove(0,pos+1);
  string->remove(0, s.length());
  kdDebug(24000) << *string << endl;
  kdDebug(24000) << completion->userdata << endl;
  if (completion->type == "charCompletion")
  {
    *string = completion->userdata;
    uint line, col;
    viewCursorIf->cursorPositionReal(&line, &col);
    QString s2 = editIf->textLine(line).left(col);
    kdDebug(24000) << s2 << endl;
    int pos = s2.findRev('&');
    if (pos != -1)
    {
      s2 = s2.mid(pos + 1);
      string->remove(s2);
    }
    string->append(";");
    kdDebug(24000) << *string << endl;
  } else
  if ( completion->type == "attributeValue")
  {
    uint line, col;
    viewCursorIf->cursorPositionReal(&line, &col);
    QString textLine = editIf->textLine(line);
    QChar tagSeparator = completionDTD->tagSeparator;
    if (tagSeparator == '\'' || tagSeparator =='"')
        tagSeparator = qConfig.attrValueQuotation;
    if (textLine[col] != tagSeparator)
        string->append(tagSeparator);
  } else
  if ( completion->type == "attribute" )
  {
    string->append("="+QString(qConfig.attrValueQuotation)+QString(qConfig.attrValueQuotation));
  } else
  if (completion->type == "doctypeList")
  {
    s = *string;
    string->remove(0, string->length());
    QString s2 = QString("public \""+DTDs::ref()->getDTDNameFromNickName(s)+"\"");
    const DTDStruct *dtd = DTDs::ref()->find(DTDs::ref()->getDTDNameFromNickName(s));
    if (dtd && !dtd->url.isEmpty())
    {
      s2 += " \""+dtd->url+"\"";
    }
    string->append(QuantaCommon::attrCase(s2));
  } else
  if (completion->type == "script")
  {
    string->append(completionDTD->attrAutoCompleteAfter);
  }
}

void Document::slotReplaceChar()
{
  reparseEnabled = false;
  editIf->removeText(m_replaceLine, m_replaceCol, m_replaceLine, m_replaceCol+1);
  insertText(m_replaceStr, true, false);
}
/** Called when a user types in a character. From this we can show possibile
    completions based on what they are trying to input.
*/
void Document::slotCharactersInserted(int line, int column, const QString& string)
{
  if (qConfig.replaceNotInEncoding)
  {
    if (encodingIf)
    {
      QString encoding = encodingIf->encoding();
      if (encoding != m_encoding)
      {
        m_encoding = encoding;
        m_codec = QTextCodec::codecForName(encoding);
      }
      if (!m_codec->canEncode(string[0]))
      {
        m_replaceLine = line;
        m_replaceCol = column;
        m_replaceStr = QuantaCommon::encodedChar(string[0].unicode());
        QTimer::singleShot(0, this, SLOT(slotReplaceChar()));
        return;
      }
    }
  }
  if (qConfig.replaceAccented)
  {
    uint c = string[0].unicode();
    if (c > 191)
    {
      m_replaceLine = line;
      m_replaceCol = column;
      m_replaceStr = QuantaCommon::encodedChar(c);
      QTimer::singleShot(0, this, SLOT(slotReplaceChar()));
      return;
    }
  }


 if ( (string == ">") ||
      (string == "<") )
 {
   slotDelayedTextChanged(true);
 }
 bool handled = false;
 if (qConfig.useAutoCompletion)
 {
  if (completionInProgress)
  {
    handleCodeCompletion();
  } else
  {
    completionDTD = currentDTD();
    if (completionDTD->family == Xml)
    {
      handled = xmlAutoCompletion(line, column, string);
    }
    if (completionDTD->family == Script)
    {
      handled = scriptAutoCompletion(line, column, string);
      if (!handled && string == ">")
      {
         Node *node = parser->nodeAt(line, column, false);
         if (node && node->tag->validXMLTag && node->tag->type == Tag::ScriptTag)
         {
            column++;
            editIf->insertText(line, column, "</" + node->tag->name + ">");
            viewCursorIf->setCursorPositionReal( line, column );
        }
      }
      handled = true;
    }

    if (!handled)
    {
      const DTDStruct *lastDTD = completionDTD;
      completionDTD = defaultDTD();
      if (lastDTD != completionDTD && completionDTD->family == Xml)
      {
        handled = xmlAutoCompletion(line, column, string);
      }
/*TODO: Can the default DTD be a script?
      if (dtd->family == Script)
      {
        scriptAutoCompletion(dtd, line, column, string);
      }
*/
    }
  }
 }
}

/** Called whenever a user inputs text in an XML type document.
    Returns true if the code completionw as handled.
*/
bool Document::xmlAutoCompletion(int line, int column, const QString & string)
{
  QTag *tag;
  QString tagName;
  bool handled = false;
  tagName = getTagNameAt(line, column);
  tag = QuantaCommon::tagFromDTD(completionDTD, tagName);
  if (!tag && !tagName.isEmpty())
     tag = userTagList.find(tagName.lower());

  QString s = editIf->textLine(line).left(column + 1);
  bool namespacecompletion = false;
  if (!tagName.isEmpty() && string ==":" && s.endsWith("<" + tagName + ":"))
    namespacecompletion = true;
  int i = column;
  while (i > 0 && s[i].isSpace())
    i--;
  s = s.left(i + 1);

  if ( !tag || tagName.isEmpty() || namespacecompletion)  //we are outside of any tag
  {

    if (s.endsWith(completionDTD->tagAutoCompleteAfter) ||
        namespacecompletion)  // a tag is started, either with < or <namespace:
    {
      //we need to complete a tag name
      showCodeCompletions( getTagCompletions(line, column + 1) );
      handled = true;
    } else
    if (string == ">" && !tagName.isEmpty() && tagName[0] != '!' && tagName[0] != '?' &&
        tagName[0] != '/' && !tagName.endsWith("/") && !s.endsWith("/>") &&
        qConfig.closeTags &&
        currentDTD(true)->family == Xml) //close unknown tags
    {
      //add closing tag if wanted
      column++;
      editIf->insertText(line, column, "</" + tagName + ">");
      docUndoRedo->dontAddModifsSet(2);
      viewCursorIf->setCursorPositionReal( line, column );
      handled = true;
    } else
    if (string == "/" && s.endsWith("</") && tagName.isEmpty())
    {
      Node *node = parser->nodeAt(line, column, false);
      if (node && node->parent )
      {
        node = node->parent;
        if (node->tag->type == Tag::XmlTag && (!node->next || !QuantaCommon::closesTag(node->tag, node->next->tag)))
        {
            QString name = node->tag->name;
            name = name.left(name.find(" | "));
            if (!node->tag->nameSpace.isEmpty())
              name.prepend(node->tag->nameSpace + ":");
            editIf->insertText(line, column + 1, name + ">");
            docUndoRedo->dontAddModifsSet(2);
            viewCursorIf->setCursorPositionReal( line, column + name.length() + 2);
            handled = true;
        }
      }
    }
  }
  else  // we are inside of a tag
  {
    if ( string == ">" && tagName[0] != '/' && !tagName.endsWith("/") &&
         !s.endsWith("/>") && tag)
    {
      if ( tag->parentDTD->singleTagStyle == "xml" &&
           (tag->isSingle() || (!qConfig.closeOptionalTags && tag->isOptional()))
         )
      {
        editIf->insertText(line, column, " /");
        docUndoRedo->dontAddModifsSet(2);
        viewCursorIf->setCursorPositionReal( line, column+3 );
        handled = true;
      }
      if ( ( !tag->isSingle() && !tag->isOptional() && qConfig.closeTags) ||
           ( tag->isOptional() && qConfig.closeOptionalTags ) )
      {
        //add closing tag if wanted
        Node *node = parser->nodeAt(line, column, false);
        if (node && (!node->next || !QuantaCommon::closesTag(node->tag, node->next->tag)))
        {
            if (node && !node->tag->nameSpace.isEmpty())
                tagName.prepend(node->tag->nameSpace + ":");
            column++;
            editIf->insertText(line, column, "</" + tagName + ">");
            docUndoRedo->dontAddModifsSet(2);
            viewCursorIf->setCursorPositionReal( line, column );
            handled = true;
        }
      }
      if (!tag->childTags.isEmpty())
      {
        reparseEnabled = false;
 //       insertText("\n", false, false);
        insertChildTags(tag);
        reparseEnabled = true;
        baseNode = parser->rebuild(this);
        if (qConfig.instantUpdate && quantaApp->structTreeVisible())
        {
          typingInProgress = false;
          StructTreeView::ref()->slotReparse(this, baseNode , qConfig.expandLevel);
        }
      }
    }
    else if ( string == " " )
         {
           QString textLine = editIf->textLine(line);
           if (!QuantaCommon::insideCommentsOrQuotes(column, textLine, completionDTD))
           {
             showCodeCompletions(getAttributeCompletions(tagName, ""));
             handled = true;
           }
         }
    else if ( string[0] == qConfig.attrValueQuotation )
          {
            //we need to find the attribute name
            QString textLine = editIf->textLine(line).left(column-1);
            QString attribute = textLine.mid(textLine.findRev(' ')+1);
            if (attribute == "style" && completionDTD->insideDTDs.contains("css"))
            {
              completionDTD = DTDs::ref()->find("text/css");
              completionRequested = true;
              return scriptAutoCompletion(line, column + 1, string);
            }
            showCodeCompletions( getAttributeValueCompletions(tagName, attribute) );
            handled = true;
          }
  } // else - we are inside of a tag
  if (!handled)
  {
    //check if we are inside a style attribute, and use css autocompletion if we are
    QString textLine = editIf->textLine(line);
    textLine = textLine.left(column);
    int pos = textLine.findRev('"');
    if (pos != -1)
    {
      pos = textLine.findRev(' ', pos);
      if (pos != -1)
      {
        textLine = textLine.mid(pos + 1);
        pos = textLine.find('=');
        if (pos != -1)
        {
          QString attribute = textLine.left(pos);
          if (attribute == "style" && completionDTD->insideDTDs.contains("css"))
          {
            completionDTD = DTDs::ref()->find("text/css");
            completionRequested = true;
            return scriptAutoCompletion(line, column + 1, string);
          }
        }
      }
    }
    QString s = editIf->textLine(line).left(column + 1);
    pos = s.findRev('&');
    if (pos != -1)
    {
      //complete character codes
      s = s.mid(pos + 1);
      showCodeCompletions(getCharacterCompletions(s));
      handled = true;
    }
  }
 return handled;
}

/** Return a list of possible variable name completions */
QValueList<KTextEditor::CompletionEntry>* Document::getGroupCompletions(Node *node, const StructTreeGroup& group, int line, int col)
{
  QValueList<KTextEditor::CompletionEntry> *completions = new QValueList<KTextEditor::CompletionEntry>();
  KTextEditor::CompletionEntry completion;

  completion.type = "variable";

  QString textLine = editIf->textLine(line).left(col);
  QString word = findWordRev(textLine);
  if (!group.removeFromAutoCompleteWordRx.pattern().isEmpty())
      word.remove(group.removeFromAutoCompleteWordRx);
  completion.userdata = word + "|";
  GroupElementMapList::Iterator it;
  QString str = group.name;
  str.append("|");
  str.append(word);
  for ( it = globalGroupMap.begin(); it != globalGroupMap.end(); ++it )
  {
    if (it.key().startsWith(str) && it.key() != str )
    {
      GroupElementList elementList = it.data();
      for (uint i = 0; i < elementList.count(); i++)
      {
        if (elementList[i]->parentNode == 0L || elementList[i]->global)
        {
          completion.text = it.key().section('|', -1).stripWhiteSpace();
          completions->append(completion);
          break;
        } else
        {
          Node *n = node;
          while (n && n != elementList[i]->parentNode)
          {
            n = n->parent;
          }
          if (n == elementList[i]->parentNode)
          {
            completion.text = it.key().section('|', -1).stripWhiteSpace();
            completions->append(completion);
            break;
          }
        }
      }
    }
  }
  IncludedGroupElementsMap elements = parser->includedMap;
  IncludedGroupElementsMap::Iterator it2;
  for ( it2 = elements.begin(); it2 != elements.end(); ++it2 )
  {
    QStringList list = it2.data()[group.name].keys();
    list.sort();
    for (uint i = 0; i < list.count(); i++)
    {
      if (list[i].startsWith(word) && list[i] != word)
      {
        completion.text = list[i].stripWhiteSpace();
        completions->append(completion);
      }
    }
  }


  return completions;
}

bool Document::isDerivatedFrom(const QString& className, const QString &baseClass)
{
  if (className.isEmpty() || !completionDTD->classInheritance.contains(className))
    return false;

  QString parentClass = completionDTD->classInheritance[className];
  int result = 0;
  do {
    if (parentClass == baseClass)
      result = 1; //className extends baseClass
    else
    {
      if (completionDTD->classInheritance.contains(parentClass))
        parentClass = completionDTD->classInheritance[parentClass];
      else
        result = -1;//nothing was found in the inheritance list
    }
  } while (result == 0);

  return (result == 1);
}


/** Return a list of possible tag name completions */
QValueList<KTextEditor::CompletionEntry>* Document::getTagCompletions(int line, int col)
{
  QValueList<KTextEditor::CompletionEntry> *completions = new QValueList<KTextEditor::CompletionEntry>();
  KTextEditor::CompletionEntry completion;
  switch (completionDTD->family)
  {
    case Xml: completion.type = "tag";
              break;
    case Script: 
              completion.type = "script";
              break;
  }
  Node *node = parser->nodeAt(line, col);
  if (node && node->tag->type != Tag::XmlTag)
      node = node->parent;
  if (node && node->tag->type != Tag::XmlTag)
      node = 0L;
  QTag *parentQTag= 0L;
  if (node && node->parent)
      parentQTag = QuantaCommon::tagFromDTD(node->parent);
  QString textLine = editIf->textLine(line).left(col);
  QString word = findWordRev(textLine, completionDTD).upper();
  QString classStr = "";
  QString objStr;
  if (completionDTD->classGroupIndex != -1 && completionDTD->objectGroupIndex != -1)
  {
    textLine = textLine.left(textLine.length() - word.length());
    int pos = completionDTD->memberAutoCompleteAfter.searchRev(textLine);
    if (pos != -1)
    {
      textLine = textLine.left(pos);
      QRegExp *r =  &(completionDTD->structTreeGroups[completionDTD->classGroupIndex].usageRx);
      pos = r->searchRev(textLine);
      if (pos != -1)
      {
        objStr = r->cap(1);
        if (objStr == "this")
        {
          QString parentGroupStr = "";
          bool classFound = false;
          parser->synchParseInDetail(); 
          Node *n = parser->nodeAt(line, col);
          while (n && !classFound)
          {
            //Need to parser for groups, as the node tree is rebuilt before
            //autocompletion and none of the node has links to group elements
            //at this position.
            SAGroupParser *gParser = new SAGroupParser(0L, this, n, n->nextSibling(), true, false, false);
            gParser->slotParseForScriptGroup();
            GroupElementList::Iterator it = n->m_groupElements.begin();
            while (it != n->m_groupElements.end())
            {
              GroupElement *e = *it;
              if (parentGroupStr.isEmpty() && e->group->appendToTags)
              {
                parentGroupStr = e->group->parentGroup;
              }
              if (!parentGroupStr.isEmpty() && e->group->name == parentGroupStr)
              {
                classStr = e->tag->name;
                classFound = true;
              }
              //detach the groupelement from the node
              e->node = 0L;
              e->group = 0L;
              e->deleted = true;
              it = n->m_groupElements.erase(it);
            }
            delete gParser;
            n = n->parent;
          }
        } else
        {
          GroupElementList groupElementList = globalGroupMap[completionDTD->structTreeGroups[completionDTD->objectGroupIndex].name + "|" + objStr];
          for (GroupElementList::Iterator it = groupElementList.begin(); it != groupElementList.end(); ++it)
          {
            if (!(*it)->tag)
              continue;
#ifdef DEBUG_PARSER
            kdDebug(24000) << "GroupElement: " << (*it) << " " << (*it)->tag->area().bLine << " " << (*it)->tag->area().bCol << " "<< (*it)->tag->area().eLine << " "<< (*it)->tag->area().eCol << " " << (*it)->tag->tagStr() << " " << (*it)->type << endl;
#endif
            if (!(*it)->type.isEmpty())
            {
              classStr = (*it)->type;
              break;
            }
          }
        }
      }
    }
    if ((!objStr.isEmpty() || !completionRequested) && classStr.isEmpty()) //the class cannot be identified for the object or there is no object.
      return completions;
  }
  completion.userdata = word + "|";
  QStringList tagNameList;
  QMap<QString, QString> comments;
  //A QMap to hold the completion type (function/string/class/etc)
  QMap<QString, QString> type;
  QString tagName;
  QDictIterator<QTag> it(*(completionDTD->tagsList));
  int i = 0;
  for( ; it.current(); ++it )
  {
    QTag *tag = it.current();
    if ((tag->type != "entity") && (tag->className == classStr ||
        isDerivatedFrom(classStr, tag->className)))
    {
      tagName = tag->name();
      if (!tagName.isEmpty() && tagName.upper().startsWith(word))
      {
        if (!parentQTag || (parentQTag && parentQTag->isChild(tagName)))
        {
          tagName = tag->name() + QString("%1").arg(i, 10);
          tagNameList += tagName;
          comments.insert(tagName, tag->comment);
          i++;
        }
      }
    }
  }

  QDictIterator<QTag> it2(userTagList);
  for( ; it2.current(); ++it2 )
  {
    QTag *tag = it2.current();
    if ((tag->className == classStr ||
         isDerivatedFrom(classStr, tag->className)) && tag->name().upper().startsWith(word))
    {
      tagName = tag->name() + QString("%1").arg(i, 10);
      tagNameList += tagName;
      comments.insert(tagName, tag->comment);
      
       // If the completion family is script, then we want to update the tag type
       // it appears we use "script" for adding the completionDTD->attrAutoCompleteAfter when we run the slotFilterCompletion
       // so we will continue to use that for functions (they need the attribute added), but variables get a new type - and we do not
       // have to auto-complete them
      if(completionDTD->family==Script)
      {
        if(tag->type=="variable")
          type.insert(tagName, tag->type);
        else if(tag->type=="function")
          type.insert(tagName, "script");
        
        // We add the type to the comment variable, so it displays on the screen, giving the user some feedback
        if(comments[tagName].length())
          comments[tagName] = tag->type + "\n" + comments[tagName];
        else
          comments[tagName] = tag->type + comments[tagName];
      }
      i++;
    }
  }

  tagNameList.sort();
   // tagNameList is sorted above to sort the completions by name alphabetically
   // Now we want to sort the completions by their types.
   // We only want to do this if we are completing Script DTDs
   // We are going to use a couple of iterators to sort the list by Type
   // Type Sorting is as follows: 0:Other, 1:Variables, 2: Functions (script)
  QValueList<KTextEditor::CompletionEntry>::Iterator otherIt=completions->begin();
  QValueList<KTextEditor::CompletionEntry>::Iterator variableIt=completions->begin();
  for (uint i = 0; i < tagNameList.count(); i++)
  {
    if (completionDTD->family == Xml)
      completion.text = QuantaCommon::tagCase(tagNameList[i]);
    else
      completion.text = tagNameList[i];
    completion.text = completion.text.left(completion.text.length() - 10).stripWhiteSpace();
    completion.comment = comments[tagNameList[i]];

    if(completionDTD->family==Script)
    {
      // Here we actually append the completion type
      completion.type = type[tagNameList[i]];
      // And here is out sorting...
      if(completion.type.contains("variable"))
      {
        // Insert after the last variable
        variableIt++;
        variableIt = completions->insert(variableIt, completion);
      }
      else
      {
        if(completion.type.contains("script"))
        {
          //Scripts can go at the end of the list
          completions->append(completion);
        }
        else
        {
          // Other types go first, after the last other type
          otherIt++;
          otherIt = completions->insert(otherIt, completion);
          // If we have no variables in the list, we need to point variableIt to otherIt, so they will go after the 'others'
          if((*variableIt).text.length()==0)
            variableIt=otherIt;
        }
      }
    }
    else
      completions->append( completion );
  }

//  completionInProgress = true;

  return completions;
}

/** Return a list of valid attributes for the given tag */
QValueList<KTextEditor::CompletionEntry>* Document::getAttributeCompletions(const QString& tagName, const QString& a_startsWith )
{
  QValueList<KTextEditor::CompletionEntry> *completions = new QValueList<KTextEditor::CompletionEntry>();
  KTextEditor::CompletionEntry completion;
  QTag *tag = QuantaCommon::tagFromDTD(completionDTD, tagName);
  if (!tag)
  {
    tag = userTagList.find(tagName.lower());
  }
  QString startsWith = a_startsWith.upper();
  if (tag)
  {
    switch (completionDTD->family)
    {
       case Xml:
            {
              completion.type = "attribute";
              completion.userdata = startsWith+"|"+tag->name();

              //list specified attributes for this tag
              AttributeList *list = tag->attributes();
              QValueList<KTextEditor::CompletionEntry> tempCompletions;
              QStringList nameList;
              for (uint i = 0; i < list->count(); i++)
              {
                QString item = list->at(i)->name;
                if (item.upper().startsWith(startsWith))
                {
                  completion.text = QuantaCommon::attrCase(item);
                  completion.comment = list->at(i)->type;
                  tempCompletions.append( completion );
                  nameList.append(completion.text);
                }
              }

              //list common attributes for this tag
              for (QStringList::Iterator it = tag->commonGroups.begin(); it != tag->commonGroups.end(); ++it)
              {
                AttributeList *attrs = tag->parentDTD->commonAttrs->find(*it);
                for (uint j = 0; j < attrs->count(); j++)
                {
                  QString name = attrs->at(j)->name;
                  if (name.upper().startsWith(startsWith))
                  {
                    completion.text = QuantaCommon::attrCase(name);
                    completion.comment = attrs->at(j)->type;
                    tempCompletions.append( completion );
                    nameList.append(completion.text);
                  }
                }
              }

              if (tag->name().contains("!doctype",false)) //special case, list all the known document types
              {
                QStringList nickNames = DTDs::ref()->nickNameList(true);
                for ( QStringList::Iterator it = nickNames.begin(); it != nickNames.end(); ++it )
                {
                  completion.type = "doctypeList";
                  completion.text = *it;
                  tempCompletions.append(completion);
                  nameList.append(completion.text);
                }
              }
              //below isn't fast, but enough here. May be better with QMap<QString, KTextEditor::CompletionEntry>
              nameList.sort();
              for ( QStringList::Iterator it = nameList.begin(); it != nameList.end(); ++it )
              {
                 for (QValueList<KTextEditor::CompletionEntry>::Iterator compIt = tempCompletions.begin(); compIt != tempCompletions.end(); ++compIt)
                 {
                   if ( (*compIt).text == *it)
                   {
                     completions->append(*compIt);
                     break;
                   }
                 }
              }
              break;
            }
       case Script:
            {
              completion.userdata = startsWith+"|"+tag->name();
              completion.type = "script";
              AttributeList *list = tag->attributes();
              for (uint i = 0; i < list->count(); i++)
              {
                QString item = list->at(i)->name;
                completion.text = item;
                completion.comment = list->at(i)->type;
                completions->append( completion );
              }
            }
      }
    } // if (tag)

//  completionInProgress = true;
  return completions;
}

/** Return a list of valid attribute values for the given tag and attribute */
QValueList<KTextEditor::CompletionEntry>* Document::getAttributeValueCompletions(const QString& tagName, const QString& attribute, const QString& startsWith )
{
  QValueList<KTextEditor::CompletionEntry> *completions = new QValueList<KTextEditor::CompletionEntry>();

  KTextEditor::CompletionEntry completion;
  completion.type = "attributeValue";
  completion.userdata = startsWith+"|"+tagName + "," + attribute;

  bool deleteValues;
  QStringList *values = tagAttributeValues(completionDTD->name,tagName, attribute, deleteValues);
  if (attribute.lower() == "class")
  {
    if (!values)
    {
      values = new QStringList(quantaApp->selectors(tagName));
      deleteValues = true;
    }
  } else
  if (attribute.lower() == "id")
  {
    if (!values)
    {
      values = new QStringList(quantaApp->idSelectors());
      deleteValues = true;
    }
  }
  if (values)
  {
    for ( QStringList::Iterator it = values->begin(); it != values->end(); ++it )
    {
      completion.text = *it;
      if (completion.text.startsWith(startsWith))
      {
        completions->append( completion );
      }
    }
  }
  if (deleteValues)
    delete values;
  int andSignPos = startsWith.find('&');
  if (andSignPos != -1)
  {
    QValueList<KTextEditor::CompletionEntry> *charCompletions = getCharacterCompletions(startsWith.mid(andSignPos + 1));
    *completions += *charCompletions;
    delete charCompletions;
  }

//  completionInProgress = true;
  return completions;
}

/** Return a list of character completions (like &nbsp; ...) */
QValueList<KTextEditor::CompletionEntry>* Document::getCharacterCompletions(const QString& startsWith)
{
  QValueList<KTextEditor::CompletionEntry> *completions = 0L;
  QMap<QString, KTextEditor::CompletionEntry> completionMap;

  //first search for entities defined in the document
  const DTDStruct *dtdDTD = DTDs::ref()->find("dtd");
  if (dtdDTD)
  {
    StructTreeGroup group;
    for (uint j = 0; j < dtdDTD->structTreeGroups.count(); j++)
    {
      group = dtdDTD->structTreeGroups[j];
      if (!group.autoCompleteAfterRx.pattern().isEmpty() &&
          group.autoCompleteAfterRx.search("&") != -1)
      {
        uint line, col;
        viewCursorIf->cursorPositionReal(&line, &col);
        Node *node = parser->nodeAt(line, col, false);
        completions = getGroupCompletions(node, group, line, col);
        for (uint i = 0; i < completions->count(); i++)
        {
          (*completions)[i].type = "charCompletion";
          (*completions)[i].userdata = (*completions)[i].text;
          completionMap[(*completions)[i].text] = (*completions)[i];
        }
        break;
      }
    }
  }

  if (!completions)
    completions = new QValueList<KTextEditor::CompletionEntry>();

  KTextEditor::CompletionEntry completion;
  completion.type = "charCompletion";
  //add the entities from the tag files
  QDictIterator<QTag> it(*(completionDTD->tagsList));
  for( ; it.current(); ++it )
  {
    QTag *tag = it.current();
    if (tag->type == "entity")
    {
      QString tagName = tag->name(true);
      if (tagName.upper().startsWith(startsWith.upper()) || startsWith.isEmpty())
      {
        completion.text = tagName;
        completion.userdata = tagName;
        completions->append( completion );
        completionMap[tagName] = completion;
      }
    }
  }

  QValueList<KTextEditor::CompletionEntry> *completions2 = new QValueList<KTextEditor::CompletionEntry>();
  for (QMap<QString, KTextEditor::CompletionEntry>::ConstIterator it = completionMap.constBegin(); it != completionMap.constEnd(); ++it)
  {
    completions2->append(it.data());
  }
  delete completions;
  completions = completions2;

  for ( QStringList::Iterator it = charList.begin(); it != charList.end(); ++it )
  {
    completion.text = *it;
    int begin = completion.text.find("(&") + 2;
    if (begin == 1)
        continue;
    int length = completion.text.find(";)") - begin + 1;
    QString s = completion.text.mid(begin, length - 1);
    completion.text = s + " : " + completion.text.left(begin -2) + " - " + completion.text.mid(begin + length + 1);
    if (s.startsWith(startsWith))
    {
      completion.userdata = s.mid(startsWith.length());
      completions->append( completion );
    }
  }

  return completions;
}

/** Returns the DTD identifier for the document */
QString Document::getDTDIdentifier()
{
  return dtdName;
}

/** Sets the DTD identifier */
void Document::setDTDIdentifier(const QString &id)
{
  dtdName = id.lower();
  m_groupsForDTEPs.clear();
}

/** Get a pointer to the current active DTD. If fallback is true, this always gives back a valid and known DTD pointer: the active, the document specified and in last case the application default document type. */
const DTDStruct* Document::currentDTD(bool fallback)
{
  uint line, col;
  viewCursorIf->cursorPositionReal(&line, &col);

  const DTDStruct *dtd = parser->currentDTD(line, col);

  if (fallback && !dtd) return defaultDTD();

  return dtd;
}

/** Get a pointer to the default DTD (document, or app). */
const DTDStruct* Document::defaultDTD() const
{
  const DTDStruct* dtd = DTDs::ref()->find(dtdName);
  if (!dtd) dtd = DTDs::ref()->find(Project::ref()->defaultDTD());
  if (!dtd) dtd = DTDs::ref()->find(qConfig.defaultDocType); //this will always exists

  return dtd;
}

/** Find the DTD name for a part of the document. */
QString Document::findDTDName(Tag **tag)
{
 //Do some magic to find the document type
 int endLine = editIf->numLines();
 QString foundText = "";
 int pos = 0;
 int i = 0;
 int line, startPos;
 QString text;
 do
 {
   text = editIf->textLine(i);
   //search for !DOCTYPE tags
   pos = text.find("!doctype",0,false);
   if (pos != -1) //parse the found !DOCTYPE tag
   {
     int bl, bc, el, ec;
     line = i;
     bl = line;
     startPos = text.findRev('<',pos);
     while (startPos == -1 && line >=0)
     {
       text = editIf->textLine(line);
       startPos = text.findRev('<');
       bl = line;
       line--;
     }
     if (startPos == -1)
     {
        i++;
        continue;
     }
     bc = startPos;
     line = i;
     text = editIf->textLine(i);
     startPos = text.find('>',pos);
     el = line;
     while (startPos == -1 && line < endLine)
     {
       text = editIf->textLine(line);
       startPos = text.find('>');
       el = line;
       line++;
     }
     if (startPos == -1)
     {
        i++;
        continue;
     }
     ec = startPos + 1;
     *tag = new Tag();
     (*tag)->setTagPosition(bl, bc, el, ec);
     text = this->text(bl, bc, el, ec);
     (*tag)->parse(text, this);
     (*tag)->type = Tag::XmlTag;
     text.replace("\\\"", "\"");
     pos = text.find("public",0,false);
     if (pos == -1) //if no PUBLIC info, use the word after !DOCTYPE as the doc.type
     {
       foundText = (*tag)->attribute(0);
     } else
     {             //use the quoted string after PUBLIC as doc. type
       pos = text.find("\"", pos+1);
       if (pos !=-1)
       {
         int endPos = text.find("\"",pos+1);
         foundText = text.mid(pos+1, endPos-pos-1);
       }
     }
     break;
   }
   i++;
 } while (i < endLine);

 return foundText.lower();
}

/** Called whenever a user inputs text in a script type document. */
bool Document::scriptAutoCompletion(int line, int column, const QString& insertedString)
{
 bool handled = false;
 Node *node = parser->nodeAt(line, column);
 if (!node) //happens in some cases in CSS
   return false;
 if (node->tag->type == Tag::Comment)
   return true; //nothing to do
 const DTDStruct *dtd = node->tag->dtd();
 if (node->prev)
   node = node->prev;
 else
 if (node->parent)
   node = node->parent;

 int bl, bc;
 node->tag->beginPos(bl, bc);
 QString s = text(bl, bc, line, column);
 if (QuantaCommon::insideCommentsOrQuotes(s.length() -1, s, dtd))
   return true; //again, nothing to do
 QString s2 = s;
 int i = s.length() - 1;
 while (i > 0 && s[i].isSpace())
   i--;
 while (i > 0 && (s[i].isLetterOrNumber() || s[i] == '_' ||
       (completionDTD->minusAllowedInWord && s[i] == '-') ) )
   i--;
 QString startStr = s.mid(i + 1).stripWhiteSpace();
 s = s.left(i + 1);
 if (s[i] == completionDTD->attributeSeparator)
 {
  while (i > 0 && s[i] != completionDTD->attrAutoCompleteAfter)
    i--;
  s = s.left(i + 1);
 } else
 if (s[i] == completionDTD->tagSeparator)
 {
  while (i > 0 && s[i] != completionDTD->tagAutoCompleteAfter)
    i--;
  s = s.left(i + 1);
 }

 if ( s[i] == completionDTD->attrAutoCompleteAfter ||
      s[i] == completionDTD->attributeSeparator ) //if we need to list the arguments of a function
 {
   QString textLine = s.left(i);
   QString word = findWordRev(textLine, completionDTD);
   QValueList<QTag *> tags;
   if (!word.isEmpty())
   {
      tags.append(userTagList.find(word.lower()));
      QDictIterator<QTag> it(*(completionDTD->tagsList));
      for( ; it.current(); ++it )
      {
        if (it.currentKey() == word)
          tags.append(it.current());
      }
   }
   QStringList argList;
   for (QValueList<QTag*>::ConstIterator it = tags.constBegin(); it != tags.constEnd(); ++it)
   {
     QTag *tag = *it;
     if (!tag)
       continue;
     QString arguments;
     if (tag->type != "property")
     {
      for (int i =0; i < tag->attributeCount(); i++)
      {
        Attribute* attr = tag->attributeAt(i);
        if (attr->status == "optional")
        {
          arguments = arguments + "["+attr->type +" "+attr->name +"], ";
        } else
        {
          arguments = arguments + attr->type +" "+attr->name +", ";
        }
      }
      arguments = tag->returnType +" "+tag->name() + "("+arguments.left(arguments.length()-2)+")";
      argList.append(arguments);
      codeCompletionIf->showArgHint(argList, "()" , completionDTD->attributeSeparator);
      argHintVisible = true;
     } else
     {
       if (hintRequested)
       {
         arguments = tag->name() + ": " + tag->attributeAt(0)->name + ";";
         argList.append(arguments);
         codeCompletionIf->showArgHint(argList, ":;" , completionDTD->attributeSeparator);
       } else
         showCodeCompletions( getAttributeValueCompletions(tag->name(), tag->attributeAt(0)->name, startStr));
     }

     handled = true;
   }
 } else
 {
   StructTreeGroup group;
   for (uint j = 0; j < completionDTD->structTreeGroups.count(); j++)
   {
     group = completionDTD->structTreeGroups[j];
     if (!group.autoCompleteAfterRx.pattern().isEmpty() &&
         ( group.autoCompleteAfterRx.search(s2) != -1||
           group.autoCompleteAfterRx.search(s) != -1) )
     {
       Node *node = parser->nodeAt(line, column, false);
       showCodeCompletions(getGroupCompletions(node, group, line, column + 1));
       handled = true;
       break;
     }
   }
 }
 if ( !handled && !argHintVisible &&
      (completionRequested ||
       (s[i] == completionDTD->tagAutoCompleteAfter && (insertedString == " " || (insertedString[0] == completionDTD->tagAutoCompleteAfter && !completionDTD->requestSpaceBeforeTagAutoCompletion))) ||
       completionDTD->tagAutoCompleteAfter == '\1' || (!completionDTD->memberAutoCompleteAfter.pattern().isEmpty() && completionDTD->memberAutoCompleteAfter.searchRev(s) != -1))
       )
 {
   showCodeCompletions(getTagCompletions(line, column + 1));
   handled = true;
 }
 return handled;
}

/** Retrives the text from the specified rectangle. The KTextEditor::EditInterface::text seems to not
work correctly. */
QString Document::text(int bLine, int bCol, int eLine, int eCol) const
{
 if (bLine > eLine)
 {
   int tmp = bLine;
   bLine = eLine;
   eLine = tmp;
   tmp = bCol;
   bCol = eCol;
   eCol = tmp;
 }
 QString t = editIf->textLine(bLine);
 if (bLine == eLine)
 {
   return t.mid(bCol, eCol-bCol +1);
 }
 t.remove(0, bCol);
 t.append("\n");
//TODO: This is slow if the area is big. We need to speed it up!!
 for (int i = bLine+1; i < eLine ; i++)
 {
   t.append(editIf->textLine(i)+"\n");
 }
 t = t+editIf->textLine(eLine).left(eCol+1);
 return t;
}

//TODO: profile which one is used more often and time critical places and use
//that one as the default and call from that one the other version
QString Document::text(const AreaStruct &area) const
{
  return text(area.bLine, area.bCol, area.eLine, area.eCol);
}

QString Document::find(const QRegExp& regExp, int sLine, int sCol, int& fbLine, int&fbCol, int &feLine, int&feCol)
{

 QRegExp rx = regExp;
 QString foundText = "";
 int maxLine = editIf->numLines();
 QString textToSearch = text(sLine, sCol, sLine, editIf->lineLength(sLine));
 int pos;
 int line = sLine;
 do
 {
   pos = rx.search(textToSearch);
   if (pos == -1)
   {
/*     if (line + STEP < maxLine)
     {
       line += STEP;
       textToSearch.append("\n"+text(line - STEP + 1, 0, line, editIf->lineLength(line)));
     } else*/
     {
      line ++;
      if (line < maxLine) textToSearch.append("\n"+editIf->textLine(line));
     }
   }
 } while (line < maxLine && pos == -1);
// pos = rx.search(text(sLine, sCol, maxLine -1, 100));
 if (pos != -1)
 {
   foundText = rx.cap();
   QString s = textToSearch.left(pos);
   int linesUntilFound = s.contains("\n");
   fbLine = sLine + linesUntilFound;
   fbCol = s.length()-s.findRev("\n")-1;
   int linesInFound = foundText.contains("\n");
   feCol = foundText.length()-foundText.findRev("\n")-2;
   feLine = fbLine + linesInFound;
   if (linesUntilFound == 0)
   {
    fbCol = fbCol + sCol;
   }
   if (linesInFound == 0)
   {
       feCol = feCol + fbCol;
   }
   if (fbCol < 0) fbCol = 0;
   if (feCol < 0) feCol = 0;
/*
   s = text(fbLine, fbCol, feLine, feCol);
   if (s != foundText) //debug, error
   {
     KMessageBox::error(this,"Found: "+foundText+"\nRead: "+s);
   }
*/
 }

 return foundText;
}

QString Document::findRev(const QRegExp& regExp, int sLine, int sCol, int& fbLine, int&fbCol, int &feLine, int&feCol)
{
 QRegExp rx = regExp;
 QString foundText = "";
 int pos = -1;
 int line = sLine;
 QString textToSearch = text(sLine, 0, sLine, sCol);
 do
 {
   pos = rx.searchRev(textToSearch);
   if (pos == -1)
   {
/*     if (line - STEP >= 0)
     {
         textToSearch.prepend(text(line - STEP, 0, line - 1, editIf->lineLength(line-1)) + "\n");
         line -= STEP;
     } else */
     {
       line--;
       if (line >=0) textToSearch.prepend(editIf->textLine(line) + "\n");
     }
   }
 } while (line >=0 && pos == -1);
 if (pos != -1)
 {
   foundText = rx.cap();
   fbLine = line;
   fbCol = pos;
   int linesInFound = foundText.contains("\n");
   feCol = foundText.length()-foundText.findRev("\n")-2;
   feLine = fbLine + linesInFound;
   if (linesInFound == 0)
   {
       feCol = feCol + fbCol;
   }
   if (fbCol < 0) fbCol = 0;
   if (feCol < 0) feCol = 0;
/*
   QString s = text(fbLine, fbCol, feLine, feCol);
   if (s != foundText) //debug, error
   {
     KMessageBox::error(this,"FindRev\nFound: "+foundText+"\nRead: "+s);
   }
*/
 }

 return foundText;
}

/** Code completion was requested by the user. */
void Document::codeCompletionRequested()
{
  completionRequested = true;
  completionInProgress = false;
  argHintVisible = false;
  hintRequested = false;
  handleCodeCompletion();
  completionRequested = false;
}

void Document::handleCodeCompletion()
{
  slotDelayedTextChanged(true);
  bool handled = false;
  uint line, col;
  viewCursorIf->cursorPositionReal(&line, &col);
  completionDTD = currentDTD();
  if (completionDTD->family == Xml)
  {
    handled = xmlCodeCompletion(line, col);
  }
  if (completionDTD->family == Script)
  {
    if (completionDTD->tagAutoCompleteAfter == '\0')
        completionDTD->tagAutoCompleteAfter = '\1';
    handled = scriptAutoCompletion(line, col - 1, "");
    if (completionDTD->tagAutoCompleteAfter == '\1')
        completionDTD->tagAutoCompleteAfter = '\0';
/*    if (!handled)
    {
      completionDTD = defaultDTD();
      QString s = text(line, 0, line, col).stripWhiteSpace();
      if (s.findRev("<") != -1)
      {
        //showCodeCompletions(getTagCompletions(line, col + 1));

        handled = true;
      }
    }*/
  }
  if (!handled)
  {
    completionDTD = defaultDTD();
    if (completionDTD->family == Xml)
    {
    //  xmlCodeCompletion(line, col);
      xmlAutoCompletion(line, col, " ");
    }
  }

  completionInProgress = true;
}

/** Bring up the code completion tooltip. */
void Document::codeCompletionHintRequested()
{
  completionRequested = true;
  slotDelayedTextChanged(true);
  uint line, col;
  viewCursorIf->cursorPositionReal(&line, &col);
  completionDTD = currentDTD();
  if (completionDTD->family == Script)
  {
//    QString textLine = editIf->textLine(line).left(col);
//    int pos = textLine.findRev("(");
//    int pos2 = textLine.findRev(")");
    //if (pos > pos2 )
    hintRequested = true;
    scriptAutoCompletion(line, col - 1, "");
  }
  completionRequested = false;
}

QString Document::currentWord()
{
  uint line, col;
  viewCursorIf->cursorPositionReal(&line, &col);
  QString textLine = editIf->textLine(line);
  int startPos = textLine.findRev(QRegExp("\\W"), col);
  int endPos = textLine.find(QRegExp("\\W"), col);
  if (startPos == -1)
     startPos = 0;
  else
      startPos++;
  if (endPos == -1)
     endPos = textLine.length();
  return textLine.mid(startPos, endPos - startPos);
}

/** Find the word until the first word boundary backwards */
QString Document::findWordRev(const QString& textToSearch, const DTDStruct *dtd)
{
  QString t = textToSearch;
  while (t.endsWith(" "))
    t = t.left(t.length()-1);
  int startPos = -1;
  int pos;
  bool end = false;
  do{
    pos = t.findRev(QRegExp("\\W"), startPos);
    if (t[pos] == '_' ||
        (dtd && dtd->minusAllowedInWord && t[pos] == '-'))
    {
      startPos = pos - t.length()-1;
      end = false;
    } else
    {
      end = true;
    }
  } while (!end);
  return t.remove(0,pos+1);
}


/** Invoke code completion dialog for XML like tags according to the position (line, col), using DTD dtd. */
bool Document::xmlCodeCompletion(int line, int col)
{
  bool handled = false;
  Node *node = parser->nodeAt(line, col);
  if (node && node->tag && node->tag->type == Tag::XmlTag )
  {
    Tag *tag = node->tag;
    int bLine, bCol;
    tag->beginPos(bLine, bCol);
    QString s;
    int index;
    QString tagName = tag->name.section('|', 0, 0).stripWhiteSpace();
    int nameCol = bCol + tagName.length() + 1;
    if (!tag->nameSpace.isEmpty())
      nameCol += 1 + tag->nameSpace.length();
    if (col > bCol && col <= nameCol) //we are inside a tag name, so show the possible tags
    {
     showCodeCompletions( getTagCompletions(line, col) );
     handled = true;
    } else
    {
      index = tag->valueIndexAtPos(line,col);
      if (index != -1)      //inside a value
      {
        s = tag->attribute(index);
        if (s == "style" && completionDTD->insideDTDs.contains("css"))
        {
          completionDTD = DTDs::ref()->find("text/css");
          return scriptAutoCompletion(line, col, "");
        } else
        {
          tag->attributeValuePos(index, bLine, bCol);
          s = tag->attributeValue(index).left(col - bCol);
          showCodeCompletions( getAttributeValueCompletions(tagName, tag->attribute(index), s) );
          handled = true;
        }
      } else
      {
        index = tag->attributeIndexAtPos(line,col);
        s = text(line,col,line,col);
        if (index != -1 || s ==" " || s==">" || s == "/") //inside an attribute or between attributes
        {
          if (index !=-1)
          {
           tag->attributeNamePos(index, bLine, bCol);
           s = tag->attribute(index).left(col - bCol);
          } else
          {
            s = text(line, 0, line, col -1);
            s = s.section(' ', -1);
          }
          showCodeCompletions( getAttributeCompletions(tagName, s) );
          handled = true;
        }
      }
    }
  }
  if (!handled)
  {
    QString s = editIf->textLine(line).left(col);
    int pos = s.findRev('&');
    if (pos != -1)
    {
      s = s.mid(pos + 1);
      if (!s.stripWhiteSpace().isEmpty())
      {
        //complete character codes
        showCodeCompletions(getCharacterCompletions(s));
        handled = true;
      }
    }
  }
  return handled;
}

void Document::slotCompletionAborted()
{
  completionInProgress = false;
  argHintVisible = false;
}

/** Ask for user confirmation if the file was changed outside. */
void Document::checkDirtyStatus()
{
  QString fileName;
  if (url().isLocalFile())
      fileName = url().path();
  if (m_dirty)
  {
    createTempFile();
    if (!fileName.isEmpty())
    {
      QDateTime modifTime = QFileInfo(fileName).lastModified();
      if (modifTime == m_modifTime)
        m_dirty = false;
    }
    if (m_dirty)
    {
      if (m_md5sum.isEmpty())
      {
        QFile f(fileName);
        if (f.open(IO_ReadOnly))
        {
          const char* c = "";
          KMD5 context(c);
          context.reset();
          context.update(f);
          m_md5sum = context.hexDigest();
          f.close();
        }
        m_dirty = false;
      } else
      {
        //check if the file is changed, also by file content. Might help to reduce
        //unwanted warning on NFS
        QFile f(fileName);
        if (f.open(IO_ReadOnly))
        {
          QString md5sum;
          const char* c = "";
          KMD5 context(c);
          context.reset();
          context.update(f);
          md5sum = context.hexDigest();
          kdDebug(24000) << "MD5 sum of current doc: " << m_md5sum << endl;
          kdDebug(24000) << "MD5 sum of doc on disc : " << md5sum << endl;
          if (md5sum == m_md5sum)
          {
            m_dirty = false;
          }
          f.close();
        }
      }
    }
    if (m_dirty)
    {
      DirtyDlg *dlg = new DirtyDlg(url().path(), m_tempFileName, false, this);
      DirtyDialog *w = static_cast<DirtyDialog*>(dlg->mainWidget());
      QString kompareStr = KStandardDirs::findExe("kompare");
      if (kompareStr.isEmpty())
      {
        w->buttonCompare->setEnabled(false);
        w->buttonLoad->setChecked(true);
      }
      if (dlg->exec())
      {
          m_doc->setModified(false);
          openURL(url());
      }
      m_modifTime = QFileInfo(fileName).lastModified();
      delete dlg;
    }
    closeTempFile();
    m_dirty = false;
  }
}

/** Save the document and reset the dirty status. */
void Document::save()
{
  if (url().isLocalFile())
  {
    QString fileName;
    fileName = url().path();
    fileWatcher->removeFile(fileName);
//    kdDebug(24000) << "removeFile[save]: " << fileName << endl;
    m_doc->save();
    m_dirty = false;
    m_modifTime = QFileInfo(fileName).lastModified();
    fileWatcher->addFile(fileName);
//    kdDebug(24000) << "addFile[save]: " << fileName << endl;
  } else
  {
    m_doc->save();
    m_dirty = false;
  }
//  kdDebug(24000) << "Document " << url() << " saved." << endl;
}

bool Document::saveAs(const KURL& url)
{
  bool result = m_doc->saveAs(url);
  if (result)
  {
    m_md5sum = "";
    if (url.isLocalFile())
    {
      QFile f(url.path());
      if (f.open(IO_ReadOnly))
      {
        const char* c = "";
        KMD5 context(c);
        context.reset();
        context.update(f);
        m_md5sum = context.hexDigest();
        f.close();
      }
    }
  }
  return result;
}

void Document::enableGroupsForDTEP(const QString& dtepName, bool enable)
{
  if (m_groupsForDTEPs.isEmpty())
    m_groupsForDTEPs = m_DTEPList;
  if (enable)
  {
    if (m_groupsForDTEPs.contains(dtepName) == 0)
      m_groupsForDTEPs.append(dtepName);
  } else
  {
    m_groupsForDTEPs.remove(dtepName);
  }
}

void Document::resetGroupsForDTEPList()
{
  m_groupsForDTEPs.clear();
}

/** Returns true if the number of " (excluding \") inside text is even. */
bool Document::evenQuotes(const QString &text)
{
 int num = text.contains(QRegExp("[^\\\\]\""));

 return (num /2 *2 == num);
}

void Document::slotTextChanged()
{
  changed = true;
  parser->setSAParserEnabled(false); //disable special area parsing if the text was changed.
  if (reparseEnabled && delayedTextChangedEnabled)
  {
    kdDebug(24000) << "Delayed text changed called." << endl;
    //delay the handling, otherwise we may get wrong values for (line,column)
    QTimer::singleShot(0, this, SLOT(slotDelayedTextChanged()));
    delayedTextChangedEnabled = false;
  }
}

void Document::slotDelayedTextChanged(bool forced)
{
   if (!forced && typingInProgress)
   {
     kdDebug(24000) << "Reparsing delayed!" << endl;
     parser->setParsingNeeded(true);
     QTimer::singleShot(1000, this, SLOT(slotDelayedTextChanged()));
     reparseEnabled = false;
     delayedTextChangedEnabled = false;
     return;
   }

    uint line, column;
    QString oldNodeName = "";
    Node *node;
    Node *currentNode = 0L; //holds a copy of the node which is at (line,column)
    Node *previousNode = 0L;//holds a copy of the node before currentNode
    if (qConfig.updateClosingTags)
    {
      viewCursorIf->cursorPositionReal(&line, &column);
      node = parser->nodeAt(line, column, false);
      if (node &&
          ((node->tag->type == Tag::XmlTag && !node->tag->single) ||
            node->tag->type == Tag::XmlTagEnd)
           )
      {
        Tag *tag;
        tag = new Tag(*node->tag);
        currentNode = new Node(0L);
        currentNode->removeAll = false;
        currentNode->tag = tag;

        node = node->previousSibling();
        if (node)
        {
          tag = new Tag(*node->tag);
          previousNode = new Node(0L);
          previousNode->removeAll = false;
          previousNode->tag = tag;
        }
      }
    }
    parser->setSAParserEnabled(true); //enable special area parsing, it was disabled in slotTextChanged()
    baseNode = parser->rebuild(this);
    if (qConfig.updateClosingTags && currentNode)
    {
      viewCursorIf->cursorPositionReal(&line, &column);
      node = parser->nodeAt(line, column, false);
      if (node &&
          node->tag->nameSpace + node->tag->name != currentNode->tag->nameSpace + currentNode->tag->name &&
          ((node->tag->type == Tag::XmlTag && !node->tag->single) || node->tag->type == Tag::XmlTagEnd) && node->tag->validXMLTag)
      {
        int bl, bc, bl2, bc2;
        node->tag->beginPos(bl, bc);
        currentNode->tag->beginPos(bl2,bc2);
        if ( (bl != bl2 || bc !=bc2) && previousNode)
        {
          previousNode->tag->beginPos(bl2, bc2);
          Node::deleteNode(currentNode);
          currentNode = previousNode;
          previousNode = 0L;
        } else
        {
          Node::deleteNode(previousNode);
          previousNode = 0L;
        }
        if (bl == bl2 && bc == bc2 &&
           ((node->tag->type == Tag::XmlTag && !node->tag->single) || currentNode->tag->type == Tag::XmlTagEnd))
        {
          QString newName = node->tag->name;
          bool updateClosing = (currentNode->tag->type == Tag::XmlTag) && !newName.startsWith("!");
          int num = 1;
          if (!node->tag->nameSpace.isEmpty())
            newName.prepend(node->tag->nameSpace + ":");
          if (updateClosing)
            node = node->nextSibling();
          else
            node = node->previousSibling();
          while (node)
          {
            if (node->tag->validXMLTag && ((node->tag->type == Tag::XmlTag && !node->tag->single) || node->tag->type == Tag::XmlTagEnd))
            {
              if (node->tag->nameSpace + node->tag->name == currentNode->tag->nameSpace + currentNode->tag->name )
              {
                num++;
              }
              if ( (updateClosing && QuantaCommon::closesTag(currentNode->tag, node->tag)) ||
                  (!updateClosing && QuantaCommon::closesTag(node->tag, currentNode->tag)) )
              {
                num--;
              }
              if (num == 0)
              {
                reparseEnabled = false;
                node->tag->beginPos(bl, bc);
                bc++;
                if(editIfExt)
                  editIfExt->editBegin();
                int len = node->tag->name.length();
                if (!node->tag->nameSpace.isEmpty())
                  len += 1 + node->tag->nameSpace.length();
                editIf->removeText(bl, bc, bl, bc + len);
                if (updateClosing)
                {
                  editIf->insertText(bl, bc, "/"+newName);
                } else
                {
                  editIf->insertText(bl, bc, newName.mid(1));
                  if (bl == (int)line)
                  {
                    column += (newName.length() - currentNode->tag->name.length());
                  }
                }
                if(editIfExt)
                  editIfExt->editEnd();
                viewCursorIf->setCursorPositionReal(bl, bc);
                docUndoRedo->mergeNextModifsSet();
                baseNode = parser->parse(this, true);
                viewCursorIf->setCursorPositionReal(line, column);
                reparseEnabled = true;
                break;
              }
            }
            if (updateClosing)
              node = node->nextSibling();
            else
              node = node->previousSibling();
          }
        }
      }
      Node::deleteNode(currentNode);
      Node::deleteNode(previousNode);
    }

    quantaApp->slotNewLineColumn();
    if (qConfig.instantUpdate && quantaApp->structTreeVisible())
    {
      typingInProgress = false;
      StructTreeView::ref()->slotReparse(this, baseNode , qConfig.expandLevel);
    }
    reparseEnabled = true;
   delayedTextChangedEnabled = true;
}

/** Returns list of values for attribute */
QStringList* Document::tagAttributeValues(const QString& dtdName, const QString& tag, const QString &attribute, bool &deleteResult)
{
  QStringList *values = 0L;
  deleteResult = true;
  const DTDStruct* dtd = DTDs::ref()->find(dtdName);
  if (dtd)
  {
    QString searchForAttr = (dtd->caseSensitive) ? attribute : attribute.upper();
    AttributeList* attrs = QuantaCommon::tagAttributes(dtdName, tag);
    if (attrs)
    {
      Attribute *attr;
      KURL u;
      KURL base = url();
      base.setPath(base.directory(false,false));
      QString s;
      for ( attr = attrs->first(); attr; attr = attrs->next() )
      {
        QString attrName = (dtd->caseSensitive) ? attr->name : attr->name.upper();
        if (attrName == searchForAttr)
        {
          if (attr->type == "url") {
            Project *project = Project::ref();
            if (project->hasProject())
            {
              values = new QStringList(project->fileNameList());
              for (uint i = 0; i < values->count(); i++)
              {
                u = (*values)[i];
                u = QExtFileInfo::toRelative(u, base);
                (*values)[i] = u.path();
              }
              values->remove(values->at(0));
              values->append("mailto:" + project->email());
            } else
            {
              QDir dir = QDir(url().directory());
              values = new QStringList(dir.entryList());
            }
            break;
          } else {
            values = &attr->values;
            deleteResult = false;
            break;
          }
        }
      }
    }
  }
  return values;
}

bool Document::hasChanged()
{
  bool b = changed;
  changed = false;
  return b;
}

void Document::setChanged(bool newStatus)
{
  changed = newStatus;
}

void Document::paste()
{
  reparseEnabled = false;
  dynamic_cast<KTextEditor::ClipboardInterface*>(view())->paste();
  reparseEnabled = true;
  baseNode = parser->rebuild(this);
}

/** returns all the areas that are between tag and it's closing pair */
QStringList Document::tagAreas(const QString& tag, bool includeCoordinates, bool skipFoundContent)
{
  Node *node = baseNode;
  int bl, bc, el, ec;
  QStringList result;

  while (node)
  {
    if (node->tag->type == Tag::XmlTag)
    {
      if ( (node->tag->dtd()->caseSensitive && node->tag->name == tag) ||
           (!node->tag->dtd()->caseSensitive && node->tag->name.lower() == tag.lower()) )
      {
        node->tag->beginPos(bl, bc);
        if (node->next)
            node->next->tag->endPos(el, ec);
        else
        {
          el = editIf->numLines()-1;
          ec = editIf->lineLength(el);
        }
        QString s = text(bl, bc, el, ec);
        if (includeCoordinates)
        {
          s.prepend(QString("%1,%2,%3,%4\n").arg(bl).arg(bc).arg(el).arg(ec));
        }
        result += s;
        if (skipFoundContent)
            node = node->next;
        else
            node = node->nextSibling();
      } else
        node = node->nextSibling();
    } else
        node = node->nextSibling();
  }

  return result;
}

void Document::activateRepaintView(bool activation)
{
  repaintEnabled = activation;
  m_view->setUpdatesEnabled(activation);
}

void Document::setErrorMark(int line)
{
  if (!markIf)
    return;
  markIf->addMark(line, KTextEditor::MarkInterface::markType07);
}

void Document::clearErrorMarks()
{
  if (!markIf)
    return;
  QPtrList<KTextEditor::Mark> marks = markIf->marks();
  KTextEditor::Mark* mark;
  for (mark = marks.first(); mark; mark = marks.next())
  {
    if (mark->type & KTextEditor::MarkInterface::markType07)
        markIf->removeMark(mark->line, KTextEditor::MarkInterface::markType07);
  }
}

QString Document::backupPathEntryValue()
{
  return m_backupPathValue;
}

void Document::setBackupPathEntryValue(const QString& ev)
{
  m_backupPathValue = ev;
}

/** if the document is modified then backup it and insert an entry in quantarc */
void Document::createBackup(KConfig* config)
{
  if (isModified())
  {
    if (isUntitled())
    {
      m_backupPathValue = qConfig.backupDirPath + untitledUrl + "." + hashFilePath("file:///" + untitledUrl) + "U";
    } else
    {
      m_backupPathValue = qConfig.backupDirPath + url().fileName() + "." + hashFilePath(url().url());
    }
    QString backupPathValueURL = KURL::fromPathOrURL(m_backupPathValue).url();

    //the encoding used for the current document
    QString encoding = quantaApp->defaultEncoding();
    if (encodingIf)
      encoding = encodingIf->encoding();
    if (encoding.isEmpty())
      encoding = "utf8";  //final fallback

    //creates an entry string in quantarc if it does not exist yet
    config->setGroup("General Options");
    QStringList backedupFilesEntryList = QuantaCommon::readPathListEntry(config, "List of backedup files"); //the files that were backedup
    QStringList autosavedFilesEntryList = QuantaCommon::readPathListEntry(config, "List of autosaved files"); //the list of actual backup files inside $KDEHOME/share/apps/quanta/backups
    if (!autosavedFilesEntryList.contains(backupPathValueURL)) //not yet backed up, add an entry for this file
    {
      autosavedFilesEntryList.append(backupPathValueURL);
      config->writePathEntry("List of autosaved files", autosavedFilesEntryList);
      if (!isUntitled())
        backedupFilesEntryList.append(KURL::fromPathOrURL(url().path() + "." + qConfig.quantaPID).url());
      else
        backedupFilesEntryList.append(url().url() + "." + qConfig.quantaPID);
      config->writePathEntry("List of backedup files", backedupFilesEntryList);
      config->sync();
    }

    //creates a copy of this specific document
    QFile file(m_backupPathValue);
    if (file.open(IO_WriteOnly))
    {
      QTextStream stream(&file);
      stream.setCodec(QTextCodec::codecForName(encoding));
      stream << editIf->text();
      file.close();
    }
  }
}
/** if there is no more need for a backup copy then remove it */
void Document::removeBackup(KConfig *config)
{
  QString backupPathValueURL = KURL::fromPathOrURL(m_backupPathValue).url();
  
  config->reparseConfiguration();
  config->setGroup("General Options");

  QStringList backedupFilesEntryList = QuantaCommon::readPathListEntry(config, "List of backedup files");
  QStringList autosavedFilesEntryList = QuantaCommon::readPathListEntry(config, "List of autosaved files");

  autosavedFilesEntryList.remove(backupPathValueURL);
  config->writePathEntry("List of autosaved files", autosavedFilesEntryList);
  backedupFilesEntryList.remove(KURL::fromPathOrURL(url().path() + "." + qConfig.quantaPID).url());
  config->writePathEntry("List of backedup files", backedupFilesEntryList);
  config->sync();

  if(QFile::exists(m_backupPathValue))
    QFile::remove(m_backupPathValue);
}
/** creates a string by hashing a bit the path string of this document */
QString Document::hashFilePath(const QString& p)
{
 switch(p.length())
 {
  case 1: {
           int c = int(p[0]);
           return QString::number(c, 10) + "P" + qConfig.quantaPID;
          }

  case 2: {
           int c = int(p[1]) * 2;
           return QString::number(c, 10) + "P" + qConfig.quantaPID;
          }

  default: {
             int sign = 1,
                 sum = 0;
             uint plen = p.length();
             for (uint i = 0; i+1 < plen; i++)
             {
               sum += int(p[i]) + int(p[i + 1]) * sign;
               sign *= -1;
             }
             if( sum >= 0 )
               return QString::number(sum, 10) + "P" + qConfig.quantaPID;
             else
               return QString::number(sum*(-1), 10) + "N" + qConfig.quantaPID;
            }
 }
}

void Document::convertCase()
{
  int tagCase = 0;
  int attrCase = 0;
  KDialogBase dlg(this, 0L, false, i18n("Change Tag & Attribute Case"), KDialogBase::Ok | KDialogBase::Cancel);
  CaseWidget w(&dlg);
  dlg.setMainWidget(&w);
  const DTDStruct *dtd = defaultDTD();
  switch (qConfig.attrCase)
  {
    case 1: {w.lowerAttr->setChecked(true); break;}
    case 2: {w.upperAttr->setChecked(true); break;}
    default:{w.unchangedAttr->setChecked(true); break;}
  }
  switch (qConfig.tagCase)
  {
    case 1: {w.lowerTag->setChecked(true); break;}
    case 2: {w.upperTag->setChecked(true); break;}
    default:{w.unchangedTag->setChecked(true); break;}
  }

  if (dlg.exec())
  {
    KProgressDialog progressDlg(this, 0, i18n("Working..."));
    progressDlg.setLabel(i18n("Changing tag and attribute case. This may take some time, depending on the document complexity."));
    progressDlg.setAllowCancel(false);
    progressDlg.show();
    kapp->eventLoop()->processEvents( QEventLoop::ExcludeUserInput | QEventLoop::ExcludeSocketNotifiers);
    KProgress *pBar = progressDlg.progressBar();
    pBar->setValue(0);
    pBar->setTotalSteps(nodeNum);
    pBar->setTextEnabled(true);
    if (w.lowerTag->isChecked())
        tagCase = 1;
    if (w.upperTag->isChecked())
        tagCase = 2;
    if (w.lowerAttr->isChecked())
        attrCase = 1;
    if (w.upperAttr->isChecked())
        attrCase = 2;
    if (tagCase == 0 && attrCase == 0)
        return;
    reparseEnabled = false;
    int bl, bc, ec;
    uint line, col;
    viewCursorIf->cursorPositionReal(&line, &col);
    Node *node = baseNode;
    while (node)
    {
      pBar->advance(1);
      if (node->tag->dtd() == dtd)
      {
        if (tagCase !=0)
        {
          if(editIfExt)
            editIfExt->editBegin();
          node->tag->namePos(bl, bc);
          ec = bc + node->tag->name.length();
          editIf->removeText(bl, bc, bl, ec);
          viewCursorIf->setCursorPositionReal(bl, bc);
          QString newName = node->tag->name;
          if (tagCase == 1)
            newName = newName.lower();
          else if (tagCase == 2)
            newName = newName.upper();
          editIf->insertText(bl, bc, newName);
          if(editIfExt)
            editIfExt->editEnd();
        }
        if (attrCase != 0)
        {
          QString newName;
          for (int i = 0; i < node->tag->attrCount(); i++)
          {
          if(editIfExt)
            editIfExt->editBegin();
            node->tag->attributeNamePos(i, bl, bc);
            newName = node->tag->attribute(i);
            ec = bc + newName.length();
            editIf->removeText(bl, bc, bl, ec);
            if (attrCase == 1)
              newName = newName.lower();
            else if (attrCase == 2)
              newName = newName.upper();
            editIf->insertText(bl, bc, newName);
          if(editIfExt)
            editIfExt->editEnd();
          }
        }
      }
      node = node->nextSibling();
    }
    reparseEnabled = true;
    viewCursorIf->setCursorPositionReal(line, col);
    quantaApp->reparse(true);
  }
}

void Document::open(const KURL &url, const QString &encoding)
{
    if (encodingIf)
    {
       encodingIf->setEncoding(encoding);
       m_encoding = encoding;
       m_codec = QTextCodec::codecForName(m_encoding);
    }
    connect(m_doc, SIGNAL(completed()), this, SLOT(slotOpeningCompleted()));
    connect(m_doc, SIGNAL(canceled(const QString&)), this, SLOT(slotOpeningFailed(const QString&)));
    if (!openURL(url))
      slotOpeningFailed(QString::null);
    if (!url.isLocalFile())
    {
      QExtFileInfo internalFileInfo;
      internalFileInfo.enter_loop();
    }
}

void Document::slotOpeningCompleted()
{
  KURL u = url();
  if (!u.isLocalFile())
  {
    m_modifTime = QDateTime();
    qApp->exit_loop();
  }
  else
  {
      fileWatcher->addFile(u.path());
      m_modifTime = QFileInfo(u.path()).lastModified();
//      kdDebug(24000) << "addFile[Document::open]: " << u.path() << endl;
  }
  disconnect(m_doc, SIGNAL(completed()), this, SLOT(slotOpeningCompleted()));
  disconnect(m_doc, SIGNAL(canceled(const QString&)), this, SLOT(slotOpeningFailed(const QString&)));
  m_dirty = false;
  m_view->setFocus();
  processDTD();
  emit openingCompleted(u);
}

void Document::slotOpeningFailed(const QString &errorMessage)
{
  m_md5sum = "";
  Q_UNUSED(errorMessage); //TODO: append the error message to our own error message
  if (!url().isLocalFile())
    qApp->exit_loop();
  disconnect(m_doc, SIGNAL(completed()), this, SLOT(slotOpeningCompleted()));
  disconnect(m_doc, SIGNAL(canceled(const QString&)), this, SLOT(slotOpeningFailed(const QString&)));
  emit openingFailed(url());
}

void Document::processDTD(const QString& documentType)
{
 QString foundName;
 QString projectDTD = Project::ref()->defaultDTD();
 setDTDIdentifier(projectDTD);
 Tag *tag = 0L;
 if (documentType.isEmpty())
 {
   foundName = findDTDName(&tag); //look up the whole file for DTD definition
   bool found = false;
   if (!foundName.isEmpty())   //!DOCTYPE found in file
   {
      KDialogBase dlg(this, 0L, true, i18n("DTD Selector"), KDialogBase::Ok | KDialogBase::Cancel);
      DTDSelectDialog *dtdWidget = new DTDSelectDialog(&dlg);
      dlg.setMainWidget(dtdWidget);
      QStringList lst = DTDs::ref()->nickNameList(true);
      QString foundNickName = DTDs::ref()->getDTDNickNameFromName(foundName);
      for (uint i = 0; i < lst.count(); i++)
      {
        dtdWidget->dtdCombo->insertItem(lst[i]);
        if (lst[i] == foundNickName)
        {
          setDTDIdentifier(foundName);
          found =true;
        }
      }

      if (!DTDs::ref()->find(foundName))
      {
        //try to find the closest matching DTD
        QString s = foundName.lower();
        uint spaceNum = s.contains(' ');
        QStringList dtdList = DTDs::ref()->nameList();
        QStringList lastDtdList;
        for (uint i = 0; i <= spaceNum && !dtdList.empty(); i++)
        {
          lastDtdList = dtdList;
          QStringList::Iterator strIt = dtdList.begin();
          while (strIt != dtdList.end())
          {
            if (!(*strIt).startsWith(s.section(' ', 0, i)))
            {
              strIt = dtdList.remove(strIt);
            } else
            {
              ++strIt;
            }
          }
        }
        dtdList = lastDtdList;
        for (uint i = 0; i <= spaceNum && !dtdList.empty(); i++)
        {
          lastDtdList = dtdList;
          QStringList::Iterator strIt = dtdList.begin();
          while (strIt != dtdList.end())
          {
            if (!(*strIt).endsWith(s.section(' ', -(i+1), -1)))
            {
              strIt = dtdList.remove(strIt);
            } else
            {
              ++strIt;
            }
          }
        }
        if (lastDtdList.count() == 1 || lastDtdList[0].startsWith(s.section(' ', 0, 0)))
        {
          projectDTD = lastDtdList[0];
        }
      }

//    dlg->dtdCombo->insertItem(i18n("Create New DTD Info"));
      dtdWidget->messageLabel->setText(i18n("This DTD is not known for Quanta. Choose a DTD or create a new one."));
      dtdWidget->currentDTD->setText(DTDs::ref()->getDTDNickNameFromName(foundName));
      QString projectDTDNickName = DTDs::ref()->getDTDNickNameFromName(projectDTD);
      for (int i = 0; i < dtdWidget->dtdCombo->count(); i++)
      {
        if (dtdWidget->dtdCombo->text(i) == projectDTDNickName)
        {
          dtdWidget->dtdCombo->setCurrentItem(i);
          break;
        }
      }
      if (!found && qConfig.showDTDSelectDialog)
      {
        quantaApp->slotHideSplash();
        if (dlg.exec())
        {
          qConfig.showDTDSelectDialog = !dtdWidget->useClosestMatching->isChecked();
          setDTDIdentifier(DTDs::ref()->getDTDNameFromNickName(dtdWidget->dtdCombo->currentText()));
          const DTDStruct *dtd = DTDs::ref()->find(dtdName);
          if (dtdWidget->convertDTD->isChecked() && dtd->family == Xml)
          {
            int bLine, bCol, eLine, eCol;
            tag->beginPos(bLine,bCol);
            tag->endPos(eLine,eCol);
            editIf->removeText(bLine, bCol, eLine, eCol+1);
            viewCursorIf->setCursorPositionReal((uint)bLine, (uint)bCol);
            insertText("<!DOCTYPE" + dtd->doctypeStr +">");
          }
        }
      }
   } else //DOCTYPE not found in file
   {
     KURL u = url();
     QString dtdId = DTDs::ref()->DTDforURL(u)->name;
//     if (dtdId == "empty")
     {
       const DTDStruct * dtd = DTDs::ref()->find(projectDTD);
       if (DTDs::canHandle(dtd, u))
        dtdId = projectDTD;
       else
       {
         dtd = DTDs::ref()->find(qConfig.defaultDocType);
         if (DTDs::canHandle(dtd, u))
           dtdId = qConfig.defaultDocType;
       }
     }
     setDTDIdentifier(dtdId);
   }
 } else //dtdName is read from the method's parameter
 {
   setDTDIdentifier(documentType);
 }

  if (!isUntitled())
  {
    quantaApp->messageOutput()->showMessage(i18n("\"%1\" is used for \"%2\".\n").arg(DTDs::ref()->getDTDNickNameFromName(dtdName)).arg(url().prettyURL(0, KURL::StripFileProtocol)));
  }
  quantaApp->slotLoadToolbarForDTD(dtdName);
  StructTreeView::ref()->useOpenLevelSetting = true;
  delete tag;
}


/** Called when a file on the disk has changed. */
void Document::slotFileDirty(const QString& fileName)
{
  if ( url().path() == fileName && !dirty() )
  {
    setDirtyStatus(true);
    if (this == ViewManager::ref()->activeDocument())
    {
      checkDirtyStatus();
    }
  }
}

void Document::slotMarkChanged(KTextEditor::Mark mark, KTextEditor::MarkInterfaceExtension::MarkChangeAction action)
{
  if(mark.type & KTextEditor::MarkInterface::markType02)
  {
    if(action == KTextEditor::MarkInterfaceExtension::MarkRemoved)
      emit breakpointUnmarked(this, mark.line);
    else
      emit breakpointMarked(this, mark.line);
  }
}

void Document::resetDTEPs()
{
  m_DTEPList.clear();
  m_DTEPList.append(defaultDTD()->name);
}

void Document::addDTEP(const QString &dtepName)
{
  if (m_DTEPList.contains(dtepName) == 0)
  {
    m_DTEPList.append(dtepName);
  }
}

QStringList Document::groupsForDTEPs()
{
  if (m_groupsForDTEPs.isEmpty())
    return m_DTEPList;
  else
    return m_groupsForDTEPs;
}

QString Document::annotationText(uint line)
{
  QMap<uint, QPair<QString, QString> >::Iterator it = m_annotations.find(line);
  if (it != m_annotations.end())
    return it.data().first;
  else
   return QString::null;
}

void Document::setAnnotationText(uint line, const QString& text)
{
  if (text.isEmpty())
  {
    m_annotations.remove(line);
    if (markIf)
      markIf->removeMark(line, KTextEditor::MarkInterface::markType08);
  } else
  {
    m_annotations.insert(line, qMakePair(text, QString("")));
    if (markIf)
      markIf->setMark(line, KTextEditor::MarkInterface::markType08);
    uint line, column;
    viewCursorIf->cursorPositionReal(&line, &column);
    viewCursorIf->setCursorPositionReal(line, 0);
    const DTDStruct *dtd = currentDTD(true);
    QString commentBegin = "";
    QString commentEnd = "";
    for (QMap<QString, QString>::ConstIterator it = dtd->comments.constBegin(); it != dtd->comments.constEnd(); ++it)
    {
      commentBegin = it.key();
      commentEnd = it.data();
      if (commentEnd != "\n")
        break;
    }
    if (commentBegin.isEmpty())
    {
      if (dtd->family == Xml)
      {
        commentBegin = "<!--";
        commentEnd = "-->";
      } else
      {
        commentBegin = "/*";
        commentEnd = "*/";
      }
    }
    QString s = "@annotation: " + text;
    s.prepend(commentBegin + " ");
    s.append(" " + commentEnd + "\n");
    insertText(s, true, true);
    emit showAnnotation(line, "", qMakePair(text, QString("")));
  }
}

void Document::addAnnotation(uint line, const QPair<QString, QString>& annotation)
{
  m_annotations.insert(line, annotation);
  if (markIf)
    markIf->setMark(line, KTextEditor::MarkInterface::markType08);
  emit showAnnotation(line, "", annotation);
}

void Document::clearAnnotations()
{
  if (markIf)
  {
    QPtrList<KTextEditor::Mark> m = markIf->marks();
    for (uint i=0; i < m.count(); i++)
      markIf->removeMark( m.at(i)->line, KTextEditor::MarkInterface::markType08 );
 }
 m_annotations.clear();
}

bool Document::openURL(const KURL& url)
{
  m_md5sum = "";
  if (url.isLocalFile())
  {
    QFile f(url.path());
    if (f.open(IO_ReadOnly))
    {
      const char* c = "";
      KMD5 context(c);
      context.reset();
      context.update(f);
      m_md5sum = context.hexDigest();
      f.close();
    }
  }
  return m_doc->openURL(url);
}

#include "document.moc"
