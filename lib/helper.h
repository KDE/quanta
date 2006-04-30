/***************************************************************************
    begin                : Tue Jul 12 2005
    copyright            : (C) 2005 by Jens Herden <jens@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/
#ifndef HELPER_H
#define HELPER_H

#include "quantaexport.h"

#include <qstring.h>

/**
@short some static helper functions

This class has some static helper functions that do not fit somewhere else.

@author Jens Herden
*/
class LIBQUANTA_EXPORT Helper
{
public:
  Helper();

  ~Helper();
  
  /** messagebox that ask if a unknown filetype should be opened
   * @return 
   */
  static int denyBinaryInsert();
  
  /**
   * @return the folder where temp files should be created
   */
  static QString tmpFilePrefix();

  /**
   * @return the folder where application data was found
   */
  static QString dataDir();
  
  /**
   * @return the extension of Quanta toolbar packages
   */
  static QString toolbarExtension() {return ".toolbar.tgz";};
  
  /**
   * @return the folder for resources
   */
  static QString resourceDir() {return "quanta/";};
  
  typedef struct DirInfo
  {
    QString mimeType;
    QString preText;
    QString postText;
    bool    usePrePostText;
  };

private:

  /**
   * @return reads the KDE configuration and returns a folder for temp files
   */
  static QString getTmpFolder();

};

#endif
