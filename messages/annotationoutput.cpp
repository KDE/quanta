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
#include <qfile.h>
#include <qheader.h>
#include <qmap.h>
#include <qregexp.h>
#include <qstringlist.h>
#include <qtextstream.h>
#include <qtimer.h>

#include <kdebug.h>
#include <kio/netaccess.h>
#include <klistview.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kurl.h>

#include "annotationoutput.h"
#include "messageoutput.h"
#include "project.h"
#include "projectlist.h"
#include "qextfileinfo.h"
#include "quantacommon.h"
#include "viewmanager.h"

AnnotationOutput::AnnotationOutput(QWidget *parent, const char *name)
 : KTabWidget(parent, name)
{
  m_currentFileAnnotations = new MessageOutput(this);
  addTab(m_currentFileAnnotations, i18n("Current File"));

  m_yourAnnotations = new KListView(this);
  m_yourAnnotations->addColumn("1", -1);
  m_yourAnnotations->addColumn("2", 0);
  m_yourAnnotations->setRootIsDecorated(true);
  m_yourAnnotations->header()->hide();
  m_yourAnnotations->setSorting(1);
  m_yourAnnotations->setLineWidth(2);
  addTab(m_yourAnnotations, i18n("For You"));
  
  connect(m_yourAnnotations, SIGNAL(executed(QListViewItem*)), SLOT(yourAnnotationsItemExecuted(QListViewItem *)));

  m_allAnnotations = new KListView(this);
  m_allAnnotations->addColumn("1", -1);
  m_allAnnotations->addColumn("2", 0);
  m_allAnnotations->setRootIsDecorated(true);
  m_allAnnotations->header()->hide();
  m_allAnnotations->setSorting(1);
  m_allAnnotations->setLineWidth(2);
  connect(m_allAnnotations, SIGNAL(executed(QListViewItem*)), SLOT(allAnnotationsItemExecuted(QListViewItem *)));
  addTab(m_allAnnotations, i18n("All Files"));


  connect(this, SIGNAL(currentChanged(QWidget*)), SLOT(tabChanged(QWidget*)));

  m_updateTimer = new QTimer(this);
  connect(m_updateTimer, SIGNAL(timeout()), this, SLOT(slotUpdateNextFile()));

  m_yourAnnotationsNum = 0;
}

AnnotationOutput::~AnnotationOutput()
{
}

void AnnotationOutput::tabChanged(QWidget *w)
{
 if (w == m_allAnnotations || w == m_yourAnnotations)
  readAnnotations();
}

void AnnotationOutput::insertAnnotation(uint line, const QString& fileName, const QPair<QString, QString>& annotation)
{
  line++;
  QString s = i18n("Line %1: %2").arg(line).arg(annotation.first);
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
  {
    m_allAnnotations->clear();
    m_yourAnnotations->clear();
    m_yourAnnotationsNum = 0;
    setTabLabel(m_yourAnnotations, i18n("For You"));
    return;
  }

  KURL baseURL = Project::ref()->projectBaseURL();
  QStringList openedItems;
  QListViewItem *item = m_allAnnotations->firstChild();
  while (item)
  {
    if (item->isOpen())
        openedItems += item->text(0);
    item = item->nextSibling();
  }
  m_allAnnotations->clear();
  m_annotatedFileItems.clear();
  m_fileNames.clear();
  m_lines.clear();

  QStringList yourOpenedItems;
  item = m_yourAnnotations->firstChild();
  while (item)
  {
    if (item->isOpen())
        yourOpenedItems += item->text(0);
    item = item->nextSibling();
  }

  m_yourAnnotations->clear();
  m_yourFileItems.clear();
  m_yourFileNames.clear();
  m_yourLines.clear();
  m_yourAnnotationsNum = 0;

  QDomElement annotationElement = Project::ref()->dom()->firstChild().firstChild().namedItem("annotations").toElement();
  if (annotationElement.isNull())
    return;
  QString yourself = Project::ref()->yourself().lower();
  QStringList roles = Project::ref()->yourRoles();
  QDomNodeList nodes = annotationElement.childNodes();
  int count = nodes.count();
  for (int i = 0; i < count; i++)
  {
    QDomElement el = nodes.item(i).toElement();
    QString fileName = el.attribute("url");
    KURL u = baseURL;
    QuantaCommon::setUrl(u, fileName);
    u = QExtFileInfo::toAbsolute(u, baseURL);
    if (Project::ref()->contains(u))
    {
      bool ok;
      int line = el.attribute("line").toInt(&ok, 10);
      QString text = el.attribute("text");
      QString receiver = el.attribute("receiver");
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
      if (openedItems.contains(fileName))
        fileIt->setOpen(true);
      m_fileNames[it] = u.url();
      m_lines[it] = line;
   
      if (!yourself.isEmpty() && (receiver == yourself || roles.contains(receiver)))
      {
        m_yourAnnotationsNum++;
        KListViewItem *fileIt = m_yourFileItems[fileName];
        if (!fileIt)
        {
          fileIt = new KListViewItem(m_yourAnnotations, fileName);
          m_yourFileItems.insert(fileName, fileIt);
          m_yourFileNames[fileIt] = u.url();
        }
        KListViewItem *it = new KListViewItem(fileIt, fileIt, text, lineText);
        if (yourOpenedItems.contains(fileName))
          fileIt->setOpen(true);
        m_yourFileNames[it] = u.url();
        m_yourLines[it] = line;
      }
    } else
    {
      annotationElement.removeChild(el);
    }
  }
  if (m_yourAnnotationsNum > 0)
  {
    setTabLabel(m_yourAnnotations, i18n("For You: %1").arg(m_yourAnnotationsNum));
  } else
  {
    setTabLabel(m_yourAnnotations, i18n("For You"));
  }
}

