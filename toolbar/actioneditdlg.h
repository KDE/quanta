#ifndef ACTIONEDITDLG_H
#define ACTIONEDITDLG_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "actioneditdlgs.h"
#include <qdom.h>

class TagAction;
class KShortcut;

/** configure actions
  *@author Dmitry Poplavsky, Andras Mantia
  */
  
class ActionEditDlg : public ActionEditDlgS {
  Q_OBJECT
  
public:
  
  ActionEditDlg( QWidget* parent = 0, const char* name = 0, bool modal = true, WFlags fl = 0 );
  ~ActionEditDlg();
  
public slots:
    virtual void deleteAction();
    virtual void newAction();
    virtual void actionSelected();
    virtual void accept() { saveAction(action); ActionEditDlgS::accept(); }
private slots:
    void slotShortcutCaptured(const KShortcut&);
protected:
    TagAction *action;

    void loadAction( TagAction *);
    void saveAction( TagAction *);
    TagAction *selectedAction();
};

#endif

