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
//#include <qregexp.h>

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

#include "quantacommon.h"
#include "document.h"
#include "resource.h"

#include <cctype>

extern QDict<AttributeList> *tagsDict;
extern uint tagsCase, attrsCase;

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

//	unsigned int line,col; // cursor position
//  viewCursorIf->cursorPositionReal(&line,&col);
/*  editIf->insertText(line, col, s1+selection+s2);

   KTextEditor::WordWrapInterface *ww = dynamic_cast<KTextEditor::WordWrapInterface*>(_doc);
   if ( (ww->wordWrap()) && ((col+s1.length()) > ww->wordWrapAt()) )
   {
     line++;
     col = col+s1.length()-ww->wordWrapAt()+1;
   } else
   {
     col = col+s1.length();
   }
  viewCursorIf->setCursorPosition(line,col);
  */

// kate_view->insertText(s1+selection+s2);
// viewCursorIf->setCursorPosition(pos2y(line),pos2x(col));
/* for (unsigned int i=0; i < s2.length(); i++)
   kate_view->cursorLeft();*/

  insertText(s1+selection);
  insertText(s2, FALSE); // don't adjust cursor, thereby leaving it in the middle of tag
}

//FIXME: This method can go away
QString Document::getLine(int y)
{
  return editIf->textLine(y);
}

QString Document::findBeginOfTag(QString t, int x, int y)
{
  QString s;
  QString line = getLine(y);

  if ( !line.isNull() )
     s = QString(line)+" "+t;
   else
     return QString("");

  if ( x )   s = s.left(x);
  if ( x<=0 || x>(int)s.length() ) x = s.length();

  while ( x ) {
    if ( s[x-1] == QChar('>') ) return "";
    if ( s[x-1] == QChar('<') ) { // found !!!
      s.remove(0,x-1);
      tagBeginX = x;
      tagBeginY = y;
      return s;
    }
    x--;
  }
  return findBeginOfTag( s, 0, y-1);
}

QString Document::findEndOfTag( QString t, int x, int y)
{
  int oldlen = t.length();
  QString s;
  QString line = getLine(y);

  if ( !line.isNull() )
    if ( !t.isEmpty() )
    	s = t+" "+QString(line);
    else s = QString(line);
  else
      return QString("");

  if ( x ) s = s.remove(0,x);

  int len = s.length(); // len of all righttag

  int i=0;
  while ( i<len ) {
    if ( s[i] == QChar('<') ) return "";
    if ( s[i] == QChar('>') ) { // found !!!
      tagEndX = x+i-oldlen;
      tagEndY = y;
      return s.left(i+1);
    }
    i++;
  }
  return findEndOfTag( s, 0, y+1);
}

void Document::parseTagAttr( QString t, int &x, int &y)
{
  QString tagname;
  int i = tagAttrNum;

  while ( t[x].isSpace() ) x++;

  if ( t[x]=='>' || x>= (int)t.length() ) return;

  int begin = x;

  tagAttr[i].x  = x;
  tagAttr[i].endx = 0;
  tagAttr[i].y  = y;
  tagAttr[i].textlen  = 0;
  tagAttr[i].valuelen = 0;
  tagAttr[i].quotes   = false;

  int len = 0;

  while ( !t[x].isSpace() && t[x]!='=' && x<(int)t.length() && t[x]!='>')
    { len++;x++; }

  tagAttr[i].textlen = len;
  tagAttr[i].endx = x;

  tagAttr[i].text = t.mid( begin, len);

  tagAttrNum++;

	while ( t[x].isSpace() ) x++;
	
  if ( t[x]!='=' || x>=(int)t.length() ) return;

  x++;

  while ( t[x].isSpace() ) x++;

  if ( t[x]=='>' || x>=(int)t.length() ) return;

  if ( t[x]=='\"' ) {
    tagAttr[i].quotes = true;
    x++;
  }
  begin = x;

  len = 0;
  while ((( !t[x].isSpace() && t[x]!='>') || tagAttr[i].quotes ) &&
           t[x]!='\"' && x<(int)t.length() )
    { len++;x++; }

  if ( t[x]=='\"' ) x++;

  tagAttr[i].valuelen = len;
  tagAttr[i].endx = x;

  tagAttr[i].value = t.mid( begin, len);
}

void Document::parseTag()
{
  tagAttrNum = 0;
  int x = tagBeginX;
  int y = tagBeginY;

  QString t = getLine(y);

  while ( t[x] != '>' && tagAttrNum < 50)
  {
    if ( t.isNull() )
    	break;
    	
    parseTagAttr( t, x, y);

    if ( x>=(int)t.length() )
    {
      t = getLine(++y);
      x = 0;
    }
  }
}

