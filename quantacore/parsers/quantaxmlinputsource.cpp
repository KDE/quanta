/* This file is part of Quanta
 *
 *    Copyright (C) 2006 Jens Herden   <jens@kdewebdev.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 */


#include "QXmlInputSource"
#include <QXmlLocator>

#include "ktexteditor/smartcursor.h"
#include "ktexteditor/document.h"
#include "ktexteditor/smartinterface.h"

#include "quantaxmlinputsource.h"


class Locator : public QXmlLocator
{
  public:
    Locator(const QuantaXmlInputSource * source) : m_source(source) {};
    ~Locator() {};
    /**
     * \name QXmlLocator Interface
     *
     * The following methods implement the \ref QXmlLocator interface
     * \{
     */
    /**
     * \return the current column position, -1 means invalid position!
     */
    int columnNumber() const
    {
      return m_source->m_oldColumn;
    }
    /**
     * \return the current line number, -1 means invalid position!
     */
    int lineNumber() const
    {
      return m_source->m_oldLine;
    }
    /**
     * \}
     */
  private:
    const QuantaXmlInputSource * m_source;
};



QuantaXmlInputSource::QuantaXmlInputSource(KTextEditor::Document * doc)
  : QXmlInputSource(), m_document(doc)
{
  reset();
  // TODO connect to the document to know when it is about to close
}

QuantaXmlInputSource::~QuantaXmlInputSource()
{
}

QXmlLocator * QuantaXmlInputSource::newLocator() const
{
  return new Locator(this);
}


QString QuantaXmlInputSource::data() const
{
  return m_document->text();
}

QString QuantaXmlInputSource::charactersUntil(const QChar &searchCh)
{
  QString result;
  result.reserve(1000);
  int pos = m_buffer.indexOf(searchCh, m_column);
  if ( pos != -1)
  {
    result = m_buffer.mid(m_column, pos - m_column);
    m_column = pos;
    return result;
  }
  m_buffer = m_buffer.mid(m_column);
  while (pos == -1)
  {
    result.append(m_buffer);
    m_line++;
    if (m_line >= m_numLines)
    {
      m_line = -1;
      return result;
    }
    m_buffer = m_document->line(m_line);
    pos = m_buffer.indexOf(searchCh);
  }
  if (pos > 0)
  {
    result.append(m_buffer.left(pos - 1));
    m_column = pos - 1;
  } else
  {
    m_line--;
    m_buffer = m_document->line(m_line);
    m_column = m_buffer.length();
  }
  return result;
}


QChar QuantaXmlInputSource::next()
{
  m_oldLine = m_line;
  m_oldColumn = m_column;
  if (m_line < 0)
    return QXmlInputSource::EndOfDocument;

  if (m_column >= m_buffer.length())
  {
    m_line++;
    if (m_line >= m_numLines)
    {
      m_line = -1;
    }
    else
    {
      m_column = 0;
      m_buffer = m_document->line(m_line);
    }
    return '\n';
  }
  else
  {
    QChar c = m_buffer.at(m_column);
    m_column++;
    return c;
  }
}

void QuantaXmlInputSource::reset()
{
  m_numLines = m_document->lines() ;
  if (m_numLines > 0)
  {
    m_line = 0;
    m_column = 0;
    m_buffer = m_document->line(0);
  }
  else
  {
    m_line = -1;
    m_buffer = QString();
  }
  m_oldLine = m_line;
  m_oldColumn = m_column;
}



//kate: space-indent on; indent-width 2; replace-tabs on; mixedindent off; encoding utf-8
