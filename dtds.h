/***************************************************************************
                          dtds.h  -  description
                             -------------------
    begin                : 12.02.2004  (extract from quanta_init and others)
    
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev <pdima@users.sourceforge.net,yshurik@linuxfan.com>
                           (C) 2001-2003 by Andras Mantia <amantia@kde.org>
                           (C) 2000, 2003 by Eric Laffoon <sequitur@kde.org>
                           (C) 2004 by Jens Herden <jhe at epost.de>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/

#ifndef DTDS_H
#define DTDS_H

// application specific includes
#include "qtag.h"
#include "qobject.h"
#include "qfile.h"

//qt includes
#include <qdict.h>

//kde includes
#include <kdebug.h>


class QDomNode;
class QDomDocument;
class QTag;
class QString;
class QStringList;
class KConfig;
struct DTDStruct;


/** @short This class manages the DTD's for Quanta.
 *
 *  The constructor only loads the necessary information from the description.rc file. 
 *  The rest is later loaded on demand of the find() function. 
 */
class DTDs : public QObject
{
  Q_OBJECT
  
public:
  
  /**
   *  since this class is a singleton you must use this function to access it
   *  @return the class pointer
   */
  static DTDs* ref()
  {
    static DTDs *m_ref;
    if (!m_ref) m_ref = new DTDs();
    return m_ref;
  }
  
  
  ~DTDs();
  
  /**
   * searchs a dtd and loads it from disk if not present in memory
   *
   * @param dtdName name of the dtd, will be converted to lowercase inside
   * @return the found dtd structure
   */
  const DTDStruct * find (const QString &dtdName) 
  {
//    kdDebug(24000) << "dtds::find " << dtdName << endl;
    DTDStruct *dtd = m_dict->find(dtdName.lower()) ;
    if (dtd && ! dtd->loaded ) readTagDir2(dtd);
    return dtd;
  }
  /**
   * @param nickName nickname of the DTD
   * @return the name (identifier) to the nickname. If the DTD is not found you get 
   *         nickName back!  
   */
  QString getDTDNameFromNickName(const QString& nickName);  
  /**
   * @param name name of the DTD
   * @return the nickname to the name. If the DTD is not found you get 
   *         name back!  
   */
  QString getDTDNickNameFromName(const QString& name)
  {
    DTDStruct *dtd = m_dict->find(name);
    if ( dtd ) 
      return dtd->nickName;
    else
      return name;
  };

  /**
   * creates a list of all available nicknames 
   *
   * @param topLevelOnly true = only the DTD's which can be the main DTD of a document
   *                     are included
   * @return all known nick names 
   */
  QStringList nickNameList (bool topLevelOnly=false);

  /**
   * creates a list of all available names
   *
   * @param topLevelOnly true = only the DTD's which can be the main DTD of a document
   *                     are included
   * @return all known names 
   */
  QStringList nameList (bool topLevelOnly=false);


  /** finds a dtd for a given mimetype 
   * 
   * @param mimetype look for this type
   * @return the first DTD for this mimetype or 0L
   */
  const DTDStruct * DTDfromMimeType(const QString &mimetype);
  
public slots:
  
  /** Load a DTD and convert to a DTEP */
  void slotLoadDTD();
  
  /** Load a DTEP*/
  void slotLoadDTEP();

signals:
  /** used to hide the splash screen */
  void hideSplash();
  
  /** DTD has changed need reparse */
  void forceReparse();

private:

  /** The constructor is privat because we use singleton patter.
   *  If you need the class use DTDs::ref() for
   *  construction and reference
   */    
  DTDs();
  
  
  /** Reads the tag files and the description.rc from tagDir in order to
   *  build up the internal DTD and tag structures. 
   * 
   * @param dirName folder where dtd definition is located
   * @param loadAll true = all information and tags will be loaded now (@ref readTagDir2 will be called)
   * @return true = no error
   */
  bool readTagDir(const QString &dirName, bool loadAll=true);
  
  /**  Reads the tag files and the description.rc from tagDir in order to
    *  build up the internal DTD and tag structures. 
    *
    * @param dtd the already preloaded dtd structure
    * @return true = no error
    */
  bool readTagDir2(DTDStruct *dtd);


  /**  helper for @ref readTagDir2
    *
    * @param dtd the already preloaded dtd structure
    */
  void resolveInherited (DTDStruct *dtd);

  /** Reads the tags from the tag files. 
   * 
   * @param fileName path of the tag file
   * @param parentDTD the DTD where the tags belog to 
   * @param tagList the list where the tags are inserted
   * @return the number of read tags.
   */
  uint readTagFile(const QString& fileName, DTDStruct* parentDTD, QTagList *tagList);

  /**
  Parse the dom document and retrieve the tag attributes
  */
  void setAttributes(QDomNode *dom, QTag* tag);
  /** removes dtd from dictonary and deletes all components 
   * 
   * @param dtd the dtd to delete
   */
  void removeDTD(DTDStruct *dtd);
  
  /** helper to read the tag files */
  QDomDocument *m_doc;
  
  /** dictonary with references to all DTD's in memory */
  QDict<DTDStruct> *m_dict;  
};


#endif
