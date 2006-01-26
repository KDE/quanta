/***************************************************************************
                               pathmapper.h
                         ------------------------
    begin                : 2004-10-10
    copyright            : (C) 2004 Linus McCabe

 ***************************************************************************/

/****************************************************************************
 *                                                                          *
 *   This program is free software; you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation; either version 2 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 ***************************************************************************/

#include "pathmapper.h"
#include "pathmapperdialog.h"

#include "quantacommon.h"
#include "project.h"

#include <kdebug.h>
#include <klocale.h>
#include <qstring.h>
#include <qextfileinfo.h>
#include <qdom.h>

PathMapper::PathMapper(QObject *parent, const char *name)
    : QObject(parent, name)
{

}

PathMapper::PathMapper()
{

}

QString PathMapper::translate(const QString & path, const QString & from, const QString &to)
{
  QString translated = path;
  
  // Check if this dir is matched by the maps
  if(path.startsWith(from, false))
  {
    translated.remove(0, from.length());
    translated = to + translated;
  }

  return translated;
}

QString PathMapper::mapLocalPathToServer(const QString &localpath)
{
  if(localpath.isEmpty())
    return localpath;
    
  if(m_localBasedir == "/" && m_serverBasedir == "/")
    return localpath;
    
  QString newpath = translate(localpath, m_localBasedir, m_serverBasedir);
  
  // If no translation occurred, check and see if some of the other translations does
  if(newpath == localpath)
  {
    for (unsigned int cnt = 0; cnt < m_serverlist.count(); cnt++ )
    {
      // If the entire filename matches, count it as a match even if there is no difference
      if(m_locallist[cnt] == localpath)
        return localpath;

      // If both are slashes, count it as a local project
      if(m_locallist[cnt] == "/" && m_serverlist[cnt] == "/")
        return localpath;

      // Check if it translates into something 
      newpath = translate(localpath, m_locallist[cnt], m_serverlist[cnt]);
      if(newpath != localpath)
      {
        addHistory(localpath, newpath, true);
        return newpath;
      }
    }

    // No translation found -> show dialog
    PathMapperDialog pmdlg(localpath, PathMapperDialog::LocalToServer);
    for (unsigned int cnt = 0; cnt < m_serverlist.count(); cnt++ )
      pmdlg.addHistory(m_serverlist[cnt], m_locallist[cnt]);

    if(pmdlg.exec() == QDialog::Accepted )
    {
      newpath = translate(localpath, pmdlg.localPath(), pmdlg.serverPath());
      addHistory(pmdlg.localPath(), pmdlg.serverPath(), newpath != localpath); 
      
      return newpath;
    }
    return localpath;
  }
  return newpath;
}

QString PathMapper::mapServerPathToLocal(const QString &serverpath)
{
  QString newpath;
  if(serverpath.isEmpty())
    return serverpath;

  newpath = translate(serverpath, m_serverBasedir, m_localBasedir);
  
  // Check if this dir is matched by the basedirs
  if(QExtFileInfo::exists(newpath, true, 0L))
    return newpath;

  // Check if any previous mappings fit...
  for (unsigned int cnt = 0; cnt < m_serverlist.count(); cnt++ )
  {
    newpath = translate(serverpath, m_serverlist[cnt], m_locallist[cnt]);
    if(QExtFileInfo::exists(newpath, true, 0L))
      return newpath;
  }
  
  // If the basedirs didnt match, check if the file exists,
  // otherwise scan through the mapping history or show the
  // mapping dialog
  if(!QExtFileInfo::exists(serverpath, true, 0L))
  {
    PathMapperDialog pmdlg(serverpath, PathMapperDialog::ServerToLocal);
    for (unsigned int cnt = 0; cnt < m_serverlist.count(); cnt++ )
      pmdlg.addHistory(m_serverlist[cnt], m_locallist[cnt]);

    if(pmdlg.exec() == QDialog::Accepted )
    {
      addHistory(pmdlg.localPath(), pmdlg.serverPath(), true);
      newpath = translate(serverpath, pmdlg.localPath(), pmdlg.serverPath());
      return newpath;
    }
  }

  return serverpath;
}

void PathMapper::setLocalBasedir(const QString &localpath)
{
  m_localBasedir = localpath;
}
void PathMapper::setServerBasedir(const QString &serverpath)
{
  m_serverBasedir = serverpath;
}

QDomNode PathMapper::pathMapperNode()
{
  QDomNode nodeThisDbg;
  QDomDocument *dom = Project::ref()->sessionDom();
  QDomNode projectNode = dom->firstChild().firstChild();
  QDomNode nodeDbg  = projectNode.namedItem("debuggers");
  if(nodeDbg.isNull())
  {
    nodeDbg = dom->createElement("debuggers");
    projectNode.appendChild(nodeDbg);
  }

  // Find the pathmapper section
  nodeThisDbg = nodeDbg.namedItem("pathmapper");
  if(nodeThisDbg.isNull())
  {
    nodeThisDbg = dom->createElement("pathmapper");
    nodeDbg.appendChild(nodeThisDbg);
  }
  
  return nodeThisDbg;
}

void PathMapper::addHistory(const QString &localpath, const QString &serverpath, bool saveinproject)
{
  bool exists = false;
  for (unsigned int cnt = 0; cnt < m_serverlist.count() && !exists; cnt++ )
    if(m_serverlist[cnt] == serverpath &&  m_locallist[cnt] == localpath)
      exists = true;

  if(!exists)
  {
    if(saveinproject)
    {
      QDomNode node = pathMapperNode();
      QDomNode newnode = Project::ref()->dom()->createElement("mapping");
  
      QDomAttr serverattr = Project::ref()->dom()->createAttribute("serverpath");
      serverattr.setValue(serverpath);
      QDomAttr localattr = Project::ref()->dom()->createAttribute("localpath");
      localattr.setValue(localpath);
  
      newnode.attributes().setNamedItem(serverattr);
      newnode.attributes().setNamedItem(localattr);
  
      node = node.namedItem("mappings");
      node.insertAfter(newnode, node.lastChild());
    }
    
    m_serverlist.append(serverpath);
    m_locallist.append(localpath);
  }

}

void PathMapper::readConfig()
{
  QDomNode node = pathMapperNode();

  // Server
  QDomNode valuenode = node.namedItem("mappings");
  QDomNode child = valuenode.firstChild();
  QString serverpath, localpath;
  while(!child.isNull())
  {
    serverpath = child.attributes().namedItem("serverpath").nodeValue();
    localpath = child.attributes().namedItem("localpath").nodeValue();
    kdDebug(24002) << "PathMapper::readConfig " << serverpath << ", " << localpath << endl;

    m_serverlist.append(serverpath);
    m_locallist.append(localpath);
    child = child.nextSibling();
  }
}

#include "pathmapper.moc"
