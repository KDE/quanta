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


void QXsldbgDoc::slotDataArrived( KIO::Job * job, const QByteArray& data)
{
    Q_UNUSED(job);
    if (!data.isEmpty())
	fileContents.append(data);
}


void QXsldbgDoc::slotResult( KIO::Job *job )
{
    if ( job->error() != 0 ){
	this->fileName = "";
    }else{
	QStringList lines = QStringList::split('\n', fileContents, TRUE);
	QXsldbgTextLine *item;

	textLines.clear();
	for (QStringList::ConstIterator it = lines.begin(); it != lines.end(); it++){
	    item = new QXsldbgTextLine(*it);
	    if (item == 0L)
		break;
	    else
		textLines.append(item);
	}
    }
    fileContents = ""; 
    disconnect( job, SIGNAL( data( KIO::Job *, const QByteArray& )),
	    this,  SLOT( slotDataArrived( KIO::Job *, const QByteArray& )));
    disconnect( job, SIGNAL( result( KIO::Job * )), 
	    this, SLOT( slotResult( KIO::Job * )));
    emit docChanged();
}


bool QXsldbgDoc::load(QString fileName)
{
    KIO::TransferJob *job = KIO::get( fileName );
    this->fileName = fileName;
    connect( job, SIGNAL( data( KIO::Job *, const QByteArray& )),
	    this, SLOT( slotDataArrived( KIO::Job *, const QByteArray& )));
    connect( job, SIGNAL( result( KIO::Job * )), 
	    this, SLOT( slotResult( KIO::Job * )));

  return true;
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

