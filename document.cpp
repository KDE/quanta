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

#include "document.h"

#include "kwrite/kwdoc.h"
#include "kwrite/highlight/highlight.h"

Document::Document( KWriteDoc *doc, QWidget *parent, const char *name , QString fname)
	:	KWrite( doc, parent, name),filename(fname)
{
}

Document::~Document()
{
}

bool Document::hasFileName()
{
  KURL url;
  if ( kWriteDoc->url().fileName().isEmpty() ) return false;
  return true;
}

QString Document::fileName()
{
  if ( hasFileName() )
  	return kWriteDoc->url().path();
  else
  	return filename;
}

/** Insert tag in cursor position and set cursor between s1 and s2 */
void Document::insert_Tag( char* s1,  char * s2 ){
	int line,col; // cursor position
	VConfig c;

		
  if ( s2 ) { // use 2 tags
  	// QString marked = markedText();
  	if ( !hasMarkedText() ) {
  		kWriteView ->getVConfig(c);
		  kWriteDoc->insert( c, s1);
  	}
  	else {
  		cut();
  		kWriteView ->getVConfig(c);
		  kWriteDoc->insert( c, s1);
		  paste();
  	}
  	
  	col = currentColumn();
  	line = currentLine();
  	kWriteView ->getVConfig(c);
	  kWriteDoc->insert( c, s2);
	  setCursorPosition(line,col);
  }
  else {  // using only 1 tag
  	kWriteView ->getVConfig(c);
	  kWriteDoc->insert( c, s1);
  }


  kWriteDoc->updateViews();
	
}
/** insert tag in document  */

void Document::insertTag(QString s1,QString s2){
	if ( !s2.isEmpty() )
		insert_Tag( const_cast<char*>(s1.data()), const_cast<char*>(s2.data()) );
	else
		insert_Tag( const_cast<char*>(s1.data()) );
}

QString Document::getLine(int y)
{
  if ( y < 0 || y > kWriteDoc->lastLine()) return QString();
  TextLine *textLine;
  textLine = kWriteDoc->getTextLine(y);
  QString s(textLine->getText(), textLine->length());
  // debug( "line : "+s );
  return s;
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
  int x = currentColumn();
  int y = currentLine();

  QString begTag = findBeginOfTag( QString(""), x, y); // find begin
  QString endTag = findEndOfTag(   QString(""), x, y); // end

  QString tag = begTag+endTag;

  //debug( "tag : "+tag );

  if ( !tag.isEmpty() ) parseTag(); // if have tag parse it

  return tag;
}

void Document::changeCurrentTag( QDict<char> *dict )
{
  QDictIterator<char> it( *dict ); // iterator for dict
  QDict<char> oldAttr(1,false);

  VConfig c;
  kWriteView->getVConfig( c);
  kWriteDoc ->recordStart( c, KWActionGroup::ugNone ); // start action

  for ( int i=1; i<tagAttrNum; i++ )
    oldAttr.insert( getTagAttr(i), getTagAttrValue(i) );

  while ( it.current() ) { // for insert new attr

    QString attr = attrCase(it.currentKey());
    const char *val = it.current();

    if ( ! oldAttr.find(attr) ) // insert this attr. in end of tag
    {
      QString value = val;

      QString attrval;

      attrval = QString(" ")+attr+"=\""+val+"\"";

      if ( *val == 0 )  // for checkboxes ( without val) don't print =""
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

    const char *val   = dict->find(attr);

    int x1 = tagAttr[i].x;
    int x2 = tagAttr[i].endx;

    PointStruc cursor;
    cursor.x = x1;
    cursor.y = tagAttr[i].y;

    if ( val ) // change attr
    {
      QString t;
      QString value = val;
      bool trans;          // if val is number, dont use ""
      value.toInt( &trans);

      if ( !trans )
        t = attr+"=\""+val+"\"";
      else
        t = attr+"="+val;

      if ( *val == 0 )  // for checkboxes ( without val) don't print =""
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
  kWriteView->repaint();
}

// return global( on the desktop ) position of text cursor
QPoint Document::getGlobalCursorPos()
{
  int h, y, x;

  h = kWriteDoc->fontHeight;
  y = h*kWriteView->cursor.y - kWriteView->yPos;
  x = kWriteView->cXPos - (kWriteView->xPos-2);

  return kWriteView->mapToGlobal( QPoint(x,y) );
}


extern bool tagsCapital, attrCapital;

QString Document::tagCase( const char*  tag)
{
  QString sTag = tag;

  if ( tagsCapital )
    sTag = sTag.upper();
  else
    sTag = sTag.lower();

  return sTag;
}

/** convert attr of tag to upper or lower case */
QString Document::attrCase( const char*  attr)
{
  QString sAttr = attr;

  if ( attrCapital )
    sAttr = sAttr.upper();
  else
    sAttr = sAttr.lower();

  return sAttr;
}


/**  */
void Document::selectText(int x1, int y1, int x2, int y2 )
{
  VConfig c;
  kWriteView ->getVConfig(c);
  c.cursor.x = x1;
  c.cursor.y = y1;

  PointStruc end;
  end.x = x2;
  end.y = y2;

	doc()->selectTo( c,end,0);
	
	view()->repaint();
}


void Document::replaceSelected(QString s)
{
  VConfig c;
	cut();
  kWriteView ->getVConfig(c);
	kWriteDoc->insert( c, s);
}
