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
      return m_source->m_lastCursor.column();
    }
    /**
     * \return the current line number, -1 means invalid position!
     */
    int lineNumber() const
    {
      return m_source->m_lastCursor.line();
    }
    /**
     * \}
     */
  private:
    const QuantaXmlInputSource * m_source;
};



QuantaXmlInputSource::QuantaXmlInputSource(KTextEditor::Document * doc)
  : QXmlInputSource(), KTextEditor::SmartCursorWatcher(), m_cursor(0)
{
  KTextEditor::SmartInterface* smart = qobject_cast<KTextEditor::SmartInterface*>(doc);

  Q_ASSERT_X(smart != 0, "constructor QuantaXmlInputSource", "the provided document does not support the smart interface");
  if (smart)
  {
    m_cursor = smart->newSmartCursor();
    m_cursor->setWatcher(this);
    m_lastCursor.setPosition(*m_cursor);
  }
}

QuantaXmlInputSource::~QuantaXmlInputSource()
{
  delete m_cursor;
  m_cursor = 0;
}

QXmlLocator * QuantaXmlInputSource::newLocator() const
{
  return new Locator(this);
}


QString QuantaXmlInputSource::data() const
{
  if (m_cursor)
    return m_cursor->document()->text();
  else
    return QString::Null();
}

QChar QuantaXmlInputSource::next()
{
  m_lastCursor.setPosition(*m_cursor);
  if (! m_cursor || ! m_cursor->isValid())
    return QXmlInputSource::EndOfDocument;

  QChar c = m_cursor->character();
  if (!m_cursor->advance(+1, KTextEditor::SmartCursor::ByCharacter))
    m_cursor->setPosition(-1, -1); //will be invalid on the following call to next
  return c;
}

void QuantaXmlInputSource::reset()
{
  if (m_cursor)
  {
    m_cursor->setPosition(m_cursor->start());
    m_lastCursor.setPosition(*m_cursor);
  }
}

void QuantaXmlInputSource::deleted(KTextEditor::SmartCursor * cursor)
{
  if (m_cursor == cursor)
  {
    m_cursor = 0;
    m_lastCursor = KTextEditor::Cursor::invalid();
  }
}



//kate: space-indent on; indent-width 2; replace-tabs on; mixedindent off; encoding utf-8
