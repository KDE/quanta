/***************************************************************************
                          ftpclient.cpp  -  description
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

#include "ftpclient.h"
#include "ftpclientview.h"

#include <qnetwork.h>
#include <qurlinfo.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qmessagebox.h>
#include <qprogressbar.h>
#include <qinputdialog.h>
#include <qurloperator.h>


FtpClient::FtpClient(QWidget *parent, const char *name )
	:FtpClientS(parent,name), localOperator( "/" )
{
	qInitNetworkProtocols();
	
  connect( &localOperator,SIGNAL( newChildren( const QValueList<QUrlInfo> &, QNetworkOperation * ) ),
     				leftView, 		SLOT  ( slotInsertEntries( const QValueList<QUrlInfo> & ) ) );
  connect( &localOperator,SIGNAL( start( QNetworkOperation * ) ),
     				this, 				SLOT  ( slotLocalStart( QNetworkOperation *) ) );
  connect( &localOperator,SIGNAL( finished( QNetworkOperation * ) ),
     				this, 				SLOT	( slotLocalFinished( QNetworkOperation *) ) );
  connect( &localOperator,SIGNAL( removed( QNetworkOperation * ) ),
     				this, 				SLOT	( slotLocalFinished( QNetworkOperation *) ) );
  connect( leftView, 			SIGNAL( itemSelected( const QUrlInfo & ) ),
     				this, 				SLOT	( slotLocalDirChanged( const QUrlInfo & ) ) );
  connect( &localOperator,SIGNAL( dataTransferProgress( int, int, QNetworkOperation * ) ),
           	this, 				SLOT	( slotLocalDataTransferProgress( int, int, QNetworkOperation * ) ) );

  connect( &remoteOperator,	SIGNAL( newChildren( const QValueList<QUrlInfo> &, QNetworkOperation * ) ),
     				rightView, 			SLOT	( slotInsertEntries( const QValueList<QUrlInfo> & ) ) );
  connect( &remoteOperator, SIGNAL( start( QNetworkOperation * ) ),
     				this, 					SLOT	( slotRemoteStart( QNetworkOperation *) ) );
  connect( &remoteOperator, SIGNAL( finished( QNetworkOperation * ) ),
     				this, 					SLOT	( slotRemoteFinished( QNetworkOperation *) ) );
  connect( &remoteOperator,	SIGNAL( removed( QNetworkOperation * ) ),
     				this, 					SLOT	( slotRemoteFinished( QNetworkOperation *) ) );
  connect( rightView, 			SIGNAL( itemSelected( const QUrlInfo & ) ),
     				this, 					SLOT	( slotRemoteDirChanged( const QUrlInfo & ) ) );
  connect( &remoteOperator, SIGNAL( dataTransferProgress( int, int, QNetworkOperation * ) ),
           	this, 					SLOT	( slotRemoteDataTransferProgress( int, int, QNetworkOperation * ) ) );

  connect( leftView,	SIGNAL(selectionChanged()),
  					this,			SLOT(slotSelectLeft()) );
  connect( rightView,	SIGNAL(selectionChanged()),
  					this,			SLOT(slotSelectRight()) );

  connect( buttonF5,	SIGNAL(clicked()),
  				 this,			SLOT(slotCopy()) );
  connect( buttonF6,	SIGNAL(clicked()),
  				 this,			SLOT(slotMove()) );
  connect( buttonF7,	SIGNAL(clicked()),
  				 this,			SLOT(slotMkdir()) );
  connect( buttonF8,	SIGNAL(clicked()),
  				 this,			SLOT(slotRemove()));
  				
  connect( connectButton, SIGNAL( clicked() ),
	     		 this, 					SLOT( slotConnect() ) );
	connect( remotePath,SIGNAL( activated( const QString & ) ),
	     		 this, 			SLOT( slotRemoteDirChanged( const QString & ) ) );
	
  localOperator.listChildren();

  sel = IDLEFT;
}

FtpClient::~FtpClient(){
}

void FtpClient::slotLocalDirChanged( const QString &path )
{
  // The user changed the path on the left side

  oldLocal = localOperator;
  localOperator.setPath( path );
  localOperator.listChildren();
}

void FtpClient::slotLocalDirChanged( const QUrlInfo &info )
{
  // The user changed the path on the left side
  if (	!info.isDir() ||
  			!info.isReadable() ||
  			!info.isExecutable() ) return;

  oldLocal = localOperator;
  localOperator.addPath( info.name() );
  localOperator.listChildren();
}

void FtpClient::slotRemoteDirChanged( const QString &path )
{
  // The user changed the path on the right side
  if ( !remoteOperator.isValid() )
		return;
		
  oldRemote = remoteOperator;
  remoteOperator.setPath( path );
  remoteOperator.listChildren();
}

void FtpClient::slotRemoteDirChanged( const QUrlInfo &info )
{
  // The user changed the path on the right side

  oldRemote = remoteOperator;
  remoteOperator.addPath( info.name() );
  remoteOperator.listChildren();
  remotePath->insertItem( remoteOperator.path(), 0 );
  remotePath->setCurrentItem( 0 );
}

void FtpClient::slotConnect()
{
  // The user pressed the connect button, so let's connect to the
  // FTP server
  // First we need to set stuff (host, path, etc.) which the user
  // entered on the right side to the remote QUrlOperator

  // protocol + hostname
  QString s = "ftp://" + remoteHost->currentText();
  oldRemote = remoteOperator;
  remoteOperator = s;

  // path on the server
  if ( !remotePath->currentText().isEmpty() )
		remoteOperator.setPath( remotePath->currentText() );
  else
		remoteOperator.setPath( "/" );

  // if nothing or "ftp" or "anonymous" has been entered into the username combo,
  // let's connect anonymous, else private with password
  if ( !userName->currentText().isEmpty() &&
	 			userName->currentText().lower() != "anonymous" &&
	 			userName->currentText().lower() != "ftp" )
	{
		remoteOperator.setUser( userName->currentText() );
		remoteOperator.setPassword( password->text() );
  }

  // set the port
  remoteOperator.setPort( port->text().toInt() );

  // finally read the directory on the ftp server
  remoteOperator.listChildren();
}

void FtpClient::slotUpload(bool move)
{
  // the user pressed the upload button

  // if files have been selected on the left side (local filesystem)
  QValueList<QUrlInfo> files = leftView->selectedItems();
  if ( files.isEmpty() )
		return;

  // create a list of the URLs which should be copied
  QStringList lst;
  QValueList<QUrlInfo>::Iterator it = files.begin();
  for ( ; it != files.end(); ++it )
	lst << QUrl( localOperator, ( *it ).name() );

  // copy the list of selected files to the directory in which the
  // remoteOperator currently is (upload)
  remoteOperator.copy( lst, remoteOperator, move );
}

void FtpClient::slotDownload(bool move)
{
  // if the user pressed the download button

  // if files have been selected on the right side (remote filesystem)
  QValueList<QUrlInfo> files = rightView->selectedItems();
  if ( files.isEmpty() )
		return;

  // create a list of the URLs which should be downloaded
  QStringList lst;
  QValueList<QUrlInfo>::Iterator it = files.begin();
  for ( ; it != files.end(); ++it )
		lst << QUrl( remoteOperator, ( *it ).name() );

  // copy the list of selected files to the directory in which the
  // localOperator currently is (download)
  localOperator.copy( lst, localOperator, move );
}

void FtpClient::slotLocalStart( QNetworkOperation *op )
{
  // this slot is always called if the local QUrlOperator starts
  // listing a directory or dowloading a file

  if ( !op )
		return;

  if ( op->operation() == QNetworkProtocol::OpListChildren )
  {
		// start listing a dir? clear the left view!
		leftView->clear();
  }
  else if ( op->operation() == QNetworkProtocol::OpGet )
  {
 		// start downloading a file? reset the progress bar!
 		progress->setTotalSteps( 0 );
 		progress->reset();
   }
}

void FtpClient::slotLocalFinished( QNetworkOperation *op )
{
  // this slot is always called if the local QUrlOperator finished
  // an operation

  if ( !op )
		return;

  if ( op && op->state() == QNetworkProtocol::StFailed )
  {
    // an error happend, let the user know that
    QMessageBox::critical( this, tr( "ERROR" ), op->protocolDetail() );

    // do something depending in the error code
    int ecode = op->errorCode();
		if ( ecode == QNetworkProtocol::ErrListChlidren || ecode == QNetworkProtocol::ErrParse ||
	    ecode == QNetworkProtocol::ErrUnknownProtocol || ecode == QNetworkProtocol::ErrLoginIncorrect ||
	    ecode == QNetworkProtocol::ErrValid || ecode == QNetworkProtocol::ErrHostNotFound ||
	    ecode == QNetworkProtocol::ErrFileNotExisting )
	  {
	    localOperator = oldLocal;
	    localOperator.listChildren();
		}
  }
  else if ( op->operation() == QNetworkProtocol::OpPut ) {
  	// finished saving the downloaded file? reread the dir and hide the progress bar
  	localOperator.listChildren();
  }
  else if ( op->operation() == QNetworkProtocol::OpGet ) {
  	// finished reading a file from the ftp server? reset the progress bar
  	progress->setTotalSteps( 0 );
  	progress->reset();
  }
  else if ( op->operation() == QNetworkProtocol::OpRemove ) {
  	// finished reading a file from the ftp server? reset the progress bar
  	progress->setTotalSteps( 0 );
  	progress->reset();
  	localOperator.listChildren();
  }
}

void FtpClient::slotRemoteStart( QNetworkOperation *op )
{
  // this slot is always called if the remote QUrlOperator starts
  // listing a directory or uploading a file

  if ( !op )
		return;

  if ( op->operation() == QNetworkProtocol::OpListChildren )
  {
    // start listing a dir? clear the right view!
    rightView->clear();
  }
  else if ( op->operation() == QNetworkProtocol::OpGet ) {
    // start downloading a file? reset the progress bar!
    progress->setTotalSteps( 0 );
    progress->reset();
  }
}

void FtpClient::slotRemoteFinished( QNetworkOperation *op )
{
  // this slot is always called if the remote QUrlOperator finished
  // an operation

  if ( !op )
		return;

  if ( op && op->state() == QNetworkProtocol::StFailed )
  {
  	// an error happend, let the user know that
  	QMessageBox::critical( this, tr( "ERROR" ), op->protocolDetail() );

  	QString t;
  	// do something depending in the error code
  	int ecode = op->errorCode();
		if ( ecode == QNetworkProtocol::ErrListChlidren || ecode == QNetworkProtocol::ErrParse ||
	    ecode == QNetworkProtocol::ErrUnknownProtocol || ecode == QNetworkProtocol::ErrLoginIncorrect ||
	    ecode == QNetworkProtocol::ErrValid || ecode == QNetworkProtocol::ErrHostNotFound ||
	    ecode == QNetworkProtocol::ErrFileNotExisting )
	  {
	    remoteOperator = oldRemote;
	    remoteHost->setEditText( remoteOperator.host() );
	    remotePath->setEditText( remoteOperator.path() );
	    password->setText( remoteOperator.password() );
	    userName->setEditText( remoteOperator.user() );
	    port->setText( t.sprintf( "%i", remoteOperator.port() ) );
	    remoteOperator.listChildren();
		}
  }
  else if ( op->operation() == QNetworkProtocol::OpListChildren ) {
		// finished reading a dir? set the correct path to the pth combo of the right view
		remotePath->setEditText( remoteOperator.path() );
  }
  else if ( op->operation() == QNetworkProtocol::OpPut ) {
  	// finished saving the uploaded file? reread the dir and hide the progress bar
  	remoteOperator.listChildren();
  }
  else if ( op->operation() == QNetworkProtocol::OpGet ) {
  	// finished reading a file from the local filesystem? reset the progress bar
  	progress->setTotalSteps( 0 );
  	progress->reset();
  }
  else if ( op->operation() == QNetworkProtocol::OpRemove ) {
  	// finished reading a file from the ftp server? reset the progress bar
  	progress->setTotalSteps( 0 );
  	progress->reset();
  	remoteOperator.listChildren();
  }
}

void FtpClient::slotLocalDataTransferProgress( int bytesDone, int bytesTotal,
						   QNetworkOperation *op )
{
  // Show the progress here of the local QUrlOperator reads or writes data

  if ( !op )
		return;

  if ( !progress->isVisible() ) {
		if ( bytesDone < bytesTotal) {
	    progress->show();
	    progress->setTotalSteps( bytesTotal );
	    progress->setProgress( 0 );
	    progress->reset();
		} else
	    return;
  }

  if ( progress->totalSteps() == bytesTotal )
		progress->setTotalSteps( bytesTotal );

	return;

  progress->setProgress( bytesDone );
}

void FtpClient::slotRemoteDataTransferProgress( int bytesDone, int bytesTotal,
						    QNetworkOperation *op )
{
  // Show the progress here of the remote QUrlOperator reads or writes data

  if ( !op )
		return;

  if ( !progress->isVisible() ) {
		if ( bytesDone < bytesTotal) {
	    progress->show();
	    progress->setTotalSteps( bytesTotal );
	    progress->setProgress( 0 );
	    progress->reset();
		} else
	    return;
  }

  if ( progress->totalSteps() != bytesTotal )
		progress->setTotalSteps( bytesTotal );


  progress->setProgress( bytesDone );
}

void FtpClient::slotMkdir()
{
	if ( sel == IDLEFT  ) slotLocalMkdir();
	if ( sel == IDRIGHT ) slotRemoteMkdir();
}

void FtpClient::slotRemove()
{
	if ( sel == IDLEFT  ) slotLocalRemove();
	if ( sel == IDRIGHT ) slotRemoteRemove();
}

void FtpClient::slotCopy()
{
	if ( sel == IDLEFT  ) slotUpload();
	if ( sel == IDRIGHT ) slotDownload();
}

void FtpClient::slotMove()
{
	if ( sel == IDLEFT  ) slotUpload(true);
	if ( sel == IDRIGHT ) slotDownload(true);
}

void FtpClient::slotLocalMkdir()
{
  // create a dir on the local filesystem

  bool ok = FALSE;
  QString name = QInputDialog::getText( tr( "Directory Name:" ), QString::null, QString::null, &ok, this );

  if ( !name.isEmpty() && ok )
		localOperator.mkdir( name );
}

void FtpClient::slotRemoteMkdir()
{
  // create a dir on the remote filesystem (FTP server)

  bool ok = FALSE;
  QString name = QInputDialog::getText( tr( "Directory Name:" ), QString::null, QString::null, &ok, this );

  if ( !name.isEmpty() && ok )
		remoteOperator.mkdir( name );
}

void FtpClient::slotLocalRemove()
{
	QValueList<QUrlInfo> files = leftView->selectedItems();
  if ( files.isEmpty() )
		return;
		
 	QMessageBox mb( "Wow !!!",
    "You want delete the important information\nAre you sure?",
    QMessageBox::Critical,
    QMessageBox::Yes 			| QMessageBox::Default,
    QMessageBox::No  			| QMessageBox::Escape,
    QMessageBox::NoButton
  );

 	if ( mb.exec() == QMessageBox::No ) return;
 	
 	QValueList<QUrlInfo>::Iterator it;
 	for ( it = files.begin(); it != files.end(); ++it )
		localOperator.remove( (*it).name() );
}

void FtpClient::slotRemoteRemove()
{
	QValueList<QUrlInfo> files = rightView->selectedItems();
  if ( files.isEmpty() )
		return;
		
	QMessageBox mb( "Wow !!!",
    "You want delete the important information\nAre you sure?",
    QMessageBox::Critical,
    QMessageBox::Yes 			| QMessageBox::Default,
    QMessageBox::No  			| QMessageBox::Escape,
    QMessageBox::NoButton
  );

 	if ( mb.exec() == QMessageBox::No ) return;
 	
 	QValueList<QUrlInfo>::Iterator it;
 	for ( it = files.begin(); it != files.end(); ++it )
		remoteOperator.remove( (*it).name() );
}

void FtpClient::slotSelectLeft()
{
	sel = IDLEFT;
	rightView->clearSelection();
}

void FtpClient::slotSelectRight()
{
	sel = IDRIGHT;
	leftView->clearSelection();
}

