/****************************************************************************
** Form interface generated from reading ui file 'project-upload.ui'
**
** Created: Mon Nov 6 03:51:34 2000
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
class QPushButton;

class ProjectUploadS : public QDialog
{ 
    Q_OBJECT

public:
    ProjectUploadS( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~ProjectUploadS();

    QPushButton* buttonOk;
    QPushButton* buttonCancel;
    QPushButton* buttonHelp;
    QLabel* TextLabel4;
    QLabel* TextLabel2;
    QLabel* TextLabel3;
    QListView* list;
    QLineEdit* linePasswd;
    QLineEdit* lineUser;
    QLineEdit* lineUrl;
    QPushButton* buttonAll;
    QPushButton* buttonModified;
    QPushButton* buttonClear;

protected:
    QVBoxLayout* vbox;
    QGridLayout* grid;
};

#endif // PROJECTUPLOADS_H
