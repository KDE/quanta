/***************************************************************************
                          document.cpp  -  description
                             -------------------
    begin                : Tue Jun 6 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
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

//QT includes
#include <qfile.h>
#include <qfileinfo.h>
#include <qtextstream.h>
#include <qregexp.h>

// KDE includes
#include <kapp.h>
#include <kwin.h>
#include <klocale.h>
#include <kaction.h>
#include <kdialogbase.h>
#include <kiconloader.h>
#include <kmessagebox.h>
#include <ktempfile.h>
#include <kregexp.h>

#include <ktexteditor/cursorinterface.h>
#include <ktexteditor/clipboardinterface.h>
#include <ktexteditor/configinterface.h>
#include <ktexteditor/wordwrapinterface.h>

#include "parser/tag.h"
#include "quantacommon.h"
#include "document.h"
#include "resource.h"

#include <cctype>

Document::Document(const QString& basePath, KTextEditor::Document *doc, QWidget *parent,
                   const char *name, WFlags f ) : QWidget(parent, name, f)
{
  busy    = false;
  oldstat = false;
  _doc = doc;
  _view = (KTextEditor::View *)_doc->createView(this, 0L);
  int w = parent->width() -5 ;
  int h = parent->height() - 35;
   _view->resize(w,h);
//  _view->setGeometry(parent->geometry());

  kate_doc = dynamic_cast<Kate::Document*>(_doc);
  kate_view = dynamic_cast<Kate::View*>(_view);

  editIf = dynamic_cast<KTextEditor::EditInterface *>(_doc);
  selectionIf = dynamic_cast<KTextEditor::SelectionInterface *>(_doc);
  viewCursorIf = dynamic_cast<KTextEditor::ViewCursorInterface *>(_view);
  codeCompletionIf = dynamic_cast<KTextEditor::CodeCompletionInterface *>(_view);
  this->basePath = basePath;
  tempFile = 0;
  dtdName = "";


  connect( _doc,  SIGNAL(charactersInteractivelyInserted (int ,int ,const QString&)),
           this,  SLOT(slotCharactersInserted(int ,int ,const QString&)) );

  connect( _view, SIGNAL(completionDone(KTextEditor::CompletionEntry)),
           this,  SLOT(  slotCompletionDone(KTextEditor::CompletionEntry)) );

  connect( _view, SIGNAL(filterInsertString(KTextEditor::CompletionEntry*,QString *)),
           this,  SLOT(  slotFilterCompletion(KTextEditor::CompletionEntry*,QString *)) );
}

Document::~Document()
{
}

void Document::setUntitledUrl(QString url)
{
  untitledUrl = url;
}

bool Document::isUntitled()
{
  return (_doc->url().url().isEmpty()) ? true : false;
}

KURL Document::url()
{
  return ( isUntitled() ) ? KURL(untitledUrl) : _doc->url();
}

// kwrite addons

void Document::insertTag(QString s1,QString s2)
{
  QString selection = "";

  if ( selectionIf->hasSelection() )
  {
    selection = selectionIf->selection();
    selectionIf->removeSelectedText();
  }

  insertText(s1+selection);
  insertText(s2, FALSE); // don't adjust cursor, thereby leaving it in the middle of tag
}

/** return a pointet to the Node according to p_line, p_col (or current cursor pos, if both are -1)  */
Node *Document::nodeAt(int p_line, int p_col)
{
  Node *foundNode = 0L;
  if (baseNode)
  {
    uint line;
    uint col;
    if ( (p_line < 0) && (p_col < 0))
    {
      viewCursorIf->cursorPositionReal(&line, &col);
    } else
    {
      line = p_line;
      col = p_col;
    }
    Node *currentNode = baseNode;
    int bLine, bCol, eLine, eCol;
    int foundPos; //-1 before, 0 between, 1 after
    while (currentNode && !foundNode)
    {
      currentNode->tag->beginPos(bLine, bCol);
      currentNode->tag->endPos(eLine, eCol);
      foundPos = QuantaCommon::isBetween(line, col, bLine, bCol, eLine, eCol);
      switch (foundPos)
      {
        case 0: {
                  foundNode = currentNode;
                  break;
                }
        case 1: {
                  if (currentNode->next) currentNode = currentNode->next;
                  else currentNode = currentNode->child;
                  break;
                }
        case -1:{
                  if (currentNode->prev)
                  {
                    currentNode = currentNode->prev->child;
                  }
                  break;
                }
      } //switch
    }  //while
    if (!foundNode)
    {
      KMessageBox::error(this, i18n("Node for current position not found.\n This should never happen."));
    }
  } //if
  return foundNode;
}

