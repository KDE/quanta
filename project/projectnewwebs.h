/****************************************************************************
** Form interface generated from reading ui file 'project-web.ui'
**
** Created: Fri Oct 27 22:11:20 2000
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef PROJECTNEWWEBS_H
#define PROJECTNEWWEBS_H

#include <qvariant.h>
#include <qwidget.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QLabel;
class QLineEdit;
class QListView;
class QListViewItem;
class QProgressBar;
class QPushButton;

class ProjectNewWebS : public QWidget
{ 
    Q_OBJECT

public:
    ProjectNewWebS( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~ProjectNewWebS();

    QLineEdit* siteUrl;
    QLabel* TextLabel9;
    QLabel* TextLabel10;
    QLineEdit* commandLine;
    QListView* listFiles;
    QLabel* textFromTo;
    QProgressBar* progress;
    QPushButton* button;

protected:
    QHBoxLayout* hbox;
    QGridLayout* grid;
};

#endif // PROJECTNEWWEBS_H
