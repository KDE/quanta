/***********************************************************************
                          projectlist.cpp  -  List of ProjectURL's
                             -------------------
    begin                : June 19 2004
    copyright            : (C) 2004 by Jens Herden <jens@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

// qt includes
#include <qdom.h>
#include <qstring.h>
#include <qfileinfo.h>

// kde includes
#include <kprogress.h>

#include "projectlist.h"
#include "projecturl.h"
#include "resource.h"
#include "quantacommon.h"


ProjectList::ProjectList(int size) : ProjectUrlList(size)
{
}


void ProjectList::clear()
{
  m_baseURL = KURL();
  ProjectUrlList::clear();
}

bool ProjectList::readFromXML(QDomDocument &dom, const KURL &baseURL,
                                 const KURL &/*templateURL*/, const QRegExp &excludeRx)
{
  clear();  // empty the list
  m_baseURL = baseURL; // remember this
  bool modified = false;
  QDomElement el;
  QDomNodeList nl = dom.firstChild().firstChild().childNodes();
  progressBar->setTotalSteps(nl.count() - 1);
  progressBar->setValue(0);
  progressBar->setTextEnabled(true);
  QString path;
  QString tmpString;
  uint nlCount = nl.count();
  for ( uint i = 0; i < nlCount; i++ )
  {
    KURL url = baseURL;
    el = nl.item(i).toElement();
    tmpString = el.attribute("url");
    if (!tmpString.isEmpty())
    {
      QuantaCommon::setUrl(url,tmpString);
      //Compatibility conversion
      if (tmpString != QuantaCommon::qUrl(url))
      {
        el.setAttribute("url", QuantaCommon::qUrl(url));
        modified = true;
      }
    }
    path = url.path();
    url = QExtFileInfo::toAbsolute(url, baseURL);
    if ( el.nodeName() == "item" )
    {
      if (excludeRx.exactMatch(path) || find(url.url(-1)))
      {
        el.parentNode().removeChild(el);
        modified = true;
        i--;
      } else
      {
        bool docFolder = (el.attribute("documentFolder", "false") == "true");
        int uploadStatus = el.attribute("uploadstatus", "-1").toInt();
        if (uploadStatus == -1)
          el.setAttribute("uploadstatus", 1);
        //remove non-existent local files
        if ( url.isLocalFile() )
        {
          QFileInfo fi( url.path() );
          if ( !fi.exists() )
          {
            el.parentNode().removeChild( el );
            modified = true;
            i--;
          } else
          {
            insert(url.url(-1), new ProjectURL(url, el.attribute("desc"), el.attribute("uploadstatus", "1").toInt(),
                                             docFolder, el));
          }
        } else
        {
            insert(url.url(-1), new ProjectURL(url, el.attribute("desc"), el.attribute("uploadstatus", "1").toInt(),
                                             docFolder, el));
        }
      }
    }
    progressBar->advance(1);
  }
  ProjectURL *proUrl = find(baseURL);
  if (!proUrl)
  {
      el = dom.createElement("item");
      el.setAttribute("url", "");
      el.setAttribute("uploadstatus", "1");
      dom.firstChild().firstChild().appendChild(el);
      insert(baseURL.url(-1), new ProjectURL(baseURL, "", ProjectURL::AlwaysUpload,
                                             true, el));
      modified = true;
  }
  progressBar->setTotalSteps(1);
  progressBar->setValue(0);
  progressBar->setTextEnabled(false);
  return modified;
}


bool ProjectList::removeFromListAndXML(const KURL &url)
{
  ProjectURL *p = find(url);
  if (p) {
    QDomElement el = p->domElement;
    el.parentNode().removeChild(el);
    remove(url.url(-1));
    return true;
  }
  return false;
}


bool ProjectList::contains(const KURL &url) const
{
  return ProjectUrlList::find(url.url(-1));
}


ProjectURL * ProjectList::find(const KURL &url) const
{
  return ProjectUrlList::find(url.url(-1));
}


void ProjectList::insert(ProjectURL *url)
{
  ProjectUrlList::insert(url->url(-1), url);
}


bool ProjectList::isFolder(const KURL &url) const
{
  ProjectURL *p = find(url);
  return ( p && url.url(-1) != (*p).url() );
}


void ProjectList::insert(const QString & key, const ProjectURL * item)
{
  ProjectUrlList::insert(key, item);
}


ProjectURL * ProjectList::find ( const QString & key ) const
{
  return ProjectUrlList::find(key);
}


bool ProjectList::remove ( const QString & key )
{
  return ProjectUrlList::remove(key);
}


void ProjectList::replace(const QString & key, const ProjectURL * item)
{
  ProjectUrlList::replace(key, item);
}

