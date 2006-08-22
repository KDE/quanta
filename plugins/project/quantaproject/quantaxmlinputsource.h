/* This file is part of Quanta
 *
 *    Copyright (C) 2006 Jens Herden   <jens@kdewebdev.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 */

#ifndef QUANTAXMLINPUTSOURCE_H
#define QUANTAXMLINPUTSOURCE_H

#include "QXmlInputSource"

#include "ktexteditor/cursorfeedback.h"
#include "ktexteditor/cursor.h"

class KTextEditor::Document;
class KTextEditor::SmartCursor;

/**
 * \short A QXmlInputSource for a KTextEditor::document.
 *
 * This class allows to use a KTextEditor::document as a source for parsing.
 * The document must implement the KTextEditor::SmartInterface because
 * internally a SmartCursor is used.
 *
 * \author Jens Herden \<jens@kdewebdev.org\>
 **/

class QuantaXmlInputSource: 
    public QXmlInputSource, public KTextEditor::SmartCursorWatcher
{
public:
  /**
   * \param doc the document to read from, must implement KTextEditor::SmartInterface
   **/
  QuantaXmlInputSource(KTextEditor::Document * doc);
  
  virtual ~QuantaXmlInputSource();
  /**
   * \return the whole content of the textdocument or QString::Null() if there is no document
   **/
  virtual QString data() const;
  /**
   * get the next character and move the cursor one position
   * 
   * \return the next character or QXmlInputSource::EndOfDocument
   **/
  virtual QChar next();
  /**
   * reset the cursor to the beginning of the textdocument
   **/
  virtual void reset();
  /**
   * from SmartCursorWatcher to make sure that we do not use the cursor anymore
   * after it was deleted.
   **/
  void deleted(KTextEditor::SmartCursor * cursor);
  /**
   * \return the current position as a cursor, might be invalid!
   **/
  KTextEditor::Cursor currentPosition();
  
private:
  KTextEditor::SmartCursor * m_cursor;
};

#endif 

// kate: space-indent on; indent-width 2; replace-tabs on;
