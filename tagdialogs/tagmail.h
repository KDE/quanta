/****************************************************************************
** Form interface generated from reading ui file './tagmail.ui'
**
** Created: Mon May 20 11:27:20 2002
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef TAGMAIL_H
#define TAGMAIL_H

#include <qvariant.h>
#include <qdialog.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QLabel;
class QLineEdit;
class QPushButton;

class TagMail : public QDialog
{ 
    Q_OBJECT

public:
    TagMail( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~TagMail();

    QPushButton* buttonHelp;
    QPushButton* buttonOk;
    QPushButton* buttonCancel;
    QLabel* TextLabel2;
    QLabel* TextLabel2_2;
    QLabel* TextLabel1;
    QLineEdit* lineEmail;
    QLineEdit* lineSubject;
    QLineEdit* lineTitle;
    QPushButton* buttonAddressSelect;


protected:
    QHBoxLayout* Layout9;
    QVBoxLayout* Layout8;
    QVBoxLayout* Layout2;
};

#endif // TAGMAIL_H
