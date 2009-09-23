/***************************************************************************
    begin                : Tue May 24 2005
    copyright            : (C) 2005 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/

#include "myprocess.h"
//other includes
#include <sys/types.h>
#include <unistd.h>

MyProcess::MyProcess():KProcess()
{
}

void MyProcess::setupChildProcess()
{
#if defined Q_OS_UNIX
  ::setpgid(pid(), 0);
#endif
}

#include "myprocess.moc"
