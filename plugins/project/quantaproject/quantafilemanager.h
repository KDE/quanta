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

class QuantaFileManager: public KDevFileManager
{

  Q_OBJECT

public:
  QuantaFileManager(QObject *parent);
  ~QuantaFileManager();
  /**
   * Imports the base into the model. Creates a KDevProjectFolderItem for the base
   * and appends it to the KDevProjectModel passed as the argument
   * @param model the KDevProjectModel to insert the created item
   * @param base the base URL to import (points to a directory)
   * @return the item created for base and inserted into the model
   */
  virtual KDevProjectItem *import(KDevProjectModel *model, const KUrl &base);

  /**
   * Parses the project file for the files and folders and builds the internal tree
   * consisting of KDevProjectFolderItem and KDevProjectFileItem objects. The base of the
   * tree will be the KDevProjectFolderItem used as the argument.
   * The generic usage would be
   * @code
   *  QuantaFileManager *manager = new QuantaFileManager(projectObject);
   *  KDevProjectFolderItem *baseItem = manager->import(model, baseUrl);
   *  manager->parse(baseItem);
   * @endcode
   *
   * @param base the base folder item in the project
   * @return This implementation always returns an empty subfolder list and should be ignored.
   */
  virtual QList<KDevProjectFolderItem *> parse(KDevProjectFolderItem *base);
  /**
   * Reimplemented from KDevFileManager.
   * @return the top base item, created with import();
   */
  virtual KDevProjectFolderItem *top() {return m_baseItem;}

  private:
    KDevProjectFolderItem *m_baseItem;
};

#endif


