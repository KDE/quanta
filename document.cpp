/***************************************************************************
                          document.cpp  -  description
                             -------------------
    begin                : Tue Jun 6 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
                           (C) 2001-2003 Andras Mantia <amantia@freemail.hu>
    email                : pdima@users.sourceforge.net,yshurik@penguinpowered.com,sequitur@easystreet.com
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

//QT includes
#include <qdir.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qtextstream.h>
#include <qregexp.h>
#include <qradiobutton.h>

// KDE includes
#include <kwin.h>
#include <klocale.h>
#include <kaction.h>
#include <kdialogbase.h>
#include <kiconloader.h>
#include <kmessagebox.h>
#include <ktempfile.h>
#include <kdirwatch.h>
#include <kdebug.h>

#include <ktexteditor/cursorinterface.h>
#include <ktexteditor/clipboardinterface.h>
#include <ktexteditor/configinterface.h>
#include <ktexteditor/wordwrapinterface.h>

#include "parser/tag.h"
#include "quantacommon.h"
#include "document.h"
#include "resource.h"
#include "dialogs/dirtydlg.h"
#include "project/project.h"
#include "plugins/quantaplugininterface.h"

#include "quanta.h"
#include "treeviews/structtreeview.h"

#define STEP 1

Document::Document(const KURL& p_baseURL, KTextEditor::Document *doc,
                   Project *project, QuantaPluginInterface *a_pIf,
                   QWidget *parent, const char *name, WFlags f )
  : QWidget(parent, name, f)
{
  m_dirty   = false;
  busy    = false;
  oldstat = false;
  m_doc = doc;
  m_view = m_doc->createView(this, 0L);
  completionInProgress = false;

  kate_doc = dynamic_cast<Kate::Document*>(m_doc);
  kate_view = dynamic_cast<Kate::View*>(m_view);

  editIf = dynamic_cast<KTextEditor::EditInterface *>(m_doc);
  selectionIf = dynamic_cast<KTextEditor::SelectionInterface *>(m_doc);
  viewCursorIf = dynamic_cast<KTextEditor::ViewCursorInterface *>(m_view);
  codeCompletionIf = dynamic_cast<KTextEditor::CodeCompletionInterface *>(m_view);
  baseURL = p_baseURL;
  m_project = project;
  tempFile = 0;
  dtdName = project->defaultDTD();
  m_parsingDTD = dtdName;
  reparseEnabled = true;

  //need access to plugin interface. and we can't get to app from here ..
  m_pluginInterface = a_pIf;

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
  int w = wd->width() -5 ;
  int h = wd->height() - 9;
  m_view->resize(w,h);
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
  QDictIterator<QString> it( *dict ); // iterator for dict
  QDict<QString> oldAttr(1,false);
  QString tagStr = "";
  QTag *qTag = QuantaCommon::tagFromDTD(currentDTD(true), tag->name);

  while ( it.current() )  // for insert new attr
  {
    QString val = *(it.current());
    QString attr = QuantaCommon::attrCase(it.currentKey());
    QString attrval;

    if (qTag && qTag->parentDTD->singleTagStyle == "xml" && attr=="/")
    {
     ++it;
     continue;
    }

    if ( val.isEmpty() )  // for checkboxes ( without val) don't print =""
        attrval = QString(" ")+attr;
    else
    {
      attrval = QString(" ")+attr+"=";
      if (! val.startsWith("\\\"")) attrval += "\"";
      attrval += val;
      if (! val.endsWith("\\\"")) attrval += "\"";
    }
    tagStr = attrval + tagStr;
    ++it;
  }
  tagStr = "<"+QuantaCommon::tagCase(tag->name)+tagStr;

  if ( qTag && qTag->parentDTD->singleTagStyle == "xml" &&
       (qTag->isSingle() || (!qConfig.closeOptionalTags && qTag->isOptional()))
     )
  {
    tagStr.append(" /");
  }
  tagStr.append(">");

  int bLine, bCol, eLine, eCol;
  tag->beginPos(bLine,bCol);
  tag->endPos(eLine,eCol);
  editIf->removeText(bLine, bCol, eLine, eCol+1);
  viewCursorIf->setCursorPositionReal((uint)bLine, (uint)bCol);
  insertText(tagStr);
}

/**  */
void Document::selectText(int x1, int y1, int x2, int y2 )
{
  selectionIf->setSelection(x1, y1, x2, y2);
}


