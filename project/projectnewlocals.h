/****************************************************************************
** Form interface generated from reading ui file 'project-local.ui'
**
** Created: Sat Oct 21 03:13:53 2000
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef PROJECTNEWLOCALS_H
#define PROJECTNEWLOCALS_H

#include <qvariant.h>
#include <qwidget.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QCheckBox;
class QLabel;
class QLineEdit;
class QListView;
class QListViewItem;

class ProjectNewLocalS : public QWidget
{ 
    Q_OBJECT

public:
    ProjectNewLocalS( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~ProjectNewLocalS();

    QCheckBox* checkInsert;
    QCheckBox* checkInsertWithMask;
    QLineEdit* mask;
    QLineEdit* webmask;
    QLabel* TextLabel8;
    QCheckBox* checkInsertWeb;
    QListView* listFiles;

protected:
    QGridLayout* grid;
};

#endif // PROJECTNEWLOCALS_H
