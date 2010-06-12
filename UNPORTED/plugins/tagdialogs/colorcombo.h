/* This file is part of the KDE libraries
    Copyright (C) 1997 Martin Jones (mjones@kde.org)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

*/
//-----------------------------------------------------------------------------
// KDE color selection combo box

// layout management added Oct 1997 by Mario Weilguni
// <mweilguni@sime.com>


#ifndef COLORCOMBO_H
#define COLORCOMBO_H

#include <QComboBox>
#include <kdialog.h>
#include <qframe.h>
#include <QPixmap>
#include <kcolordialog.h>


/**
 * Combobox for colours.
 */
class ColorCombo : public QComboBox
{
  Q_OBJECT
public:
  explicit ColorCombo( QWidget *parent = 0L, const char *name = 0L );
  ~ColorCombo();

  void setColor( const QColor &col );
  void setColorName(const QString &color);
  QString colorName();
  /**
   * Retrieve the currently selected color.
   **/
  QColor color() const;

  bool hasColor() const;

public slots:
  void slotActivated( int index );
  void slotHighlighted( int index );

signals:
  void activated( const QColor &col );
  void highlighted( const QColor &col );

protected:
        /**
         * @reimplemented
         */
  virtual void resizeEvent( QResizeEvent *re );

private:
  void createStandardPalette();
  void addColors();
  QColor customColor;
  QColor internalcolor;

  bool hascolor;

  QColor *standardPalette;
  QString *standardPaletteNames;


};

#endif  // COLORCOMBO_H
