#ifndef TOOLBARCONFIGI_H
#define TOOLBARCONFIGI_H

#include "toolbarconfig.h"

class ToolBars;

class ToolBarConfigI : public ToolBarConfig
{ 
    Q_OBJECT

public:
    ToolBarConfigI( ToolBars *t, QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~ToolBarConfigI();

public slots:
    void actionDown();
    void actionEdit();
    void actionNew();
    void actionRemove();
    void actionSelected();
    void actionUp();
    void toolbarSelected();
    void toolbarDown();
    void toolbarEdit();
    void toolbarNew();
    void toolbarRemove();
    void toolbarUp();
    void updateActionsList();
    void updateToolbarsList();

private:
    ToolBars *t;
    int currentToolbar;
    int currentAction;

    int currentToolbarPos();
    int currentActionPos();

//    QDomElement *currentToolbar;
//    QDomElement *currentAction;
};

#endif // TOOLBARCONFIGI_H
