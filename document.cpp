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

QString Document::findBeginOfTag(QString t, int line, int col, int &beginLine, int &beginCol, QString beginStr,QString endStr)
{
  if (line < 0 ) return "";
  QString textLine = editIf->textLine(line);
  QString s = textLine+t;

  if ( col ) s = s.left(col);
  if ( col > (int)s.length() )
  {
    col = s.length();
  }
  if (col < 0)
  {
   col = s.length() - t.length() - 1;
  }

  while ( col > 0)
  {
    if ( s.mid(col-endStr.length(), endStr.length()) == endStr ) return "";
    if ( s.mid(col-beginStr.length(), beginStr.length()) == beginStr )  // found !!!
    {
      s.remove(0,col-1);
      beginLine = line;
      beginCol = col - 1;
      return s;
    }
    col--;
  }
  return findBeginOfTag( s, line-1, -1, beginLine, beginCol, beginStr, endStr);
}

QString Document::findEndOfTag( QString t, int line, int col, int &endLine, int &endCol, QString beginStr,QString endStr)
{
  int oldlen = t.length();
  if (line >= (int) editIf->numLines())
  {
   return "";
  }

  QString textLine = editIf->textLine(line);
  QString s = t + textLine;

  if ( col ) s = s.remove(0,col);

  int len = s.length(); // len of all righttag

  int i=0;
  while ( i < len )
  {
    if ( !beginStr.isEmpty() && s.mid(i-beginStr.length()+1, beginStr.length()) == beginStr ) return QString("");
    if ( s.mid(i-endStr.length()+1, endStr.length()) == endStr )  // found !!!
    {
      endCol = col+i-oldlen + 1;
      endLine = line;
      return s.left(i+1);
    }
    i++;
  }
  return findEndOfTag( s, line+1, 0, endLine, endCol, beginStr, endStr);
}


/** Return a node Tag accroding to line,col (or current cursor pos if line==col==-1) */
Tag *Document::currentTag(int p_line, int p_col)
{
  uint line;
  uint col;

  if ( (p_line < 0) && (p_col < 0))
  {
    viewCursorIf->cursorPosition(&line, &col);
  } else
  {
    line = p_line;
    col = p_col;
  }

  DTDStruct* dtd = dtds->find(findDTDName(line, 0));
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
    if (!tag) tag = findScriptStructBegin(line, col2);
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
    pos = textLine.find(QRegExp("\\{|\\}"));
    if (pos == -1) line--;
  }
  if (pos != -1)
  {
    bLine = line;
    bCol = pos + 1;
  }
  line = origLine;
  pos = -1;
  while (line < (int) editIf->numLines() && pos == -1)
  {
    textLine = editIf->textLine(line);
    if (line == origLine) pos = textLine.find(QRegExp("(\\{|\\})"), col);
    else pos = textLine.find(QRegExp("(\\{|\\})"));
    if (pos == -1) line++;
  }
  eLine = line;
  if (pos == -1)
  {
   eCol = textLine.length();
  } else
  {
   eCol = pos-1;
   if (eCol < 0)
   {
    eLine = (eLine >0)?eLine--:0;
    eCol = editIf->lineLength(eLine);
   }
  }

  tag = new Tag();
  tag->setTagPosition(bLine, bCol, eLine, eCol);
  tag->type = Tag::Text;
  tag->name = "Text";
  tag->single = false;
  tag->setWrite(this);
  tag->setStr(text(bLine, bCol, eLine, eCol));
  return tag;
}

Tag *Document::findScriptStructBegin(int line, int col)
{
  Tag *tag = 0L;
  int origLine = line;
  QString textLine = editIf->textLine(line);
  if (textLine[col] == '{')
  {
    tag = new Tag();
    tag->setTagPosition(line, col, line, col);
    tag->type = Tag::ScriptStructureBegin;
    tag->single = false;
    tag->setWrite(this);
    QRegExp rx("\\b(for|foreach|if|else|elseif|while|do|switch|declare)\\b");
    int pos = -1;
    while (line >=0 && pos == -1)
    {
      textLine = editIf->textLine(line);
      if (line == origLine) textLine = textLine.left(col);
      pos = rx.searchRev(textLine);
      if (pos == -1)
      {
        line--;
      }
      else
      {
        QString s = text(line, pos, origLine, col);
        tag->setStr(s);
        if (s.find("}") == -1)
        {
          if (rx.search(s) != -1) tag->name = "Structure: "+rx.cap();
        }
      }
    }
  }

  return tag;
}

