
// Qt includes
#include <qdialog.h>
#include <qwidget.h>

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
  new FtpClientS( this, "Ftp Client");
}

FtpClient::~FtpClient()
{
}
