#ifndef ACTIONEDITDLG_H
#define ACTIONEDITDLG_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "actioneditdlgs.h"

class KActionCollection;

/** configure actions
  *@author Dmitry Poplavsky
  */
  
class ActionEditDlg : public ActionEditDlgS {
  Q_OBJECT
  
public:
  
  ActionEditDlg( KActionCollection *col, QWidget* parent = 0, const char* name = 0, bool modal = true, WFlags fl = 0 );
  ~ActionEditDlg();
  
public slots:
    virtual void deleteAction();
    virtual void newAction();
    virtual void actionSelected();
};

#endif

