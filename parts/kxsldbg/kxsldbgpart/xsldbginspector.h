/** 
   File : xsldbginspector.h 
   Author : Keith Isdale 
   Date : 30th March 2002 
   Description : Dialog to inspect stylesheet using xsldbg. Based on 
                  file created by uic 
*/ 
#ifndef XSLDBGINSPECTOR_H 
#define XSLDBGINSPECTOR_H 
 
#include <qvariant.h> 
#include <qdialog.h> 
class QVBoxLayout;  
class QHBoxLayout;  
class QGridLayout;  
class QListBox; 
class QListBoxItem; 
class QPushButton; 
class QTabWidget; 
class QWidget; 
 
class XsldbgDebugger; 
class XsldbgBreakpointsImpl; 
class XsldbgLocalVariablesImpl; 
class XsldbgGlobalVariablesImpl; 
class XsldbgCallStackImpl; 
class XsldbgTemplatesImpl; 
class XsldbgSourcesImpl; 
class XsldbgEntitiesImpl; 
 
class XsldbgInspector : public QDialog 
{  
    Q_OBJECT 
 
public: 
    XsldbgInspector( XsldbgDebugger *debugger, QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 ); 
    ~XsldbgInspector(); 
 
    QTabWidget* tabWidget; 
    QPushButton* buttonHelp; 
    QPushButton* buttonOk; 
    QPushButton* buttonApply; 
    QPushButton* buttonRefresh; 
    QPushButton* buttonCancel; 
 
 
public slots: 
   void accept(); 
   void reject(); 
   void refresh(); 
   void refreshBreakpoints(); 
   void refreshVariables(); 
    
 
protected: 
    QGridLayout* XsldbgInspectorLayout; 
    QHBoxLayout* Layout1; 
    XsldbgDebugger *debugger; 
    XsldbgBreakpointsImpl *breakpointWidget; 
    XsldbgLocalVariablesImpl *localWidget; 
    XsldbgGlobalVariablesImpl *globalWidget; 
    XsldbgCallStackImpl *callStackWidget; 
    XsldbgTemplatesImpl *templateWidget; 
    XsldbgSourcesImpl *sourceWidget; 
    XsldbgEntitiesImpl *entityWidget; 
}; 
 
#endif // XSLDBGINPECTOR_H 
