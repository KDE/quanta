/***************************************************************************
                          document.cpp  -  description
                             -------------------
    begin                : Tue Jun 6 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon <pdima@users.sourceforge.net,yshurik@penguinpowered.com,sequitur@easystreet.com>
                           (C) 2001-2003 Andras Mantia <amantia@kde.org>
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

//QT includes
#include <qdir.h>
#include <qeventloop.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qtextcodec.h>
#include <qtextstream.h>
#include <qregexp.h>
#include <qradiobutton.h>

// KDE includes
#include <kapplication.h>
#include <kwin.h>
#include <klocale.h>
#include <kaction.h>
#include <kdialogbase.h>
#include <kiconloader.h>
#include <kmessagebox.h>
#include <ktempfile.h>
#include <kdirwatch.h>
#include <kdebug.h>
#include <kprogress.h>
#include <kio/netaccess.h>

#include <ktexteditor/cursorinterface.h>
#include <ktexteditor/clipboardinterface.h>
#include <ktexteditor/configinterface.h>
#include <ktexteditor/encodinginterface.h>
#include <ktexteditor/wordwrapinterface.h>
#include <ktexteditor/markinterfaceextension.h>

#include "parser/tag.h"
#include "quantacommon.h"
#include "document.h"
#include "resource.h"
#include "dialogs/dirtydlg.h"
#include "dialogs/dirtydialog.h"
#include "dialogs/casewidget.h"
#include "project/project.h"
#include "plugins/quantaplugininterface.h"

#include "quanta.h"
#include "quantaview.h"
#include "treeviews/structtreeview.h"

#define STEP 1

Document::Document(const KURL& p_baseURL, KTextEditor::Document *doc,
                   Project *project, QuantaPluginInterface *a_pIf,
                   QWidget *parent, const char *name, WFlags f )
  : QWidget(parent, name, f)
{
  m_dirty   = false;
  busy    = false;
  changed = false;
  m_doc = doc;
  m_view = 0L; //needed, because createView() calls processEvents() and the "this" may be deleted before m_view gets a value => crash on delete m_view;
  m_view = m_doc->createView(this, 0L);
  completionInProgress = false;

  kate_doc = dynamic_cast<Kate::Document*>(m_doc);
  kate_view = dynamic_cast<Kate::View*>(m_view);

  editIf = dynamic_cast<KTextEditor::EditInterface *>(m_doc);
#ifdef BUILD_KAFKAPART
  editIfExt = dynamic_cast<KTextEditor::EditInterfaceExt *>(m_doc);
  cursorIf = dynamic_cast<KTextEditor::CursorInterface *>(m_doc);
#endif
  selectionIf = dynamic_cast<KTextEditor::SelectionInterface *>(m_doc);
  configIf = dynamic_cast<KTextEditor::ConfigInterface*>(m_doc);
  viewCursorIf = dynamic_cast<KTextEditor::ViewCursorInterface *>(m_view);
  codeCompletionIf = dynamic_cast<KTextEditor::CodeCompletionInterface *>(m_view);
  markIf = dynamic_cast<KTextEditor::MarkInterface *>(m_doc);
  KTextEditor::MarkInterfaceExtension* iface = dynamic_cast<KTextEditor::MarkInterfaceExtension*>( m_doc );
  iface->setPixmap(KTextEditor::MarkInterface::markType10, SmallIcon("stop"));
  baseURL = p_baseURL;
  m_project = project;
  tempFile = 0;
  dtdName = project->defaultDTD();
  m_parsingDTD = dtdName;
  reparseEnabled = true;
  repaintEnabled = true;
#ifdef BUILD_KAFKAPART
  docUndoRedo = new undoRedo(this);
#endif

  //need access to plugin interface. and we can't get to app from here ..
  m_pluginInterface = a_pIf;
  //each document remember wheter it has a entry in quantarc
  m_backupEntry = false;
  //path of the backup copy file of the document
  m_backupPathValue = QString::null;
  userTagList.setAutoDelete(true);

  connect( m_doc,  SIGNAL(charactersInteractivelyInserted (int ,int ,const QString&)),
           this,  SLOT(slotCharactersInserted(int ,int ,const QString&)) );

  connect( m_view, SIGNAL(completionAborted()),
           this,  SLOT(  slotCompletionAborted()) );

  connect( m_view, SIGNAL(completionDone(KTextEditor::CompletionEntry)),
           this,  SLOT(  slotCompletionDone(KTextEditor::CompletionEntry)) );

  connect( m_view, SIGNAL(filterInsertString(KTextEditor::CompletionEntry*,QString *)),
           this,  SLOT(  slotFilterCompletion(KTextEditor::CompletionEntry*,QString *)) );
  connect( m_doc, SIGNAL(textChanged()), SLOT(slotTextChanged()));

//  setFocusProxy(m_view);
}

Document::~Document()
{
 delete m_view;
 delete m_doc;
}

void Document::resizeEvent(QResizeEvent *e)
{
  QWidget::resizeEvent(e);
  QWidget *wd=dynamic_cast<QWidget*>(parent());
  if (wd)
  {
    int w = wd->width() -5 ;
    int h = wd->height() - 9;
    quantaApp->view()->resize(w,h);
  }
}

void Document::setUntitledUrl(QString url)
{
  untitledUrl = url;
  m_doc->openURL(KURL());
}

bool Document::isUntitled()
{
  return (m_doc->url().url().isEmpty()) ? true : false;
}

KURL Document::url()
{
  return ( isUntitled() ) ? KURL("file:/"+untitledUrl) : m_doc->url();
}

// kwrite addons

void Document::insertTag(QString s1,QString s2)
{
  QString selection = "";

  if ( selectionIf->hasSelection() )
  {
    reparseEnabled = false;
    selection = selectionIf->selection();
    selectionIf->removeSelectedText();
    reparseEnabled = true;
  }
  insertText(s1+selection);
  insertText(s2, FALSE); // don't adjust cursor, thereby leaving it in the middle of tag
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
    tag->namePos(nl, nc);
    reparseEnabled = false;
    editIf->removeText(bl, bc+1, nl, nc);
    reparseEnabled = true;
  } else
  {
    tag->beginPos(bl, bc);
  }
  if (nameSpace.isEmpty())
  {
    slotDelayedTextChanged();
  } else
  {
    viewCursorIf->setCursorPositionReal((uint)bl, (uint)(bc+1));
    insertText(nameSpace+":");
  }
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

/**  */
void Document::selectText(int x1, int y1, int x2, int y2 )
{
  selectionIf->setSelection(x1, y1, x2, y2);
}


