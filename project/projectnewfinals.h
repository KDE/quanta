/****************************************************************************
** Form interface generated from reading ui file 'project-final.ui'
**
** Created: Sat Nov 4 01:22:40 2000
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef PROJECTNEWFINALS_H
#define PROJECTNEWFINALS_H

#include <qvariant.h>
#include <qwidget.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QCheckBox;
class QFrame;
class QLabel;
class QLineEdit;
class QPushButton;

class ProjectNewFinalS : public QWidget
{ 
    Q_OBJECT

public:
    ProjectNewFinalS( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~ProjectNewFinalS();

    QLineEdit* lineUrl;
    QLabel* TextLabel3;
    QLabel* TextLabel4;
    QLabel* TextLabel5;
    QLineEdit* lineUser;
    QLineEdit* linePasswd;
    QFrame* Line1;
    QLabel* TextLabel6;
    QLineEdit* linePrefix;
    QCheckBox* checkPublishing;
    QCheckBox* checkPrefix;
    QCheckBox* checkFilter;
    QLabel* TextLabel7;
    QLineEdit* lineFilter;
    QPushButton* buttonFilter;

protected:
    QGridLayout* grid;
};

#endif // PROJECTNEWFINALS_H
