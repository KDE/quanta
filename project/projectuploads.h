/****************************************************************************
** Form interface generated from reading ui file './projectuploads.ui'
**
** Created: Tue Feb 6 13:25:13 2001
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
class QLabel;
class QLineEdit;
class QListView;
class QListViewItem;
class QProgressBar;
class QPushButton;
class QSpinBox;

class ProjectUploadS : public QDialog
{ 
    Q_OBJECT

public:
    ProjectUploadS( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~ProjectUploadS();

    QPushButton* buttonAll;
    QPushButton* buttonModified;
    QPushButton* buttonClear;
    QListView* list;
    QPushButton* buttonCancel;
    QPushButton* buttonUpload;
    QProgressBar* ProgressBar1;
    QLabel* TextLabel2_2;
    QLabel* TextLabel4;
    QLabel* TextLabel2;
    QLabel* labelCurFile;
    QLineEdit* lineUser;
    QLabel* TextLabel3;
    QLineEdit* linePasswd;
    QLineEdit* linePath;
    QSpinBox* spinPort;
    QLabel* TextLabel1_2;
    QLineEdit* lineHost;

public slots:
    virtual void clearSelection();
    virtual void startUpload();
    virtual void selectAll();
    virtual void selectModified();

protected:
    QGridLayout* ProjectUploadSLayout;
};

#endif // PROJECTUPLOADS_H
