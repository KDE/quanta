#ifndef FTPCLIENT_H
#define FTPCLIENT_H

// app headers
#include <qdialog.h>
#include <qurloperator.h>

#define ID_CONNECT      100010
#define ID_DISCONNECT   100020
#define ID_DOWNLOAD     100030
#define ID_UPLOAD       100040
#define ID_RELOAD       100050
#define ID_MAKEDIR      100060
#define ID_DELETE       100070

class FtpClientS;
class QNetworkOperation;

class FtpClient : public QDialog
{ 
    Q_OBJECT

public:
    FtpClient( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~FtpClient();

public:   
    FtpClientS *client;
    QUrlOperator localOperator, remoteOperator;
    
private slots:
  void slotLocalStart( QNetworkOperation * );
  void slotLocalFinished( QNetworkOperation * );
 
};
    
#endif

