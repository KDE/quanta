#include "fwglobal.h"
#include <qstringlist.h>
#include <qregexp.h>

int proxInt(double d){
  if((d-int(d))>= 0.5 ) return int(d)+1;
  return int(d);
}

int cancelledPixels(int n){
   return (n-1)*6;
}

QString absolutize(QString dummySrc,QString path){
  dummySrc.remove(QRegExp("/(./)"));
  int c = dummySrc.contains("../");
  if(c!=0) {
    QStringList pl = QStringList::split("/",path);
    for(int i=1; i<=c; i++)
      pl.pop_back();
    QString absPath("/"+pl.join("/"));
    dummySrc.remove("../");
    absPath += ("/"+dummySrc); 
    return absPath;  
  }
  else return path+dummySrc;
 }
 
 QString relativize(QString dummySrc,QString path){
  QStringList dsl=QStringList::split("/",dummySrc);
  QStringList pl=QStringList::split("/",path);
  QString relPath(QString::null);
  QString fileName(dsl.last());//we don't need the name of the file
  dsl.pop_back();
  if(pl != dsl){
        while(1){
                if(pl.first()==dsl.first()){
                        pl.pop_front();
                        dsl.pop_front();
                }
                else break;
        }
        uint i;
        for (i=1;i<=pl.count();i++) { relPath+="../"; }
        QStringList::iterator it;
        for ( it = dsl.begin(); it != dsl.end(); ++it ) relPath+=((*it)+"/");

  }
  else return fileName;
  return (relPath+fileName);
 }

