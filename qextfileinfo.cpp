/*
    From WebMaker - KDE HTML Editor
    Copyright (C) 1998, 1999 Alexei Dets <dets@services.ru>
	
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
*/    

#include <qdir.h>
#include <kurl.h>
#include <iostream.h>
#include "qextfileinfo.h"

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

 path = QDir::cleanDirPath(path);
 path.remove(0, 1);
 path.append("/");
 basePath = QDir::cleanDirPath(basePath);
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

  if ( basePath.left(5) == "file:" ) basePath.remove(0,5);
  if ( path.left(5) == "file:" ) path.remove(0,5);

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
/** all files in dir ( added yshurik, quanta team ).
	The return will also contain the name of the subdirectories.
	This is needed for empty directory adding/handling. (Andras)*/
QStringList QExtFileInfo::allFiles( QString path, QString mask, int level )
{
	QStringList r;
		
	QDir dir( path );
	
	if ( level > 10        ) return r;
	if ( !dir.isReadable() ) return r;
    if ( !dir.exists()     ) return r;
	
	QStringList  dirList;
	QStringList  fileList;
    QStringList::Iterator it;
		
	dir.setFilter ( QDir::Dirs);
	dirList = dir.entryList();
		
	dirList.remove(".");
	dirList.remove("..");
		
	dir.setFilter( QDir::Files | QDir::Hidden | QDir::System);
	fileList = dir.entryList();
	
	for ( it = dirList.begin(); it != dirList.end(); ++it )
	{
		QString name = *it;
	  if ( QFileInfo( dir, *it ).isSymLink() ) continue;
	    r += path+name+"/";
		r += allFiles( path+name+"/", mask, level+1 );
    }

    for ( it = fileList.begin(); it != fileList.end(); ++it )
	{
	  QString name = *it;
	
	  QFileInfo fi( dir, *it );
	  if ( fi.isSymLink() || !fi.isFile() ) continue;
		
	  if ( QDir::match( mask, name) ) r += (path+name);
	}
	
	return r;
}

QStringList QExtFileInfo::allFilesRelative( QString path, QString mask, int level )
{
	QStringList::Iterator it;
	QStringList r = QExtFileInfo::allFiles( path, mask, level );
	
	for ( it = r.begin(); it != r.end(); ++it )
	{
		*it = QExtFileInfo::toRelative( *it, path );
	}
	
	return r;
}

void QExtFileInfo::createDir( QString path )
{
	QDir dir;
	int i=0;
	while ( !dir.exists(path) && i<20 )
	{
		QString d1(path);
		QString d2(path);
		
		d1=cdUp(d1);
		while ( !dir.exists(d1) && !d1.isEmpty() )
			{ d1=cdUp(d1);d2=cdUp(d2);debug(d1);}
		
		dir.mkdir(d2);
		i++;
	}
}

QString QExtFileInfo::cdUp(QString &dir)
{
	if ( dir.right(1) == "/" ) dir.remove( dir.length()-1,1);
	while ( dir.right(1) != "/" ) dir.remove( dir.length()-1,1);
	return dir;
}

QString QExtFileInfo::shortName(QString fileName)
{
  int pos;
  QString fname = fileName;
  while ( (pos = (int)fname.find('/')) != -1 )
    fname.remove(0,pos+1);
  
  return fname;
}

QString QExtFileInfo::path( KURL &url )
{
  QString furl = url.url();
  int i = furl.findRev('/');
  
  if ( i != -1 ) return furl.left(i+1);
  
  return QString::null;
}

QString QExtFileInfo::home()
{
  return QDir::currentDirPath()+"/";
}
