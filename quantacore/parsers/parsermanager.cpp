/***************************************************************************
    begin                : Mon Jun 27 2005
    copyright            : (C) 2005 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/
#include "parsermanager.h"
#include "statemachine.h"

#include <kinstance.h>
#include <kstandarddirs.h>

ParserManager *ParserManager::m_ref = 0L;

ParserManager::ParserManager(QObject *parent)
 : QObject(parent)
{
  m_parser = new Parser();
  connect(m_parser, SIGNAL(finishedParsing(const EditorSource *, const ParseResult  *)), SIGNAL(finishedParsing(const EditorSource *, const ParseResult  *)));
  connect(m_parser, SIGNAL(groupsParsed(const EditorSource *, const ParseResult  *)), SIGNAL(groupsParsed(const EditorSource *, const ParseResult  *)));

  m_xmlStateMachine = new StateMachine();
  QString xmlFile = KGlobal::instance()->dirs()->findResource("data", "quanta/statemachines/xmlstates.xml");
  m_xmlStateMachine->build(xmlFile);

}


ParserManager::~ParserManager()
{
  delete m_parser;
  m_parser = 0L;
  delete m_xmlStateMachine;
  m_xmlStateMachine = 0L;
  m_ref = 0L;
}


void ParserManager::parse(EditorSource *source, ParseResult *base, const DTDStruct *dtd, bool detailed)
{
  emit startParsing(source);
  m_parser->parse(source, base, dtd, detailed);
  emit finishedParsing(source, base);
}


void ParserManager::rebuild(EditorSource *source, ParseResult *base, const DTDStruct *dtd, bool detailed)
{
  emit startParsing(source);
  m_parser->rebuild(source, base, dtd, detailed);
  emit finishedParsing(source, base);
}


#include "parsermanager.moc"