/** Return a node Tag accroding to line,col (or current cursor pos if line==col==-1) */
Tag *Document::tagAt(int p_line, int p_col)
{
  uint line;
  uint col;

  if ( (p_line < 0) && (p_col < 0))
  {
    viewCursorIf->cursorPositionReal(&line, &col);
    col++;
  } else
  {
    line = p_line;
    col = p_col;
  }

  DTDStruct* dtd = dtds->find(dtdName);/*= dtds->find(findDTDName(line, 0)); */
  if (!dtd) dtd = dtds->find(defaultDocType);
  QString textLine = editIf->textLine(line);
  int tabWidth = kate_view->tabWidth();

  uint col2=0;
  uint i=0;
  while (i < col)
  {
    if (textLine[col2] == '\t' )
    {
      i = ((i+tabWidth)/tabWidth)*tabWidth;
    }
    else
    {
      i++;
    }
    col2++;
  }

  Tag *tag = 0L;
//  if (!tag) tag = findComment(line, col2);
  if (dtd->family == Xml)
  {
    if (!tag) tag = findXMLTag(line, col2);
    if (!tag) tag = findText(line, col2);
  }
  if (dtd->family == Script)
  {
    if (!tag) tag = findScriptStruct(line, col2);
    if (!tag)
    {
      if (textLine[col2] == '}')
      {
        tag = new Tag();
        tag->setTagPosition(line, col2, line, col2);
        tag->type = Tag::ScriptStructureEnd;
        tag->name = "Structure End";
        tag->setStr("}");
        tag->single = false;
        tag->setWrite(this);
      }
    }
    if (!tag) tag = findScriptText(line, col2);
  }
  return tag;
}

Tag *Document::findScriptText(int line, int col)
{
  int bLine = 0;
  int bCol = 0;
  int eLine = 0;
  int eCol = 0;
  Tag *tag = 0L;
  int origLine = line;
  int pos = -1;
  QString textLine;

  while (line >=0 && pos == -1)
  {
    textLine = editIf->textLine(line);
    if (line == origLine) textLine = textLine.left(col);
    pos = textLine.findRev(QRegExp("\\{|\\}"));
    if (pos == -1) line--;
  }
  if (pos != -1)
  {
    bLine = line;
    bCol = pos + 1;
  }
  line = origLine;
  pos = -1;
  int startCol = col;
//search for the first { or }
  while (line < (int) editIf->numLines() && pos == -1)
  {
    textLine = editIf->textLine(line);
    pos = textLine.find(QRegExp("(\\{|\\})"), startCol);
    startCol = 0;
    if (pos == -1) line++;
  }
  if (pos != -1 && textLine[pos] == '{')  //search back for the first keyword
  {
    //TODO: The keyword shouldn't be hardcoded but read from description.rc
    QRegExp rx("\\b(for|foreach|if|else|elseif|while|do|switch|declare|function)\\b");
    origLine = line;
    int origPos = pos;
    pos = -1;
    while (line > 0 && pos == -1)
    {
      textLine = editIf->textLine(line);
      if (line == origLine) textLine = textLine.left(origPos);
      pos = rx.searchRev(textLine);
      if (pos == -1) line --;
    }
    if (line < bLine || (line == bLine && pos < bCol))
    {
      line = origLine;
      pos = origPos;
    }
  }
  eLine = line;
  if (pos == -1)
  {
   eCol = textLine.length();
   eLine = (line > 0)?line - 1 : 0;
  } else
  {
   eCol = pos-1;
   if (eCol < 0)
   {
    eLine = (eLine >0)?eLine-1:0;
    eCol = editIf->lineLength(eLine);
   }
  }

  QString tagStr = text(bLine, bCol, eLine, eCol);
  QString s = tagStr.stripWhiteSpace();
  if (!s.isEmpty() && s != " ")
  {
    tag = new Tag();
    tag->setTagPosition(bLine, bCol, eLine, eCol);
    tag->type = Tag::Text;
    tag->name = "Text";
    tag->single = false;
    tag->setWrite(this);
    tag->setStr(tagStr);
  }
  return tag;
}

