
// Qt includes
#include <qdialog.h>
#include <qwidget.h>
#include <qnetwork.h>
#include <qlistview.h>
#include <qprogressbar.h>
#include <qurloperator.h>

// Kde includes
#include <klocale.h>
#include <ktoolbar.h>
#include <kiconloader.h>

// app headers
#include "ftpclient.h"
#include "ftpclients.h"

/**** DLL Interface ****/

extern "C" {
  QDialog *create_ftpclient(QWidget *parent, const char *name) 
  {
    return new FtpClient(parent,name);
  }
}

// Class Implementation

FtpClient::FtpClient( QWidget* parent, const char* name, WFlags fl )
  :QDialog(parent, name, true, fl)
{
  setCaption( i18n("Small FTP client") );
  
  client = new FtpClientS( this, "Ftp Client");
  
  client->toolBar->insertButton(BarIcon("connect_creating"), ID_CONNECT, true, i18n("Connect"));
  client->toolBar->insertButton(BarIcon("connect_no"),    ID_DISCONNECT, true, i18n("Disconnect"));
  client->toolBar->insertSeparator();
  client->toolBar->insertSeparator();
  client->toolBar->insertButton(BarIcon("back"), ID_DOWNLOAD, true, i18n("Download"));
  client->toolBar->insertButton(BarIcon("reload"), ID_RELOAD, true, i18n("Reload"));
  client->toolBar->insertButton(BarIcon("forward"),ID_UPLOAD, true, i18n("Upload"));
  client->toolBar->insertSeparator();
  client->toolBar->insertSeparator();
  client->toolBar->insertButton(BarIcon("folder_new"), ID_MAKEDIR, true, i18n("Make Dir"));
  client->toolBar->insertButton(UserIcon("delete"),    ID_DELETE,  true, i18n("Delete"));
  
  client->toolBar->setIconText(KToolBar::IconTextBottom);
  
  qInitNetworkProtocols();
  
  connect(&localOperator,SIGNAL( start( QNetworkOperation *)),
          this, SLOT(slotLocalStart( QNetworkOperation *)));
  
  localOperator.setPath( QDir::homeDirPath() );
  localOperator.listChildren();
}

FtpClient::~FtpClient()
{
}

void FtpClient::slotLocalStart( QNetworkOperation *op )
{
  // this slot is always called if the local QUrlOperator starts
  // listing a directory or dowloading a file

  if ( !op )	return;

  if ( op->operation() == QNetworkProtocol::OpListChildren )
  {
		client->listViewLocal->clear();
  }
  else if ( op->operation() == QNetworkProtocol::OpGet )
  {
 		client->progress->setTotalSteps( 0 );
 		client->progress->reset();
   }
}

void FtpClient::slotLocalFinished( QNetworkOperation *op )
{
}
