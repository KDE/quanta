/***************************************************************************
                          myprocess.h
                             -------------------
    begin                : Jun 25 2003
    copyright            : (C) 2003 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/

#ifndef MYPROCESS_H
#define MYPROCESS_H

#include <kprocess.h>

class MyProcess:public KProcess
{
  Q_OBJECT

  public:
    MyProcess();
    virtual ~MyProcess() {};

  protected:
    virtual int commSetupDoneC();
};



#endif

