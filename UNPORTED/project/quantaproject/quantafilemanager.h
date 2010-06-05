/**************************************************************************
*   Copyright (C) 2006 by Andras Mantia                                   *
*   amantia@kde.org                                                       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/

#ifndef QUANTAFILEMANAGER_H
#define QUANTAFILEMANAGER_H

#include <kdevfilemanager.h>

class QuantaFileManager: public Koncrete::FileManager
{
  Q_OBJECT
      
public:
  explicit QuantaFileManager(const KComponentData &instance, QObject *parent);
  ~QuantaFileManager();
  /**
   * Imports the base into the model. Creates a Koncrete::ProjectFolderItem for the base
   * and appends it to the Koncrete::ProjectModel passed as the argument
   * @param model the Koncrete::ProjectModel to insert the created item
   * @param base the base URL to import (points to a directory)
   * @return the item created for base and inserted into the model
   */
  virtual Koncrete::ProjectItem *import(Koncrete::ProjectModel *model, const KUrl &base);

  /**
   * Parses the project file for the files and folders and builds the internal tree
   * consisting of Koncrete::ProjectFolderItem and Koncrete::ProjectFileItem objects. The base of the
   * tree will be the Koncrete::ProjectFolderItem used as the argument.
   * The generic usage would be
   * @code
   *  QuantaFileManager *manager = new QuantaFileManager(projectObject);
   *  Koncrete::ProjectFolderItem *baseItem = manager->import(model, baseUrl);
   *  manager->parse(baseItem);
   * @endcode
   *
   * @param base the base folder item in the project
   * @return This implementation always returns an empty subfolder list and should be ignored.
   */
  virtual QList<Koncrete::ProjectFolderItem *> parse(Koncrete::ProjectFolderItem *base);
  /**
   * Reimplemented from Koncrete::FileManager.
   * @return the top base item, created with import();
   */
  virtual Koncrete::ProjectFolderItem *top() {return m_baseItem;}
  /**
   * Add a folder url to the project files.
   * @param folder the url pointing to the folder
   * @param parent the parent folder item
   * @return 
   */
 // virtual Koncrete::ProjectFolderItem *addFolder(const KUrl &folder, Koncrete::ProjectFolderItem *parent);
//  virtual Koncrete::ProjectFileItem *addFile(const KUrl &folder, Koncrete::ProjectFolderItem *parent);
  
  private:
    Koncrete::ProjectFolderItem *m_baseItem;
};

#endif


