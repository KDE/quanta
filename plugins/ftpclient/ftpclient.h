#ifndef FTPCLIENT_H
#define FTPCLIENT_H

// app headers
#include <qdialog.h>

class FtpClient : public QDialog
{ 
    Q_OBJECT

public:
    FtpClient( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~FtpClient();
    
};
    
#endif

