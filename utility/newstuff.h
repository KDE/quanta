/***************************************************************************
                          newstuff.h  -  description
                             -------------------
    begin                : Tue Jun 22 12:19:55 2004
    copyright          : (C) 2004 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/

#ifndef NEWSTUFF_H
#define NEWSTUFF_H

//qt includes
#include <qobject.h>

//kde includes
#include <knewstuff/knewstuffsecure.h>
/**
Makes possible downloading and installing a DTEP resource files from a server.

@author Andras Mantia
*/

class KURL;

class QNewDTEPStuff: public KNewStuffSecure
{
   Q_OBJECT

public:
    QNewDTEPStuff(const QString &type,  QWidget *parentWidget=0)
      :KNewStuffSecure(type, parentWidget){};
    ~QNewDTEPStuff() {};

private:
     virtual void installResource();
};

/**
Makes possible downloading and installing a Toolbar resource files from a server.

@author Andras Mantia
*/
class QNewToolbarStuff: public KNewStuffSecure
{
   Q_OBJECT

public:
  QNewToolbarStuff(const QString &type,  QWidget *parentWidget=0);
  ~QNewToolbarStuff() {};
  
signals:
  void loadToolbarFile(const KURL&);       

private:
   virtual void installResource();
};

/**
Makes possible downloading and installing a template resource files from a server.

@author Andras Mantia
*/
class QNewTemplateStuff: public KNewStuffSecure
{
   Q_OBJECT

public:
  QNewTemplateStuff(const QString &type,  QWidget *parentWidget=0);
  ~QNewTemplateStuff() {};

signals:
  void openFile(const KURL&);
  
private:
   virtual void installResource();
};

/**
Makes possible downloading and installing a script resource files from a server.

@author Andras Mantia
*/
class QNewScriptStuff: public KNewStuffSecure
{
   Q_OBJECT

public:
    QNewScriptStuff(const QString &type,  QWidget *parentWidget=0)
      :KNewStuffSecure(type, parentWidget){};
    ~QNewScriptStuff() {};

private:
     virtual void installResource();
};

/**
Makes possible downloading and installing a documentation resource files from a server.

@author Andras Mantia
 */
class QNewDocStuff: public KNewStuffSecure
{
  Q_OBJECT

  public:
    QNewDocStuff(const QString &type,  QWidget *parentWidget=0)
  :KNewStuffSecure(type, parentWidget){};
    ~QNewDocStuff() {};

  private:
    virtual void installResource();
};

#endif
