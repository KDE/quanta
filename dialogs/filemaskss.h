/****************************************************************************
** Form interface generated from reading ui file 'filemasks.ui'
**
** Created: Tue Oct 31 21:07:27 2000
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef FILEMASKSS_H
#define FILEMASKSS_H

#include <qvariant.h>
#include <qwidget.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QLabel;
class QLineEdit;
class QPushButton;

class FileMasksS : public QWidget
{ 
    Q_OBJECT

public:
    FileMasksS( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~FileMasksS();

    QLabel* TextLabel4;
    QLabel* TextLabel3;
    QLabel* TextLabel2;
    QLabel* TextLabel1;
    QLineEdit* lineHTML;
    QLineEdit* linePHP;
    QLineEdit* lineImages;
    QLineEdit* lineText;
    QPushButton* buttonDefault;

protected:
    QGridLayout* grid;
};

#endif // FILEMASKSS_H
