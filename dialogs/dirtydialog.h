/****************************************************************************
** Form interface generated from reading ui file './dirtydialog.ui'
**
** Created: Fri Sep 13 16:30:50 2002
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef DIRTYDIALOG_H
#define DIRTYDIALOG_H

#include <qvariant.h>
#include <qdialog.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QButtonGroup;
class QLabel;
class QPushButton;
class QRadioButton;

class DirtyDialog : public QDialog
{ 
    Q_OBJECT

public:
    DirtyDialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~DirtyDialog();

    QPushButton* okButton;
    QPushButton* cancelButton;
    QLabel* TextLabel1;
    QButtonGroup* ButtonGroup2;
    QRadioButton* buttonLoad;
    QLabel* TextLabel2;
    QRadioButton* buttonIgnore;
    QRadioButton* buttonCompare;


};

#endif // DIRTYDIALOG_H
