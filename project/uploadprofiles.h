//
// C++ Interface: UploadProfiles
//
// Description: 
//
//
// Author: Jens Herden <jens@kdewebdev.org>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef UPLOADPROFILES_H
#define UPLOADPROFILES_H

// QT includes
#include <qmap.h>
#include <qdom.h>
#include <qguardedptr.h>

// KDE includes
#include <kurl.h>


class QObject;

/**
       @short The internal representation of upload profiles.

*/


struct UploadProfile 
{
  QString name;                    ///< name of profile
  QDomElement domElement;          ///< domtree element of this profile
  QGuardedPtr<QWidget> treeview;   ///< treeview for this url
};

/** a map for the upload profiles */
typedef  QMap<QString, UploadProfile> UploadProfileMap;


class UploadProfiles : public UploadProfileMap
{
public:
  /**
   *  since this class is a singleton you must use this function to access it
   */
  static UploadProfiles* const ref()
  {
    static UploadProfiles *m_ref;
    if (!m_ref) m_ref = new UploadProfiles();
    return m_ref;
  }

  ~UploadProfiles();

  /**
      reads the available profiles from the tree and fills the map
  
      @param the dom document to parse 
  */
  void readFromXML(const QDomDocument &dom);

  /**
      removes a profile from the map and the dom tree
  
      @param the profile to remove 
      
      @return true if profile was found and removed
   */
  bool removeFromMapAndXML(const QString &name);

  /**
      clears the map and removes all treeviews
    */
  void clear();

private:
  /** The constructor is privat because we use singleton pattern.
   *  If you need the class use UploadProfiles::ref() for
   *  construction and reference
   */
  UploadProfiles();
  
  /**
      creates a treeview for a profile and adds it to the right toolviews
      
      @param profile the profile 
      
      @return the pointer to the treeview
   */
  QWidget * createTreeview(const UploadProfile &profile);

  /**
      creates a KURL from a QDomElement
  
      @param e a QDomElement where the path is saved
      
      @return KURL of the location
   */
  KURL url(const QDomElement &e);
  
  QDomNode m_profilesNode;  ///< under this node the profiles are saved in the dom tree
};

#endif
