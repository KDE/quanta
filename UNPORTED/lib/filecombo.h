/***************************************************************************
                          filecombo.h  -  description
                             -------------------
    begin                : Wed Sep 27 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon <pdima@users.sourceforge.net,yshurik@penguinpowered.com,sequitur@easystreet.com>
                           (C) 2002 Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef FILECOMBO_H
#define FILECOMBO_H

#include "quantaexport.h"

#include <qwidget.h>
#include <kurl.h>

/**
  *@author Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon & Andras Mantia
  */

class QComboBox;
class QPushButton;
class KUrl;

class KDEVQUANTA_EXPORT FileCombo : public QWidget  {
   Q_OBJECT
public:
  explicit FileCombo(const KUrl& a_baseURL, QWidget *parent=0);
  FileCombo(QWidget *parent=0);
  ~FileCombo();

  QString text() const;
  void setText( const QString &);
  /** The select URL dialogs shows this URL, and the returned value is relative to this */
  void setBaseURL(const KUrl& a_baseURL);
  /** The selected URL will be stored with absolute path if absolutePath is true */
  void setReturnAbsolutePath(bool absolutePath);

public slots:
  void slotFileSelect();
  void slotComboActivated(const QString&);

signals:
  void activated(const QString&);

protected:
  KUrl baseURL;
  bool m_absolutePath;

  QComboBox *combo;
  QPushButton *button;

};

#endif
