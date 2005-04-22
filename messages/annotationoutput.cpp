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
  m_annotatedFilesAnnotations = new MessageOutput(this);
  addTab(m_annotatedFilesAnnotations, i18n("Annotated Files"));
  connect(this, SIGNAL(currentChanged(QWidget*)), SLOT(tabChanged(QWidget*)));
}

AnnotationOutput::~AnnotationOutput()
{
}

void AnnotationOutput::refreshAnnotations()
{
  m_allAnnotations->clear();
  m_annotatedFilesAnnotations->clear();
  QStringList files = Project::ref()->fileNameList();
  for (QStringList::ConstIterator it = files.constBegin(); it != files.constEnd(); ++it)
  {
    KURL u = KURL::fromPathOrURL(*it);
    QMap<uint, QString> annotations;
    QString file = QuantaCommon::readAnnotations(u, annotations);
    if (!u.isLocalFile())
      KIO::NetAccess::removeTempFile(file);
    for (QMap<uint, QString>::ConstIterator it2 = annotations.constBegin(); it2 != annotations.constEnd(); ++it2)
    {
      m_allAnnotations->insertItem(i18n("File: %1, Line: %2, Column: 1 : %3").arg(*it).arg(it2.key() + 1).arg(it2.data()));
    }
    if (!annotations.isEmpty())
      m_annotatedFilesAnnotations->insertItem(i18n("File: %1, Line: 1, Column: 1").arg(*it));
  }
}

void AnnotationOutput::tabChanged(QWidget *w)
{
  if (w != m_currentFileAnnotations)
    refreshAnnotations();
}


#include "annotationoutput.moc"
