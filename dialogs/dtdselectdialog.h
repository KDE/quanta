/****************************************************************************
** Form interface generated from reading ui file './dtdselectdialog.ui'
**
** Created: Fri Aug 23 22:11:02 2002
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
class QPushButton;

class DTDSelectDialog : public QDialog
{ 
    Q_OBJECT

public:
    DTDSelectDialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~DTDSelectDialog();

    QLabel* TextLabel1;
    QPushButton* cancelButton;
    QPushButton* okButton;
    QComboBox* dtdCombo;


public slots:
    virtual void init();

};

#endif // DTDSELECTDIALOG_H
