/****************************************************************************
** Form interface generated from reading ui file 'eventswidgetdlgs.ui'
**
** Created: Fri Nov 3 19:03:34 2000
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef EVENTSWIDGETDLGS_H
#define EVENTSWIDGETDLGS_H

#include <qvariant.h>
#include <qwidget.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QLabel;
class QLineEdit;

class EventsWidgetDlgS : public QWidget
{ 
    Q_OBJECT

public:
    EventsWidgetDlgS( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~EventsWidgetDlgS();

    QLabel* TextLabel1;
    QLabel* TextLabel2;
    QLabel* TextLabel3;
    QLabel* TextLabel4;
    QLabel* TextLabel5;
    QLabel* TextLabel6;
    QLabel* TextLabel7;
    QLabel* TextLabel8;
    QLabel* TextLabel9;
    QLabel* TextLabel10;
    QLineEdit* lineClick;
    QLineEdit* lineDblClick;
    QLineEdit* lineMouseDown;
    QLineEdit* lineMouseUp;
    QLineEdit* lineMouseOver;
    QLineEdit* lineMouseMove;
    QLineEdit* lineMouseOut;
    QLineEdit* lineKeyPress;
    QLineEdit* lineKeyDown;
    QLineEdit* lineKeyUp;

protected:
    QGridLayout* grid;
};

#endif // EVENTSWIDGETDLGS_H
