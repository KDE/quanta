/***************************************************************************
                          scripttreeview.h  -  description
                             -------------------
    begin                : Thu Sep 16 2003
    copyright            : (C) 2003 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/
#ifndef SCRIPTTREEVIEW_H
#define SCRIPTTREEVIEW_H

//own includes
#include "filestreeview.h"

/**
 * @short Treeview with all in Quanta available scripts.
 *
 *   You can manage and excecute scripts from here.
 *
 *   There are local and global scripts in different branches.
 *
 *   For every script should exist an .info file!
 *
 *   The .info file is an xhtml file with the extention .info and
 *   must be in one folder together with the script.
 *
 *   Inside of your .info file should be an options tag like this
 *   example:
 *
 *   <options editor="kmdr-editor" interpreter="kmdr-executor" />
 *
 *   Here is defined which program is used for editing and executing the script.
 *
 * @author Andras Mantia <amantia@kde.org>
 */
class ScriptTreeView : public FilesTreeView  {
   Q_OBJECT


public:
   ScriptTreeView(KActionCollection *ac,
                  KDockWidget *parent, const char *name = 0L);
  ~ScriptTreeView();

public slots:

  /**
   *  displays the RBM
   *
   *
   *  @param listView KListView where the event comes from
   *
   *  @param item QListViewItem where the mousepointer is hovering
   *
   *  @param point QPoint coordinates of the event
   *
   */
  virtual void slotMenu(KListView *listView, QListViewItem *item, const QPoint &point);

  /**
   *  slot of QListView
   *
   *  shows the .info file for the script
   *
   *  @param item the selected QListViewItem
   */
  virtual void slotSelectFile(QListViewItem *item);

  /**
   *  slot for the RBM
   *
   *  invokes the editor for a script
   */
  void slotEditScript();

  /**
   *  slot for the RBM
   *
   *  opens the script in Quanta
   *
   *  if @ref infoOptionValue can not find the definition of the editor
   *  in the .info file the script will be opened in Quanta
   */
  void slotEditInQuanta();

  /**
   *  slot for the RBM
   *
   *  opens the .info file of the script in Quanta
   */
  void slotEditDescription();

  /**
   *  slot for the RBM
   *
   *  excecutes the script
   *
   *  If @ref infoOptionValue can not find the definition of the interpreter
   *  in the .info file you will get a open-with dialog to choose a program.
   */
  void slotRun();

  /**
   *  slot for the RBM
   *
   *  opens an dialog to assign an action to this script
   */
  void slotAssignAction();

  /**
   *  slot for the RBM
   *
   *  packs and sends the script as attachment to an email
   */
  void slotSendScriptInMail();

  /**
   *  slot for the RBM
   *
   *  shows .info file for the script
   *
   *  calls @ref slotSelectFile
   */
  void slotProperties();

signals:

  /**
   *  emmited from @ref slotSelectFile to display the .info file
   *
   */
  void openFileInPreview(const KURL&);

  /**
   *  emmited from @ref slotAssignAction to open the assignment dialog
   *
   */
  void assignActionToScript(const KURL&, const QString&);

private:

  /**
   *  creates the URL of the .info file
   *
   *  @param url URL of the script file
   *
   *  @return URL of the matching .info file (no check is done if the file exists)
   */
  KURL infoFile(const KURL& url);

  /**
   *  query options from the .info file
   *
   *  inside of your .info file only the first options tag is located
   *
   *  @param infoURL URL of the .info file
   *
   *  @param optionName name of the option you want to query
   *
   *  @return the value of the option
   */
  QString infoOptionValue(const KURL& infoURL, const QString& optionName);

};

#endif
