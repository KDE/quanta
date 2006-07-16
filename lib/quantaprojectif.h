/***************************************************************************
    begin                : Tue Jul 19 2005
    copyright            : (C) 2005 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/
#ifndef QUANTAPROJECTIF_H
#define QUANTAPROJECTIF_H

#include "quantaexport.h"

//kdevelop includes
#include <interfaces/kdevproject.h>

class QDomElement;
class KUrl;
/**
Extension of the KDevProject interface for our needs, especially to work with remote projects and files.

	@author Andras Mantia <amantia@kde.org>
*/
class LIBQUANTA_EXPORT QuantaProjectIf : public KDevProject
{
public:
  QuantaProjectIf(KInstance * info, QObject *parent);
  ~QuantaProjectIf();

  /**
   * Checks if an url is part of the project or not.
   * @param url the url (absolute path)
   * @return true if url is part of the project, false otherwise
   */
  virtual bool isProjectFile(const KUrl &url) = 0;

  /**
   * @param relPath the relative path of a project file or folder
   * @return the QDomElement associated to a project url 
   */
  virtual QDomElement domElementForPath(const QString &relPath) = 0;

  /**
   * Returns the element pointing to the node of the project dom tree holding all the project information. 
   * @return the project dom element
   */
  virtual QDomElement domProjectElement() = 0;

};

#endif