void Document::replaceSelected(const QString &s)
{
 unsigned int line, col;

 viewCursorIf->cursorPositionReal(&line, &col);
 reparseEnabled = false;
 selectionIf->removeSelectedText();
 reparseEnabled = true;
 editIf->insertText(line, col, s);

}

void Document::readConfig(KConfig *config)
{
  bool m = m_doc->isModified();
  configIf->readConfig( config );
  m_doc->setModified(m);
}

void Document::writeConfig(KConfig *config)
{
  configIf->writeConfig( config );
}

/** No descriptions */
void Document::insertFile(const KURL& url)
{
  QString fileName;
  if (url.isLocalFile())
  {
    fileName = url.path();
  } else
  {
    if (!KIO::NetAccess::download(url, fileName))
    {
      KMessageBox::error(0, i18n("<qt>Cannot download <b>%1</b>.</qt>").arg( url.prettyURL(0, KURL::StripFileProtocol)));
      return;
    }
  }
  QFile file(fileName);
  if (file.open(IO_ReadOnly))
  {
    QTextStream stream( &file );
    stream.setEncoding(QTextStream::UnicodeUTF8);
//  kate_view->insertText(stream.read());
    insertText(stream.read());
    file.close();
  } else
    KMessageBox::error(this, i18n("<qt>Cannot open <b>%1</b> for reading.</qt>").arg(url.prettyURL(0, KURL::StripFileProtocol)));
}

