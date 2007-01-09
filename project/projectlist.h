/***********************************************************************
                          projectlist.h  -  List of ProjectURL's
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

#ifndef PROJECTLIST_H
#define PROJECTLIST_H

#include "projecturl.h"
#include "qextfileinfo.h"

class QDomDocument;


/**
       @short Container for the project files.

       For fast lookup this container is based on QDict.
       The key is generated without a trailing '/' so it doesn't matter if you
       search for a folder in this container with or without trailing '/'
*/

class ProjectList : public ProjectUrlList
{
public:
  /** 1009 is a guess for maximum files in an average project, this number should be prime (@ref QDict)
  */
  ProjectList( int size = 1009 );
  ~ProjectList() {};

  typedef QDictIterator<ProjectURL> Iterator;

  /** resets @ref m_baseURL */
  void clear();

  /**
       reads entries from the dom tree
       invalid entries gets removed!
       @return true if an old structure was found
   */
  bool readFromXML(QDomDocument &dom, const KURL &baseURL,
                     const KURL &templateURL, const QRegExp &excludeRx);
  /**
       @return the relative URL
  */
  KURL toRelative(const KURL &url) const
  {
    return QExtFileInfo::toRelative(url, m_baseURL, false);
  }

  /**
      removes an url from the list and deletes the corresponding node

       @return true if url was in list
  */
  bool removeFromListAndXML(const KURL &url);

  /**   @return true if url is in container
  */
  bool contains(const KURL &url) const;

  /**   @return pointer to the entry or 0 if not found
  */
  ProjectURL * find(const KURL &url) const;

  /**
        Attention: never delete the object you have given here because the
        container takes ownership and deletes it later!

        @param the ProjectURL to add to the container
  */
  void insert(ProjectURL *url);

  /**   @return true if url is in container and a folder
  */
  bool isFolder(const KURL &url) const;

private:
  KURL m_baseURL;

  /**
       private because I want to control the key.
       use insert(ProjectURL *url) instead.
  */
  void insert(const QString & key, const ProjectURL * item);

  /**
       private because I want to control the key.
       use find(const KURL &url) instead.
  */
  ProjectURL *find ( const QString & key ) const;

  /**
       private because I want to control the key.
  */
  bool remove ( const QString & key );

  /**
       private because I want to control the key.
  */
  void replace(const QString & key, const ProjectURL * item);
};

#endif // PROJECTFILES_H
