/****************************************************************************
** $Id$
**
** Copyright (C) 1992-2000 Trolltech AS.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef FTPVIEW_H
#define FTPVIEW_H

#include <qlistview.h>
#include <qvaluelist.h>
#include <qurlinfo.h>

class FtpClientViewItem : public QListViewItem
{
public:
  FtpClientViewItem( QListView *parent, const QUrlInfo &i );
    
  QUrlInfo entryInfo() { return info; }
    
private:
    QUrlInfo info;
};

class FtpClientView : public QListView
{
    Q_OBJECT
    
public:
    FtpClientView( QWidget *parent );
    QValueList<QUrlInfo> selectedItems() const;
    
public slots:
    void slotInsertEntries( const QValueList<QUrlInfo> &info );
    
signals:
    void itemSelected( const QUrlInfo &info );
    
private slots:
    void slotSelected( QListViewItem *item );
};

#endif