Tag *Document::findScriptStruct(int line, int col)
{
  Tag *tag = 0L;
  int bLine = 0;
  int bCol = 0;
  int eLine = 0;
  int eCol = 0;
  QString textLine;
  int origLine = line;
  int pos = -1;
  int maxLine = editIf->numLines();
  int startCol = col;
  while (line < maxLine && pos == -1)
  {
    textLine = editIf->textLine(line);
    pos = textLine.find(QRegExp("\\{|\\}"), startCol);
    if (pos == -1) line++;
    startCol = 0;
  }
  if (pos == -1 || textLine[pos] == '}') return 0L;
  eLine = line;
  eCol = pos;
//now search back for a keyword
//TODO: The keyword shouldn't be hardcoded but read from description.rc
  QRegExp rx("\\b(for|foreach|if|else|elseif|while|do|switch|declare|function)\\b");
  int savedLine = line;
  pos = -1;
  while (line >=0 && pos == -1)
  {
    textLine = editIf->textLine(line);
    if (line == savedLine) textLine = textLine.left(eCol);
    pos = rx.searchRev(textLine);
    if (pos == -1) line--;
  }
  if (pos == -1) return 0L;
  if (line > origLine || (line == origLine && pos > col))
  {
    return 0L; // the keyword starts AFTER the cursor position
  }
  bLine = line;
  bCol = pos;
  QString tagStr = text(bLine, bCol, eLine, eCol);
  tag = new Tag();
  tag->setTagPosition(bLine, bCol, eLine, eCol);
  tag->type = Tag::ScriptStructureBegin;
  tag->single = false;
  tag->setWrite(this);
  tag->setStr(tagStr);
  tag->name = tagStr.left(tagStr.find("{")).simplifyWhiteSpace();
  return tag;
}

Tag *Document::findXMLTag(int line, int col)
{
  Tag *tag = 0L;
//  QRegExp quotedTextRx("(((\\(?=[\"]))\")*[^\"]*)*");
  int bLine, bCol, eLine, eCol;
  bLine = bCol = eLine = eCol = 0;
  QRegExp xmlTagRx("<([^>]*(\"([^\"]*(<[^\"]*>)+[^\"<]*)*\")*('([^']*(<[^']*>)+[^'<]*)*')*[^>]*)*>","i");
  int sLine = line;
  int sCol = col;
  bool tagFound = false;
  //search backwards
  QString foundText = findRev(xmlTagRx, line, col, bLine, bCol, eLine, eCol);
  if (!foundText.isEmpty())
  {
    if (QuantaCommon::isBetween(line, col, bLine, bCol, eLine, eCol) == 0)
    {
      tagFound = true;
    } else
    {
      sLine = eLine;
      sCol = eCol+1;
    }
  } else
  {
    sLine = sCol = 0;
  }
  //if not found, search forward
  if (!tagFound)
  {
   foundText = find(xmlTagRx, sLine, sCol, bLine, bCol, eLine, eCol);
   if (!foundText.isEmpty() &&
       QuantaCommon::isBetween(line, col, bLine, bCol, eLine, eCol) == 0)
   {
      tagFound = true;
   }
  }

  if (tagFound) //build the Tag object
  {
      tag = new Tag();
      tag->setTagPosition(bLine, bCol,eLine, eCol);
      tag->parse(foundText, this);
      tag->type = Tag::XmlTag;
      if (tag->name[0] == '/') tag->type =  Tag::XmlTagEnd;
      if (tag->name == "!--") tag->type = Tag::Comment;
      if (foundText.right(2) == "/>") tag->single = true;
  }

  return tag;
}

