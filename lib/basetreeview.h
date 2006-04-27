/***************************************************************************
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
#include <qlist.h>
#include <qregexp.h>

//kde includes
#include <kfiletreebranch.h>
#include <kfiletreeview.h>
#include <kfiletreeviewitem.h>

#include <kdevdocumentcontroller.h>

//forward declarations
class Q3ListViewItem;
class QString;
class QPixmap;
class QPoint;
class QCloseEvent;

class KAction;
class KFileItem;
class KPropertiesDialog;
class KUrl;
class FileInfoDlg;
class BaseTreeViewToolTip;
class KDevDocument;
class KDevPlugin;
class BaseTreeView;


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
  int compare( Q3ListViewItem *i, int col, bool ascending ) const;

  /**
  makes open files bold and the second column lighter
  */
  void paintCell(QPainter *p, const QColorGroup &cg, int column, int width, int align);

  /**
  refresh icon
  */
  void refreshIcon();

private:
  BaseTreeView * m_baseTreeView;
};

/**
 * @short a KFileTreeBranch that uses @ref BaseTreeViewItem and does some special filtering.
 *
 * @author Jens Herden <jens@kdewebdev.org>
 */
class BaseTreeBranch : public KFileTreeBranch {
   Q_OBJECT

public:
  BaseTreeBranch(BaseTreeView *parent, const KUrl& url,
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

  inline BaseTreeView * baseTreeView() {return m_baseTreeView;};

private:

  BaseTreeView * m_baseTreeView;
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
  BaseTreeView(KDevPlugin *plugin, QWidget * parent);
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
  bool saveOpenFolder() const { return m_saveOpenFolder; };

  /** check if a file is currently open */
  inline bool isFileOpen(const KUrl & url) const {return m_partController->documentForUrl(url);};


public slots:

  /**
  repaints all treeview items
  */
  void slotDocumentClosed(KDevDocument* document);

protected slots:

  /**
   * try to find a nice icon for a url
   * @param url
   * @return an icon name
   */
  QString iconNameForURL(const KUrl & url);

  void slotCopy();
  void slotPaste();
  void slotDelete();
  virtual void slotJobFinished( KJob *job);
  virtual void slotReturnPressed(Q3ListViewItem *item);
  virtual void slotDropped (QWidget *, QDropEvent *, KUrl::List&, KUrl&);
  /**
   sends value from KIO::Job to progressbar
   */
  virtual void slotPercent(KJob *job, unsigned long value);

  void slotPopulateFinished(KFileTreeViewItem *item);

  /**
  Called for: double click, return, Open

  emits a signal (@ref openFile or @ref openImage)
  or expands an archiv (@ref expandArchiv) in the tree
  */
  virtual void slotSelectFile(Q3ListViewItem *item);
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
  virtual void slotRenameItem(Q3ListViewItem* kvtvi, const QString & newText, int col);

  /**
   Creates a folder under the current item
   */
  virtual void slotCreateFolder();

  /**
  Creates a file under the current item
   */
  virtual void slotCreateFile();

  /**
   * slot for the context menu,
   * checks if the click was on a file a folder or outside
   * calls @see fileMenu() or @see folderMenu() or @see emptyMenu()
   * @param listView
   * @param item
   * @param point
   */
  virtual void slotMenu(K3ListView* listView, Q3ListViewItem *item, const QPoint &point);

protected:

  /**
   * reimplement this if you want context menus
   * @param point position for the popup menu
   */
  virtual void fileMenu(const QPoint &) {}
  virtual void folderMenu(const QPoint &) {}
  virtual void emptyMenu(const QPoint &) {}


  /**
  implement this to create a branch and add this branch to the treeview
  @param url the root of the branch
  @return the new branch
  */
  virtual KFileTreeBranch* newBranch(const KUrl& url) = 0;

  /**
  called when an item was renamed in the properties dialog
  the default implementation emits the signal @ref renamed
  */
  virtual void itemRenamed(const KUrl& , const KUrl& );

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
  }

  /**
  @return true if the clipboard contains one or more valid path
  */
  bool isPathInClipboard();

  /**
  adds the Quanta fileinfopage to the properties dialog
  @param propDlg the dialog where to add the page
  @return pointer to the just added page
  */
  virtual FileInfoDlg* addFileInfoPage(KPropertiesDialog *propDlg);

  /**
   * Reimplemented to handle the tooltip event.
   * @param event
   * @return
   */
  virtual bool event(QEvent *event);

  /**
  expands an archiv inside of the tree
  @return true if expanding was possible
  */
  bool expandArchiv (KFileTreeViewItem *item);
  bool acceptDrag(QDropEvent* e ) const;
  void findDrop(const QPoint &pos, Q3ListViewItem *&parent, Q3ListViewItem *&after);

  KPropertiesDialog *propDlg;
  QString m_projectName;
  KUrl m_projectBaseURL;
  FileInfoDlg* fileInfoDlg;

  /** the parent of the treeview
      the passwords are cached there */
  QWidget * m_parent;

  KDevPlugin * m_plugin;

  /** some stuff for renaming */
  void doRename(KFileTreeViewItem* kvtvi, const QString & newName);

  /** reimplemented to reset renameable */
  void cancelRename(int col);

signals:

  void open(Q3ListViewItem *name);
  /**
  file or folder has been renamed
  */
  void renamed(const KUrl &, const KUrl &);
  /**
  close the file in Quanta
  */
  void closeFile( const KUrl& );

private:
  bool m_saveOpenFolder;
  BaseTreeViewToolTip * m_tooltip;
  KDevDocumentController * m_partController;
};

#endif

// kate: space-indent on; indent-width 2; mixedindent off; indent-mode cstyle;