QString Document::getTagAttr(int i)
{
  QString attr;
  if ( tagAttrNum ) {
    attr = tagAttr[i].text;
  } else attr = "";
  return attr;
}

QString Document::getTagAttrValue(int i)
{
  QString val;
  if ( tagAttrNum && tagAttr[i].valuelen ) {
    val = tagAttr[i].value;
  } else val = "";
  return val;
}

#include <cctype>

/** return qstring with current tag for parse */
QString Document::currentTag()
{
  tagAttrNum = 0;

  unsigned int y;// = currentLine();
  unsigned int ox;// = currentColumn(); // need to reorganise ;)

  viewCursorIf->cursorPosition(&y, &ox);


  QString t = getLine(y);
  int x=0;
  unsigned int i=0;

  int tab = kate_view->tabWidth();

  while (i<ox)
  {
    if (t[x] == '\t' ) i = ((i+tab)/tab)*tab;
    else               i++;
    x++;
  }

  int origTagBeginY = tagBeginY;
  int origTagBeginX = tagBeginX;
  int origTagEndY = tagEndY;
  int origTagEndX = tagEndX;

  QString begTag = findBeginOfTag( QString(""), x, y); // find begin
  QString endTag = findEndOfTag(   QString(""), x, y); // end

  if ( begTag.isEmpty() || endTag.isEmpty() ) {
    tagBeginY = origTagBeginY;
    tagBeginX = origTagBeginX;
    tagEndY = origTagEndY;
    tagEndX = origTagEndX;
    return QString("");
  } else {
    parseTag(); // if have tag parse it
    return begTag+endTag;
  }
}

void Document::changeCurrentTag( QDict<QString> *dict )
{
  QDictIterator<QString> it( *dict ); // iterator for dict
  QDict<QString> oldAttr(1,false);

  for ( int i=1; i<tagAttrNum; i++ )
    oldAttr.insert( getTagAttr(i), new QString(getTagAttrValue(i)) );

  while ( it.current() ) { // for insert new attr

    QString attr = QuantaCommon::attrCase(it.currentKey());
    QString *val = it.current();

    if ( ! oldAttr.find(attr) ) // insert this attr. in end of tag
    {
      QString attrval;

      if ( val->isEmpty() )  // for checkboxes ( without val) don't print =""
        attrval = QString(" ")+attr;
      else
        attrval = QString(" ")+attr+"=\""+*val+"\"";

      // insert attribute
      editIf->insertText(tagEndY, tagEndX, attrval);
    }

    ++it;
  }

  for ( int i=tagAttrNum-1; i>=0; i--)
  {

    QString attr;

    if ( i )
      attr = QuantaCommon::attrCase( getTagAttr(i) );
    else
      attr = QuantaCommon::tagCase( getTagAttr(i) );

    QString *val   = dict->find(attr);

    int x1 = tagAttr[i].x;
    int x2 = tagAttr[i].endx;
    int y = tagAttr[i].y;

    if ( val ) // change attr
    {
      QString t;

      t = attr+"=\""+*val+"\"";

      if ( val->isEmpty() )  // for checkboxes ( without val) don't print =""
        t = attr;

      // replace attribut on new value
      editIf->removeText(y, x1, y, x2);
      editIf->insertText(y, x1, t);
    }
    else {
      if ( i ) { // remove attr
        // delete attribute
        editIf->removeText(y, x1-1, y, x2);
      } else { // insert tag name
        editIf->removeText(y, x1, y, x2);
        editIf->insertText(y, x1, attr);
      }
    }
  }
}

// return global( on the desktop ) position of text cursor
QPoint Document::getGlobalCursorPos()
{
  return kate_view->mapToGlobal(viewCursorIf->cursorCoordinates());
}

