/****************************************************************************
** Form interface generated from reading ui file 'project1.ui'
**
** Created: Fri Oct 27 23:20:30 2000
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef PROJECTNEWGENERALS_H
#define PROJECTNEWGENERALS_H

#include <qvariant.h>
#include <qwidget.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QButtonGroup;
class QLabel;
class QLineEdit;
class QPushButton;
class QRadioButton;

class ProjectNewGeneralS : public QWidget
{ 
    Q_OBJECT

public:
    ProjectNewGeneralS( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~ProjectNewGeneralS();

    QLabel* TextLabel2;
    QLabel* TextLabel3;
    QLineEdit* lineAuthor;
    QLineEdit* linePrjFile;
    QLineEdit* lineEmail;
    QLabel* TextLabel4;
    QLabel* TextLabel5;
    QLabel* TextLabel1;
    QLineEdit* linePrjName;
    QButtonGroup* ButtonGroup1_2;
    QRadioButton* radioLocal;
    QRadioButton* radioWeb;
    QRadioButton* radioCvs;
    QPushButton* buttonDir;
    QLineEdit* linePrjDir;

protected:
    QGridLayout* grid;
    QGridLayout* grid_2;
};

#endif // PROJECTNEWGENERALS_H
