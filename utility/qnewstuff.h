/***************************************************************************
                          qnewstuff.h  -  description
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

 /* This file and the corresponding .cpp file contains the general framework to download
 resources from a server and the resource specific classes which inherit from it */

#ifndef QNEWSTUFF_H
#define QNEWSTUFF_H

//qt includes
#include <qobject.h>

//kde includes
#include <knewstuff/knewstuff.h>

class KTempDir;
/**
Makes possible downloading and installing resource files from the server

@author Andras Mantia
*/
class QNewStuff : public  QObject, public KNewStuff
{
  Q_OBJECT

public:
    QNewStuff(const QString &type,  QWidget *parentWidget=0);
    ~QNewStuff();

    /** Installs the downloaded resource */
    bool install( const QString &fileName );
    /** Creates a tarball to be uploaded */
    bool createUploadFile( const QString &fileName );

private slots:
    /** Checks the validity of the downloaded tarball and installs it*/
    void slotValidated(int result);

protected:
    virtual void installResource() = 0;

    KTempDir *m_tempDir;
    QString m_tarName;
};

class QNewDTEPStuff: public QNewStuff
{
   Q_OBJECT

public:
    QNewDTEPStuff(const QString &type,  QWidget *parentWidget=0)
      :QNewStuff(type, parentWidget){};
    ~QNewDTEPStuff() {};

private:
     virtual void installResource();
};

#endif