void Document::insertAttrib(QString attr)
{
   viewCursorIf->setCursorPosition( tagEndY, tagEndX );
   insertTag( QString(" ") + QuantaCommon::attrCase(attr) + "=\"", QString( "\"" ) );
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


/**  */
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

  viewCursorIf->cursorPosition(&line, &col);
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
        if(wordLength > wordWrapAt)
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
    viewCursorIf->setCursorPosition(line, col);
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
  if ( _doc )	
   return _doc->isModified();
  else
   return false;	  
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
  if ( line == -1 ) return "";

  QString text = editIf->textLine( line );

  if ( col == -1 ) col = text.length();

  int tagBegin = text.findRev( '<', col );
  int previousTagEnd = text.findRev( '>', col);

  if ( tagBegin != -1  && tagBegin > previousTagEnd ) {
    int tagEnd = text.find( '>', col );
    int nextTagBegin = text.find( '>', col);
    int firstSpace = text.find( ' ', tagBegin);

    if ( tagEnd != -1 && ( tagEnd < firstSpace || firstSpace == -1 ) && ( tagEnd < nextTagBegin || nextTagBegin == -1 ) ) {
      text.remove( tagEnd, text.length() );
    } else if ( firstSpace != -1 ) {
      text.remove( firstSpace, text.length() );
    } else {
      text.remove( col+1, text.length() );
    }

    text.remove( 0, tagBegin+1 );
    return text;

  } else if ( previousTagEnd != -1 ) {
    return "";

  } else {
    return getTagNameAt( line-1, -1 );
  }
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
  if (completion.type == "attribute") {
    unsigned int row,col;
    viewCursorIf->cursorPositionReal(&row,&col);
    viewCursorIf->setCursorPositionReal(row,col-1);
    showCodeCompletions( getAttributeValueCompletions(completion.userdata, completion.text) );
  } else if (completion.type == "attributeValue") {
    unsigned int row,col;
    viewCursorIf->cursorPositionReal(&row,&col);
    viewCursorIf->setCursorPositionReal(row,col+1);
  }
}

/** This is called when the user selects a completion. We
    can filter this completion to allow more intelligent
    code compeltions
*/
void Document::slotFilterCompletion( KTextEditor::CompletionEntry *completion ,QString *string ) {
  if ( completion->type == "attribute" ) {
    string->append("=\"\"");
  }
}

/** Called when a user types in a character. From this we can show possibile
    completions based on what they are trying to input.
*/
void Document::slotCharactersInserted(int line,int column,const QString& string)
{
 if (useAutoCompletion)
 {
  QString tag;
  if ( string == ">")
    tag = getTagNameAt( line, column-1 );
  else
    tag = getTagNameAt( line, column );

  if ( tag == "" ) {
    if ( string == "<" ) {
      //we need to complete a tag name
      showCodeCompletions( getTagCompletions() );
    } else if ( string == "&") {
      //complete character codes
      //showCodeCompletions( getCharacterCompletions() );
    }
  } else {
    if ( string == ">" && tag[0] != '/' && tagsList->find(tag.upper()) && singleTags->find(tag.upper()) == -1 && ( optionalTags->find(tag.upper()) != -1 || useCloseTag )) {
      //add closing tag if wanted
      column++;
      editIf->insertText(line, column, "</" + tag + ">");
      viewCursorIf->setCursorPosition( line, column );
    } else if ( string == " " ) {
      //suggest attribute completions
      showCodeCompletions( getAttributeCompletions(tag) );
    } else if ( string == "\"" ) {
      //we need to find the attribute name
      //QString attribute = "";
      //showCodeCompletions( getAttributeValueCompletions(tag, attribute) );
    }
  }
 }
}

/** Return a list of possible tag name completions */
QValueList<KTextEditor::CompletionEntry>* Document::getTagCompletions()
{
  QValueList<KTextEditor::CompletionEntry> *completions = new QValueList<KTextEditor::CompletionEntry>();
  KTextEditor::CompletionEntry completion;
  completion.type = "tag";

  QDictIterator<QString> it(*tagsList);
  for( ; it.current(); ++it )
  {
    completion.text = QuantaCommon::tagCase( * it.current() );
    completions->append( completion );
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

  if ( tagsList->find( tag.upper()) )
  {
    AttributeList *list = tagsDict->find( tag );
    for (uint i = 0; i < list->count(); i++)
    {
      QString item = list->at(i)->name;
      completion.text = item;
      completions->append( completion );
    }
  }

  return completions;
}

/** Return a list of valid attribute values for the given tag and attribute */
QValueList<KTextEditor::CompletionEntry>* Document::getAttributeValueCompletions( QString tag, QString attribute )
{
  QValueList<KTextEditor::CompletionEntry> *completions = new QValueList<KTextEditor::CompletionEntry>();

  //need to get this information from something

  return completions;
}

/** Return a list of chatacter completions (like &nbsp; ...) */
QValueList<KTextEditor::CompletionEntry>* Document::getCharacterCompletions()
{
  QValueList<KTextEditor::CompletionEntry> *completions = new QValueList<KTextEditor::CompletionEntry>();

  //need to get this information from something

  return completions;
}
