
#include <dcopobject.h>

class DCOPWindowManagerIf : virtual public DCOPObject
{
  K_DCOP

  k_dcop:
  
  virtual int currentEditorIfNum() const = 0;
};
