/****************************************************************************
** Form interface generated from reading ui file 'ftp-client-2.ui'
**
** Created: Fri Jan 12 20:04:21 2001
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
class KToolBar;
class QComboBox;
class QGroupBox;
class QLabel;
class QListView;
class QListViewItem;
class QProgressBar;
class QPushButton;

class FtpClientS : public QWidget
{ 
    Q_OBJECT

public:
    FtpClientS( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~FtpClientS();

    QGroupBox* GroupBox1_2;
    QLabel* TextLabel3_2;
    QComboBox* comboDirRemote;
    QPushButton* buttonRemote;
    QListView* listViewRemote;
    KToolBar* toolBar;
    QGroupBox* GroupBox3;
    QLabel* statusText;
    QGroupBox* GroupBox1;
    QLabel* TextLabel3;
    QComboBox* comboDirLocal;
    QPushButton* buttonLocal;
    QListView* listViewLocal;
    QProgressBar* progress;

protected:
    QGridLayout* FtpClientSLayout;
    QGridLayout* Layout22;
    QVBoxLayout* Layout21;
    QGridLayout* GroupBox1_2Layout;
    QHBoxLayout* Layout10_2;
    QGridLayout* GroupBox3Layout;
    QVBoxLayout* Layout20;
    QGridLayout* GroupBox1Layout;
    QHBoxLayout* Layout10;
};

#endif // FTPCLIENTS_H
