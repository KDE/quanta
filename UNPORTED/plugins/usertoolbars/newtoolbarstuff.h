/***************************************************************************
    begin                : Thu May 19 2005
    copyright            : (C) 2005 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/
#ifndef NEWTOOLBARSTUFF_H
#define NEWTOOLBARSTUFF_H

//qt includes
#include <QObject>
#include <QMap>

//kde includes

class KUrl;
/**
Makes possible downloading and installing a Toolbar resource files from a server.


@author Andras Mantia
*/

//FIXME: Port to KNewStuff2
/*
class NewToolbarStuff : public KNewStuffSecure
{
   Q_OBJECT

public:
  explicit NewToolbarStuff(const QString &type,  QWidget *parentWidget=0);
  ~NewToolbarStuff() {};
  
Q_SIGNALS:
  void loadToolbarFile(const KUrl&);       

private:
   virtual void installResource();
};
*/

#endif