//findXMLTag must be called before
Tag *Document::findText(int line, int col)
{
  int bLine = 0;
  int bCol = 0;
  int eLine = 0;
  int eCol = 0;
  Tag *tag = 0L;
  int t_bLine, t_bCol, t_eLine, t_eCol;
  t_bLine = t_bCol = t_eLine = t_eCol = -1;

  QRegExp xmlTagRx("<([^>]*(\"([^\"]*(<[^\"]*>)+[^\"<]*)*\")*('([^']*(<[^']*>)+[^'<]*)*')*[^>]*)*>","i");

  //search backwards
  QString foundText = findRev(xmlTagRx, line, col, bLine, bCol, eLine, eCol);
  if (!foundText.isEmpty())
  {
    t_bCol = eCol+1;
    t_bLine = eLine;
    if (t_bCol > editIf->lineLength(eLine))
    {
      t_bCol = 0;
      t_bLine++;
    }
  } else
  {
    t_bCol = t_bLine = 0;
  }
  foundText = find(xmlTagRx, line, col, bLine, bCol, eLine, eCol);
  if (!foundText.isEmpty())
  {
    t_eCol = bCol-1;
    t_eLine = bLine;
    if (t_eCol < 0 )
    {
      t_eLine = (t_eLine > 0)?t_eLine -1:0;
      t_eCol = editIf->lineLength(t_eLine);
    }
  } else
  {
    t_eLine = editIf->numLines()-1;
    t_eCol = editIf->lineLength(t_eLine);
  }

  tag = new Tag();
  tag->setTagPosition(t_bLine, t_bCol, t_eLine, t_eCol);
  QString s = text(t_bLine, t_bCol, t_eLine, t_eCol);
  s.replace(QRegExp("\\n"),"");
  s = s.stripWhiteSpace();
  if (s.isEmpty() || s == " ")  //whitespaces are not text
  {
    tag->type = 100;
  } else
  {
    tag->type = Tag::Text;
    tag->setStr(s);
    tag->setWrite(this);
    tag->name = "Text";
  }
 return tag;

/*
  int origLine = line;
  int pos = -1;
  QString textLine;

  while (line >=0 && pos == -1)
  {
    textLine = editIf->textLine(line);
    if (line == origLine) textLine = textLine.left(col+1);
    pos = textLine.findRev(QRegExp("\\<|\\>"));
    if (pos == -1) line--;
  }
  if ( pos != -1 && textLine[pos] == '<')
  {
    return 0L; // > or no < is found
  }
  bLine = (line < 0) ? 0 : line;
  bCol = (pos < 0) ? 0 : pos+1;
  line = origLine;
  pos = -1;
  int maxLines = editIf->numLines();
  while (line < maxLines  && pos == -1)
  {
    textLine = editIf->textLine(line);
    if (line == origLine) pos = textLine.find(QRegExp("(\\<|\\>)"), col);
    else pos = textLine.find(QRegExp("(\\<|\\>)"));
    if (pos == -1) line++;
  }
  if ( pos != -1 && textLine[pos] == '>')
  {
    return 0L;
  }
  eLine = (line >= maxLines) ? maxLines-1 : line ;
  eCol = (pos < 0) ? editIf->lineLength(eLine):pos-1;
  if (eCol < 0)
  {
    eLine = (eLine > 0)? eLine-1:0;
    eCol = editIf->lineLength(eLine);
  }
  if (eLine == bLine && eCol == bCol)
  {
    return 0L;
  }
  QString tagStr = text(bLine, bCol, eLine, eCol);
  QString s = tagStr.stripWhiteSpace();
  if (!s.isEmpty() && s != " ")
  {
    tag = new Tag();
    tag->setTagPosition(bLine, bCol, eLine, eCol);
    tag->setStr(tagStr);
    tag->setWrite(this);
    tag->type = Tag::Text;
    tag->name = "Text";
    tag->single = true;
  }

  return tag; */
}

