/***************************************************************************
    begin                : Tue Mar 14 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavsky <pdima@mail.univ.kiev.ua>
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

#ifndef PROJECTTREEVIEW_H
#define PROJECTTREEVIEW_H

//own includes
#include "basetreeview.h"
#include "filterableitemif.h"

//qt includes
#include <qdom.h>

class FileInfoDlg;
class ProjectList;
class KDevPlugin;
class KDevProject;
namespace KParts{
  class Part;
}


class ProjectTreeViewItem : public BaseTreeViewItem, public FilterableItemIf
{

public:
  ProjectTreeViewItem( KFileTreeViewItem *parent, KFileItem* item, KFileTreeBranch *brnch );
  
  /**
   * @param s the string to compare with
   * @return true if this item should be visible
   */
  bool filter(const QString &s) const;
  
  /**
   * adjust myself and the children to new settings
   */
  void settingsChanged();
  
  /**
   * @return true if this item should be visible
   */
  bool shouldBeVisible() const;

};

class ProjectTreeBranch : public BaseTreeBranch
{

public:
  ProjectTreeBranch(KDevProject * project, BaseTreeView *parent, const KUrl& url,
                    const QString& name, const QPixmap& pix,
                    bool showHidden = false, KFileTreeViewItem *branchRoot = 0L);

  /** check for CVS and visible*/
  virtual KFileTreeViewItem* createTreeViewItem(KFileTreeViewItem *parent, KFileItem *fileItem );

  KDevProject * project() {return m_project;}
  
  private:
    KDevProject * m_project;
};



class ProjectTreeView : public BaseTreeView
{
  Q_OBJECT

public:

  ProjectTreeView(KDevPlugin * plugin, QWidget *parent);

  virtual ~ProjectTreeView();
  
public slots: // Public slots
  void slotOpen();

  void slotProjectOpened();
  
  void slotProjectClosed();

  void slotPopulateFinished(KFileTreeViewItem* );

  /** makes the url visible in the tree */
  void slotActivePartChanged(KParts::Part *part);

  /** fills the list with the url's of the open folder  */
  void slotGetTreeStatus(QStringList *folderToOpen) {m_projectDir->addOpenFolder(folderToOpen);}

  /** adjust the treeview to the new settings
   */
  void slotSettingsChanged();

protected slots:
  void slotRenameItem(Q3ListViewItem* kvtvi, const QString & newText, int col);
  virtual void slotCreateFolder();
  virtual void slotCreateFile();

signals: // Signals
  /** No descriptions */
  void rescanProjectDir();
  /** No descriptions */
  void showProjectOptions();
  void changeDocumentFolderStatus(const KUrl& url, bool status);
  void reloadProject();

protected:
  /**
  creates a branch and adds this branch to the treeview
  @param url the root of the branch
  @return the new branch
  */
  virtual KFileTreeBranch* newBranch(const KUrl& url);
  /**
  called when the description of an item was changed,
  @param item the treeview item
  @param newDesc the new description of item
  */
  void itemDescChanged(KFileTreeViewItem* item, const QString& newDesc);
  /**
  adds the Quanta fileinfopage to the properties dialog
  overwritten to enable the file description
  @param propDlg the dialog where to add the page
  @return pointer to to just added page
  */
  virtual FileInfoDlg* addFileInfoPage(KPropertiesDialog *propDlg);
  
  virtual void fileMenu(const QPoint &point);
  virtual void folderMenu(const QPoint &point);
  virtual void emptyMenu(const QPoint &point);

private slots: // Private slots
  /** Bring up the project options dialog */
  void slotOptions();
  
  void slotRescan();
  
  /** open a dialog for a new foldername and add it to the project
   */
  void createFolder();

private:
  
  /**
   * @param item 
   * @return the QDomElement for this url from the Quanta Project
   */
  QDomElement getDomElement(KFileTreeViewItem* item);

  ProjectTreeBranch *m_projectDir;
  ProjectList *m_projectFiles;
  KDevPlugin * m_plugin;
  KDevProject * m_quantaProject;
  static const QString & m_attrDesc;
  static const QString & m_attrOpen;
};

#endif

// kate: space-indent on; indent-width 2; mixedindent off; indent-mode cstyle;
