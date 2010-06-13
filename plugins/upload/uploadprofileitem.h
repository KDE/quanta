/***************************************************************************
*   Copyright 2007 Niko Sams <niko.sams@gmail.com>                        *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#ifndef UPLOADPROFILEITEM_H
#define UPLOADPROFILEITEM_H

#include <QStandardItem>

class KUrl;
class KConfigGroup;

class UploadProfileItem : public QStandardItem
{
    enum {
        UrlRole = Qt::UserRole+1,
        IsDefaultRole = Qt::UserRole+2,
        ProfileNrRole = Qt::UserRole+3
    };
public:
    UploadProfileItem();
    virtual ~UploadProfileItem() {}

    void setUrl(const KUrl& url);

    /**
     * Set if this item is the default upload-profile.
     * Sets default to false for all other items in this model
     */
    void setDefault(bool isDefault);

    /**
     * Set the profile-number, which is used as group-name in the config
     */
    void setProfileNr(const QString& nr);

    KUrl url() const;
    bool isDefault() const;

    /**
     * Returns the profile-number, which is used as group-name in the config
     */
    QString profileNr() const;

    /**
     * Returns the KConfigGroup for this upload profile if one exists
     */
    KConfigGroup profileConfigGroup() const;

    int type() const {
        return UserType+1;
    }
};


#endif
// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
