/***********************************************************************
                          projecturl.h  -  ProjectURL class definition
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
  int uploadStatus; ///< The upload state, see UploadStatus

  ProjectURL()
    : KURL(), uploadStatus(1) {}
  ProjectURL(const KURL& url)
    : KURL(url), uploadStatus(1) {}
  ProjectURL(const KURL& url, const QString& desc, int status)
    : KURL(url), fileDesc(desc), uploadStatus(status) {}
  virtual ~ProjectURL() {}

  /** The default state for a file when uploading */
  enum UploadStatus
  {
     NeverUpload = 0, ///< the files is not selected for upload, even if it was modified
     AlwaysUpload, ///< the file is automatically selected for upload if it was modified
     ConfirmUpload ///<the file is selected for upload, but the user must confirm the upload
  };
};

typedef QValueList<ProjectURL> ProjectUrlList;

#endif // PROJECT_URL_H
