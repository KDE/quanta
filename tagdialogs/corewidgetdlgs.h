/****************************************************************************
** Form interface generated from reading ui file 'corewidgetdlg.ui'
**
** Created: Thu Nov 2 18:48:27 2000
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef COREWIDGETDLGS_H
#define COREWIDGETDLGS_H

#include <qvariant.h>
#include <qwidget.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QFrame;
class QLabel;
class QLineEdit;

class CoreWidgetDlgS : public QWidget
{ 
    Q_OBJECT

public:
    CoreWidgetDlgS( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~CoreWidgetDlgS();

    QLabel* TextLabel1;
    QLabel* TextLabel2;
    QLabel* TextLabel3;
    QLabel* TextLabel4;
    QFrame* Line1;
    QLabel* TextLabel5;
    QLabel* TextLabel6;
    QLineEdit* lineId;
    QLineEdit* lineClass;
    QLineEdit* lineStyle;
    QLineEdit* lineTitle;
    QLineEdit* lineLang;
    QLineEdit* lineDir;

protected:
    QGridLayout* grid;
};

#endif // COREWIDGETDLGS_H
