/***************************************************************************
                          fwglobal.cpp  -  description
                             -------------------
    begin                : mar feb 17 2004
    copyright            : (C) 2003 by gulmini luciano
    email                : gulmini.luciano@student.unife.it
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "fwglobal.h"

int proxInt(double d){
  if((d-int(d))>= 0.5 ) return int(d)+1;
  return int(d);
}
