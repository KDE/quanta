//
// C++ Implementation: annotationoutput
//
// Description: 
//
//
// Author: Andras Mantia <amantia@kde.org>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include <qmap.h>
#include <qstringlist.h>

#include <kio/netaccess.h>
#include <klocale.h>
#include <kurl.h>

#include "annotationoutput.h"
#include "messageoutput.h"
#include "project.h"
#include "quantacommon.h"

AnnotationOutput::AnnotationOutput(QWidget *parent, const char *name)
 : KTabWidget(parent, name)
{
  m_currentFileAnnotations = new MessageOutput(this);
  addTab(m_currentFileAnnotations, i18n("Current File"));
  m_allAnnotations = new MessageOutput(this);
  addTab(m_allAnnotations, i18n("All Files"));
  connect(this, SIGNAL(currentChanged(QWidget*)), SLOT(tabChanged(QWidget*)));
}

AnnotationOutput::~AnnotationOutput()
{
}

void AnnotationOutput::refreshAnnotations()
{
  m_allAnnotations->clear();
}

void AnnotationOutput::tabChanged(QWidget *w)
{
  if (w != m_currentFileAnnotations)
    refreshAnnotations();
}

void AnnotationOutput::insertAnnotation(uint scope, uint line, const QString& text)
{
  line++;
  QString s = i18n("Line %1: %2").arg(line).arg(text);
  s.remove('\n');
  if (scope == CurrentFile)
    m_currentFileAnnotations->showMessage(line, 1, "", s);
  else if (scope == AllFiles)
    m_allAnnotations->showMessage(line, 1, "", s);
}

void AnnotationOutput::clearAnnotations()
{
  m_currentFileAnnotations->clear();
  m_allAnnotations->clear();
}

#include "annotationoutput.moc"
