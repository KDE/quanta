/****************************************************************************
** Form interface generated from reading ui file './dtdselectdialog.ui'
**
** Created: Sat Aug 24 15:27:53 2002
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef DTDSELECTDIALOG_H
#define DTDSELECTDIALOG_H

#include <qvariant.h>
#include <qdialog.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QComboBox;
class QLabel;
class QLineEdit;
class QPushButton;

class DTDSelectDialog : public QDialog
{ 
    Q_OBJECT

public:
    DTDSelectDialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~DTDSelectDialog();

    QLabel* messageLabel;
    QComboBox* dtdCombo;
    QLabel* TextLabel1_2;
    QLabel* TextLabel1;
    QLineEdit* currentDTD;
    QPushButton* okButton;


public slots:
    virtual void init();

protected:
    QGridLayout* Layout1;
};

#endif // DTDSELECTDIALOG_H
