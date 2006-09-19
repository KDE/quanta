/***************************************************************************
 *   Copyright (C) 2005 by Andras Mantia   *
 *   amantia@kde.org   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef KDEVQUANTAPROJECT_H
#define KDEVQUANTAPROJECT_H

#include <kdevproject.h>
#include <kdevplugin.h>

#include <qmap.h>
#include <qdom.h>
#include <QPointer>
#include <QStringList>

#include <kurl.h>

class BrowserPopupMenu;
class QuantaProjectManager;

class QAction;
class KMenu;
class KAction;
class KDialog;
class Context;
class ConfigWidgetProxy;
class KDevProjectModel;
class KDevProjectItem;

/**
Please read the README.dox file for more info about this part
*/
class QuantaProjectPart: public KDevPlugin
{
    Q_OBJECT
public:
    QuantaProjectPart(QObject *parent, const QStringList &args);
    ~QuantaProjectPart();
    
    // the methods to embed the view 
    virtual QWidget *pluginView() const;
    
    virtual Qt::DockWidgetArea dockWidgetAreaHint() const 
    	{return Qt::RightDockWidgetArea;}
    
    virtual void closeProject();
//    virtual KDE_DEPRECATED QString projectDirectory() const;
    /**
     * Get the URL pointing to the base project directory
     * @return the URL pointing to the base project directory
    */
    virtual KUrl projectDirectory() const {return m_projectBase;}
    virtual void openProject(const KUrl &dirName, const QString &projectName);
    virtual QString projectName() const {return m_projectName;}
    virtual QStringList allFiles () const;
    virtual QList<KDevProjectFileItem*> allFiles();
    virtual void addFiles (const QStringList &fileList);

    virtual void addFile (const QString &fileName) {addFiles(QStringList(fileName));}

    virtual void removeFiles (const QStringList &fileList);
    virtual void removeFile (const QString &fileName);
/*Remove if inProject works!!!
    virtual KDE_DEPRECATED bool isProjectFile(const QString &path) {return isProjectFile(KUrl(path));}

        virtual bool isProjectFile(const KUrl &url);
*/    
    /**
    * Returns the element pointing to the node of the project dom tree holding all the project information.
    * @return the project dom element ("/kdevelop/project" in our case)
    */
    virtual QDomElement domProjectElement() {return m_projectDomElement;}

    /**
      * @param url url of a project file or folder
      * @return the QDomElement associated to a project url
      */
    virtual QDomElement domElementForPath(const QString &relPath) {return m_files.contains(relPath) ? m_files[relPath] : QDomElement();}

    virtual QString relativeProjectFile(const QString &absPath);

private slots:
    void init();

    void insertConfigWidget(const KDialog *dlg, QWidget *page, unsigned int pageNo);
    void contextMenu(KMenu *popup, const Context *context);

    void slotInsertFiles();
    void slotInsertFolder();
    void slotTargetFolderSelected(QAction *action, const KUrl& url);

private:
    void setupActions();
    QStringList removeItems(const QStringList &item);
    QList<KDevProjectFileItem*> recurseFiles(KDevProjectItem *item);

    QuantaProjectManager *m_widget;
    ConfigWidgetProxy *m_configProxy;

    KUrl m_projectBase;
    QString m_projectName;
    QMap<QString, QDomElement> m_files;
    QDomDocument *m_projectDom;
    QDomNode m_itemsNode;
    KUrl::List m_fileContextURLs;
    BrowserPopupMenu *m_browserMenu;
    QDomElement m_projectDomElement;
    KDevProjectModel *m_projectModel;
//     KDevProjectFolderItem *m_workspace;

};

#endif
