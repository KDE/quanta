
#include "phpdebugsocket.h"
#include <kdebug.h>
#include <klocale.h>

#include "quanta.h"
#include "resource.h"

// CTor
PHPDebugSocket::PHPDebugSocket(PHPDebuggerInterface * iface)
{
  // Save pointer to debugger interface
  m_interface = iface;
}

