/****************************************************************************
** Form interface generated from reading ui file 'ftp-client.ui'
**
** Created: Fri Dec 1 23:08:43 2000
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef FTPCLIENTS_H
#define FTPCLIENTS_H

#include <qvariant.h>
#include <qwidget.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QButtonGroup;
class QComboBox;
class QFrame;
class QLabel;
class QLineEdit;
class QListView;
class QListViewItem;
class QProgressBar;
class QPushButton;
class FtpClientView;

class FtpClientS : public QWidget
{ 
    Q_OBJECT

public:
    FtpClientS( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~FtpClientS();

    FtpClientView* leftView;
    QProgressBar* progress;
    FtpClientView* rightView;
    QButtonGroup* ButtonGroup1;
    QLabel* TextLabel19;
    QLabel* TextLabel16;
    QLabel* TextLabel18;
    QLabel* TextLabel20;
    QComboBox* remoteHost;
    QComboBox* remotePath;
    QComboBox* userName;
    QLineEdit* password;
    QLabel* TextLabel17;
    QLineEdit* port;
    QPushButton* connectButton;
    QFrame* Frame6;
    QPushButton* buttonF5;
    QPushButton* buttonF8;
    QPushButton* buttonF1;
    QPushButton* buttonF6;
    QPushButton* buttonF7;
    QPushButton* buttonF10;

protected:
    QGridLayout* FtpClientSLayout;
    QGridLayout* ButtonGroup1Layout;
    QGridLayout* Frame6Layout;
};

#endif // FTPCLIENTS_H
