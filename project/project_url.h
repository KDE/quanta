/***********************************************************************
                          project_url.h  -  ProjectURL class definition
                             -------------------
    begin                : Wed Feb 18 2004
    copyright            : (C) 2004 by Andrei Berezin <aberezin@comcast.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef PROJECT_URL_H
#define PROJECT_URL_H

#include <qvaluelist.h>
#include <qstring.h>

#include <kurl.h>


/**
 * Project item.
 * Consists of URL and description text.
 */
class ProjectURL : public KURL
{
public:
  QString fileDesc;
  
  ProjectURL()
    : KURL() {}
  ProjectURL(const KURL& url)
    : KURL(url) {}
  ProjectURL(const KURL& url, const QString& desc)
    : KURL(url), fileDesc(desc) {}
  virtual ~ProjectURL() {}
};

typedef QValueList<ProjectURL> ProjectUrlList;

#endif // PROJECT_URL_H
