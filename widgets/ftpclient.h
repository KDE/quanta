/***************************************************************************
                          ftpclient.h  -  description
                             -------------------
    begin                : Fri Dec 1 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
    email                : pdima@users.sourceforge.net,yshurik@penguinpowered.com,sequitur@easystreet.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef FTPCLIENT_H
#define FTPCLIENT_H

#define IDLEFT	1
#define IDRIGHT	2

#include <qwidget.h>
#include <qurlinfo.h>
#include <qurloperator.h>
#include "ftpclients.h"

/**
  *@author Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
  */

class QNetworkOperation;

class FtpClient : public FtpClientS  {
   Q_OBJECT
public: 
	FtpClient(QWidget *parent=0, const char *name=0);
	~FtpClient();
	
private:

    void setup();

private slots:
    void slotLocalDirChanged( const QString &path );
    void slotLocalDirChanged( const QUrlInfo &info );
    void slotRemoteDirChanged( const QString &path );
    void slotRemoteDirChanged( const QUrlInfo &info );
    void slotConnect();
    void slotCopy();
    void slotMove();
    void slotUpload( bool move=false );
    void slotDownload( bool move=false );
    void slotLocalStart( QNetworkOperation * );
    void slotLocalFinished( QNetworkOperation * );
    void slotRemoteStart( QNetworkOperation * );
    void slotRemoteFinished( QNetworkOperation * );
    void slotLocalDataTransferProgress( int, int, QNetworkOperation * );
    void slotRemoteDataTransferProgress( int, int, QNetworkOperation * );
    void slotLocalMkdir();
    void slotLocalRemove();
    void slotRemoteMkdir();
    void slotRemoteRemove();

    void slotMkdir();
    void slotRemove();
    void slotSelectLeft();
    void slotSelectRight();

private:
	QUrlOperator localOperator, remoteOperator, oldLocal, oldRemote;

	int sel;
};

#endif
