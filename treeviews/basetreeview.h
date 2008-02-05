/***************************************************************************
                          BaseTreeView.h  -  description
                             -------------------
    begin                : Thu Jun 1 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon <pdima@users.sourceforge.net,yshurik@linuxfan.com,sequitur@easystreet.com>
                           (C) 2001-2004 Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef BASETREEVIEW_H
#define BASETREEVIEW_H

//qt includes
#include <qvaluelist.h>

//kde includes
#include <kfiletreebranch.h>
#include <kfiletreeview.h>
#include <kfiletreeviewitem.h>

// quanta includes
#include "quantacommon.h"

//foward declarations
class QListViewItem;
class QString;
class QPixmap;
class QPoint;
class QRegExp;
class QCloseEvent;

class KAction;
class KFileItem;
class KPopupMenu;
class KPropertiesDialog;
class KURL;
class FileInfoDlg;
class BaseTreeViewToolTip;
class QuantaPlugin;

/**
 * @short a KFileTreeViewItem with some specials.
 *
 * @author Jens Herden <jens@kdewebdev.org>
 */
class BaseTreeViewItem : public KFileTreeViewItem {

public:
  BaseTreeViewItem( KFileTreeViewItem *parent, KFileItem* item, KFileTreeBranch *brnch );

  /**
  sorts folders separate from files
  */
  QString key (int column, bool ascending) const;

  /**
  makes compare independent from locale
  */
  int compare( QListViewItem *i, int col, bool ascending ) const;

  /**
  makes open files bold and the second column lighter
  */
  void paintCell(QPainter *p, const QColorGroup &cg, int column, int width, int align);

  /**
  refresh icon
  */
  void refreshIcon();
};

/**
 * @short a KFileTreeBranch that uses @ref BaseTreeViewItem and does some special filtering.
 *
 * @author Jens Herden <jens@kdewebdev.org>
 */
class BaseTreeBranch : public KFileTreeBranch {
   Q_OBJECT

public:
  BaseTreeBranch(KFileTreeView *parent, const KURL& url,
                  const QString& name, const QPixmap& pix,
                  bool showHidden = false, KFileTreeViewItem *branchRoot = 0);
  /**
  get the BaseTreeViewItem
  */
  virtual KFileTreeViewItem* createTreeViewItem( KFileTreeViewItem *parent,
                                                 KFileItem *fileItem );
  /**
  does custom filtering
  */
  bool matchesFilter(const KFileItem *item) const;

  /**
  files matching to this will not be shown
  */
  QRegExp excludeFilterRx;

  /**
  adds the current open folders of the branch to a stringslist.
  Attention: no check for double entries is done

  @param openFolder the list where to add the URL strings
  */
  void addOpenFolder(QStringList *openFolder);

  /** opens the next folder in @ref folderToOpen */
  void reopenFolder();

  /** list of folders to open */
  QStringList folderToOpen;

  /** inform the dirwatcher to update all open folders */
  void updateOpenFolder();

};


/**
 * @short the base class for many of Quantas treeviews
 *
 * @ref FilesTreeView, @ref ScriptTreeView, @ref ProjectTreeView and
 * @ref TemplatesTreeView use this class.
 *
 */
class BaseTreeView : public KFileTreeView {
   Q_OBJECT

public:
  BaseTreeView(QWidget *parent = 0L, const char *name = 0L);
  virtual ~BaseTreeView();

  /**
   * Saves the list view's layout (column widtsh, column order, sort column)
   * to a KConfig group. Reimplemented to save the open folders.
   *
   * @param config the KConfig object to write to
   * @param group the config group to use
   */
  void saveLayout(KConfig *config, const QString &group);

  /**
   * Reads the list view's layout from a KConfig group as stored with
   * saveLayout. Reimplemented to load the open folders.
   *
   * @param config the KConfig object to read from
   * @param group the config group to use
   */
  void restoreLayout(KConfig *config, const QString &group);

   /**
  * en/disable saving a list of open folders in @ref saveLayout and
  * restoring the tree status in @ref restoreLayout
  */
  void setSaveOpenFolder(bool b = true) { m_saveOpenFolder = b; };

  /** reads property @ref setSaveOpenFolder */
  bool saveOpenFolder() { return m_saveOpenFolder; };

public slots:
  /**
  sets new project information
  */
  void slotNewProjectLoaded(const QString &, const KURL &, const KURL &);
  /**
  repaints all treeview items
  */
  void slotDocumentClosed(const KURL& url);

protected slots:

  void slotCopy();
  void slotPaste();
  void slotDelete();
  virtual void slotJobFinished( KIO::Job *job);
  void slotInsertInProject();
  void slotInsertDirInProject();
  virtual void slotReturnPressed(QListViewItem *item);
  virtual void slotDropped (QWidget *, QDropEvent *, KURL::List&, KURL&);
  /**
   sends value from KIO::Job to progressbar
   */
  virtual void slotPercent(KIO::Job *job, unsigned long value);

