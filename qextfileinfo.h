/*
    From WebMaker - KDE HTML Editor
    Copyright (C) 1998, 1999 Alexei Dets <dets@services.ru>
	
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
*/    

#ifndef _QEXTFILEINFO_H_
#define _QEXTFILEINFO_H_

#include <qfileinfo.h>
#include <qstringlist.h>

class QExtFileInfo:public QFileInfo
{
public:
	QExtFileInfo(const char *file);
	bool convertToRelative(const char *baseDir);

	/** create to ralative short name */
	static QString toRelative(QString fname,QString dir);
  /** convert relative filename to absolute */
  static QString toAbsolute( QString  fname, QString dir);
  /** recurse function for all files in dir */
  static QStringList allFiles( QString path, QString mask, int level = 0 );
  static QStringList allFilesRelative( QString path, QString mask, int level = 0 );
};

#endif
