/* This file is part of the KDE libraries
    Copyright (C) 1997 Martin Jones (mjones@kde.org)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

*/
//-----------------------------------------------------------------------------
// KDE color selection dialog.
//
// 1999-09-27 Espen Sand <espensa@online.no>
// KColorDialog is now subclassed from KDialog. I have also extended
// KColorDialog::getColor() so that in contains a parent argument. This
// improves centering capability.
//
// layout management added Oct 1997 by Mario Weilguni
// <mweilguni@sime.com>
//

#include "colorcombo.h"

#include <stdio.h>
#include <stdlib.h>

#include <qdrawutil.h>
#include <QEvent>
#include <QFile>
#include <QImage>
#include <QLabel>
#include <QLayout>
#include <klineedit.h>
#include <QValidator>
#include <QPainter>
#include <QPushButton>
#include <QTimer>

#include <kcolordialog.h>
#include <kconfig.h>
#include <kglobal.h>
#include <kglobalsettings.h>
#include <kiconloader.h>
#include <k3listbox.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kseparator.h>

#include "colorcombo.moc"

// This is repeated from the KColorDlg, but I didn't
// want to make it public BL.
// We define it out when compiling with --enable-final in which case
// we use the version defined in KColorDlg

#define STANDARD_PAL_SIZE 17

ColorCombo::ColorCombo( QWidget *parent, const char *name )
  : QComboBox( parent )
{
  setObjectName(name);
  customColor.setRgb( 255, 255, 255 );
  internalcolor.setRgb( 255, 255, 255 );

  hascolor = false;

  createStandardPalette();

  addColors();

  connect( this, SIGNAL( activated(int) ), SLOT( slotActivated(int) ) );
  connect( this, SIGNAL( highlighted(int) ), SLOT( slotHighlighted(int) ) );
}

ColorCombo::~ColorCombo()
{
  delete []standardPalette;
  delete []standardPaletteNames;
}

/**
   Sets the current color
 */
void ColorCombo::setColor( const QColor &col )
{
  internalcolor = col;
  hascolor = true;

  addColors();
}


/**
   Returns the currently selected color
 */
QColor ColorCombo::color() const {
  return internalcolor;
}


bool ColorCombo::hasColor() const{
  return hascolor;
}


void ColorCombo::setColorName( const QString &color )
{

  QColor c(color);

  if ( c.isValid() && !color.isEmpty() ) {
    setColor( c );
  }
  else {
    hascolor = false;
    addColors();
  }

}

QString ColorCombo::colorName()
{
  if ( hascolor ) {
    int i;

    for ( i = 0; i < STANDARD_PAL_SIZE; i++ )
      if ( standardPalette[i] == internalcolor )
          return standardPaletteNames[i];

    return internalcolor.name();

  }
  else
    return "";
}


void ColorCombo::resizeEvent( QResizeEvent *re )
{
  QComboBox::resizeEvent( re );

  addColors();
}

void ColorCombo::slotActivated( int index )
{
  hascolor = ( index != 0);

  if ( index == 1 )
  {
    if ( KColorDialog::getColor( customColor ) == QDialog::Accepted )
    {
      QRect rect( 0, 0, width(), 20 );
      QPixmap pixmap( rect.width(), rect.height() );
      QPainter painter;
      QPen pen;

      if ( qGray( customColor.rgb() ) < 128 )
        pen.setColor( Qt::white );
      else
        pen.setColor( Qt::black );

      painter.begin( &pixmap );
      QBrush brush( customColor );
      painter.fillRect( rect, brush );
      painter.setPen( pen );
      painter.drawText( 2, painter.fontMetrics().height(),
            i18n("Custom...") );
      painter.end();

      setItemIcon( 1, QIcon(pixmap) );
      pixmap.detach();
    }

    internalcolor = customColor;
  }
  else
    if ( index > 1 )
      internalcolor = standardPalette[ index - 2 ];

  emit activated( internalcolor );
}

void ColorCombo::slotHighlighted( int index )
{
  if ( index < 2 )
    internalcolor = customColor;
  else
    internalcolor = standardPalette[ index - 2 ];

  emit highlighted( internalcolor );
}

void ColorCombo::addColors()
{
  QRect rect( 0, 0, width(), 20 );
  QPixmap pixmap( rect.width(), rect.height() );
  QPainter painter;
  QPen pen;
  int i;

  clear();

  for ( i = 0; i < STANDARD_PAL_SIZE; i++ )
    if ( standardPalette[i] == internalcolor ) break;

  if ( i == STANDARD_PAL_SIZE )
    customColor = internalcolor;

  addItem( i18n("None") );

  if ( qGray( customColor.rgb() ) < 128 )
    pen.setColor( Qt::white );
  else
    pen.setColor( Qt::black );

  painter.begin( &pixmap );
  QBrush brush( customColor );
  painter.fillRect( rect, brush );
  painter.setPen( pen );

  painter.drawText( 2, painter.fontMetrics().height(), i18n("Custom...") );
  painter.end();

  insertItem( -1, QIcon(pixmap), "" );
  pixmap.detach();

  bool findColor = false;

  for ( i = 0; i < STANDARD_PAL_SIZE; i++ )
  {
    painter.begin( &pixmap );
    QBrush brush( standardPalette[i] );
    painter.fillRect( rect, brush );
    painter.end();

    insertItem( -1, QIcon(pixmap), "" );
    pixmap.detach();

    if ( standardPalette[i] == internalcolor ) {
      setCurrentIndex( i + 2 );
      findColor = true;
    }
  }

  if ( !findColor )
    setCurrentIndex(1);



  if ( !hascolor )
    setCurrentIndex(0);

}

void ColorCombo::createStandardPalette()
{

    standardPalette = new QColor [STANDARD_PAL_SIZE];

    int i = 0;

    standardPalette[i++] = Qt::red;
    standardPalette[i++] = Qt::green;
    standardPalette[i++] = Qt::blue;
    standardPalette[i++] = Qt::cyan;
    standardPalette[i++] = Qt::magenta;
    standardPalette[i++] = Qt::yellow;
    standardPalette[i++] = Qt::darkRed;
    standardPalette[i++] = Qt::darkGreen;
    standardPalette[i++] = Qt::darkBlue;
    standardPalette[i++] = Qt::darkCyan;
    standardPalette[i++] = Qt::darkMagenta;
    standardPalette[i++] = Qt::darkYellow;
    standardPalette[i++] = Qt::white;
    standardPalette[i++] = Qt::lightGray;
    standardPalette[i++] = Qt::gray;
    standardPalette[i++] = Qt::darkGray;
    standardPalette[i++] = Qt::black;


    standardPaletteNames = new QString [STANDARD_PAL_SIZE];

    i = 0;

    standardPaletteNames[i++] = "red";
    standardPaletteNames[i++] = "green";
    standardPaletteNames[i++] = "blue";
    standardPaletteNames[i++] = "cyan";
    standardPaletteNames[i++] = "magenta";
    standardPaletteNames[i++] = "yellow";
    standardPaletteNames[i++] = "darkRed";
    standardPaletteNames[i++] = "darkGreen";
    standardPaletteNames[i++] = "darkBlue";
    standardPaletteNames[i++] = "darkCyan";
    standardPaletteNames[i++] = "darkMagenta";
    standardPaletteNames[i++] = "darkYellow";
    standardPaletteNames[i++] = "white";
    standardPaletteNames[i++] = "lightGray";
    standardPaletteNames[i++] = "gray";
    standardPaletteNames[i++] = "darkGray";
    standardPaletteNames[i++] = "black";
}