  void slotPopulateFinished(KFileTreeViewItem *item);
  /**
  shows open with dialog for the current item
  */
  void slotOpenWith();

  /** Opens the current item with the selected associated application */
  void slotOpenWithApplication();
  /**
  Called for: double click, return, Open

  emits a signal (@ref openFile or @ref openImage)
  or expands an archiv (@ref expandArchiv) in the tree
  */
  virtual void slotSelectFile(QListViewItem *item);
  /**
  emits the signal @ref insertTag
  */
  void slotInsertTag();
  /**
  reloads the current branch
  */
  void slotReload();
  /**
  reloads all branches
  */
  virtual void slotReloadAllTrees();
  /**
  reloads one branche
  */
  virtual void reload(BaseTreeBranch *btb);
  /**
  opens the properties dialog
  */
  void slotProperties();
  /**
  applies changed properties
  */
  virtual void slotPropertiesApplied();
  /**
  emits the signal @ref open
  */
  void slotOpen();
  /**
  emits the signal @ref closeFile
  */
  void slotClose();

  /**
  starts the inline rename of the current item
  */
  void slotStartRename();

  /**
  connect signal itemRenamed to this slot to handle inline rename
   */
  virtual void slotRenameItem(QListViewItem* kvtvi, const QString & newText, int col);
  /**
  create a site template tarball from the selected directory
  */
  virtual void slotCreateSiteTemplate();
  /**
   Creates a folder under the current item
   */
  virtual void slotCreateFolder();
  /**
  Creates a file under the current item
   */
  virtual void slotCreateFile();

  virtual void slotOpenWithActivated(int id);

protected:
  /**
  implement this to create a branch and add this branch to the treeview
  @param url the root of the branch
  @return the new branch
  */
  virtual KFileTreeBranch* newBranch(const KURL& url) = 0;

  /**
  called when an item was renamed in the properties dialog
  the default implementation emits the signal @ref renamed
  */
  virtual void itemRenamed(const KURL& , const KURL& );

  /**
  called when the description of an item was changed,
  the default implementation does nothing
  @param item the treeview item
  @param newDesc the new description of item
  */
  virtual void itemDescChanged(KFileTreeViewItem* item, const QString& newDesc)
  {
    Q_UNUSED(item);
    Q_UNUSED(newDesc);
  };
  /**
  @return true if the clipboard contains one or more valid path
  */
  bool isPathInClipboard();

  /**
  adds the Quanta fileinfopage to the properties dialog
  @param propDlg the dialog where to add the page
  @return pointer to to just added page
  */
  virtual FileInfoDlg* addFileInfoPage(KPropertiesDialog *propDlg);

  /**
  expands an archiv inside of the tree
  @return true if expanding was possible
  */
  bool expandArchiv (KFileTreeViewItem *item);
  bool acceptDrag(QDropEvent* e ) const;
  void findDrop(const QPoint &pos, QListViewItem *&parent, QListViewItem *&after);

  /**
  this is for project and template tree to reduce includes there
  */
  bool isFileOpen(const KURL &url);

  /** Inserts an Open With submenu into @param menu which lists
  the possible applications that can open the current item and an
  "Other" entry to bring up the Open With dialog. If no application
  is associated with the type of the current item, the menu will bring up the Open With dialog immediately.
  @param menu the menu where the Open With submenu should be inserted
  @param position the position in this menu
  */
  void insertOpenWithMenu(KPopupMenu *menu, int position);
  KPropertiesDialog *propDlg;
  QString m_projectName;
  KURL m_projectBaseURL;
  FileInfoDlg* fileInfoDlg;
  /** the parent of the treeview
      the passwords are cached there
    */
  QWidget * m_parent;

  BaseTreeViewToolTip * m_tooltip;
  // some stuff for renaming
  void doRename(KFileTreeViewItem* kvtvi, const QString & newName);
  /** reimplemented to reset renameable */
  void cancelRename(int col);

signals:
  void openFile(const KURL&);
  void openImage(const KURL&);
  void open(QListViewItem *name);
  void insertDirInProject(const KURL&);
  void insertFileInProject(const KURL&);
  void insertTag(const KURL &, DirInfo);
  /**
  file or folder has been renamed
  */
  void renamed(const KURL &, const KURL &);
  /**
  close the file in Quanta
  */
  void closeFile( const KURL& );

private:
  bool m_saveOpenFolder;
  KPopupMenu *m_openWithMenu;
  int m_openWithMenuId;
  QValueList<KAction *> m_openWithActions;
  QMap<int, QuantaPlugin*> m_pluginIds;
};

#endif
