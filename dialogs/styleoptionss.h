/****************************************************************************
** Form interface generated from reading ui file 'styleoptionss.ui'
**
** Created: Tue Oct 31 23:14:55 2000
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef STYLEOPTIONSS_H
#define STYLEOPTIONSS_H

#include <qvariant.h>
#include <qwidget.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QCheckBox;

class StyleOptionsS : public QWidget
{ 
    Q_OBJECT

public:
    StyleOptionsS( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~StyleOptionsS();

    QCheckBox* checkTagsCapital;
    QCheckBox* checkAttrCapital;
    QCheckBox* checkEndTag;

protected:
    QGridLayout* grid;
};

#endif // STYLEOPTIONSS_H
