/****************************************************************************
** Form interface generated from reading ui file 'copytos.ui'
**
** Created: Sat Oct 21 03:25:55 2000
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef COPYTOS_H
#define COPYTOS_H

#include <qvariant.h>
#include <qdialog.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QLineEdit;
class QPushButton;

class CopyToS : public QDialog
{ 
    Q_OBJECT

public:
    CopyToS( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~CopyToS();

    QPushButton* buttonCancel;
    QPushButton* buttonOk;
    QPushButton* buttonDir;
    QLineEdit* lineDir;

protected:
    QGridLayout* grid;
};

#endif // COPYTOS_H