/** Inserts text at the current cursor position */
void Document::insertText(const QString &text, bool adjustCursor, bool reparse)
{
  if(text.isEmpty())
    return;

  reparseEnabled = false;
  unsigned int line, col;

  viewCursorIf->cursorPositionReal(&line, &col);
  editIf->insertText(line, col, text);

  // calculate new cursor position
  // counts the words and whitespace of the text so we can place the
  // cursor correctly and quickly with the viewCursorInterace, avoiding
  // the Kate::View::insertText method
  if(adjustCursor)
  {
    unsigned textLength = text.length();
    unsigned int wordWrapAt = kate_doc->wordWrapAt();
    uint i=0, j=0;
    int wordLength;
    bool noWordWrap = !(kate_doc->wordWrap());
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
          quantaApp->getsTab()->slotReparse(this, baseNode , qConfig.expandLevel);
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
        insertText("\n<" +tagStr + "/>", true, false);
      } else
      {
        insertText("\n<" +tagStr + ">", true, false);
      }
      QString closingStr;
      if (insertChildTags(childTag, tag))
      {
        closingStr = "\n";
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
  if ( m_doc )
   modified = m_doc->isModified();

  return modified;
}
/** Sets the modifiedFlag value. */
void Document::setModified(bool flag)
{
  m_doc->setModified(flag);
}

/** No descriptions */
int Document::checkOverwrite(const KURL& u)
{
  int query = KMessageBox::Yes;

  if (QExtFileInfo::exists(u))
  {
    QString s = (u.isLocalFile()) ? u.path(): u.prettyURL();
    query = KMessageBox::warningYesNo( this,
            i18n( "<qt>The file <b>%1</b> already exists.<br>Do you want to overwrite it?</qt>" ).arg(s) );

  }

  return query;
}

/** Creates a temporary file where the url is backed up. */
int Document::createTempFile()
{
 closeTempFile();
 tempFile = new KTempFile(tmpDir);
 tempFile->setAutoDelete(true);
 tempFile->textStream()->setEncoding(QTextStream::UnicodeUTF8);
 * (tempFile->textStream()) << editIf->text();

 m_tempFileName = QFileInfo(*(tempFile->file())).filePath();
 tempFile->close();

 return 1;
}

/** No descriptions */
int Document::closeTempFile()
{
// tempFile->unlink();
 if (tempFile != 0)
 {
   delete tempFile;
   tempFile = 0;
 }

 return 1; //not used yet
}
/** No descriptions */
void Document::clearTempFile()
{
// tempFile->unlink();
 delete tempFile;
 tempFile = new KTempFile(tmpDir);
}
/** No descriptions */
QString Document::tempFileName()
{
 return m_tempFileName;
}

/** No descriptions */
bool Document::saveIt()
{
 bool modifyStatus = m_doc->isModified();
 save();
 m_doc->setModified(modifyStatus);
 return true;   //not used yet
}

/** This will return the current tag name at the given position.
    It will work even if the tag has not been completed yet. An
    empty string will be returned if no tag is found.
*/
QString Document::getTagNameAt(int line, int col )
{
 QString name = "";
 Node *node = parser->nodeAt(line, col, false);

 if (node && node->tag)
 {
   Tag *tag = new Tag();
   int bl, bc;
   uint el, ec;
   node->tag->beginPos(bl, bc);
   int el2, ec2;
   node->tag->endPos(el2, ec2);
   if (el2 == line && ec2 + 1 == col && node->tag->tagStr().endsWith(">"))
     return name;
   viewCursorIf->cursorPositionReal(&el, &ec);
   ec--;
   tag->setTagPosition(bl, bc, el, ec);
   tag->parse(text(bl,bc,el,ec), this);

   name = (tag->nameSpace.isEmpty()) ? tag->name : tag->nameSpace + ":" + tag->name;
   if (name.isEmpty())
   {
     QString s = tag->tagStr();
     int pos = s.find("<");
     if (pos !=-1)
     {
       s.remove(0,pos);
       pos = 0;
       while (pos < (int)s.length() &&
              !s[pos].isSpace() &&
              s[pos] != '>') pos++;
       name = s.mid(1, pos -1).stripWhiteSpace();
     } else
     {
       name = "";
     }
   }
   delete tag;
 }
 return name;
}

/** Show the code completions passed in as an argument */
void Document::showCodeCompletions( QValueList<KTextEditor::CompletionEntry> *completions ) {
  codeCompletionIf->showCompletionBox( *completions, false );
}

/** Once the completed text has been inserted into the document we
    want to update the cursor position.
*/
void Document::slotCompletionDone( KTextEditor::CompletionEntry completion )
{
  unsigned int line,col;
  completionInProgress = false;
  viewCursorIf->cursorPositionReal(&line,&col);
  DTDStruct* dtd = currentDTD();
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
  scriptAutoCompletion(m_lastLine, m_lastCol);
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
  int pos = completion->userdata.find("|");
  QString s = completion->userdata.left(pos);
  completion->userdata.remove(0,pos+1);
  string->remove(0, s.length());
  if (completion->type == "charCompletion")
  {
    *string = completion->userdata;
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
    QString s2 = QString("public \""+QuantaCommon::getDTDNameFromNickName(s)+"\"");
    DTDStruct *dtd = dtds->find(QuantaCommon::getDTDNameFromNickName(s));
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

/** Called when a user types in a character. From this we can show possibile
    completions based on what they are trying to input.
*/
void Document::slotCharactersInserted(int line,int column,const QString& string)
{
 bool handled = false;
 if (qConfig.useAutoCompletion)
 {
  if (completionInProgress)
  {
    codeCompletionRequested();
  } else
  {
    completionDTD = currentDTD();
    if (completionDTD->family == Xml)
    {
      handled = xmlAutoCompletion(line, column, string);
    }
    if (completionDTD->family == Script)
    {
      handled = scriptAutoCompletion(line, column);
      handled = true;
    }

    if (!handled)
    {
      DTDStruct *lastDTD = completionDTD;
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
  if (!tag)
     tag = userTagList.find(tagName.lower());

  QString s = editIf->textLine(line).left(column + 1);
  int i = column;
  while (i > 0 && s[i].isSpace())
    i--;
  s = s.left(i + 1);
  
  if ( !tag || tagName.isEmpty() )  //we are outside of any tag
  {

    if ( s.endsWith(completionDTD->tagAutoCompleteAfter) )  // a tag is started
    {
      //we need to complete a tag name
      showCodeCompletions( getTagCompletions(line, column) );
      handled = true;
    } else
    if (string == ">" && !tagName.isEmpty() && tagName[0] != '!' &&
        tagName[0] != '/' && !tagName.endsWith("/") && !s.endsWith("/>") &&
        qConfig.closeTags &&
        currentDTD(true)->family == Xml) //close unknown tags
    {
      //add closing tag if wanted
      column++;
      editIf->insertText(line, column, "</" + tagName + ">");
#ifdef BUILD_KAFKAPART
      docUndoRedo->dontAddModifsSet(2);
#endif
      viewCursorIf->setCursorPositionReal( line, column );
      handled = true;
    } else
    if (string == "/" && tagName == "/")
    {
      Node *node = parser->nodeAt(line, column, false);
      if (node && node->parent)
      {
        node = node->parent;
        QString name = node->tag->name;
        name = name.left(name.find(" | "));
        editIf->insertText(line, column + 1, name + ">");
#ifdef BUILD_KAFKAPART
        docUndoRedo->dontAddModifsSet(2);
#endif
        viewCursorIf->setCursorPositionReal( line, column + node->tag->name.length() + 2);
        handled = true;
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
#ifdef BUILD_KAFKAPART
        docUndoRedo->dontAddModifsSet(2);
#endif
        viewCursorIf->setCursorPositionReal( line, column+3 );
        handled = true;
      }
      if ( ( !tag->isSingle() && !tag->isOptional() && qConfig.closeTags) ||
           ( tag->isOptional() && qConfig.closeOptionalTags ) )
      {
        //add closing tag if wanted
        column++;
        editIf->insertText(line, column, "</" + tagName + ">");
#ifdef BUILD_KAFKAPART
        docUndoRedo->dontAddModifsSet(2);
#endif
        viewCursorIf->setCursorPositionReal( line, column );
        handled = true;
      }
      if (!tag->childTags.isEmpty())
      {
        reparseEnabled = false;
        insertText("\n", false, false);
        insertChildTags(tag);
        reparseEnabled = true;
        baseNode = parser->rebuild(this);
        if (qConfig.instantUpdate && quantaApp->structTreeVisible())
              quantaApp->getsTab()->slotReparse(this, baseNode , qConfig.expandLevel);
      }
    }
    else if ( string == " " )
         {
           kdDebug(24000) << "TagName: " << tagName << endl;
           bool showAttributes = true;
           Node *node = parser->nodeAt(line, column);
           if (node)
           {
             int index = node->tag->valueIndexAtPos(line, column);
             if (index != -1)
                showAttributes = false;
           }
          //suggest attribute completions
          if (node && showAttributes)
          {
            int bl, bc;
            node->tag->beginPos(bl, bc);
            if (node->tag->attrCount() > 0)
            {
              s = editIf->text(bl, bc, line, column);
              if (s.contains(' ') != 0)
                s = s.section(' ', -1);
              else
                s = "";
              if (s.contains("="))
                  s = "";
            }
            kdDebug(24000) << "s: " << s << endl;
/*            if (node->tag->attrCount() > 0)
            {
              s =  node->tag->tagStr().section(' ', -1);
              if (s.endsWith(">"))
                  s = s.left(s.length() - 1);
              if (s.startsWith("<"))
                  s = s.mid(1);
              s = s.stripWhiteSpace();
              if (s.contains("="))
                  s = "";
            } */
            showCodeCompletions( getAttributeCompletions(tagName, s) );
          }
         }
    else if ( string[0] == qConfig.attrValueQuotation )
          {
          //we need to find the attribute name
          QString textLine = editIf->textLine(line).left(column-1);
          QString attribute = textLine.mid(textLine.findRev(' ')+1);
          showCodeCompletions( getAttributeValueCompletions(tagName, attribute) );
          handled = true;
          }
  } // else - we are inside of a tag
  if (!handled)
  {
    QString s = editIf->textLine(line).left(column + 1);
    int pos = s.findRev('&');
    if (pos != -1)
    {
      s = s.mid(pos + 1);
      //complete character codes
      showCodeCompletions( getCharacterCompletions(s) );
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
  GroupElementMapList map = parser->m_groups;
  GroupElementMapList::Iterator it;
  QString str = group.name;
  str.append("|");
  str.append(word);
  for ( it = map.begin(); it != map.end(); ++it )
  {
    if (it.key().startsWith(str) && it.key() != str )
    {
      GroupElementList elementList = it.data();
      for (uint i = 0; i < elementList.count(); i++)
      {
        if (elementList[i].parentNode == 0L || elementList[i].global)
        {
          completion.text = it.key().section('|', -1);
          completions->append( completion );
          break;
        } else
        {
          Node *n = node;
          while (n && n != elementList[i].parentNode)
          {
            n = n->parent;
          }
          if (n == elementList[i].parentNode)
          {
            completion.text = it.key().section('|', -1);
            completions->append( completion );
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
        completion.text = list[i];
        completions->append( completion );
      }
    }
  }


  return completions;
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
    case Script: completion.type = "script";
                 break;
  }
  Node *node = parser->nodeAt(line, col);
  if (node && node->tag->type != Tag::XmlTag)
      node = node->parent;
  if (node && node->tag->type != Tag::XmlTag)
      node = 0L;
  QTag *parentQTag= 0L;
  if (node)
      parentQTag = QuantaCommon::tagFromDTD(node->tag->dtd, node->tag->name);
  QString textLine = editIf->textLine(line).left(col);
  QString word = findWordRev(textLine, completionDTD).upper();
  completion.userdata = word +"|";
  QStringList tagNameList;
  QString tagName;
  QDictIterator<QTag> it(*(completionDTD->tagsList));
  for( ; it.current(); ++it )
  {
    tagName = it.current()->name();
    if (tagName.upper().startsWith(word))
    {
      if (!parentQTag || (parentQTag && parentQTag->isChild(tagName)))
          tagNameList += it.current()->name();
    }
  }

  QDictIterator<QTag> it2(userTagList);
  for( ; it2.current(); ++it2 )
  {
    if (it2.current()->name().upper().startsWith(word))
    {
      tagNameList += it2.current()->name();
    }
  }

  tagNameList.sort();
  for (uint i = 0; i < tagNameList.count(); i++)
  {
   completion.text = QuantaCommon::tagCase( tagNameList[i]);
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
                QDictIterator<DTDStruct> it(*dtds);
                for( ; it.current(); ++it )
                {
                 completion.type = "doctypeList";
                 completion.text = it.current()->nickName;
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
      values = new QStringList();
      deleteValues = true;
      for ( QStringList::Iterator it = parser->selectors.begin(); it != parser->selectors.end(); ++it )
      {
        int index = (*it).find('.');
        if (index != -1)
        {
          QString tmpStr = (*it).left(index);
          if (tmpStr.isEmpty() || tagName.lower() == tmpStr)
          {
            values->append((*it).mid(index + 1));
          }
        }
      }
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
  QValueList<KTextEditor::CompletionEntry> *completions = new QValueList<KTextEditor::CompletionEntry>();

  KTextEditor::CompletionEntry completion;
  completion.type = "charCompletion";

  for ( QStringList::Iterator it = charList.begin(); it != charList.end(); ++it )
  {
    completion.text = *it;
    int begin = completion.text.find("(&") + 2;
    if (begin == 1)
        continue;
    int length = completion.text.find(";)") - begin + 1;
    QString s = completion.text.mid(begin, length);
    completion.text = "&" + s + " : " + completion.text.left(begin -2) + " - " + completion.text.mid(begin + length + 1);
    if (s.startsWith(startsWith))
    {
      completion.userdata = startsWith + "|" + s.mid(startsWith.length());
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
void Document::setDTDIdentifier(QString id)
{
  dtdName = id.lower();
  if (dtds->find(dtdName))
  {
    m_parsingDTD = dtdName;
  }
}

/** Get a pointer to the current active DTD. If fallback is true, this always gives back a valid and known DTD pointer: the active, the document specified and in last case the application default document type. */
DTDStruct* Document::currentDTD(bool fallback)
{
  uint line, col;
  viewCursorIf->cursorPositionReal(&line, &col);

  DTDStruct *dtd = parser->currentDTD(line, col);
  if (fallback)
  {
    if (!dtd) dtd = dtds->find(dtdName.lower());
    if (!dtd) dtd = dtds->find(m_project->defaultDTD());
    if (!dtd) dtd = dtds->find(qConfig.defaultDocType); //this will always exists
  }

  return dtd;
}

/** Get a pointer to the default DTD (document, or app). */
DTDStruct* Document::defaultDTD()
{
  DTDStruct* dtd =  dtds->find(dtdName);
  if (!dtd) dtd = dtds->find(m_project->defaultDTD());
  if (!dtd) dtd = dtds->find(qConfig.defaultDocType); //this will always exists

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
     line = i;
     startPos = text.findRev('<',pos);
     while (startPos == -1 && line >=0)
     {
       text = editIf->textLine(line);
       startPos = text.findRev('<');
       line--;
     }
     if (startPos == -1)
     {
        i++;
        continue;
     }
     int bl, bc, el, ec;
     bl = line++;
     bc = startPos;
     line = i;
     text = editIf->textLine(i);
     startPos = text.find('>',pos);
     while (startPos == -1 && line < endLine)
     {
       text = editIf->textLine(line);
       startPos = text.find('>');
       line++;
     }
     if (startPos == -1)
     {
        i++;
        continue;
     }
     el = line--;
     ec = startPos + 1;
     *tag = new Tag();
     (*tag)->setTagPosition(bl, bc, el, ec);
     text = this->text(bl, bc, el, ec);
     (*tag)->parse(text, this);
     (*tag)->type = Tag::XmlTag;
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
bool Document::scriptAutoCompletion(int line, int column)
{
 bool handled = false;
 QString s = editIf->textLine(line).left(column + 1);
 QString s2 = s;
 int i = column;
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
   QTag *tag = completionDTD->tagsList->find(word);
   if (!tag) 
     tag = userTagList.find(word.lower());
   if (tag)
   {
     QStringList argList;
     QString arguments;
     if (tag->type != "property")
     {
      for (int i =0; i < tag->attributeCount(); i++)
      {
        Attribute* attr = tag->attributeAt(i);
        if (attr->status == "optional")
        {
          arguments = arguments + "["+attr->type +" "+attr->name +"],";
        } else
        {
          arguments = arguments + attr->type +" "+attr->name +",";
        }
      }
      arguments = tag->returnType +" "+tag->name() + "("+arguments.left(arguments.length()-1)+")";
      argList.append(arguments);
      codeCompletionIf->showArgHint(argList, "()" , completionDTD->attributeSeparator);
     } else
     {
       arguments = tag->name() + ": " + tag->attributeAt(0)->name + ";";
       argList.append(arguments);
       codeCompletionIf->showArgHint(argList, ":;" , completionDTD->attributeSeparator);
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
 if ( !handled && (s[i] == completionDTD->tagAutoCompleteAfter || completionDTD->tagAutoCompleteAfter == '\1'))
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
    handled = scriptAutoCompletion(line, col - 1);
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
  uint line, col;
  viewCursorIf->cursorPositionReal(&line, &col);
  completionDTD = currentDTD();
  if (completionDTD->family == Script)
  {
//    QString textLine = editIf->textLine(line).left(col);
//    int pos = textLine.findRev("(");
//    int pos2 = textLine.findRev(")");
    //if (pos > pos2 )
       scriptAutoCompletion(line, col - 1);
  }
}

/** Find the word until the first word boundary backwards */
QString Document::findWordRev(const QString& textToSearch, DTDStruct *dtd)
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
    if (col > bCol && col <= (int)(bCol + tagName.length()+1)) //we are inside a tag name, so show the possible tags
    {
     showCodeCompletions( getTagCompletions(line, col) );
     handled = true;
    } else
    {
      index = tag->valueIndexAtPos(line,col);
      if (index != -1)      //inside a value
      {
        tag->attributeValuePos(index, bLine, bCol);
        s = tag->attributeValue(index).left(col - bCol);
        showCodeCompletions( getAttributeValueCompletions(tagName, tag->attribute(index), s) );
        handled = true;
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
      //complete character codes
      showCodeCompletions( getCharacterCompletions(s) );
      handled = true;
    }
  }
  return handled;
}

/** No descriptions */
void Document::slotCompletionAborted()
{
 completionInProgress = false;
}

/** Ask for user confirmation if the file was changed outside. */
void Document::checkDirtyStatus()
{
  QString fileName;
  if (url().isLocalFile())
      fileName = url().path();
  if (m_dirty)
  {
    if (url().isLocalFile())
    {
      //check if the file is changed, also by file content. Might help to reduce
      //unwanted warning on NFS
      QFile f(fileName);
      QFile tmpFile(m_tempFileName);
      if (f.open(IO_ReadOnly) && tmpFile.open(IO_ReadOnly))
      {
        QString content;
        QTextStream stream(&f);
        stream.setEncoding(QTextStream::UnicodeUTF8);
        content = stream.read();
        QString tmpContent;
        QTextStream tmpStream(&tmpFile);
        tmpStream.setEncoding(QTextStream::UnicodeUTF8);
        tmpContent = tmpStream.read();
        if (content == tmpContent)
        {
          m_dirty = false;
        }
        f.close();
      }

    }
    if (m_dirty)
    {
      createTempFile();
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
          m_doc->openURL(url());
          createTempFile();
      }
      delete dlg;
    }
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
    m_doc->save();
    m_dirty = false;
    fileWatcher->addFile(fileName);
  } else
  {
    m_doc->save();
    m_dirty = false;
  }
}

/** No descriptions */
void Document::setParsingDTD(const QString& dtdName)
{
  m_parsingDTD = dtdName;
}

/** No descriptions */
QString Document::parsingDTD()
{
 return m_parsingDTD;
}

/** Returns true if the number of " (excluding \") inside text is even. */
bool Document::evenQuotes(const QString &text)
{
 int num = text.contains(QRegExp("[^\\\\]\""));

 return (num /2 *2 == num);
}


/** No descriptions */
void Document::slotTextChanged()
{
  changed = true;
  if (reparseEnabled)
  {
    //delay the handling, otherwise we may get wrong values for (line,column)
    QTimer::singleShot(0, this, SLOT(slotDelayedTextChanged()));
  }
}

void Document::slotDelayedTextChanged()
{
    uint line, column;
    QString oldNodeName = "";
    Node *node;
    Node *currentNode = 0L; //holds a copy of the node which is at (line,column)
    Node *previousNode = 0L;//holds a copy of the node before currentNode
    if (qConfig.updateClosingTags)
    {
      viewCursorIf->cursorPositionReal(&line, &column);
      node = parser->nodeAt(line, column, false);
      if (node)
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

    baseNode = parser->rebuild(this);
    if (qConfig.updateClosingTags && currentNode)
    {
      viewCursorIf->cursorPositionReal(&line, &column);
      node = parser->nodeAt(line, column, false);
      if (node && node->tag->name != currentNode->tag->name && (node->tag->type == Tag::XmlTag || node->tag->type == Tag::XmlTagEnd) && node->tag->validXMLTag)
      {
        int bl, bc, bl2, bc2;
        node->tag->beginPos(bl, bc);
        currentNode->tag->beginPos(bl2,bc2);
        if ( (bl != bl2 || bc !=bc2) && previousNode)
        {
          previousNode->tag->beginPos(bl2, bc2);
          delete currentNode;
          currentNode = previousNode;
        } else
        if (previousNode)
        {
          delete previousNode;
          previousNode = 0L;
        }
        if (bl == bl2 && bc == bc2 &&
           (currentNode->tag->type == Tag::XmlTag || currentNode->tag->type == Tag::XmlTagEnd))
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
            if (node->tag->type == Tag::XmlTag || node->tag->type == Tag::XmlTagEnd)
            {
              if (node->tag->name == currentNode->tag->name )
              {
                num++;
              }
              if ( updateClosing && QuantaCommon::closesTag(currentNode->tag, node->tag) ||
                  !updateClosing && QuantaCommon::closesTag(node->tag, currentNode->tag) )
              {
                num--;
              }
              if (num == 0)
              {
                reparseEnabled = false;
                node->tag->namePos(bl, bc);
#ifdef BUILD_KAFKAPART
                if(editIfExt)
                  editIfExt->editBegin();
#endif
                editIf->removeText(bl, bc, bl, bc + node->tag->name.length());
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
#ifdef BUILD_KAFKAPART
                if(editIfExt)
                  editIfExt->editEnd();
#endif
                viewCursorIf->setCursorPositionReal(bl, bc);
#ifdef BUILD_KAFKAPART
                docUndoRedo->mergeNextModifsSet();
#endif
                baseNode = parser->rebuild(this);
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
          delete currentNode;
        }
      }
    }

    quantaApp->slotNewLineColumn();
    if (qConfig.instantUpdate && quantaApp->structTreeVisible())
        quantaApp->getsTab()->slotReparse(this, baseNode , qConfig.expandLevel);
}

/** Returns list of values for attribute */
QStringList* Document::tagAttributeValues(const QString& dtdName, const QString& tag, const QString &attribute, bool &deleteResult)
{
  QStringList *values = 0L;
  deleteResult = true;
  DTDStruct* dtd = dtds->find(dtdName.lower());
  if (dtd)
  {
    QString searchForAttr = (dtd->caseSensitive) ? attribute : attribute.upper();
    AttributeList* attrs = QuantaCommon::tagAttributes(dtdName, tag);
    Attribute *attr;
    KURL u;
    KURL base = QExtFileInfo::toRelative(url(), baseURL);
    base.setPath("/"+base.directory(false, false));
    QString s;
    if (attrs)
    {
      for ( attr = attrs->first(); attr; attr = attrs->next() )
      {
        QString attrName = (dtd->caseSensitive) ? attr->name : attr->name.upper();
        if (attrName == searchForAttr)
        {
          if (attr->type == "url") {
            Project *project = quantaApp->project();
            if (project->hasProject())
            {
              values = new QStringList(project->fileNameList(true).toStringList());
              for (uint i = 0; i < values->count(); i++)
              {
                u = (*values)[i];
                u.setPath("/"+u.path());
                u = QExtFileInfo::toRelative(u, base);
                (*values)[i] = u.path();
              }
              values->append("mailto:" + project->email);
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
      if ( (node->tag->dtd->caseSensitive && node->tag->name == tag) ||
           (!node->tag->dtd->caseSensitive && node->tag->name.lower() == tag.lower()) )
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
  markIf->addMark(line, KTextEditor::MarkInterface::markType10);
}

void Document::clearErrorMarks()
{
  QPtrList<KTextEditor::Mark> marks = markIf->marks();
  KTextEditor::Mark* mark;
  for (mark = marks.first(); mark; mark = marks.next())
  {
    if (mark->type & KTextEditor::MarkInterface::markType10)
        markIf->removeMark(mark->line, KTextEditor::MarkInterface::markType10);
  }
}

/** obvious */
void Document::setBackupEntry(bool b)
{
   m_backupEntry = b;
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
    if(isModified())
    {
     if(isUntitled())
      m_backupPathValue = qConfig.backupDirPath + url().fileName() + "." + hashFilePath(url().path())+"U";
     else
      m_backupPathValue = qConfig.backupDirPath + url().fileName() + "." + hashFilePath(url().path());
     //creates an entry string in quantarc if it does not exist yet
     config->setGroup("General Options");
#if KDE_IS_VERSION(3,1,3)
     QStringList backedupFilesEntryList = config->readPathListEntry("List of backedup files");
     QStringList autosavedFilesEntryList = config->readPathListEntry("List of autosaved files");
#else
     QStringList backedupFilesEntryList = config->readListEntry("List of backedup files");
     QStringList autosavedFilesEntryList = config->readListEntry("List of autosaved files");
#endif
     QString encoding = dynamic_cast<KTextEditor::EncodingInterface*>(m_doc)->encoding();
     if(!autosavedFilesEntryList.contains(m_backupPathValue))
     {
       autosavedFilesEntryList.append(m_backupPathValue);
       config->writeEntry("List of autosaved files", autosavedFilesEntryList);
       backedupFilesEntryList.append(url().path() + "." + qConfig.quantaPID);
       config->writeEntry("List of backedup files", backedupFilesEntryList);
       config->sync();
       setBackupEntry(true);
     }
     //creates a copy of this specific document
     QFile file(m_backupPathValue);
     if(file.open(IO_WriteOnly))
     {
      QTextStream stream(&file);
      stream.setCodec(QTextCodec::codecForName(encoding));
      stream << editIf->text();
      file.close();
     }

    }
}
/** if there is no more need of a backup copy then remove it */
void Document::removeBackup(KConfig *config)
{
  config->reparseConfiguration();
  config->setGroup("General Options");

#if KDE_IS_VERSION(3,1,3)
  QStringList backedupFilesEntryList = config->readPathListEntry("List of backedup files");
  QStringList autosavedFilesEntryList = config->readPathListEntry("List of autosaved files");
#else
  QStringList backedupFilesEntryList = config->readListEntry("List of backedup files");
  QStringList autosavedFilesEntryList = config->readListEntry("List of autosaved files");
#endif
  
  autosavedFilesEntryList.remove(m_backupPathValue);
  config->writeEntry("List of autosaved files",autosavedFilesEntryList);
  backedupFilesEntryList.remove(url().path() + "." + qConfig.quantaPID);
  config->writeEntry("List of backedup files", backedupFilesEntryList);
  config->sync();

  setBackupEntry(false);

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
             for (uint i = 0; i < (p.length() - 1); i++)
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
  DTDStruct *dtd = defaultDTD();
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
    pBar->setTotalSteps(parser->nodeNum);
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
      if (node->tag->dtd == dtd)
      {
        if (tagCase !=0)
        {
#ifdef BUILD_KAFKAPART
          if(editIfExt)
            editIfExt->editBegin();
#endif
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
#ifdef BUILD_KAFKAPART
          if(editIfExt)
            editIfExt->editEnd();
#endif
        }
        if (attrCase != 0)
        {
          QString newName;
          for (int i = 0; i < node->tag->attrCount(); i++)
          {
#ifdef BUILD_KAFKAPART
          if(editIfExt)
            editIfExt->editBegin();
#endif
            node->tag->attributeNamePos(i, bl, bc);
            newName = node->tag->attribute(i);
            ec = bc + newName.length();
            editIf->removeText(bl, bc, bl, ec);
            if (attrCase == 1)
              newName = newName.lower();
            else if (attrCase == 2)
              newName = newName.upper();
            editIf->insertText(bl, bc, newName);
#ifdef BUILD_KAFKAPART
          if(editIfExt)
            editIfExt->editEnd();
#endif
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

#include "document.moc"
