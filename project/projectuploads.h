/****************************************************************************
** Form interface generated from reading ui file './projectuploads.ui'
**
** Created: Sun Feb 11 21:16:47 2001
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef PROJECTUPLOADS_H
#define PROJECTUPLOADS_H

#include <qvariant.h>
#include <qdialog.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QComboBox;
class QLabel;
class QLineEdit;
class QListView;
class QListViewItem;
class QProgressBar;
class QPushButton;

class ProjectUploadS : public QDialog
{ 
    Q_OBJECT

public:
    ProjectUploadS( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~ProjectUploadS();

    QListView* list;
    QProgressBar* ProgressBar1;
    QLabel* labelCurFile;
    QPushButton* buttonAll;
    QPushButton* buttonModified;
    QPushButton* buttonClear;
    QPushButton* buttonUpload;
    QPushButton* buttonCancel;
    QLabel* TextLabel2_2;
    QLabel* TextLabel2;
    QLineEdit* linePasswd;
    QLineEdit* lineHost;
    QComboBox* comboProtocol;
    QLabel* TextLabel1;
    QLineEdit* lineUser;
    QLabel* TextLabel4;
    QLabel* TextLabel1_2;
    QLineEdit* port;
    QLineEdit* linePath;
    QLabel* TextLabel3;

public slots:
    virtual void clearSelection();
    virtual void startUpload();
    virtual void selectAll();
    virtual void selectModified();

protected:
    QGridLayout* ProjectUploadSLayout;
    QVBoxLayout* Layout1;
    QGridLayout* Layout3;
};

#endif // PROJECTUPLOADS_H
