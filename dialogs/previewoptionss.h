/****************************************************************************
** Form interface generated from reading ui file 'previewoptions.ui'
**
** Created: Tue Oct 31 22:17:12 2000
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef PREVIEWOPTIONSS_H
#define PREVIEWOPTIONSS_H

#include <qvariant.h>
#include <qwidget.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QButtonGroup;
class QLabel;
class QRadioButton;

class PreviewOptionsS : public QWidget
{ 
    Q_OBJECT

public:
    PreviewOptionsS( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~PreviewOptionsS();

    QLabel* pixmap;
    QButtonGroup* ButtonGroup1;
    QRadioButton* radioRight;
    QRadioButton* radioBottom;
    QRadioButton* radioFullworkspace;
    QRadioButton* radioDisabled;

protected:
    QGridLayout* grid;
    QGridLayout* grid_2;
};

#endif // PREVIEWOPTIONSS_H
