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

#include <qdict.h>
#include <qdom.h>
#include <qheader.h>
#include <qmap.h>
#include <qstringlist.h>

#include <kdebug.h>
#include <kio/netaccess.h>
#include <klistview.h>
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
  m_allAnnotations = new KListView(this);
  m_allAnnotations->addColumn("1", -1);
  m_allAnnotations->addColumn("2", 0);
  m_allAnnotations->setRootIsDecorated(true);
  m_allAnnotations->header()->hide();
  m_allAnnotations->setSorting(1);
  m_allAnnotations->setLineWidth(2);
 connect(m_allAnnotations, SIGNAL(executed(QListViewItem*)), SLOT(itemExecuted(QListViewItem *)));
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
  QString s = i18n("Line %1: %2", line, text);
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
  m_annotatedFileItems.clear();
  m_fileNames.clear();
  m_lines.clear();
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
    int line = el.attribute("line").toInt(&ok, 10);
    QString text = el.attribute("text");     
    text.replace('\n',' ');
    QString lineText = QString("%1").arg(line);
    if (lineText.length() < 20)
    {
      QString s;
      s.fill('0', 20 - lineText.length());  
      lineText.prepend(s);
    }
    KListViewItem *fileIt = m_annotatedFileItems[fileName];
    if (!fileIt)
    {
      fileIt = new KListViewItem(m_allAnnotations, fileName);
      m_annotatedFileItems.insert(fileName, fileIt);
      m_fileNames[fileIt] = u.url();
    }
    KListViewItem *it = new KListViewItem(fileIt, fileIt, text, lineText);
    m_fileNames[it] = u.url();
    m_lines[it] = line;
  }
}

void AnnotationOutput::writeAnnotations(const QString &fileName, const QMap<uint, QString> &a_annotations)
{
  m_allAnnotations->clear();  
  m_annotatedFileItems.clear();
  m_fileNames.clear();
  m_lines.clear();
  bool modified = false;
  QMap<uint, QString> annotations = a_annotations;
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
      QString text = el.attribute("text");
      bool ok;
      int line = el.attribute("line").toInt(&ok, 10);
      if (!annotations.contains(line) || (annotations[line] != text))
      {
        n.parentNode().removeChild(n);
        modified = true;
      } else
        annotations.remove(line);
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
    modified = true;
  }
  if (modified)
    Project::ref()->setModified(true);
  if (m_allAnnotations->isVisible())
    readAnnotations();
}

void AnnotationOutput::itemExecuted(QListViewItem *item)
{
  if (dynamic_cast<KListView*> (item->parent()) != m_allAnnotations)
  {
    emit clicked(m_fileNames[item], m_lines[item], 0);
  } else
    emit clicked(m_fileNames[item], 0, 0);
}

#include "annotationoutput.moc"
