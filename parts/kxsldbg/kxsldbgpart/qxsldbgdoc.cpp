/**
   File : qxsldbgdoc.cpp
   Author : Keith Isdale
   Date : 19th April 2002
   Description : The document to handle source and breakpoints
 */

#include <qfile.h>
#include <qtextstream.h>

#include "qxsldbgdoc.h"

QXsldbgTextLine::QXsldbgTextLine(const QString & text)
{
  this->text = text;
  breakPointState = QXsldbgTextLine::empty;
  selected = false;
}


QXsldbgTextLine::BreakPointState QXsldbgTextLine::getBreakpointState()
{
  return breakPointState;
}


const QString & QXsldbgTextLine::getText()
{
  return text;
}

void QXsldbgTextLine::enableBreakPoint(bool state)
{
  if (state)
    breakPointState = QXsldbgTextLine::enabled;
  else
    breakPointState = QXsldbgTextLine::disabled;    
}


void QXsldbgTextLine::deleteBreakPoint()
{
  breakPointState = QXsldbgTextLine::empty;
}


void QXsldbgTextLine::selectBreakPoint(bool state)
{
  selected = state;
}

bool QXsldbgTextLine::isSelected()
{
  return selected;
}


QXsldbgDoc::QXsldbgDoc()
{
  fileName = "";
  textLines.setAutoDelete(TRUE);
}


QXsldbgDoc::~QXsldbgDoc()
{
}


bool QXsldbgDoc::load(QString fileName)
{
  bool result = false;
  QString text;
  QFile file( fileName );

  if (file.open( IO_ReadOnly ) ) {
    QTextStream ts( &file );
    QXsldbgTextLine *item;

    text = ts.readLine();	    
    textLines.clear();
    while (text != 0L){
      item = new QXsldbgTextLine(text);
      if (item == 0L)
	break;
      else
	textLines.append(item);
      text = ts.readLine();
      if (ts.atEnd())
	break;
    }
    this->fileName = fileName;
  } else {
    qDebug( "Unable to open file %s", (char *) fileName.latin1() );
  }
  return result;
}


QXsldbgTextLine* QXsldbgDoc::getText(uint lineNumber)
{
  QXsldbgTextLine * result = 0L;
  if (lineNumber < textLines.count())
    result = textLines.at(lineNumber);
  return result;
}



QString QXsldbgDoc::getFileName()
{
  return fileName;
}

