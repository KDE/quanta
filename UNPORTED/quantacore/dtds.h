/***************************************************************************
                          dtds.h  -  description
                             -------------------
    begin                : 12.02.2004  (extract from quanta_init and others)

    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev <pdima@users.sourceforge.net,yshurik@linuxfan.com>
                           (C) 2001-2003 by Andras Mantia <amantia@kde.org>
                           (C) 2000, 2003 by Eric Laffoon <sequitur@kde.org>
                           (C) 2004-2005 by Jens Herden <jens@kdewebdev.org>
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
#include "dtdstruct.h"

//qt includes
#include <QHash>
#include <QObject>
#include <QFile>

//kde includes


class QDomNode;
class QDomDocument;
class QTag;
class QString;
class QStringList;


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
  static DTDs* ref(QObject *parent = 0L)
  {
    if (!m_ref) m_ref = new DTDs(parent);
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
    DTDStruct *dtd = m_dict->value(dtdName.toLower()) ;
    if (dtd && ! dtd->loaded )
      readTagDir2(dtd);

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
    DTDStruct *dtd = m_dict->value(name.toLower());
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
  /**
   * creates a list with the path to the description.rc of all available DTEPs
   *
   * @param topLevelOnly true = only the DTD's which can be the main DTD of a document
   *                     are included
   * @return a list with the name and the path to the description.rc of all available DTEPs in form of
   *               DTEPName | path to description.rc
   */
  QStringList fileNameList (bool topLevelOnly=false);


  /** finds a dtd for a given mimetype
   *
   * @param mimetype look for this type
   * @return the first DTD for this mimetype or 0L
   */
  const DTDStruct * DTDfromMimeType(const QString &mimetype);

  /**
   * Get a list of the global entities
   * @return the global entities (a list of QTag objects)
   */
  QHash<QString, QTag*> *globalEntities() {return m_globalEntities;}

public slots:

  /** Load a DTD and convert to a DTEP */
  void slotLoadDTD();

  /** Loads a DTEP from a directory
   *  @param dirName The directory containing the DTEP
   *  @param askForAutoLoad if true and the user answer with yes to the autoload question
   *                the DTEP will be copied to the local resource directory and will be autoloaded on
   *                startup
  */
  void slotLoadDTEP(const QString& dirName, bool askForAutoLoad);

signals:
  /** used to hide the splash screen */
//  void hideSplash();

  /** DTD has changed need reparse */
  void forceReparse();

  /** Enable/disable the idle timer*/
//  void enableIdleTimer(bool);

  void loadToolbarForDTD(const QString&);

private:

  /** The constructor is privat because we use singleton patter.
   *  If you need the class use DTDs::ref() for
   *  construction and reference
   */
  explicit DTDs(QObject *parent);


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

  /** Parses the dom document and retrieve the tag attributes
  *
  * @param dom the DOM node containing the tag definition
  * @param tag the QTag object that will be initialized by using the information in dom
  * @param common will be true, if the tag is a just a list of common group attributes
  */
  void setAttributes(QDomNode *dom, QTag* tag, bool &common);
  /** removes dtd from dictonary and deletes all components
   *
   * @param dtd the dtd to delete
   */
  void removeDTD(DTDStruct *dtd);

  /** helper to read the tag files */
  QDomDocument *m_doc;

  /** dictonary with references to all DTD's in memory */
  QHash<QString, DTDStruct*> *m_dict;

  QTagList *m_globalEntities;

  static DTDs *m_ref;
  static const QString m_rcFilename;
};


#endif

// kate: space-indent on; indent-width 2; mixedindent off; indent-mode cstyle;
