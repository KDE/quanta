/****************************************************************************
** Form interface generated from reading ui file './dtdselectdialog.ui'
**
** Created: Sun Aug 18 15:45:56 2002
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

    QComboBox* ComboBox1;
    QLabel* TextLabel1;
    QPushButton* cancelButton;
    QPushButton* okButton;


public slots:
    virtual void init();

};

#endif // DTDSELECTDIALOG_H
