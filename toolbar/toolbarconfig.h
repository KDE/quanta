/****************************************************************************
** Form interface generated from reading ui file 'toolbarconfig.ui'
**
** Created: Sat Oct 14 22:41:44 2000
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef TOOLBARCONFIG_H
#define TOOLBARCONFIG_H

#include <qdialog.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QGroupBox;
class QListView;
class QListViewItem;
class QPushButton;

class ToolBarConfig : public QDialog
{ 
    Q_OBJECT

public:
    ToolBarConfig( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~ToolBarConfig();

    QGroupBox* GroupBox2;
    QListView* listToolbar;
    QPushButton* buttonToolbarUp;
    QPushButton* buttonToolbarDown;
    QPushButton* buttonToolBarRemove;
    QPushButton* buttonToolbarEdit;
    QPushButton* buttonToolbarNew;
    QGroupBox* GroupBox2_2;
    QListView* listAction;
    QPushButton* buttonActionUp;
    QPushButton* buttonActionDown;
    QPushButton* buttonActionRemove;
    QPushButton* buttonActionEdit;
    QPushButton* buttonActionNew;
    QPushButton* buttonHelp;
    QPushButton* buttonOk;
    QPushButton* buttonCancel;

public slots:
    virtual void actionDown();
    virtual void actionEdit();
    virtual void actionNew();
    virtual void actionRemove();
    virtual void actionSelected();
    virtual void actionUp();
    virtual void toolbarSelected();
    virtual void toolbarDown();
    virtual void toolbarEdit();
    virtual void toolbarNew();
    virtual void toolbarRemove();
    virtual void toolbarUp();
    virtual void updateActionsList();
    virtual void updateToolbarsList();

protected:
    QHBoxLayout* hbox;
    QGridLayout* grid;
    QGridLayout* grid_2;
    QGridLayout* grid_3;
};

#endif // TOOLBARCONFIG_H
