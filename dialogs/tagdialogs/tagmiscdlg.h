/***************************************************************************
                          tagmiscdlg.h  -  description
                             -------------------
    copyright            : (C) 2005 Laurent Montel <montel@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef TAGMISCDLG_H
#define TAGMISCDLG_H

#include <kdialogbase.h>


class TagMisc;

class TagMiscDlg : public KDialogBase
{
    Q_OBJECT

public:
    TagMiscDlg( QWidget* parent = 0, const char* name = 0, bool addClosingTag=true, const QString &element=QString::null );
    ~TagMiscDlg();

    QString elementTagName() const;
    bool addClosingTag() const;

protected slots:
    void slotMiscTagChanged( const QString & );

private:
    TagMisc *miscWidget;
};

#endif // TAGMISCDLG_H
