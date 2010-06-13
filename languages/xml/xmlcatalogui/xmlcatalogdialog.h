/*****************************************************************************
 * Copyright (c) 2009 Ruan Strydom <rm3dom@gmail.com>                        *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
 *****************************************************************************/

#ifndef XMLCATALOGDIALOG_H
#define XMLCATALOGDIALOG_H

#include <QtCore/QAbstractItemModel>

#include <KDE/KDialog>

class ICatalogEntry;
class ICatalog;

class XmlCatalogDialog : public KDialog {
    Q_OBJECT
public:
    XmlCatalogDialog ( QWidget* parent = 0, Qt::WFlags flags = 0 );
    virtual ~XmlCatalogDialog();
    virtual void showEvent(QShowEvent* );
    private slots:
        void exit();
        void canceled();
        void addCatalog(const QString&);
        void addEntry();
        void removeItem();
        void updateItem();
        void itemSelected(const QModelIndex& index);
        void update();
        void slotAddEntry();
        void slotOpenDocument();
    private:
        bool save();
        void getSelection(ICatalog *&catalog, ICatalogEntry *&entry);
        class XmlCatalogDialogPrivate;
        XmlCatalogDialogPrivate *d;
};

class AddSchemaDialog : public KDialog{
    Q_OBJECT
    public:
        enum Type {PublicId, SystemId, Uri, Doctype};
        AddSchemaDialog(QWidget* parent = 0, Qt::WFlags flags = 0);
        virtual ~AddSchemaDialog();
        QString id() const;
        QString URI() const;
        Type type() const;
    private:
        class AddSchemaDialogPrivate;
        AddSchemaDialogPrivate *d;
};

#endif // XMLCATALOGDIALOG_H
