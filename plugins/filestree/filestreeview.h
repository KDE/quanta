/***************************************************************************
    begin                : Thu Jun 1 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon <pdima@users.sourceforge.net,yshurik@linuxfan.com,sequitur@easystreet.com>
                           (C) 2001-2004 Andras Mantia <amantia@kde.org>
                           (C) 2004-2005 Jens Herden <jens@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef FILESTREEVIEW_H
#define FILESTREEVIEW_H

//qt includes
#include <qmap.h>
#include <QDir>

// quanta includes
#include "basetreeview.h"

#include <ksharedconfig.h>

//forward declarations
class QPoint;
class KMenu;
class KUrl;
class QuantaFilesTreePart;


class FilesTreeView : public BaseTreeView {
   Q_OBJECT

public:
  FilesTreeView(QuantaFilesTreePart *part, QWidget *parent);
  virtual ~FilesTreeView();

public slots:
  /**
   * adjust the treeview to the new settings
   */
  void slotSettingsChanged();

protected slots:
  /**
  opens dialog to choose a url for a new branch
  */
  void slotNewTopFolder();

  /**
  creates a new branch from the current url
  */
  void slotAddToTop();

  /**
      remove folder from the top list
   */
  void slotRemoveFromTop();

  /**
  Changes the alias (alternative name) for a top folder
   */
  void slotChangeAlias();

protected:

  virtual KFileTreeBranch* newBranch(const KUrl& url);

  KMenu *m_emptyMenu;
  // config
  KSharedConfigPtr m_config;

  /**
   * show a context menu if no file or folder is available
   * @param point position to popup
   */
  void emptyMenu(const QPoint &point);

  /**
   * show a context menu for the current file
   * @param point position to popup
   */
  void fileMenu(const QPoint &point);

  /**
   * show a context menu for the current folder
   * @param point position to popup
   */
  void folderMenu(const QPoint &point);

private:

  inline bool blockedURL(const KUrl &url) {return (url == KUrl("file:/")) || (url == KUrl("file:" + QDir::homePath()+"/"));}

  /**
   * read the branches from config file and creates them
   */
  void restoreBranches();

  typedef QMap<QString, QString> BranchMap;
  BranchMap topURLAliases;

  QuantaFilesTreePart *m_part;  ///< the part which created me
};

#endif

// kate: space-indent on; indent-width 2; mixedindent off; indent-mode cstyle;
