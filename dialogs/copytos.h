/****************************************************************************
** Form interface generated from reading ui file './copytos.ui'
**
** Created: Mon Sep 23 19:01:16 2002
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
class KURLRequester;
class QPushButton;

class CopyToS : public QDialog
{ 
    Q_OBJECT

public:
    CopyToS( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~CopyToS();

    QPushButton* buttonCancel;
    QPushButton* buttonOk;
    KURLRequester* urlRequester;


protected:
    QGridLayout* CopyToSLayout;
};

#endif // COPYTOS_H
