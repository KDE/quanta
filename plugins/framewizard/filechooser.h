/***************************************************************************
                          filechooser.h  -  description
                             -------------------
    begin                : mar mar 25 2003
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

#ifndef FILECHOOSER_H
#define FILECHOOSER_H

#include <qwidget.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qfiledialog.h>

/**
  *@author gulmini luciano
  */

class FileChooser : public QWidget  {
   Q_OBJECT
   Q_ENUMS( Mode )
   Q_PROPERTY( Mode mode READ mode WRITE setMode )
   Q_PROPERTY( QString fileName READ fileName WRITE setFileName )
   public: 
	    FileChooser(QWidget *parent=0, const char *name=0);
	    ~FileChooser(){};
      enum Mode { File, Directory };
      QString fileName() const;
      Mode mode() const { return md; } 
  public slots:
      void setFileName( const QString &fn );
      void setMode( Mode m ){ md = m; }   

  signals:
      void fileNameChanged( const QString & );

  private slots:
      void chooseFile();
  private:
      QLineEdit *lineEdit;
      QPushButton *button;
      Mode md;
};

#endif
