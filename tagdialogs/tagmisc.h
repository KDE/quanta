/****************************************************************************
** Form interface generated from reading ui file './tagmisc.ui'
**
** Created: Mon May 20 11:27:20 2002
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef TAGMISC_H
#define TAGMISC_H

#include <qvariant.h>
#include <qdialog.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButton;

class TagMisc : public QDialog
{ 
    Q_OBJECT

public:
    TagMisc( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~TagMisc();

    QLineEdit* elementName;
    QLabel* TextLabel1;
    QCheckBox* addClosingTag;
    QPushButton* buttonCancel;
    QPushButton* buttonOk;
    QPushButton* buttonHelp;


protected:
    QGridLayout* Layout4;
    QGridLayout* Layout5;
};

#endif // TAGMISC_H
