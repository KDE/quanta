/***************************************************************************
                          filecombo.h  -  description
                             -------------------
    begin                : Wed Sep 27 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
    email                : pdima@users.sourceforge.net,yshurik@penguinpowered.com,sequitur@easystreet.com
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

#include <qwidget.h>

/**
  *@author Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
  */

class QComboBox;
class QPushButton;

class FileCombo : public QWidget  {
   Q_OBJECT
public: 
	FileCombo(QString basePath, QWidget *parent=0, const char *name=0);
	FileCombo(QWidget *parent=0, const char *name=0);
	~FileCombo();
	
	QString text();
	void setText( QString );
  /** No descriptions */
  void setBasePath(const QString& p_basePath);
  /** No descriptions */
  void setReturnAbsolutePath(bool absolutPath);

public slots:
  void slotFileSelect();
		
protected:
  QString     basePath;
  QComboBox   *combo;
private: // Private attributes
  /**  */
  bool m_absolutePath;
  QPushButton *button;

};

#endif
