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

#include <qdom.h>
#include <qmap.h>
#include <qstringlist.h>

#include <kdebug.h>
#include <kio/netaccess.h>
#include <klocale.h>
#include <kurl.h>

#include "annotationoutput.h"
#include "messageoutput.h"
#include "project.h"
#include "qextfileinfo.h"
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

void AnnotationOutput::tabChanged(QWidget *w)
{
 if (w == m_allAnnotations)
  readAnnotations();
}

void AnnotationOutput::insertAnnotation(uint line, const QString& fileName, const QString& text)
{
  line++;
  QString s = i18n("Line %1: %2").arg(line).arg(text);
  s.replace('\n', ' ');
  m_currentFileAnnotations->showMessage(line, 1, fileName, s);
}

void AnnotationOutput::clearAnnotations()
{
  m_currentFileAnnotations->clear();
}

void AnnotationOutput::readAnnotations()
{
  if (!Project::ref()->hasProject())
    return;
  KURL baseURL = Project::ref()->projectBaseURL();
  m_allAnnotations->clear();  
  QDomElement annotationElement = Project::ref()->dom()->firstChild().firstChild().namedItem("annotations").toElement();
  if (annotationElement.isNull())
    return;
  QDomNodeList nodes = annotationElement.childNodes();
  int count = nodes.count();
  for (int i = 0; i < count; i++)
  {
    QDomElement el = nodes.item(i).toElement();
    QString fileName = el.attribute("url");
    KURL u = baseURL;
    QuantaCommon::setUrl(u, fileName);
    u = QExtFileInfo::toAbsolute(u, baseURL);
    bool ok;
    int line = el.attribute("line").toInt(&ok, 10) + 1;
    QString text = el.attribute("text");
    text = i18n("File: %1, Line %2: %3").arg(fileName).arg(line).arg(text);
    text.replace('\n',' ');
    m_allAnnotations->showMessage(line, 1, u.url(), text);
  }
}

void AnnotationOutput::writeAnnotations(const QString &fileName, const QMap<uint, QString> &annotations)
{
  m_allAnnotations->clear();  
  QDomDocument *dom = Project::ref()->dom();
  QDomElement annotationElement = dom->firstChild().firstChild().namedItem("annotations").toElement();
  if (annotationElement.isNull())
  {
    annotationElement = dom->createElement("annotations");
    dom->firstChild().firstChild().appendChild(annotationElement);
  }
  QDomNode n = annotationElement.firstChild();
  while ( !n.isNull() ) 
  {
    QDomElement el = n.toElement();
    QString fName = el.attribute("url");
    QDomNode n2 = n.nextSibling();
    if (fileName == fName)    
    {
      n.parentNode().removeChild(n);
    }
    n = n2;
  }
  for (QMap<uint, QString>::ConstIterator it = annotations.constBegin(); it != annotations.constEnd(); ++it)
  {
    QDomElement el = dom->createElement("item");
    el.setAttribute("url", fileName);
    el.setAttribute("line", it.key());
    el.setAttribute("text", it.data());
    annotationElement.appendChild(el);
  }
  Project::ref()->setModified(true); //TODO: set the modified flag only when needed
  if (m_allAnnotations->isVisible())
    readAnnotations();
}

#include "annotationoutput.moc"