Tag *Document::findXMLTag(int line, int col)
{
  int tagBeginLine = 0;
  int tagBeginCol = 0;
  int tagEndLine = 0;
  int tagEndCol = 0;
  QString begTag = findBeginOfTag( QString(""), line, col, tagBeginLine, tagBeginCol); // find begin
  QString endTag = findEndOfTag(   QString(""), line, col, tagEndLine, tagEndCol); // end
  QString tagStr = "";
  Tag *tag = 0L;
  if ( ! begTag.isEmpty() && !endTag.isEmpty()
       && tagStr.startsWith("<") && tagStr.endsWith(">"))
  {
    tagStr = begTag+endTag;
    tag = new Tag();
    tag->setTagPosition(tagBeginLine, tagBeginCol, tagEndLine, tagEndCol);
    tag->parse(tagStr, this);
    tag->type = Tag::XmlTag;
    if (tag->name[0] == '/') tag->type =  Tag::XmlTagEnd;
    if (tag->name == "!--") tag->type = Tag::Comment;
    tag->single = false;
    if (tagStr.right(2) == "/>") tag->single = true;
  }
 return tag;
}

Tag *Document::findText(int line, int col)
{
  int tagBeginLine = 0;
  int tagBeginCol = 0;
  int tagEndLine = 0;
  int tagEndCol = 0;
  Tag *tag = 0L;
  QString begTag = findBeginOfTag( QString(""), line, col, tagBeginLine, tagBeginCol, ">","<");
  QString endTag = findEndOfTag(   QString(""), line, col, tagEndLine, tagEndCol, "", "<");
  QString tagStr = begTag.right(begTag.length()-1)+endTag.left(endTag.length()-1);
  tagStr = tagStr.stripWhiteSpace();
  if (!tagStr.isEmpty() && tagStr != " ")
  {
      tag = new Tag();
      tag->setStr(tagStr);
      tag->setTagPosition(tagBeginLine, tagBeginCol+1, tagEndLine, tagEndCol-1);
      tag->type = Tag::Text;
      tag->setWrite(this);
      tag->name = "Text";
  }
 return tag;
}

Tag *Document::findComment(int line, int col)
{
  int tagBeginLine = 0;
  int tagBeginCol = 0;
  int tagEndLine = 0;
  int tagEndCol = 0;
  Tag *tag = 0L;
  QString begTag = findBeginOfTag( QString(""), line, col, tagBeginLine, tagBeginCol, "<!--","-->");
  QString endTag = findEndOfTag(   QString(""), line, col, tagEndLine, tagEndCol, "<!--","-->");
  QString tagStr = begTag+endTag;
  if (!tagStr.isEmpty())
  {
      tag = new Tag();
      tag->setStr(tagStr);
      tag->setTagPosition(tagBeginLine, tagBeginCol+1, tagEndLine, tagEndCol-1);
      tag->type = Tag::Comment;
  }
 return tag;
}

/** Change the current tag's attributes with those from dict */
void Document::changeCurrentTag( QDict<QString> *dict )
{
  QDictIterator<QString> it( *dict ); // iterator for dict
  QDict<QString> oldAttr(1,false);
  Tag *tag = currentTag();
  if (!tag) return;
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

// return global( on the desktop ) position of text cursor
QPoint Document::getGlobalCursorPos()
{
  return kate_view->mapToGlobal(viewCursorIf->cursorCoordinates());
}

void Document::insertAttrib(QString attr)
{
  Tag * tag = currentTag();
  if (tag)
  {
   int line, col;
   tag->endPos(line, col);
   viewCursorIf->setCursorPosition( line, col - 1 );
   insertTag( QString(" ") + QuantaCommon::attrCase(attr) + "=\"", QString( "\"" ) );
   delete tag;
  }
}

/**  */
void Document::selectText(int x1, int y1, int x2, int y2 )
{
   selectionIf->setSelection(x1, y1, x2, y2);
}


void Document::replaceSelected(QString s)
{
 unsigned int line, col;

 viewCursorIf->cursorPosition(&line, &col);
 selectionIf->removeSelectedText();
 editIf->insertText(line, col, s);

}


/**  */   /*
int Document::pos2y( int pos )
{
	QString s = editIf->text();
	int endLineCount = 0;
	if ( pos<0 ) pos = 0;
	
	for (int i=0; i<=pos && !s[i].isNull(); i++)
		if (s[i]=='\n')
			endLineCount++;
	return endLineCount;
}

int Document::pos2x( int pos )
{
  QString s = editIf->text();
	int i;
	if ( pos<0 ) pos = 0;
	for (i=pos; s[i]!='\n' && i; i--);
	return pos-i;
}

int Document::xy2pos( int x, int y )
{
  QString s = editIf->text();
  int pos = 0;
  QStringList slist = QStringList::split('\n',s,true);

  if ( y > (int) slist.count() )
  	y = slist.count();

  for ( int i=0; i<y; i++ )
  	 pos += slist[i].length()+1;

  int len = slist[y].length();
  if ( len>x )
  	pos+=x;
  else
  	pos+=len;

	return (pos);
}           */

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
      viewCursorIf->setCursorPosition( line, column );
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
 viewCursorIf->cursorPosition(&line,&col);
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
 QString t = editIf->textLine(bLine);
 if (bLine == eLine)
 {
   return t.mid(bCol, eCol-bCol);
 }
 t.remove(0, bCol);
 for (int i = bLine+1; i < eLine ; i++)
 {
   t = t+editIf->textLine(i);
 }
 t = t+editIf->textLine(eLine).left(eCol+1);
 return t;
}