void AnnotationOutput::writeAnnotations(const QString &fileName, const QMap<uint, QPair<QString, QString> > &a_annotations)
{
  m_annotatedFileItems.clear();
  m_fileNames.clear();
  m_lines.clear();
  m_yourFileItems.clear();
  m_yourFileNames.clear();
  m_yourLines.clear();

  bool modified = false;
  QMap<uint, QPair<QString, QString> > annotations = a_annotations;
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
      if (!annotations.contains(line) || (annotations[line].first != text))
      {
        n.parentNode().removeChild(n);
        modified = true;
      } else
        annotations.remove(line);
    }
    n = n2;
  }
  for (QMap<uint, QPair<QString, QString> >::ConstIterator it = annotations.constBegin(); it != annotations.constEnd(); ++it)
  {
    QDomElement el = dom->createElement("annotation");
    el.setAttribute("url", fileName);
    el.setAttribute("line", it.key());
    el.setAttribute("text", it.data().first);
    el.setAttribute("receiver", it.data().second.lower());
    annotationElement.appendChild(el);
    modified = true;
  }
  if (modified)
    Project::ref()->setModified(true);
  if (m_allAnnotations->isVisible() || m_yourAnnotations->isVisible())
    readAnnotations();
}

void AnnotationOutput::allAnnotationsItemExecuted(QListViewItem *item)
{
  if (dynamic_cast<KListView*> (item->parent()) != m_allAnnotations)
  {
    emit clicked(m_fileNames[item], m_lines[item], 0);
  } else
    emit clicked(m_fileNames[item], 0, 0);
}

void AnnotationOutput::yourAnnotationsItemExecuted(QListViewItem *item)
{
  if (dynamic_cast<KListView*> (item->parent()) != m_yourAnnotations)
  {
    emit clicked(m_yourFileNames[item], m_yourLines[item], 0);
  } else
    emit clicked(m_yourFileNames[item], 0, 0);
}

void AnnotationOutput::updateAnnotations()
{
  m_updateTimer->stop();
  m_currentFileAnnotations->clear();  
  readAnnotations();
  if (Project::ref()->hasProject() && Project::ref()->projectBaseURL().isLocalFile())
  {
    m_files = Project::ref()->files();
    m_fileIndex = 0;
    m_updateTimer->start(0, true);
  }
}

void AnnotationOutput::updateAnnotationForFile(const KURL& url)
{
  static const QRegExp rx("-->|\\*/");
  if (!ViewManager::ref()->isOpened(url) && QuantaCommon::checkMimeGroup(url, "text" ))
  {    
 //   kdDebug(24000) << "Testing " << url << endl;
    QFile f(url.path());
    if (f.open(IO_ReadOnly))
    {
      QMap<uint, QPair<QString, QString> > annotations;
      uint i = 0;
      QTextStream stream(&f);
      stream.setEncoding(QTextStream::UnicodeUTF8);
      QString line;
      while (!stream.atEnd())
      {
        line = stream.readLine();
        int pos = line.find("@annotation");
        if (pos != -1)
        {
          QString receiver;
          pos += 11;
          if (line[pos] == '(')
          {
            int p = pos;
            pos = line.find(')');
            if (pos != -1)
            {
              receiver = line.mid(p + 1, pos - p - 1);
              pos += 2;
            }
          } else
            pos++;
          int pos2 = line.find(rx);
          annotations.insert(i, qMakePair(line.mid(pos, pos2 - pos).stripWhiteSpace(), receiver));
        }
        ++i;
      }
      f.close();
      if (!annotations.isEmpty())
      {
        KURL u = QExtFileInfo::toRelative(url, Project::ref()->projectBaseURL());
        writeAnnotations(QuantaCommon::qUrl(u), annotations);
      }
    }
  }
}

void AnnotationOutput::slotUpdateNextFile()
{
  updateAnnotationForFile(m_files[m_fileIndex]);
  if (m_fileIndex < m_files.count())
  {
    m_fileIndex++;
    m_updateTimer->start(2, true);
  } else
  if (m_yourAnnotationsNum > 0)
  {
    KMessageBox::information(this, i18n("<qt>There are annotations addressed for you.<br> To view them select the <i>For You</i> tab in the <i>Annotations</i> toolview.</qt>"), i18n("New Annotations"), "Show Your Annotations");
  }
}

#include "annotationoutput.moc"