/** Change the current tag's attributes with those from dict */
void Document::changeCurrentTag( QDict<QString> *dict )
{
  QDictIterator<QString> it( *dict ); // iterator for dict
  QDict<QString> oldAttr(1,false);
/*  Node *node = nodeAt(); //get node at current position
  if (!node || !node->tag) return;
  Tag *tag = node->tag;*/
  Tag *tag = tagAt();
  if (tag)
  {
    QString tagStr = "";
    while ( it.current() )  // for insert new attr
    {
      QString attr = QuantaCommon::attrCase(it.currentKey());
      QString *val = it.current();
      QString attrval;

      if ( val->isEmpty() )  // for checkboxes ( without val) don't print =""
          attrval = QString(" ")+attr;
      else
          attrval = QString(" ")+attr+"=\""+*val+"\"";
      tagStr = attrval + tagStr;
      ++it;
    }
    tagStr = "<"+QuantaCommon::tagCase(tag->name)+tagStr+">";
    int bLine, bCol, eLine, eCol;
    tag->beginPos(bLine,bCol);
    tag->endPos(eLine,eCol);
    editIf->removeText(bLine, bCol, eLine, eCol);
    editIf->insertText(bLine, bCol, tagStr);
    delete tag;
  }
//  parser->update(node); //very important to call
}

// return global( on the desktop ) position of text cursor
QPoint Document::getGlobalCursorPos()
{
  return kate_view->mapToGlobal(viewCursorIf->cursorCoordinates());
}

void Document::insertAttrib(QString attr)
{
//  Node *node = nodeAt(); //get node at current position
//  if (!node || !node->tag) return;
//  Tag *tag = node->tag;
  int line, col;
  Tag *tag = tagAt();
  if (tag)
  {
    tag->endPos(line, col);
    viewCursorIf->setCursorPositionReal( line, col - 1 );
    insertTag( QString(" ") + QuantaCommon::attrCase(attr) + "=\"", QString( "\"" ) );
    delete tag;
  }
//  tag->beginPos(line, col);
//  parser->update(node); //very important to call
//  parser->coutTree(baseNode,4);
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
 selectionIf->removeSelectedText();
 editIf->insertText(line, col, s);

}

void Document::readConfig(KConfig *config)
{
  bool m = _doc->isModified();
 dynamic_cast<KTextEditor::ConfigInterface*>(_doc)->readConfig( config );
  _doc->setModified(m);
}

void Document::writeConfig(KConfig *config)
{
  dynamic_cast<KTextEditor::ConfigInterface*>(_doc)->writeConfig( config );
}

/** No descriptions */
void Document::insertFile(QString fileName)
{
  QFile file(fileName);

  file.open(IO_ReadOnly);

  QTextStream stream( &file );

//  kate_view->insertText(stream.read());
  insertText(stream.read());

  file.close();
}

/** Inserts text at the current cursor position */
void Document::insertText(QString text, bool adjustCursor)
{
  if(text.isEmpty())
    return;

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
}

/** Get the view of the document */
KTextEditor::View* Document::view()
{
  return _view;
}

/** Get the KTextEditor::Document of the document */
KTextEditor::Document* Document::doc()
{
  return _doc;
}


#include "document.moc"
/** Returns true if the document was modified. */
bool Document::isModified()
{
  bool modified = false;
  if ( _doc )	
   modified = _doc->isModified();

  return modified;	
}
/** Sets the modifiedFlag value. */
void Document::setModified(bool flag)
{
  _doc->setModified(flag);
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
 tempFile = new KTempFile();
 tempFile->setAutoDelete(true);
 * (tempFile->textStream()) << editIf->text();

 tempUrl = KURL(QFileInfo(*(tempFile->file())).filePath());
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
 tempFile = new KTempFile();
}
/** No descriptions */
KURL Document::tempURL()
{
 return tempUrl;
}

/** No descriptions */
bool Document::saveIt()
{
 bool modifyStatus = _doc->isModified();
 _doc->save();
 _doc->setModified(modifyStatus);

 return true;   //not used yet
}

