#include "fwglobal.h"

int proxInt(double d){
  if((d-int(d))>= 0.5 ) return int(d)+1;
  return int(d);
}