void Document::replaceSelected(QString s)
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
  dynamic_cast<KTextEditor::ConfigInterface*>(m_doc)->readConfig( config );
  m_doc->setModified(m);
}

void Document::writeConfig(KConfig *config)
{
  dynamic_cast<KTextEditor::ConfigInterface*>(m_doc)->writeConfig( config );
}

/** No descriptions */
void Document::insertFile(const KURL& url)
{
  if (url.isLocalFile())
  {
    QFile file(url.path());
    file.open(IO_ReadOnly);

    QTextStream stream( &file );
  //  kate_view->insertText(stream.read());
    insertText(stream.read());

    file.close();
  }
}

/** Inserts text at the current cursor position */
void Document::insertText(QString text, bool adjustCursor)
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
    viewCursorIf->setCursorPositionReal(line, col);
  }

  reparseEnabled = true;
  baseNode = parser->rebuild(this);
  if (qConfig.instantUpdate)
        quantaApp->getsTab()->slotReparse(this, baseNode , qConfig.expandLevel);
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
int Document::checkOverwrite(KURL u)
{
  int query = KMessageBox::Yes;

  if( u.isLocalFile() )
  {
    QFileInfo info;
    QString name( u.path() );
    info.setFile( name );
    if( info.exists() )
      query = KMessageBox::warningYesNoCancel( this,
        i18n( "A Document with this name already exists.\nDo you want to overwrite it?" ) );
  }
  return query;
}