/** This will return the current tag name at the given position.
    It will work even if the tag has not been completed yet. An
    empty string will be returned if no tag is found.
*/
QString Document::getTagNameAt( int line, int col )
{
 QString name = "";
 Tag * tag = tagAt(line, col);
 if (tag) name = tag->name;

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
  unsigned int row,col;
  viewCursorIf->cursorPositionReal(&row,&col);
  if (completion.type == "attribute")
  {
    viewCursorIf->setCursorPositionReal(row,col-1);
    showCodeCompletions( getAttributeValueCompletions(completion.userdata, completion.text) );
  }
  if (completion.type == "attributeValue")
  {
    viewCursorIf->setCursorPositionReal(row,col+1);
  }
}

/** This is called when the user selects a completion. We
    can filter this completion to allow more intelligent
    code compeltions
*/
void Document::slotFilterCompletion( KTextEditor::CompletionEntry *completion ,QString *string )
{
  if ( completion->type == "attribute" )
  {
    string->append("=\"\"");
  }
  if (completion->type == "doctypeList")
  {
    QString s = *string;
    string->remove(0, string->length());
    string->append(QuantaCommon::attrCase("public")+" \""+QuantaCommon::getDTDNameFromNickName(s)+"\"");
  }
}

/** Called when a user types in a character. From this we can show possibile
    completions based on what they are trying to input.
*/
void Document::slotCharactersInserted(int line,int column,const QString& string)
{
 if (useAutoCompletion)
 {
  DTDStruct* dtd = dtds->find(findDTDName(line, 0));
  if (dtd->family == Xml)
  {
    xmlAutoCompletion(line, column, string);
  }
  if (dtd->family == Script)
  {
    scriptAutoCompletion(line, column, string);
  }
 }
}

/** Called whenever a user inputs text in an XML type document. */
void Document::xmlAutoCompletion(int line, int column, const QString & string)
{
  QString tag;
  if ( string == ">")
    tag = getTagNameAt( line, column-1 );
  else
    tag = getTagNameAt( line, column );

  if ( tag == "" )  //we are outside of any tag
  {
    if ( string == "<" )  // a tag is started
    {
      //we need to complete a tag name
      showCodeCompletions( getTagCompletions(line, column) );
    }
    else if ( string == "&")
         {
          //complete character codes
          //showCodeCompletions( getCharacterCompletions() );
         }
  }
  else  // we are inside of a tag
  {
    if ( string == ">" && tag[0] != '/' &&
         QuantaCommon::isKnownTag(dtdName, tag) &&
         !QuantaCommon::isSingleTag(dtdName, tag) &&
         ( QuantaCommon::isOptionalTag(dtdName, tag) || useCloseTag ))
    {
      //add closing tag if wanted
      column++;
      editIf->insertText(line, column, "</" + tag + ">");
      viewCursorIf->setCursorPositionReal( line, column );
    }
    else if ( string == " " )
         {
          //suggest attribute completions
          showCodeCompletions( getAttributeCompletions(tag) );
         }
         else if ( string == "\"" )
              {
               //we need to find the attribute name
               QString textLine = editIf->textLine(line).left(column-1);
               QString attribute = textLine.mid(textLine.findRev(' ')+1);
               showCodeCompletions( getAttributeValueCompletions(tag, attribute) );
              }
  } // else - we are inside of a tag
}


/** Return a list of possible tag name completions */
QValueList<KTextEditor::CompletionEntry>* Document::getTagCompletions(int line, int col)
{
  QValueList<KTextEditor::CompletionEntry> *completions = new QValueList<KTextEditor::CompletionEntry>();
  KTextEditor::CompletionEntry completion;
  completion.type = "tag";

  DTDStruct* dtd = dtds->find(findDTDName(line, 0));

  if (dtd) {
    QDictIterator<QTag> it(* dtd->tagsList);
    for( ; it.current(); ++it )
    {
      completion.text = QuantaCommon::tagCase( it.current()->name() );
      completions->append( completion );
    }
  }
  
  return completions;
}

