/***************************************************************************
                          filechooser.cpp  -  description
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

#include "filechooser.h"
#include <qlayout.h>

FileChooser::FileChooser( QWidget *parent, const char *name )
	: QWidget( parent, name ), md( File )
{
    QHBoxLayout *layout = new QHBoxLayout( this );
    layout->setMargin( 0 );

    lineEdit = new QLineEdit( this, "filechooser_lineedit" );
    layout->addWidget( lineEdit );
    connect( lineEdit, SIGNAL( textChanged( const QString & ) ),this, SIGNAL( fileNameChanged( const QString & ) ) );

    button = new QPushButton( "...", this, "filechooser_button" );
    button->setFixedWidth( button->fontMetrics().width( " ... " ) );
    layout->addWidget( button );

    connect( button, SIGNAL( clicked() ),this, SLOT( chooseFile() ) );
    setFocusProxy( lineEdit );
}

void FileChooser::setFileName( const QString &fn )
{
    lineEdit->setText( fn );
}

QString FileChooser::fileName() const
{
    return lineEdit->text();
}

void FileChooser::chooseFile()
{
    QString fn;
    if ( mode() == File )
	    fn = QFileDialog::getOpenFileName( lineEdit->text(), QString::null, this );
    else
	    fn = QFileDialog::getExistingDirectory( lineEdit->text(),this );

    if ( !fn.isEmpty() ) {
	    lineEdit->setText( fn );
	    emit fileNameChanged( fn );
    }
}

#include "filechooser.moc"
