/****************************************************************************
** Form interface generated from reading ui file './quicktable.ui'
**
** Created: Mon May 20 11:27:20 2002
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef QUICK_TABLE_H
#define QUICK_TABLE_H

#include <qvariant.h>
#include <qdialog.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QCheckBox;
class QFrame;
class QLabel;
class QListView;
class QListViewItem;
class QPushButton;
class QSpinBox;

class Quick_Table : public QDialog
{ 
    Q_OBJECT

public:
    Quick_Table( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~Quick_Table();

    QSpinBox* SpinBoxCol;
    QLabel* TextLabel3;
    QLabel* TextLabel3_2;
    QSpinBox* SpinBoxRow;
    QCheckBox* CheckBox4;
    QCheckBox* useTHead;
    QCheckBox* useTFoot;
    QPushButton* buttonHelp;
    QPushButton* buttonApply;
    QPushButton* buttonOk;
    QPushButton* buttonCancel;
    QFrame* Line1;
    QListView* ListView1;
    QPushButton* newColumn;


protected:
    QVBoxLayout* Layout10;
    QGridLayout* Layout6;
    QHBoxLayout* Layout1;
};

#endif // QUICK_TABLE_H