/** Return a list of valid attributes for the given tag */
QValueList<KTextEditor::CompletionEntry>* Document::getAttributeCompletions( QString tag )
{
  QValueList<KTextEditor::CompletionEntry> *completions = new QValueList<KTextEditor::CompletionEntry>();
  KTextEditor::CompletionEntry completion;
  completion.type = "attribute";
  completion.userdata = tag;

  if ( QuantaCommon::isKnownTag(dtdName,tag) )
  {
    AttributeList *list = QuantaCommon::tagAttributes(dtdName,tag);
    for (uint i = 0; i < list->count(); i++)
    {
      QString item = list->at(i)->name;
      completion.text = item;
      completions->append( completion );
    }
  }

  if (tag.contains("!doctype",false)) //special case, list all the known document types
  {
    QDictIterator<DTDStruct> it(*dtds);
    for( ; it.current(); ++it )
    {
     completion.type = "doctypeList";
     completion.text = it.current()->nickName;
     completions->append(completion);
    }
  }

  return completions;
}

/** Return a list of valid attribute values for the given tag and attribute */
QValueList<KTextEditor::CompletionEntry>* Document::getAttributeValueCompletions( QString tag, QString attribute )
{
  QValueList<KTextEditor::CompletionEntry> *completions = new QValueList<KTextEditor::CompletionEntry>();

  KTextEditor::CompletionEntry completion;
  completion.type = "attributeValue";
  completion.userdata = tag + "," + attribute;

  QStringList *values = QuantaCommon::tagAttributeValues(dtdName,tag,attribute);
  if (values) {
    for ( QStringList::Iterator it = values->begin(); it != values->end(); ++it ) {
      completion.text = *it;
      completions->append( completion );
    }
  }
  
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
  dtdName = id;
}

/** Find the DTD name for a part of the document. Search all the document if startLine=endLine=-1. */
QString Document::findDTDName(int startLine, int endLine, bool checkCursorPos)
{
 //Do some magic to find the document type
 if ( (startLine == -1) && (endLine == -1) )
 {
   startLine = 0;
   endLine = editIf->numLines();
 }
 int i=startLine;
 int dir = (startLine > endLine)?-1:1;
 int pos = 0;
 QString foundName = "";
 bool endReached;
 uint line, col;
 viewCursorIf->cursorPositionReal(&line,&col);
 do
 {
    QString s = editIf->textLine(i);
    //search for !DOCTYPE definitions
    pos = s.find("!doctype",0,false);
    if (pos != -1) //parse the found !DOCTYPE tag
      {
//        Tag *tag = tagAt(i, pos);
        Tag *tag = findXMLTag(i, pos);
        if (!tag) return foundName;
        s = tag->tagStr();
        pos = s.find("public",0,false);
        if (pos == -1) //if no PUBLIC info, use the word after !DOCTYPE as the doc.type
        {
          foundName = tag->attribute(0);
        } else
        {             //use the quoted string after PUBLIC as doc. type
          pos = s.find("\"", pos+1);
          if (pos !=-1)
          {
            int endPos = s.find("\"",pos+1);
            foundName = s.mid(pos+1, endPos-pos-1);
          }
        }
        delete tag;
      }
    //search for script type definitions
    QDictIterator<DTDStruct> it(*dtds);
    for( ; it.current(); ++it )
    {
      DTDStruct *dtd = it.current();

      if (!dtd->scriptName.isEmpty()) //it may be defined as <script language="name">
      {
        pos = s.find("<script",false);
        if ( (pos != -1) && ( ((int)line != i) || (pos < (int) col) )) //script tag found
        {
//          Tag *tag2 = tagAt(i, pos+2);
          Tag *tag2 = findXMLTag(i, pos+2);
          QString s2 = tag2->attributeValue("language");
          if (s2.lower() == dtd->scriptName)
          {
            //now check if we are after the closing tag
            bool afterClosingTag = false;
            int j = i;
            while (j != startLine - dir)
            {
              s2 = editIf->textLine(j);
              pos = s2.find("</script",false);
              if ( (pos != -1) && ( ((int)line != j) || (pos < (int)col) ) )
              {
               afterClosingTag = true;
               break;
              }
              j -= dir;
            }
            if (!afterClosingTag) foundName = dtd->name;
          } //if this script was found
        } //if script tag was found
      } //if it has a scriptName

     for ( QStringList::Iterator tagIt = dtd->startTags.begin(); tagIt != dtd->startTags.end(); ++tagIt )
     {
       pos = s.find(*tagIt,false);
       if ( (pos != -1) && ( !checkCursorPos || ((int)line != i) || (pos < (int)col) )) //start tag found
       {
         //now check if we are after the closing tag
         bool afterClosingTag = false;
         int j = i;
         while (j != startLine - dir)
         {
           QString s2 = editIf->textLine(j);
           pos = s2.find(dtd->endTags[dtd->startTags.findIndex(*tagIt)],false);
           if ( (pos != -1) && (!checkCursorPos || ((int)line != j) || (pos < (int)col) ) )
           {
             afterClosingTag = true;
             break;
           }
           j -= dir;
         }
         if (!afterClosingTag) foundName = dtd->name;
        } //if start tag was found
     }
    } //dtd->startTags interation (for cycle)

   i += dir;
   endReached = (dir < 0)?(i < endLine):(i > endLine);
 } while ((foundName.isEmpty()) && (!endReached));

 if (foundName.isEmpty()) foundName = dtdName;
 return foundName;
}

