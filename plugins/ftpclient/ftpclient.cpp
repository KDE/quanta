
// Qt includes
#include <qdialog.h>
#include <qwidget.h>
#include <qnetwork.h>
#include <qlistview.h>
#include <qprogressbar.h>
#include <qurloperator.h>
#include <qlayout.h>
#include <qmessagebox.h>
#include <qdir.h>

// Kde includes
#include <klocale.h>
#include <ktoolbar.h>
#include <kiconloader.h>

// app headers
#include "ftpclient.h"
#include "ftpclients.h"
#include "ftpclientview.h"

/**** DLL Interface ****/

extern "C" {
  QDialog *create_ftpclient(QWidget *parent, const char *name) 
  {
    return new FtpClient(parent,name);
  }
}

// Class Implementation

FtpClient::FtpClient( QWidget* parent, const char* name, WFlags fl )
  :QDialog(parent, name, true, fl),localOperator( "/" )
{
  setCaption( i18n("Small FTP client") );
  
  resize(500,400);
  
  client = new FtpClientS( this, "Ftp Client");
  
  QGridLayout *layout = new QGridLayout( this ); 
  layout->addWidget( client, 0, 0 );
  layout->setSpacing(0);
  layout->setMargin( 0);
  
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
  
  connect(client->comboDirLocal,SIGNAL( activated(const QString&) ),
          this, SLOT(slotLocalStart(const QString&)));
  connect(&localOperator,SIGNAL( start( QNetworkOperation *)),
          this, SLOT(slotLocalStart( QNetworkOperation *)));
  connect(&localOperator,SIGNAL( finished( QNetworkOperation *)),
          this, SLOT(slotLocalFinished( QNetworkOperation *)));
          
  connect(&localOperator,SIGNAL( newChildren( const QValueList<QUrlInfo> &, QNetworkOperation *)),
          client->listViewLocal, SLOT  ( slotInsertEntries( const QValueList<QUrlInfo> & ) ) );
          
  localOperator.setPath( QDir::homeDirPath() );
  localOperator.listChildren();
}

FtpClient::~FtpClient()
{
}

void FtpClient::slotLocalStart(const QString & dir)
{
  localOperator.setPath(dir);
  localOperator.listChildren();
}

void FtpClient::slotLocalStart( QNetworkOperation *op )
{
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
  if ( !op ) return;
 
  if ( op && op->state() == QNetworkProtocol::StFailed )
  {
    // an error happend, let the user know that
    QMessageBox::critical( this, i18n( "ERROR" ), op->protocolDetail() );
 
    // do something depending in the error code
    int ecode = op->errorCode();
    if ( ecode == QNetworkProtocol::ErrListChlidren || 
         ecode == QNetworkProtocol::ErrUnknownProtocol || 
         ecode == QNetworkProtocol::ErrValid || 
         ecode == QNetworkProtocol::ErrFileNotExisting )
    { localOperator.listChildren();}
  }
  else if ( op->operation() == QNetworkProtocol::OpPut ) {
  }
  else if ( op->operation() == QNetworkProtocol::OpGet ) {
    // finished reading a file from the ftp server? reset the progress bar
    //progress->setTotalSteps( 0 );
    //progress->reset();
  }
  else if ( op->operation() == QNetworkProtocol::OpRemove ) {
    // finished reading a file from the ftp server? reset the progress bar
    //progress->setTotalSteps( 0 );
    //progress->reset();
    localOperator.listChildren();
  }
}
