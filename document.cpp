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

#include "document.h"


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
  this->basePath = basePath;
  tempFile = 0;
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
/*
	unsigned int line,col; // cursor position
  viewCursorIf->cursorPosition(&line,&col);
  editIf->insertText(line, col, s1+selection+s2);

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

 kate_view->insertText(s1+selection+s2);
 for (unsigned int i=0; i < s2.length(); i++)
   kate_view->cursorLeft();
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
  

  QString begTag = findBeginOfTag( QString(""), x, y); // find begin
  QString endTag = findEndOfTag(   QString(""), x, y); // end

  QString tag = begTag+endTag;

  //debug( "tag : "+tag );

  if ( !tag.isEmpty() ) parseTag(); // if have tag parse it

  return tag;
}

void Document::changeCurrentTag( QDict<QString> *dict )
{          /*
  QDictIterator<QString> it( *dict ); // iterator for dict
  QDict<QString> oldAttr(1,false);

  VConfig c;
  kWriteView->getVConfig( c);
  kWriteDoc ->recordStart( c, KWActionGroup::ugNone ); // start action

  for ( int i=1; i<tagAttrNum; i++ )
    oldAttr.insert( getTagAttr(i), new QString(getTagAttrValue(i)) );

  while ( it.current() ) { // for insert new attr

    QString attr = attrCase(it.currentKey());
    QString *val = it.current();

    if ( ! oldAttr.find(attr) ) // insert this attr. in end of tag
    {
      QString value = *val;

      QString attrval;

      attrval = QString(" ")+attr+"=\""+*val+"\"";

      if ( val->isEmpty() )  // for checkboxes ( without val) don't print =""
        attrval = QString(" ")+attr;

      PointStruc cursor;
      cursor.x = tagEndX;
      cursor.y = tagEndY;
      // insert attribut
      kWriteDoc->recordReplace( cursor, 0, attrval );
    }

    ++it;
  }

  for ( int i=tagAttrNum-1; i>=0; i--)
  {

    QString attr;
    if ( i )
      attr = attrCase( getTagAttr(i) );
    else
      attr = tagCase( getTagAttr(i) );

    QString *val   = dict->find(attr);

    int x1 = tagAttr[i].x;
    int x2 = tagAttr[i].endx;

    PointStruc cursor;
    cursor.x = x1;
    cursor.y = tagAttr[i].y;

    if ( val ) // change attr
    {
      QString t;

      t = attr+"=\""+*val+"\"";

      if ( val->isEmpty() )  // for checkboxes ( without val) don't print =""
        t = attr;

      // replace attribut on new value
      kWriteDoc->recordReplace( cursor, x2-x1, t );
    }
    else {
      if ( i ) { // remove attr
        cursor.x -= 1;
        // delete attribut
        kWriteDoc->recordReplace( cursor, x2-x1+1, QString("") );
      } else { // insert tag name
        kWriteDoc->recordReplace( cursor, x2-x1, attr );
      }
    }
  }
  // end of action
  kWriteDoc->recordEnd( c );

//  kWriteDoc->updateLines( tagBeginY, tagEndY, 0 );
  kWriteView->repaint();                            */
}

// return global( on the desktop ) position of text cursor
QPoint Document::getGlobalCursorPos()
{
/*  int h, y, x;

  h = kWriteDoc->fontHeight;
  y = h*kWriteView->cursor.y - kWriteView->yPos;
  x = kWriteView->cXPos - (kWriteView->xPos-2);

  return kWriteView->mapToGlobal( QPoint(x,y) );*/
}


extern bool tagsCapital, attrCapital;

QString Document::tagCase( QString  tag)
{
  QString sTag = tag;

  if ( tagsCapital )
    sTag = sTag.upper();
  else
    sTag = sTag.lower();

  return sTag;
}

/** convert attr of tag to upper or lower case */
QString Document::attrCase( QString  attr)
{
  QString sAttr = attr;
  
  if ( attrCapital )
    sAttr = sAttr.upper();
  else
    sAttr = sAttr.lower();

  return sAttr;
}


void Document::insertAttrib(QString attr)
{
   viewCursorIf->setCursorPosition( tagEndY, tagEndX );
   insertTag( QString(" ") + attrCase(attr) + "=\"", QString( "\"" ) );
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
  unsigned int line, col;

  QFile file(fileName);

  file.open(IO_ReadOnly);

  QTextStream stream( &file );

  viewCursorIf->cursorPosition(&line, &col);
  editIf->insertText(line, col, stream.read());

  file.close();
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
}
