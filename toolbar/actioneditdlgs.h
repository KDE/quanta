/****************************************************************************
** Form interface generated from reading ui file './actioneditdlgs.ui'
**
** Created: Thu Feb 15 22:37:55 2001
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef ACTIONEDITDLGS_H
#define ACTIONEDITDLGS_H

#include <qvariant.h>
#include <qdialog.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class FileCombo;
class KIconButton;
class QButtonGroup;
class QCheckBox;
class QLabel;
class QLineEdit;
class QListBox;
class QListBoxItem;
class QMultiLineEdit;
class QPushButton;
class QRadioButton;
class QTabWidget;
class QWidget;

class ActionEditDlgS : public QDialog
{ 
    Q_OBJECT

public:
    ActionEditDlgS( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~ActionEditDlgS();

    QLineEdit* lineStatusText;
    QTabWidget* actionTab;
    QWidget* Widget3;
    QLabel* TextLabel5_2;
    QCheckBox* useTagClose;
    QLineEdit* tag;
    QLineEdit* tagClose;
    QCheckBox* useActionDialog;
    QWidget* Widget4;
    FileCombo* scriptPath;
    QTabWidget* tabScript;
    QWidget* Widget5;
    QButtonGroup* ButtonGroup34_2;
    QRadioButton* inputNone;
    QRadioButton* inputSelected;
    QRadioButton* inputCurrent;
    QWidget* Widget6;
    QButtonGroup* ButtonGroup35_3;
    QRadioButton* outputNone;
    QRadioButton* outputCursor_2;
    QRadioButton* outputReplace;
    QRadioButton* outputNew;
    QRadioButton* outputMessage;
    QWidget* Widget7;
    QButtonGroup* ButtonGroup35_2_2;
    QRadioButton* errorNone;
    QRadioButton* errorCursor;
    QRadioButton* errorReplace;
    QRadioButton* errorNew;
    QRadioButton* errorMessage;
    QWidget* Widget8;
    QMultiLineEdit* text;
    QPushButton* buttonNew;
    QPushButton* buttonDelete;
    QPushButton* buttonOk_2;
    QPushButton* buttonCancel_2;
    QLabel* TextLabel3;
    QLineEdit* lineToolTip;
    QListBox* actionsList;
    QLabel* TextLabel2;
    QLabel* TextLabel1_2;
    QLineEdit* lineText;
    KIconButton* actionIcon;

public slots:
    virtual void deleteAction();
    virtual void newAction();
    virtual void actionSelected();

protected:
    QGridLayout* ActionEditDlgSLayout;
    QGridLayout* Widget3Layout;
    QGridLayout* Widget4Layout;
    QGridLayout* Widget5Layout;
    QGridLayout* ButtonGroup34_2Layout;
    QGridLayout* Widget6Layout;
    QGridLayout* ButtonGroup35_3Layout;
    QGridLayout* Widget7Layout;
    QVBoxLayout* ButtonGroup35_2_2Layout;
    QGridLayout* Widget8Layout;
    QHBoxLayout* Layout5;
};

#endif // ACTIONEDITDLGS_H
