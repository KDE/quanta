/***************************************************************************
                          bashhighlighter.cpp  -  description
                             -------------------
    begin                : dom mar 16 2003
    copyright            : (C) 2003 by Emiliano Gulmini
    email                : emi_barbarossa@yahoo.it
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qmyhighlighter.h"
#include <qregexp.h>
QMyHighlighter::QMyHighlighter(QTextEdit* Qtxt):QSyntaxHighlighter(Qtxt){
}
/*****************************************************************************/
QMyHighlighter::~QMyHighlighter(){
}

/*****************************************************************************/
int QMyHighlighter::highlightParagraph( const QString & text, int /*endStateOfLastPara*/ )
{
  //QRegExp pattern("\\s*\\{([\\w\\s\\d:;-\"]*)\\}\\s*");
  QRegExp pattern("([#:\\.\\w]*)\\s*\\{");
  int pos=pattern.search(text,0);
  int l=int(pattern.cap(1).length());

  setFormat(pos,l,QColor("red"));

  if( pos== -1)
    pos = 0;

  const int npos = pos+l;

  pattern.setPattern("\\s*([\\s\\w\\d-]*)\\s*:");
  pos=npos;
  while ( pos >= 0 ) {
        pos = pattern.search( text, pos );
        if ( pos > -1 ) {
            l = pattern.matchedLength();

            setFormat(pos,l,QColor("mediumvioletred"));
            pos  += pattern.matchedLength();
        }
    }
  pattern.setPattern(":\\s*([\\.\\#\\w\\s\\d-\\(\\)\",%/]*)\\s*;");
  pattern.setPattern(":\\s*([\\W\\w]*)\\s*;");
  pos=npos;
  while ( pos >= 0 ) {
        pos = pattern.search( text, pos );
        if ( pos > -1 ) {
            l = pattern.cap(1).length();
            setFormat(pos + 2 ,l,QColor("steelblue"));
            pos  += pattern.matchedLength();
        }
    }
   return 0;
}

