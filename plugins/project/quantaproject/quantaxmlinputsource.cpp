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

#include "ktexteditor/smartcursor.h"
#include "ktexteditor/document.h"
#include "ktexteditor/smartinterface.h"

#include "quantaxmlinputsource.h"

QuantaXmlInputSource::QuantaXmlInputSource(KTextEditor::Document * doc)
  : QXmlInputSource(), KTextEditor::SmartCursorWatcher(), m_cursor(0)
{
  KTextEditor::SmartInterface* smart = qobject_cast<KTextEditor::SmartInterface*>(doc);
  
  Q_ASSERT(smart != 0);
  if ( smart )
  {
    m_cursor = smart->newSmartCursor();
    m_cursor->setWatcher(this);
    reset();
  }
}

QuantaXmlInputSource::~QuantaXmlInputSource()
{
  delete m_cursor;
  m_cursor = 0;
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
  if (! m_cursor || ! m_cursor->isValid())
    return QXmlInputSource::EndOfDocument;
  
  QChar c = m_cursor->character();
  m_cursor->advance(+1, KTextEditor::SmartCursor::ByCharacter);
  return c;
}

void QuantaXmlInputSource::reset()
{
  if (m_cursor)
    m_cursor->setPosition(m_cursor->start());
}

void QuantaXmlInputSource::deleted(KTextEditor::SmartCursor * cursor)
{
  if (m_cursor == cursor)
    m_cursor = 0;
}

KTextEditor::Cursor QuantaXmlInputSource::currentPosition()
{
  if (m_cursor && m_cursor->isValid())
    return KTextEditor::Cursor(*m_cursor);
  else
    return KTextEditor::Cursor::invalid();
}

// kate: space-indent on; indent-width 2; replace-tabs on;
