/***********************************************************************
                          uploadprofiles.cpp  -  extended Map of UploadProfiles
                             -------------------
    begin                : July 11 2004
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
#include <qobject.h>

// KDE includes
#include <kiconloader.h>
#include <klocale.h>

// app includes
#include "servertreeview.h"
#include "uploadprofiles.h"
#include "quanta.h"
#include "resource.h"


UploadProfiles::UploadProfiles() : UploadProfileMap()
{
}


void UploadProfiles::readFromXML(const QDomDocument &dom)
{
  clear();  // empty the list
  m_profilesNode = dom.firstChild().firstChild().namedItem("uploadprofiles");
  if (m_profilesNode.isNull())
    return;
    
  QDomNodeList profileList = m_profilesNode.toElement().elementsByTagName("profile");
  UploadProfile newProfile;
  for (uint i = 0; i < profileList.count(); i++)
  {
    newProfile.domElement = profileList.item(i).toElement();
    newProfile.name = newProfile.domElement.attribute("name");
    newProfile.treeview = createTreeview(newProfile);
    insert(newProfile.name, newProfile);
  }
}


bool UploadProfiles::removeFromMapAndXML(const QString &name)
{
  UploadProfiles::Iterator it = find(name);
  if ( it == end() )
    return false;
  // ok now remove
  QDomElement el = (*it).domElement;
  el.parentNode().removeChild(el);
  erase(name);
  return true;
}

void UploadProfiles::clear()
{
  // remove all treeviews
  for (UploadProfiles::iterator it = begin(); it != end(); ++it)
  {
    if ( (*it).treeview )
    {
      quantaApp->deleteToolWindow( (*it).treeview );
    }
  }
  // clear the map
  UploadProfileMap::clear();
}

QWidget * UploadProfiles::createTreeview(const UploadProfile &profile)
{
  QWidget *widget = 0L;
  KURL kurl = url(profile.domElement);
  if (kurl.isValid() && ! kurl.isEmpty()) 
  {
    widget = new ServerTreeView(quantaApp->config(), quantaApp, kurl, "ServerTreeView" + profile.name);
    widget->setIcon(SmallIcon("up"));
    widget->setCaption( i18n("Upload Profile") + ": " + profile.name );
    quantaApp->addToolWindow(widget, KDockWidget::DockRight, quantaApp->getMainDockWidget());
  }
  return widget;
}
    
    
KURL UploadProfiles::url(const QDomElement &e)
{
  QString s = e.attribute("remote_protocol","ftp") + "://";
  QString tmp = e.attribute("user","");
  if (! tmp.isEmpty()) {
    s += tmp + "@";
  }
  s += e.attribute("remote_host","");
  tmp = e.attribute("remote_port","");
  if (! tmp.isEmpty()) {
    s += ":" + tmp;
  }
  s += e.attribute("remote_path","");
  return KURL::fromPathOrURL(s);
}
