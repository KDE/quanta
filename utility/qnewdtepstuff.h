/***************************************************************************
                          qnewdtepstuff.h  -  description
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

#ifndef QNEWDTEPSTUFF_H
#define QNEWDTEPSTUFF_H

//qt includes
#include <qobject.h>

//kde includes
#include <knewstuff/knewstuff.h>

class KTempDir;
/**
Makes possible downloading and installing new DTEP packages

@author Andras Mantia
*/
class QNewDTEPStuff : public  QObject, public KNewStuff
{
  Q_OBJECT

public:
    QNewDTEPStuff(const QString &type,  QWidget *parentWidget=0);
    ~QNewDTEPStuff();

    /** Installs the downloaded DTEP tarball */
    bool install( const QString &fileName );
    /** Creates a DTEP tarball to be uploaded */
    bool createUploadFile( const QString &fileName );

private slots:
    /** Checks the validity of the downloaded tarball and installs it*/
    void slotValidated(int result);

private:
    KTempDir *m_tempDir;
    QString m_tarName;
};

#endif
