/*
    From WebMaker - KDE HTML Editor
    Copyright (C) 1998, 1999 Alexei Dets <dets@services.ru>
	
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
*/    

#include "qextfileinfo.h"
#include <iostream.h>

QExtFileInfo::QExtFileInfo(const char *file):QFileInfo(file)
{
		QString temp=filePath();
		//This hack is needed because of bug in QFileInfo::fileName
		//Otherwise, directories ended with "/" will have no filename
		if (temp.right(1)=="/" && temp.length()>1)
				setFile(temp.left(temp.length()-1));
}

bool QExtFileInfo::convertToRelative(const char *baseDir)
{
 QString path=dirPath(TRUE);
 QString basePath=baseDir;

 path.remove(0, 1);
 path.append("/");
 basePath.remove(0, 1);
 basePath.append("/");
 int pos=0;
 int pos1=0;
 for (;;)
  {
   pos=path.find("/");
   pos1=basePath.find("/");
   if (pos<0 || pos1<0) break;
   if (path.left(pos+1)==basePath.left(pos1+1))
    {
     path.remove(0, pos+1);
     basePath.remove(0, pos1+1);
    }
   else
    break;
  };
 if (basePath=="/") basePath="";
 int level=basePath.contains("/");
 for (int i=0; i<level; i++)
  {
   path="../"+path;
  };

	if (isFile())
			setFile(path+fileName());
	else
			setFile(path+fileName()+"/");     //it is good to have trailing / on the web
 return TRUE;
}

/** create to ralative short name */
QString QExtFileInfo::toRelative(QString fname,QString dir)
{
  QString path = fname;
  QString basePath = dir;

  path.remove(0, 1);
  basePath.remove(0, 1);
  if ( basePath.right(1) != "/" ) basePath.append("/");

  int pos=0;
  int pos1=0;

  for (;;)
  {
    pos=path.find("/");
    pos1=basePath.find("/");
    if ( pos<0 || pos1<0 ) break;
    if ( path.left(pos+1 ) == basePath.left(pos1+1) )
    {
      path.remove(0, pos+1);
      basePath.remove(0, pos1+1);
    }
    else
      break;
  };

  if ( basePath == "/" ) basePath="";
  int level = basePath.contains("/");
  for (int i=0; i<level; i++)
  {
    path="../"+path;
  };

  return path;
}
/** convert relative filename to absolute */
QString QExtFileInfo::toAbsolute( QString  fname, QString dir)
{
  int pos;
  QString cutname = fname;
  QString cutdir = dir;
  while ( (pos = cutname.find("../")) >=0 ) {
     cutname.remove( 0, pos+3 );
     cutdir.remove( cutdir.length()-1, 1 );
     cutdir.remove( cutdir.findRev('/')+1 , 1000);
  }

  return cutdir+cutname;
}
