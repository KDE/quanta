/***************************************************************************
*   Copyright 2007 Niko Sams <niko.sams@gmail.com>                        *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#ifndef PRILFESFILETREE_H
#define PRILFESFILETREE_H

#include <QWidget>

class QModelIndex;
class QComboBox;
class QLabel;

class KUrl;
class KFileTreeView;

class UploadPlugin;
class AllProfilesModel;
class UploadProfileDlg;

/**
 * The Profiles-File-Tree Dock
 *
 * A ComboBox to select the profile and a KFileTreeView below.
 */
class ProfilesFileTree : public QWidget
{
    Q_OBJECT

public:
    ProfilesFileTree( UploadPlugin* plugin, QWidget *parent = 0 );

    /**
     * Sets the Model for this widget
     */
    void setModel(AllProfilesModel* model);

protected Q_SLOTS:
    /**
     * Open the Url in the editor
     */
    void openUrl(const KUrl& url);

    /**
     * Slot called when on file tree is doubleclicked
     */
    void treeDoubleClicked();

    /**
     * Slot called when current profile has changed, sets the new root-url for
     * the tree
     */
    void profileIndexChanged(int index);

    /**
     * Slot called when data of the profile has changed, sets the new root url
     * (if changed)
     */
    void dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight);

    /**
     * Modify the current profile
     */
    void modifyProfile();

private:
    UploadPlugin* m_plugin;
    AllProfilesModel* m_profilesModel;

    QComboBox* m_profilesCombo;
    QLabel* m_pleaseSelectLabel;
    KFileTreeView* m_tree;
    UploadProfileDlg* m_editProfileDlg;
};


#endif
// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