/** Called whenever a user inputs text in a script type document. */
void Document::scriptAutoCompletion(int line, int column, const QString & string)
{
 //TODO: Implement this.
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
 QString t = editIf->textLine(bLine)+"\n";
 if (bLine == eLine)
 {
   return t.mid(bCol, eCol-bCol +1);
 }
 t.remove(0, bCol);
 for (int i = bLine+1; i < eLine ; i++)
 {
   t = t+editIf->textLine(i)+"\n";
 }
 t = t+editIf->textLine(eLine).left(eCol+1);
 return t;
}

QString Document::find(QRegExp& rx, int sLine, int sCol, int& fbLine, int&fbCol, int &feLine, int&feCol)
{
 int maxLine = editIf->numLines();
 int lastLineLength = editIf->lineLength(maxLine);
 QString textToSearch = text(sLine, sCol, maxLine, lastLineLength);
 int pos = rx.search(textToSearch,0);
 QString foundText = "";
 if (pos != -1)
 {
   foundText = rx.cap();
   QString s = textToSearch.left(pos);
   int linesUntilFound = s.contains("\n");
   s = s.remove(0,s.findRev("\n")+1);
   fbCol = s.length();
   int linesInFound = foundText.contains("\n");
   s = foundText;
   s = s.remove(0,s.findRev("\n")+1);
   feCol = s.length()-1;
   fbLine = sLine + linesUntilFound;
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
   s = text(fbLine, fbCol, feLine, feCol);
   if (s != foundText) //debug, error
   {
     KMessageBox::error(this,"Found: "+foundText+"\nRead: "+s);
   }
 }

 return foundText;
}

QString Document::findRev(QRegExp& rx, int sLine, int sCol, int& fbLine, int&fbCol, int &feLine, int&feCol)
{
 QString textToSearch = text(0,0, sLine, sCol);
 int pos = rx.searchRev(textToSearch);
 QString foundText = "";
 if (pos != -1)
 {
   foundText = rx.cap();
   QString s = textToSearch.left(pos);
   int linesUntilFound = s.contains("\n");
   fbLine = linesUntilFound;
   s = s.remove(0,s.findRev("\n")+1);
   fbCol = s.length();
   int linesInFound = foundText.contains("\n");
   s = foundText;
   s = s.remove(0,s.findRev("\n")+1);
   feCol = s.length()-1;
   feLine = fbLine + linesInFound;
   if (linesInFound == 0)
   {
       feCol = feCol + fbCol;
   }
   if (fbCol < 0) fbCol = 0;
   if (feCol < 0) feCol = 0;
   s = text(fbLine, fbCol, feLine, feCol);
   if (s != foundText) //debug, error
   {
     KMessageBox::error(this,"FindRev\nFound: "+foundText+"\nRead: "+s);
   }
 }

 return foundText;
}
