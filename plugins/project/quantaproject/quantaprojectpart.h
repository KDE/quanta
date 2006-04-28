/***************************************************************************
 *   Copyright (C) 2005 by Andras Mantia   *
 *   amantia@kde.org   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Steet, Fifth Floor, Boston, MA 02110-1301, USA.           *
 ***************************************************************************/

#ifndef KDEVQUANTAPROJECT_H
#define KDEVQUANTAPROJECT_H

#include "quantaprojectif.h"

#include <qmap.h>
#include <qdom.h>
#include <qpointer.h>

#include <kurl.h>

class BrowserPopupMenu;
class QuantaProjectWidget;

class QAction;
class QMenu;
class KAction;
class KDialogBase;
class Context;
class ConfigWidgetProxy;

/**
Please read the README.dox file for more info about this part
*/
class QuantaProjectPart: public QuantaProjectIf
{
    Q_OBJECT
public:
    QuantaProjectPart(QObject *parent, const char *name, const QStringList &args);
    ~QuantaProjectPart();

    virtual void closeProject();
    virtual QString projectDirectory() const KDE_DEPRECATED; 
    /**
     * Get the URL pointing to the base project directory
     * @return the URL pointing to the base project directory
    */
    virtual KUrl projectBase() const {return m_projectBase;}
    virtual void openProject(const QString &dirName, const QString &projectName);
    virtual QString projectName() const {return m_projectName;} 
    virtual QStringList allFiles () const;
    virtual void addFiles (const QStringList &fileList);
    
    virtual void addFile (const QString &fileName) {addFiles(QStringList(fileName));}
    
    virtual void removeFiles (const QStringList &fileList);
    virtual void removeFile (const QString &fileName);
    virtual KDE_DEPRECATED bool isProjectFile(const QString &path) {return isProjectFile(KUrl::fromPathOrURL(path));}
    
    virtual bool isProjectFile(const KUrl &url);
    /**
    * Returns the element pointing to the node of the project dom tree holding all the project information. 
    * @return the project dom element ("/kdevelop/project" in our case)
    */
    virtual QDomElement domProjectElement() {return m_projectDomElement;}

    /* A lot of pure virtuals in the KDevProject interface that we don't use, don't care about them, but have to reimplement them. */
    virtual DomUtil::PairList runEnvironmentVars() const {return DomUtil::PairList();}
    virtual QString mainProgram (bool relative = false) const {Q_UNUSED(relative) return QString();}
    virtual QString runDirectory () const {return QString();}
    virtual QString runArguments () const {return QString();}
    virtual QString activeDirectory () const {return QString();}
    virtual QString buildDirectory () const {return QString();}
    virtual QStringList distFiles () const {return QStringList();}
    
    /**
      * @param url url of a project file or folder
      * @return the QDomElement associated to a project url 
      */
    virtual QDomElement domElementForPath(const QString &relPath) {return m_files.contains(relPath) ? m_files[relPath] : QDomElement();}

    virtual QString relativeProjectFile(const QString &absPath);
    
private slots:
    void init();
    
    void insertConfigWidget(const KDialogBase *dlg, QWidget *page, unsigned int pageNo);
    void contextMenu(QMenu *popup, const Context *context);
    
    void slotInsertFiles();
    void slotInsertFolder();
    void slotTargetFolderSelected(QAction *action, const KUrl& url);

private:
    void setupActions();
    QStringList removeItems(const QStringList &item);
    
    QPointer<QuantaProjectWidget> m_widget;
    ConfigWidgetProxy *m_configProxy;

    KUrl m_projectBase;
    QString m_projectName;
    QMap<QString, QDomElement> m_files;
    QDomDocument *m_projectDom;
    QDomNode m_itemsNode;
    KUrl::List m_fileContextURLs;
    BrowserPopupMenu *m_browserMenu;
    QDomElement m_projectDomElement;
};

#endif