/** Creates a temporary file where the url is backed up. */
int Document::createTempFile()
{
 closeTempFile();
 tempFile = new KTempFile(tmpDir);
 tempFile->setAutoDelete(true);
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
   Tag *tag = new Tag;
   int bl, bc;
   uint el, ec;
   node->tag->beginPos(bl, bc);
   viewCursorIf->cursorPositionReal(&el, &ec);
   tag->parse(text(bl,bc,el,ec), this);

   name = tag->name;
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
  if (completion.type == "attribute")
  {
    viewCursorIf->setCursorPositionReal(line,col-1);
    if (dtd)
    {
      QTag *tag = QuantaCommon::tagFromDTD(dtd,completion.userdata);
      if (tag)
      {
        showCodeCompletions( getAttributeValueCompletions(tag->name(), completion.text) );
      }
    }
  }
  if (completion.type == "attributeValue")
  {
    viewCursorIf->setCursorPositionReal(line, col);
  }
  if (completion.type == "doctypeList")
  {
    viewCursorIf->setCursorPositionReal(line,col+1);
  }
  if (completion.type == "script")
  {
    viewCursorIf->setCursorPositionReal(line,col);
    if (dtd) scriptAutoCompletion(line, col-1, completionDTD->attrAutoCompleteAfter);
  }
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

  if ( completion->type == "attributeValue")
  {
    string->append(completionDTD->attributeSeparator);
  }
  if ( completion->type == "attribute" )
  {
    string->append("=\"\"");
  }
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
  }
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
      handled = scriptAutoCompletion(line, column, string);
      //handled = true;
    }

    if (!handled)
    {
      completionDTD = defaultDTD();
      if (completionDTD->family == Xml)
      {
        xmlAutoCompletion(line, column, string);
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
  if (!tag) tag = userTagList.find(tagName.lower());

  if ( !tag || tagName.isEmpty() )  //we are outside of any tag
  {
    if ( string == "<" )  // a tag is started
    {
      //we need to complete a tag name
      showCodeCompletions( getTagCompletions(line, column) );
      handled = true;
    } else
    if ( string == "&")
    {
          //complete character codes
          //showCodeCompletions( getCharacterCompletions() );
    } else
    if (string == ">" && tagName[0] != '/' && qConfig.closeTags
        && currentDTD(true)->family == Xml) //close unknown tags
    {
      //add closing tag if wanted
      column++;
      editIf->insertText(line, column, "</" + tagName + ">");
      viewCursorIf->setCursorPositionReal( line, column );
      handled = true;
    }
  }
  else  // we are inside of a tag
  {
    if ( string == ">" && tagName[0] != '/' && tag)
    {
      if ( tag->parentDTD->singleTagStyle == "xml" &&
           (tag->isSingle() || (!qConfig.closeOptionalTags && tag->isOptional()))
         )
      {
        editIf->insertText(line, column, " /");
        viewCursorIf->setCursorPositionReal( line, column+3 );
        handled = true;
      }
      if ( ( !tag->isSingle() && !tag->isOptional() && qConfig.closeTags) ||
           ( tag->isOptional() && qConfig.closeOptionalTags ) )
      {
        //add closing tag if wanted
        column++;
        editIf->insertText(line, column, "</" + tagName + ">");
        viewCursorIf->setCursorPositionReal( line, column );
        handled = true;
      }
    }
    else if ( string == " " )
         {
          //suggest attribute completions
          showCodeCompletions( getAttributeCompletions(tagName) );
         }
    else if ( string == "\"" )
          {
          //we need to find the attribute name
          QString textLine = editIf->textLine(line).left(column-1);
          QString attribute = textLine.mid(textLine.findRev(' ')+1);
          showCodeCompletions( getAttributeValueCompletions(tagName, attribute) );
          handled = true;
          }
  } // else - we are inside of a tag

 return handled;
}

/** Return a list of possible variable name completions */
QValueList<KTextEditor::CompletionEntry>* Document::getVariableCompletions(int line, int col)
{
  QValueList<KTextEditor::CompletionEntry> *completions = new QValueList<KTextEditor::CompletionEntry>();
  KTextEditor::CompletionEntry completion;

  completion.type = "variable";

  QString textLine = editIf->textLine(line).left(col);
  QString word = findWordRev(textLine);
  completion.userdata = word;

  for (uint i = 0; i < variableList.count(); i++)
  {

   if (variableList[i].startsWith(word))
   {
     completion.text = variableList[i];
     completions->append( completion );
   }
  }

//  completionInProgress = true;
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
  QString textLine = editIf->textLine(line).left(col);
  QString word = findWordRev(textLine).upper();
  completion.userdata = word;
  QStringList tagNameList;
  QDictIterator<QTag> it(*(completionDTD->tagsList));
  for( ; it.current(); ++it )
  {
    if (it.current()->name().upper().startsWith(word))
    {
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
              for (uint i = 0; i < list->count(); i++)
              {
                QString item = list->at(i)->name;
                if (item.upper().startsWith(startsWith))
                {
                  completion.text = QuantaCommon::attrCase(item);
                  completion.comment = list->at(i)->type;
                  completions->append( completion );
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
                    completions->append( completion );
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
                completions->append(completion);
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

  QStringList *values = tagAttributeValues(completionDTD->name,tagName,attribute);
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

//  completionInProgress = true;
  return completions;
}

/** Return a list of chatacter completions (like &nbsp; ...) */
QValueList<KTextEditor::CompletionEntry>* Document::getCharacterCompletions()
{
  QValueList<KTextEditor::CompletionEntry> *completions = new QValueList<KTextEditor::CompletionEntry>();

  //need to get this information from something

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
bool Document::scriptAutoCompletion(int line, int column, const QString& string)
{
 bool handled = false;
 QString s = editIf->textLine(line).left(column);
 int i = column - 1;
 while (i > 0 && s[i].isSpace())
   i--;
 s = s.left(i + 1);
 if (
     s.endsWith(completionDTD->attrAutoCompleteAfter) ) //if we need to list the arguments of a function
 {
   QString textLine = editIf->textLine(line).left(i);
   QString word = findWordRev(textLine, completionDTD);
   QTag *tag = completionDTD->tagsList->find(word);
   if (!tag) tag = userTagList.find(word.lower());
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
       showCodeCompletions( getAttributeValueCompletions(tag->name(), tag->attributeAt(0)->name, ""));
     }

     handled = true;
   }
 }
 //TODO: this is PHP specific. Make it generic
 if (string == "$")
 {
   showCodeCompletions( getVariableCompletions(line, column) );
   handled = true;
 }

 return handled;
}

/** Retriwes the text from the specified rectangle. The KTextEditor::EditInterface::text seems to not
work correctly. */
QString Document::text(int bLine, int bCol, int eLine, int eCol)
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
    handled = scriptCodeCompletion(line, col);
  }
  if (!handled)
  {
    completionDTD = defaultDTD();
    if (completionDTD->family == Xml)
    {
      xmlCodeCompletion(line, col);

    }
/* TODO: Can the default DTD be a script??
    if (dtd->family == Script)
    {
      scriptCodeCompletion(dtd, line, col);
    }
*/
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
       scriptCodeCompletion(line, col);
  }
}

/** Find the word until the first word boundary backwards */
QString Document::findWordRev(const QString& textToSearch, DTDStruct *dtd)
{
  QString t = textToSearch;
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
  if (node && node->tag)
  {
    Tag *tag = node->tag;
    int bLine, bCol;
    tag->beginPos(bLine, bCol);
    QString s;
    int index;
    if (col <= (int)(bCol+tag->name.length()+1)) //we are inside a tag name, so show the possible tags
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
        showCodeCompletions( getAttributeValueCompletions(tag->name, tag->attribute(index), s) );
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
            s="";
          }
          showCodeCompletions( getAttributeCompletions(tag->name, s) );
          handled = true;
        }
      }
    }
  }
  return handled;
}
/** Code completion is manually invoked for script type languages. */
bool Document::scriptCodeCompletion(int line, int col)
{
 bool handled = false;
 QString s = text(line, 0,line, col).section(completionDTD->attrAutoCompleteAfter,-1);
 if (s.stripWhiteSpace().isEmpty())
 {
   scriptAutoCompletion(line, col, completionDTD->attrAutoCompleteAfter);
   handled = true;
 } else
 {
   bool goAhead = true; //go ahead and bring up completion box for XML tags
   int pos;
   s = editIf->textLine(line);
   pos = s.findRev("$",col);
   if (pos != -1)
   {
     s = text(line, pos+1,line,col);
     QRegExp rx("[A-Za-z0-9_]*",false);
     if (rx.exactMatch(s))
     {
       scriptAutoCompletion(line, col, "$");
       goAhead = false;
       handled = true;
     }
   }

   if (goAhead)
   {
     s = editIf->textLine(line);
     pos = s.findRev("<",col);
     if (pos != -1)
     {
       s = text(line, pos+1,line,col);
       QRegExp rx("[A-Za-z0-9_]*",false);
       if (rx.exactMatch(s))
       {
         goAhead = false;
       }
     }
     if (goAhead)
     {
       showCodeCompletions(getTagCompletions(line, col));
       handled = true;
     }
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
  fileWatcher->stopScan();
  if (m_dirty)
  {
    if (url().isLocalFile())
    {
      //check if the file is changed, also by file content. Might help to reduce
      //unwanted warning on NFS
      QFile f(url().path());
      if (f.open(IO_ReadOnly))
      {
        QString content;
        QTextStream stream(&f);
        content = stream.read();
        if (content == editIf->text())
        {
          m_dirty = false;
        }
        f.close();
      }

    }
    if (m_dirty)
    {
      createTempFile();
      DirtyDlg *dlg = new DirtyDlg(url().path(), m_tempFileName, this);
      if (!m_pluginInterface || !(m_pluginInterface->pluginAvailable("kompare")))
      {
        dlg->buttonCompare->setEnabled(false);
        dlg->buttonLoad->setChecked(true);
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
  fileWatcher->startScan();
}

/** Save the document and reset the dirty status. */
void Document::save()
{
  fileWatcher->stopScan();
  m_doc->save();
  m_dirty = false;
  fileWatcher->startScan();
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
void Document::parseVariables()
{
 variableList.clear();
 QString text = editIf->text();
//TODO: Make general for all script languages
 int pos = 0;
 int pos2 = 0;

 QRegExp varRx("\\$+[a-zA-Z_\\x7f-\\xff][a-zA-Z0-9_\\x7f-\\xff]*",false);
 QRegExp inclRx("((?:include|require|include_once|require_once)[\\s]+[^;]*)(;|\\?>|\\n)");
 QString variable;

 while (pos != -1)
 {
   pos = varRx.search(text,pos);
   if (pos != -1)
   {
     variable = varRx.cap();
     pos += variable.length();
     variable.replace(QRegExp("\\$"),"");
     if (!variableList.contains(variable))
     {
       variableList.append(variable);
     }
   }
   pos2 = inclRx.search(text, pos2);
   if (pos2 != -1)
   {
    variable = inclRx.cap(1);
    pos2 += inclRx.cap().length();
    includeList.append(variable);
   }
 }

 variableList.sort();
}

/** No descriptions */
void Document::slotTextChanged()
{
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
      node = parser->nodeAt(line, column, false);
      if (node && (node->tag->type == Tag::XmlTag || node->tag->type == Tag::XmlTagEnd))
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
          bool updateClosing = (currentNode->tag->type == Tag::XmlTag);
          int num = 1;
          QString newName = node->tag->name;
          if (updateClosing)
            node = node->nextSibling();
          else
            node = node->previousSibling();
          while (node)
          {
            if (node->tag->name == currentNode->tag->name)
            {
              num++;
            }
            if ( (updateClosing && node->tag->name == "/"+currentNode->tag->name) ||
                 (!updateClosing && "/"+node->tag->name == currentNode->tag->name) )
            {
              num--;
            }
            if (num == 0)
            {
              reparseEnabled = false;
              node->tag->namePos(bl, bc);
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
              viewCursorIf->setCursorPositionReal(bl, bc);
              baseNode = parser->rebuild(this);
              viewCursorIf->setCursorPositionReal(line, column);
              reparseEnabled = true;
              break;
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

    if (qConfig.instantUpdate)
        quantaApp->getsTab()->slotReparse(this, baseNode , qConfig.expandLevel);
}

/** Returns list of values for attribute */
QStringList* Document::tagAttributeValues(const QString& dtdName, const QString& tag, const QString &attribute)
{
  QStringList *values = 0L;

  DTDStruct* dtd = dtds->find(dtdName.lower());
  if (dtd)
  {
    QString searchForAttr = (dtd->caseSensitive) ? attribute : attribute.upper();
    AttributeList* attrs = QuantaCommon::tagAttributes(dtdName, tag);
    Attribute *attr;
    if (attrs)
    {
      for ( attr = attrs->first(); attr; attr = attrs->next() )
      {
        QString attrName = (dtd->caseSensitive) ? attr->name : attr->name.upper();        if (attrName == searchForAttr)
        {
          if (attr->type == "url") {
            Project *project = quantaApp->project();
            if (project->hasProject())
            {
              values = new QStringList(project->fileNameList(true).toStringList());
              values->append("mailto:" + project->email);
            } else
            {
              QDir dir = QDir(url().directory());
              values = new QStringList(dir.entryList());
            }
            break;
          } else {
            values = &attr->values;
            break;
          }
        }
      }
    }
  }
  return values;
}

#include "document.moc"
